/* Open and close files for bison,
   Copyright 1984, 1986, 1989, 1992, 2000, 2001 Free Software Foundation, Inc.

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
#include "xalloc.h"
#include "gram.h"
#include "complain.h"

FILE *finput = NULL;

struct obstack action_obstack;
struct obstack attrs_obstack;
struct obstack table_obstack;
struct obstack defines_obstack;
struct obstack guard_obstack;
struct obstack output_obstack;
struct obstack graph_obstack;

char *spec_outfile = NULL;	/* for -o. */
char *spec_file_prefix = NULL;	/* for -b. */
char *spec_name_prefix = NULL;	/* for -p. */

char *infile = NULL;
char *attrsfile = NULL;

static char *base_name = NULL;
static char *short_base_name = NULL;

/* C source file extension (the parser source).  */
const char *src_extension = NULL;
/* Header file extension (if option ``-d'' is specified).  */
const char *header_extension = NULL;


/*--------------------------.
| Is SUFFIX ending STRING?  |
`--------------------------*/

static int
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


/*---------------------------------------------------------------.
|  Computes the macro name used to avoid double inclusion in the |
|  header of the parser and store it in header_macro_name.       |
`---------------------------------------------------------------*/

static char *
compute_header_macro (void)
{
  int ite;
  char *macro_name;

  macro_name = XMALLOC (char,
			strlen (base_name) +
			strlen (header_extension) + 1);

  stpcpy (macro_name, base_name);
  strcat (macro_name, header_extension);

  for (ite = 0; macro_name[ite]; ite++)
    if (macro_name[ite] == '.')
      macro_name[ite] = '_';
    else
      {
	if (islower (macro_name[ite]))
	  macro_name[ite] -= ('a' - 'A');
      }
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

#ifdef MSDOS
  const char *cp;

  /* File doesn't exist in current directory; try in INIT directory.  */
  if (!res && (cp = getenv ("INIT")))
    {
      res = XMALLOC (char, strlen (cp) + strlen (skeleton_name) + 2);
      sprintf (res, "%s%c%s", cp, '/', skeleton_name);
    }
#endif /* !MSDOS */

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

/* Gets the extension index in FILENAME. Returns 0 if fails to
   find an extension.  */
static int
get_extension_index (const char *filename)
{
  int len;

  len = strlen (filename);

  if (filename[len-- - 1] == '.')
    return (0);

  while ((len > 0) && (filename[len - 1] != '.'))
    if (filename[len - 1] == '/')
      return (0);
    else
      len--;

  return (len - 1);
}

/* Computes extensions from the grammar file extension. */
static void
compute_exts_from_gf (const char *ext)
{
  /* Checks if SRC_EXTENSION is NULL. In the other case, %source_extension
     was specified in the grammar file.  */
  if (src_extension == NULL)
    {
      src_extension = tr (ext, 'y', 'c');
      src_extension = tr (src_extension, 'Y', 'C');
    }
  /* Checks if HEADER_EXTENSION is NULL. In the other case,
     %header_extension was specified in the grammar file.  */
  if (header_extension == NULL)
    {
      header_extension = tr (ext, 'y', 'h');
      header_extension = tr (header_extension, 'Y', 'H');
    }
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

/* FIXME: Should use xstrndup. */

static void
compute_base_names (void)
{
  size_t base_length;
  size_t short_base_length;
  size_t ext_index;

  /* If --output=foo.c was specified (SPEC_OUTFILE == foo.c),
     BASE_NAME and SHORT_BASE_NAME are `foo'.

     If --output=foo.tab.c was specified, BASE_NAME is `foo.tab' and
     SHORT_BASE_NAME is `foo'.

     The precise -o name will be used for FTABLE.  For other output
     files, remove the ".c" or ".tab.c" suffix.  */
  if (spec_outfile)
    {
#ifdef MSDOS
      strlwr (spec_outfile);
#endif /* MSDOS */
      /* BASE_LENGTH includes ".tab" but not ".c".  */
      base_length = strlen (spec_outfile);

      ext_index = get_extension_index (spec_outfile);
      /* If the initial segment of extension contains 'c' or a 'C', I assume
         that it is a C or C++ source file.  */
      if (ext_index)
	ext_index =
	  (strspn (spec_outfile + ext_index + 1, "cC")) ? ext_index : 0;
      if (ext_index)
	{
	  base_length -= strlen (spec_outfile + ext_index);
	  compute_exts_from_src (spec_outfile + ext_index);
	}

      base_name = strndup (spec_outfile, base_length);
      /* SHORT_BASE_LENGTH includes neither ".tab" nor ".c".  */
      short_base_length = base_length;
      if (strsuffix (base_name, ".tab") || strsuffix (base_name, "_tab"))
	short_base_length -= 4;
      short_base_name = strndup (spec_outfile, short_base_length);

      return;
    }

  /* If --file-prefix=foo was specified, BASE_NAME and SHORT_BASE_NAME
     are `foo'.

     Construct names from it.  */
  if (spec_file_prefix)
    {
#ifdef MSDOS
      strlwr (spec_file_prefix);
#endif /* MSDOS */
      short_base_name = xstrdup (spec_file_prefix);
      base_name = XMALLOC (char,
			   strlen (short_base_name) + strlen (EXT_TAB) + 1);
      stpcpy (stpcpy (base_name, short_base_name), EXT_TAB);

      /* Computes the extensions from the garmmar file name.  */
      ext_index = get_extension_index (infile);
      /* If the initial segment of extension contains a 'y' or a 'Y', I assume
         that it is a yacc or bison grammar file.  */
      if (ext_index)
	ext_index = (strspn (infile + ext_index + 1, "yY")) ? ext_index : 0;
      if (ext_index)
	compute_exts_from_gf (infile + ext_index);

      return;
    }

  /* If neither -o nor --file-prefix were specified, and the input
     file is foo.y, BASE_NAME is `foo.tab', and SHORT_BASE_NAME is
     `foo'.

     If --yacc is used, do as if the input file was `y.y'.  */
  {
    const char *name_base = yacc_flag ? "y.y" : infile;

    /* BASE_LENGTH gets length of BASE_NAME, sans ".y" suffix if any.  */

    base_length = strlen (name_base);

    ext_index = get_extension_index (name_base);
    /* If the initial segment of extension contains a 'y' or a 'Y', I assume
       that it is a yacc or bison grammar file.  */
    if (ext_index)
      ext_index = (strspn (name_base + ext_index + 1, "yY")) ? ext_index : 0;
    if (ext_index)
      {
	base_length -= strlen (name_base + ext_index);
	compute_exts_from_gf (name_base + ext_index);
      }

    short_base_length = base_length;
    short_base_name = strndup (name_base, short_base_length);

    base_name = XMALLOC (char,
			 strlen (short_base_name) + strlen (EXT_TAB) + 1);
    stpcpy (stpcpy (base_name, short_base_name), EXT_TAB);

    return;
  }
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
  obstack_init (&graph_obstack);
}



/*-----------------------------------------------------.
| Close the open files, produce all the output files.  |
`-----------------------------------------------------*/

void
output_files (void)
{
  xfclose (finput);

  compute_base_names ();

  /* Set default extensions */
  if (!src_extension)
    src_extension = ".c";
  if (!header_extension)
    header_extension = ".h";

  attrsfile = stringappend (short_base_name, EXT_STYPE_H);
#ifndef MSDOS
  stringappend (attrsfile, header_extension);
#endif /* MSDOS */

  /* Output the main file.  */
  if (spec_outfile)
    obstack_save (&table_obstack, spec_outfile);
  else
    obstack_save (&table_obstack, stringappend (base_name, src_extension));

  /* Output the header file if wanted. */
  if (defines_flag)
    defines_obstack_save (stringappend (base_name, header_extension));

  /* If we output only the table, dump the actions in ACTFILE. */
  if (no_parser_flag)
    obstack_save (&action_obstack, stringappend (short_base_name, ".act"));

  /* If we produced a semantic parser ATTRS_OBSTACK must be dumped
     into its own file, ATTTRSFILE.  */
  if (semantic_parser)
    {
      char *temp_name;

      obstack_save (&attrs_obstack, attrsfile);
      temp_name = stringappend (short_base_name, EXT_GUARD_C);
#ifndef MSDOS
      temp_name = stringappend (temp_name, src_extension);
#endif /* MSDOS */
      obstack_save (&guard_obstack, temp_name);
    }

  if (verbose_flag)
    /* We used to use just .out if spec_name_prefix (-p) was used, but
       that conflicts with Posix.  */
    obstack_save (&output_obstack,
		  stringappend (short_base_name, EXT_OUTPUT));

  if (graph_flag)
    obstack_save (&graph_obstack, stringappend (short_base_name, ".vcg"));
}
