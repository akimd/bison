/* Print information on generated parser, for bison,
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


#include "system.h"
#include "quotearg.h"
#include "files.h"
#include "symtab.h"
#include "gram.h"
#include "LR0.h"
#include "lalr.h"
#include "conflicts.h"
#include "getargs.h"
#include "state.h"
#include "reader.h"
#include "print.h"
#include "reduce.h"
#include "closure.h"
#include "bitset.h"

static bitset shiftset;
static bitset lookaheadset;

#if 0
static void
print_token (int extnum, int token)
{
  fprintf (out, _(" type %d is %s\n"), extnum, tags[token]);
}
#endif


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

static void
print_core (FILE *out, state_t *state)
{
  int i;
  item_number_t *sitems = state->items;
  int snritems   = state->nitems;

  /* Output all the items of a state, not only its kernel.  */
  if (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nritemset;
    }

  if (snritems)
    {
      for (i = 0; i < snritems; i++)
	{
	  item_number_t *sp;
	  item_number_t *sp1;
	  int rule;

	  sp1 = sp = ritem + sitems[i];

	  while (*sp >= 0)
	    sp++;

	  rule = -(*sp);
	  fprintf (out, "    %s  ->  ", symbol_tag_get (rules[rule].lhs));

	  for (sp = rules[rule].rhs; sp < sp1; sp++)
	    fprintf (out, "%s ", symbol_tag_get (symbols[*sp]));

	  fputc ('.', out);

	  for (/* Nothing */; *sp >= 0; ++sp)
	    fprintf (out, " %s", symbol_tag_get (symbols[*sp]));

	  /* Display the lookaheads?  */
	  if (report_flag & report_lookaheads)
	    state_rule_lookaheads_print (state, &rules[rule], out);

	  fprintf (out, _("   (rule %d)"), rule - 1);
	  fputc ('\n', out);
	}

      fputc ('\n', out);
    }
}


static void
print_shifts (FILE *out, state_t *state)
{
  int i;
  shifts_t *shiftp = state->shifts;

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	state_number_t state1 = shiftp->shifts[i];
	symbol_number_t symbol = states[state1]->accessing_symbol;
	fprintf (out,
		 _("    %-4s\tshift, and go to state %d\n"),
		 symbol_tag_get (symbols[symbol]), state1);
      }

  if (i > 0)
    fputc ('\n', out);
}


static void
print_errs (FILE *out, state_t *state)
{
  errs_t *errp = state->errs;
  int i;

  for (i = 0; i < errp->nerrs; ++i)
    if (errp->errs[i])
      fprintf (out, _("    %-4s\terror (nonassociative)\n"),
	       symbol_tag_get (symbols[errp->errs[i]]));

  if (i > 0)
    fputc ('\n', out);
}


static void
print_gotos (FILE *out, state_t *state)
{
  int i;
  shifts_t *shiftp = state->shifts;

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    /* Skip token shifts.  */;

  if (i < shiftp->nshifts)
    {
      for (; i < shiftp->nshifts; i++)
	if (!SHIFT_IS_DISABLED (shiftp, i))
	  {
	    state_number_t state1 = shiftp->shifts[i];
	    symbol_number_t symbol = states[state1]->accessing_symbol;
	    fprintf (out, _("    %-4s\tgo to state %d\n"),
		     symbol_tag_get (symbols[symbol]), state1);
	  }

      fputc ('\n', out);
    }
}


/*----------------------------------------------------------.
| Return the default rule of this STATE if it has one, NULL |
| otherwise.                                                |
`----------------------------------------------------------*/

