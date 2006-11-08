/* Bison Action Scanner

   Copyright (C) 2006 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301  USA
*/

#ifndef SCAN_CODE_H_
# define SCAN_CODE_H_

# include "location.h"
# include "symlist.h"

/* Keeps track of the maximum number of semantic values to the left of
   a handle (those referenced by $0, $-1, etc.) are required by the
   semantic actions of this grammar. */
extern int max_left_semantic_context;

void code_scanner_free (void);

/* The action of the rule R contains $$, $1 etc. referring to the values
   of the rule R. */
char const *translate_rule_action (symbol_list *r);

/* The action A refers to $$ and @$ only, referring to a symbol. */
char const *translate_symbol_action (char const *a, location l);

/* The action contains no special escapes, just protect M4 special
   symbols.  */
char const *translate_code (char const *a, location l);

#endif /* !SCAN_CODE_H_ */
