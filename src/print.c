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
  item_number_t *sitems = state->items;
  int snritems   = state->nitems;

  /* New experimental feature: if TRACE_FLAGS output all the items of
     a state, not only its kernel.  */
  if (trace_flag)
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
	  fprintf (out, "    %s  ->  ", escape (rules[rule].lhs->tag));

	  for (sp = rules[rule].rhs; sp < sp1; sp++)
	    fprintf (out, "%s ", escape (symbols[*sp]->tag));

	  fputc ('.', out);

	  for (/* Nothing */; *sp >= 0; ++sp)
	    fprintf (out, " %s", escape (symbols[*sp]->tag));

	  /* Experimental feature: display the lookaheads. */
	  if (trace_flag && state->nlookaheads)
	    {
	      int j, k;
	      int nlookaheads = 0;
	      /* Look for lookaheads corresponding to this rule. */
	      for (j = 0; j < state->nlookaheads; ++j)
		for (k = 0; k < ntokens; ++k)
		  if (bitset_test (LA[state->lookaheadsp + j], k)
		      && LArule[state->lookaheadsp + j]->number == rule)
		    nlookaheads++;
	      if (nlookaheads)
		{
		  fprintf (out, "  [");
		  for (j = 0; j < state->nlookaheads; ++j)
		    for (k = 0; k < ntokens; ++k)
		      if (bitset_test (LA[state->lookaheadsp + j], k)
			  && LArule[state->lookaheadsp + j]->number == rule)
			fprintf (out, "%s%s",
				 quotearg_style (escape_quoting_style,
						 symbols[k]->tag),
				 --nlookaheads ? ", " : "");
		  fprintf (out, "]");
		}
	    }

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
	token_number_t symbol = states[state1]->accessing_symbol;
	fprintf (out,
		 _("    %-4s\tshift, and go to state %d\n"),
		 escape (symbols[symbol]->tag), state1);
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
	       escape (symbols[errp->errs[i]]->tag));

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
	    token_number_t symbol = states[state1]->accessing_symbol;
	    fprintf (out, _("    %-4s\tgo to state %d\n"),
		     escape (symbols[symbol]->tag), state1);
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
      token_number_t symbol = rules[rule].lhs->number;
      fprintf (out, _("    $default\treduce using rule %d (%s)\n\n"),
	       rule - 1, escape (symbols[symbol]->tag));
      return;
    }

  bitset_zero (shiftset);

  for (i = 0; i < shiftp->nshifts && SHIFT_IS_SHIFT (shiftp, i); i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	/* if this state has a shift for the error token, don't use a
	   default rule.  */
	if (SHIFT_IS_ERROR (shiftp, i))
	  nodefault = 1;
	bitset_set (shiftset, SHIFT_SYMBOL (shiftp, i));
      }

  for (i = 0; i < errp->nerrs; i++)
    if (errp->errs[i])
      bitset_set (shiftset, errp->errs[i]);

  if (state->nlookaheads == 1 && !nodefault)
    {
      rule_t *default_rule = LArule[state->lookaheadsp];

      bitset_and (lookaheadset, LA[state->lookaheadsp], shiftset);

      for (i = 0; i < ntokens; i++)
	if (bitset_test (lookaheadset, i))
	  fprintf (out, _("    %-4s\t[reduce using rule %d (%s)]\n"),
		   escape (symbols[i]->tag),
		   default_rule->number - 1,
		   escape2 (default_rule->lhs->tag));

      fprintf (out, _("    $default\treduce using rule %d (%s)\n\n"),
	       default_rule->number - 1,
	       escape (default_rule->lhs->tag));
    }
  else if (state->nlookaheads >= 1)
    {
      int cmax = 0;
      int default_LA = -1;
      rule_t *default_rule = NULL;

      if (!nodefault)
	for (i = 0; i < state->nlookaheads; ++i)
	  {
	    int count = 0;
	    int j;

	    bitset_andn (lookaheadset, LA[state->lookaheadsp + i], shiftset);

	    for (j = 0; j < ntokens; j++)
	      if (bitset_test (lookaheadset, j))
		count++;

	    if (count > cmax)
	      {
		cmax = count;
		default_LA = state->lookaheadsp + i;
		default_rule = LArule[state->lookaheadsp + i];
	      }

	    bitset_or (shiftset, shiftset, lookaheadset);
	  }

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
	    if (bitset_test (LA[state->lookaheadsp + j], i))
	      {
		if (count == 0)
		  {
		    if (state->lookaheadsp + j != default_LA)
		      fprintf (out,
			       _("    %-4s\treduce using rule %d (%s)\n"),
			       escape (symbols[i]->tag),
			       LArule[state->lookaheadsp + j]->number - 1,
			       escape2 (LArule[state->lookaheadsp + j]->lhs->tag));
		    else
		      defaulted = 1;

		    count++;
		  }
		else
		  {
		    if (defaulted)
		      fprintf (out,
			       _("    %-4s\treduce using rule %d (%s)\n"),
			       escape (symbols[i]->tag),
			       LArule[default_LA]->number - 1,
			       escape2 (LArule[default_LA]->lhs->tag));
		    defaulted = 0;
		    fprintf (out,
			     _("    %-4s\t[reduce using rule %d (%s)]\n"),
			     escape (symbols[i]->tag),
			     LArule[state->lookaheadsp + j]->number - 1,
			     escape2 (LArule[state->lookaheadsp + j]->lhs->tag));
		  }
	      }
	}

      if (default_LA >= 0)
	fprintf (out, _("    $default\treduce using rule %d (%s)\n"),
		 default_rule->number - 1,
		 escape (default_rule->lhs->tag));
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
  token_number_t i;
  int j;
  item_number_t *rule;
  char buffer[90];
  int column = 0;

  /* rule # : LHS -> RHS */
  fprintf (out, "%s\n\n", _("Grammar"));
  fprintf (out, "  %s\n", _("Number, Line, Rule"));
  for (j = 1; j < nrules + 1; j++)
    {
      fprintf (out, _("  %3d %3d %s ->"),
	       j - 1, rules[j].line, escape (rules[j].lhs->tag));
      rule = rules[j].rhs;
      if (*rule >= 0)
	while (*rule >= 0)
	  fprintf (out, " %s", escape (symbols[*rule++]->tag));
      else
	fprintf (out, " /* %s */", _("empty"));
      fputc ('\n', out);
    }
  fputs ("\n\n", out);


  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  for (i = 0; i < max_user_token_number + 1; i++)
    if (token_translations[i] != undeftoken->number)
      {
	buffer[0] = 0;
	column = strlen (escape (symbols[token_translations[i]]->tag));
	fputs (escape (symbols[token_translations[i]]->tag), out);
	END_TEST (50);
	sprintf (buffer, " (%d)", i);

	for (j = 1; j < nrules + 1; j++)
	  for (rule = rules[j].rhs; *rule >= 0; rule++)
	    if (item_number_as_token_number (*rule) == token_translations[i])
	      {
		END_TEST (65);
		sprintf (buffer + strlen (buffer), " %d", j - 1);
		break;
	      }
	fprintf (out, "%s\n", buffer);
      }
  fputs ("\n\n", out);


  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  for (i = ntokens; i < nsyms; i++)
    {
      int left_count = 0, right_count = 0;

      for (j = 1; j < nrules + 1; j++)
	{
	  if (rules[j].lhs->number == i)
	    left_count++;
	  for (rule = rules[j].rhs; *rule >= 0; rule++)
	    if (item_number_as_token_number (*rule) == i)
	      {
		right_count++;
		break;
	      }
	}

      buffer[0] = 0;
      fputs (escape (symbols[i]->tag), out);
      column = strlen (escape (symbols[i]->tag));
      sprintf (buffer, " (%d)", i);
      END_TEST (0);

      if (left_count > 0)
	{
	  END_TEST (50);
	  sprintf (buffer + strlen (buffer), _(" on left:"));

	  for (j = 1; j < nrules + 1; j++)
	    {
	      END_TEST (65);
	      if (rules[j].lhs->number == i)
		sprintf (buffer + strlen (buffer), " %d", j - 1);
	    }
	}

      if (right_count > 0)
	{
	  if (left_count > 0)
	    sprintf (buffer + strlen (buffer), ",");
	  END_TEST (50);
	  sprintf (buffer + strlen (buffer), _(" on right:"));
	  for (j = 1; j < nrules + 1; j++)
	    {
	      for (rule = rules[j].rhs; *rule >= 0; rule++)
		if (item_number_as_token_number (*rule) == i)
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
  size_t i;

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
    new_closure (nritems);
  /* Storage for print_reductions.  */
  shiftset =  bitset_create (ntokens, BITSET_FIXED);
  lookaheadset = bitset_create (ntokens, BITSET_FIXED);
  for (i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (shiftset);
  bitset_free (lookaheadset);
  if (trace_flag)
    free_closure ();

  xfclose (out);
}
