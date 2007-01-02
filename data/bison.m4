m4_divert(-1)                                               -*- Autoconf -*-

# Language-independent M4 Macros for Bison.
# Copyright (C) 2002, 2004, 2005, 2006, 2007 Free Software Foundation, Inc.

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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301  USA


## ---------------- ##
## Identification.  ##
## ---------------- ##

# b4_copyright(TITLE, YEARS)
# --------------------------
m4_define([b4_copyright],
[b4_comment([A Bison parser, made by GNU Bison b4_version.])

b4_comment([$1

m4_text_wrap([Copyright (C) $2 Free Software Foundation, Inc.], [   ])

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
Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.])

b4_comment([As a special exception, you may create a larger work that contains
part or all of the Bison parser skeleton and distribute that work
under terms of your choice, so long as that work isn't itself a
parser generator using the skeleton or a modified version thereof
as a parser skeleton.  Alternatively, if you modify or redistribute
the parser skeleton itself, you may (at your option) remove this
special exception, which will cause the skeleton and the resulting
Bison output files to be licensed under the GNU General Public
License without this special exception.

This special exception was added by the Free Software Foundation in
version 2.2 of Bison.])])


## ---------------- ##
## Default values.  ##
## ---------------- ##

# m4_define_default([b4_lex_param], [])   dnl breaks other skeletons
m4_define_default([b4_epilogue], [])
m4_define_default([b4_parse_param], [])

# The initial column and line.
m4_define_default([b4_location_initial_column], [1])
m4_define_default([b4_location_initial_line],   [1])


## ------------ ##
## Data Types.  ##
## ------------ ##

# b4_ints_in(INT1, INT2, LOW, HIGH)
# ---------------------------------
# Return 1 iff both INT1 and INT2 are in [LOW, HIGH], 0 otherwise.
m4_define([b4_ints_in],
[m4_eval([$3 <= $1 && $1 <= $4 && $3 <= $2 && $2 <= $4])])



## ------------------ ##
## Decoding options.  ##
## ------------------ ##

# b4_flag_if(FLAG, IF-TRUE, IF-FALSE)
# -----------------------------------
# Run IF-TRUE if b4_FLAG_flag is 1, IF-FALSE if FLAG is 0, otherwise fail.
m4_define([b4_flag_if],
[m4_case(b4_$1_flag,
         [0], [$3],
	 [1], [$2],
	 [m4_fatal([invalid $1 value: ]$1)])])


# b4_define_flag_if(FLAG)
# -----------------------
# Define "b4_FLAG_if(IF-TRUE, IF-FALSE)" that depends on the
# value of the Boolean FLAG.
m4_define([b4_define_flag_if],
[_b4_define_flag_if($[1], $[2], [$1])])

# _b4_define_flag_if($1, $2, FLAG)
# --------------------------------
# This macro works around the impossibility to define macros
# inside macros, because issuing `[$1]' is not possible in M4 :(.
# This sucks hard, GNU M4 should really provide M5 like $$1.
m4_define([_b4_define_flag_if],
[m4_if([$1$2], $[1]$[2], [],
       [m4_fatal([$0: Invalid arguments: $@])])dnl
m4_define([b4_$3_if],
          [b4_flag_if([$3], [$1], [$2])])])


# b4_FLAG_if(IF-TRUE, IF-FALSE)
# -----------------------------
# Expand IF-TRUE, if FLAG is true, IF-FALSE otherwise.
b4_define_flag_if([defines])	        # Whether headers are requested.
b4_define_flag_if([error_verbose])	# Whether error are verbose.
b4_define_flag_if([glr])		# Whether a GLR parser is requested.
b4_define_flag_if([locations])	        # Whether locations are tracked.
b4_define_flag_if([nondeterministic])	# Whether conflicts should be handled.
b4_define_flag_if([pull])	        # Whether pull parsing is requested.
b4_define_flag_if([pure])	        # Whether the interface is pure.
b4_define_flag_if([push])	        # Whether push parsing is requested.
b4_define_flag_if([use_push_for_pull])  # Whether push parsing should be used
                                        # in place of pull parsing (where
                                        # available) for the sake of the test
                                        # suite.
b4_define_flag_if([yacc])	        # Whether POSIX Yacc is emulated.


## ------------------------- ##
## Assigning token numbers.  ##
## ------------------------- ##


## ----------- ##
## Synclines.  ##
## ----------- ##

# b4_basename(NAME)
# -----------------
# Similar to POSIX basename; the differences don't matter here.
# Beware that NAME is not evaluated.
m4_define([b4_basename],
[m4_bpatsubst([$1], [^.*/\([^/]+\)/*$], [\1])])


# b4_syncline(LINE, FILE)
# -----------------------
m4_define([b4_syncline],
[b4_flag_if([synclines], [
b4_sync_end([__line__], [b4_basename(m4_quote(__file__))])
b4_sync_start([$1], [$2])])])

m4_define([b4_sync_end], [b4_comment([Line $1 of $2])])
m4_define([b4_sync_start], [b4_comment([Line $1 of $2])])

# b4_user_code(USER-CODE)
# -----------------------
# Emit code from the user, ending it with synclines.
m4_define([b4_user_code],
[$1
b4_syncline([@oline@], [@ofile@])])


# b4_define_user_code(MACRO)
# --------------------------
# From b4_MACRO, build b4_user_MACRO that includes the synclines.
m4_define([b4_define_user_code],
[m4_define([b4_user_$1],
[b4_user_code([b4_$1])])])


# b4_user_actions
# b4_user_initial_action
# b4_user_post_prologue
# b4_user_pre_prologue
# b4_user_provides
# b4_user_requires
# b4_user_stype
# ----------------------
# Macros that issue user code, ending with synclines.
b4_define_user_code([actions])
b4_define_user_code([initial_action])
b4_define_user_code([post_prologue])
b4_define_user_code([pre_prologue])
b4_define_user_code([provides])
b4_define_user_code([requires])
b4_define_user_code([stype])


