/* Open and close files for bison,
   Copyright 1984, 1986, 1989, 1992, 2000 Free Software Foundation, Inc.

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

#if defined (VMS) & !defined (__VMS_POSIX)
# ifndef BISON_SIMPLE
#  define BISON_SIMPLE "GNU_BISON:[000000]BISON.SIMPLE"
# endif
# ifndef BISON_HAIRY
#  define BISON_HARIRY "GNU_BISON:[000000]BISON.HAIRY"
# endif
#endif

#if defined (_MSC_VER)
# ifndef BISON_SIMPLE
#  define BISON_SIMPLE "c:/usr/local/lib/bison.simple"
# endif
# ifndef BISON_HAIRY
#  define BISON_HAIRY "c:/usr/local/lib/bison.hairy"
# endif
#endif

#include "getargs.h"
#include "files.h"
#include "xalloc.h"
#include "gram.h"
#include "complain.h"

FILE *finput = NULL;
FILE *foutput = NULL;
FILE *fguard = NULL;
FILE *fparser = NULL;

struct obstack action_obstack;
struct obstack attrs_obstack;
struct obstack table_obstack;
struct obstack defines_obstack;

/* File name specified with -o for the output file, or 0 if no -o.  */
char *spec_outfile;

char *infile;
char *attrsfile;

static char *outfile;
static char *defsfile;
static char *tabfile;
static char *guardfile;
static char *actfile;

extern char *getenv ();

extern char *program_name;


static char *
stringappend (const char *string1, int end1, const char *string2)
{
  register char *ostring;
  register char *cp;
  register const char *cp1;
  register int i;

  cp1 = string2;
  i = 0;
  while (*cp1++)
    i++;

  ostring = XCALLOC (char, i + end1 + 1);

  cp = ostring;
  cp1 = string1;
  for (i = 0; i < end1; i++)
    *cp++ = *cp1++;

  cp1 = string2;
  while ((*cp++ = *cp1++))
    ;

  return ostring;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

static FILE *
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

static int
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


static const char *
skeleton_find (const char *envvar, const char *skeleton)
{
  const char *res = getenv (envvar);

#ifdef MSDOS
  const char *cp;

  /* File doesn't exist in current directory; try in INIT directory.  */
  if (!res && (cp = getenv ("INIT")))
    {
      res = XMALLOC (char, strlen (cp) + strlen (skeleton) + 2);
      sprintf (res, "%s%c%s", cp, '/', skeleton);
    }
#endif /* !MSDOS */

  if (!res)
    res = skeleton;

  return res;
}


/*-----------------------------------------------------------------.
| Open the input file.  Look for the skeletons.  Find the names of |
| the output files.  Prepare the obstacks.                         |
`-----------------------------------------------------------------*/

void
open_files (void)
{
  char *name_base;
#ifdef MSDOS
  register char *cp;
#endif
  int base_length;
  int short_base_length;

#if defined (VMS) & !defined (__VMS_POSIX)
  const char *tmp_base = "sys$scratch:b_";
#else
  const char *tmp_base = "/tmp/b.";
#endif
  int tmp_len;

#ifdef MSDOS
  tmp_base = getenv ("TMP");
  if (tmp_base == 0)
    tmp_base = "";
  strlwr (infile);
#endif /* MSDOS */

#if (defined(_WIN32) && !defined(__CYGWIN32__))
  tmp_base = getenv ("TEMP");	/* Windows95 defines this ... */
  if (tmp_base == 0)
    tmp_base = getenv ("Temp");	/* ... while NT prefers this */
  if (tmp_base == 0)
    tmp_base = "";
  strlwr (infile);
#endif /* _WIN32 && !__CYGWIN32__ */

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__EMX__))
  {
    char *tmp_ptr = getenv ("TMPDIR");

    if (tmp_ptr != 0)
      tmp_base = stringappend (tmp_ptr, strlen (tmp_ptr), "/b.");
  }
