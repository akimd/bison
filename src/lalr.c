/* Compute look-ahead criteria for bison,
   Copyright 1984, 1986, 1989, 2000 Free Software Foundation, Inc.

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


/* Compute how to make the finite state machine deterministic; find
   which rules need lookahead in each state, and which lookahead
   tokens they accept.  */

#include "system.h"
#include "types.h"
#include "LR0.h"
#include "gram.h"
#include "complain.h"
#include "lalr.h"
#include "nullable.h"
#include "derives.h"

/* All the decorated states, indexed by the state number.  Warning:
   there is a state_TABLE in LR0.c, but it is different and static.
   */
state_t *state_table = NULL;

int tokensetsize;
short *LAruleno;
unsigned *LA;

short *goto_map;
short *from_state;
short *to_state;

extern void berror PARAMS ((const char *));

static int infinity;
static int maxrhs;
static int ngotos;
static unsigned *F;
static short **includes;
static shorts **lookback;
static short **R;
static short *INDEX;
static short *VERTICES;
static int top;


static void
traverse (int i)
{
  unsigned *fp1;
  unsigned *fp2;
  unsigned *fp3;
  int j;
  short *rp;

  int height;
  unsigned *base;

  VERTICES[++top] = i;
  INDEX[i] = height = top;

  base = F + i * tokensetsize;
  fp3 = base + tokensetsize;

  rp = R[i];
  if (rp)
    {
      while ((j = *rp++) >= 0)
	{
	  if (INDEX[j] == 0)
	    traverse (j);

	  if (INDEX[i] > INDEX[j])
	    INDEX[i] = INDEX[j];

	  fp1 = base;
	  fp2 = F + j * tokensetsize;

	  while (fp1 < fp3)
	    *fp1++ |= *fp2++;
	}
    }

  if (INDEX[i] == height)
    {
      for (;;)
	{
	  j = VERTICES[top--];
	  INDEX[j] = infinity;

	  if (i == j)
	    break;

	  fp1 = base;
	  fp2 = F + j * tokensetsize;

	  while (fp1 < fp3)
	    *fp2++ = *fp1++;
	}
    }
}


static void
digraph (short **relation)
{
  int i;

  infinity = ngotos + 2;
  INDEX = XCALLOC (short, ngotos + 1);
  VERTICES = XCALLOC (short, ngotos + 1);
  top = 0;

  R = relation;

  for (i = 0; i < ngotos; i++)
    INDEX[i] = 0;

  for (i = 0; i < ngotos; i++)
    {
      if (INDEX[i] == 0 && R[i])
	traverse (i);
    }

  XFREE (INDEX);
  XFREE (VERTICES);
}

static void
set_state_table (void)
{
  /* NSTATES + 1 because lookahead for the pseudo state number NSTATES
     might be used (see conflicts.c).  It is too opaque for me to
     provide a probably less hacky implementation. --akim */
  state_table = XCALLOC (state_t, nstates + 1);

  {
    core *sp;
    for (sp = first_state; sp; sp = sp->next)
      {
	state_table[sp->number].state = sp;
	state_table[sp->number].accessing_symbol = sp->accessing_symbol;
      }
  }

  {
    shifts *sp;
    for (sp = first_shift; sp; sp = sp->next)
      state_table[sp->number].shift_table = sp;
  }

  {
    reductions *rp;
    for (rp = first_reduction; rp; rp = rp->next)
      state_table[rp->number].reduction_table = rp;
  }
}


static void
set_maxrhs (void)
{
  short *itemp;
  int length;
  int max;

  length = 0;
  max = 0;
  for (itemp = ritem; *itemp; itemp++)
    {
      if (*itemp > 0)
	{
	  length++;
	}
      else
	{
	  if (length > max)
	    max = length;
	  length = 0;
	}
    }

  maxrhs = max;
}


static void
initialize_LA (void)
{
  int i;
  int j;
  int count = 0;
  reductions *rp;
  shifts *sp;
  short *np;

  for (i = 0; i < nstates; i++)
    {
      int k;

      state_table[i].lookaheads = count;

      rp = state_table[i].reduction_table;
      sp = state_table[i].shift_table;
      if (rp
	  && (rp->nreds > 1
	      || (sp && !ISVAR (state_table[sp->shifts[0]].accessing_symbol))))
	count += rp->nreds;
      else
	state_table[i].consistent = 1;

      if (sp)
	for (k = 0; k < sp->nshifts; k++)
	  if (state_table[sp->shifts[k]].accessing_symbol
	      == error_token_number)
	    {
	      state_table[i].consistent = 0;
	      break;
	    }
    }

  state_table[nstates].lookaheads = count;

  if (count == 0)
    {
      LA = XCALLOC (unsigned, 1 * tokensetsize);
      LAruleno = XCALLOC (short, 1);
      lookback = XCALLOC (shorts *, 1);
    }
  else
    {
      LA = XCALLOC (unsigned, count * tokensetsize);
      LAruleno = XCALLOC (short, count);
      lookback = XCALLOC (shorts *, count);
    }

  np = LAruleno;
  for (i = 0; i < nstates; i++)
    {
      if (!state_table[i].consistent)
	{
	  if ((rp = state_table[i].reduction_table))
	    for (j = 0; j < rp->nreds; j++)
	      *np++ = rp->rules[j];
	}
    }
}


