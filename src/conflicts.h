/* Find and resolve or report look-ahead conflicts for bison,
   Copyright 2000 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#ifndef CONFLICTS_H_
# define CONFLICTS_H_
# include "state.h"

void solve_conflicts PARAMS ((void));
void print_conflicts PARAMS ((FILE *out));
void print_reductions PARAMS ((int));
void free_conflicts PARAMS ((void));

/* Were there conflicts? */
extern int any_conflicts;
extern errs **err_table;
extern int expected_conflicts;
#endif /* !CONFLICTS_H_ */
