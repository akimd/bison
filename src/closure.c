/* Subroutines for bison
   Copyright 1984, 1989, 2000, 2001 Free Software Foundation, Inc.

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
#include "getargs.h"
#include "gram.h"
#include "reader.h"
#include "closure.h"
#include "derives.h"
#include "warshall.h"

/* NITEMSET is the size of the array ITEMSET.  */
short *itemset;
int nitemset;

static unsigned *ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
static unsigned *fderives;
static unsigned *firsts;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   (fderives + ((Var) - ntokens) * rulesetsize)
#define   FIRSTS(Var)   (firsts   + ((Var) - ntokens) * varsetsize)

/* number of words required to hold a bit for each rule */
static int rulesetsize;

/* number of words required to hold a bit for each variable */
static int varsetsize;


/*-----------------.
| Debugging code.  |
`-----------------*/

static void
print_closure (const char *title, short *array, size_t size)
{
  size_t i;
  fprintf (stderr, "Closure: %s\n", title);
  for (i = 0; i < size; ++i)
    {
      short *rp;
      fprintf (stderr, "  %2d: .", array[i]);
      for (rp = &ritem[array[i]]; *rp > 0; ++rp)
	fprintf (stderr, " %s", tags[*rp]);
      fprintf (stderr, "  (rule %d)\n", -*rp);
    }
  fputs ("\n\n", stderr);
}


static void
print_firsts (void)
{
  int i, j;

  fprintf (stderr, "FIRSTS\n");
  for (i = ntokens; i < nsyms; i++)
    {
      fprintf (stderr, "\t%s firsts\n", tags[i]);
      for (j = 0; j < nvars; j++)
	if (BITISSET (FIRSTS (i), j))
	  fprintf (stderr, "\t\t%d (%s)\n", j + ntokens, tags[j + ntokens]);
    }
  fprintf (stderr, "\n\n");
}


static void
print_fderives (void)
{
  int i;
  int j;

  fprintf (stderr, "FDERIVES\n");

  for (i = ntokens; i < nsyms; i++)
    {
      fprintf (stderr, "\t%s derives\n", tags[i]);
      for (j = 0; j <= nrules; j++)
	if (BITISSET (FDERIVES (i), j))
	  {
	    short *rhsp;
	    fprintf (stderr, "\t\t%d:", j);
	    for (rhsp = ritem + rule_table[j].rhs; *rhsp > 0; ++rhsp)
	      fprintf (stderr, " %s", tags[*rhsp]);
	    fputc ('\n', stderr);
	  }
    }
  fprintf (stderr, "\n\n");
}

/*-------------------------------------------------------------------.
| Set FIRSTS to be an NVARS by NVARS bit matrix indicating which     |
| items can represent the beginning of the input corresponding to    |
| which other items.                                                 |
|                                                                    |
| For example, if some rule expands symbol 5 into the sequence of    |
| symbols 8 3 20, the symbol 8 can be the beginning of the data for  |
| symbol 5, so the bit [8 - ntokens, 5 - ntokens] in firsts is set.  |
`-------------------------------------------------------------------*/

static void
set_firsts (void)
{
  int i, j;

  varsetsize = WORDSIZE (nvars);

  firsts = XCALLOC (unsigned, nvars * varsetsize);

  for (i = ntokens; i < nsyms; i++)
    for (j = 0; derives[i][j] >= 0; ++j)
      {
	int symbol = ritem[rule_table[derives[i][j]].rhs];
	if (ISVAR (symbol))
	  SETBIT (FIRSTS (i), symbol - ntokens);
      }

  RTC (firsts, nvars);

  if (trace_flag)
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
  int i, j, k;

  fderives = XCALLOC (unsigned, nvars * rulesetsize);

  set_firsts ();

  for (i = ntokens; i < nsyms; ++i)
    for (j = ntokens; j < nsyms; ++j)
      if (BITISSET (FIRSTS (i), j - ntokens))
	for (k = 0; derives[j][k] > 0; ++k)
	  SETBIT (FDERIVES (i), derives[j][k]);

  if (trace_flag)
    print_fderives ();

  XFREE (firsts);
}


void
new_closure (int n)
{
  itemset = XCALLOC (short, n);

  rulesetsize = WORDSIZE (nrules + 1);
  ruleset = XCALLOC (unsigned, rulesetsize);

  set_fderives ();
}



void
closure (short *core, int n)
{
  /* Index over CORE. */
  int c;

  /* Index over RULESET. */
  int r;

  /* A bit index over RULESET. */
  int ruleno;

  if (trace_flag)
    print_closure ("input", core, n);

  if (n == 0)
    {
      for (r = 0; r < rulesetsize; ++r)
	ruleset[r] = FDERIVES (start_symbol)[r];
    }
  else
    {
      for (r = 0; r < rulesetsize; ++r)
	ruleset[r] = 0;

      for (c = 0; c < n; ++c)
	if (ISVAR (ritem[core[c]]))
	  for (r = 0; r < rulesetsize; ++r)
	    ruleset[r] |= FDERIVES (ritem[core[c]])[r];
    }

  nitemset = 0;
  c = 0;
  for (ruleno = 0; ruleno < rulesetsize * BITS_PER_WORD; ++ruleno)
    if (BITISSET (ruleset, ruleno))
      {
	int itemno = rule_table[ruleno].rhs;
	while (c < n && core[c] < itemno)
	  {
	    itemset[nitemset] = core[c];
	    nitemset++;
	    c++;
	  }
	itemset[nitemset] = itemno;
	nitemset++;
      }

  while (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  if (trace_flag)
    print_closure ("output", itemset, nitemset);
}


void
free_closure (void)
{
  XFREE (itemset);
  XFREE (ruleset);
  XFREE (fderives);
}
