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

#include <stdarg.h>
#if ! (HAVE_VPRINTF || defined vfprintf)
# define vfprintf(stream, message, args) _doprnt (message, args, stream)
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
char *strerror (int);
#  endif
# else
static char *
private_strerror (int errnum)
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

/* This variable is set each time `warn' is called.  */
bool warning_issued;

/* This variable is set each time `complain' is called.  */
bool complaint_issued;


/*--------------------------------.
| Report a warning, and proceed.  |
`--------------------------------*/

void
warn_at (location_t location, const char *message, ...)
{
  va_list args;

  fflush (stdout);
  LOCATION_PRINT (stderr, location);
  fputs (": ", stderr);
  fputs (_("warning: "), stderr);

  va_start (args, message);
  vfprintf (stderr, message, args);
  va_end (args);

  warning_issued = true;
  putc ('\n', stderr);
  fflush (stderr);
}

void
warn (const char *message, ...)
{
  va_list args;

  fflush (stdout);
  fprintf (stderr, "%s: %s", infile ? infile : program_name, _("warning: "));

  va_start (args, message);
  vfprintf (stderr, message, args);
  va_end (args);

  warning_issued = true;
  putc ('\n', stderr);
  fflush (stderr);
}

/*-----------------------------------------------------------.
| An error has occurred, but we can proceed, and die later.  |
`-----------------------------------------------------------*/

void
complain_at (location_t location, const char *message, ...)
{
  va_list args;

  fflush (stdout);
  LOCATION_PRINT (stderr, location);
  fputs (": ", stderr);

  va_start (args, message);
  vfprintf (stderr, message, args);
  va_end (args);

  complaint_issued = true;
  putc ('\n', stderr);
  fflush (stderr);
}

void
complain (const char *message, ...)
{
  va_list args;

  fflush (stdout);
  fprintf (stderr, "%s: ", infile ? infile : program_name);

  va_start (args, message);
  vfprintf (stderr, message, args);
  va_end (args);

  complaint_issued = true;
  putc ('\n', stderr);
  fflush (stderr);
}

/*-------------------------------------------------.
| A severe error has occurred, we cannot proceed.  |
`-------------------------------------------------*/

void
fatal_at (location_t location, const char *message, ...)
{
  va_list args;

  fflush (stdout);
  LOCATION_PRINT (stderr, location);
  fputs (": ", stderr);
  fputs (_("fatal error: "), stderr);

  va_start (args, message);
  vfprintf (stderr, message, args);
  va_end (args);
  putc ('\n', stderr);
  fflush (stderr);
  exit (EXIT_FAILURE);
}

void
fatal (const char *message, ...)
{
  va_list args;

  fflush (stdout);
  fprintf (stderr, "%s: ", infile ? infile : program_name);

  fputs (_("fatal error: "), stderr);

  va_start (args, message);
  vfprintf (stderr, message, args);
  va_end (args);
  putc ('\n', stderr);
  fflush (stderr);
  exit (EXIT_FAILURE);
}
