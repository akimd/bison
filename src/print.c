/* Print information on generated parser, for bison,
   Copyright 1984, 1986, 1989, 2000, 2001 Free Software Foundation, Inc.

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

static unsigned *shiftset = NULL;
static unsigned *lookaheadset = NULL;

#if 0
static void
print_token (int extnum, int token)
{
  fprintf (out, _(" type %d is %s\n"), extnum, tags[token]);
}
#endif

static inline const char *
escape (const char *s)
{
  return quotearg_n_style (1, escape_quoting_style, s);
}

/* Be cautious not to use twice the same slot in a single expression. */
static inline const char *
escape2 (const char *s)
{
  return quotearg_n_style (2, escape_quoting_style, s);
}


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

static void
print_core (FILE *out, state_t *state)
{
  int i;
  short *sitems = state->items;
  int snitems   = state->nitems;

  /* New experimental feature: if TRACE_FLAGS output all the items of
     a state, not only its kernel.  */
  if (trace_flag)
    {
      closure (sitems, snitems);
      sitems = itemset;
      snitems = nitemset;
    }

  if (snitems)
    {
      for (i = 0; i < snitems; i++)
	{
	  short *sp;
	  short *sp1;
	  int rule;

	  sp1 = sp = ritem + sitems[i];

	  while (*sp >= 0)
	    sp++;

	  rule = -(*sp);
	  fprintf (out, "    %s  ->  ", escape (tags[rule_table[rule].lhs]));

	  for (sp = ritem + rule_table[rule].rhs; sp < sp1; sp++)
	    fprintf (out, "%s ", escape (tags[*sp]));

	  fputc ('.', out);

	  for (/* Nothing */; *sp >= 0; ++sp)
	    fprintf (out, " %s", escape (tags[*sp]));

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
  shifts *shiftp = state->shifts;

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	int state1 = shiftp->shifts[i];
	int symbol = state_table[state1]->accessing_symbol;
	fprintf (out,
		 _("    %-4s\tshift, and go to state %d\n"),
		 escape (tags[symbol]), state1);
      }

  if (i > 0)
    fputc ('\n', out);
}


static void
print_errs (FILE *out, state_t *state)
{
  errs *errp = state->errs;
  int i;

  for (i = 0; i < errp->nerrs; ++i)
    if (errp->errs[i])
      fprintf (out, _("    %-4s\terror (nonassociative)\n"),
	       escape (tags[errp->errs[i]]));

  if (i > 0)
    fputc ('\n', out);
}


static void
print_gotos (FILE *out, state_t *state)
{
  int i;
  shifts *shiftp = state->shifts;

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    /* Skip token shifts.  */;

  if (i < shiftp->nshifts)
    {
      for (; i < shiftp->nshifts; i++)
	if (!SHIFT_IS_DISABLED (shiftp, i))
	  {
	    int state1 = shiftp->shifts[i];
	    int symbol = state_table[state1]->accessing_symbol;
	    fprintf (out, _("    %-4s\tgo to state %d\n"),
		     escape (tags[symbol]), state1);
	  }

      fputc ('\n', out);
    }
}

static void
print_reductions (FILE *out, state_t *state)
{
  int i;
  shifts *shiftp = state->shifts;
  reductions *redp = state->reductions;
  errs *errp = state->errs;
  int nodefault = 0;

  if (redp->nreds == 0)
    return;

  if (state->consistent)
    {
      int rule = redp->rules[0];
      int symbol = rule_table[rule].lhs;
      fprintf (out, _("    $default\treduce using rule %d (%s)\n\n"),
	       rule - 1, escape (tags[symbol]));
      return;
    }

  for (i = 0; i < tokensetsize; i++)
    shiftset[i] = 0;

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	/* if this state has a shift for the error token, don't use a
	   default rule.  */
	if (SHIFT_IS_ERROR (shiftp, i))
	  nodefault = 1;
	SETBIT (shiftset, SHIFT_SYMBOL (shiftp, i));
      }

  for (i = 0; i < errp->nerrs; i++)
    if (errp->errs[i])
      SETBIT (shiftset, errp->errs[i]);

  if (state->nlookaheads == 1 && !nodefault)
    {
      int k;
      int default_rule = LAruleno[state->lookaheadsp];

      for (k = 0; k < tokensetsize; ++k)
	lookaheadset[k] = LA (state->lookaheadsp)[k] & shiftset[k];

      for (i = 0; i < ntokens; i++)
	if (BITISSET (lookaheadset, i))
	  fprintf (out, _("    %-4s\t[reduce using rule %d (%s)]\n"),
		   escape (tags[i]), default_rule - 1,
		   escape2 (tags[rule_table[default_rule].lhs]));

      fprintf (out, _("    $default\treduce using rule %d (%s)\n\n"),
	       default_rule - 1, escape (tags[rule_table[default_rule].lhs]));
    }
  else if (state->nlookaheads >= 1)
    {
      int cmax = 0;
      int default_LA = -1;
      int default_rule = 0;

      if (!nodefault)
	for (i = 0; i < state->nlookaheads; ++i)
	  {
	    int count = 0;
	    int j, k;

	    for (k = 0; k < tokensetsize; ++k)
	      lookaheadset[k] = LA (state->lookaheadsp + i)[k] & ~shiftset[k];

	    for (j = 0; j < ntokens; j++)
	      if (BITISSET (lookaheadset, j))
		count++;

	    if (count > cmax)
	      {
		cmax = count;
		default_LA = state->lookaheadsp + i;
		default_rule = LAruleno[state->lookaheadsp + i];
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

	  for (j = 0; j < state->nlookaheads; ++j)
	    {
	      if (BITISSET (LA (state->lookaheadsp + j), i))
		{
		  if (count == 0)
		    {
		      if (state->lookaheadsp + j != default_LA)
			fprintf (out,
				 _("    %-4s\treduce using rule %d (%s)\n"),
				 escape (tags[i]),
				 LAruleno[state->lookaheadsp + j] - 1,
				 escape2 (tags[rule_table[LAruleno[state->lookaheadsp + j]].lhs]));
		      else
			defaulted = 1;

		      count++;
		    }
		  else
		    {
		      if (defaulted)
			fprintf (out,
				 _("    %-4s\treduce using rule %d (%s)\n"),
				 escape (tags[i]),
				 LAruleno[default_LA] - 1,
				 escape2 (tags[rule_table[LAruleno[default_LA]].lhs]));
		      defaulted = 0;
		      fprintf (out,
			       _("    %-4s\t[reduce using rule %d (%s)]\n"),
			       escape (tags[i]),
			       LAruleno[state->lookaheadsp + j] - 1,
			       escape2 (tags[rule_table[LAruleno[state->lookaheadsp + j]].lhs]));
		    }
		}
	    }
	}

      if (default_LA >= 0)
	fprintf (out, _("    $default\treduce using rule %d (%s)\n"),
		 default_rule - 1,
		 escape (tags[rule_table[default_rule].lhs]));
    }
}


static void
print_actions (FILE *out, state_t *state)
{
  reductions *redp = state->reductions;
  shifts *shiftp = state->shifts;

  if (shiftp->nshifts == 0 && redp->nreds == 0)
    {
      if (final_state == state->number)
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
  int i, j;
  short *rule;
  char buffer[90];
  int column = 0;

  /* rule # : LHS -> RHS */
  fprintf (out, "%s\n\n", _("Grammar"));
  fprintf (out, "  %s\n", _("Number, Line, Rule"));
  for (i = 1; i <= nrules; i++)
    /* Don't print rules disabled in reduce_grammar_tables.  */
    if (rule_table[i].useful)
      {
	fprintf (out, _("  %3d %3d %s ->"),
		 i - 1, rule_table[i].line, escape (tags[rule_table[i].lhs]));
	rule = &ritem[rule_table[i].rhs];
	if (*rule >= 0)
	  while (*rule >= 0)
	    fprintf (out, " %s", escape (tags[*rule++]));
	else
	  fprintf (out, " /* %s */", _("empty"));
	fputc ('\n', out);
      }
  fputs ("\n\n", out);


  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  for (i = 0; i <= max_user_token_number; i++)
    if (token_translations[i] != 2)
      {
	buffer[0] = 0;
	column = strlen (escape (tags[token_translations[i]]));
	fputs (escape (tags[token_translations[i]]), out);
	END_TEST (50);
	sprintf (buffer, " (%d)", i);

	for (j = 1; j <= nrules; j++)
	  for (rule = &ritem[rule_table[j].rhs]; *rule >= 0; rule++)
	    if (*rule == token_translations[i])
	      {
		END_TEST (65);
		sprintf (buffer + strlen (buffer), " %d", j - 1);
		break;
	      }
	fprintf (out, "%s\n", buffer);
      }
  fputs ("\n\n", out);


  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  for (i = ntokens; i <= nsyms - 1; i++)
    {
      int left_count = 0, right_count = 0;

      for (j = 1; j <= nrules; j++)
	{
	  if (rule_table[j].lhs == i)
	    left_count++;
	  for (rule = &ritem[rule_table[j].rhs]; *rule >= 0; rule++)
	    if (*rule == i)
	      {
		right_count++;
		break;
	      }
	}

      buffer[0] = 0;
      fputs (escape (tags[i]), out);
      column = strlen (escape (tags[i]));
      sprintf (buffer, " (%d)", i);
      END_TEST (0);

      if (left_count > 0)
	{
	  END_TEST (50);
	  sprintf (buffer + strlen (buffer), _(" on left:"));

	  for (j = 1; j <= nrules; j++)
	    {
	      END_TEST (65);
	      if (rule_table[j].lhs == i)
		sprintf (buffer + strlen (buffer), " %d", j - 1);
	    }
	}

      if (right_count > 0)
	{
	  if (left_count > 0)
	    sprintf (buffer + strlen (buffer), ",");
	  END_TEST (50);
	  sprintf (buffer + strlen (buffer), _(" on right:"));
	  for (j = 1; j <= nrules; j++)
	    {
	      for (rule = &ritem[rule_table[j].rhs]; *rule >= 0; rule++)
		if (*rule == i)
		  {
		    END_TEST (65);
		    sprintf (buffer + strlen (buffer), " %d", j - 1);
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
  int i;

  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *out = xfopen (spec_verbose_file, "w");

  size_t size = obstack_object_size (&output_obstack);
  fwrite (obstack_finish (&output_obstack), 1, size, out);
  obstack_free (&output_obstack, NULL);

  if (size)
    fputs ("\n\n", out);

  reduce_output (out);
  conflicts_output (out);

  print_grammar (out);

  /* New experimental feature: output all the items of a state, not
     only its kernel.  Requires to run closure, which need memory
     allocation/deallocation.  */
  if (trace_flag)
    new_closure (nitems);
  /* Storage for print_reductions.  */
  shiftset = XCALLOC (unsigned, tokensetsize);
  lookaheadset = XCALLOC (unsigned, tokensetsize);
  for (i = 0; i < nstates; i++)
    print_state (out, state_table[i]);
  free (shiftset);
  free (lookaheadset);
  if (trace_flag)
    free_closure ();

  xfclose (out);
}
