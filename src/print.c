/* Print information on generated parser, for bison,
   Copyright 1984, 1986, 1989, 2000 Free Software Foundation, Inc.

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
#include "xalloc.h"
#include "files.h"
#include "gram.h"
#include "LR0.h"
#include "lalr.h"
#include "conflicts.h"
#include "getargs.h"
#include "state.h"
#include "reader.h"
#include "print.h"

#if 0
static void
print_token (int extnum, int token)
{
  obstack_fgrow2 (&output_obstack, _(" type %d is %s\n"), extnum, tags[token]);
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
      obstack_fgrow1 (&output_obstack, "    %s  ->  ", tags[rlhs[rule]]);

      for (sp = ritem + rrhs[rule]; sp < sp1; sp++)
	{
	  obstack_fgrow1 (&output_obstack, "%s ", tags[*sp]);
	}

      obstack_1grow (&output_obstack, '.');

      while (*sp > 0)
	{
	  obstack_fgrow1 (&output_obstack, " %s", tags[*sp]);
	  sp++;
	}

      obstack_fgrow1 (&output_obstack, _("   (rule %d)"), rule);
      obstack_1grow (&output_obstack, '\n');
    }

  obstack_1grow (&output_obstack, '\n');
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
	obstack_sgrow (&output_obstack, _("    $default\taccept\n"));
      else
	obstack_sgrow (&output_obstack, _("    NO ACTIONS\n"));
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
	    obstack_fgrow1 (&output_obstack,
			    _("    $   \tgo to state %d\n"), state1);
	  else
	    obstack_fgrow2 (&output_obstack,
			    _("    %-4s\tshift, and go to state %d\n"),
			    tags[symbol], state1);
	}

      if (i > 0)
	obstack_1grow (&output_obstack, '\n');
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
	  obstack_fgrow1 (&output_obstack, _("    %-4s\terror (nonassociative)\n"),
		   tags[symbol]);
	}

      if (j > 0)
	obstack_1grow (&output_obstack, '\n');
    }

  if (consistent[state] && redp)
    {
      rule = redp->rules[0];
      symbol = rlhs[rule];
      obstack_fgrow2 (&output_obstack,
		      _("    $default\treduce using rule %d (%s)\n\n"),
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
	  obstack_fgrow2 (&output_obstack,
			  _("    %-4s\tgo to state %d\n"),
			  tags[symbol], state1);
	}

      obstack_1grow (&output_obstack, '\n');
    }
}

static void
print_state (int state)
{
  obstack_sgrow (&output_obstack, "\n\n");
  obstack_fgrow1 (&output_obstack, _("state %d"), state);
  obstack_sgrow (&output_obstack, "\n\n");
  print_core (state);
  print_actions (state);
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

#define END_TEST(End)						\
do {								\
  if (column + strlen(buffer) > (End))				\
    {								\
      obstack_fgrow1 (&output_obstack, "%s\n   ", buffer);	\
      column = 3;						\
      buffer[0] = 0;						\
    }								\
} while (0)


static void
print_grammar (void)
{
  int i, j;
  short *rule;
  char buffer[90];
  int column = 0;

  /* rule # : LHS -> RHS */
  obstack_1grow (&output_obstack, '\n');
  obstack_sgrow (&output_obstack, _("Grammar"));
  obstack_1grow (&output_obstack, '\n');
  for (i = 1; i <= nrules; i++)
    /* Don't print rules disabled in reduce_grammar_tables.  */
    if (rlhs[i] >= 0)
      {
	obstack_fgrow2 (&output_obstack,
			_("rule %-4d %s ->"), i, tags[rlhs[i]]);
	rule = &ritem[rrhs[i]];
	if (*rule > 0)
	  while (*rule > 0)
	    obstack_fgrow1 (&output_obstack, " %s", tags[*rule++]);
	else
	  obstack_sgrow (&output_obstack, _("		/* empty */"));
	obstack_1grow (&output_obstack, '\n');
      }

  /* TERMINAL (type #) : rule #s terminal is on RHS */
  obstack_sgrow (&output_obstack, "\n");
  obstack_sgrow (&output_obstack,
		 _("Terminals, with rules where they appear"));
  obstack_sgrow (&output_obstack, "\n\n");
  obstack_fgrow1 (&output_obstack, "%s (-1)\n", tags[0]);
  if (translations)
    {
      for (i = 0; i <= max_user_token_number; i++)
	if (token_translations[i] != 2)
	  {
	    buffer[0] = 0;
	    column = strlen (tags[token_translations[i]]);
	    obstack_sgrow (&output_obstack, tags[token_translations[i]]);
	    END_TEST (50);
	    sprintf (buffer, " (%d)", i);

	    for (j = 1; j <= nrules; j++)
	      for (rule = &ritem[rrhs[j]]; *rule > 0; rule++)
		if (*rule == token_translations[i])
		  {
		    END_TEST (65);
		    sprintf (buffer + strlen (buffer), " %d", j);
		    break;
		  }
	    obstack_fgrow1 (&output_obstack, "%s\n", buffer);
	  }
    }
  else
    {
      for (i = 1; i < ntokens; i++)
	{
	  buffer[0] = 0;
	  column = strlen (tags[i]);
	  obstack_sgrow (&output_obstack, tags[i]);
	  END_TEST (50);
	  sprintf (buffer, " (%d)", i);

	  for (j = 1; j <= nrules; j++)
	    for (rule = &ritem[rrhs[j]]; *rule > 0; rule++)
	      if (*rule == i)
		{
		  END_TEST (65);
		  sprintf (buffer + strlen (buffer), " %d", j);
		  break;
		}
	  obstack_fgrow1 (&output_obstack, "%s\n", buffer);
	}
    }

  obstack_sgrow (&output_obstack, "\n");
  obstack_sgrow (&output_obstack,
		 _("Nonterminals, with rules where they appear"));
  obstack_sgrow (&output_obstack, "\n\n");
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
      obstack_sgrow (&output_obstack, tags[i]);
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
      obstack_fgrow1 (&output_obstack, "%s\n", buffer);
    }
}

void
print_results (void)
{
  int i;

  if (any_conflicts)
    print_conflicts ();

  if (verbose_flag)
    print_grammar ();

  if (verbose_flag)
    for (i = 0; i < nstates; i++)
      print_state (i);
}
