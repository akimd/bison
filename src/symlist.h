/* Lists of symbols for Bison
   Copyright (C) 2002  Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef SYMLIST_H_
# define SYMLIST_H_

# include "symtab.h"
# include "location.h"

typedef struct symbol_list_s
{
  struct symbol_list_s *next;
  symbol_t *sym;
  location_t location;

  /* The action is attached to the LHS of a rule. */
  const char *action;
  location_t action_location;

  symbol_t *ruleprec;
  int dprec;
  int merger;
} symbol_list_t;


/* Create a list containing SYMBOL at LOCATION.  */
symbol_list_t *symbol_list_new PARAMS ((symbol_t *sym, location_t location));

/* Prepend SYMBOL at LOCATION to the LIST.  */
symbol_list_t * symbol_list_prepend PARAMS ((symbol_list_t *list,
					     symbol_t *symbol,
					     location_t location));

/* Free the LIST, but not the symbols it contains.  */
void symbol_list_free PARAMS ((symbol_list_t *list));

/* Return its length. */
unsigned int symbol_list_length PARAMS ((symbol_list_t *list));

/* Get the data type (alternative in the union) of the value for
   symbol N in rule RULE.  */
char *symbol_list_n_type_name_get PARAMS ((symbol_list_t *rule,
					   location_t location, int n));

#endif /* !SYMLIST_H_ */
