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
#include "obstack.h"
#include "quotearg.h"
#include "getargs.h"
#include "xalloc.h"
#include "files.h"
#include "gram.h"
#include "LR0.h"
#include "complain.h"
#include "output.h"
#include "lalr.h"
#include "reader.h"
#include "conflicts.h"
#include "muscle_tab.h"

extern void berror PARAMS((const char *));

static int nvectors;
static int nentries;
static short **froms;
static short **tos;
static short *tally;
static short *width;
static short *actrow;
static short *state_count;
static short *order;
static short *base;
static short *pos;
static short *table;
static short *check;
static int lowzero;
static int high;

struct obstack muscle_obstack;
struct obstack output_obstack;

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
}


static void
output_gram (void)
{
  output_table_data (&output_obstack, rrhs, 
		     0, 1, nrules + 1);
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
  if (!semantic_parser && !no_parser_flag)
    obstack_sgrow (&table_obstack, "\n#endif\n");
#endif
}


static void
output_stos (void)
{
  output_table_data (&output_obstack, accessing_symbol, 
		     0, 1, nstates);
  muscle_insert ("stos", obstack_finish (&output_obstack));
}


static void
output_rule_data (void)
{
  int i;
  int j;
  short *short_tab = NULL;

  output_table_data (&output_obstack, rline,
		     0, 1, nrules + 1);
  muscle_insert ("rline", obstack_finish (&output_obstack));

  j = 0;
  for (i = 0; i < nsyms; i++)
    /* this used to be i<=nsyms, but that output a final "" symbol
       almost by accident */
    {
      /* Width of the next token, including the two quotes, the coma
	 and the space.  */
      int strsize = 4;
      char *p;

      for (p = tags[i]; p && *p; p++)
	if (*p == '"' || *p == '\\' || *p == '\n' || *p == '\t'
	    || *p == '\b')
	  strsize += 2;
	else if (*p < 040 || *p >= 0177)
	  strsize += 4;
	else
	  strsize++;

      if (j + strsize > 75)
	{
	  obstack_sgrow (&output_obstack, "\n  ");
	  j = 2;
	}

      obstack_1grow (&output_obstack, '\"');
      for (p = tags[i]; p && *p; p++)
	{
	  if (*p == '"' || *p == '\\')
	    obstack_fgrow1 (&output_obstack, "\\%c", *p);
	  else if (*p == '\n')
	    obstack_sgrow (&output_obstack, "\\n");
	  else if (*p == '\t')
	    obstack_sgrow (&output_obstack, "\\t");
	  else if (*p == '\b')
	    obstack_sgrow (&output_obstack, "\\b");
	  else if (*p < 040 || *p >= 0177)
	    obstack_fgrow1 (&output_obstack, "\\%03o", *p);
	  else
	    obstack_1grow (&output_obstack, *p);
	}

      obstack_sgrow (&output_obstack, "\", ");
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
  output_table_data (&output_obstack, rlhs, 
		     0, 1, nrules + 1);
  muscle_insert ("r1", obstack_finish (&output_obstack));
  XFREE (rlhs + 1);

  /* Output YYR2. */
  short_tab = XMALLOC (short, nrules + 1);
  for (i = 1; i < nrules; i++)
    short_tab[i] = rrhs[i + 1] - rrhs[i] - 1;
  short_tab[nrules] = nitems - rrhs[nrules] - 1;
  output_table_data (&output_obstack, short_tab, 
		     0, 1, nrules + 1);
  muscle_insert ("r2", obstack_finish (&output_obstack));
  XFREE (short_tab);

  XFREE (rrhs + 1);
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
  int k;
  int m = 0;
  int n = 0;
  int count;
  int default_rule;
  int nreds;
  int max;
  int rule;
  int shift_state;
  int symbol;
  unsigned mask;
  unsigned *wordp;
  reductions *redp;
  shifts *shiftp;
  errs *errp;
  int nodefault = 0;		/* set nonzero to inhibit having any default reduction */

  for (i = 0; i < ntokens; i++)
    actrow[i] = 0;

  default_rule = 0;
  nreds = 0;
  redp = reduction_table[state];

  if (redp)
    {
      nreds = redp->nreds;

      if (nreds >= 1)
	{
	  /* loop over all the rules available here which require
	     lookahead */
	  m = lookaheads[state];
	  n = lookaheads[state + 1];

	  for (i = n - 1; i >= m; i--)
	    {
	      rule = -LAruleno[i];
	      wordp = LA + i * tokensetsize;
	      mask = 1;

	      /* and find each token which the rule finds acceptable
	         to come next */
	      for (j = 0; j < ntokens; j++)
		{
		  /* and record this rule as the rule to use if that
		     token follows.  */
		  if (mask & *wordp)
		    actrow[j] = rule;

		  mask <<= 1;
		  if (mask == 0)
		    {
		      mask = 1;
		      wordp++;
		    }
		}
	    }
	}
    }

  shiftp = shift_table[state];

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */

  if (shiftp)
    {
      k = shiftp->nshifts;

      for (i = 0; i < k; i++)
	{
	  shift_state = shiftp->shifts[i];
	  if (!shift_state)
	    continue;

	  symbol = accessing_symbol[shift_state];

	  if (ISVAR (symbol))
	    break;

	  actrow[symbol] = shift_state;

	  /* Do not use any default reduction if there is a shift for
	     error */
	  if (symbol == error_token_number)
	    nodefault = 1;
	}
    }

  errp = err_table[state];

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record MINSHORT as the action.  */

  if (errp)
    {
      k = errp->nerrs;

      for (i = 0; i < k; i++)
	{
	  symbol = errp->errs[i];
	  actrow[symbol] = MINSHORT;
	}
    }

  /* Now find the most common reduction and make it the default action
     for this state.  */

  if (nreds >= 1 && !nodefault)
    {
      if (consistent[state])
	default_rule = redp->rules[0];
      else
	{
	  max = 0;
	  for (i = m; i < n; i++)
	    {
	      count = 0;
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
    {
      if (actrow[i] != 0)
	count++;
    }

  if (count == 0)
    return;

  froms[state] = sp1 = sp = XCALLOC (short, count);
  tos[state] = sp2 = XCALLOC (short, count);

  for (i = 0; i < ntokens; i++)
    {
      if (actrow[i] != 0)
	{
	  *sp1++ = i;
	  *sp2++ = actrow[i];
	}
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


static void
free_shifts (void)
{
  shifts *sp, *sptmp;	/* JF derefrenced freed ptr */

  XFREE (shift_table);

  for (sp = first_shift; sp; sp = sptmp)
    {
      sptmp = sp->next;
      XFREE (sp);
    }
}


static void
free_reductions (void)
{
  reductions *rp, *rptmp;	/* JF fixed freed ptr */

  XFREE (reduction_table);

  for (rp = first_reduction; rp; rp = rptmp)
    {
      rptmp = rp->next;
      XFREE (rp);
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
    {
      if (to_state[i] != default_state)
	count++;
    }

  if (count == 0)
    return;

  symno = symbol - ntokens + nstates;

  froms[symno] = sp1 = sp = XCALLOC (short, count);
  tos[symno] = sp2 = XCALLOC (short, count);

  for (i = begin; i < end; i++)
    {
      if (to_state[i] != default_state)
	{
	  *sp1++ = from_state[i];
	  *sp2++ = to_state[i];
	}
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
    {
      if (state_count[i] > max)
	{
	  max = state_count[i];
	  default_state = i;
	}
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
    {
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
	{
	  if (tos[j][k] != tos[i][k] || froms[j][k] != froms[i][k])
	    match = 0;
	}

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
	{
	  if (pos[k] == j)
	    ok = 0;
	}

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

  berror ("pack_vector");
  return 0;			/* JF keep lint happy */
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
      if (froms[i])
	XFREE (froms[i]);
      if (tos[i])
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
  nvectors = nstates + nvars;

  froms = XCALLOC (short *, nvectors);
  tos = XCALLOC (short *, nvectors);
  tally = XCALLOC (short, nvectors);
  width = XCALLOC (short, nvectors);

  token_actions ();
  free_shifts ();
  free_reductions ();
  XFREE (lookaheads);
  XFREE (LA);
  XFREE (LAruleno);
  XFREE (accessing_symbol);

  goto_actions ();
  XFREE (goto_map + ntokens);
  XFREE (from_state);
  XFREE (to_state);

  sort_actions ();
  pack_table ();

  output_base ();
  output_table ();

  output_check ();
}

/*------------------------------------------.
| Copy the parser code into TABLE_OBSTACK.  |
`------------------------------------------*/

static void
output_parser (void)
{
  int c;
  FILE *fskel;
  size_t line;
  int actions_dumped = 0;

  /* Loop over lines in the standard parser file.  */
  if (!skeleton)
    {
      if (semantic_parser)
	skeleton = skeleton_find ("BISON_HAIRY", BISON_HAIRY);
      else
	skeleton = skeleton_find ("BISON_SIMPLE", BISON_SIMPLE);
    }
  fskel = xfopen (skeleton, "r");

  /* New output code.  */
  line = 1;
  c = getc (fskel);
  while (c != EOF)
    {
      if (c != '%')
	{
	  if (c == '\n')
	    ++line;
	  obstack_1grow (&table_obstack, c);
	  c = getc (fskel);
	}
      else if ((c = getc (fskel)) == '%')
	{
	  /* Read the muscle.  */
	  const char *muscle_key = 0;
	  const char *muscle_value = 0;
	  while (isalnum (c = getc (fskel)) || c == '_')
	    obstack_1grow (&muscle_obstack, c);
	  obstack_1grow (&muscle_obstack, 0);

	  /* Output the right value, or see if it's something special.  */
	  muscle_key = obstack_finish (&muscle_obstack);
	  muscle_value = muscle_find (muscle_key);
	  if (muscle_value)
	    obstack_sgrow (&table_obstack, muscle_value);
	  else if (!strcmp (muscle_key, "line"))
	    obstack_fgrow1 (&table_obstack, "%d", line + 1);
	  else if (!strcmp (muscle_key, "action"))
	    {
	      size_t size = obstack_object_size (&action_obstack);
	      obstack_grow (&table_obstack, 
			    obstack_finish (&action_obstack), size);
	    }
	  else
	    {
	      obstack_sgrow (&table_obstack, "%%");
	      obstack_sgrow (&table_obstack, muscle_key);
	    }
	}
      else
	obstack_1grow (&table_obstack, '%');
    }

  /* End.  */
  xfclose (fskel);
}

static void
free_itemsets (void)
{
  core *cp, *cptmp;

  XFREE (state_table);

  for (cp = first_state; cp; cp = cptmp)
    {
      cptmp = cp->next;
      XFREE (cp);
    }
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
  MUSCLE_INSERT_INT ("verbose", 0);

  MUSCLE_INSERT_INT ("nnts", nvars);
  MUSCLE_INSERT_INT ("nrules", nrules);
  MUSCLE_INSERT_INT ("nstates", nstates);
  MUSCLE_INSERT_INT ("ntokens", ntokens);

  MUSCLE_INSERT_INT ("locations_flag", locations_flag);

  if (spec_name_prefix)
    MUSCLE_INSERT_STRING ("prefix", spec_name_prefix);
}

/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

void
output (void)
{
  obstack_init (&output_obstack);

  free_itemsets ();

  output_token_translations ();
  output_gram ();

  XFREE (ritem);
  if (semantic_parser)
    output_stos ();
  output_rule_data ();
  output_actions ();
  
#if 0
  if (!no_parser_flag) */
#endif
  prepare ();
  /* Copy definitions in directive.  */
  muscle_insert ("prologue", obstack_finish (&attrs_obstack));
  output_parser ();

  obstack_free (&muscle_obstack, 0);
  obstack_free (&output_obstack, 0);
}