#endif /* unix || __unix || __unix__ */

  tmp_len = strlen (tmp_base);

  if (spec_outfile)
    {
      /* -o was specified.  The precise -o name will be used for FTABLE.
         For other output files, remove the ".c" or ".tab.c" suffix.  */
      name_base = spec_outfile;
#ifdef MSDOS
      strlwr (name_base);
#endif /* MSDOS */
      /* BASE_LENGTH includes ".tab" but not ".c".  */
      base_length = strlen (name_base);
      if (!strcmp (name_base + base_length - 2, ".c"))
	base_length -= 2;
      /* SHORT_BASE_LENGTH includes neither ".tab" nor ".c".  */
      short_base_length = base_length;
      if (!strncmp (name_base + short_base_length - 4, ".tab", 4))
	short_base_length -= 4;
      else if (!strncmp (name_base + short_base_length - 4, "_tab", 4))
	short_base_length -= 4;
    }
  else if (spec_file_prefix)
    {
      /* -b was specified.  Construct names from it.  */
      /* SHORT_BASE_LENGTH includes neither ".tab" nor ".c".  */
      short_base_length = strlen (spec_file_prefix);
      /* Count room for `.tab'.  */
      base_length = short_base_length + 4;
      name_base = XMALLOC (char, base_length + 1);
      /* Append `.tab'.  */
      strcpy (name_base, spec_file_prefix);
#ifdef VMS
      strcat (name_base, "_tab");
#else
      strcat (name_base, ".tab");
#endif
#ifdef MSDOS
      strlwr (name_base);
#endif /* MSDOS */
    }
  else
    {
      /* -o was not specified; compute output file name from input
         or use y.tab.c, etc., if -y was specified.  */

      static char FIXED_NAME_BASE[] = "y.y";

      name_base = yacc_flag ? FIXED_NAME_BASE : infile;

      /* BASE_LENGTH gets length of NAME_BASE, sans ".y" suffix if any.  */

      base_length = strlen (name_base);
      if (!strcmp (name_base + base_length - 2, ".y"))
	base_length -= 2;
      short_base_length = base_length;

#ifdef VMS
      name_base = stringappend (name_base, short_base_length, "_tab");
#else
#ifdef MSDOS
      name_base = stringappend (name_base, short_base_length, "_tab");
#else
      name_base = stringappend (name_base, short_base_length, ".tab");
#endif /* not MSDOS */
#endif
      base_length = short_base_length + 4;
    }

  finput = xfopen (infile, "r");

  if (!no_parser_flag)
    fparser = xfopen (skeleton_find ("BISON_SIMPLE", BISON_SIMPLE), "r");

  if (verbose_flag)
    {
#ifdef MSDOS
      outfile = stringappend (name_base, short_base_length, ".out");
#else
      /* We used to use just .out if spec_name_prefix (-p) was used,
         but that conflicts with Posix.  */
      outfile = stringappend (name_base, short_base_length, ".output");
#endif
      foutput = xfopen (outfile, "w");
    }

  if (no_parser_flag)
    {
      /* use permanent name for actions file */
      actfile = stringappend (name_base, short_base_length, ".act");
    }

  if (defines_flag)
    {
      defsfile = stringappend (name_base, base_length, ".h");
    }

  /* These are opened by `done' or `open_extra_files', if at all */
  if (spec_outfile)
    tabfile = spec_outfile;
  else
    tabfile = stringappend (name_base, base_length, ".c");

#ifdef VMS
  attrsfile = stringappend (name_base, short_base_length, "_stype.h");
  guardfile = stringappend (name_base, short_base_length, "_guard.c");
#else
#ifdef MSDOS
  attrsfile = stringappend (name_base, short_base_length, ".sth");
  guardfile = stringappend (name_base, short_base_length, ".guc");
#else
  attrsfile = stringappend (name_base, short_base_length, ".stype.h");
  guardfile = stringappend (name_base, short_base_length, ".guard.c");
#endif /* not MSDOS */
#endif /* not VMS */

  /* Initialize the obstacks. */
  obstack_init (&action_obstack);
  obstack_init (&attrs_obstack);
  obstack_init (&table_obstack);
  obstack_init (&defines_obstack);
}



/*--------------------------------------------------------------------.
| Open the output files needed only for the semantic parser.  This    |
| is done when %semantic_parser is seen in the declarations section.  |
`--------------------------------------------------------------------*/

void
open_extra_files (void)
{
  xfclose (fparser);

  if (!no_parser_flag)
    fparser = xfopen (skeleton_find ("BISON_HAIRY", BISON_HAIRY), "r");
  fguard = xfopen (guardfile, "w");
}


/*-----------------------------------------------------.
| Close the open files, produce all the output files.  |
`-----------------------------------------------------*/

void
output_files (void)
{
  xfclose (fguard);
  xfclose (finput);
  xfclose (fparser);
  xfclose (foutput);

  /* Output the main file.  */
  obstack_save (&table_obstack, tabfile);

  /* Output the header file if wanted. */
  if (defines_flag)
    obstack_save (&defines_obstack, defsfile);

  /* If we output only the table, dump the actions in ACTFILE.
     */
  if (no_parser_flag)
    obstack_save (&action_obstack, actfile);

  /* If we produced a semantic parser ATTRS_OBSTACK must be dumped
     into its own file, ATTTRSFILE.  */
  if (semantic_parser)
    obstack_save (&attrs_obstack, attrsfile);
}
