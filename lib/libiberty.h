/* Fake libiberty.h for Bison.
   Copyright (C) 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */


/* Bison depends on libiberty's implementation of bitsets, which
   requires a `libiberty.h' file.  This file provides the minimum
   services.  */

#ifndef BISON_LIBIBERTY_H_
# define BISON_LIBIBERTY_H_ 1

# if HAVE_CONFIG_H
#  include <config.h>
# endif
# include <stdlib.h>

/* This file is the public interface to the bitset abstract data type.
   Only use the functions and macros defined in this file.  */

# ifndef PARAMS
#  if defined PROTOTYPES || (defined __STDC__ && __STDC__)
#   define PARAMS(Args) Args
#  else
#   define PARAMS(Args) ()
#  endif
# endif

# ifndef __attribute__
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 8) || __STRICT_ANSI__
#   define __attribute__(x)
#  endif
# endif

# ifndef ATTRIBUTE_NORETURN
#  define ATTRIBUTE_NORETURN __attribute__ ((__noreturn__))
# endif

# ifndef ATTRIBUTE_UNUSED
#  define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# endif

# include "xalloc.h"

#endif /* ! BISON_LIBIBERTY_H_ */
