/* Find and resolve or report look-ahead conflicts for bison,
   Copyright 1984, 1989, 1992, 2000, 2001 Free Software Foundation, Inc.

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
#include "complain.h"
#include "getargs.h"
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

static unsigned *shiftset = NULL;
static unsigned *lookaheadset = NULL;


static inline void
log_resolution (int state, int LAno, int token, const char *resolution)
{
  obstack_fgrow4 (&output_obstack,
		  _("\
Conflict in state %d between rule %d and token %s resolved as %s.\n"),
		  state, LAruleno[LAno], tags[token], resolution);
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift-reduce conflict in |
| favor of the reduction.                                           |
`------------------------------------------------------------------*/

static void
flush_shift (int state, int token)
{
  shifts *shiftp = state_table[state].shifts;
  int i;

  for (i = 0; i < shiftp->nshifts; i++)
    if (!SHIFT_IS_DISABLED (shiftp, i) && SHIFT_SYMBOL (shiftp, i) == token)
      SHIFT_DISABLE (shiftp, i);
}


/*------------------------------------------------------------------.
| Attempt to resolve shift-reduce conflict for one rule by means of |
| precedence declarations.  It has already been checked that the    |
| rule has a precedence.  A conflict is resolved by modifying the   |
| shift or reduce tables so that there is no longer a conflict.     |
`------------------------------------------------------------------*/

static void
resolve_sr_conflict (int state, int lookaheadnum)
{
  int i;
  /* find the rule to reduce by to get precedence of reduction  */
  int redprec = rule_table[LAruleno[lookaheadnum]].prec;
  errs *errp = ERRS_ALLOC (ntokens + 1);
  short *errtokens = errp->errs;

  for (i = 0; i < ntokens; i++)
    if (BITISSET (LA (lookaheadnum), i)
	&& BITISSET (lookaheadset, i)
	&& sprec[i])
      /* Shift-reduce conflict occurs for token number i
	 and it has a precedence.
	 The precedence of shifting is that of token i.  */
      {
	if (sprec[i] < redprec)
	  {
	    log_resolution (state, lookaheadnum, i, _("reduce"));
	    /* flush the shift for this token */
	    RESETBIT (lookaheadset, i);
	    flush_shift (state, i);
	  }
	else if (sprec[i] > redprec)
	  {
	    log_resolution (state, lookaheadnum, i, _("shift"));
	    /* flush the reduce for this token */
	    RESETBIT (LA (lookaheadnum), i);
	  }
	else
	  {
	    /* Matching precedence levels.
	       For left association, keep only the reduction.
	       For right association, keep only the shift.
	       For nonassociation, keep neither.  */

	    switch (sassoc[i])
	      {
	      case right_assoc:
		log_resolution (state, lookaheadnum, i, _("shift"));
		break;

	      case left_assoc:
		log_resolution (state, lookaheadnum, i, _("reduce"));
		break;

	      case non_assoc:
		log_resolution (state, lookaheadnum, i, _("an error"));
		break;
	      }

	    if (sassoc[i] != right_assoc)
	      {
		/* flush the shift for this token */
		RESETBIT (lookaheadset, i);
		flush_shift (state, i);
	      }
	    if (sassoc[i] != left_assoc)
	      {
		/* flush the reduce for this token */
		RESETBIT (LA (lookaheadnum), i);
	      }
	    if (sassoc[i] == non_assoc)
	      {
		/* Record an explicit error for this token.  */
		*errtokens++ = i;
	      }
	  }
      }

  errp->nerrs = errtokens - errp->errs;
  /* Some tokens have been explicitly made errors.  Allocate a
     permanent errs structure for this state, to record them.  */
  i = (char *) errtokens - (char *) errp;
  state_table[state].errs = ERRS_ALLOC (i + 1);
  memcpy (state_table[state].errs, errp, i);
  free (errp);
}


