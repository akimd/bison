/* Find and resolve or report look-ahead conflicts for bison,
   Copyright 1984, 1989, 1992, 2000, 2001, 2002 Free Software Foundation, Inc.

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

static bitset shiftset;
static bitset lookaheadset;


static inline void
log_resolution (state_t *state, int LAno, int token, char *resolution)
{
  if (verbose_flag)
    obstack_fgrow4 (&output_obstack,
		    _("\
Conflict in state %d between rule %d and token %s resolved as %s.\n"),
		    state->number, LAruleno[LAno], symbols[token]->tag,
		    resolution);
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift-reduce conflict in |
| favor of the reduction.                                           |
`------------------------------------------------------------------*/

static void
flush_shift (state_t *state, int token)
{
  shifts *shiftp = state->shifts;
  int i;

  bitset_reset (lookaheadset, token);
  for (i = 0; i < shiftp->nshifts; i++)
    if (!SHIFT_IS_DISABLED (shiftp, i) && SHIFT_SYMBOL (shiftp, i) == token)
      SHIFT_DISABLE (shiftp, i);
}


/*-------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token for the       |
| specified lookahead.  Used when we resolve a shift-reduce conflict |
| in favor of the shift.                                             |
`-------------------------------------------------------------------*/

static void
flush_reduce (int lookahead, int token)
{
  bitset_reset (LA[lookahead], token);
}


/*------------------------------------------------------------------.
| Attempt to resolve shift-reduce conflict for one rule by means of |
| precedence declarations.  It has already been checked that the    |
| rule has a precedence.  A conflict is resolved by modifying the   |
| shift or reduce tables so that there is no longer a conflict.     |
`------------------------------------------------------------------*/

static void
resolve_sr_conflict (state_t *state, int lookahead)
{
  int i;
  /* find the rule to reduce by to get precedence of reduction  */
  int redprec = rules[LAruleno[lookahead]].prec;
  errs *errp = errs_new (ntokens + 1);
  errp->nerrs = 0;

  for (i = 0; i < ntokens; i++)
    if (bitset_test (LA[lookahead], i)
	&& bitset_test (lookaheadset, i)
	&& symbols[i]->prec)
      {
	/* Shift-reduce conflict occurs for token number i
	   and it has a precedence.
	   The precedence of shifting is that of token i.  */
	if (symbols[i]->prec < redprec)
	  {
	    log_resolution (state, lookahead, i, _("reduce"));
	    flush_shift (state, i);
	  }
	else if (symbols[i]->prec > redprec)
	  {
	    log_resolution (state, lookahead, i, _("shift"));
	    flush_reduce (lookahead, i);
	  }
	else
	  /* Matching precedence levels.
	     For left association, keep only the reduction.
	     For right association, keep only the shift.
	     For nonassociation, keep neither.  */

	  switch (symbols[i]->assoc)
	    {
	    case right_assoc:
	      log_resolution (state, lookahead, i, _("shift"));
	      flush_reduce (lookahead, i);
	      break;

	    case left_assoc:
	      log_resolution (state, lookahead, i, _("reduce"));
	      flush_shift (state, i);
	      break;

	    case non_assoc:
	      log_resolution (state, lookahead, i, _("an error"));
	      flush_shift (state, i);
	      flush_reduce (lookahead, i);
	      /* Record an explicit error for this token.  */
	      errp->errs[errp->nerrs++] = i;
	      break;
	    }
      }

  /* Some tokens have been explicitly made errors.  Allocate a
     permanent errs structure for this state, to record them.  */
  state->errs = errs_dup (errp);
  free (errp);
}


static void
set_conflicts (state_t *state)
{
  int i, j;
  shifts *shiftp;

  if (state->consistent)
    return;

  bitset_zero (lookaheadset);

  shiftp = state->shifts;
  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      bitset_set (lookaheadset, SHIFT_SYMBOL (shiftp, i));

  /* Loop over all rules which require lookahead in this state.  First
     check for shift-reduce conflict, and try to resolve using
     precedence */
  for (i = 0; i < state->nlookaheads; ++i)
    if (rules[LAruleno[state->lookaheadsp + i]].prec)
      for (j = 0; j < ntokens; ++j)
	if (bitset_test (LA[state->lookaheadsp + i], j)
	    && bitset_test (lookaheadset, j))
	  {
	    resolve_sr_conflict (state, state->lookaheadsp + i);
	    break;
	  }


  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.  */
  for (i = 0; i < state->nlookaheads; ++i)
    {
      /* FIXME: Here, I need something like `bitset_disjoint_p'. */
      for (j = 0; j < ntokens; ++j)
	if (bitset_test (LA[state->lookaheadsp + i], j)
	    && bitset_test (lookaheadset, j))
	  conflicts[state->number] = 1;

      bitset_or (lookaheadset, lookaheadset, LA[state->lookaheadsp + i]);
    }
}

