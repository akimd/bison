/* Compute look-ahead criteria for bison,
   Copyright (C) 1984, 1986, 1989, 2000, 2001, 2002
   Free Software Foundation, Inc.

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
#include "bitset.h"
#include "bitsetv.h"
#include "quotearg.h"
#include "symtab.h"
#include "gram.h"
#include "reader.h"
#include "types.h"
#include "LR0.h"
#include "complain.h"
#include "lalr.h"
#include "nullable.h"
#include "derives.h"
#include "getargs.h"

rule_t **LArule = NULL;
bitsetv LA = NULL;
size_t nLA;

static int ngotos;
short *goto_map = NULL;
state_number_t *from_state = NULL;
state_number_t *to_state = NULL;

/* And for the famous F variable, which name is so descriptive that a
   comment is hardly needed.  <grin>.  */
static bitsetv F = NULL;

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
  int height;

  VERTICES[++top] = i;
  INDEX[i] = height = top;

  if (R[i])
    for (j = 0; R[i][j] >= 0; ++j)
      {
	if (INDEX[R[i][j]] == 0)
	  traverse (R[i][j]);

	if (INDEX[i] > INDEX[R[i][j]])
	  INDEX[i] = INDEX[R[i][j]];

	bitset_or (F[i], F[i], F[R[i][j]]);
      }

  if (INDEX[i] == height)
    for (;;)
      {
	j = VERTICES[top--];
	INDEX[j] = infinity;

	if (i == j)
	  break;

	bitset_copy (F[j], F[i]);
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


static void
initialize_LA (void)
{
  state_number_t i;
  int j;
  rule_t **np;

  /* Avoid having to special case 0.  */
  if (!nLA)
    nLA = 1;

  LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);
  LArule = XCALLOC (rule_t *, nLA);
  lookback = XCALLOC (shorts *, nLA);

  np = LArule;
  for (i = 0; i < nstates; i++)
    if (!states[i]->consistent)
      for (j = 0; j < states[i]->reductions->nreds; j++)
	*np++ = &rules[states[i]->reductions->rules[j]];
}


static void
set_goto_map (void)
{
  state_number_t state;
  short *temp_map;

  goto_map = XCALLOC (short, nvars + 1) - ntokens;
  temp_map = XCALLOC (short, nvars + 1) - ntokens;

  ngotos = 0;
  for (state = 0; state < nstates; ++state)
    {
      shifts_t *sp = states[state]->shifts;
      int i;
      for (i = sp->nshifts - 1; i >= 0 && SHIFT_IS_GOTO (sp, i); --i)
	{
	  if (ngotos == SHRT_MAX)
	    fatal (_("too many gotos (max %d)"), SHRT_MAX);

	  ngotos++;
	  goto_map[SHIFT_SYMBOL (sp, i)]++;
	}
    }

  {
    int k = 0;
    int i;
    for (i = ntokens; i < nsyms; i++)
      {
	temp_map[i] = k;
	k += goto_map[i];
      }

    for (i = ntokens; i < nsyms; i++)
      goto_map[i] = temp_map[i];

    goto_map[nsyms] = ngotos;
    temp_map[nsyms] = ngotos;
  }

  from_state = XCALLOC (state_number_t, ngotos);
  to_state = XCALLOC (state_number_t, ngotos);

  for (state = 0; state < nstates; ++state)
    {
      shifts_t *sp = states[state]->shifts;
      int i;
      for (i = sp->nshifts - 1; i >= 0 && SHIFT_IS_GOTO (sp, i); --i)
	{
	  int k = temp_map[SHIFT_SYMBOL (sp, i)]++;
	  from_state[k] = state;
	  to_state[k] = sp->shifts[i];
	}
    }

  XFREE (temp_map + ntokens);
}



/*----------------------------------------------------------.
| Map a state/symbol pair into its numeric representation.  |
`----------------------------------------------------------*/

