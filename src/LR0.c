/* Generate the nondeterministic finite state machine for bison,

   Copyright (C) 1984, 1986, 1989, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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


/* See comments in state.h for the data structures that represent it.
   The entry point is generate_states.  */

#include "system.h"
#include "bitset.h"
#include "quotearg.h"
#include "symtab.h"
#include "gram.h"
#include "getargs.h"
#include "reader.h"
#include "gram.h"
#include "state.h"
#include "complain.h"
#include "closure.h"
#include "LR0.h"
#include "lalr.h"
#include "reduce.h"

typedef struct state_list_s
{
  struct state_list_s *next;
  state_t *state;
} state_list_t;

static state_list_t *first_state = NULL;
static state_list_t *last_state = NULL;


/*------------------------------------------------------------------.
| A state was just discovered from another state.  Queue it for     |
| later examination, in order to find its transitions.  Return it.  |
`------------------------------------------------------------------*/

static state_t *
state_list_append (symbol_number_t symbol,
		   size_t core_size, item_number_t *core)
{
  state_list_t *node = XMALLOC (state_list_t, 1);
  state_t *state = state_new (symbol, core_size, core);

  if (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
	     nstates, symbol, symbols[symbol]->tag);

  /* If this is the endtoken, and this is not the initial state, then
     this is the final state.  */
  if (symbol == 0 && first_state)
    final_state = state;

  node->next = NULL;
  node->state = state;

  if (!first_state)
    first_state = node;
  if (last_state)
    last_state->next = node;
  last_state = node;

  return state;
}

static int nshifts;
static symbol_number_t *shift_symbol = NULL;

static rule_t **redset = NULL;
static state_t **shiftset = NULL;

static item_number_t **kernel_base = NULL;
static int *kernel_size = NULL;
static item_number_t *kernel_items = NULL;


