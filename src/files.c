/* Open and close files for bison,
   Copyright 1984, 1986, 1989, 1992, 2000, 2001, 2002
   Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */


#include "system.h"
#include "getargs.h"
#include "files.h"
#include "gram.h"
#include "error.h"
#include "complain.h"

/* From basename.c.  Almost a lie, as it returns a char *. */
const char *base_name PARAMS ((char const *name));
/* From xstrndup.c.  */
char *xstrndup PARAMS ((const char *s, size_t n));

FILE *finput = NULL;

struct obstack action_obstack;
struct obstack attrs_obstack;
struct obstack table_obstack;
struct obstack defines_obstack;
struct obstack guard_obstack;
struct obstack output_obstack;

char *spec_outfile = NULL;	/* for -o. */
char *spec_file_prefix = NULL;	/* for -b. */
char *spec_name_prefix = NULL;	/* for -p. */
char *spec_verbose_file = NULL;   /* for --verbose. */
char *spec_graph_file = NULL;   /* for -g. */
char *spec_defines_file = NULL; /* for --defines. */

char *infile = NULL;
char *attrsfile = NULL;

static char *full_base_name = NULL;
static char *short_base_name = NULL;

/* C source file extension (the parser source).  */
const char *src_extension = NULL;
/* Header file extension (if option ``-d'' is specified).  */
const char *header_extension = NULL;


/*--------------------------.
| Is SUFFIX ending STRING?  |
`--------------------------*/

int
strsuffix (const char *string, const char *suffix)
{
  size_t string_len = strlen (string);
  size_t suffix_len = strlen (suffix);
  if (suffix_len <= string_len)
    return !strcmp (string + string_len - suffix_len, suffix);
  else
    return 0;
}


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STRING1, and STRING2.                                            |
`-----------------------------------------------------------------*/

static char *
stringappend (const char *string1, const char *string2)
{
  size_t len = strlen (string1) + strlen (string2);
  char *res = XMALLOC (char, len + 1);
  char *cp;
  cp = stpcpy (res, string1);
  cp = stpcpy (cp, string2);
  return res;
}


/*-----------------------------------------------------------------.
| Computes the macro name used to avoid double inclusion in the    |
| header of the parser and store it in header_macro_name.  Be sure |
| to produce valid CPP names (don't start with digit, remain       |
| alphanumerical + underscore).                                    |
`-----------------------------------------------------------------*/

static char *
compute_header_macro (void)
{
  const char *prefix = "BISON_";
  char *macro_name, *cp;

  if (spec_defines_file)
    {
      macro_name = XMALLOC (char,
			    strlen (prefix) +
			    strlen (spec_defines_file) + 1);
      cp = stpcpy (macro_name, prefix);
      cp = stpcpy (cp, spec_defines_file);
    }
  else
    {
      macro_name = XMALLOC (char,
			    strlen (prefix) +
			    strlen (full_base_name) +
			    strlen (header_extension) + 1);
      cp = stpcpy (macro_name, prefix);
      cp = stpcpy (cp, full_base_name);
      cp = stpcpy (cp, header_extension);
    }

  for (cp = macro_name; *cp; ++cp)
    if (islower (*cp))
      *cp = toupper (*cp);
    else if (!isalnum (*cp))
      *cp = '_';

  return macro_name;
}


