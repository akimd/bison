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
#include "relation.h"
#include "quotearg.h"
#include "symtab.h"
#include "gram.h"
#include "reader.h"
#include "LR0.h"
#include "complain.h"
#include "lalr.h"
#include "nullable.h"
#include "derives.h"
#include "getargs.h"

goto_number_t *goto_map = NULL;
static goto_number_t ngotos = 0;
state_number_t *from_state = NULL;
state_number_t *to_state = NULL;

/* Linked list of goto numbers.  */
typedef struct goto_list_s
{
  struct goto_list_s *next;
  goto_number_t value;
} goto_list_t;


/* LA is a LR by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

static bitsetv LA = NULL;
size_t nLA;


/* And for the famous F variable, which name is so descriptive that a
   comment is hardly needed.  <grin>.  */
static bitsetv F = NULL;

static goto_number_t **includes;
static goto_list_t **lookback;




static void
set_goto_map (void)
{
  state_number_t state;
  goto_number_t *temp_map;

  goto_map = XCALLOC (goto_number_t, nvars + 1) - ntokens;
  temp_map = XCALLOC (goto_number_t, nvars + 1) - ntokens;

  ngotos = 0;
  for (state = 0; state < nstates; ++state)
    {
      transitions_t *sp = states[state]->transitions;
      int i;
      for (i = sp->num - 1; i >= 0 && TRANSITION_IS_GOTO (sp, i); --i)
	{
	  if (ngotos == GOTO_NUMBER_MAX)
	    fatal (_("too many gotos (max %d)"), GOTO_NUMBER_MAX);

	  ngotos++;
	  goto_map[TRANSITION_SYMBOL (sp, i)]++;
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
      transitions_t *sp = states[state]->transitions;
      int i;
      for (i = sp->num - 1; i >= 0 && TRANSITION_IS_GOTO (sp, i); --i)
	{
	  int k = temp_map[TRANSITION_SYMBOL (sp, i)]++;
	  from_state[k] = state;
	  to_state[k] = sp->states[i]->number;
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
  goto_number_t **reads = XCALLOC (goto_number_t *, ngotos);
  goto_number_t *edge = XCALLOC (goto_number_t, ngotos + 1);
  int nedges = 0;

  int i;

  F = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  for (i = 0; i < ngotos; i++)
    {
      state_number_t stateno = to_state[i];
      transitions_t *sp = states[stateno]->transitions;

      int j;
      FOR_EACH_SHIFT (sp, j)
	bitset_set (F[i], TRANSITION_SYMBOL (sp, j));

      for (; j < sp->num; j++)
	{
	  symbol_number_t symbol = TRANSITION_SYMBOL (sp, j);
	  if (nullable[symbol])
	    edge[nedges++] = map_goto (stateno, symbol);
	}

      if (nedges)
	{
	  reads[i] = XCALLOC (goto_number_t, nedges + 1);
	  memcpy (reads[i], edge, nedges * sizeof (edge[0]));
	  reads[i][nedges] = -1;
	  nedges = 0;
	}
    }

  relation_digraph (reads, ngotos, &F);

  for (i = 0; i < ngotos; i++)
    XFREE (reads[i]);

  XFREE (reads);
  XFREE (edge);
}


static void
add_lookback_edge (state_t *state, rule_t *rule, int gotono)
{
  int r = state_reduction_find (state, rule);
  goto_list_t *sp = XCALLOC (goto_list_t, 1);
  sp->next = lookback[(state->reductions->lookaheads - LA) + r];
  sp->value = gotono;
  lookback[(state->reductions->lookaheads - LA) + r] = sp;
}



static void
build_relations (void)
{
  goto_number_t *edge = XCALLOC (goto_number_t, ngotos + 1);
  state_number_t *states1 = XCALLOC (state_number_t, ritem_longest_rhs () + 1);
  int i;

  includes = XCALLOC (goto_number_t *, ngotos);

  for (i = 0; i < ngotos; i++)
    {
      int nedges = 0;
      symbol_number_t symbol1 = states[to_state[i]]->accessing_symbol;
      rule_t **rulep;

      for (rulep = derives[symbol1]; *rulep; rulep++)
	{
	  int done;
	  int length = 1;
	  item_number_t *rp;
	  state_t *state = states[from_state[i]];
	  states1[0] = state->number;

	  for (rp = (*rulep)->rhs; *rp >= 0; rp++)
	    {
	      state = transitions_to (state->transitions,
				      item_number_as_symbol_number (*rp));
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
	  includes[i] = XCALLOC (goto_number_t, nedges + 1);
	  for (j = 0; j < nedges; j++)
	    includes[i][j] = edge[j];
	  includes[i][nedges] = -1;
	}
    }

  XFREE (edge);
  XFREE (states1);

  relation_transpose (&includes, ngotos);
}



static void
compute_FOLLOWS (void)
{
  int i;

  relation_digraph (includes, ngotos, &F);

  for (i = 0; i < ngotos; i++)
    XFREE (includes[i]);

  XFREE (includes);
}


static void
compute_lookaheads (void)
{
  size_t i;
  goto_list_t *sp;

  for (i = 0; i < nLA; i++)
    for (sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], F[sp->value]);

  /* Free LOOKBACK. */
  for (i = 0; i < nLA; i++)
    LIST_FREE (goto_list_t, lookback[i]);

  XFREE (lookback);
  bitsetv_free (F);
}


/*---------------------------------------------------------------.
| Count the number of lookaheads required for STATE (NLOOKAHEADS |
| member).                                                       |
`---------------------------------------------------------------*/

static int
state_lookaheads_count (state_t *state)
{
  int k;
  int nlookaheads = 0;
  reductions_t *rp = state->reductions;
  transitions_t *sp = state->transitions;

  /* We need a lookahead either to distinguish different
     reductions (i.e., there are two or more), or to distinguish a
     reduction from a shift.  Otherwise, it is straightforward,
     and the state is `consistent'.  */
  if (rp->num > 1
      || (rp->num == 1 && sp->num &&
	  !TRANSITION_IS_DISABLED (sp, 0) && TRANSITION_IS_SHIFT (sp, 0)))
    nlookaheads += rp->num;
  else
    state->consistent = 1;

  for (k = 0; k < sp->num; k++)
    if (!TRANSITION_IS_DISABLED (sp, k) && TRANSITION_IS_ERROR (sp, k))
      {
	state->consistent = 0;
	break;
      }

  return nlookaheads;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

static void
initialize_LA (void)
{
  state_number_t i;
  bitsetv pLA;

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  for (i = 0; i < nstates; i++)
    nLA += state_lookaheads_count (states[i]);
  /* Avoid having to special case 0.  */
  if (!nLA)
    nLA = 1;

  pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);
  lookback = XCALLOC (goto_list_t *, nLA);

  /* Initialize the members LOOKAHEADS for each state which reductions
     require lookaheads.  */
  for (i = 0; i < nstates; i++)
    {
      int count = state_lookaheads_count (states[i]);
      if (count)
	{
	  states[i]->reductions->lookaheads = pLA;
	  pLA += count;
	}
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
      reductions_t *reds = states[i]->reductions;
      bitset_iterator iter;
      int nlookaheads = 0;

      if (reds->lookaheads)
	for (k = 0; k < reds->num; ++k)
	  if (reds->lookaheads[k])
	    ++nlookaheads;

      fprintf (out, "State %d: %d lookaheads\n",
	       i, nlookaheads);

      if (reds->lookaheads)
	for (j = 0; j < reds->num; ++j)
	  BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
	  {
	    fprintf (out, "   on %d (%s) -> rule %d\n",
		     k, symbols[k]->tag,
		     reds->rules[j]->number);
	  };
    }
  fprintf (out, "Lookaheads: END\n");
}

void
lalr (void)
{
  initialize_LA ();
  set_goto_map ();
  initialize_F ();
  build_relations ();
  compute_FOLLOWS ();
  compute_lookaheads ();

  if (trace_flag & trace_sets)
    lookaheads_print (stderr);
}


void
lalr_free (void)
{
  state_number_t s;
  for (s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = NULL;
  bitsetv_free (LA);
}
