/* Compute look-ahead criteria for bison,
   Copyright 1984, 1986, 1989, 2000, 2001  Free Software Foundation, Inc.

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
#include "getargs.h"

/* All the decorated states, indexed by the state number.  Warning:
   there is a state_TABLE in LR0.c, but it is different and static.
   */
state_t *state_table = NULL;

int tokensetsize;
short *LAruleno;
unsigned *LA;

static int ngotos;
short *goto_map;
short *from_state;
short *to_state;

/* And for the famous F variable, which name is so descriptive that a
   comment is hardly needed.  <grin>.  */
static unsigned *F = NULL;
#define F(Rule)  (F + (Rule) * tokensetsize)

static short **includes;
static shorts **lookback;


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

static short **R;
static short *INDEX;
static short *VERTICES;
static int top;
static int infinity;

static void
traverse (int i)
{
  int j;
  size_t k;
  int height;
  size_t size = F (i + 1) - F(i);

  VERTICES[++top] = i;
  INDEX[i] = height = top;

  if (R[i])
    for (j = 0; R[i][j] >= 0; ++j)
      {
	if (INDEX[R[i][j]] == 0)
	  traverse (R[i][j]);

	if (INDEX[i] > INDEX[R[i][j]])
	  INDEX[i] = INDEX[R[i][j]];

	for (k = 0; k < size; ++k)
	  F (i)[k] |= F (R[i][j])[k];
      }

  if (INDEX[i] == height)
    for (;;)
      {
	j = VERTICES[top--];
	INDEX[j] = infinity;

	if (i == j)
	  break;

	for (k = 0; k < size; ++k)
	  F (j)[k] = F (i)[k];
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
    if (INDEX[i] == 0 && R[i])
      traverse (i);

  XFREE (INDEX);
  XFREE (VERTICES);
}


/*--------------------.
| Build STATE_TABLE.  |
`--------------------*/

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
      state_table[sp->number].shifts = sp;
  }

  {
    reductions *rp;
    for (rp = first_reduction; rp; rp = rp->next)
      state_table[rp->number].reductions = rp;
  }

  /* Pessimization, but simplification of the code: make sense all the
     states have a shifts, even if reduced to 0 shifts.  */
  {
    int i;
    for (i = 0; i < nstates; i++)
      if (!state_table[i].shifts)
	state_table[i].shifts = shifts_new (0);
  }

  /* Initializing the lookaheads members.  Please note that it must be
     performed after having set some of the other members which are
     used below.  Change with extreme caution.  */
  {
    int i;
    int count = 0;
    for (i = 0; i < nstates; i++)
      {
	int k;
	reductions *rp = state_table[i].reductions;
	shifts *sp = state_table[i].shifts;

	state_table[i].lookaheads = count;

	if (rp
	    && (rp->nreds > 1 || (sp->nshifts && SHIFT_IS_SHIFT (sp, 0))))
	  count += rp->nreds;
	else
	  state_table[i].consistent = 1;

	for (k = 0; k < sp->nshifts; k++)
	  if (SHIFT_IS_ERROR (sp, k))
	    {
	      state_table[i].consistent = 0;
	      break;
	    }
      }
     state_table[nstates].lookaheads = count;
  }
}


