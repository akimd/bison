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
print_core (FILE *out, int state)
{
  int i;
  short *sitems = state_table[state].state->items;
  int snitems   = state_table[state].state->nitems;

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

	  while (*sp > 0)
	    sp++;

	  rule = -(*sp);
	  fprintf (out, "    %s  ->  ", tags[rule_table[rule].lhs]);

	  for (sp = ritem + rule_table[rule].rhs; sp < sp1; sp++)
	    fprintf (out, "%s ", tags[*sp]);

	  fputc ('.', out);

	  for (/* Nothing */; *sp > 0; ++sp)
	    fprintf (out, " %s", tags[*sp]);

	  fprintf (out, _("   (rule %d)"), rule);
	  fputc ('\n', out);
	}

      fputc ('\n', out);
    }
}

static void
print_actions (FILE *out, int state)
{
  int i;

  shifts   *shiftp = state_table[state].shifts;
  reductions *redp = state_table[state].reductions;
  errs       *errp = state_table[state].errs;

  if (!shiftp->nshifts && !redp)
    {
      if (final_state == state)
	fprintf (out, _("    $default\taccept\n"));
      else
	fprintf (out, _("    NO ACTIONS\n"));
      return;
    }

  for (i = 0; i < shiftp->nshifts; i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	int state1 = shiftp->shifts[i];
	int symbol = state_table[state1].accessing_symbol;
	/* The following line used to be turned off.  */
	if (ISVAR (symbol))
	  break;
	if (symbol == 0)	/* I.e. strcmp(tags[symbol],"$")==0 */
	  fprintf (out,
		   _("    $   \tgo to state %d\n"), state1);
	else
	  fprintf (out,
		   _("    %-4s\tshift, and go to state %d\n"),
		   tags[symbol], state1);
      }

  if (i > 0)
    fputc ('\n', out);

  if (errp)
    {
      int j;
      for (j = 0; j < errp->nerrs; j++)
	{
	  int symbol = errp->errs[j];
	  if (!symbol)
	    continue;
	  fprintf (out, _("    %-4s\terror (nonassociative)\n"),
		   tags[symbol]);
	}

      if (j > 0)
	fputc ('\n', out);
    }

  if (state_table[state].consistent && redp)
    {
      int rule = redp->rules[0];
      int symbol = rule_table[rule].lhs;
      fprintf (out, _("    $default\treduce using rule %d (%s)\n\n"),
	       rule, tags[symbol]);
    }
  else if (redp)
    {
      print_reductions (out, state);
    }

  if (i < shiftp->nshifts)
    {
      for (; i < shiftp->nshifts; i++)
	if (!SHIFT_IS_DISABLED (shiftp, i))
	  {
	    int state1 = shiftp->shifts[i];
	    int symbol = state_table[state1].accessing_symbol;
	    fprintf (out, _("    %-4s\tgo to state %d\n"),
		     tags[symbol], state1);
	  }

      fputc ('\n', out);
    }
}

static void
print_state (FILE *out, int state)
{
  fprintf (out, _("state %d"), state);
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
		 i, rule_table[i].line, tags[rule_table[i].lhs]);
	rule = &ritem[rule_table[i].rhs];
	if (*rule > 0)
	  while (*rule > 0)
	    fprintf (out, " %s", tags[*rule++]);
	else
	  fprintf (out, " /* %s */", _("empty"));
	fputc ('\n', out);
      }
  fputs ("\n\n", out);


  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  fprintf (out, "%s (-1)\n", tags[0]);

  for (i = 0; i <= max_user_token_number; i++)
    if (token_translations[i] != 2)
      {
	buffer[0] = 0;
	column = strlen (tags[token_translations[i]]);
	fputs (tags[token_translations[i]], out);
	END_TEST (50);
	sprintf (buffer, " (%d)", i);

	for (j = 1; j <= nrules; j++)
	  for (rule = &ritem[rule_table[j].rhs]; *rule > 0; rule++)
	    if (*rule == token_translations[i])
	      {
		END_TEST (65);
		sprintf (buffer + strlen (buffer), " %d", j);
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
	  for (rule = &ritem[rule_table[j].rhs]; *rule > 0; rule++)
	    if (*rule == i)
	      {
		right_count++;
		break;
	      }
	}

      buffer[0] = 0;
      fputs (tags[i], out);
      column = strlen (tags[i]);
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
		sprintf (buffer + strlen (buffer), " %d", j);
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
	      for (rule = &ritem[rule_table[j].rhs]; *rule > 0; rule++)
		if (*rule == i)
		  {
		    END_TEST (65);
		    sprintf (buffer + strlen (buffer), " %d", j);
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
  if (verbose_flag)
    {
      int i;

      /* We used to use just .out if spec_name_prefix (-p) was used, but
	 that conflicts with Posix.  */
      FILE *out = xfopen (spec_verbose_file, "w");

      size_t size = obstack_object_size (&output_obstack);
      fwrite (obstack_finish (&output_obstack), 1, size, out);
      if (size)
	fputs ("\n\n", out);

      reduce_output (out);
      conflicts_output (out);

      print_grammar (out);

      /* New experimental feature: output all the items of a state,
	 not only its kernel.  Requires to run closure, which need
	 memory allocation/deallocation.  */
      if (trace_flag)
	new_closure (nitems);
      for (i = 0; i < nstates; i++)
	print_state (out, i);
      if (trace_flag)
	free_closure ();

      xfclose (out);
    }
  obstack_free (&output_obstack, NULL);
}