void
solve_conflicts (void)
{
  size_t i;

  conflicts = XCALLOC (char, nstates);
  shiftset = bitset_create (ntokens, BITSET_FIXED);
  bitset_zero (shiftset);
  lookaheadset = bitset_create (ntokens, BITSET_FIXED);
  bitset_zero (lookaheadset);

  for (i = 0; i < nstates; i++)
    set_conflicts (states[i]);
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

static int
count_sr_conflicts (state_t *state)
{
  int i;
  int src_count = 0;
  shifts *shiftp = state->shifts;

  if (!shiftp)
    return 0;

  bitset_zero (lookaheadset);
  bitset_zero (shiftset);

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      bitset_set (shiftset, SHIFT_SYMBOL (shiftp, i));

  for (i = 0; i < state->nlookaheads; ++i)
    bitset_or (lookaheadset, lookaheadset, LA[state->lookaheadsp + i]);

  bitset_and (lookaheadset, lookaheadset, shiftset);

  for (i = 0; i < ntokens; i++)
    if (bitset_test (lookaheadset, i))
      src_count++;

  return src_count;
}


/*----------------------------------------------.
| Count the number of reduce/reduce conflicts.  |
`----------------------------------------------*/

static int
count_rr_conflicts (state_t *state)
{
  int i;
  int rrc_count = 0;

  if (state->nlookaheads < 2)
    return 0;

  for (i = 0; i < ntokens; i++)
    {
      int count = 0;
      int j;
      for (j = 0; j < state->nlookaheads; ++j)
	if (bitset_test (LA[state->lookaheadsp + j], i))
	  count++;

      if (count >= 2)
	rrc_count++;
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

  *cp++ = '.';
  *cp++ = '\n';
  *cp++ = '\0';

  return res;
}


/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

void
conflicts_output (FILE *out)
{
  bool printed_sth = FALSE;
  size_t i;
  for (i = 0; i < nstates; i++)
    if (conflicts[i])
      {
	fprintf (out, _("State %d contains "), i);
	fputs (conflict_report (count_sr_conflicts (states[i]),
				count_rr_conflicts (states[i])), out);
	printed_sth = TRUE;
      }
  if (printed_sth)
    fputs ("\n\n", out);
}


/*------------------------------------------.
| Reporting the total number of conflicts.  |
`------------------------------------------*/

void
conflicts_print (void)
{
  size_t i;

  /* Is the number of SR conflicts OK?  Either EXPECTED_CONFLICTS is
     not set, and then we want 0 SR, or else it is specified, in which
     case we want equality.  */
  int src_ok = 0;

  int src_total = 0;
  int rrc_total = 0;

  /* Conflicts by state.  */
  for (i = 0; i < nstates; i++)
    if (conflicts[i])
      {
	src_total += count_sr_conflicts (states[i]);
	rrc_total += count_rr_conflicts (states[i]);
      }

  src_ok = src_total == (expected_conflicts == -1 ? 0 : expected_conflicts);

  /* If there are no RR conflicts, and as many SR conflicts as
     expected, then there is nothing to report.  */
  if (!rrc_total && src_ok)
    return;

  /* Report the total number of conflicts on STDERR.  */
  if (yacc_flag)
    {
      /* If invoked with `--yacc', use the output format specified by
	 POSIX.  */
      fprintf (stderr, _("conflicts: "));
      if (src_total > 0)
	fprintf (stderr, _(" %d shift/reduce"), src_total);
      if (src_total > 0 && rrc_total > 0)
	fprintf (stderr, ",");
      if (rrc_total > 0)
	fprintf (stderr, _(" %d reduce/reduce"), rrc_total);
      putc ('\n', stderr);
    }
  else
    {
      fprintf (stderr, _("%s contains "), infile);
      fputs (conflict_report (src_total, rrc_total), stderr);
    }

  if (expected_conflicts != -1 && !src_ok)
    {
      complain_message_count++;
      fprintf (stderr, ngettext ("expected %d shift/reduce conflict\n",
				 "expected %d shift/reduce conflicts\n",
				 expected_conflicts),
	       expected_conflicts);
    }
}


void
free_conflicts (void)
{
  XFREE (conflicts);
  bitset_free (shiftset);
  bitset_free (lookaheadset);
}
