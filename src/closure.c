/* Subroutines for bison
   Copyright 1984, 1989, 2000 Free Software Foundation, Inc.

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
#include "xalloc.h"
#include "gram.h"
#include "closure.h"
#include "derives.h"
#include "warshall.h"

short *itemset;
short *itemsetend;
static unsigned *ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
static unsigned *fderives;
static unsigned *firsts;

/* number of words required to hold a bit for each rule */
static int rulesetsize;

/* number of words required to hold a bit for each variable */
static int varsetsize;

#if DEBUG

/*-----------------.
| Debugging code.  |
`-----------------*/

static void
print_closure (int n)
{
  short *isp;

  printf ("\n\nn = %d\n\n", n);
  for (isp = itemset; isp < itemsetend; isp++)
    printf ("   %d\n", *isp);
}


static void
print_firsts (void)
{
  int i;
  int j;
  unsigned *rowp;

  printf ("\n\n\nFIRSTS\n\n");

  for (i = ntokens; i < nsyms; i++)
    {
      printf ("\n\n%s firsts\n\n", tags[i]);

      rowp = firsts + ((i - ntokens) * varsetsize);

      for (j = 0; j < nvars; j++)
	if (BITISSET (rowp, j))
	  printf ("   %s\n", tags[j + ntokens]);
    }
}


static void
print_fderives (void)
{
  int i;
  int j;
  unsigned *rp;

  printf ("\n\n\nFDERIVES\n");

  for (i = ntokens; i < nsyms; i++)
    {
      printf ("\n\n%s derives\n\n", tags[i]);
      rp = fderives + i * rulesetsize;

      for (j = 0; j <= nrules; j++)
	if (BITISSET (rp, j))
	  printf ("   %d\n", j);
    }

  fflush (stdout);
}
#endif

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
	  symbol = ritem[rrhs[*sp++]];
	  if (ISVAR (symbol))
	    {
	      symbol -= ntokens;
	      SETBIT (row, symbol);
	    }
	}

      row += rowsize;
    }

  RTC (firsts, nvars);

#ifdef	DEBUG
  print_firsts ();
#endif
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

  rrow = fderives + ntokens * rulesetsize;

  for (i = ntokens; i < nsyms; i++)
    {
      vrow = firsts + ((i - ntokens) * varsetsize);
      cword = *vrow++;
      b = 0;
      for (j = ntokens; j < nsyms; j++)
	{
	  if (cword & (1 << b))
	    {
	      rp = derives[j];
	      while ((ruleno = *rp++) > 0)
		{
		  SETBIT (rrow, ruleno);
		}
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

#ifdef	DEBUG
  print_fderives ();
#endif

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
  unsigned word;
  short *csp;
  unsigned *dsp;
  unsigned *rsp;

  short *csend;
  unsigned *rsend;
  int symbol;
  int itemno;

  rsp = ruleset;
  rsend = ruleset + rulesetsize;
  csend = core + n;

  if (n == 0)
    {
      dsp = fderives + start_symbol * rulesetsize;
      while (rsp < rsend)
	*rsp++ = *dsp++;
    }
  else
    {
      while (rsp < rsend)
	*rsp++ = 0;

      csp = core;
      while (csp < csend)
	{
	  symbol = ritem[*csp++];
	  if (ISVAR (symbol))
	    {
	      dsp = fderives + symbol * rulesetsize;
	      rsp = ruleset;
	      while (rsp < rsend)
		*rsp++ |= *dsp++;
	    }
	}
    }

  ruleno = 0;
  itemsetend = itemset;
  csp = core;
  rsp = ruleset;
  while (rsp < rsend)
    {
      word = *rsp++;
      if (word == 0)
	{
	  ruleno += BITS_PER_WORD;
	}
      else
	{
	  int b;

	  for (b = 0; b < BITS_PER_WORD; b++)
	    {
	      if (word & (1 << b))
		{
		  itemno = rrhs[ruleno];
		  while (csp < csend && *csp < itemno)
		    *itemsetend++ = *csp++;
		  *itemsetend++ = itemno;
		}

	      ruleno++;
	    }
	}
    }

  while (csp < csend)
    *itemsetend++ = *csp++;

#if DEBUG
  print_closure (n);
#endif
}


void
free_closure (void)
{
  XFREE (itemset);
  XFREE (ruleset);
  XFREE (fderives + ntokens * rulesetsize);
}
