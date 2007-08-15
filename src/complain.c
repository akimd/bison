/* Declaration for error-reporting function for Bison.

   Copyright (C) 2000, 2001, 2002, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Based on error.c and error.h,
   written by David MacKenzie <djm@gnu.ai.mit.edu>.  */

#include <config.h>
#include "system.h"

#include <stdarg.h>

#include "complain.h"
#include "files.h"
#include "getargs.h"

bool complaint_issued;



/** Report an error message.
 *
 * \param loc     the location, defaulting to the current file,
 *                or the program name.
 * \param prefix  put before the message (e.g., "warning").
 * \param message the error message, a printf format string.
 * \param args    the arguments of the format string.
 */
static
void
error_message (location *loc,
	       const char *prefix,
	       const char *message, va_list args)
{
  if (loc)
    location_print (stderr, *loc);
  else
    fputs (current_file ? current_file : program_name, stderr);
  fputs (": ", stderr);

  if (prefix)
    fprintf (stderr, "%s: ", prefix);

  vfprintf (stderr, message, args);
  va_end (args);
  putc ('\n', stderr);
  fflush (stderr);
}

/** Wrap error_message() with varargs handling. */
#define ERROR_MESSAGE(Loc, Prefix, Message)	\
{						\
  va_list args;					\
  va_start (args, Message);			\
  error_message (Loc, Prefix, Message, args);	\
}


/*--------------------------------.
| Report a warning, and proceed.  |
`--------------------------------*/

static void
set_warning_issued (void)
{
  static bool warning_issued = false;
  if (!warning_issued && (warnings_flag & warnings_error))
    {
      fprintf (stderr, "%s: warnings being treated as errors\n", program_name);
      complaint_issued = true;
    }
  warning_issued = true;
}

void
warn_at (location loc, const char *message, ...)
{
  set_warning_issued ();
  ERROR_MESSAGE (&loc, _("warning"), message);
}

void
warn (const char *message, ...)
{
  set_warning_issued ();
  ERROR_MESSAGE (NULL, _("warning"), message);
}


/*-----------------------------------------------------------.
| An error has occurred, but we can proceed, and die later.  |
`-----------------------------------------------------------*/

void
complain_at (location loc, const char *message, ...)
{
  ERROR_MESSAGE (&loc, NULL, message);
  complaint_issued = true;
}

void
complain (const char *message, ...)
{
  ERROR_MESSAGE (NULL, NULL, message);
  complaint_issued = true;
}


/*-------------------------------------------------.
| A severe error has occurred, we cannot proceed.  |
`-------------------------------------------------*/

void
fatal_at (location loc, const char *message, ...)
{
  ERROR_MESSAGE (&loc, _("fatal error"), message);
  exit (EXIT_FAILURE);
}

void
fatal (const char *message, ...)
{
  ERROR_MESSAGE (NULL, _("fatal error"), message);
  exit (EXIT_FAILURE);
}
