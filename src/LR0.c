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
#include "getargs.h"
#include "reader.h"
#include "gram.h"
#include "state.h"
#include "complain.h"
#include "closure.h"
#include "LR0.h"
#include "reduce.h"

int nstates;
int final_state;
core *first_state = NULL;
shifts *first_shift = NULL;
reductions *first_reduction = NULL;

static core *this_state = NULL;
static core *last_state = NULL;
static shifts *last_shift = NULL;
static reductions *last_reduction = NULL;

static int nshifts;
static short *shift_symbol = NULL;

static short *redset = NULL;
static short *shiftset = NULL;

static short **kernel_base = NULL;
static int *kernel_size = NULL;
static short *kernel_items = NULL;

/* hash table for states, to recognize equivalent ones.  */

#define	STATE_TABLE_SIZE	1009
static core **state_table = NULL;


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

  for (i = 0; ritem[i]; ++i)
    if (ritem[i] > 0)
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
  state_table = XCALLOC (core *, STATE_TABLE_SIZE);
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
  free (state_table);
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

  shift_symbol = XCALLOC (short, nsyms);
  nshifts = 0;

  for (i = 0; i < nitemset; ++i)
    {
      int symbol = ritem[itemset[i]];
      if (symbol > 0)
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

static core *
new_state (int symbol)
{
  core *p;

  if (trace_flag)
    fprintf (stderr, "Entering new_state, state = %d, symbol = %d (%s)\n",
	     this_state->number, symbol, tags[symbol]);

  if (nstates >= MAXSHORT)
    fatal (_("too many states (max %d)"), MAXSHORT);

  p = CORE_ALLOC (kernel_size[symbol]);
  p->accessing_symbol = symbol;
  p->number = nstates;
  p->nitems = kernel_size[symbol];

  shortcpy (p->items, kernel_base[symbol], kernel_size[symbol]);

  last_state->next = p;
  last_state = p;
  nstates++;

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
  core *sp;

  if (trace_flag)
    fprintf (stderr, "Entering get_state, state = %d, symbol = %d (%s)\n",
	     this_state->number, symbol, tags[symbol]);

  /* Add up the target state's active item numbers to get a hash key.
     */
  key = 0;
  for (i = 0; i < kernel_size[symbol]; ++i)
    key += kernel_base[symbol][i];
  key = key % STATE_TABLE_SIZE;
  sp = state_table[key];

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
      state_table[key] = sp = new_state (symbol);
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
  first_state = last_state = this_state = CORE_ALLOC (0);
  nstates = 1;
}


/*------------------------------------------------------------.
| Save the NSHIFTS of SHIFTSET into the current linked list.  |
`------------------------------------------------------------*/

static void
save_shifts (void)
{
  shifts *p = shifts_new (nshifts);

  p->number = this_state->number;

  shortcpy (p->shifts, shiftset, nshifts);

  if (last_shift)
    last_shift->next = p;
  else
    first_shift = p;
  last_shift = p;
}


/*------------------------------------------------------------------.
| Subroutine of augment_automaton.  Create the next-to-final state, |
| to which a shift has already been made in the initial state.      |
`------------------------------------------------------------------*/

static void
insert_start_shift (void)
{
  core *statep;
  shifts *sp;

  statep = CORE_ALLOC (0);
  statep->number = nstates;
  statep->accessing_symbol = start_symbol;

  last_state->next = statep;
  last_state = statep;

  /* Make a shift from this state to (what will be) the final state.  */
  sp = shifts_new (1);
  sp->number = nstates++;
  sp->shifts[0] = nstates;

  last_shift->next = sp;
  last_shift = sp;
}


/*------------------------------------------------------------------.
| Make sure that the initial state has a shift that accepts the     |
| grammar's start symbol and goes to the next-to-final state, which |
| has a shift going to the final state, which has a shift to the    |
| termination state.  Create such states and shifts if they don't   |
| happen to exist already.                                          |
`------------------------------------------------------------------*/

static void
augment_automaton (void)
{
  core *statep;
  shifts *sp;
  shifts *sp1 = NULL;

  sp = first_shift;

  if (!sp->nshifts)
    {
      /* There are no shifts for any state.  Make one shift, from the
	 initial state to the next-to-final state.  */

      sp = shifts_new (1);
      sp->shifts[0] = nstates;

      /* Initialize the chain of shifts with sp.  */
      first_shift = sp;
      last_shift = sp;

      /* Create the next-to-final state, with shift to
         what will be the final state.  */
      insert_start_shift ();
    }
  else if (sp->number == 0)
    {
      statep = first_state->next;

      /* The states reached by shifts from FIRST_STATE are numbered
	 1..(SP->NSHIFTS).  Look for one reached by START_SYMBOL.  */
      while (statep->accessing_symbol < start_symbol
	     && statep->number < sp->nshifts)
	statep = statep->next;

      if (statep->accessing_symbol == start_symbol)
	{
	  /* We already have a next-to-final state.
	     Make sure it has a shift to what will be the final state.  */
	  while (sp && sp->number < statep->number)
	    {
	      sp1 = sp;
	      sp = sp->next;
	    }

	  if (sp && sp->number == statep->number)
	    {
	      int i;
	      shifts *sp2 = shifts_new (sp->nshifts + 1);
	      sp2->number = statep->number;
	      sp2->shifts[0] = nstates;
	      for (i = sp->nshifts; i > 0; i--)
		sp2->shifts[i] = sp->shifts[i - 1];

	      /* Patch sp2 into the chain of shifts in place of sp,
		 following sp1.  */
	      sp2->next = sp->next;
	      sp1->next = sp2;
	      if (sp == last_shift)
		last_shift = sp2;
	      XFREE (sp);
	    }
	  else
	    {
	      shifts *sp2 = shifts_new (1);
	      sp2->number = statep->number;
	      sp2->shifts[0] = nstates;

	      /* Patch sp2 into the chain of shifts between sp1 and sp.  */
	      sp2->next = sp;
	      sp1->next = sp2;
	      if (sp == 0)
		last_shift = sp2;
	    }
	}
      else
	{
	  int i, k;
	  shifts *sp2;

	  /* There is no next-to-final state as yet.  */
	  /* Add one more shift in first_shift,
	     going to the next-to-final state (yet to be made).  */
	  sp = first_shift;

	  sp2 = shifts_new (sp->nshifts + 1);

	  /* Stick this shift into the vector at the proper place.  */
	  statep = first_state->next;
	  for (k = 0, i = 0; i < sp->nshifts; k++, i++)
	    {
	      if (statep->accessing_symbol > start_symbol && i == k)
		sp2->shifts[k++] = nstates;
	      sp2->shifts[k] = sp->shifts[i];
	      statep = statep->next;
	    }
	  if (i == k)
	    sp2->shifts[k++] = nstates;

	  /* Patch sp2 into the chain of shifts
	     in place of sp, at the beginning.  */
	  sp2->next = sp->next;
	  first_shift = sp2;
	  if (last_shift == sp)
	    last_shift = sp2;

	  XFREE (sp);

	  /* Create the next-to-final state, with shift to
	     what will be the final state.  */
	  insert_start_shift ();
	}
    }
  else
    {
      /* The initial state didn't even have any shifts.
	 Give it one shift, to the next-to-final state.  */
      sp = shifts_new (1);
      sp->shifts[0] = nstates;

      /* Patch sp into the chain of shifts at the beginning.  */
      sp->next = first_shift;
      first_shift = sp;

      /* Create the next-to-final state, with shift to
	 what will be the final state.  */
      insert_start_shift ();
    }

  /* Make the final state--the one that follows a shift from the
     next-to-final state.
     The symbol for that shift is 0 (end-of-file).  */
  statep = CORE_ALLOC (0);
  statep->number = nstates;
  last_state->next = statep;
  last_state = statep;

  /* Make the shift from the final state to the termination state.  */
  sp = shifts_new (1);
  sp->number = nstates++;
  sp->shifts[0] = nstates;
  last_shift->next = sp;
  last_shift = sp;

  /* Note that the variable `final_state' refers to what we sometimes call
     the termination state.  */
  final_state = nstates;

  /* Make the termination state.  */
  statep = CORE_ALLOC (0);
  statep->number = nstates++;
  last_state->next = statep;
  last_state = statep;
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

static void
save_reductions (void)
{
  int count;
  int i;

  /* Find and count the active items that represent ends of rules. */

  count = 0;
  for (i = 0; i < nitemset; ++i)
    {
      int item = ritem[itemset[i]];
      if (item < 0)
	redset[count++] = -item;
    }

  /* Make a reductions structure and copy the data into it.  */

  if (count)
    {
      reductions *p = REDUCTIONS_ALLOC (count);

      p->number = this_state->number;
      p->nreds = count;

      shortcpy (p->rules, redset, count);

      if (last_reduction)
	last_reduction->next = p;
      else
	first_reduction = p;
      last_reduction = p;
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
  new_closure (nitems);
  new_states ();

  while (this_state)
    {
      if (trace_flag)
	fprintf (stderr, "Processing state %d (reached by %s)\n",
		 this_state->number, tags[this_state->accessing_symbol]);
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

  /* set up initial and final states as parser wants them */
  augment_automaton ();
}
