m4_divert(-1)                                               -*- Autoconf -*-

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
## Copyright.  ##
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
   Boston, MA 02111-1307, USA.  */])


## ------------ ##
## Data Types.  ##
## ------------ ##


# b4_ints_in(INT1, INT2, LOW, HIGH)
# ---------------------------------
# Return 1 iff both INT1 and INT2 are in [LOW, HIGH], 0 otherwise.
m4_define([b4_ints_in],
[m4_eval([$3 <= $1 && $1 <= $4 && $3 <= $2 && $2 <= $4])])


# b4_int_type(MIN, MAX)
# ---------------------
# Return the smallest int type able to handle numbers ranging from
# MIN to MAX (included).
m4_define([b4_int_type],
[m4_if(b4_ints_in($@,      [0],   [255]), [1], [unsigned char],
       b4_ints_in($@,   [-128],   [127]), [1], [yysigned_char],

       b4_ints_in($@,      [0], [65535]), [1], [unsigned short],
       b4_ints_in($@, [-32768], [32767]), [1], [short],

       m4_eval([0 <= $1]),                [1], [unsigned int],

 	                                       [int])])

# b4_int_type_for(NAME)
# ---------------------
# Return the smallest int type able to handle numbers ranging from
# `NAME_min' to `NAME_max' (included).
m4_define([b4_int_type_for],
[b4_int_type($1_min, $1_max)])


## ------------------ ##
## Decoding options.  ##
## ------------------ ##


# b4_location_if(IF-TRUE, IF-FALSE)
# ---------------------------------
# Expand IF-TRUE, if locations are used, IF-FALSE otherwise.
m4_define([b4_location_if],
[m4_if(b4_locations_flag, [1],
       [$1],
       [$2])])


# b4_pure_if(IF-TRUE, IF-FALSE)
# -----------------------------
# Expand IF-TRUE, if %pure-parser, IF-FALSE otherwise.
m4_define([b4_pure_if],
[m4_if(b4_pure, [1],
       [$1],
       [$2])])



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
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
m4_map_sep([     b4_token_enum], [,
],
           [$@])
   };
#endif
m4_map([b4_token_define], [$@])
])
])



## --------------------------------------------- ##
## Defining C functions in both K&R and ANSI-C.  ##
## --------------------------------------------- ##


# b4_c_function_def(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# ----------------------------------------------------------
# Declare the function NAME.
m4_define([b4_c_function_def],
[#if defined (__STDC__) || defined (__cplusplus)
b4_c_ansi_function_def($@)
#else
$2
$1 (b4_c_knr_formal_names(m4_shiftn(2, $@)))
b4_c_knr_formal_decls(m4_shiftn(2, $@))
#endif[]dnl
])


# b4_c_ansi_function_def(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# ---------------------------------------------------------------
# Declare the function NAME in ANSI.
m4_define([b4_c_ansi_function_def],
[$2
$1 (b4_c_ansi_formals(m4_shiftn(2, $@)))[]dnl
])


# b4_c_ansi_formals([DECL1, NAME1], ...)
# --------------------------------------
# Output the arguments ANSI-C definition.
m4_define([b4_c_ansi_formals],
[m4_case([$@],
         [],   [void],
         [[]], [void],
               [m4_map_sep([b4_c_ansi_formal], [, ], [$@])])])

m4_define([b4_c_ansi_formal],
[$1])


# b4_c_knr_formal_names([DECL1, NAME1], ...)
# ------------------------------------------
# Output the argument names.
m4_define([b4_c_knr_formal_names],
[m4_map_sep([b4_c_knr_formal_name], [, ], [$@])])

m4_define([b4_c_knr_formal_name],
[$2])


# b4_c_knr_formal_decls([DECL1, NAME1], ...)
# ------------------------------------------
# Output the K&R argument declarations.
m4_define([b4_c_knr_formal_decls],
[m4_map_sep([b4_c_knr_formal_decl],
            [
],
            [$@])])

m4_define([b4_c_knr_formal_decl],
[    $1;])



## ------------------------------------------------------------ ##
## Declaring (prototyping) C functions in both K&R and ANSI-C.  ##
## ------------------------------------------------------------ ##


# b4_c_function_decl(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# -----------------------------------------------------------
# Declare the function NAME.
m4_define([b4_c_function_decl],
[#if defined (__STDC__) || defined (__cplusplus)
b4_c_ansi_function_decl($@)
#else
$2 $1 ();
#endif[]dnl
])


# b4_c_ansi_function_decl(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# ----------------------------------------------------------------
# Declare the function NAME.
m4_define([b4_c_ansi_function_decl],
[$2 $1 (b4_c_ansi_formals(m4_shiftn(2, $@)));[]dnl
])




## --------------------- ##
## Calling C functions.  ##
## --------------------- ##


# b4_c_function_call(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# -----------------------------------------------------------
# Call the function NAME with arguments NAME1, NAME2 etc.
m4_define([b4_c_function_call],
[$1 (b4_c_args(m4_shiftn(2, $@)))[]dnl
])


# b4_c_args([DECL1, NAME1], ...)
# ------------------------------
# Output the arguments NAME1, NAME2...
m4_define([b4_c_args],
[m4_map_sep([b4_c_arg], [, ], [$@])])

m4_define([b4_c_arg],
[$2])
