/* Type definitions for nondeterministic finite state machine for bison,
   Copyright (C) 2001, 2002  Free Software Foundation, Inc.

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


#include "system.h"
#include "hash.h"
#include "complain.h"
#include "gram.h"
#include "state.h"


			/*-------------------.
			| Shifts and Gotos.  |
			`-------------------*/


/*---------------------------------------.
| Create a new array of N shifts/gotos.  |
`---------------------------------------*/

#define SHIFTS_ALLOC(Nshifts)						\
  (shifts_t *) xcalloc ((unsigned) (sizeof (shifts_t) 			\
                                  + (Nshifts - 1) * sizeof (state_number_t)), 1)

static shifts_t *
shifts_new (int nshifts, state_number_t *shifts)
{
  shifts_t *res = SHIFTS_ALLOC (nshifts);
  res->nshifts = nshifts;
  memcpy (res->shifts, shifts, nshifts * sizeof (shifts[0]));
  return res;
}


/*-----------------------------------------------------------------.
| Return the state such these SHIFTS contain a shift/goto to it on |
| SYMBOL.  Aborts if none found.                                   |
`-----------------------------------------------------------------*/

state_t *
shifts_to (shifts_t *shifts, symbol_number_t s)
{
  int j;
  for (j = 0; j < shifts->nshifts; j++)
    if (SHIFT_SYMBOL (shifts, j) == s)
      return states[shifts->shifts[j]];
  abort ();
}


			/*--------------------.
			| Error transitions.  |
			`--------------------*/


/*-------------------------------.
| Create a new array of N errs.  |
`-------------------------------*/

#define ERRS_ALLOC(Nerrs)						\
  (errs_t *) xcalloc ((unsigned) (sizeof (errs_t)			\
                                  + (Nerrs - 1) * sizeof (short)), 1)


errs_t *
errs_new (int n)
{
  errs_t *res = ERRS_ALLOC (n);
  res->nerrs = n;
  return res;
}


errs_t *
errs_dup (errs_t *src)
{
  errs_t *res = errs_new (src->nerrs);
  memcpy (res->errs, src->errs, src->nerrs * sizeof (src->errs[0]));
  return res;
}




			/*-------------.
			| Reductions.  |
			`-------------*/


/*-------------------------------------.
| Create a new array of N reductions.  |
`-------------------------------------*/

#define REDUCTIONS_ALLOC(Nreductions)					\
  (reductions_t *) xcalloc ((unsigned) (sizeof (reductions_t)		\
                                  + (Nreductions - 1) * sizeof (short)), 1)

static reductions_t *
reductions_new (int nreductions, short *reductions)
{
  reductions_t *res = REDUCTIONS_ALLOC (nreductions);
  res->nreds = nreductions;
  memcpy (res->rules, reductions, nreductions * sizeof (reductions[0]));
  return res;
}



			/*---------.
			| States.  |
			`---------*/


state_number_t nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: EOF.  */
state_t *final_state = NULL;

#define STATE_ALLOC(Nitems)						\
  (state_t *) xcalloc ((unsigned) (sizeof (state_t) 			\
                                  + (Nitems - 1) * sizeof (item_number_t)), 1)

/*------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  |
`------------------------------------------------------------*/

state_t *
state_new (symbol_number_t accessing_symbol,
	   size_t core_size, item_number_t *core)
{
  state_t *res;

  if (nstates >= STATE_NUMBER_MAX)
    fatal (_("too many states (max %d)"), STATE_NUMBER_MAX);

  res = STATE_ALLOC (core_size);
  res->accessing_symbol = accessing_symbol;
  res->number = nstates;
  ++nstates;
  res->solved_conflicts = NULL;

  res->nitems = core_size;
  memcpy (res->items, core, core_size * sizeof (core[0]));

  return res;
}


/*--------------------------.
| Set the shifts of STATE.  |
`--------------------------*/

void
state_shifts_set (state_t *state, int nshifts, state_number_t *shifts)
{
  state->shifts = shifts_new (nshifts, shifts);
}


/*------------------------------.
| Set the reductions of STATE.  |
`------------------------------*/

void
state_reductions_set (state_t *state, int nreductions, short *reductions)
{
  state->reductions = reductions_new (nreductions, reductions);
}



/*--------------------------------------------------------------.
| Print on OUT all the lookaheads such that this STATE wants to |
| reduce this RULE.                                             |
`--------------------------------------------------------------*/

void
state_rule_lookaheads_print (state_t *state, rule_t *rule, FILE *out)
{
  int j, k;
  int nlookaheads = 0;
  /* Count the number of lookaheads corresponding to this rule.  */
  for (j = 0; j < state->nlookaheads; ++j)
    BITSET_EXECUTE (state->lookaheads[j], 0, k,
    {
      if (state->lookaheads_rule[j]->number == rule->number)
	nlookaheads++;
    });

  /* Print them if there are.  */
  if (nlookaheads)
    {
      fprintf (out, "  [");
      for (j = 0; j < state->nlookaheads; ++j)
	BITSET_EXECUTE (state->lookaheads[j], 0, k,
	{
	  if (state->lookaheads_rule[j]->number == rule->number)
	    fprintf (out, "%s%s",
		     symbol_tag_get (symbols[k]),
		     --nlookaheads ? ", " : "");
	});
      fprintf (out, "]");
    }
}


/*----------------------.
| A state hash table.  |
`----------------------*/

/* Initial capacity of states hash table.  */
#define HT_INITIAL_CAPACITY 257

static struct hash_table *state_table = NULL;

/* Two states are equal if they have the same core items.  */
static bool
state_compare (const state_t *s1, const state_t *s2)
{
  int i;

  if (s1->nitems != s2->nitems)
    return FALSE;

  for (i = 0; i < s1->nitems; ++i)
    if (s1->items[i] != s2->items[i])
      return FALSE;

  return TRUE;
}

static unsigned int
state_hash (const state_t *state, unsigned int tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  int key = 0;
  int i;
  for (i = 0; i < state->nitems; ++i)
    key += state->items[i];
  return key % tablesize;
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

void
state_hash_new (void)
{
  state_table = hash_initialize (HT_INITIAL_CAPACITY,
				 NULL,
				 (Hash_hasher) state_hash,
				 (Hash_comparator) state_compare,
				 (Hash_data_freer) NULL);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

void
state_hash_free (void)
{
  hash_free (state_table);
}


/*---------------------------------------.
| Insert STATE in the state hash table.  |
`---------------------------------------*/

void
state_hash_insert (state_t *state)
{
  hash_insert (state_table, state);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state_t *
state_hash_lookup (size_t core_size, item_number_t *core)
{
  state_t *probe = STATE_ALLOC (core_size);
  state_t *entry;

  probe->nitems = core_size;
  memcpy (probe->items, core, core_size * sizeof (core[0]));
  entry = hash_lookup (state_table, probe);
  free (probe);
  return entry;
}

/* All the decorated states, indexed by the state number.  */
state_t **states = NULL;


/*----------------------.
| Free all the states.  |
`----------------------*/

void
states_free (void)
{
  state_number_t i;

  for (i = 0; i < nstates; ++i)
    {
      free (states[i]->shifts);
      XFREE (states[i]->reductions);
      free (states[i]->errs);
      free (states[i]);
    }
  XFREE (states);
}