static int
map_goto (state_number_t state, symbol_number_t symbol)
{
  int high;
  int low;
  int middle;
  state_number_t s;

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

  F = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  for (i = 0; i < ngotos; i++)
    {
      state_number_t stateno = to_state[i];
      shifts_t *sp = states[stateno]->shifts;

      int j;
      for (j = 0; j < sp->nshifts && SHIFT_IS_SHIFT (sp, j); j++)
	bitset_set (F[i], SHIFT_SYMBOL (sp, j));

      for (; j < sp->nshifts; j++)
	{
	  symbol_number_t symbol = SHIFT_SYMBOL (sp, j);
	  if (nullable[symbol])
	    edge[nedges++] = map_goto (stateno, symbol);
	}

      if (nedges)
	{
	  reads[i] = XCALLOC (short, nedges + 1);
	  memcpy (reads[i], edge, nedges * sizeof (edge[0]));
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
add_lookback_edge (state_t *state, int ruleno, int gotono)
{
  int i;
  shorts *sp;

  for (i = 0; i < state->nlookaheads; ++i)
    if (state->lookaheads_rule[i]->number == ruleno)
      break;

  assert (state->lookaheads_rule[i]->number == ruleno);

  sp = XCALLOC (shorts, 1);
  sp->next = lookback[(state->lookaheads - LA) + i];
  sp->value = gotono;
  lookback[(state->lookaheads - LA) + i] = sp;
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
  state_number_t *states1 = XCALLOC (state_number_t, ritem_longest_rhs () + 1);
  int i;

  includes = XCALLOC (short *, ngotos);

  for (i = 0; i < ngotos; i++)
    {
      int nedges = 0;
      symbol_number_t symbol1 = states[to_state[i]]->accessing_symbol;
      short *rulep;

      for (rulep = derives[symbol1]; *rulep > 0; rulep++)
	{
	  int done;
	  int length = 1;
	  item_number_t *rp;
	  state_t *state = states[from_state[i]];
	  states1[0] = state->number;

	  for (rp = rules[*rulep].rhs; *rp >= 0; rp++)
	    {
	      shifts_t *sp = state->shifts;
	      int j;
	      for (j = 0; j < sp->nshifts; j++)
		{
		  state = states[sp->shifts[j]];
		  if (state->accessing_symbol
		      == item_number_as_symbol_number (*rp))
		    break;
		}

	      states1[length++] = state->number;
	    }

	  if (!state->consistent)
	    add_lookback_edge (state, *rulep, i);

	  length--;
	  done = 0;
	  while (!done)
	    {
	      done = 1;
	      rp--;
	      /* JF added rp>=ritem &&   I hope to god its right! */
	      if (rp >= ritem && ISVAR (*rp))
		{
		  /* Downcasting from item_number_t to symbol_number_t. */
		  edge[nedges++] = map_goto (states1[--length],
					     item_number_as_symbol_number (*rp));
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
  XFREE (states1);

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
  size_t i;
  shorts *sp;

  for (i = 0; i < nLA; i++)
    for (sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], F[sp->value]);

  /* Free LOOKBACK. */
  for (i = 0; i < nLA; i++)
    LIST_FREE (shorts, lookback[i]);

  XFREE (lookback);
  bitsetv_free (F);
}


/*-------------------------------------------------------------.
| Count the number of lookaheads required for each state       |
| (NLOOKAHEADS member).  Compute the total number of LA, NLA.  |
`-------------------------------------------------------------*/

static void
states_lookaheads_count (void)
{
  state_number_t i;
  nLA = 0;

  /* Count   */
  for (i = 0; i < nstates; i++)
    {
      int k;
      int nlookaheads = 0;
      reductions *rp = states[i]->reductions;
      shifts_t *sp = states[i]->shifts;

      /* We need a lookahead either to distinguish different
	 reductions (i.e., there are two or more), or to distinguish a
	 reduction from a shift.  Otherwise, it is straightforward,
	 and the state is `consistent'.  */
      if (rp->nreds > 1
	  || (rp->nreds == 1 && sp->nshifts && SHIFT_IS_SHIFT (sp, 0)))
	nlookaheads += rp->nreds;
      else
	states[i]->consistent = 1;

      for (k = 0; k < sp->nshifts; k++)
	if (SHIFT_IS_ERROR (sp, k))
	  {
	    states[i]->consistent = 0;
	    break;
	  }

      states[i]->nlookaheads = nlookaheads;
      nLA += nlookaheads;
    }
}


/*--------------------------------------.
| Initializing the lookaheads members.  |
`--------------------------------------*/

static void
states_lookaheads_initialize (void)
{
  state_number_t i;
  bitsetv pLA = LA;
  rule_t **pLArule = LArule;

  /* Initialize the members LOOKAHEADS and LOOKAHEADS_RULE for each
     state.  */
  for (i = 0; i < nstates; i++)
    {
      states[i]->lookaheads = pLA;
      states[i]->lookaheads_rule = pLArule;
      pLA += states[i]->nlookaheads;
      pLArule += states[i]->nlookaheads;
    }
}


/*---------------------------------------.
| Output the lookaheads for each state.  |
`---------------------------------------*/

static void
lookaheads_print (FILE *out)
{
  state_number_t i;
  int j, k;
  fprintf (out, "Lookaheads: BEGIN\n");
  for (i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d: %d lookaheads\n",
	       i, states[i]->nlookaheads);

      for (j = 0; j < states[i]->nlookaheads; ++j)
	for (k = 0; k < ntokens; ++k)
	  if (bitset_test (states[i]->lookaheads[j], k))
	    fprintf (out, "   on %d (%s) -> rule %d\n",
		     k, symbol_tag_get (symbols[k]),
		     states[i]->lookaheads_rule[j]->number - 1);
    }
  fprintf (out, "Lookaheads: END\n");
}

void
lalr (void)
{
  states_lookaheads_count ();
  initialize_LA ();
  states_lookaheads_initialize ();
  set_goto_map ();
  initialize_F ();
  build_relations ();
  compute_FOLLOWS ();
  compute_lookaheads ();

  if (trace_flag)
    lookaheads_print (stderr);
}
