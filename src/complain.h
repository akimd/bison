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

# ifdef __cplusplus
extern "C" {
# endif

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
    Wother            = 1 << 4,  /**< All other warnings.  */

    Werror            = 1 << 10, /**< Warnings are treated as errors.  */
    complaint         = 1 << 11, /**< All complaints.  */
    fatal             = 1 << 12, /**< All fatal errors.  */
    silent            = 1 << 13, /**< Do not display the warning type.  */
    Wall              = ~Werror  /**< All above warnings.  */
  } warnings;

/** What warnings are issued.  */
extern warnings warnings_flag;

/** Display a "[-Wyacc]" like message on stderr.  */
void warnings_print_categories (warnings warn_flags);

/** Record that a warning is about to be issued, and treat it as an
    error if <tt>warnings_flag & Werror</tt>.  This is exported
    only for the sake of Yacc-compatible conflict reports in conflicts.c.
    All other warnings should be implemented in complain.c and should use
    the normal warning format.  */
void set_warning_issued (void);

/** Make a complaint, but don't specify any location.  */
void complain (warnings flags, char const *message, ...)
  __attribute__ ((__format__ (__printf__, 2, 3)));

/** Make a complaint with location.  */
void complain_at (location loc, warnings flags, char const *message, ...)
  __attribute__ ((__format__ (__printf__, 3, 4)));

/** Make a complaint with location and some indentation.  */
void complain_at_indent (location loc, warnings flags, unsigned *indent,
                         char const *message, ...)
  __attribute__ ((__format__ (__printf__, 4, 5)));

/** Whether an error was reported.  */
extern bool complaint_issued;

# ifdef __cplusplus
}
# endif

#endif /* !COMPLAIN_H_ */