static void
allocate_itemsets (void)
{
  symbol_number_t i;
  rule_number_t r;
  item_number_t *rhsp;

  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  int count = 0;
  short *symbol_count = XCALLOC (short, nsyms + nuseless_nonterminals);

  for (r = 0; r < nrules; ++r)
    for (rhsp = rules[r].rhs; *rhsp >= 0; ++rhsp)
      {
	count++;
	symbol_count[*rhsp]++;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol cannot be more than the number of times that symbol
     appears as an item, which is SYMBOL_COUNT[SYMBOL].
     We allocate that much space for each symbol.  */

  kernel_base = XCALLOC (item_number_t *, nsyms);
  if (count)
    kernel_items = XCALLOC (item_number_t, count);

  count = 0;
  for (i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_items + count;
      count += symbol_count[i];
    }

  free (symbol_count);
  kernel_size = XCALLOC (int, nsyms);
}


static void
allocate_storage (void)
{
  allocate_itemsets ();

  shiftset = XCALLOC (state_t *, nsyms);
  redset = XCALLOC (rule_t *, nrules);
  state_hash_new ();
  shift_symbol = XCALLOC (symbol_number_t, nsyms);
}


static void
free_storage (void)
{
  free (shift_symbol);
  free (redset);
  free (shiftset);
  free (kernel_base);
  free (kernel_size);
  XFREE (kernel_items);
  state_hash_free ();
}




/*---------------------------------------------------------------.
| Find which symbols can be shifted in STATE, and for each one   |
| record which items would be active after that shift.  Uses the |
| contents of itemset.                                           |
|                                                                |
| shift_symbol is set to a vector of the symbols that can be     |
| shifted.  For each symbol in the grammar, kernel_base[symbol]  |
| points to a vector of item numbers activated if that symbol is |
| shifted, and kernel_size[symbol] is their numbers.             |
`---------------------------------------------------------------*/

static void
new_itemsets (state_t *state)
{
  int i;

  if (trace_flag & trace_automaton)
    fprintf (stderr, "Entering new_itemsets, state = %d\n",
	     state->number);

  for (i = 0; i < nsyms; i++)
    kernel_size[i] = 0;

  nshifts = 0;

  for (i = 0; i < nritemset; ++i)
    if (ritem[itemset[i]] >= 0)
      {
	symbol_number_t symbol
	  = item_number_as_symbol_number (ritem[itemset[i]]);
	if (!kernel_size[symbol])
	  {
	    shift_symbol[nshifts] = symbol;
	    nshifts++;
	  }

	kernel_base[symbol][kernel_size[symbol]] = itemset[i] + 1;
	kernel_size[symbol]++;
      }
}



/*-----------------------------------------------------------------.
| Find the state we would get to (from the current state) by       |
| shifting SYMBOL.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                                 |
`-----------------------------------------------------------------*/

static state_t *
get_state (symbol_number_t symbol, size_t core_size, item_number_t *core)
{
  state_t *sp;

  if (trace_flag & trace_automaton)
    fprintf (stderr, "Entering get_state, symbol = %d (%s)\n",
	     symbol, symbols[symbol]->tag);

  sp = state_hash_lookup (core_size, core);
  if (!sp)
    sp = state_list_append (symbol, core_size, core);

  if (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", sp->number);

  return sp;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from STATE.           |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

static void
append_states (state_t *state)
{
  int i;
  int j;
  symbol_number_t symbol;

  if (trace_flag & trace_automaton)
    fprintf (stderr, "Entering append_states, state = %d\n",
	     state->number);

  /* first sort shift_symbol into increasing order */

  for (i = 1; i < nshifts; i++)
    {
      symbol = shift_symbol[i];
      j = i;
      while (j > 0 && shift_symbol[j - 1] > symbol)
	{
	  shift_symbol[j] = shift_symbol[j - 1];
	  j--;
	}
      shift_symbol[j] = symbol;
    }

  for (i = 0; i < nshifts; i++)
    {
      symbol = shift_symbol[i];
      shiftset[i] = get_state (symbol,
			       kernel_size[symbol], kernel_base[symbol]);
    }
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

static void
save_reductions (state_t *state)
{
  int count = 0;
  int i;

  /* Find and count the active items that represent ends of rules. */
  for (i = 0; i < nritemset; ++i)
    {
      int item = ritem[itemset[i]];
      if (item < 0)
	redset[count++] = &rules[item_number_as_rule_number (item)];
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (state, count, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

static void
set_states (void)
{
  states = XCALLOC (state_t *, nstates);

  while (first_state)
    {
      state_list_t *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
	 the states have valid transitions and reductions members,
	 even if reduced to 0.  It is too soon for errs, which are
	 computed later, but set_conflicts.  */
      state_t *state = this->state;
      if (!state->transitions)
	state_transitions_set (state, 0, 0);
      if (!state->reductions)
	state_reductions_set (state, 0, 0);

      states[state->number] = state;

      first_state = this->next;
      free (this);
    }
  first_state = NULL;
  last_state = NULL;
}


/*-------------------------------------------------------------------.
| Compute the nondeterministic finite state machine (see state.h for |
| details) from the grammar.                                         |
`-------------------------------------------------------------------*/

void
generate_states (void)
{
  state_list_t *list = NULL;
  allocate_storage ();
  new_closure (nritems);

  /* Create the initial state.  The 0 at the lhs is the index of the
     item of this initial rule.  */
  kernel_base[0][0] = 0;
  kernel_size[0] = 1;
  state_list_append (0, kernel_size[0], kernel_base[0]);

  list = first_state;

  while (list)
    {
      state_t *state = list->state;
      if (trace_flag & trace_automaton)
	fprintf (stderr, "Processing state %d (reached by %s)\n",
		 state->number,
		 symbols[state->accessing_symbol]->tag);
      /* Set up ruleset and itemset for the transitions out of this
         state.  ruleset gets a 1 bit for each rule that could reduce
         now.  itemset gets a vector of all the items that could be
         accepted next.  */
      closure (state->items, state->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (state);
      /* Find the itemsets of the states that shifts can reach.  */
      new_itemsets (state);
      /* Find or create the core structures for those states.  */
      append_states (state);

      /* Create the shifts structures for the shifts to those states,
	 now that the state numbers transitioning to are known.  */
      state_transitions_set (state, nshifts, shiftset);

      /* States are queued when they are created; process them all.
	 */
      list = list->next;
    }

  /* discard various storage */
  free_closure ();
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
