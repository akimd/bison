/* Output the generated parsing program for bison,
   Copyright 1984, 1986, 1989, 1992, 2000, 2001 Free Software Foundation, Inc.

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


/* The parser tables consist of these tables.
   Starred ones needed only for the semantic parser.
   Double starred are output only if switches are set.

   yytranslate = vector mapping yylex's token numbers into bison's token
   numbers.

   ** yytname = vector of string-names indexed by bison token number

   ** yytoknum = vector of yylex token numbers corresponding to entries
   in yytname

   yyrline = vector of line-numbers of all rules.  For yydebug printouts.

   yyrhs = vector of items of all rules.
   This is exactly what ritems contains.  For yydebug and for semantic
   parser.

   yyprhs[r] = index in yyrhs of first item for rule r.

   yyr1[r] = symbol number of symbol that rule r derives.

   yyr2[r] = number of symbols composing right hand side of rule r.

   * yystos[s] = the symbol number of the symbol that leads to state s.

   yydefact[s] = default rule to reduce with in state s,
   when yytable doesn't specify something else to do.
   Zero means the default is an error.

   yydefgoto[i] = default state to go to after a reduction of a rule that
   generates variable ntokens + i, except when yytable
   specifies something else to do.

   yypact[s] = index in yytable of the portion describing state s.
   The lookahead token's type is used to index that portion
   to find out what to do.

   If the value in yytable is positive,
   we shift the token and go to that state.

   If the value is negative, it is minus a rule number to reduce by.

   If the value is zero, the default action from yydefact[s] is used.

   yypgoto[i] = the index in yytable of the portion describing
   what to do after reducing a rule that derives variable i + ntokens.
   This portion is indexed by the parser state number, s,
   as of before the text for this nonterminal was read.
   The value from yytable is the state to go to if
   the corresponding value in yycheck is s.

   yytable = a vector filled with portions for different uses,
   found via yypact and yypgoto.

   yycheck = a vector indexed in parallel with yytable.
   It indicates, in a roundabout way, the bounds of the
   portion you are trying to examine.

   Suppose that the portion of yytable starts at index p
   and the index to be examined within the portion is i.
   Then if yycheck[p+i] != i, i is outside the bounds
   of what is actually allocated, and the default
   (from yydefact or yydefgoto) should be used.
   Otherwise, yytable[p+i] should be used.

   YYFINAL = the state number of the termination state.
   YYFLAG = most negative short int.  Used to flag ??
   YYNTBASE = ntokens.
*/

#include "system.h"
#include "quotearg.h"
#include "getargs.h"
#include "files.h"
#include "gram.h"
#include "LR0.h"
#include "complain.h"
#include "output.h"
#include "lalr.h"
#include "reader.h"
#include "conflicts.h"
#include "muscle_tab.h"


static int nvectors;
static int nentries;
static short **froms = NULL;
static short **tos = NULL;
static short *tally = NULL;
static short *width = NULL;
static short *actrow = NULL;
static short *state_count = NULL;
static short *order = NULL;
static short *base = NULL;
static short *pos = NULL;
static short *table = NULL;
static short *check = NULL;
static int lowzero;
static int high;

struct obstack muscle_obstack;
struct obstack output_obstack;

int error_verbose = 0;

/* Returns the number of lines of S.  */
static size_t
get_lines_number (const char *s)
{
  size_t lines = 0;
  
  size_t i;
  for (i = 0; s[i]; ++i)
    {
      if (s[i] == '\n')
	++lines;
    }   
  
  return lines;
}


/* FIXME. */

static inline void
output_table_data (struct obstack *oout,
		   short *table_data,
		   short first,
		   short begin,
		   short end)
{
  int i;
  int j = 1;

  obstack_fgrow1 (oout, "%6d", first);
  for (i = begin; i < end; ++i)
    {
      obstack_1grow (oout, ',');
      if (j >= 10)
	{
	  obstack_sgrow (oout, "\n  ");
	  j = 1;
	}
      else
	++j;
      obstack_fgrow1 (oout, "%6d", table_data[i]);
    }
  obstack_1grow (oout, 0);
}


