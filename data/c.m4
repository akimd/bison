m4_divert(-1)                                                     -*- C -*-

# C M4 Macros for Bison.
# Copyright (C) 2002 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA


## ----------- ##
## Copuright.  ##
## ----------- ##

# b4_copyright(TITLE, YEARS)
# --------------------------
m4_define([b4_copyright],
[/* A Bison parser, made from b4_filename, by GNU bison b4_version.  */

/* $1,
   Copyright (C) $2 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */])


## ------------ ##
## Data Types.  ##
## ------------ ##

# b4_sint_type(MAX)
# -----------------
# Return the smallest signed int type able to handle the number MAX.
m4_define([b4_sint_type],
[m4_if(m4_eval([$1 <= 127]),        [1], [signed char],
       m4_eval([$1 <= 32767]),      [1], [signed short],
       [signed int])])


# b4_uint_type(MAX)
# -----------------
# Return the smallest unsigned int type able to handle the number MAX.
m4_define([b4_uint_type],
[m4_if(m4_eval([$1 <= 255]),        [1], [unsigned char],
       m4_eval([$1 <= 65535]),      [1], [unsigned short],
       [unsigned int])])



## ------------------------- ##
## Assigning token numbers.  ##
## ------------------------- ##

# b4_token_define(TOKEN-NAME, TOKEN-NUMBER)
# -----------------------------------------
# Output the definition of this token as #define.
m4_define([b4_token_define],
[#define $1 $2
])


# b4_token_enum(TOKEN-NAME, TOKEN-NUMBER)
# ---------------------------------------
# Output the definition of this token as an enum.
m4_define([b4_token_enum],
[$1 = $2])


# b4_token_defines(LIST-OF-PAIRS-TOKEN-NAME-TOKEN-NUMBER)
# -------------------------------------------------------
# Output the definition of the tokens (if there are) as enums and #define.
m4_define([b4_token_defines],
[m4_if([$@], [[]], [],
[/* Tokens.  */
#ifndef YYTOKENTYPE
# if defined (__STDC__) || defined (__cplusplus)
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
m4_map_sep([     b4_token_enum], [,
],
           [$@])
   };
# endif
  /* POSIX requires `int' for tokens in interfaces.  */
# define YYTOKENTYPE int
#endif /* !YYTOKENTYPE */
m4_map([b4_token_define], [$@])
])
])
