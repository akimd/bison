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

/* ITEMSETSIZE is the size of the array ITEMSET.  */
short *itemset;
int itemsetsize;

static unsigned *ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
static unsigned *fderives;
static unsigned *firsts;

#define FDERIVES(Symbol)   (fderives + (Symbol) * rulesetsize)
#define   FIRSTS(Symbol)   (firsts   + (Symbol) * varsetsize)

/* number of words required to hold a bit for each rule */
static int rulesetsize;

/* number of words required to hold a bit for each variable */
static int varsetsize;


/*-----------------.
| Debugging code.  |
`-----------------*/

static void
print_closure (int n)
{
  int i;
  fprintf (stderr, "n = %d\n", n);
  for (i = 0; i < itemsetsize; ++i)
    fprintf (stderr, "   %d\n", itemset[i]);
  fprintf (stderr, "\n\n");
}


static void
print_firsts (void)
{
  int i;
  int j;
  unsigned *rowp;

  fprintf (stderr, "FIRSTS\n");

  for (i = ntokens; i < nsyms; i++)
    {
      fprintf (stderr, "\t%s firsts\n", tags[i]);

      rowp = FIRSTS (i - ntokens);

      for (j = 0; j < nvars; j++)
	if (BITISSET (rowp, j))
	  fprintf (stderr, "\t\t%d (%s)\n", j + ntokens, tags[j + ntokens]);
    }
  fprintf (stderr, "\n\n");
}


static void
print_fderives (void)
{
  int i;
  int j;
  unsigned *rp;

  fprintf (stderr, "FDERIVES\n");

  for (i = ntokens; i < nsyms; i++)
    {
      fprintf (stderr, "\t%s derives\n", tags[i]);
      rp = FDERIVES (i);

      for (j = 0; j <= nrules; j++)
	if (BITISSET (rp, j))
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
  unsigned *row;
  int symbol;
  short *sp;
  int rowsize;

  int i;

  varsetsize = rowsize = WORDSIZE (nvars);

  firsts = XCALLOC (unsigned, nvars * rowsize);

  row = firsts;
  for (i = ntokens; i < nsyms; i++)
    {
      sp = derives[i];
      while (*sp >= 0)
	{
	  symbol = ritem[rule_table[*sp++].rhs];
	  if (ISVAR (symbol))
	    {
	      symbol -= ntokens;
	      SETBIT (row, symbol);
	    }
	}

      row += rowsize;
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
  unsigned *rrow;
  unsigned *vrow;
  int j;
  unsigned cword;
  short *rp;
  int b;

  int ruleno;
  int i;

  fderives = XCALLOC (unsigned, nvars * rulesetsize) - ntokens * rulesetsize;

  set_firsts ();

  rrow = FDERIVES (ntokens);

  for (i = ntokens; i < nsyms; i++)
    {
      vrow = FIRSTS (i - ntokens);
      cword = *vrow++;
      b = 0;
      for (j = ntokens; j < nsyms; j++)
	{
	  if (cword & (1 << b))
	    {
	      rp = derives[j];
	      while ((ruleno = *rp++) > 0)
		SETBIT (rrow, ruleno);
	    }

	  b++;
	  if (b >= BITS_PER_WORD && j + 1 < nsyms)
	    {
	      cword = *vrow++;
	      b = 0;
	    }
	}

      rrow += rulesetsize;
    }

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
  int ruleno;
  /* Index over CORE. */
  int c;

  /* Index over RULESET. */
  int r;

  int itemno;

  if (trace_flag)
    {
      fprintf (stderr, "Entering closure (items = {");
      for (c = 0; c < n; ++c)
	fprintf (stderr, " %d ", core[c]);
      fprintf (stderr, "})\n");
    }

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

  ruleno = 0;
  itemsetsize = 0;
  c = 0;
  for (r = 0; r < rulesetsize; ++r)
    {
      if (ruleset[r] == 0)
	{
	  ruleno += BITS_PER_WORD;
	}
      else
	{
	  int b;

	  for (b = 0; b < BITS_PER_WORD; b++)
	    {
	      if (ruleset[r] & (1 << b))
		{
		  itemno = rule_table[ruleno].rhs;
		  while (c < n && core[c] < itemno)
		    {
		      itemset[itemsetsize] = core[c];
		      itemsetsize++;
		      c++;
		    }
		  itemset[itemsetsize] = itemno;
		  itemsetsize++;
		}

	      ruleno++;
	    }
	}
    }

  while (c < n)
    {
      itemset[itemsetsize] = core[c];
      itemsetsize++;
      c++;
    }

  if (trace_flag)
    print_closure (n);
}


void
free_closure (void)
{
  XFREE (itemset);
  XFREE (ruleset);
  XFREE (fderives + ntokens * rulesetsize);
}
