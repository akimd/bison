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
#include "getargs.h"
#include "xalloc.h"
#include "files.h"
#include "gram.h"
#include "state.h"
#include "lalr.h"
#include "conflicts.h"
#include "reader.h"
#include "LR0.h"

int any_conflicts = 0;
errs **err_table;
int expected_conflicts;
static char *conflicts;

static unsigned *shiftset;
static unsigned *lookaheadset;
static int src_total;
static int rrc_total;
static int src_count;
static int rrc_count;


static inline void
log_resolution (int state, int LAno, int token, char *resolution)
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
  shifts *shiftp;
  int k, i;

  shiftp = shift_table[state];

  if (shiftp)
    {
      k = shiftp->nshifts;
      for (i = 0; i < k; i++)
	{
	  if (shiftp->shifts[i]
	      && token == accessing_symbol[shiftp->shifts[i]])
	    (shiftp->shifts[i]) = 0;
	}
    }
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
  int mask;
  unsigned *fp1;
  unsigned *fp2;
  int redprec;
  errs *errp = (errs *) xcalloc (sizeof (errs) + ntokens * sizeof (short), 1);
  short *errtokens = errp->errs;

  /* find the rule to reduce by to get precedence of reduction  */
  redprec = rprec[LAruleno[lookaheadnum]];

  mask = 1;
  fp1 = LA + lookaheadnum * tokensetsize;
  fp2 = lookaheadset;
  for (i = 0; i < ntokens; i++)
    {
      if ((mask & *fp2 & *fp1) && sprec[i])
	/* Shift-reduce conflict occurs for token number i
	   and it has a precedence.
	   The precedence of shifting is that of token i.  */
	{
	  if (sprec[i] < redprec)
	    {
	      log_resolution (state, lookaheadnum, i, _("reduce"));
	      *fp2 &= ~mask;	/* flush the shift for this token */
	      flush_shift (state, i);
	    }
	  else if (sprec[i] > redprec)
	    {
	      log_resolution (state, lookaheadnum, i, _("shift"));
	      *fp1 &= ~mask;	/* flush the reduce for this token */
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
		  *fp2 &= ~mask;	/* flush the shift for this token */
		  flush_shift (state, i);
		}
	      if (sassoc[i] != left_assoc)
		{
		  *fp1 &= ~mask;	/* flush the reduce for this token */
		}
	      if (sassoc[i] == non_assoc)
		{
		  /* Record an explicit error for this token.  */
		  *errtokens++ = i;
		}
	    }
	}

      mask <<= 1;
      if (mask == 0)
	{
	  mask = 1;
	  fp2++;
	  fp1++;
	}
    }
  errp->nerrs = errtokens - errp->errs;
  if (errp->nerrs)
    {
      /* Some tokens have been explicitly made errors.  Allocate
         a permanent errs structure for this state, to record them.  */
      i = (char *) errtokens - (char *) errp;
      err_table[state] = (errs *) xcalloc ((unsigned int) i, 1);
      bcopy (errp, err_table[state], i);
    }
  else
    err_table[state] = 0;
  free (errp);
}


static void
set_conflicts (int state)
{
  int i;
  int k;
  shifts *shiftp;
  unsigned *fp2;
  unsigned *fp3;
  unsigned *fp4;
  unsigned *fp1;
  int symbol;

  if (consistent[state])
    return;

  for (i = 0; i < tokensetsize; i++)
    lookaheadset[i] = 0;

  shiftp = shift_table[state];
  if (shiftp)
    {
      k = shiftp->nshifts;
      for (i = 0; i < k; i++)
	{
	  symbol = accessing_symbol[shiftp->shifts[i]];
	  if (ISVAR (symbol))
	    break;
	  SETBIT (lookaheadset, symbol);
	}
    }

  k = lookaheads[state + 1];
  fp4 = lookaheadset + tokensetsize;

  /* Loop over all rules which require lookahead in this state.  First
     check for shift-reduce conflict, and try to resolve using
     precedence */
  for (i = lookaheads[state]; i < k; i++)
    if (rprec[LAruleno[i]])
      {
	fp1 = LA + i * tokensetsize;
	fp2 = fp1;
	fp3 = lookaheadset;

	while (fp3 < fp4)
	  {
	    if (*fp2++ & *fp3++)
	      {
		resolve_sr_conflict (state, i);
		break;
	      }
	  }
      }


  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.  */
  for (i = lookaheads[state]; i < k; i++)
    {
      fp1 = LA + i * tokensetsize;
      fp2 = fp1;
      fp3 = lookaheadset;

      while (fp3 < fp4)
	{
	  if (*fp2++ & *fp3++)
	    {
	      conflicts[state] = 1;
	      any_conflicts = 1;
	    }
	}

      fp2 = fp1;
      fp3 = lookaheadset;

      while (fp3 < fp4)
	*fp3++ |= *fp2++;
    }
}

