/* Subroutines for bison
   Copyright (C) 1984, 1989, 2000, 2001, 2002 Free Software Foundation, Inc.

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

#include "system.h"
#include "quotearg.h"
#include "bitset.h"
#include "bitsetv.h"
#include "bitsetv-print.h"
#include "getargs.h"
#include "symtab.h"
#include "gram.h"
#include "reader.h"
#include "closure.h"
#include "derives.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_number_t *itemset;
int nritemset;

static bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
static bitsetv fderives = NULL;
static bitsetv firsts = NULL;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define   FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

static void
print_closure (const char *title, item_number_t *array, size_t size)
{
  size_t i;
  fprintf (stderr, "Closure: %s\n", title);
  for (i = 0; i < size; ++i)
    {
      item_number_t *rp;
      fprintf (stderr, "  %2d: .", array[i]);
      for (rp = &ritem[array[i]]; *rp >= 0; ++rp)
	fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", -*rp - 1);
    }
  fputs ("\n\n", stderr);
}


static void
print_firsts (void)
{
  symbol_number_t i, j;

  fprintf (stderr, "FIRSTS\n");
  for (i = ntokens; i < nsyms; i++)
    {
      bitset_iterator iter;
      fprintf (stderr, "\t%s firsts\n", symbols[i]->tag);
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
	{
	  fprintf (stderr, "\t\t%s\n",
		   symbols[j + ntokens]->tag);
	}
    }
  fprintf (stderr, "\n\n");
}


static void
print_fderives (void)
{
  int i;
  rule_number_t r;

  fprintf (stderr, "FDERIVES\n");
  for (i = ntokens; i < nsyms; i++)
    {
      bitset_iterator iter;
      fprintf (stderr, "\t%s derives\n", symbols[i]->tag);
      BITSET_FOR_EACH (iter, FDERIVES (i), r, 0)
	{
	  fprintf (stderr, "\t\t%3d ", r);
	  rule_rhs_print (&rules[r], stderr);
	}
    }
  fprintf (stderr, "\n\n");
}

/*------------------------------------------------------------------.
| Set FIRSTS to be an NVARS array of NVARS bitsets indicating which |
| items can represent the beginning of the input corresponding to   |
| which other items.                                                |
|                                                                   |
| For example, if some rule expands symbol 5 into the sequence of   |
| symbols 8 3 20, the symbol 8 can be the beginning of the data for |
| symbol 5, so the bit [8 - ntokens] in first[5 - ntokens] (= FIRST |
| (5)) is set.                                                      |
`------------------------------------------------------------------*/

static void
set_firsts (void)
{
  symbol_number_t i, j;

  firsts = bitsetv_create (nvars, nvars, BITSET_FIXED);

  for (i = ntokens; i < nsyms; i++)
    for (j = 0; derives[i][j]; ++j)
      {
	int symbol = derives[i][j]->rhs[0];
	if (ISVAR (symbol))
	  bitset_set (FIRSTS (i), symbol - ntokens);
      }

  if (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  if (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  if (trace_flag & trace_sets)
    print_firsts ();
}

/*-------------------------------------------------------------------.
| Set FDERIVES to an NVARS by NRULES matrix of bits indicating which |
| rules can help derive the beginning of the data for each           |
| nonterminal.                                                       |
|                                                                    |
| For example, if symbol 5 can be derived as the sequence of symbols |
| 8 3 20, and one of the rules for deriving symbol 8 is rule 4, then |
| the [5 - NTOKENS, 4] bit in FDERIVES is set.                       |
`-------------------------------------------------------------------*/

static void
set_fderives (void)
{
  symbol_number_t i, j;
  rule_number_t k;

  fderives = bitsetv_create (nvars, nrules, BITSET_FIXED);

  set_firsts ();

  for (i = ntokens; i < nsyms; ++i)
    for (j = ntokens; j < nsyms; ++j)
      if (bitset_test (FIRSTS (i), j - ntokens))
	for (k = 0; derives[j][k]; ++k)
	  bitset_set (FDERIVES (i), derives[j][k]->number);

  if (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



void
new_closure (int n)
{
  itemset = XCALLOC (item_number_t, n);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



void
closure (item_number_t *core, int n)
{
  /* Index over CORE. */
  int c;

  /* A bit index over RULESET. */
  rule_number_t ruleno;

  bitset_iterator iter;

  if (trace_flag & trace_sets)
    print_closure ("input", core, n);

  bitset_zero (ruleset);

  for (c = 0; c < n; ++c)
    if (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  nritemset = 0;
  c = 0;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_number_t itemno = rules[ruleno].rhs - ritem;
      while (c < n && core[c] < itemno)
	{
	  itemset[nritemset] = core[c];
	  nritemset++;
	  c++;
	}
      itemset[nritemset] = itemno;
      nritemset++;
    };

  while (c < n)
    {
      itemset[nritemset] = core[c];
      nritemset++;
      c++;
    }

  if (trace_flag & trace_sets)
    print_closure ("output", itemset, nritemset);
}


void
free_closure (void)
{
  XFREE (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
