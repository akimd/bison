/* Part of the bison parser generator,
   Copyright 1984, 1989, 2000, 2001 Free Software Foundation, Inc.

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


/* Set up NULLABLE, a vector saying which nonterminals can expand into
   the null string.  NULLABLE[I - NTOKENS] is nonzero if symbol I can
   do so.  */

#include "system.h"
#include "getargs.h"
#include "symtab.h"
#include "types.h"
#include "gram.h"
#include "reduce.h"
#include "nullable.h"

char *nullable = NULL;

static void
nullable_print (FILE *out)
{
  int i;
  fputs ("NULLABLE\n", out);
  for (i = ntokens; i < nsyms; i++)
    fprintf (out, "\t%s: %s\n", symbols[i]->tag, nullable[i] ? "yes" : "no");
  fputs ("\n\n", out);
}

void
set_nullable (void)
{
  rule_number_t ruleno;
  symbol_number_t *s1;
  symbol_number_t *s2;
  shorts *p;

  symbol_number_t *squeue = XCALLOC (symbol_number_t, nvars);
  short *rcount = XCALLOC (short, nrules + 1);
  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  shorts **rsets = XCALLOC (shorts *, nvars) - ntokens;
  /* This is said to be more elements than we actually use.
     Supposedly NRITEMS - NRULES is enough.  But why take the risk?  */
  shorts *relts = XCALLOC (shorts, nritems + nvars + 1);

  if (trace_flag)
    fprintf (stderr, "Entering set_nullable\n");

  nullable = XCALLOC (char, nvars) - ntokens;

  s1 = s2 = squeue;
  p = relts;

  for (ruleno = 1; ruleno < nrules + 1; ++ruleno)
    if (rules[ruleno].useful)
      {
	rule_t *rule = &rules[ruleno];
	if (rule->rhs[0] >= 0)
	  {
	    /* This rule has a non empty RHS. */
	    item_number_t *r = NULL;
	    int any_tokens = 0;
	    for (r = rule->rhs; *r >= 0; ++r)
	      if (ISTOKEN (*r))
		any_tokens = 1;

	    /* This rule has only nonterminals: schedule it for the second
	       pass.  */
	    if (!any_tokens)
	      for (r = rule->rhs; *r >= 0; ++r)
		{
		  rcount[ruleno]++;
		  p->next = rsets[*r];
		  p->value = ruleno;
		  rsets[*r] = p;
		  p++;
		}
	  }
	else
	  {
	    /* This rule has an empty RHS. */
	    assert (rule_number_of_item_number (rule->rhs[0]) == ruleno);
	    if (rule->useful && !nullable[rule->lhs->number])
	      {
		nullable[rule->lhs->number] = 1;
		*s2++ = rule->lhs->number;
	      }
	  }
      }

  while (s1 < s2)
    for (p = rsets[*s1++]; p; p = p->next)
      {
	ruleno = p->value;
	if (--rcount[ruleno] == 0)
	  if (rules[ruleno].useful && !nullable[rules[ruleno].lhs->number])
	    {
	      nullable[rules[ruleno].lhs->number] = 1;
	      *s2++ = rules[ruleno].lhs->number;
	    }
      }

  XFREE (squeue);
  XFREE (rcount);
  XFREE (rsets + ntokens);
  XFREE (relts);

  if (trace_flag)
    nullable_print (stderr);
}


void
free_nullable (void)
{
  XFREE (nullable + ntokens);
}