static void
set_conflicts (int state)
{
  int i, j;
  shifts *shiftp;

  if (state_table[state].consistent)
    return;

  for (i = 0; i < tokensetsize; i++)
    lookaheadset[i] = 0;

  shiftp = state_table[state].shifts;
  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      SETBIT (lookaheadset, SHIFT_SYMBOL (shiftp, i));

  /* Loop over all rules which require lookahead in this state.  First
     check for shift-reduce conflict, and try to resolve using
     precedence */
  for (i = state_table[state].lookaheads;
       i < state_table[state + 1].lookaheads;
       ++i)
    if (rule_table[LAruleno[i]].prec)
      for (j = 0; j < tokensetsize; ++j)
	if (LA (i)[j] & lookaheadset[j])
	  {
	    resolve_sr_conflict (state, i);
	    break;
	  }


  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.  */
  for (i = state_table[state].lookaheads;
       i < state_table[state + 1].lookaheads;
       ++i)
    {
      for (j = 0; j < tokensetsize; ++j)
	if (LA (i)[j] & lookaheadset[j])
	  conflicts[state] = 1;

      for (j = 0; j < tokensetsize; ++j)
	lookaheadset[j] |= LA (i)[j];
    }
}

void
solve_conflicts (void)
{
  int i;

  conflicts = XCALLOC (char, nstates);
  shiftset = XCALLOC (unsigned, tokensetsize);
  lookaheadset = XCALLOC (unsigned, tokensetsize);

  for (i = 0; i < nstates; i++)
    set_conflicts (i);
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

static int
count_sr_conflicts (int state)
{
  int i, k;
  int src_count = 0;
  shifts *shiftp = state_table[state].shifts;

  if (!shiftp)
    return 0;

  for (i = 0; i < tokensetsize; i++)
    {
      shiftset[i] = 0;
      lookaheadset[i] = 0;
    }

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      SETBIT (shiftset, SHIFT_SYMBOL (shiftp, i));

  for (i = state_table[state].lookaheads;
       i < state_table[state + 1].lookaheads;
       ++i)
    for (k = 0; k < tokensetsize; ++k)
      lookaheadset[k] |= LA (i)[k];

  for (k = 0; k < tokensetsize; ++k)
    lookaheadset[k] &= shiftset[k];

  for (i = 0; i < ntokens; i++)
    if (BITISSET (lookaheadset, i))
      src_count++;

  return src_count;
}


/*----------------------------------------------.
| Count the number of reduce/reduce conflicts.  |
`----------------------------------------------*/

static int
count_rr_conflicts (int state)
{
  int i;
  int rrc_count = 0;

  int m = state_table[state].lookaheads;
  int n = state_table[state + 1].lookaheads;

  if (n - m < 2)
    return 0;

  for (i = 0; i < ntokens; i++)
    {
      int count = 0;
      int j;
      for (j = m; j < n; j++)
	if (BITISSET (LA (j), i))
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
  int i;
  for (i = 0; i < nstates; i++)
    if (conflicts[i])
      {
	fprintf (out, _("State %d contains "), i);
	fputs (conflict_report (count_sr_conflicts (i),
				count_rr_conflicts (i)), out);
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
  int i;

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
	src_total += count_sr_conflicts (i);
	rrc_total += count_rr_conflicts (i);
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
      /* FIXME: BIG ATTROCIOUS HACK.  For flames, complaints and so
	 on, see Bruno Haible.  GNU Gettext's plural.y's %expect is
	 wrong, and newer Bison reject it.  But then, Bruno will be
	 overwhelmed by bug reports, until a fixed Gettext is
	 released.  So for the 1.3x generation only, we leave a puke
	 puke hack.  */
      if (!strsuffix (infile, "plural.y"))
	complain_message_count++;
      fprintf (stderr, ngettext ("expected %d shift/reduce conflict\n",
				 "expected %d shift/reduce conflicts\n",
				 expected_conflicts),
	       expected_conflicts);
    }
}


void
print_reductions (FILE *out, int state)
{
  int i;
  int m;
  int n;
  shifts *shiftp;
  errs *errp;
  int nodefault = 0;

  for (i = 0; i < tokensetsize; i++)
    shiftset[i] = 0;

  shiftp = state_table[state].shifts;
  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	/* if this state has a shift for the error token, don't use a
	   default rule.  */
	if (SHIFT_IS_ERROR (shiftp, i))
	  nodefault = 1;
	SETBIT (shiftset, SHIFT_SYMBOL (shiftp, i));
      }

  errp = state_table[state].errs;
  if (errp)
    for (i = 0; i < errp->nerrs; i++)
      if (errp->errs[i])
	SETBIT (shiftset, errp->errs[i]);

  m = state_table[state].lookaheads;
  n = state_table[state + 1].lookaheads;

  if (n - m == 1 && !nodefault)
    {
      int k;
      int default_rule = LAruleno[m];

      for (k = 0; k < tokensetsize; ++k)
	lookaheadset[k] = LA (m)[k] & shiftset[k];

      for (i = 0; i < ntokens; i++)
	if (BITISSET (lookaheadset, i))
	  fprintf (out, _("    %-4s\t[reduce using rule %d (%s)]\n"),
		   tags[i], default_rule,
		   tags[rule_table[default_rule].lhs]);

      fprintf (out, _("    $default\treduce using rule %d (%s)\n\n"),
	       default_rule, tags[rule_table[default_rule].lhs]);
    }
  else if (n - m >= 1)
    {
      int cmax = 0;
      int default_LA = -1;
      int default_rule = 0;

      if (!nodefault)
	for (i = m; i < n; i++)
	  {
	    int count = 0;
	    int j, k;

	    for (k = 0; k < tokensetsize; ++k)
	      lookaheadset[k] = LA (i)[k] & ~shiftset[k];

	    for (j = 0; j < ntokens; j++)
	      if (BITISSET (lookaheadset, j))
		count++;

	    if (count > cmax)
	      {
		cmax = count;
		default_LA = i;
		default_rule = LAruleno[i];
	      }

	    for (k = 0; k < tokensetsize; ++k)
	      shiftset[k] |= lookaheadset[k];
	  }

      for (i = 0; i < tokensetsize; i++)
	shiftset[i] = 0;

      for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
	if (!SHIFT_IS_DISABLED (shiftp, i))
	  SETBIT (shiftset, SHIFT_SYMBOL (shiftp, i));

      for (i = 0; i < ntokens; i++)
	{
	  int j;
	  int defaulted = 0;
	  int count = BITISSET (shiftset, i);

	  for (j = m; j < n; j++)
	    {
	      if (BITISSET (LA (j), i))
		{
		  if (count == 0)
		    {
		      if (j != default_LA)
			fprintf (out,
				 _("    %-4s\treduce using rule %d (%s)\n"),
				 tags[i],
				 LAruleno[j],
				 tags[rule_table[LAruleno[j]].lhs]);
		      else
			defaulted = 1;

		      count++;
		    }
		  else
		    {
		      if (defaulted)
			fprintf (out,
				 _("    %-4s\treduce using rule %d (%s)\n"),
				 tags[i],
				 LAruleno[default_LA],
				 tags[rule_table[LAruleno[default_LA]].lhs]);
		      defaulted = 0;
		      fprintf (out,
			       _("    %-4s\t[reduce using rule %d (%s)]\n"),
			       tags[i],
			       LAruleno[j],
			       tags[rule_table[LAruleno[j]].lhs]);
		    }
		}
	    }
	}

      if (default_LA >= 0)
	fprintf (out, _("    $default\treduce using rule %d (%s)\n"),
		 default_rule, tags[rule_table[default_rule].lhs]);
    }
}


void
free_conflicts (void)
{
  XFREE (conflicts);
  XFREE (shiftset);
  XFREE (lookaheadset);
}
