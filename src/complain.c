/* Declaration for error-reporting function for Bison.
   Copyright (C) 2000 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

/* Based on error.c and error.h,
   written by David MacKenzie <djm@gnu.ai.mit.edu>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#if HAVE_VPRINTF || HAVE_DOPRNT || _LIBC
# if __STDC__
#  include <stdarg.h>
#  define VA_START(args, lastarg) va_start(args, lastarg)
# else
#  include <varargs.h>
#  define VA_START(args, lastarg) va_start(args)
# endif
#else
# define va_alist a1, a2, a3, a4, a5, a6, a7, a8
# define va_dcl char *a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8;
#endif

#if STDC_HEADERS || _LIBC
# include <stdlib.h>
# include <string.h>
#else
void exit ();
#endif

#include "complain.h"

#ifndef _
# define _(String) String
#endif

#ifdef _LIBC
/* In the GNU C library, there is a predefined variable for this.  */
# define program_name program_invocation_name
#else /* not _LIBC */
/* The calling program should define program_name and set it to the
   name of the executing program.  */
extern char *program_name;
#endif

/* This variable is incremented each time `warn' is called.  */
unsigned int warn_message_count;

/* This variable is incremented each time `complain' is called.  */
unsigned int complain_message_count;

/* Sometimes we want to have at most one error per line.  This
   variable controls whether this mode is selected or not.  */
int error_one_per_line;

/*--------------------------------.
| Report a warning, and proceed.  |
`--------------------------------*/

void
#if defined VA_START && __STDC__
warn (const char *message, ...)
#else
warn (message, va_alist)
     char *message;
     va_dcl
#endif
{
#ifdef VA_START
  va_list args;
#endif

  if (error_one_per_line)
    {
      static const char *old_infile;
      static unsigned int old_lineno;

      if (old_lineno == lineno &&
	  (infile == old_infile || !strcmp (old_infile, infile)))
	/* Simply return and print nothing.  */
	return;

      old_infile = infile;
      old_lineno = lineno;
    }

  fflush (stdout);
  if (infile != NULL)
    fprintf (stderr, "%s:%d: ", infile, lineno);
  else
    fprintf (stderr, "%s:", program_name);

  fputs (_("warning: "), stderr);

#ifdef VA_START
  VA_START (args, message);
  vfprintf (stderr, message, args);
  va_end (args);
#else
  fprintf (stderr, message, a1, a2, a3, a4, a5, a6, a7, a8);
#endif

  ++warn_message_count;
  putc ('\n', stderr);
  fflush (stderr);
}

/*-----------------------------------------------------------.
| An error has occurred, but we can proceed, and die later.  |
`-----------------------------------------------------------*/

void
#if defined VA_START && __STDC__
complain (const char *message, ...)
#else
complain (message, va_alist)
     char *message;
     va_dcl
#endif
{
#ifdef VA_START
  va_list args;
#endif

  if (error_one_per_line)
    {
      static const char *old_infile;
      static unsigned int old_lineno;

      if (old_lineno == lineno &&
	  (infile == old_infile || !strcmp (old_infile, infile)))
	/* Simply return and print nothing.  */
	return;

      old_infile = infile;
      old_lineno = lineno;
    }

  fflush (stdout);
  if (infile != NULL)
    fprintf (stderr, "%s:%d: ", infile, lineno);
  else
    fprintf (stderr, "%s:", program_name);

#ifdef VA_START
  VA_START (args, message);
  vfprintf (stderr, message, args);
  va_end (args);
#else
  fprintf (stderr, message, a1, a2, a3, a4, a5, a6, a7, a8);
#endif

  ++complain_message_count;
  putc ('\n', stderr);
  fflush (stderr);
}

/*-------------------------------------------------.
| A severe error has occurred, we cannot proceed.  |
`-------------------------------------------------*/

void
#if defined VA_START && __STDC__
fatal (const char *message, ...)
#else
fatal (message, va_alist)
     char *message;
     va_dcl
#endif
{
#ifdef VA_START
  va_list args;
#endif

  fflush (stdout);
  if (infile != NULL)
    fprintf (stderr, "%s:%d: ", infile, lineno);
  else
    fprintf (stderr, "%s:", program_name);

  fputs (_("fatal error: "), stderr);

#ifdef VA_START
  VA_START (args, message);
  vfprintf (stderr, message, args);
  va_end (args);
#else
  fprintf (stderr, message, a1, a2, a3, a4, a5, a6, a7, a8);
#endif
  putc ('\n', stderr);
  fflush (stderr);
  exit (1);
}

/*------------------------------------------------------------------.
| A severe error has occurred, we cannot proceed. Exit with STATUS, |
| and report the error message of the errno ERRNUM.                 |
`------------------------------------------------------------------*/

void
#if defined VA_START && __STDC__
error (int status, int errnum,
       const char *message, ...)
#else
error (status, errnum, message, va_alist)
     int status;
     int errnum;
     char *message;
     va_dcl
#endif
{
#ifdef VA_START
  va_list args;
#endif

  fflush (stdout);
  if (infile != NULL)
    fprintf (stderr, "%s:%d: ", infile, lineno);
  else
    fprintf (stderr, "%s:", program_name);

  fputs (_("fatal error: "), stderr);

#ifdef VA_START
  VA_START (args, message);
  vfprintf (stderr, message, args);
  va_end (args);
#else
  fprintf (stderr, message, a1, a2, a3, a4, a5, a6, a7, a8);
#endif

  if (errnum)
    {
#if defined HAVE_STRERROR_R || _LIBC
      char errbuf[1024];
# if HAVE_WORKING_STRERROR_R || _LIBC
      fprintf (stderr, ": %s", __strerror_r (errnum, errbuf, sizeof errbuf));
# else
      /* Don't use __strerror_r's return value because on some systems
	 (at least DEC UNIX 4.0[A-D]) strerror_r returns `int'.  */
      __strerror_r (errnum, errbuf, sizeof errbuf);
      fprintf (stderr, ": %s", errbuf);
# endif
#else
      fprintf (stderr, ": %s", strerror (errnum));
#endif
    }
  putc ('\n', stderr);
  fflush (stderr);
  if (status)
    exit (status);
}