static void
initialize_LA (void)
{
  int i;
  int j;
  short *np;
  reductions *rp;

  size_t nLA = state_table[nstates].lookaheads;
  if (!nLA)
    nLA = 1;

  LA = XCALLOC (unsigned, nLA * tokensetsize);
  LAruleno = XCALLOC (short, nLA);
  lookback = XCALLOC (shorts *, nLA);

  np = LAruleno;
  for (i = 0; i < nstates; i++)
    if (!state_table[i].consistent)
      if ((rp = state_table[i].reductions))
	for (j = 0; j < rp->nreds; j++)
	  *np++ = rp->rules[j];
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
    if (sp->nshifts)
      for (i = sp->nshifts - 1; i >= 0 && SHIFT_IS_GOTO (sp, i); --i)
	{
	  symbol = state_table[sp->shifts[i]].accessing_symbol;

	  if (ngotos == MAXSHORT)
	    fatal (_("too many gotos (max %d)"), MAXSHORT);

	  ngotos++;
	  goto_map[symbol]++;
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
      if (sp->nshifts)
	for (i = sp->nshifts - 1; i >= 0 && SHIFT_IS_GOTO (sp, i); --i)
	  {
	    state2 = sp->shifts[i];
	    symbol = state_table[state2].accessing_symbol;

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
  short **reads = XCALLOC (short *, ngotos);
  short *edge = XCALLOC (short, ngotos + 1);
  int nedges = 0;

  int i;

  F = XCALLOC (unsigned, ngotos * tokensetsize);

  for (i = 0; i < ngotos; i++)
    {
      int stateno = to_state[i];
      shifts *sp = state_table[stateno].shifts;

      int j;
      for (j = 0; j < sp->nshifts && SHIFT_IS_SHIFT (sp, j); j++)
	{
	  int symbol = state_table[sp->shifts[j]].accessing_symbol;
	  SETBIT (F (i), symbol);
	}

      for (; j < sp->nshifts; j++)
	{
	  int symbol = state_table[sp->shifts[j]].accessing_symbol;
	  if (nullable[symbol])
	    edge[nedges++] = map_goto (stateno, symbol);
	}

      if (nedges)
	{
	  reads[i] = XCALLOC (short, nedges + 1);
	  shortcpy (reads[i], edge, nedges);
	  reads[i][nedges] = -1;
	  nedges = 0;
	}
    }

  digraph (reads);

  for (i = 0; i < ngotos; i++)
    XFREE (reads[i]);

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


static void
matrix_print (FILE *out, short **matrix, int n)
{
  int i, j;

  for (i = 0; i < n; ++i)
    {
      fprintf (out, "%3d: ", i);
      if (matrix[i])
	for (j = 0; matrix[i][j] != -1; ++j)
	  fprintf (out, "%3d ", matrix[i][j]);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/*-------------------------------------------------------------------.
| Return the transpose of R_ARG, of size N.  Destroy R_ARG, as it is |
| replaced with the result.                                          |
|                                                                    |
| R_ARG[I] is NULL or a -1 terminated list of numbers.               |
|                                                                    |
| RESULT[NUM] is NULL or the -1 terminated list of the I such as NUM |
| is in R_ARG[I].                                                    |
`-------------------------------------------------------------------*/

static short **
transpose (short **R_arg, int n)
{
  /* The result. */
  short **new_R = XCALLOC (short *, n);
  /* END_R[I] -- next entry of NEW_R[I]. */
  short **end_R = XCALLOC (short *, n);
  /* NEDGES[I] -- total size of NEW_R[I]. */
  short *nedges = XCALLOC (short, n);
  int i, j;

  if (trace_flag)
    {
      fputs ("transpose: input\n", stderr);
      matrix_print (stderr, R_arg, n);
    }

  /* Count. */
  for (i = 0; i < n; i++)
    if (R_arg[i])
      for (j = 0; R_arg[i][j] >= 0; ++j)
	++nedges[R_arg[i][j]];

  /* Allocate. */
  for (i = 0; i < n; i++)
    if (nedges[i] > 0)
      {
	short *sp = XCALLOC (short, nedges[i] + 1);
	sp[nedges[i]] = -1;
	new_R[i] = sp;
	end_R[i] = sp;
      }

  /* Store. */
  for (i = 0; i < n; i++)
    if (R_arg[i])
      for (j = 0; R_arg[i][j] >= 0; ++j)
	{
	  *end_R[R_arg[i][j]] = i;
	  ++end_R[R_arg[i][j]];
	}

  free (nedges);
  free (end_R);

  /* Free the input: it is replaced with the result. */
  for (i = 0; i < n; i++)
    XFREE (R_arg[i]);
  free (R_arg);

  if (trace_flag)
    {
      fputs ("transpose: output\n", stderr);
      matrix_print (stderr, new_R, n);
    }

  return new_R;
}


static void
build_relations (void)
{
  short *edge = XCALLOC (short, ngotos + 1);
  short *states = XCALLOC (short, ritem_longest_rhs () + 1);
  int i;

  includes = XCALLOC (short *, ngotos);

  for (i = 0; i < ngotos; i++)
    {
      int nedges = 0;
      int state1 = from_state[i];
      int symbol1 = state_table[to_state[i]].accessing_symbol;
      short *rulep;

      for (rulep = derives[symbol1]; *rulep > 0; rulep++)
	{
	  int done;
	  int length = 1;
	  int stateno = state1;
	  short *rp;
	  states[0] = state1;

	  for (rp = ritem + rule_table[*rulep].rhs; *rp > 0; rp++)
	    {
	      shifts *sp = state_table[stateno].shifts;
	      int j;
	      for (j = 0; j < sp->nshifts; j++)
		{
		  stateno = sp->shifts[j];
		  if (state_table[stateno].accessing_symbol == *rp)
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
	  int j;
	  includes[i] = XCALLOC (short, nedges + 1);
	  for (j = 0; j < nedges; j++)
	    includes[i][j] = edge[j];
	  includes[i][nedges] = -1;
	}
    }

  XFREE (edge);
  XFREE (states);

  includes = transpose (includes, ngotos);
}



static void
compute_FOLLOWS (void)
{
  int i;

  digraph (includes);

  for (i = 0; i < ngotos; i++)
    XFREE (includes[i]);

  XFREE (includes);
}


static void
compute_lookaheads (void)
{
  int i;
  shorts *sp;

  for (i = 0; i < state_table[nstates].lookaheads; i++)
    for (sp = lookback[i]; sp; sp = sp->next)
      {
	int size = LA (i + 1) - LA (i);
	int j;
	for (j = 0; j < size; ++j)
	  LA (i)[j] |= F (sp->value)[j];
      }

  /* Free LOOKBACK. */
  for (i = 0; i < state_table[nstates].lookaheads; i++)
    LIST_FREE (shorts, lookback[i]);

  XFREE (lookback);
  XFREE (F);
}


void
lalr (void)
{
  tokensetsize = WORDSIZE (ntokens);

  set_state_table ();
  initialize_LA ();
  set_goto_map ();
  initialize_F ();
  build_relations ();
  compute_FOLLOWS ();
  compute_lookaheads ();
}