/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (const char *name, const char *mode)
{
  FILE *ptr;

  ptr = fopen (name, mode);
  if (!ptr)
    error (2, errno, _("cannot open file `%s'"), name);

  return ptr;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

int
xfclose (FILE *ptr)
{
  int result;

  if (ptr == NULL)
    return 0;

  result = fclose (ptr);
  if (result == EOF)
    error (2, errno, _("cannot close file"));

  return result;
}

/*--------------------------------------------------.
| Save the content of the obstack OBS in FILENAME.  |
`--------------------------------------------------*/

static void
obstack_save (struct obstack *obs, const char *filename)
{
  FILE *out = xfopen (filename, "w");
  size_t size = obstack_object_size (obs);
  fwrite (obstack_finish (obs), 1, size, out);
  xfclose (out);
}

/*---------------------------------------------------------------------.
| Output double inclusion protection macros and saves defines_obstack  |
`---------------------------------------------------------------------*/

static void
defines_obstack_save (const char *filename)
{
  FILE *out = xfopen (filename, "w");
  size_t size = obstack_object_size (&defines_obstack);
  char *macro_name = compute_header_macro ();

  fprintf (out, "#ifndef %s\n", macro_name);
  fprintf (out, "# define %s\n\n", macro_name);
  fwrite (obstack_finish (&defines_obstack), 1, size, out);
  fprintf (out, "\n#endif /* not %s */\n", macro_name);

  free (macro_name);
  xfclose (out);
}

/*------------------------------------------------------------------.
| Return the path to the skeleton which locaction might be given in |
| ENVVAR, otherwise return SKELETON_NAME.                           |
`------------------------------------------------------------------*/

const char *
skeleton_find (const char *envvar, const char *skeleton_name)
{
  const char *res = getenv (envvar);

#if defined (MSDOS) || defined (_WIN32)
  const char *cp = getenv ("INIT");
  if (!res)
    {
      /* Skeleton file name without path */
      const char *skel_name = strrchr (skeleton_name, '/');
      if (!skel_name)
        skel_name = strrchr (skeleton_name, '\\');
      if (!skel_name)
        skel_name = skeleton_name;
      else
        ++skel_name;

      /* File doesn't exist in current directory; try in INIT directory.  */
      if (cp)
	{
	  res = XMALLOC (char, strlen (cp) + strlen (skel_name) + 2);
	  sprintf (res, "%s%c%s", cp, '\\', skel_name);
	}
      else if (access (skel_name, 4) == 0) /* Look in current dir. */
        res = skel_name;
      else
	{
	  /* Look in program locations dir. */
	  extern char *program_name;
	  cp = strrchr(program_name, '\\');
	  if (!cp)
	    return skeleton_name;
	  else
	    ++cp;
	  res = XMALLOC (char, cp - program_name + strlen (skel_name) + 1);
	  strncpy (res, program_name, cp - program_name);
	  strcpy (res + (cp - program_name), skel_name);
	}
    }
#endif /* defined (MSDOS) || defined (_WIN32) */
  if (!res)
    res = skeleton_name;

  return res;
}


/*----------------------------------------------------------------.
| Compute BASE_NAME, SHORT_BASE_NAME and output files extensions. |
`----------------------------------------------------------------*/

/* Replace all characters FROM by TO in the string IN.
   and returns a new allocated string.  */
static char *
tr (const char *in, char from, char to)
{
  char *temp;
  char *out;

  out = XMALLOC (char, strlen (in) + 1);

  for (temp = out; *in; in++, out++)
    if (*in == from)
      *out = to;
    else
      *out = *in;
  *out = 0;
  return (temp);
}

/* Computes extensions from the grammar file extension. */
static void
compute_exts_from_gf (const char *ext)
{
  src_extension = tr (ext, 'y', 'c');
  src_extension = tr (src_extension, 'Y', 'C');
  header_extension = tr (ext, 'y', 'h');
  header_extension = tr (header_extension, 'Y', 'H');
}

/* Computes extensions from the given c source file extension. */
static void
compute_exts_from_src (const char *ext)
{
  /* We use this function when the user specifies `-o' or `--output',
     so the extenions must be computed unconditionally from the file name
     given by this option.  */
  src_extension = xstrdup (ext);
  header_extension = tr (ext, 'c', 'h');
  header_extension = tr (header_extension, 'C', 'H');
}


/* Decompose FILENAME in four parts: *BASE, *TAB, and *EXT, the fourth
   part, (the directory) is ranging from FILENAME to the char before
   *BASE, so we don't need an additional parameter.

   *EXT points to the last period in the basename, or NULL if none.

   If there is no *EXT, *TAB is NULL.  Otherwise, *TAB points to
   `.tab' or `_tab' if present right before *EXT, or is NULL. *TAB
   cannot be equal to *BASE.

   None are allocated, they are simply pointers to parts of FILENAME.
   Examples:

   '/tmp/foo.tab.c' -> *BASE = 'foo.tab.c', *TAB = '.tab.c', *EXT =
   '.c'

   'foo.c' -> *BASE = 'foo.c', *TAB = NULL, *EXT = '.c'

   'tab.c' -> *BASE = 'tab.c', *TAB = NULL, *EXT = '.c'

   '.tab.c' -> *BASE = '.tab.c', *TAB = NULL, *EXT = '.c'

   'foo.tab' -> *BASE = 'foo.tab', *TAB = NULL, *EXT = '.tab'

   'foo_tab' -> *BASE = 'foo_tab', *TAB = NULL, *EXT = NULL

   'foo' -> *BASE = 'foo', *TAB = NULL, *EXT = NULL.  */

static void
filename_split (const char *filename,
		const char **base, const char **tab, const char **ext)
{
  *base = base_name (filename);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = NULL;

  /* If there is an exentension, check if there is a `.tab' part right
     before.  */
  if (*ext
      && (*ext - *base) > (int) strlen (".tab")
      && (!strncmp (*ext - strlen (".tab"), ".tab", strlen (".tab"))
	  || !strncmp (*ext - strlen ("_tab"), "_tab", strlen ("_tab"))))
    *tab = *ext - strlen (".tab");
}


/* FIXME: Should use xstrndup. */

static void
compute_base_names (void)
{
  const char *base, *tab, *ext;

  /* If --output=foo.c was specified (SPEC_OUTFILE == foo.c),
     BASE_NAME and SHORT_BASE_NAME are `foo'.

     If --output=foo.tab.c was specified, BASE_NAME is `foo.tab' and
     SHORT_BASE_NAME is `foo'.

     The precise -o name will be used for FTABLE.  For other output
     files, remove the ".c" or ".tab.c" suffix.  */
  if (spec_outfile)
    {
      filename_split (spec_outfile, &base, &tab, &ext);

      /* The full base name goes up the EXT, excluding it. */
      full_base_name =
 	xstrndup (spec_outfile,
 		  (strlen (spec_outfile) - (ext ? strlen (ext) : 0)));
      /* The short base name goes up to TAB, excluding it.  */
      short_base_name =
 	xstrndup (spec_outfile,
 		  (strlen (spec_outfile)
 		   - (tab ? strlen (tab) : (ext ? strlen (ext) : 0))));

      if (ext)
 	compute_exts_from_src (ext);
    }

  /* If --file-prefix=foo was specified, FULL_BASE_NAME = `foo.tab'
     and SHORT_BASE_NAME = `foo'.

     Construct names from it.  */
  else
    {
      if (spec_file_prefix)
 	{
 	  /* If --file-prefix=foo was specified, SHORT_BASE_NAME =
 	     `foo'.  */
 	  short_base_name = xstrdup (spec_file_prefix);
 	}
      else if (yacc_flag)
 	{
 	  /* If --yacc, then the output is `y.tab.c'. */
 	  short_base_name = xstrdup ("y");
 	}
      else
 	{
 	  /* Otherwise, the short base name is computed from the input
 	     grammar: `foo.yy' => `foo'.  */
 	  filename_split (infile, &base, &tab, &ext);
 	  short_base_name =
 	    xstrndup (infile,
 		      (strlen (infile) - (ext ? strlen (ext) : 0)));
 	}

      /* In these cases, always append `.tab'. */
      full_base_name = XMALLOC (char,
 				strlen (short_base_name)
 				+ strlen (EXT_TAB) + 1);
      stpcpy (stpcpy (full_base_name, short_base_name), EXT_TAB);

      /* Computes the extensions from the grammar file name.  */
      filename_split (infile, &base, &tab, &ext);
      
      if (ext && !yacc_flag)
	compute_exts_from_gf (ext);
    }
}


/*-------------------------------------------------------.
| Close the open files, compute the output files names.  |
`-------------------------------------------------------*/

void
compute_output_file_names (void)
{
  compute_base_names ();

  /* If not yet done. */
  if (!src_extension)
    src_extension = ".c";
  if (!header_extension)
    header_extension = ".h";

  /* It the defines filename if not given, we create it.  */
  if (!spec_defines_file)
    spec_defines_file = stringappend (full_base_name, header_extension);

  /* It the graph filename if not given, we create it.  */
  if (!spec_graph_file)
    spec_graph_file = stringappend (short_base_name, ".vcg");

  spec_verbose_file = stringappend (short_base_name, EXT_OUTPUT);

  attrsfile = stringappend (short_base_name, EXT_STYPE_H);
#ifndef MSDOS
  attrsfile = stringappend (attrsfile, header_extension);
#endif /* MSDOS */

}

/*-----------------------------------------------------------------.
| Open the input file.  Look for the skeletons.  Find the names of |
| the output files.  Prepare the obstacks.                         |
`-----------------------------------------------------------------*/

void
open_files (void)
{
  finput = xfopen (infile, "r");

  /* Initialize the obstacks. */
  obstack_init (&action_obstack);
  obstack_init (&attrs_obstack);
  obstack_init (&table_obstack);
  obstack_init (&defines_obstack);
  obstack_init (&guard_obstack);
  obstack_init (&output_obstack);
}



/*-----------------------.
| Close the open file..  |
`-----------------------*/

void
close_files (void)
{
  xfclose (finput);
}

/*---------------------------.
| Produce the output files.  |
`---------------------------*/

void
output_files (void)
{
  /* Output the main file.  */
  if (spec_outfile)
    obstack_save (&table_obstack, spec_outfile);
  else
    obstack_save (&table_obstack,
		  stringappend (full_base_name, src_extension));
  obstack_free (&table_obstack, NULL);

  /* Output the header file if wanted. */
  if (defines_flag)
    defines_obstack_save (spec_defines_file);
  obstack_free (&defines_obstack, NULL);

  /* If we output only the table, dump the actions in ACTFILE. */
  if (no_parser_flag)
    obstack_save (&action_obstack, stringappend (short_base_name, ".act"));
  obstack_free (&action_obstack, NULL);

  /* If we produced a semantic parser ATTRS_OBSTACK must be dumped
     into its own file, ATTTRSFILE.  */
  if (semantic_parser)
    {
      char *temp_name;

      obstack_save (&attrs_obstack, attrsfile);
      obstack_free (&attrs_obstack, NULL);
      temp_name = stringappend (short_base_name, EXT_GUARD_C);
#ifndef MSDOS
      temp_name = stringappend (temp_name, src_extension);
#endif /* MSDOS */
      obstack_save (&guard_obstack, temp_name);
      obstack_free (&guard_obstack, NULL);
    }
}
