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



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

static void
max_length (size_t *width, const char *str)
{
  size_t len = strlen (str);
  if (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

static void
print_core (FILE *out, state_t *state)
{
  int i;
  item_number_t *sitems = state->items;
  int snritems   = state->nitems;
  symbol_t *previous_lhs = NULL;

  /* Output all the items of a state, not only its kernel.  */
  if (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nritemset;
    }

  if (!snritems)
    return;

  fputc ('\n', out);

  for (i = 0; i < snritems; i++)
    {
      item_number_t *sp;
      item_number_t *sp1;
      int rule;

      sp1 = sp = ritem + sitems[i];

      while (*sp >= 0)
	sp++;

      rule = item_number_as_rule_number (*sp);

      rule_lhs_print (&rules[rule], previous_lhs, out);
      previous_lhs = rules[rule].lhs;

      for (sp = rules[rule].rhs; sp < sp1; sp++)
	fprintf (out, " %s", symbols[*sp]->tag);
      fputs (" .", out);
      for (/* Nothing */; *sp >= 0; ++sp)
	fprintf (out, " %s", symbols[*sp]->tag);

      /* Display the lookaheads?  */
      if (report_flag & report_lookaheads)
	state_rule_lookaheads_print (state, &rules[rule], out);

      fputc ('\n', out);
    }
}


/*----------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of STATE on |
| OUT.                                                            |
`----------------------------------------------------------------*/

static void
print_transitions (state_t *state, FILE *out, bool display_transitions_p)
{
  transitions_t *transitions = state->transitions;
  size_t width = 0;
  int i;

  /* Compute the width of the lookaheads column.  */
  for (i = 0; i < transitions->num; i++)
    if (!TRANSITION_IS_DISABLED (transitions, i)
	&& TRANSITION_IS_SHIFT (transitions, i) == display_transitions_p)
      {
	symbol_t *symbol = symbols[TRANSITION_SYMBOL (transitions, i)];
	max_length (&width, symbol->tag);
      }

  /* Nothing to report. */
  if (!width)
    return;

  fputc ('\n', out);
  width += 2;

  /* Report lookaheads and shifts.  */
  for (i = 0; i < transitions->num; i++)
    if (!TRANSITION_IS_DISABLED (transitions, i)
	&& TRANSITION_IS_SHIFT (transitions, i) == display_transitions_p)
      {
	symbol_t *symbol = symbols[TRANSITION_SYMBOL (transitions, i)];
	const char *tag = symbol->tag;
	state_t *state1 = transitions->states[i];
	int j;

	fprintf (out, "    %s", tag);
	for (j = width - strlen (tag); j > 0; --j)
	  fputc (' ', out);
	if (display_transitions_p)
	  fprintf (out, _("shift, and go to state %d\n"), state1->number);
	else
	  fprintf (out, _("go to state %d\n"), state1->number);
      }
}


/*------------------------------------------------------------.
| Report the explicit errors of STATE raised from %nonassoc.  |
`------------------------------------------------------------*/

static void
print_errs (FILE *out, state_t *state)
{
  errs_t *errp = state->errs;
  size_t width = 0;
  int i;

  /* Compute the width of the lookaheads column.  */
  for (i = 0; i < errp->num; ++i)
    if (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  if (!width)
    return;

  fputc ('\n', out);
  width += 2;

  /* Report lookaheads and errors.  */
  for (i = 0; i < errp->num; ++i)
    if (errp->symbols[i])
      {
	const char *tag = errp->symbols[i]->tag;
	int j;
	fprintf (out, "    %s", tag);
	for (j = width - strlen (tag); j > 0; --j)
	  fputc (' ', out);
	fputs (_("error (nonassociative)\n"), out);
      }
}


/*----------------------------------------------------------.
| Return the default rule of this STATE if it has one, NULL |
| otherwise.                                                |
`----------------------------------------------------------*/

static rule_t *
state_default_rule (state_t *state)
{
  reductions_t *redp = state->reductions;
  rule_t *default_rule = NULL;
  int cmax = 0;
  int i;

  /* No need for a lookahead.  */
  if (state->consistent)
    return redp->rules[0];

  /* 1. Each reduction is possibly masked by the lookaheads on which
     we shift (S/R conflicts)...  */
  bitset_zero (shiftset);
  {
    transitions_t *transitions = state->transitions;
    FOR_EACH_SHIFT (transitions, i)
      {
	/* If this state has a shift for the error token, don't use a
	     default rule.  */
	if (TRANSITION_IS_ERROR (transitions, i))
	  return NULL;
	bitset_set (shiftset, TRANSITION_SYMBOL (transitions, i));
      }
  }

  /* 2. Each reduction is possibly masked by the lookaheads on which
     we raise an error (due to %nonassoc).  */
  {
    errs_t *errp = state->errs;
    for (i = 0; i < errp->num; i++)
      if (errp->symbols[i])
	bitset_set (shiftset, errp->symbols[i]->number);
  }

  for (i = 0; i < redp->num; ++i)
    {
      int count = 0;

      /* How many non-masked lookaheads are there for this reduction?
	 */
      bitset_andn (lookaheadset, redp->lookaheads[i], shiftset);
      count = bitset_count (lookaheadset);

      if (count > cmax)
	{
	  cmax = count;
	  default_rule = redp->rules[i];
	}

      /* 3. And finally, each reduction is possibly masked by previous
	 reductions (in R/R conflicts, we keep the first reductions).
	 */
      bitset_or (shiftset, shiftset, redp->lookaheads[i]);
    }

  return default_rule;
}


/*--------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEADS (which can be `default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and  |
| R/R conflicts).                                                     |
`--------------------------------------------------------------------*/

static void
print_reduction (FILE *out, size_t width,
		 const char *lookahead,
		 rule_t *rule, bool enabled)
{
  int j;
  fprintf (out, "    %s", lookahead);
  for (j = width - strlen (lookahead); j > 0; --j)
    fputc (' ', out);
  if (!enabled)
    fputc ('[', out);
  if (rule->number)
    fprintf (out, _("reduce using rule %d (%s)"),
	     rule->number, rule->lhs->tag);
  else
    fprintf (out, _("accept"));
  if (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*----------------------------------------------------.
| Report on OUT the reduction actions of this STATE.  |
`----------------------------------------------------*/

static void
print_reductions (FILE *out, state_t *state)
{
  transitions_t *transitions = state->transitions;
  reductions_t *redp = state->reductions;
  rule_t *default_rule = NULL;
  size_t width = 0;
  int i, j;

  if (redp->num == 0)
    return;

  default_rule = state_default_rule (state);

  bitset_zero (shiftset);
  FOR_EACH_SHIFT (transitions, i)
    bitset_set (shiftset, TRANSITION_SYMBOL (transitions, i));

  /* Compute the width of the lookaheads column.  */
  if (default_rule)
    width = strlen (_("$default"));

  if (redp->lookaheads)
    for (i = 0; i < ntokens; i++)
      {
	int count = bitset_test (shiftset, i);

	for (j = 0; j < redp->num; ++j)
	  if (bitset_test (redp->lookaheads[j], i))
	    {
	      if (count == 0)
		{
		  if (redp->rules[j] != default_rule)
		    max_length (&width, symbols[i]->tag);
		  count++;
		}
	      else
		{
		  max_length (&width, symbols[i]->tag);
		}
	    }
      }

  /* Nothing to report. */
  if (!width)
    return;

  fputc ('\n', out);
  width += 2;

  /* Report lookaheads (or $default) and reductions.  */
  if (redp->lookaheads)
    for (i = 0; i < ntokens; i++)
      {
	int defaulted = 0;
	int count = bitset_test (shiftset, i);

	for (j = 0; j < redp->num; ++j)
	  if (bitset_test (redp->lookaheads[j], i))
	    {
	      if (count == 0)
		{
		  if (redp->rules[j] != default_rule)
		    print_reduction (out, width,
				     symbols[i]->tag,
				     redp->rules[j], TRUE);
		  else
		    defaulted = 1;
		  count++;
		}
	      else
		{
		  if (defaulted)
		    print_reduction (out, width,
				     symbols[i]->tag,
				     default_rule, TRUE);
		  defaulted = 0;
		  print_reduction (out, width,
				   symbols[i]->tag,
				   redp->rules[j], FALSE);
		}
	    }
      }

  if (default_rule)
    print_reduction (out, width,
		     _("$default"), default_rule, TRUE);
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit erros from %nonassoc) of STATE.                      |
`--------------------------------------------------------------*/

static void
print_actions (FILE *out, state_t *state)
{
  /* Print shifts.  */
  print_transitions (state, out, TRUE);
  print_errs (out, state);
  print_reductions (out, state);
  /* Print gotos.  */
  print_transitions (state, out, FALSE);
}


/*--------------------------------------.
| Report all the data on STATE on OUT.  |
`--------------------------------------*/

static void
print_state (FILE *out, state_t *state)
{
  fputs ("\n\n", out);
  fprintf (out, _("state %d"), state->number);
  fputc ('\n', out);
  print_core (out, state);
  print_actions (out, state);
  if ((report_flag & report_solved_conflicts)
      && state->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (state->solved_conflicts, out);
    }
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
	const char *tag = symbols[token_translations[i]]->tag;
	rule_number_t r;
	item_number_t *rhsp;

	buffer[0] = 0;
	column = strlen (tag);
	fputs (tag, out);
	END_TEST (50);
	sprintf (buffer, " (%d)", i);

	for (r = 0; r < nrules; r++)
	  for (rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
	    if (item_number_as_symbol_number (*rhsp) == token_translations[i])
	      {
		END_TEST (65);
		sprintf (buffer + strlen (buffer), " %d", r);
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
      const char *tag = symbols[i]->tag;

      for (r = 0; r < nrules; r++)
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

	  for (r = 0; r < nrules; r++)
	    {
	      END_TEST (65);
	      if (rules[r].lhs->number == i)
		sprintf (buffer + strlen (buffer), " %d", r);
	    }
	}

      if (right_count > 0)
	{
	  if (left_count > 0)
	    sprintf (buffer + strlen (buffer), ",");
	  END_TEST (50);
	  sprintf (buffer + strlen (buffer), _(" on right:"));
	  for (r = 0; r < nrules; r++)
	    {
	      item_number_t *rhsp;
	      for (rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
		if (item_number_as_symbol_number (*rhsp) == i)
		  {
		    END_TEST (65);
		    sprintf (buffer + strlen (buffer), " %d", r);
		    break;
		  }
	    }
	}
      fprintf (out, "%s\n", buffer);
    }
}

void
print_results (void)
{
  state_number_t i;

  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *out = xfopen (spec_verbose_file, "w");

  reduce_output (out);
  grammar_rules_partial_print (out,
			       _("Rules never reduced"), rule_never_reduced_p);
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
