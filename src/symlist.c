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

#include "system.h"
#include "complain.h"
#include "symlist.h"


/*----------------------------------------------.
| Create a list containing SYMBOL at LOCATION.  |
`----------------------------------------------*/

symbol_list_t *
symbol_list_new (symbol_t *sym, location_t location)
{
  symbol_list_t *res = XMALLOC (symbol_list_t, 1);
  res->next = NULL;
  res->sym = sym;
  res->location = location;
  res->action = NULL;
  res->ruleprec = NULL;
  res->dprec = 0;
  res->merger = 0;
  return res;
}


/*-----------------------------------------.
| Prepend SYMBOL at LOCATION to the LIST.  |
`-----------------------------------------*/

symbol_list_t *
symbol_list_prepend (symbol_list_t *list,
		     symbol_t *symbol, location_t location)
{
  symbol_list_t *res = symbol_list_new (symbol, location);
  res->next = list;
  return res;
}


/*-------------------------------------------------.
| Free the LIST, but not the symbols it contains.  |
`-------------------------------------------------*/

void
symbol_list_free (symbol_list_t *list)
{
  LIST_FREE (symbol_list_t, list);
}


/*--------------------.
| Return its length.  |
`--------------------*/

unsigned int
symbol_list_length (symbol_list_t *list)
{
  int res = 0;
  for (/* Nothing. */; list; list = list->next)
    ++res;
  return res;
}


/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in rule RULE.                                        |
`--------------------------------------------------------------*/

char *
symbol_list_n_type_name_get (symbol_list_t *rule, location_t location, int n)
{
  int i;
  symbol_list_t *rp;

  if (n < 0)
    {
      complain_at (location, _("invalid $ value"));
      return NULL;
    }

  rp = rule;
  i = 0;

  while (i < n)
    {
      rp = rp->next;
      if (rp == NULL || rp->sym == NULL)
	{
	  complain_at (location, _("invalid $ value"));
	  return NULL;
	}
      ++i;
    }

  return rp->sym->type_name;
}
