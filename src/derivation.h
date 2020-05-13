/* Counterexample derivation trees
 
 Copyright (C) 2019-2020 Free Software Foundation, Inc.
 
 This file is part of Bison, the GNU Compiler Compiler.
 
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

#ifndef DERIVATION_H
#define DERIVATION_H

#include <gl_list.h>
#include "gram.h"

/*
 Derivations are trees of symbols such that each non terminal's
 children are symbols that produce that nonterminal if they are
 relevant to the counterexample. The leaves of a derivation form
 a counterexample when printed.
 */

typedef struct derivation
{
  symbol_number sym;
  gl_list_t children;
} derivation;

derivation *derivation_new (symbol_number sym, gl_list_t children);
size_t derivation_size (const derivation *deriv);
void derivation_print (const derivation *deriv, FILE *f);
void derivation_print_leaves (const derivation *deriv, FILE *f);
void derivation_free (derivation *deriv);

const derivation *derivation_dot (void);

#endif /* DERIVATION_H */
