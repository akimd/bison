/* Find and resolve or report look-ahead conflicts for bison,
   Copyright (C) 1984, 1989, 1992, 2000, 2001, 2002
   Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include "system.h"
#include "bitset.h"
#include "complain.h"
#include "getargs.h"
#include "symtab.h"
#include "files.h"
#include "gram.h"
#include "state.h"
#include "lalr.h"
#include "conflicts.h"
#include "reader.h"
#include "LR0.h"

/* -1 stands for not specified. */
int expected_conflicts = -1;
static char *conflicts = NULL;
struct obstack solved_conflicts_obstack;

static bitset shiftset;
static bitset lookaheadset;



enum conflict_resolution_e
  {
    shift_resolution,
    reduce_resolution,
    left_resolution,
    right_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

static inline void
log_resolution (rule_t *rule, symbol_number_t token,
		enum conflict_resolution_e resolution)
{
  if (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      switch (resolution)
	{
	case shift_resolution:
	case right_resolution:
	  obstack_fgrow2 (&solved_conflicts_obstack,
			  _("\
    Conflict between rule %d and token %s resolved as shift"),
			  rule->number,
			  symbols[token]->tag);
	  break;
	case reduce_resolution:
	case left_resolution:
	  obstack_fgrow2 (&solved_conflicts_obstack,
			  _("\
    Conflict between rule %d and token %s resolved as reduce"),
			  rule->number,
			  symbols[token]->tag);
	  break;
	case nonassoc_resolution:
	  obstack_fgrow2 (&solved_conflicts_obstack,
			  _("\
    Conflict between rule %d and token %s resolved as an error"),
			  rule->number,
			  symbols[token]->tag);
	  break;
	}

      /* The reason. */
      switch (resolution)
	{
	case shift_resolution:
	  obstack_fgrow2 (&solved_conflicts_obstack,
			  " (%s < %s)",
			  rule->prec->tag,
			  symbols[token]->tag);
	  break;

	case reduce_resolution:
	  obstack_fgrow2 (&solved_conflicts_obstack,
			  " (%s < %s)",
			  symbols[token]->tag,
			  rule->prec->tag);
	  break;

	case left_resolution:
	  obstack_fgrow1 (&solved_conflicts_obstack,
			  " (%%left %s)",
			  symbols[token]->tag);
	  break;

	case right_resolution:
	  obstack_fgrow1 (&solved_conflicts_obstack,
			  " (%%right %s)",
			  symbols[token]->tag);
	  break;
	case nonassoc_resolution:
	  obstack_fgrow1 (&solved_conflicts_obstack,
			  " (%%nonassoc %s)",
			  symbols[token]->tag);
	  break;
	}
      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift-reduce conflict in |
| favor of the reduction.                                           |
`------------------------------------------------------------------*/

static void
flush_shift (state_t *state, int token)
{
  transitions_t *transitions = state->transitions;
  int i;

  bitset_reset (lookaheadset, token);
  for (i = 0; i < transitions->num; i++)
    if (!TRANSITION_IS_DISABLED (transitions, i)
	&& TRANSITION_SYMBOL (transitions, i) == token)
      TRANSITION_DISABLE (transitions, i);
}


/*-------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token for the       |
| specified lookahead.  Used when we resolve a shift-reduce conflict |
| in favor of the shift.                                             |
`-------------------------------------------------------------------*/

static void
flush_reduce (bitset lookaheads, int token)
{
  bitset_reset (lookaheads, token);
}


/*------------------------------------------------------------------.
| Attempt to resolve shift-reduce conflict for one rule by means of |
| precedence declarations.  It has already been checked that the    |
| rule has a precedence.  A conflict is resolved by modifying the   |
| shift or reduce tables so that there is no longer a conflict.     |
|                                                                   |
| LOOKAHEAD is the number of the lookahead bitset to consider.      |
|                                                                   |
| ERRS can be used to store discovered explicit errors.             |
`------------------------------------------------------------------*/

static void
resolve_sr_conflict (state_t *state, int ruleno,
		     symbol_t **errs)
{
  symbol_number_t i;
  reductions_t *reds = state->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule_t *redrule = reds->rules[ruleno];
  int redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];
  int nerrs = 0;

  for (i = 0; i < ntokens; i++)
    if (bitset_test (lookaheads, i)
	&& bitset_test (lookaheadset, i)
	&& symbols[i]->prec)
      {
	/* Shift-reduce conflict occurs for token number i
	   and it has a precedence.
	   The precedence of shifting is that of token i.  */
	if (symbols[i]->prec < redprec)
	  {
	    log_resolution (redrule, i, reduce_resolution);
	    flush_shift (state, i);
	  }
	else if (symbols[i]->prec > redprec)
	  {
	    log_resolution (redrule, i, shift_resolution);
	    flush_reduce (lookaheads, i);
	  }
	else
	  /* Matching precedence levels.
	     For left association, keep only the reduction.
	     For right association, keep only the shift.
	     For nonassociation, keep neither.  */

	  switch (symbols[i]->assoc)
	    {
	    case right_assoc:
	      log_resolution (redrule, i, right_resolution);
	      flush_reduce (lookaheads, i);
	      break;

	    case left_assoc:
	      log_resolution (redrule, i, left_resolution);
	      flush_shift (state, i);
	      break;

	    case non_assoc:
	      log_resolution (redrule, i, nonassoc_resolution);
	      flush_shift (state, i);
	      flush_reduce (lookaheads, i);
	      /* Record an explicit error for this token.  */
	      errs[nerrs++] = symbols[i];
	      break;

	    case undef_assoc:
	      assert (symbols[i]->assoc != undef_assoc);
	      break;
	    }
      }

  /* Some tokens have been explicitly made errors.  Allocate a
     permanent errs structure for this state, to record them.  */
  state_errs_set (state, nerrs, errs);

  if (obstack_object_size (&solved_conflicts_obstack))
    {
      obstack_1grow (&solved_conflicts_obstack, '\0');
      state->solved_conflicts = obstack_finish (&solved_conflicts_obstack);
    }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of STATE using the                         |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRS can be used as storage to compute the list of     |
| lookaheads on which this STATE raises a parse error (%nonassoc).   |
`-------------------------------------------------------------------*/

static void
set_conflicts (state_t *state, symbol_t **errs)
{
  int i;
  transitions_t *transitions = state->transitions;
  reductions_t *reds = state->reductions;

  if (state->consistent)
    return;

  bitset_zero (lookaheadset);

  FOR_EACH_SHIFT (transitions, i)
    bitset_set (lookaheadset, TRANSITION_SYMBOL (transitions, i));

  /* Loop over all rules which require lookahead in this state.  First
     check for shift-reduce conflict, and try to resolve using
     precedence.  */
  for (i = 0; i < reds->num; ++i)
    if (reds->rules[i]->prec && reds->rules[i]->prec->prec
	&& !bitset_disjoint_p (reds->lookaheads[i], lookaheadset))
      {
	resolve_sr_conflict (state, i, errs);
	break;
      }

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.  */
  for (i = 0; i < reds->num; ++i)
    {
      if (!bitset_disjoint_p (reds->lookaheads[i], lookaheadset))
	conflicts[state->number] = 1;

      bitset_or (lookaheadset, lookaheadset, reds->lookaheads[i]);
    }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

void
conflicts_solve (void)
{
  state_number_t i;
  /* List of lookaheads on which we explicitly raise a parse error.  */
  symbol_t **errs = XMALLOC (symbol_t *, ntokens + 1);

  conflicts = XCALLOC (char, nstates);
  shiftset = bitset_create (ntokens, BITSET_FIXED);
  lookaheadset = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);

  for (i = 0; i < nstates; i++)
    {
      set_conflicts (states[i], errs);

      /* For uniformity of the code, make sure all the states have a valid
	 `errs' member.  */
      if (!states[i]->errs)
	states[i]->errs = errs_new (0, 0);
    }

  free (errs);
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

static int
count_sr_conflicts (state_t *state)
{
  int i;
  int src_count = 0;
  transitions_t *transitions = state->transitions;
  reductions_t *reds = state->reductions;

  if (!transitions)
    return 0;

  bitset_zero (lookaheadset);
  bitset_zero (shiftset);

  FOR_EACH_SHIFT (transitions, i)
    bitset_set (shiftset, TRANSITION_SYMBOL (transitions, i));

  for (i = 0; i < reds->num; ++i)
    bitset_or (lookaheadset, lookaheadset, reds->lookaheads[i]);

  bitset_and (lookaheadset, lookaheadset, shiftset);

  src_count = bitset_count (lookaheadset);

  return src_count;
}


/*----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  If ONE_PER_TOKEN, |
| count one conflict for each token that has any reduce/reduce    |
| conflicts.  Otherwise, count one conflict for each pair of      |
| conflicting reductions.                                         |
+`----------------------------------------------------------------*/

static int
count_rr_conflicts (state_t *state, int one_per_token)
{
  int i;
  reductions_t *reds = state->reductions;
  int rrc_count = 0;

  for (i = 0; i < ntokens; i++)
    {
      int count = 0;
      int j;
      for (j = 0; j < reds->num; ++j)
	if (bitset_test (reds->lookaheads[j], i))
	  count++;

      if (count >= 2)
	rrc_count += one_per_token ? 1 : count-1;
    }

  return rrc_count;
}


/*--------------------------------------------------------------.
| Return a human readable string which reports shift/reduce and |
| reduce/reduce conflict numbers (SRC_NUM, RRC_NUM).            |
`--------------------------------------------------------------*/

static const char *
conflict_report (int src_num, int rrc_num)
{
  static char res[4096];
  char *cp = res;

  if (src_num >= 1)
    {
      sprintf (cp, ngettext ("%d shift/reduce conflict",
			     "%d shift/reduce conflicts", src_num), src_num);
      cp += strlen (cp);
    }

  if (src_num > 0 && rrc_num > 0)
    {
      sprintf (cp, " %s ", _("and"));
      cp += strlen (cp);
    }

  if (rrc_num >= 1)
    {
      sprintf (cp, ngettext ("%d reduce/reduce conflict",
			     "%d reduce/reduce conflicts", rrc_num), rrc_num);
      cp += strlen (cp);
    }

  *cp++ = '\0';

  return res;
}


/*----------------------------------------------------------------.
| Same as above, but report the number of conflicts a` la POSIX.  |
`----------------------------------------------------------------*/

static void
conflict_report_yacc (int src_num, int rrc_num)
{
  /* If invoked with `--yacc', use the output format specified by
     POSIX.  */
  fprintf (stderr, _("conflicts: "));
  if (src_num > 0)
    fprintf (stderr, _(" %d shift/reduce"), src_num);
  if (src_num > 0 && rrc_num > 0)
    fprintf (stderr, ",");
  if (rrc_num > 0)
    fprintf (stderr, _(" %d reduce/reduce"), rrc_num);
  putc ('\n', stderr);
}


/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

void
conflicts_output (FILE *out)
{
  bool printed_sth = FALSE;
  state_number_t i;
  for (i = 0; i < nstates; i++)
    {
      state_t *s = states[i];
      if (conflicts[i])
	{
	  fprintf (out, _("State %d contains "), i);
	  fprintf (out, "%s.\n",
		   conflict_report (count_sr_conflicts (s),
				    count_rr_conflicts (s, TRUE)));
	  printed_sth = TRUE;
	}
    }
  if (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------------------.
| Total the number of S/R and R/R conflicts.  Unlike the  |
| code in conflicts_output, however, count EACH pair of   |
| reductions for the same state and lookahead as one      |
| conflict.						  |
`--------------------------------------------------------*/

int
conflicts_total_count (void)
{
  state_number_t i;
  int count;

  /* Conflicts by state.  */
  count = 0;
  for (i = 0; i < nstates; i++)
    if (conflicts[i])
      {
	count += count_sr_conflicts (states[i]);
	count += count_rr_conflicts (states[i], FALSE);
      }
  return count;
}


/*------------------------------------------.
| Reporting the total number of conflicts.  |
`------------------------------------------*/

void
conflicts_print (void)
{
  /* Is the number of SR conflicts OK?  Either EXPECTED_CONFLICTS is
     not set, and then we want 0 SR, or else it is specified, in which
     case we want equality.  */
  int src_ok = 0;

  int src_total = 0;
  int rrc_total = 0;

  /* Conflicts by state.  */
  {
    state_number_t i;

    for (i = 0; i < nstates; i++)
      if (conflicts[i])
	{
	  src_total += count_sr_conflicts (states[i]);
	  rrc_total += count_rr_conflicts (states[i], TRUE);
	}
  }

  src_ok = src_total == (expected_conflicts == -1 ? 0 : expected_conflicts);

  /* If there are no RR conflicts, and as many SR conflicts as
     expected, then there is nothing to report.  */
  if (!rrc_total && src_ok)
    return;

  /* Report the total number of conflicts on STDERR.  */
  if (yacc_flag)
    conflict_report_yacc (src_total, rrc_total);
  else
    warn ("%s", conflict_report (src_total, rrc_total));

  if (expected_conflicts != -1 && !src_ok)
    complain (ngettext ("expected %d shift/reduce conflict",
			"expected %d shift/reduce conflicts",
			expected_conflicts),
	      expected_conflicts);
}


void
conflicts_free (void)
{
  XFREE (conflicts);
  bitset_free (shiftset);
  bitset_free (lookaheadset);
  obstack_free (&solved_conflicts_obstack, NULL);
}