static void
set_goto_map (void)
{
  shifts *sp;
  int i;
  int symbol;
  int k;
  short *temp_map;
  int state2;
  int state1;

  goto_map = XCALLOC (short, nvars + 1) - ntokens;
  temp_map = XCALLOC (short, nvars + 1) - ntokens;

  ngotos = 0;
  for (sp = first_shift; sp; sp = sp->next)
    {
      for (i = sp->nshifts - 1; i >= 0; i--)
	{
	  symbol = state_table[sp->shifts[i]].accessing_symbol;

	  if (ISTOKEN (symbol))
	    break;

	  if (ngotos == MAXSHORT)
	    fatal (_("too many gotos (max %d)"), MAXSHORT);

	  ngotos++;
	  goto_map[symbol]++;
	}
    }

  k = 0;
  for (i = ntokens; i < nsyms; i++)
    {
      temp_map[i] = k;
      k += goto_map[i];
    }

  for (i = ntokens; i < nsyms; i++)
    goto_map[i] = temp_map[i];

  goto_map[nsyms] = ngotos;
  temp_map[nsyms] = ngotos;

  from_state = XCALLOC (short, ngotos);
  to_state = XCALLOC (short, ngotos);

  for (sp = first_shift; sp; sp = sp->next)
    {
      state1 = sp->number;
      for (i = sp->nshifts - 1; i >= 0; i--)
	{
	  state2 = sp->shifts[i];
	  symbol = state_table[state2].accessing_symbol;

	  if (ISTOKEN (symbol))
	    break;

	  k = temp_map[symbol]++;
	  from_state[k] = state1;
	  to_state[k] = state2;
	}
    }

  XFREE (temp_map + ntokens);
}



/*----------------------------------------------------------.
| Map a state/symbol pair into its numeric representation.  |
`----------------------------------------------------------*/

static int
map_goto (int state, int symbol)
{
  int high;
  int low;
  int middle;
  int s;

  low = goto_map[symbol];
  high = goto_map[symbol + 1] - 1;

  while (low <= high)
    {
      middle = (low + high) / 2;
      s = from_state[middle];
      if (s == state)
	return middle;
      else if (s < state)
	low = middle + 1;
      else
	high = middle - 1;
    }

  assert (0);
  /* NOTREACHED */
  return 0;
}


static void
initialize_F (void)
{
  int i;
  int j;
  int k;
  shifts *sp;
  short *edge;
  unsigned *rowp;
  short *rp;
  short **reads;
  int nedges;
  int stateno;
  int symbol;
  int nwords;

  nwords = ngotos * tokensetsize;
  F = XCALLOC (unsigned, nwords);

  reads = XCALLOC (short *, ngotos);
  edge = XCALLOC (short, ngotos + 1);
  nedges = 0;

  rowp = F;
  for (i = 0; i < ngotos; i++)
    {
      stateno = to_state[i];
      sp = state_table[stateno].shift_table;

      if (sp)
	{
	  k = sp->nshifts;

	  for (j = 0; j < k; j++)
	    {
	      symbol = state_table[sp->shifts[j]].accessing_symbol;
	      if (ISVAR (symbol))
		break;
	      SETBIT (rowp, symbol);
	    }

	  for (; j < k; j++)
	    {
	      symbol = state_table[sp->shifts[j]].accessing_symbol;
	      if (nullable[symbol])
		edge[nedges++] = map_goto (stateno, symbol);
	    }

	  if (nedges)
	    {
	      reads[i] = rp = XCALLOC (short, nedges + 1);

	      for (j = 0; j < nedges; j++)
		rp[j] = edge[j];

	      rp[nedges] = -1;
	      nedges = 0;
	    }
	}

      rowp += tokensetsize;
    }

  digraph (reads);

  for (i = 0; i < ngotos; i++)
    {
      if (reads[i])
	XFREE (reads[i]);
    }

  XFREE (reads);
  XFREE (edge);
}


static void
add_lookback_edge (int stateno, int ruleno, int gotono)
{
  int i;
  int k;
  int found;
  shorts *sp;

  i = state_table[stateno].lookaheads;
  k = state_table[stateno + 1].lookaheads;
  found = 0;
  while (!found && i < k)
    {
      if (LAruleno[i] == ruleno)
	found = 1;
      else
	i++;
    }

  assert (found);

  sp = XCALLOC (shorts, 1);
  sp->next = lookback[i];
  sp->value = gotono;
  lookback[i] = sp;
}


