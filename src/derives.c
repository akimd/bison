/* Match rules with nonterminals for bison,
   Copyright (C) 1984, 1989, 2000, 2001, 2002  Free Software Foundation, Inc.

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
#include "reader.h"
#include "gram.h"
#include "derives.h"

/* Linked list of rule numbers.  */
typedef struct rule_list_s
{
  struct rule_list_s *next;
  rule_t *value;
} rule_list_t;

rule_t ***derives = NULL;

static void
print_derives (void)
{
  int i;

  fputs ("DERIVES\n", stderr);

  for (i = ntokens; i < nsyms; i++)
    {
      rule_t **rp;
      fprintf (stderr, "\t%s derives\n", symbols[i]->tag);
      for (rp = derives[i]; *rp; ++rp)
	{
	  fprintf (stderr, "\t\t%3d ", (*rp)->user_number);
	  rule_rhs_print (*rp, stderr);
	}
    }

  fputs ("\n\n", stderr);
}


void
derives_compute (void)
{
  symbol_number_t i;
  int r;
  rule_t **q;

  /* DSET[NTERM] -- A linked list of the numbers of the rules whose
     LHS is NTERM.  */
  rule_list_t **dset = XCALLOC (rule_list_t *, nvars) - ntokens;

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list_t *delts = XCALLOC (rule_list_t, nrules);

  for (r = nrules - 1; r >= 0; --r)
    {
      symbol_number_t lhs = rules[r].lhs->number;
      rule_list_t *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs];
      p->value = &rules[r];
      dset[lhs] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = XCALLOC (rule_t **, nvars) - ntokens;
  q = XCALLOC (rule_t *, nvars + int_of_rule_number (nrules));

  for (i = ntokens; i < nsyms; i++)
    {
      rule_list_t *p = dset[i];
      derives[i] = q;
      while (p)
	{
	  *q++ = p->value;
	  p = p->next;
	}
      *q++ = NULL;
    }

  if (trace_flag & trace_sets)
    print_derives ();

  free (dset + ntokens);
  free (delts);
}


void
derives_free (void)
{
  XFREE (derives[ntokens]);
  XFREE (derives + ntokens);
}
