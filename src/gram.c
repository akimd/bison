/* Allocate input grammar variables for bison,
   Copyright (C) 1984, 1986, 1989, 2001, 2002 Free Software Foundation, Inc.

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
#include "quotearg.h"
#include "gram.h"
#include "symtab.h"
#include "reader.h"

/* comments for these variables are in gram.h  */

item_number_t *ritem = NULL;
int nritems = 0;

rule_t *rules = NULL;
int nrules = 0;

symbol_t **symbols = NULL;
int nsyms = 0;
int ntokens = 1;
int nvars = 0;

token_number_t *token_translations = NULL;

int start_symbol = 0;

int max_user_token_number = 256;

int semantic_parser = 0;

int pure_parser = 0;


/*--------------------------------------.
| Return the number of symbols in RHS.  |
`--------------------------------------*/

int
rule_rhs_length (rule_t *rule)
{
  int res = 0;
  item_number_t *rhsp;
  for (rhsp = rule->rhs; *rhsp >= 0; ++rhsp)
    ++res;
  return res;
}


/*------------------------.
| Dump RITEM for traces.  |
`------------------------*/

void
ritem_print (FILE *out)
{
  int i;
  fputs ("RITEM\n", out);
  for (i = 0; i < nritems; ++i)
    if (ritem[i] >= 0)
      fprintf (out, "  %s", quotearg_style (escape_quoting_style,
					    symbols[ritem[i]]->tag));
    else
      fprintf (out, "  (rule %d)\n", -ritem[i] - 1);
  fputs ("\n\n", out);
}


/*------------------------------------------.
| Return the size of the longest rule RHS.  |
`------------------------------------------*/

size_t
ritem_longest_rhs (void)
{
  int max = 0;
  int i;

  for (i = 1; i < nrules + 1; ++i)
    {
      int length = rule_rhs_length (&rules[i]);
      if (length > max)
	max = length;
    }

  return max;
}
