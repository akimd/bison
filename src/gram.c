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
#include "symtab.h"
#include "gram.h"
#include "reduce.h"
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

token_number_t start_symbol = 0;

int max_user_token_number = 256;

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


/*-------------------.
| Dump the grammar.  |
`-------------------*/

void
grammar_dump (FILE *out, const char *title)
{
  int i;
  item_number_t *r;

  fprintf (out, "%s\n\n", title);
  fprintf (out,
	   "ntokens = %d, nvars = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
	   ntokens, nvars, nsyms, nrules, nritems);
  fprintf (out, "Variables\n---------\n\n");
  fprintf (out, "Value  Sprec  Sassoc  Tag\n");
  for (i = ntokens; i < nsyms; i++)
    fprintf (out, "%5d  %5d   %5d  %s\n",
	     i,
	     symbols[i]->prec, symbols[i]->assoc,
	     quotearg_style (escape_quoting_style, symbols[i]->tag));
  fprintf (out, "\n\n");
  fprintf (out, "Rules\n-----\n\n");
  fprintf (out, "Num (Prec, Assoc, Useful, Ritem Range) Lhs -> Rhs (Ritem range) [Num]\n");
  for (i = 1; i < nrules + nuseless_productions + 1; i++)
    {
      int rhs_count = 0;
      /* Find the last RHS index in ritems. */
      for (r = rules[i].rhs; *r >= 0; ++r)
	++rhs_count;
      fprintf (out, "%3d (%2d, %2d, %2d, %2d-%2d)   %2d ->",
	       i - 1,
	       rules[i].prec ? rules[i].prec->prec : 0,
	       rules[i].prec ? rules[i].prec->assoc : 0,
	       rules[i].useful,
	       rules[i].rhs - ritem,
	       rules[i].rhs - ritem + rhs_count - 1,
	       rules[i].lhs->number);
      /* Dumped the RHS. */
      for (r = rules[i].rhs; *r >= 0; r++)
	fprintf (out, " %3d", *r);
      fprintf (out, "  [%d]\n", -(*r) - 1);
    }
  fprintf (out, "\n\n");
  fprintf (out, "Rules interpreted\n-----------------\n\n");
  for (i = 1; i < nrules + nuseless_productions + 1; i++)
    {
      fprintf (out, "%-5d  %s :",
	       i, quotearg_style (escape_quoting_style, rules[i].lhs->tag));
      for (r = rules[i].rhs; *r >= 0; r++)
	fprintf (out, " %s",
		 quotearg_style (escape_quoting_style, symbols[*r]->tag));
      fputc ('\n', out);
    }
  fprintf (out, "\n\n");
}


void
grammar_free (void)
{
  XFREE (ritem);
  free (rules + 1);
  XFREE (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
}