static void
output_token_translations (void)
{
  output_table_data (&output_obstack, token_translations,
		     0, 1, max_user_token_number + 1);
  muscle_insert ("translate", obstack_finish (&output_obstack));
  XFREE (token_translations);
}


static void
output_gram (void)
{
  {
    int i;
    short *values = XCALLOC (short, nrules + 1);
    for (i = 0; i < nrules + 1; ++i)
      values[i] = rule_table[i].rhs;
    output_table_data (&output_obstack, values,
		       0, 1, nrules + 1);
    XFREE (values);
  }

  muscle_insert ("prhs", obstack_finish (&output_obstack));

  {
    size_t yyrhs_size = 1;
    short *yyrhs, *sp;
    int i;

    for (sp = ritem + 1; *sp; sp++)
      ++yyrhs_size;
    yyrhs = XMALLOC (short, yyrhs_size);

    for (sp = ritem + 1, i = 1; *sp; ++sp, ++i)
      yyrhs[i] = *sp > 0 ? *sp : 0;

    output_table_data (&output_obstack, yyrhs,
		       ritem[0], 1, yyrhs_size);
    muscle_insert ("rhs", obstack_finish (&output_obstack));

    XFREE (yyrhs);
  }

#if 0
  if (!semantic_parser)
    obstack_sgrow (&table_obstack, "\n#endif\n");
#endif
}


static void
output_stos (void)
{
  int i;
  short *values = (short *) alloca (sizeof (short) * nstates);
  for (i = 0; i < nstates; ++i)
    values[i] = state_table[i]->accessing_symbol;
  output_table_data (&output_obstack, values,
		     0, 1, nstates);
  muscle_insert ("stos", obstack_finish (&output_obstack));
}


static void
output_rule_data (void)
{
  int i;
  int j;
  short *short_tab = NULL;

  {
    short *values = XCALLOC (short, nrules + 1);
    for (i = 0; i < nrules + 1; ++i)
      values[i] = rule_table[i].line;
    output_table_data (&output_obstack, values,
		       0, 1, nrules + 1);
    muscle_insert ("rline", obstack_finish (&output_obstack));
    XFREE (values);
  }


  j = 0;
  for (i = 0; i < nsyms; i++)
    {
      /* Be sure not to use twice the same quotearg slot. */
      const char *cp =
	quotearg_n_style (1, c_quoting_style,
			  quotearg_style (escape_quoting_style, tags[i]));
      /* Width of the next token, including the two quotes, the coma
	 and the space.  */
      int strsize = strlen (cp) + 2;

      if (j + strsize > 75)
	{
	  obstack_sgrow (&output_obstack, "\n  ");
	  j = 2;
	}

      obstack_sgrow (&output_obstack, cp);
      obstack_sgrow (&output_obstack, ", ");
      j += strsize;
    }
  /* add a NULL entry to list of tokens */
  obstack_sgrow (&output_obstack, "NULL");

  /* Finish table and store. */
  obstack_1grow (&output_obstack, 0);
  muscle_insert ("tname", obstack_finish (&output_obstack));

  /* Output YYTOKNUM. */
  output_table_data (&output_obstack, user_toknums,
		     0, 1, ntokens + 1);
  muscle_insert ("toknum", obstack_finish (&output_obstack));

  /* Output YYR1. */
  {
    short *values = XCALLOC (short, nrules + 1);
    for (i = 0; i < nrules + 1; ++i)
      values[i] = rule_table[i].lhs;
    output_table_data (&output_obstack, values,
		       0, 1, nrules + 1);
    muscle_insert ("r1", obstack_finish (&output_obstack));
    XFREE (values);
  }

  /* Output YYR2. */
  short_tab = XMALLOC (short, nrules + 1);
  for (i = 1; i < nrules; i++)
    short_tab[i] = rule_table[i + 1].rhs - rule_table[i].rhs - 1;
  short_tab[nrules] =  nitems - rule_table[nrules].rhs - 1;
  output_table_data (&output_obstack, short_tab,
		     0, 1, nrules + 1);
  muscle_insert ("r2", obstack_finish (&output_obstack));
  XFREE (short_tab);
}

