/* Match rules with nonterminals for bison,
   Copyright 1984, 1989, 2000, 2001, 2002  Free Software Foundation, Inc.

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
#include "getargs.h"
#include "symtab.h"
#include "types.h"
#include "reader.h"
#include "gram.h"
#include "derives.h"

rule_number_t **derives = NULL;

static void
print_derives (void)
{
  int i;

  fputs ("DERIVES\n", stderr);

  for (i = ntokens; i < nsyms; i++)
    {
      rule_number_t *rp;
      fprintf (stderr, "\t%s derives\n", symbols[i]->tag);
      for (rp = derives[i]; *rp > 0; rp++)
	{
	  item_number_t *rhsp;
	  fprintf (stderr, "\t\t%d:", *rp);
	  for (rhsp = rules[*rp].rhs; *rhsp >= 0; ++rhsp)
	    fprintf (stderr, " %s", symbols[*rhsp]->tag);
	  fprintf (stderr, " (rule %d)\n",
		   rule_number_of_item_number (*rhsp) - 1);
	}
    }

  fputs ("\n\n", stderr);
}


void
set_derives (void)
{
  symbol_number_t i;
  rule_number_t r;
  shorts *p;
  rule_number_t *q;
  shorts **dset;
  shorts *delts;

  dset = XCALLOC (shorts *, nvars) - ntokens;
  delts = XCALLOC (shorts, nrules + 1);

  p = delts;
  for (r = nrules; r > 0; r--)
    {
      symbol_number_t lhs = rules[r].lhs->number;
      p->next = dset[lhs];
      p->value = r;
      dset[lhs] = p;
      p++;
    }

  derives = XCALLOC (rule_number_t *, nvars) - ntokens;
  q = XCALLOC (short, nvars + int_of_rule_number (nrules));

  for (i = ntokens; i < nsyms; i++)
    {
      derives[i] = q;
      p = dset[i];
      while (p)
	{
	  *q++ = p->value;
	  p = p->next;
	}
      *q++ = -1;
    }

  if (trace_flag)
    print_derives ();

  XFREE (dset + ntokens);
  XFREE (delts);
}

void
free_derives (void)
{
  XFREE (derives[ntokens]);
  XFREE (derives + ntokens);
}