static rule_t *
state_default_rule_compute (state_t *state)
{
  reductions_t *redp = state->reductions;
  rule_t *default_rule = NULL;
  int cmax = 0;
  int i;

  /* No need for a lookahead.  */
  if (state->consistent)
    return &rules[redp->rules[0]];

  /* 1. Each reduction is possibly masked by the lookaheads on which
     we shift (S/R conflicts)...  */
  bitset_zero (shiftset);
  {
    shifts_t *shiftp = state->shifts;
    for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
      if (!SHIFT_IS_DISABLED (shiftp, i))
	{
	  /* If this state has a shift for the error token, don't use a
	     default rule.  */
	  if (SHIFT_IS_ERROR (shiftp, i))
	    return NULL;
	  bitset_set (shiftset, SHIFT_SYMBOL (shiftp, i));
	}
  }

  /* 2. Each reduction is possibly masked by the lookaheads on which
     we raise an error (due to %nonassoc).  */
  {
    errs_t *errp = state->errs;
    for (i = 0; i < errp->nerrs; i++)
      if (errp->errs[i])
	bitset_set (shiftset, errp->errs[i]);
  }

  for (i = 0; i < state->nlookaheads; ++i)
    {
      int count = 0;

      /* How many non-masked lookaheads are there for this reduction?
	 */
      bitset_andn (lookaheadset, state->lookaheads[i], shiftset);
      count = bitset_count (lookaheadset);

      if (count > cmax)
	{
	  cmax = count;
	  default_rule = state->lookaheads_rule[i];
	}

      /* 3. And finally, each reduction is possibly masked by previous
	 reductions (in R/R conflicts, we keep the first reductions).
	 */
      bitset_or (shiftset, shiftset, state->lookaheads[i]);
    }

  return default_rule;
}


/*----------------------------------------------------.
| Report on OUT the reduction actions of this STATE.  |
`----------------------------------------------------*/

