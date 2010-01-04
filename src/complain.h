/* Declaration for error-reporting function for Bison.
   Copyright (C) 2000-2002, 2006, 2009-2010 Free Software Foundation,
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

# ifdef	__cplusplus
extern "C" {
# endif

/** Informative messages, but we proceed.  */

void warn (char const *format, ...)
  __attribute__ ((__format__ (__printf__, 1, 2)));

void warn_at (location loc, char const *format, ...)
  __attribute__ ((__format__ (__printf__, 2, 3)));

/** An error, but we continue and die later.  */

void complain (char const *format, ...)
  __attribute__ ((__format__ (__printf__, 1, 2)));

void complain_at (location loc, char const *format, ...)
  __attribute__ ((__format__ (__printf__, 2, 3)));

/** A fatal error, causing immediate exit.  */

void fatal (char const *format, ...)
  __attribute__ ((__noreturn__, __format__ (__printf__, 1, 2)));

void fatal_at (location loc, char const *format, ...)
  __attribute__ ((__noreturn__, __format__ (__printf__, 2, 3)));

/** Whether an error was reported.  */
extern bool complaint_issued;

# ifdef	__cplusplus
}
# endif

#endif /* !COMPLAIN_H_ */