void
initialize_conflicts (void)
{
  int i;

  conflicts = XCALLOC (char, nstates);
  shiftset = XCALLOC (unsigned, tokensetsize);
  lookaheadset = XCALLOC (unsigned, tokensetsize);

  err_table = XCALLOC (errs *, nstates);

  any_conflicts = 0;

  for (i = 0; i < nstates; i++)
    set_conflicts (i);
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

static void
count_sr_conflicts (int state)
{
  int i;
  int k;
  int mask;
  shifts *shiftp;
  unsigned *fp1;
  unsigned *fp2;
  unsigned *fp3;
  int symbol;

  src_count = 0;

  shiftp = shift_table[state];
  if (!shiftp)
    return;

  for (i = 0; i < tokensetsize; i++)
    {
      shiftset[i] = 0;
      lookaheadset[i] = 0;
    }

  k = shiftp->nshifts;
  for (i = 0; i < k; i++)
    {
      if (!shiftp->shifts[i])
	continue;
      symbol = accessing_symbol[shiftp->shifts[i]];
      if (ISVAR (symbol))
	break;
      SETBIT (shiftset, symbol);
    }

  k = lookaheads[state + 1];
  fp3 = lookaheadset + tokensetsize;

  for (i = lookaheads[state]; i < k; i++)
    {
      fp1 = LA + i * tokensetsize;
      fp2 = lookaheadset;

      while (fp2 < fp3)
	*fp2++ |= *fp1++;
    }

  fp1 = shiftset;
  fp2 = lookaheadset;

  while (fp2 < fp3)
    *fp2++ &= *fp1++;

  mask = 1;
  fp2 = lookaheadset;
  for (i = 0; i < ntokens; i++)
    {
      if (mask & *fp2)
	src_count++;

      mask <<= 1;
      if (mask == 0)
	{
	  mask = 1;
	  fp2++;
	}
    }
}


/*----------------------------------------------.
| Count the number of reduce/reduce conflicts.  |
`----------------------------------------------*/

static void
count_rr_conflicts (int state)
{
  int i;
  int j;
  int count;
  unsigned mask;
  unsigned *baseword;
  unsigned *wordp;
  int m;
  int n;

  rrc_count = 0;

  m = lookaheads[state];
  n = lookaheads[state + 1];

  if (n - m < 2)
    return;

  mask = 1;
  baseword = LA + m * tokensetsize;
  for (i = 0; i < ntokens; i++)
    {
      wordp = baseword;

      count = 0;
      for (j = m; j < n; j++)
	{
	  if (mask & *wordp)
	    count++;

	  wordp += tokensetsize;
	}

      if (count >= 2)
	rrc_count++;

      mask <<= 1;
      if (mask == 0)
	{
	  mask = 1;
	  baseword++;
	}
    }
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

  if (src_num == 1)
    {
      sprintf (cp, _(" 1 shift/reduce conflict"));
      cp += strlen (cp);
    }
  else if (src_num > 1)
    {
      sprintf (cp, _(" %d shift/reduce conflicts"), src_num);
      cp += strlen (cp);
    }

  if (src_num > 0 && rrc_num > 0)
    {
      sprintf (cp, _(" and"));
      cp += strlen (cp);
    }

  if (rrc_num == 1)
    {
      sprintf (cp, _(" 1 reduce/reduce conflict"));
      cp += strlen (cp);
    }
  else if (rrc_num > 1)
    {
      sprintf (cp, _(" %d reduce/reduce conflicts"), rrc_num);
      cp += strlen (cp);
    }

  *cp++ = '.';
  *cp++ = '\n';
  *cp++ = '\0';

  return res;
}


/*---------------------------------------------.
| Compute and give a report on the conflicts.  |
`---------------------------------------------*/

void
print_conflicts (void)
{
  int i;

  src_total = 0;
  rrc_total = 0;

  /* Count the total number of conflicts, and if wanted, give a
     detailed report in FOUTPUT.  */
  for (i = 0; i < nstates; i++)
    {
      if (conflicts[i])
	{
	  count_sr_conflicts (i);
	  count_rr_conflicts (i);
	  src_total += src_count;
	  rrc_total += rrc_count;

	  if (verbose_flag)
	    {
	      obstack_fgrow1 (&output_obstack, _("State %d contains"), i);
	      obstack_sgrow (&output_obstack,
			     conflict_report (src_count, rrc_count));
	    }
	}
    }

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
      fprintf (stderr, _("%s contains"), infile);
      fputs (conflict_report (src_total, rrc_total), stderr);
    }
}


