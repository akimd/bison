/* Prepare the LALR and GLR parser tables.
   Copyright (C) 2002  Free Software Foundation, Inc.

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

#ifndef TABLES_H_
# define TABLES_H_

# include "state.h"

extern int nvectors;

typedef int base_t;
extern base_t *base;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MIN, later mapped to BASE_NINF to
   keep parser tables small.  */
extern base_t base_ninf;

extern unsigned int *conflict_table;
extern unsigned int *conflict_list;
extern int conflict_list_cnt;

extern base_t *table;
extern base_t *check;
/* The value used in TABLE to denote explicit parse errors
   (%nonassoc), a negative infinite.  */
extern base_t table_ninf;

extern state_number_t *yydefgoto;
extern rule_number_t *yydefact;
extern int high;

void tables_generate PARAMS ((void));
void tables_free PARAMS ((void));

#endif /* !TABLES_H_ */
