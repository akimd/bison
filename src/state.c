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
#include "complain.h"
#include "gram.h"
#include "state.h"


			/*-------------------.
			| Shifts and Gotos.  |
			`-------------------*/


/*---------------------------------.
| Create a new array of N shitfs.  |
`---------------------------------*/

#define SHIFTS_ALLOC(Nshifts)						\
  (shifts *) xcalloc ((unsigned) (sizeof (shifts) 			\
                                  + (Nshifts - 1) * sizeof (short)), 1)

shifts *
shifts_new (int n)
{
  shifts *res = SHIFTS_ALLOC (n);
  res->nshifts = n;
  return res;
}




			/*--------------------.
			| Error transitions.  |
			`--------------------*/


/*-------------------------------.
| Create a new array of N errs.  |
`-------------------------------*/

#define ERRS_ALLOC(Nerrs)						\
  (errs *) xcalloc ((unsigned) (sizeof (errs) 				\
                                  + (Nerrs - 1) * sizeof (short)), 1)


errs *
errs_new (int n)
{
  errs *res = ERRS_ALLOC (n);
  res->nerrs = n;
  return res;
}


errs *
errs_dup (errs *src)
{
  errs *res = errs_new (src->nerrs);
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
  (reductions *) xcalloc ((unsigned) (sizeof (reductions)		\
                                  + (Nreductions - 1) * sizeof (short)), 1)

reductions *
reductions_new (int n)
{
  reductions *res = REDUCTIONS_ALLOC (n);
  res->nreds = n;
  return res;
}



			/*---------.
			| States.  |
			`---------*/


state_number_t nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: EOF.  */
state_t *final_state = NULL;

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

#define STATE_ALLOC(Nitems)						\
  (state_t *) xcalloc ((unsigned) (sizeof (state_t) 			\
                                  + (Nitems - 1) * sizeof (item_number_t)), 1)

  res = STATE_ALLOC (core_size);
  res->accessing_symbol = accessing_symbol;
  res->number = nstates;
  ++nstates;
  res->solved_conflicts = NULL;

  res->nitems = core_size;
  memcpy (res->items, core, core_size * sizeof (core[0]));

  return res;
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
    for (k = 0; k < ntokens; ++k)
      if (bitset_test (state->lookaheads[j], k)
	  && state->lookaheads_rule[j]->number == rule->number)
	nlookaheads++;

  /* Print them if there are.  */
  if (nlookaheads)
    {
      fprintf (out, "  [");
      for (j = 0; j < state->nlookaheads; ++j)
	for (k = 0; k < ntokens; ++k)
	  if (bitset_test (state->lookaheads[j], k)
	      && state->lookaheads_rule[j]->number == rule->number)
	    fprintf (out, "%s%s",
		     symbol_tag_get (symbols[k]),
		     --nlookaheads ? ", " : "");
      fprintf (out, "]");
    }
}
