/* Declaration for error-reporting function for Bison.

   Copyright (C) 2000-2002, 2006, 2009-2012 Free Software Foundation,
   Inc.

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

#ifndef COMPLAIN_H_
# define COMPLAIN_H_ 1

# include "location.h"

/* Sub-messages indent. */
# define SUB_INDENT (4)

/*-------------.
| --warnings.  |
`-------------*/

typedef enum
  {
    Wnone             = 0,       /**< Issue no warnings.  */
    Wmidrule_values   = 1 << 0,  /**< Unset or unused midrule values.  */
    Wyacc             = 1 << 1,  /**< POSIXME.  */
    Wconflicts_sr     = 1 << 2,  /**< S/R conflicts.  */
    Wconflicts_rr     = 1 << 3,  /**< R/R conflicts.  */
    Wdeprecated       = 1 << 4,  /**< Obsolete constructs.  */
    Wother            = 1 << 5,  /**< All other warnings.  */

    Werror            = 1 << 10, /** This bit is no longer used. */

    complaint         = 1 << 11, /**< All complaints.  */
    fatal             = 1 << 12, /**< All fatal errors.  */
    silent            = 1 << 13, /**< Do not display the warning type.  */
    no_caret          = 1 << 14, /**< Do not display caret location.  */

    /**< All above warnings.  */
    Wall              = ~complaint & ~fatal & ~silent
  } warnings;

/** What warnings are issued.  */
extern warnings warnings_flag;

/** What warnings are made errors.  */
extern warnings errors_flag;

/** Display a "[-Wyacc]" like message on stderr.  */
void warnings_print_categories (warnings warn_flags);

/** Make a complaint, with maybe a location.  */
void complain (location const *loc, warnings flags, char const *message, ...)
  __attribute__ ((__format__ (__printf__, 3, 4)));

/** Likewise, but with an \a argc/argv interface.  */
void complain_args (location const *loc, warnings w, unsigned *indent,
                    int argc, char *arg[]);

/** Make a complaint with location and some indentation.  */
void complain_indent (location const *loc, warnings flags, unsigned *indent,
                      char const *message, ...)
  __attribute__ ((__format__ (__printf__, 4, 5)));


/** Report an obsolete syntax, suggest the updated one.  */
void deprecated_directive (location const *loc,
                           char const *obsolete, char const *updated);

/** Warnings treated as errors shouldn't stop the execution as regular errors
    should (because due to their nature, it is safe to go on). Thus, there are
    three possible execution statuses.  */
typedef enum
  {
    status_none,
    status_warning_as_error,
    status_complaint
  } err_status;

/** Whether an error was reported.  */
extern err_status complaint_status;

#endif /* !COMPLAIN_H_ */
