/* Print information on generated parser, for bison,
   Copyright (C) 1984, 1986, 1989, 2000 Free Software Foundation, Inc.

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
#include "alloc.h"
#include "files.h"
#include "gram.h"
#include "state.h"
#include "lalr.h"
#include "conflicts.h"
#include "getargs.h"
#include "state.h"

extern char **tags;
extern int final_state;

#if 0
static void
print_token (int extnum, int token)
{
  fprintf (foutput, _(" type %d is %s\n"), extnum, tags[token]);
}
#endif


/*================================\
| Report information on a state.  |
\================================*/

static void
print_core (int state)
{
  int i;
  int k;
  int rule;
  core *statep;
  short *sp;
  short *sp1;

  statep = state_table[state];
  k = statep->nitems;

  if (k == 0)
    return;

  for (i = 0; i < k; i++)
    {
      sp1 = sp = ritem + statep->items[i];

      while (*sp > 0)
	sp++;

      rule = -(*sp);
      fprintf (foutput, "    %s  ->  ", tags[rlhs[rule]]);

      for (sp = ritem + rrhs[rule]; sp < sp1; sp++)
	{
	  fprintf (foutput, "%s ", tags[*sp]);
	}

      putc ('.', foutput);

      while (*sp > 0)
	{
	  fprintf (foutput, " %s", tags[*sp]);
	  sp++;
	}

      fprintf (foutput, _("   (rule %d)"), rule);
      putc ('\n', foutput);
    }

  putc ('\n', foutput);
}

static void
print_actions (int state)
{
  int i;
  int k;
  int state1;
  int symbol;
  shifts *shiftp;
  errs *errp;
  reductions *redp;
  int rule;

  shiftp = shift_table[state];
  redp = reduction_table[state];
  errp = err_table[state];

  if (!shiftp && !redp)
    {
      if (final_state == state)
	fprintf (foutput, _("    $default\taccept\n"));
      else
	fprintf (foutput, _("    NO ACTIONS\n"));
      return;
    }

  if (shiftp)
    {
      k = shiftp->nshifts;

      for (i = 0; i < k; i++)
	{
	  if (!shiftp->shifts[i])
	    continue;
	  state1 = shiftp->shifts[i];
	  symbol = accessing_symbol[state1];
	  /* The following line used to be turned off.  */
	  if (ISVAR (symbol))
	    break;
	  if (symbol == 0)	/* I.e. strcmp(tags[symbol],"$")==0 */
	    fprintf (foutput, _("    $   \tgo to state %d\n"), state1);
	  else
	    fprintf (foutput, _("    %-4s\tshift, and go to state %d\n"),
		     tags[symbol], state1);
	}

      if (i > 0)
	putc ('\n', foutput);
    }
  else
    {
      i = 0;
      k = 0;
    }

  if (errp)
    {
      int j, nerrs;

      nerrs = errp->nerrs;

      for (j = 0; j < nerrs; j++)
	{
	  if (!errp->errs[j])
	    continue;
	  symbol = errp->errs[j];
	  fprintf (foutput, _("    %-4s\terror (nonassociative)\n"),
		   tags[symbol]);
	}

      if (j > 0)
	putc ('\n', foutput);
    }

  if (consistent[state] && redp)
    {
      rule = redp->rules[0];
      symbol = rlhs[rule];
      fprintf (foutput, _("    $default\treduce using rule %d (%s)\n\n"),
	       rule, tags[symbol]);
    }
  else if (redp)
    {
      print_reductions (state);
    }

  if (i < k)
    {
      for (; i < k; i++)
	{
	  if (!shiftp->shifts[i])
	    continue;
	  state1 = shiftp->shifts[i];
	  symbol = accessing_symbol[state1];
	  fprintf (foutput, _("    %-4s\tgo to state %d\n"), tags[symbol],
		   state1);
	}

      putc ('\n', foutput);
    }
}

static void
print_state (int state)
{
  fprintf (foutput, _("\n\nstate %d\n\n"), state);
  print_core (state);
  print_actions (state);
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

#define END_TEST(end)				\
  do {						\
    if (column + strlen(buffer) > (end)) {	\
      fprintf (foutput, "%s\n   ", buffer);	\
      column = 3;				\
      buffer[0] = 0;				\
    }						\
  } while (0)


static void
print_grammar (void)
{
  int i, j;
  short *rule;
  char buffer[90];
  int column = 0;

  /* rule # : LHS -> RHS */
  fputs (_("\nGrammar\n"), foutput);
  for (i = 1; i <= nrules; i++)
    /* Don't print rules disabled in reduce_grammar_tables.  */
    if (rlhs[i] >= 0)
      {
	fprintf (foutput, _("rule %-4d %s ->"), i, tags[rlhs[i]]);
	rule = &ritem[rrhs[i]];
	if (*rule > 0)
	  while (*rule > 0)
	    fprintf (foutput, " %s", tags[*rule++]);
	else
	  fputs (_("		/* empty */"), foutput);
	putc ('\n', foutput);
      }

  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fputs (_("\nTerminals, with rules where they appear\n\n"), foutput);
  fprintf (foutput, "%s (-1)\n", tags[0]);
  if (translations)
    {
      for (i = 0; i <= max_user_token_number; i++)
	if (token_translations[i] != 2)
	  {
	    buffer[0] = 0;
	    column = strlen (tags[token_translations[i]]);
	    fprintf (foutput, "%s", tags[token_translations[i]]);
	    END_TEST (50);
	    sprintf (buffer, " (%d)", i);

	    for (j = 1; j <= nrules; j++)
	      {
		for (rule = &ritem[rrhs[j]]; *rule > 0; rule++)
		  if (*rule == token_translations[i])
		    {
		      END_TEST (65);
		      sprintf (buffer + strlen (buffer), " %d", j);
		      break;
		    }
	      }
	    fprintf (foutput, "%s\n", buffer);
	  }
    }
  else
    for (i = 1; i < ntokens; i++)
      {
	buffer[0] = 0;
	column = strlen (tags[i]);
	fprintf (foutput, "%s", tags[i]);
	END_TEST (50);
	sprintf (buffer, " (%d)", i);

	for (j = 1; j <= nrules; j++)
	  {
	    for (rule = &ritem[rrhs[j]]; *rule > 0; rule++)
	      if (*rule == i)
		{
		  END_TEST (65);
		  sprintf (buffer + strlen (buffer), " %d", j);
		  break;
		}
	  }
	fprintf (foutput, "%s\n", buffer);
      }

  fputs (_("\nNonterminals, with rules where they appear\n\n"), foutput);
  for (i = ntokens; i <= nsyms - 1; i++)
    {
      int left_count = 0, right_count = 0;

      for (j = 1; j <= nrules; j++)
	{
	  if (rlhs[j] == i)
	    left_count++;
	  for (rule = &ritem[rrhs[j]]; *rule > 0; rule++)
	    if (*rule == i)
	      {
		right_count++;
		break;
	      }
	}

      buffer[0] = 0;
      fprintf (foutput, "%s", tags[i]);
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
	      if (rlhs[j] == i)
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
	      for (rule = &ritem[rrhs[j]]; *rule > 0; rule++)
		if (*rule == i)
		  {
		    END_TEST (65);
		    sprintf (buffer + strlen (buffer), " %d", j);
		    break;
		  }
	    }
	}
      fprintf (foutput, "%s\n", buffer);
    }
}

void
print_results (void)
{
  int i;

  if (any_conflicts)
    print_conflicts ();

  if (verboseflag)
    print_grammar ();

  if (verboseflag)
    for (i = 0; i < nstates; i++)
      print_state (i);
}
