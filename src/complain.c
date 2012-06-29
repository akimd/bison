/* Declaration for error-reporting function for Bison.

   Copyright (C) 2000-2002, 2004-2006, 2009-2012 Free Software
   Foundation, Inc.

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
#include <progname.h>

#include "complain.h"
#include "files.h"
#include "getargs.h"

warnings warnings_flag = Wconflicts_sr | Wconflicts_rr | Wother;

bool complaint_issued;
static unsigned *indent_ptr = 0;

void
warnings_print_categories (warnings warn_flags)
{
  if (! (warn_flags & silent))
    {
      char const *warn_names[] =
        {
          "midrule-values",
          "yacc",
          "conflicts-sr",
          "conflicts-rr",
          "other"
        };

      bool any = false;
      int i;
      for (i = 0; i < ARRAY_CARDINALITY (warn_names); ++i)
        if (warn_flags & 1 << i)
          {
            fprintf (stderr, "%s-W%s", any ? ", " : " [", warn_names[i]);
            any = true;
          }
      if (any)
        fprintf (stderr, "]");
    }
}

/** Report an error message.
 *
 * \param loc     the location, defaulting to the current file,
 *                or the program name.
 * \param flags   the category for this message.
 * \param prefix  put before the message (e.g., "warning").
 * \param message the error message, a printf format string.  Iff it
 *                ends with ": ", then no trailing newline is printed,
 *                and the caller should print the remaining
 *                newline-terminated message to stderr.
 * \param args    the arguments of the format string.
 */
static
void
error_message (const location *loc, warnings flags, const char *prefix,
               const char *message, va_list args)
{
  (void) flags;
  unsigned pos = 0;

  if (loc)
    pos += location_print (stderr, *loc);
  else
    pos += fprintf(stderr, "%s", current_file ? current_file : program_name);
  pos += fprintf(stderr, ": ");

  if (indent_ptr)
    {
      if (!*indent_ptr)
        *indent_ptr = pos;
      else if (*indent_ptr > pos)
        fprintf (stderr, "%*s", *indent_ptr - pos, "");
      indent_ptr = 0;
    }

  if (prefix)
    fprintf (stderr, "%s: ", prefix);

  vfprintf (stderr, message, args);
  warnings_print_categories (flags);
  {
    size_t l = strlen (message);
    if (l < 2 || message[l-2] != ':' || message[l-1] != ' ')
      {
        putc ('\n', stderr);
        fflush (stderr);
      }
  }
}

/** Raise a complaint. That can be a fatal error, a complaint or just a
    warning.  */

static inline void
complains (const location *loc, warnings flags, const char *message,
           va_list args)
{
  if (flags & complaint)
    {
      error_message (loc, complaint, NULL, message, args);
      complaint_issued = true;
    }
  else if (flags & fatal)
    {
      error_message (loc, fatal, _("fatal error"), message, args);
      exit (EXIT_FAILURE);
    }
  else if (flags & Wyacc)
    {
      if (yacc_flag)
        {
          error_message (loc, flags, NULL, message, args);
          complaint_issued = true;
        }
      else if (warnings_flag & Wyacc)
        {
          set_warning_issued ();
          error_message (loc, flags, _("warning"), message, args);
        }
    }
  else if (warnings_flag & flags)
    {
      set_warning_issued ();
      error_message (loc, flags, _("warning"), message, args);
    }
}

void
complain (warnings flags, const char *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (NULL, flags, message, args);
  va_end (args);
}

void
complain_at (location loc, warnings flags, const char *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (&loc, flags, message, args);
  va_end (args);
}

void complain_at_indent (location loc, warnings flags, unsigned *indent,
                         const char *message, ...)
{
  indent_ptr = indent;

  va_list args;
  va_start (args, message);
  complains (&loc, flags, message, args);
  va_end (args);
}

/*--------------------------------.
| Report a warning, and proceed.  |
`--------------------------------*/

void
set_warning_issued (void)
{
  static bool warning_issued = false;
  if (!warning_issued && (warnings_flag & Werror))
    {
      fprintf (stderr, "%s: warnings being treated as errors\n", program_name);
      complaint_issued = true;
    }
  warning_issued = true;
}