static void
print_reductions (FILE *out, state_t *state)
{
  int i;
  shifts_t *shiftp = state->shifts;
  reductions_t *redp = state->reductions;
  rule_t *default_rule = NULL;

  if (redp->nreds == 0)
    return;

  default_rule = state_default_rule_compute (state);

  bitset_zero (shiftset);
  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      bitset_set (shiftset, SHIFT_SYMBOL (shiftp, i));

  for (i = 0; i < ntokens; i++)
    {
      int j;
      int defaulted = 0;
      int count = bitset_test (shiftset, i);

      for (j = 0; j < state->nlookaheads; ++j)
	if (bitset_test (state->lookaheads[j], i))
	  {
	    if (count == 0)
	      {
		if (state->lookaheads_rule[j] != default_rule)
		  fprintf (out,
			   _("    %-4s\treduce using rule %d (%s)\n"),
			   symbol_tag_get (symbols[i]),
			   state->lookaheads_rule[j]->number - 1,
			   symbol_tag_get_n (state->lookaheads_rule[j]->lhs, 1));
		else
		  defaulted = 1;
		count++;
	      }
	    else
	      {
		if (defaulted)
		  fprintf (out,
			   _("    %-4s\treduce using rule %d (%s)\n"),
			   symbol_tag_get (symbols[i]),
			   default_rule->number - 1,
			   symbol_tag_get_n (default_rule->lhs, 1));
		defaulted = 0;
		fprintf (out,
			 _("    %-4s\t[reduce using rule %d (%s)]\n"),
			 symbol_tag_get (symbols[i]),
			 state->lookaheads_rule[j]->number - 1,
			 symbol_tag_get_n (state->lookaheads_rule[j]->lhs, 1));
	      }
	  }
    }

  if (default_rule)
    fprintf (out, _("    $default\treduce using rule %d (%s)\n"),
	     default_rule->number - 1,
	     symbol_tag_get (default_rule->lhs));
  fputc ('\n', out);
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit erros from %nonassoc) of STATE.                      |
`--------------------------------------------------------------*/

static void
print_actions (FILE *out, state_t *state)
{
  reductions_t *redp = state->reductions;
  shifts_t *shiftp = state->shifts;

  if (shiftp->nshifts == 0 && redp->nreds == 0)
    {
      if (state->number == final_state->number)
	fprintf (out, _("    $default\taccept\n"));
      else
	fprintf (out, _("    NO ACTIONS\n"));
      return;
    }

  print_shifts (out, state);
  print_errs (out, state);
  print_reductions (out, state);
  print_gotos (out, state);
}


static void
print_state (FILE *out, state_t *state)
{
  fprintf (out, _("state %d"), state->number);
  fputs ("\n\n", out);
  print_core (out, state);
  print_actions (out, state);
  if ((report_flag & report_solved_conflicts)
      && state->solved_conflicts)
    fputs (state->solved_conflicts, out);
  fputs ("\n\n", out);
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

#define END_TEST(End)				\
do {						\
  if (column + strlen(buffer) > (End))		\
    {						\
      fprintf (out, "%s\n   ", buffer);		\
      column = 3;				\
      buffer[0] = 0;				\
    }						\
} while (0)


static void
print_grammar (FILE *out)
{
  symbol_number_t i;
  char buffer[90];
  int column = 0;

  grammar_rules_print (out);

  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  for (i = 0; i < max_user_token_number + 1; i++)
    if (token_translations[i] != undeftoken->number)
      {
	const char *tag = symbol_tag_get (symbols[token_translations[i]]);
	rule_number_t r;
	item_number_t *rhsp;

	buffer[0] = 0;
	column = strlen (tag);
	fputs (tag, out);
	END_TEST (50);
	sprintf (buffer, " (%d)", i);

	for (r = 1; r < nrules + 1; r++)
	  for (rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
	    if (item_number_as_symbol_number (*rhsp) == token_translations[i])
	      {
		END_TEST (65);
		sprintf (buffer + strlen (buffer), " %d", r - 1);
		break;
	      }
	fprintf (out, "%s\n", buffer);
      }
  fputs ("\n\n", out);


  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  for (i = ntokens; i < nsyms; i++)
    {
      int left_count = 0, right_count = 0;
      rule_number_t r;
      const char *tag = symbol_tag_get (symbols[i]);

      for (r = 1; r < nrules + 1; r++)
	{
	  item_number_t *rhsp;
	  if (rules[r].lhs->number == i)
	    left_count++;
	  for (rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
	    if (item_number_as_symbol_number (*rhsp) == i)
	      {
		right_count++;
		break;
	      }
	}

      buffer[0] = 0;
      fputs (tag, out);
      column = strlen (tag);
      sprintf (buffer, " (%d)", i);
      END_TEST (0);

      if (left_count > 0)
	{
	  END_TEST (50);
	  sprintf (buffer + strlen (buffer), _(" on left:"));

	  for (r = 1; r < nrules + 1; r++)
	    {
	      END_TEST (65);
	      if (rules[r].lhs->number == i)
		sprintf (buffer + strlen (buffer), " %d", r - 1);
	    }
	}

      if (right_count > 0)
	{
	  if (left_count > 0)
	    sprintf (buffer + strlen (buffer), ",");
	  END_TEST (50);
	  sprintf (buffer + strlen (buffer), _(" on right:"));
	  for (r = 1; r < nrules + 1; r++)
	    {
	      item_number_t *rhsp;
	      for (rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
		if (item_number_as_symbol_number (*rhsp) == i)
		  {
		    END_TEST (65);
		    sprintf (buffer + strlen (buffer), " %d", r - 1);
		    break;
		  }
	    }
	}
      fprintf (out, "%s\n", buffer);
    }
  fputs ("\n\n", out);
}

void
print_results (void)
{
  state_number_t i;

  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *out = xfopen (spec_verbose_file, "w");

  reduce_output (out);
  conflicts_output (out);

  print_grammar (out);

  /* If the whole state item sets, not only the kernels, are wanted,
     `closure' will be run, which needs memory allocation/deallocation.   */
  if (report_flag & report_itemsets)
    new_closure (nritems);
  /* Storage for print_reductions.  */
  shiftset =  bitset_create (ntokens, BITSET_FIXED);
  lookaheadset = bitset_create (ntokens, BITSET_FIXED);
  for (i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (shiftset);
  bitset_free (lookaheadset);
  if (report_flag & report_itemsets)
    free_closure ();

  xfclose (out);
}
