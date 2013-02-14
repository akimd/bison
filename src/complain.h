/* Declaration for error-reporting function for Bison.

   Copyright (C) 2000-2002, 2006, 2009-2013 Free Software Foundation,
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

/** The bits assigned to each warning type.  */
typedef enum
  {
    warning_midrule_values, /**< Unset or unused midrule values.  */
    warning_yacc,           /**< POSIXME.  */
    warning_conflicts_sr,   /**< S/R conflicts.  */
    warning_conflicts_rr,   /**< R/R conflicts.  */
    warning_deprecated,     /**< Obsolete constructs.  */
    warning_precedence,     /**< Useless precedence and associativity.  */
    warning_other,          /**< All other warnings.  */

    warnings_size           /**< The number of warnings.  Must be last.  */
  } warning_bit;

typedef enum
  {
    /**< Issue no warnings.  */
    Wnone             = 0,

    Wmidrule_values   = 1 << warning_midrule_values,
    Wyacc             = 1 << warning_yacc,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wdeprecated       = 1 << warning_deprecated,
    Wprecedence       = 1 << warning_precedence,
    Wother            = 1 << warning_other,

    Werror            = 1 << 10, /** This bit is no longer used. */

    complaint         = 1 << 11, /**< All complaints.  */
    fatal             = 1 << 12, /**< All fatal errors.  */
    silent            = 1 << 13, /**< Do not display the warning type.  */
    no_caret          = 1 << 14, /**< Do not display caret location.  */

    /**< All above warnings.  */
    Wall              = ~complaint & ~fatal & ~silent
  } warnings;


/** For each warning type, its severity.  */
typedef enum
  {
    severity_disabled = 0,
    severity_unset = 1,
    severity_warning = 2,
    severity_error = 3,
    severity_fatal = 4
  } severity;

/** Whether -Werror was set. */
extern bool warnings_are_errors;

/** For each warning type, its severity.  */
extern severity warnings_flag[];

/** Initialize this module.  */
void complain_init (void);

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