/*------------------------------------------------------------------.
| Decide what to do for each type of token if seen as the lookahead |
| token in specified state.  The value returned is used as the      |
| default action (yydefact) for the state.  In addition, actrow is  |
| filled with what to do for each kind of token, index by symbol    |
| number, with zero meaning do the default action.  The value       |
| MINSHORT, a very negative number, means this situation is an      |
| error.  The parser recognizes this value specially.               |
|                                                                   |
| This is where conflicts are resolved.  The loop over lookahead    |
| rules considered lower-numbered rules last, and the last rule     |
| considered that likes a token gets to handle it.                  |
`------------------------------------------------------------------*/

static int
action_row (int state)
{
  int i;
  int j;
  int m = 0;
  int n = 0;
  int default_rule;
  int nreds;
  int rule;
  int shift_state;
  int symbol;
  reductions *redp;
  shifts *shiftp;
  errs *errp;
  int nodefault = 0;		/* set nonzero to inhibit having any default reduction */

  for (i = 0; i < ntokens; i++)
    actrow[i] = 0;

  default_rule = 0;
  nreds = 0;
  redp = state_table[state]->reductions;

  if (redp)
    {
      nreds = redp->nreds;

      if (nreds >= 1)
	{
	  /* loop over all the rules available here which require
	     lookahead */
	  m = state_table[state]->lookaheads;
	  n = state_table[state + 1]->lookaheads;

	  for (i = n - 1; i >= m; i--)
	    /* and find each token which the rule finds acceptable
	       to come next */
	    for (j = 0; j < ntokens; j++)
	      /* and record this rule as the rule to use if that
		 token follows.  */
	      if (BITISSET (LA (i), j))
		actrow[j] = -LAruleno[i];
	}
    }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  shiftp = state_table[state]->shifts;
  for (i = 0; i < shiftp->nshifts; i++)
    {
      shift_state = shiftp->shifts[i];
      if (!shift_state)
	continue;

      symbol = state_table[shift_state]->accessing_symbol;

      if (ISVAR (symbol))
	break;

      actrow[symbol] = shift_state;

      /* Do not use any default reduction if there is a shift for
	 error */
      if (symbol == error_token_number)
	nodefault = 1;
    }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record MINSHORT as the action.  */
  errp = state_table[state]->errs;

  if (errp)
    for (i = 0; i < errp->nerrs; i++)
      {
	symbol = errp->errs[i];
	actrow[symbol] = MINSHORT;
      }

  /* Now find the most common reduction and make it the default action
     for this state.  */

  if (nreds >= 1 && !nodefault)
    {
      if (state_table[state]->consistent)
	default_rule = redp->rules[0];
      else
	{
	  int max = 0;
	  for (i = m; i < n; i++)
	    {
	      int count = 0;
	      rule = -LAruleno[i];

	      for (j = 0; j < ntokens; j++)
		{
		  if (actrow[j] == rule)
		    count++;
		}

	      if (count > max)
		{
		  max = count;
		  default_rule = rule;
		}
	    }

	  /* actions which match the default are replaced with zero,
	     which means "use the default" */

	  if (max > 0)
	    {
	      for (j = 0; j < ntokens; j++)
		{
		  if (actrow[j] == default_rule)
		    actrow[j] = 0;
		}

	      default_rule = -default_rule;
	    }
	}
    }

  /* If have no default rule, the default is an error.
     So replace any action which says "error" with "use default".  */

  if (default_rule == 0)
    for (j = 0; j < ntokens; j++)
      {
	if (actrow[j] == MINSHORT)
	  actrow[j] = 0;
      }

  return default_rule;
}


