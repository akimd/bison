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

warnings warnings_flag =
  Wconflicts_sr | Wconflicts_rr | Wdeprecated  | Wother;

warnings errors_flag;

err_status complaint_status = status_none;
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
          "deprecated",
          "other"
        };

      bool any = false;
      int i;
      for (i = 0; i < ARRAY_CARDINALITY (warn_names); ++i)
        if (warn_flags & 1 << i)
          {
            bool err = warn_flags & errors_flag;
            fprintf (stderr, "%s-W", any ? ", " : " [");
            fprintf (stderr, "%s%s", err ? "error=" : "" , warn_names[i]);
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
  unsigned pos = 0;

  if (loc)
    pos += location_print (stderr, *loc);
  else
    pos += fprintf (stderr, "%s", current_file ? current_file : program_name);
  pos += fprintf (stderr, ": ");

  if (indent_ptr)
    {
      if (*indent_ptr)
        prefix = NULL;
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
  const char* prefix =
    flags & fatal ? _("fatal error")
    : flags & (errors_flag | complaint) ? _("error")
    : _("warning");

  if ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;
  else if ((flags & (warnings_flag & errors_flag)) && ! complaint_status)
    complaint_status = status_warning_as_error;
  if (flags & (warnings_flag | fatal | complaint))
    error_message (loc, flags, prefix, message, args);
  if (flags & fatal)
    exit (EXIT_FAILURE);
}

void
complain (location const *loc, warnings flags, const char *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

void
complain_indent (location const *loc, warnings flags, unsigned *indent,
                 const char *message, ...)
{
  va_list args;
  indent_ptr = indent;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

void
complain_args (location const *loc, warnings w, unsigned *indent,
               int argc, char *argv[])
{
  switch (argc)
  {
  case 1:
    complain_indent (loc, w, indent, "%s", _(argv[0]));
    break;
  case 2:
    complain_indent (loc, w, indent, _(argv[0]), argv[1]);
    break;
  case 3:
    complain_indent (loc, w, indent, _(argv[0]), argv[1], argv[2]);
    break;
  case 4:
    complain_indent (loc, w, indent, _(argv[0]), argv[1], argv[2], argv[3]);
    break;
  case 5:
    complain_indent (loc, w, indent, _(argv[0]), argv[1], argv[2], argv[3],
                     argv[4]);
    break;
  default:
    complain (loc, fatal, "too many arguments for complains");
    break;
  }
}
