/* Declaration for error-reporting function for Bison.
   Copyright (C) 2000, 2001, 2002  Free Software Foundation, Inc.

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

#include "system.h"

#if HAVE_VPRINTF || HAVE_DOPRNT || _LIBC
# ifdef __STDC__
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
# include <errno.h>

/* In GNU libc we want do not want to use the common name `error' directly.
   Instead make it a weak alias.  */
# define error __error
# define error_at_line __error_at_line

# ifdef USE_IN_LIBIO
#  include <libio/iolibio.h>
#  define fflush(s) _IO_fflush (s)
# endif

#else /* not _LIBC */

/* The calling program should define program_name and set it to the
   name of the executing program.  */
extern char *program_name;

# if HAVE_STRERROR
#  ifndef HAVE_DECL_STRERROR
"this configure-time declaration test was not run"
#  endif
#  if !HAVE_DECL_STRERROR && !defined strerror
char *strerror PARAMS ((int));
#  endif
# else
static char *
private_strerror (errnum)
     int errnum;
{
  extern char *sys_errlist[];
  extern int sys_nerr;

  if (errnum > 0 && errnum <= sys_nerr)
    return _(sys_errlist[errnum]);
  return _("Unknown system error");
}
#  define strerror private_strerror
# endif /* HAVE_STRERROR */
#endif	/* not _LIBC */

/* This variable is incremented each time `warn' is called.  */
unsigned int warn_message_count;

/* This variable is incremented each time `complain' is called.  */
unsigned int complain_message_count;


/*--------------------------------.
| Report a warning, and proceed.  |
`--------------------------------*/

void
#if defined VA_START && defined __STDC__
warn_at (location_t location, const char *message, ...)
#else
warn_at (location, message, va_alist)
  location_t location
  char *message;
  va_dcl
#endif
{
#ifdef VA_START
  va_list args;
#endif

  fflush (stdout);
  LOCATION_PRINT (stderr, location);
  fputs (": ", stderr);
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

void
#if defined VA_START && defined __STDC__
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

  fflush (stdout);
  fprintf (stderr, "%s: %s", infile ? infile : program_name, _("warning: "));

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
#if defined VA_START && defined __STDC__
complain_at (location_t location, const char *message, ...)
#else
complain_at (location, message, va_alist)
  location_t location;
  char *message;
  va_dcl
#endif
{
#ifdef VA_START
  va_list args;
#endif

  fflush (stdout);
  LOCATION_PRINT (stderr, location);
  fputs (": ", stderr);

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

void
#if defined VA_START && defined __STDC__
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

  fflush (stdout);
  fprintf (stderr, "%s: ", infile ? infile : program_name);

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
#if defined VA_START && defined __STDC__
fatal_at (location_t location, const char *message, ...)
#else
fatal_at (location, message, va_alist)
  location_t location;
  char *message;
  va_dcl
#endif
{
#ifdef VA_START
  va_list args;
#endif

  fflush (stdout);
  LOCATION_PRINT (stderr, location);
  fputs (": ", stderr);
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

void
#if defined VA_START && defined __STDC__
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
  fprintf (stderr, "%s: ", infile ? infile : program_name);

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