static void
save_row (int state)
{
  int i;
  int count;
  short *sp;
  short *sp1;
  short *sp2;

  count = 0;
  for (i = 0; i < ntokens; i++)
    if (actrow[i] != 0)
      count++;

  if (count == 0)
    return;

  froms[state] = sp1 = sp = XCALLOC (short, count);
  tos[state] = sp2 = XCALLOC (short, count);

  for (i = 0; i < ntokens; i++)
    if (actrow[i] != 0)
      {
	*sp1++ = i;
	*sp2++ = actrow[i];
      }

  tally[state] = count;
  width[state] = sp1[-1] - sp[0] + 1;
}


/*------------------------------------------------------------------.
| Figure out the actions for the specified state, indexed by        |
| lookahead token type.                                             |
|                                                                   |
| The YYDEFACT table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                       |
`------------------------------------------------------------------*/

static void
token_actions (void)
{
  int i;
  short *yydefact = XCALLOC (short, nstates);

  actrow = XCALLOC (short, ntokens);
  for (i = 0; i < nstates; ++i)
    {
      yydefact[i] = action_row (i);
      save_row (i);
    }

  output_table_data (&output_obstack, yydefact,
		     yydefact[0], 1, nstates);
  muscle_insert ("defact", obstack_finish (&output_obstack));

  XFREE (actrow);
  XFREE (yydefact);
}


/*-----------------------------.
| Output the actions to OOUT.  |
`-----------------------------*/

static void
actions_output (FILE *out, size_t *line)
{
  int rule;
  for (rule = 1; rule < nrules + 1; ++rule)
    if (rule_table[rule].action)
      {
	fprintf (out, "  case %d:\n", rule);

	if (!no_lines_flag)
	  fprintf (out, muscle_find ("linef"),
		   rule_table[rule].action_line,
		   quotearg_style (c_quoting_style,
				   muscle_find ("filename")));
	/* As a Bison extension, add the ending semicolon.  Since some
	   Yacc don't do that, help people using bison as a Yacc
	   finding their missing semicolons.  */
	fprintf (out, "{ %s%s }\n    break;\n\n",
		 rule_table[rule].action,
		 yacc_flag ? ";" : "");

	/* We always output 4 '\n' per action.  */
	*line += 4;
	/* Plus one if !no_lines_flag.  */
	if (!no_lines_flag)
	  ++*line;
	/* Get the number of lines written by the user.  */
	*line += get_lines_number (rule_table[rule].action);
      }
}


static void
save_column (int symbol, int default_state)
{
  int i;
  short *sp;
  short *sp1;
  short *sp2;
  int count;
  int symno;

  short begin = goto_map[symbol];
  short end = goto_map[symbol + 1];

  count = 0;
  for (i = begin; i < end; i++)
    if (to_state[i] != default_state)
      count++;

  if (count == 0)
    return;

  symno = symbol - ntokens + nstates;

  froms[symno] = sp1 = sp = XCALLOC (short, count);
  tos[symno] = sp2 = XCALLOC (short, count);

  for (i = begin; i < end; i++)
    if (to_state[i] != default_state)
      {
	*sp1++ = from_state[i];
	*sp2++ = to_state[i];
      }

  tally[symno] = count;
  width[symno] = sp1[-1] - sp[0] + 1;
}

