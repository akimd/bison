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

/* Comments for these variables are in gram.h.  */

item_number_t *ritem = NULL;
unsigned int nritems = 0;

rule_t *rules = NULL;
rule_number_t nrules = 0;

symbol_t **symbols = NULL;
int nsyms = 0;
int ntokens = 1;
int nvars = 0;

symbol_number_t *token_translations = NULL;

int max_user_token_number = 256;

int glr_parser = 0;
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


/*-------------------------------.
| Print this RULE's RHS on OUT.  |
`-------------------------------*/

void
rule_rhs_print (rule_t *rule, FILE *out)
{
  if (*rule->rhs >= 0)
    {
      item_number_t *r;
      for (r = rule->rhs; *r >= 0; r++)
	fprintf (out, " %s", symbol_tag_get (symbols[*r]));
      fputc ('\n', out);
    }
  else
    {
      fprintf (out, " /* %s */\n", _("empty"));
    }
}


/*-------------------------.
| Print this RULE on OUT.  |
`-------------------------*/

void
rule_print (rule_t *rule, FILE *out)
{
  fprintf (out, "%s:", symbol_tag_get (rule->lhs));
  rule_rhs_print (rule, out);
}


/*------------------------.
| Dump RITEM for traces.  |
`------------------------*/

void
ritem_print (FILE *out)
{
  unsigned int i;
  fputs ("RITEM\n", out);
  for (i = 0; i < nritems; ++i)
    if (ritem[i] >= 0)
      fprintf (out, "  %s", symbol_tag_get (symbols[ritem[i]]));
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
  rule_number_t r;

  for (r = 1; r < nrules + 1; ++r)
    {
      int length = rule_rhs_length (&rules[r]);
      if (length > max)
	max = length;
    }

  return max;
}


/*----------------------------------------------------------------.
| Print the grammar's rules numbers from BEGIN (inclusive) to END |
| (exclusive) on OUT under TITLE.                                 |
`----------------------------------------------------------------*/

static inline void
blanks_print (unsigned n, FILE *out)
{
  for (/* Nothing*/; n > 0; --n)
    fputc (' ', out);
}

void
grammar_rules_partial_print (FILE *out, const char *title,
			     rule_number_t begin, rule_number_t end)
{
  int r;
  symbol_t *last_lhs = NULL;

  /* rule # : LHS -> RHS */
  fprintf (out, "%s\n\n", title);
  for (r = begin; r < end; r++)
    {
      if (last_lhs && last_lhs != rules[r].lhs)
	fputc ('\n', out);

      fprintf (out, "  %3d ", r - 1);
      if (last_lhs != rules[r].lhs)
	{
	  last_lhs = rules[r].lhs;
	  fprintf (out, "%s:", symbol_tag_get (last_lhs));
	}
      else
	{
	  blanks_print (strlen (symbol_tag_get (last_lhs)), out);
	  fputc ('|', out);
	}
      rule_rhs_print (&rules[r], out);
    }
  fputs ("\n\n", out);
}


/*------------------------------------------.
| Print the grammar's useful rules on OUT.  |
`------------------------------------------*/

void
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), 1, nrules + 1);
}


/*-------------------.
| Dump the grammar.  |
`-------------------*/

void
grammar_dump (FILE *out, const char *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
	   "ntokens = %d, nvars = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
	   ntokens, nvars, nsyms, nrules, nritems);


  fprintf (out, "Variables\n---------\n\n");
  {
    symbol_number_t i;
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    for (i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
	       i,
	       symbols[i]->prec, symbols[i]->assoc,
	       symbol_tag_get (symbols[i]));
    fprintf (out, "\n\n");
  }

  fprintf (out, "Rules\n-----\n\n");
  {
    rule_number_t i;
    fprintf (out, "Num (Prec, Assoc, Useful, Ritem Range) Lhs -> Rhs (Ritem range) [Num]\n");
    for (i = 1; i < nrules + nuseless_productions + 1; i++)
      {
	rule_t *rule = &rules[i];
	item_number_t *r = NULL;
  	int rhs_count = 0;
	/* Find the last RHS index in ritems. */
	for (r = rule->rhs; *r >= 0; ++r)
	  ++rhs_count;
	fprintf (out, "%3d (%2d, %2d, %2d, %2d-%2d)   %2d ->",
		 i - 1,
		 rule->prec ? rule->prec->prec : 0,
		 rule->prec ? rule->prec->assoc : 0,
		 rule->useful,
		 rule->rhs - ritem,
		 rule->rhs - ritem + rhs_count - 1,
		 rule->lhs->number);
	/* Dumped the RHS. */
	for (r = rule->rhs; *r >= 0; r++)
	  fprintf (out, " %3d", *r);
	fprintf (out, "  [%d]\n", -(*r) - 1);
      }
  }
  fprintf (out, "\n\n");

  fprintf (out, "Rules interpreted\n-----------------\n\n");
  {
    rule_number_t r;
    for (r = 1; r < nrules + nuseless_productions + 1; r++)
      {
	fprintf (out, "%-5d  ", r);
	rule_print (&rules[r], out);
      }
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
  free_merger_functions ();
}
