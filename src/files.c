/* Open and close files for bison,
   Copyright (C) 1984, 1986, 1989, 1992, 2000 Free Software Foundation, Inc.

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
# include <ssdef.h>
# define unlink delete
# ifndef XPFILE
#  define XPFILE "GNU_BISON:[000000]BISON.SIMPLE"
# endif
# ifndef XPFILE1
#  define XPFILE1 "GNU_BISON:[000000]BISON.HAIRY"
# endif
#endif

#if defined (_MSC_VER)
# ifndef XPFILE
#  define XPFILE "c:/usr/local/lib/bison.simple"
# endif
# ifndef XPFILE1
#  define XPFILE1 "c:/usr/local/lib/bison.hairy"
# endif
#endif

#include "getargs.h"
#include "files.h"
#include "xalloc.h"
#include "gram.h"
#include "complain.h"

FILE *finput = NULL;
FILE *foutput = NULL;
FILE *fdefines = NULL;
FILE *ftable = NULL;
FILE *fattrs = NULL;
FILE *fguard = NULL;
FILE *faction = NULL;
FILE *fparser = NULL;

/* File name specified with -o for the output file, or 0 if no -o.  */
char *spec_outfile;

char *infile;
char *attrsfile;

static char *outfile;
static char *defsfile;
static char *tabfile;
static char *guardfile;
static char *actfile;
static char *tmpattrsfile;
static char *tmptabfile;
static char *tmpdefsfile;

extern char *mktemp ();		/* So the compiler won't complain */
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

/* JF this has been hacked to death.  Nowaday it sets up the file names for
   the output files, and opens the tmp files and the parser */
void
open_files (void)
{
  char *name_base;
#ifdef MSDOS
  register char *cp;
#endif
  char *filename;
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
      /* -o was specified.  The precise -o name will be used for ftable.
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

      name_base = fixed_outfiles ? FIXED_NAME_BASE : infile;

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

  if (!noparserflag)
    {
      filename = getenv ("BISON_SIMPLE");
#ifdef MSDOS
      /* File doesn't exist in current directory; try in INIT directory.  */
      cp = getenv ("INIT");
      if (filename == 0 && cp != NULL)
	{
	  filename = XMALLOC (char, strlen (cp) + strlen (PFILE) + 2);
	  strcpy (filename, cp);
	  cp = filename + strlen (filename);
	  *cp++ = '/';
	  strcpy (cp, PFILE);
	}
#endif /* MSDOS */
      fparser = xfopen (filename ? filename : PFILE, "r");
    }

  if (verboseflag)
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

  if (noparserflag)
    {
      /* use permanent name for actions file */
      actfile = stringappend (name_base, short_base_length, ".act");
      faction = xfopen (actfile, "w");
    }

#ifdef MSDOS
  if (!noparserflag)
    actfile = mktemp (stringappend (tmp_base, tmp_len, "acXXXXXX"));
  tmpattrsfile = mktemp (stringappend (tmp_base, tmp_len, "atXXXXXX"));
  tmptabfile = mktemp (stringappend (tmp_base, tmp_len, "taXXXXXX"));
  tmpdefsfile = mktemp (stringappend (tmp_base, tmp_len, "deXXXXXX"));
#else
  if (!noparserflag)
    actfile = mktemp (stringappend (tmp_base, tmp_len, "act.XXXXXX"));
  tmpattrsfile = mktemp (stringappend (tmp_base, tmp_len, "attrs.XXXXXX"));
  tmptabfile = mktemp (stringappend (tmp_base, tmp_len, "tab.XXXXXX"));
  tmpdefsfile = mktemp (stringappend (tmp_base, tmp_len, "defs.XXXXXX"));
#endif /* not MSDOS */

  if (!noparserflag)
    faction = xfopen (actfile, "w+");
  fattrs = xfopen (tmpattrsfile, "w+");
  ftable = xfopen (tmptabfile, "w+");

  if (definesflag)
    {
      defsfile = stringappend (name_base, base_length, ".h");
      fdefines = xfopen (tmpdefsfile, "w+");
    }

#if !(defined (MSDOS) || (defined(_WIN32) && !defined(__CYGWIN32__)))
  if (!noparserflag)
    unlink (actfile);
  unlink (tmpattrsfile);
  unlink (tmptabfile);
  unlink (tmpdefsfile);
#endif /* MSDOS || (_WIN32 && !__CYGWIN32__) */

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
}



/*--------------------------------------------------------------------.
| Open the output files needed only for the semantic parser.  This    |
| is done when %semantic_parser is seen in the declarations section.  |
`--------------------------------------------------------------------*/

void
open_extra_files (void)
{
  FILE *ftmp;
  int c;
  char *filename;
#ifdef MSDOS
  char *cp;
#endif

  xfclose (fparser);

  if (!noparserflag)
    {
      filename = (char *) getenv ("BISON_HAIRY");
#ifdef MSDOS
      /* File doesn't exist in current directory; try in INIT directory.  */
      cp = getenv ("INIT");
      if (filename == 0 && cp != NULL)
	{
	  filename = XMALLOC (char, strlen (cp) + strlen (PFILE1) + 2);
	  strcpy (filename, cp);
	  cp = filename + strlen (filename);
	  *cp++ = '/';
	  strcpy (cp, PFILE1);
	}
#endif
      fparser = xfopen (filename ? filename : PFILE1, "r");
    }

  /* JF change from inline attrs file to separate one */
  ftmp = xfopen (attrsfile, "w");
  rewind (fattrs);
  while ((c = getc (fattrs)) != EOF)	/* Thank god for buffering */
    putc (c, ftmp);
  xfclose (fattrs);
  fattrs = ftmp;

  fguard = xfopen (guardfile, "w");

}

void
done (void)
{
  xfclose (faction);
  xfclose (fattrs);
  xfclose (fguard);
  xfclose (finput);
  xfclose (fparser);
  xfclose (foutput);

  /* JF write out the output file */
  if (!complain_message_count && ftable)
    {
      FILE *ftmp;
      register int c;

      ftmp = xfopen (tabfile, "w");
      rewind (ftable);
      while ((c = getc (ftable)) != EOF)
	putc (c, ftmp);
      xfclose (ftmp);
      xfclose (ftable);

      if (definesflag)
	{
	  ftmp = xfopen (defsfile, "w");
	  fflush (fdefines);
	  rewind (fdefines);
	  while ((c = getc (fdefines)) != EOF)
	    putc (c, ftmp);
	  xfclose (ftmp);
	  xfclose (fdefines);
	}
    }

#if defined (VMS) & !defined (__VMS_POSIX)
  if (faction && !noparserflag)
    delete (actfile);
  if (fattrs)
    delete (tmpattrsfile);
  if (ftable)
    delete (tmptabfile);
/* Don't call exit again, we're in atexit ().
  if (!complain_message_count)
    sys$exit(SS$_NORMAL);
  sys$exit(SS$_ABORT); */
#else
#if (defined (MSDOS) || (defined(_WIN32) && !defined(__CYGWIN32__)))
  if (actfile && !noparserflag)
    unlink (actfile);
  if (tmpattrsfile)
    unlink (tmpattrsfile);
  if (tmptabfile)
    unlink (tmptabfile);
  if (tmpdefsfile)
    unlink (tmpdefsfile);
#endif /* MSDOS || (_WIN32 && !__CYGWIN32__) */
/* Don't call exit again, we're in atexit ().
  exit (complain_message_count ? 1 : 0); */
#endif /* not VMS, or __VMS_POSIX */
}