static int
default_goto (int symbol)
{
  int i;
  int m;
  int n;
  int default_state;
  int max;

  m = goto_map[symbol];
  n = goto_map[symbol + 1];

  if (m == n)
    return -1;

  for (i = 0; i < nstates; i++)
    state_count[i] = 0;

  for (i = m; i < n; i++)
    state_count[to_state[i]]++;

  max = 0;
  default_state = -1;

  for (i = 0; i < nstates; i++)
    if (state_count[i] > max)
      {
	max = state_count[i];
	default_state = i;
      }

  return default_state;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

static void
goto_actions (void)
{
  int i;
  short *yydefgoto = XMALLOC (short, nsyms - ntokens);

  state_count = XCALLOC (short, nstates);
  for (i = ntokens; i < nsyms; ++i)
    {
      int default_state = default_goto (i);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }

  output_table_data (&output_obstack, yydefgoto,
		     yydefgoto[0], 1, nsyms - ntokens);
  muscle_insert ("defgoto", obstack_finish (&output_obstack));

  XFREE (state_count);
  XFREE (yydefgoto);
}


/* The next few functions decide how to pack the actions and gotos
   information into yytable. */

static void
sort_actions (void)
{
  int i;
  int j;
  int k;
  int t;
  int w;

  order = XCALLOC (short, nvectors);
  nentries = 0;

  for (i = 0; i < nvectors; i++)
    if (tally[i] > 0)
      {
	t = tally[i];
	w = width[i];
	j = nentries - 1;

	while (j >= 0 && (width[order[j]] < w))
	  j--;

	while (j >= 0 && (width[order[j]] == w) && (tally[order[j]] < t))
	  j--;

	for (k = nentries - 1; k > j; k--)
	  order[k + 1] = order[k];

	order[j + 1] = i;
	nentries++;
      }
}


static int
matching_state (int vector)
{
  int i;
  int j;
  int k;
  int t;
  int w;
  int match;
  int prev;

  i = order[vector];
  if (i >= nstates)
    return -1;

  t = tally[i];
  w = width[i];

  for (prev = vector - 1; prev >= 0; prev--)
    {
      j = order[prev];
      if (width[j] != w || tally[j] != t)
	return -1;

      match = 1;
      for (k = 0; match && k < t; k++)
	if (tos[j][k] != tos[i][k] || froms[j][k] != froms[i][k])
	  match = 0;

      if (match)
	return j;
    }

  return -1;
}


static int
pack_vector (int vector)
{
  int i;
  int j;
  int k;
  int t;
  int loc = 0;
  int ok;
  short *from;
  short *to;

  i = order[vector];
  t = tally[i];

  assert (t);

  from = froms[i];
  to = tos[i];

  for (j = lowzero - from[0]; j < MAXTABLE; j++)
    {
      ok = 1;

      for (k = 0; ok && k < t; k++)
	{
	  loc = j + from[k];
	  if (loc > MAXTABLE)
	    fatal (_("maximum table size (%d) exceeded"), MAXTABLE);

	  if (table[loc] != 0)
	    ok = 0;
	}

      for (k = 0; ok && k < vector; k++)
	if (pos[k] == j)
	  ok = 0;

      if (ok)
	{
	  for (k = 0; k < t; k++)
	    {
	      loc = j + from[k];
	      table[loc] = to[k];
	      check[loc] = from[k];
	    }

	  while (table[lowzero] != 0)
	    lowzero++;

	  if (loc > high)
	    high = loc;

	  return j;
	}
    }

  assert (!"pack_vector");
  return 0;
}


static void
pack_table (void)
{
  int i;
  int place;
  int state;

  base = XCALLOC (short, nvectors);
  pos = XCALLOC (short, nentries);
  table = XCALLOC (short, MAXTABLE);
  check = XCALLOC (short, MAXTABLE);

  lowzero = 0;
  high = 0;

  for (i = 0; i < nvectors; i++)
    base[i] = MINSHORT;

  for (i = 0; i < MAXTABLE; i++)
    check[i] = -1;

  for (i = 0; i < nentries; i++)
    {
      state = matching_state (i);

      if (state < 0)
	place = pack_vector (i);
      else
	place = base[state];

      pos[i] = place;
      base[order[i]] = place;
    }

  for (i = 0; i < nvectors; i++)
    {
      XFREE (froms[i]);
      XFREE (tos[i]);
    }

  XFREE (froms);
  XFREE (tos);
  XFREE (pos);
}

/* the following functions output yytable, yycheck
   and the vectors whose elements index the portion starts */

static void
output_base (void)
{
  /* Output pact. */
  output_table_data (&output_obstack, base,
		     base[0], 1, nstates);
  muscle_insert ("pact", obstack_finish (&output_obstack));

  /* Output pgoto. */
  output_table_data (&output_obstack, base,
		     base[nstates], nstates + 1, nvectors);
  muscle_insert ("pgoto", obstack_finish (&output_obstack));

  XFREE (base);
}


static void
output_table (void)
{
  output_table_data (&output_obstack, table,
		     table[0], 1, high + 1);
  muscle_insert ("table", obstack_finish (&output_obstack));
  XFREE (table);
}


static void
output_check (void)
{
  output_table_data (&output_obstack, check,
		     check[0], 1, high + 1);
  muscle_insert ("check", obstack_finish (&output_obstack));
  XFREE (check);
}

/* compute and output yydefact, yydefgoto, yypact, yypgoto, yytable
   and yycheck.  */

static void
output_actions (void)
{
  int i;
  nvectors = nstates + nvars;

  froms = XCALLOC (short *, nvectors);
  tos = XCALLOC (short *, nvectors);
  tally = XCALLOC (short, nvectors);
  width = XCALLOC (short, nvectors);

  token_actions ();
  XFREE (LA);
  XFREE (LAruleno);

  goto_actions ();
  XFREE (goto_map + ntokens);
  XFREE (from_state);
  XFREE (to_state);

  sort_actions ();
  pack_table ();

  output_base ();
  output_table ();

  output_check ();

  for (i = 0; i < nstates; ++i)
    {
      XFREE (state_table[i]->shifts);
      XFREE (state_table[i]->reductions);
      XFREE (state_table[i]->errs);
      free (state_table[i]);
    }
  XFREE (state_table);
}


/*------------------------------------------------------------.
| Copy the parser code from SKEL_FILENAME into OOUT obstack.  |
| and do the muscle substitution.                             |
`------------------------------------------------------------*/

static void
output_parser (const char *skel_filename, FILE *out)
{
  int c;
  FILE *fskel;
  size_t line;

  fskel = xfopen (skel_filename, "r");

  /* New output code.  */
  line = 1;
  c = getc (fskel);
  while (c != EOF)
    {
      if (c != '%')
	{
	  if (c == '\n')
	    ++line;
	  putc (c, out);
	  c = getc (fskel);
	}
      else if ((c = getc (fskel)) == '%')
	{
	  /* Read the muscle.  */
	  const char *muscle_key = 0;
	  const char *muscle_value = 0;

	  while (isalnum (c = getc (fskel)) || c == '-')
	    obstack_1grow (&muscle_obstack, c);
	  obstack_1grow (&muscle_obstack, 0);

	  /* Output the right value, or see if it's something special.  */
	  muscle_key = obstack_finish (&muscle_obstack);
	  muscle_value = muscle_find (muscle_key);
	  if (!strcmp (muscle_key, "actions"))
	    actions_output (out, &line);
	  else if (!strcmp (muscle_key, "line"))
	    fprintf (out, "%d", line);
	  else if (muscle_value)
	    {
	      fputs (muscle_value, out);
	      line += get_lines_number (muscle_value);
	    }	  
	  else
	    {
	      fputs ("%%", out);
	      fputs (muscle_key, out);
	    }
	}
      else
	putc ('%', out);
    }

  /* End.  */
  xfclose (fskel);
}

/*----------------------------------------.
| Prepare the master parser to be output  |
`----------------------------------------*/

static void
output_master_parser (void)
{
  FILE *parser = xfopen (parser_file_name, "w");
  if (!skeleton)
    {
      if (semantic_parser)
	skeleton = skeleton_find ("BISON_HAIRY", BISON_HAIRY);
      else
	skeleton = skeleton_find ("BISON_SIMPLE", BISON_SIMPLE);
    }
  muscle_insert ("skeleton", skeleton);
  muscle_insert ("parser-file-name", parser_file_name);

  output_parser (skeleton, parser);
  xfclose (parser);
}


/* FIXME. */

#define MUSCLE_INSERT_INT(Key, Value)				\
{								\
  obstack_fgrow1 (&muscle_obstack, "%d", Value);	       	\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
}

#define MUSCLE_INSERT_STRING(Key, Value)			\
{								\
  obstack_sgrow (&muscle_obstack, Value);			\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
}

#define MUSCLE_INSERT_PREFIX(Key, Value)	       			\
{									\
  obstack_fgrow2 (&muscle_obstack, "%s%s", spec_name_prefix, Value);	\
  obstack_1grow (&muscle_obstack, 0);					\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	       	\
}

static void
prepare (void)
{
  MUSCLE_INSERT_INT ("last", high);
  MUSCLE_INSERT_INT ("flag", MINSHORT);
  MUSCLE_INSERT_INT ("pure", pure_parser);
  MUSCLE_INSERT_INT ("nsym", nsyms);
  MUSCLE_INSERT_INT ("debug", debug_flag);
  MUSCLE_INSERT_INT ("final", final_state);
  MUSCLE_INSERT_INT ("maxtok", max_user_token_number);
  MUSCLE_INSERT_INT ("ntbase", ntokens);
  MUSCLE_INSERT_INT ("error-verbose", error_verbose);
  MUSCLE_INSERT_STRING ("prefix", spec_name_prefix);

  MUSCLE_INSERT_INT ("nnts", nvars);
  MUSCLE_INSERT_INT ("nrules", nrules);
  MUSCLE_INSERT_INT ("nstates", nstates);
  MUSCLE_INSERT_INT ("ntokens", ntokens);

  MUSCLE_INSERT_INT ("locations-flag", locations_flag);
}


/*-------------------------.
| Output the header file.  |
`-------------------------*/

static void
header_output (void)
{
  FILE *out = xfopen (spec_defines_file, "w");
  char *macro_name = compute_header_macro ();

  fprintf (out, "#ifndef %s\n", macro_name);
  fprintf (out, "# define %s\n\n", macro_name);

  fputs (muscle_find ("tokendef"), out);
  fprintf (out, "\
#ifndef YYSTYPE\n\
typedef %s
yystype;\n\
# define YYSTYPE yystype\n\
#endif\n",
		    muscle_find ("stype"));

  if (!pure_parser)
    fprintf (out, "\nextern YYSTYPE %slval;\n",
	     spec_name_prefix);
  if (semantic_parser)
    {
      int i;

      for (i = ntokens; i < nsyms; i++)
	/* don't make these for dummy nonterminals made by gensym.  */
	if (*tags[i] != '@')
	  fprintf (out, "# define\tNT%s\t%d\n", tags[i], i);
    }

  fprintf (out, "\n#endif /* not %s */\n", macro_name);
  free (macro_name);
  xfclose (out);
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

void
output (void)
{
  obstack_init (&output_obstack);

  output_token_translations ();
  output_gram ();

  XFREE (ritem);
  if (semantic_parser)
    output_stos ();
  output_rule_data ();
  XFREE (user_toknums);
  output_actions ();

  prepare ();
  /* Copy definitions in directive.  */
  obstack_1grow (&attrs_obstack, 0);
  muscle_insert ("prologue", obstack_finish (&attrs_obstack));

  /* Output the parser. */
  output_master_parser ();
  /* Output the header if needed. */
  if (defines_flag)
    header_output ();

  free (rule_table + 1);
  obstack_free (&muscle_obstack, 0);
  obstack_free (&output_obstack, 0);
  obstack_free (&action_obstack, 0);
}