static short **
transpose (short **R_arg, int n)
{
  short **new_R;
  short **temp_R;
  short *nedges;
  short *sp;
  int i;
  int k;

  nedges = XCALLOC (short, n);

  for (i = 0; i < n; i++)
    {
      sp = R_arg[i];
      if (sp)
	{
	  while (*sp >= 0)
	    nedges[*sp++]++;
	}
    }

  new_R = XCALLOC (short *, n);
  temp_R = XCALLOC (short *, n);

  for (i = 0; i < n; i++)
    {
      k = nedges[i];
      if (k > 0)
	{
	  sp = XCALLOC (short, k + 1);
	  new_R[i] = sp;
	  temp_R[i] = sp;
	  sp[k] = -1;
	}
    }

  XFREE (nedges);

  for (i = 0; i < n; i++)
    {
      sp = R_arg[i];
      if (sp)
	{
	  while (*sp >= 0)
	    *temp_R[*sp++]++ = i;
	}
    }

  XFREE (temp_R);

  return new_R;
}


static void
build_relations (void)
{
  int i;
  int j;
  int k;
  short *rulep;
  short *rp;
  shifts *sp;
  int length;
  int nedges;
  int done;
  int state1;
  int stateno;
  int symbol1;
  int symbol2;
  short *shortp;
  short *edge;
  short *states;
  short **new_includes;

  includes = XCALLOC (short *, ngotos);
  edge = XCALLOC (short, ngotos + 1);
  states = XCALLOC (short, maxrhs + 1);

  for (i = 0; i < ngotos; i++)
    {
      nedges = 0;
      state1 = from_state[i];
      symbol1 = state_table[to_state[i]].accessing_symbol;

      for (rulep = derives[symbol1]; *rulep > 0; rulep++)
	{
	  length = 1;
	  states[0] = state1;
	  stateno = state1;

	  for (rp = ritem + rrhs[*rulep]; *rp > 0; rp++)
	    {
	      symbol2 = *rp;
	      sp = state_table[stateno].shift_table;
	      k = sp->nshifts;

	      for (j = 0; j < k; j++)
		{
		  stateno = sp->shifts[j];
		  if (state_table[stateno].accessing_symbol == symbol2)
		    break;
		}

	      states[length++] = stateno;
	    }

	  if (!state_table[stateno].consistent)
	    add_lookback_edge (stateno, *rulep, i);

	  length--;
	  done = 0;
	  while (!done)
	    {
	      done = 1;
	      rp--;
	      /* JF added rp>=ritem &&   I hope to god its right! */
	      if (rp >= ritem && ISVAR (*rp))
		{
		  stateno = states[--length];
		  edge[nedges++] = map_goto (stateno, *rp);
		  if (nullable[*rp])
		    done = 0;
		}
	    }
	}

      if (nedges)
	{
	  includes[i] = shortp = XCALLOC (short, nedges + 1);
	  for (j = 0; j < nedges; j++)
	    shortp[j] = edge[j];
	  shortp[nedges] = -1;
	}
    }

  new_includes = transpose (includes, ngotos);

  for (i = 0; i < ngotos; i++)
    if (includes[i])
      XFREE (includes[i]);

  XFREE (includes);

  includes = new_includes;

  XFREE (edge);
  XFREE (states);
}



static void
compute_FOLLOWS (void)
{
  int i;

  digraph (includes);

  for (i = 0; i < ngotos; i++)
    {
      if (includes[i])
	XFREE (includes[i]);
    }

  XFREE (includes);
}


static void
compute_lookaheads (void)
{
  int i;
  int n;
  unsigned *fp1;
  unsigned *fp2;
  unsigned *fp3;
  shorts *sp;
  unsigned *rowp;
  shorts *sptmp;		/* JF */

  rowp = LA;
  for (i = 0; i < state_table[nstates].lookaheads; i++)
    {
      fp3 = rowp + tokensetsize;
      for (sp = lookback[i]; sp; sp = sp->next)
	{
	  fp1 = rowp;
	  fp2 = F + tokensetsize * sp->value;
	  while (fp1 < fp3)
	    *fp1++ |= *fp2++;
	}

      rowp = fp3;
    }

  for (i = 0; i < state_table[nstates].lookaheads; i++)
    {
      /* JF removed ref to freed storage */
      for (sp = lookback[i]; sp; sp = sptmp)
	{
	  sptmp = sp->next;
	  XFREE (sp);
	}
    }

  XFREE (lookback);
  XFREE (F);
}


void
lalr (void)
{
  tokensetsize = WORDSIZE (ntokens);

  set_state_table ();
  set_maxrhs ();
  initialize_LA ();
  set_goto_map ();
  initialize_F ();
  build_relations ();
  compute_FOLLOWS ();
  compute_lookaheads ();
}
