/* Generate the nondeterministic finite state machine for bison,
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


/* See comments in state.h for the data structures that represent it.
   The entry point is generate_states.  */

#include "system.h"
#include "symtab.h"
#include "getargs.h"
#include "reader.h"
#include "gram.h"
#include "state.h"
#include "complain.h"
#include "closure.h"
#include "LR0.h"
#include "lalr.h"
#include "reduce.h"

int nstates;
/* Initialize the final state to -1, otherwise, it might be set to 0
   by default, and since we don't compute the reductions of the final
   state, we end up not computing the reductions of the initial state,
   which is of course needed.

   FINAL_STATE is properly set by new_state when it recognizes the
   accessing symbol: EOF.  */
int final_state = -1;
static state_t *first_state = NULL;

static state_t *this_state = NULL;
static state_t *last_state = NULL;

static int nshifts;
static short *shift_symbol = NULL;

static short *redset = NULL;
static short *shiftset = NULL;

static short **kernel_base = NULL;
static int *kernel_size = NULL;
static short *kernel_items = NULL;

/* hash table for states, to recognize equivalent ones.  */

#define	STATE_HASH_SIZE	1009
static state_t **state_hash = NULL;


static void
allocate_itemsets (void)
{
  int i;

  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  int count = 0;
  short *symbol_count = XCALLOC (short, nsyms + nuseless_nonterminals);

  for (i = 0; i < nritems; ++i)
    if (ritem[i] >= 0)
      {
	count++;
	symbol_count[ritem[i]]++;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol cannot be more than the number of times that symbol
     appears as an item, which is symbol_count[symbol].
     We allocate that much space for each symbol.  */

  kernel_base = XCALLOC (short *, nsyms);
  if (count)
    kernel_items = XCALLOC (short, count);

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

  shiftset = XCALLOC (short, nsyms);
  redset = XCALLOC (short, nrules + 1);
  state_hash = XCALLOC (state_t *, STATE_HASH_SIZE);
  shift_symbol = XCALLOC (short, nsyms);
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
  free (state_hash);
}




/*----------------------------------------------------------------.
| Find which symbols can be shifted in the current state, and for |
| each one record which items would be active after that shift.   |
| Uses the contents of itemset.                                   |
|                                                                 |
| shift_symbol is set to a vector of the symbols that can be      |
| shifted.  For each symbol in the grammar, kernel_base[symbol]   |
| points to a vector of item numbers activated if that symbol is  |
| shifted, and kernel_size[symbol] is their numbers.              |
`----------------------------------------------------------------*/

static void
new_itemsets (void)
{
  int i;

  if (trace_flag)
    fprintf (stderr, "Entering new_itemsets, state = %d\n",
	     this_state->number);

  for (i = 0; i < nsyms; i++)
    kernel_size[i] = 0;

  nshifts = 0;

  for (i = 0; i < nitemset; ++i)
    {
      int symbol = ritem[itemset[i]];
      if (symbol >= 0)
	{
	  if (!kernel_size[symbol])
	    {
	      shift_symbol[nshifts] = symbol;
	      nshifts++;
	    }

	  kernel_base[symbol][kernel_size[symbol]] = itemset[i] + 1;
	  kernel_size[symbol]++;
	}
    }
}



/*-----------------------------------------------------------------.
| Subroutine of get_state.  Create a new state for those items, if |
| necessary.                                                       |
`-----------------------------------------------------------------*/

static state_t *
new_state (int symbol)
{
  state_t *p;

  if (trace_flag)
    fprintf (stderr, "Entering new_state, state = %d, symbol = %d (%s)\n",
	     this_state->number, symbol, symbols[symbol]->tag);

  if (nstates >= MAXSHORT)
    fatal (_("too many states (max %d)"), MAXSHORT);

  p = STATE_ALLOC (kernel_size[symbol]);
  p->accessing_symbol = symbol;
  p->number = nstates;
  p->nitems = kernel_size[symbol];

  shortcpy (p->items, kernel_base[symbol], kernel_size[symbol]);

  last_state->next = p;
  last_state = p;
  nstates++;

  /* If this is the eoftoken, then this is the final state. */
  if (symbol == 0)
    final_state = p->number;

  return p;
}


/*--------------------------------------------------------------.
| Find the state number for the state we would get to (from the |
| current state) by shifting symbol.  Create a new state if no  |
| equivalent one exists already.  Used by append_states.        |
`--------------------------------------------------------------*/

static int
get_state (int symbol)
{
  int key;
  int i;
  state_t *sp;

  if (trace_flag)
    fprintf (stderr, "Entering get_state, state = %d, symbol = %d (%s)\n",
	     this_state->number, symbol, symbols[symbol]->tag);

  /* Add up the target state's active item numbers to get a hash key.
     */
  key = 0;
  for (i = 0; i < kernel_size[symbol]; ++i)
    key += kernel_base[symbol][i];
  key = key % STATE_HASH_SIZE;
  sp = state_hash[key];

  if (sp)
    {
      int found = 0;
      while (!found)
	{
	  if (sp->nitems == kernel_size[symbol])
	    {
	      found = 1;
	      for (i = 0; i < kernel_size[symbol]; ++i)
		if (kernel_base[symbol][i] != sp->items[i])
		  found = 0;
	    }

	  if (!found)
	    {
	      if (sp->link)
		{
		  sp = sp->link;
		}
	      else		/* bucket exhausted and no match */
		{
		  sp = sp->link = new_state (symbol);
		  found = 1;
		}
	    }
	}
    }
  else				/* bucket is empty */
    {
      state_hash[key] = sp = new_state (symbol);
    }

  if (trace_flag)
    fprintf (stderr, "Exiting get_state => %d\n", sp->number);

  return sp->number;
}

/*------------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state    |
| numbers reached by each shift transition from the current state.  |
|                                                                   |
| shiftset is set up as a vector of state numbers of those states.  |
`------------------------------------------------------------------*/

static void
append_states (void)
{
  int i;
  int j;
  int symbol;

  if (trace_flag)
    fprintf (stderr, "Entering append_states, state = %d\n",
	     this_state->number);

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
    shiftset[i] = get_state (shift_symbol[i]);
}


static void
new_states (void)
{
  first_state = last_state = this_state = STATE_ALLOC (0);
  nstates = 1;
}


/*------------------------------------------------------------.
| Save the NSHIFTS of SHIFTSET into the current linked list.  |
`------------------------------------------------------------*/

static void
save_shifts (void)
{
  shifts *p = shifts_new (nshifts);
  shortcpy (p->shifts, shiftset, nshifts);
  this_state->shifts = p;
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

static void
save_reductions (void)
{
  int count = 0;
  int i;

  /* If this is the final state, we want it to have no reductions at
     all, although it has one for `START_SYMBOL EOF .'.  */
  if (this_state->number == final_state)
    return;

  /* Find and count the active items that represent ends of rules. */
  for (i = 0; i < nitemset; ++i)
    {
      int item = ritem[itemset[i]];
      if (item < 0)
	redset[count++] = -item;
    }

  /* Make a reductions structure and copy the data into it.  */
  this_state->reductions = reductions_new (count);
  shortcpy (this_state->reductions->rules, redset, count);
}


/*---------------.
| Build STATES.  |
`---------------*/

static void
set_states (void)
{
  state_t *sp;
  states = XCALLOC (state_t *, nstates);

  for (sp = first_state; sp; sp = sp->next)
    {
      /* Pessimization, but simplification of the code: make sure all
	 the states have a shifts, errs, and reductions, even if
	 reduced to 0.  */
      if (!sp->shifts)
	sp->shifts = shifts_new (0);
      if (!sp->errs)
	sp->errs = errs_new (0);
      if (!sp->reductions)
	sp->reductions = reductions_new (0);

      states[sp->number] = sp;
    }
}

/*-------------------------------------------------------------------.
| Compute the nondeterministic finite state machine (see state.h for |
| details) from the grammar.                                         |
`-------------------------------------------------------------------*/

void
generate_states (void)
{
  allocate_storage ();
  new_closure (nritems);
  new_states ();

  while (this_state)
    {
      if (trace_flag)
	fprintf (stderr, "Processing state %d (reached by %s)\n",
		 this_state->number,
		 symbols[this_state->accessing_symbol]->tag);
      /* Set up ruleset and itemset for the transitions out of this
         state.  ruleset gets a 1 bit for each rule that could reduce
         now.  itemset gets a vector of all the items that could be
         accepted next.  */
      closure (this_state->items, this_state->nitems);
      /* record the reductions allowed out of this state */
      save_reductions ();
      /* find the itemsets of the states that shifts can reach */
      new_itemsets ();
      /* find or create the core structures for those states */
      append_states ();

      /* create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known */
      save_shifts ();

      /* states are queued when they are created; process them all */
      this_state = this_state->next;
    }

  /* discard various storage */
  free_closure ();
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
