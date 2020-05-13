/* Conflict counterexample generation
 
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

#ifndef COUNTEREXAMPLE_H
#define COUNTEREXAMPLE_H

#include "state-item.h"

void counterexample_init (void);
void counterexample_free (void);

void counterexample_report_shift_reduce (state_item_number itm1,
                                         state_item_number itm2,
                                         symbol_number next_sym);

void counterexample_report_reduce_reduce (state_item_number itm1,
                                          state_item_number itm2,
                                          bitset conflict_syms);
#endif /* COUNTEREXAMPLE_H */