void
print_reductions (int state)
{
  int i;
  int j;
  int k;
  unsigned *fp1;
  unsigned *fp2;
  unsigned *fp3;
  unsigned *fp4;
  int rule;
  int symbol;
  unsigned mask;
  int m;
  int n;
  int default_LA;
  int default_rule = 0;
  int cmax;
  int count;
  shifts *shiftp;
  errs *errp;
  int nodefault = 0;

  for (i = 0; i < tokensetsize; i++)
    shiftset[i] = 0;

  shiftp = shift_table[state];
  if (shiftp)
    {
      k = shiftp->nshifts;
      for (i = 0; i < k; i++)
	{
	  if (!shiftp->shifts[i])
	    continue;
	  symbol = accessing_symbol[shiftp->shifts[i]];
	  if (ISVAR (symbol))
	    break;
	  /* if this state has a shift for the error token,
	     don't use a default rule.  */
	  if (symbol == error_token_number)
	    nodefault = 1;
	  SETBIT (shiftset, symbol);
	}
    }

  errp = err_table[state];
  if (errp)
    {
      k = errp->nerrs;
      for (i = 0; i < k; i++)
	{
	  if (!errp->errs[i])
	    continue;
	  symbol = errp->errs[i];
	  SETBIT (shiftset, symbol);
	}
    }

  m = lookaheads[state];
  n = lookaheads[state + 1];

  if (n - m == 1 && !nodefault)
    {
      default_rule = LAruleno[m];

      fp1 = LA + m * tokensetsize;
      fp2 = shiftset;
      fp3 = lookaheadset;
      fp4 = lookaheadset + tokensetsize;

      while (fp3 < fp4)
	*fp3++ = *fp1++ & *fp2++;

      mask = 1;
      fp3 = lookaheadset;

      for (i = 0; i < ntokens; i++)
	{
	  if (mask & *fp3)
	    obstack_fgrow3 (&output_obstack,
			    _("    %-4s\t[reduce using rule %d (%s)]\n"),
			    tags[i], default_rule, tags[rlhs[default_rule]]);

	  mask <<= 1;
	  if (mask == 0)
	    {
	      mask = 1;
	      fp3++;
	    }
	}

      obstack_fgrow2 (&output_obstack,
		      _("    $default\treduce using rule %d (%s)\n\n"),
		      default_rule, tags[rlhs[default_rule]]);
    }
  else if (n - m >= 1)
    {
      cmax = 0;
      default_LA = -1;
      fp4 = lookaheadset + tokensetsize;

      if (!nodefault)
	for (i = m; i < n; i++)
	  {
	    fp1 = LA + i * tokensetsize;
	    fp2 = shiftset;
	    fp3 = lookaheadset;

	    while (fp3 < fp4)
	      *fp3++ = *fp1++ & (~(*fp2++));

	    count = 0;
	    mask = 1;
	    fp3 = lookaheadset;
	    for (j = 0; j < ntokens; j++)
	      {
		if (mask & *fp3)
		  count++;

		mask <<= 1;
		if (mask == 0)
		  {
		    mask = 1;
		    fp3++;
		  }
	      }

	    if (count > cmax)
	      {
		cmax = count;
		default_LA = i;
		default_rule = LAruleno[i];
	      }

	    fp2 = shiftset;
	    fp3 = lookaheadset;

	    while (fp3 < fp4)
	      *fp2++ |= *fp3++;
	  }

      for (i = 0; i < tokensetsize; i++)
	shiftset[i] = 0;

      if (shiftp)
	{
	  k = shiftp->nshifts;
	  for (i = 0; i < k; i++)
	    {
	      if (!shiftp->shifts[i])
		continue;
	      symbol = accessing_symbol[shiftp->shifts[i]];
	      if (ISVAR (symbol))
		break;
	      SETBIT (shiftset, symbol);
	    }
	}

      mask = 1;
      fp1 = LA + m * tokensetsize;
      fp2 = shiftset;
      for (i = 0; i < ntokens; i++)
	{
	  int defaulted = 0;

	  if (mask & *fp2)
	    count = 1;
	  else
	    count = 0;

	  fp3 = fp1;
	  for (j = m; j < n; j++)
	    {
	      if (mask & *fp3)
		{
		  if (count == 0)
		    {
		      if (j != default_LA)
			{
			  rule = LAruleno[j];
			  obstack_fgrow3 (&output_obstack,
				   _("    %-4s\treduce using rule %d (%s)\n"),
				   tags[i], rule, tags[rlhs[rule]]);
			}
		      else
			defaulted = 1;

		      count++;
		    }
		  else
		    {
		      if (defaulted)
			{
			  rule = LAruleno[default_LA];
			  obstack_fgrow3 (&output_obstack,
				   _("    %-4s\treduce using rule %d (%s)\n"),
				   tags[i], rule, tags[rlhs[rule]]);
			  defaulted = 0;
			}
		      rule = LAruleno[j];
		      obstack_fgrow3 (&output_obstack,
			       _("    %-4s\t[reduce using rule %d (%s)]\n"),
			       tags[i], rule, tags[rlhs[rule]]);
		    }
		}

	      fp3 += tokensetsize;
	    }

	  mask <<= 1;
	  if (mask == 0)
	    {
	      mask = 1;
	      /* We tried incrementing just fp1, and just fp2; both seem wrong.
	         It seems necessary to increment both in sync.  */
	      fp1++;
	      fp2++;
	    }
	}

      if (default_LA >= 0)
	obstack_fgrow2 (&output_obstack,
			_("    $default\treduce using rule %d (%s)\n"),
			default_rule, tags[rlhs[default_rule]]);

      obstack_1grow (&output_obstack, '\n');
    }
}


void
finalize_conflicts (void)
{
  XFREE (conflicts);
  XFREE (shiftset);
  XFREE (lookaheadset);
}
