/* Output the generated parsing program for bison,
   Copyright (C) 1984, 1986, 1989, 1992, 2000 Free Software Foundation, Inc.

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
#include "getargs.h"
#include "alloc.h"
#include "files.h"
#include "gram.h"
#include "state.h"
#include "complain.h"
#include "output.h"
#include "lalr.h"
#include "reader.h"

extern void berror PARAMS((const char *));

extern char **tags;
extern short *user_toknums;
extern int final_state;
extern errs **err_table;


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



static inline void
output_short_table (FILE *out,
		    const char *table_name,
		    short *short_table,
		    short first_value,
		    short begin, short end)
{
  int i, j;

  fprintf (out, "static const short %s[] = {%6d", table_name, first_value);

  j = 10;
  for (i = begin; i < end; i++)
    {
      putc (',', out);

      if (j >= 10)
	{
	  putc ('\n', out);
	  j = 1;
	}
      else
	{
	  j++;
	}

      fprintf (out, "%6d", short_table[i]);
    }

  fprintf (out, "\n};\n");
}


/*--------------------------------------------------------------.
| output_headers -- Output constant strings to the beginning of |
| certain files.                                                |
`--------------------------------------------------------------*/

#define	GUARDSTR	\
"\n\
#include \"%s\"\n\
extern int yyerror;\n\
extern int yycost;\n\
extern char * yymsg;\n\
extern YYSTYPE yyval;\n\
\n\
yyguard(n, yyvsp, yylsp)\n\
register int n;\n\
register YYSTYPE *yyvsp;\n\
register YYLTYPE *yylsp;\n\
{\n\
  yyerror = 0;\n\
  yycost = 0;\n\
  yymsg = 0;\n\
  switch (n)\n\
    {"

#define	ACTSTR		\
"\n\
#include \"%s\"\n\
extern YYSTYPE yyval;\n\
extern int yychar;\n\
\n\
yyaction(n, yyvsp, yylsp)\n\
register int n;\n\
register YYSTYPE *yyvsp;\n\
register YYLTYPE *yylsp;\n\
{\n\
  switch (n)\n\
    {"

#define	ACTSTR_SIMPLE	"\n  switch (yyn) {\n"

void
output_headers (void)
{
  if (semantic_parser)
    fprintf (fguard, GUARDSTR, attrsfile);

  if (noparserflag)
    return;

  fprintf (faction, (semantic_parser ? ACTSTR : ACTSTR_SIMPLE), attrsfile);
/*  if (semantic_parser)	JF moved this below
    fprintf(ftable, "#include \"%s\"\n", attrsfile);
  fprintf(ftable, "#include <stdio.h>\n\n");
*/

  /* Rename certain symbols if -p was specified.  */
  if (spec_name_prefix)
    {
      fprintf (ftable, "#define yyparse %sparse\n", spec_name_prefix);
      fprintf (ftable, "#define yylex %slex\n", spec_name_prefix);
      fprintf (ftable, "#define yyerror %serror\n", spec_name_prefix);
      fprintf (ftable, "#define yylval %slval\n", spec_name_prefix);
      fprintf (ftable, "#define yychar %schar\n", spec_name_prefix);
      fprintf (ftable, "#define yydebug %sdebug\n", spec_name_prefix);
      fprintf (ftable, "#define yynerrs %snerrs\n", spec_name_prefix);
    }
}


/*-------------------------------------------------------.
| Output constant strings to the ends of certain files.  |
`-------------------------------------------------------*/

void
output_trailers (void)
{
  if (semantic_parser)
    fprintf (fguard, "\n    }\n}\n");

  fprintf (faction, "\n");

  if (noparserflag)
    return;

  if (semantic_parser)
    fprintf (faction, "    }\n");
  fprintf (faction, "}\n");
}



static void
output_token_translations (void)
{
  int i, j;
/*   short *sp; JF unused */

  if (translations)
    {
      fprintf (ftable,
	       "\n#define YYTRANSLATE(x) ((unsigned)(x) <= %d ? yytranslate[x] : %d)\n",
	       max_user_token_number, nsyms);

      if (ntokens < 127)	/* play it very safe; check maximum element value.  */
	fprintf (ftable, "\nstatic const char yytranslate[] = {     0");
      else
	fprintf (ftable, "\nstatic const short yytranslate[] = {     0");

      j = 10;
      for (i = 1; i <= max_user_token_number; i++)
	{
	  putc (',', ftable);

	  if (j >= 10)
	    {
	      putc ('\n', ftable);
	      j = 1;
	    }
	  else
	    {
	      j++;
	    }

	  fprintf (ftable, "%6d", token_translations[i]);
	}

      fprintf (ftable, "\n};\n");
    }
  else
    {
      fprintf (ftable, "\n#define YYTRANSLATE(x) (x)\n");
    }
}


static void
output_gram (void)
{
  int j;
  short *sp;

  /* With the ordinary parser,
     yyprhs and yyrhs are needed only for yydebug. */
  /* With the noparser option, all tables are generated */
  if (!semantic_parser && !noparserflag)
    fprintf (ftable, "\n#if YYDEBUG != 0\n");

  output_short_table (ftable, "yyprhs", rrhs,
		      0, 1, nrules + 1);

  fprintf (ftable, "\nstatic const short yyrhs[] = {%6d", ritem[0]);

  j = 10;
  for (sp = ritem + 1; *sp; sp++)
    {
      putc (',', ftable);

      if (j >= 10)
	{
	  putc ('\n', ftable);
	  j = 1;
	}
      else
	{
	  j++;
	}

      if (*sp > 0)
	fprintf (ftable, "%6d", *sp);
      else
	fprintf (ftable, "     0");
    }

  fprintf (ftable, "\n};\n");

  if (!semantic_parser && !noparserflag)
    fprintf (ftable, "\n#endif\n");
}


static void
output_stos (void)
{
  output_short_table (ftable, "yystos", accessing_symbol,
		      0, 1, nstates);
}


static void
output_rule_data (void)
{
  int i;
  int j;

  fputs ("\n\
#if YYDEBUG != 0\n\
/* YYRLINE[yyn]: source line where rule number YYN was defined. */\n",
	 ftable);

  output_short_table (ftable, "yyrline", rline,
		      0, 1, nrules + 1);

  fputs ("#endif\n\n", ftable);

  if (toknumflag || noparserflag)
    {
      fprintf (ftable, "#define YYNTOKENS %d\n", ntokens);
      fprintf (ftable, "#define YYNNTS %d\n", nvars);
      fprintf (ftable, "#define YYNRULES %d\n", nrules);
      fprintf (ftable, "#define YYNSTATES %d\n", nstates);
      fprintf (ftable, "#define YYMAXUTOK %d\n\n", max_user_token_number);
    }

  if (!toknumflag && !noparserflag)
    fprintf (ftable, "\n#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)\n\n");

  /* Output the table of symbol names.  */

  fprintf (ftable,
	   "static const char * const yytname[] = {   \"%s\"", tags[0]);

  j = strlen (tags[0]) + 44;
  for (i = 1; i < nsyms; i++)
    /* this used to be i<=nsyms, but that output a final "" symbol
       almost by accident */
    {
      char *p;
      putc (',', ftable);
      j++;

      if (j > 75)
	{
	  putc ('\n', ftable);
	  j = 0;
	}

      putc ('\"', ftable);
      j++;

      for (p = tags[i]; p && *p; p++)
	{
	  if (*p == '"' || *p == '\\')
	    {
	      fprintf (ftable, "\\%c", *p);
	      j += 2;
	    }
	  else if (*p == '\n')
	    {
	      fprintf (ftable, "\\n");
	      j += 2;
	    }
	  else if (*p == '\t')
	    {
	      fprintf (ftable, "\\t");
	      j += 2;
	    }
	  else if (*p == '\b')
	    {
	      fprintf (ftable, "\\b");
	      j += 2;
	    }
	  else if (*p < 040 || *p >= 0177)
	    {
	      fprintf (ftable, "\\%03o", *p);
	      j += 4;
	    }
	  else
	    {
	      putc (*p, ftable);
	      j++;
	    }
	}

      putc ('\"', ftable);
      j++;
    }
  /* add a NULL entry to list of tokens */
  fprintf (ftable, ", NULL\n};\n");

  if (!toknumflag && !noparserflag)
    fprintf (ftable, "#endif\n\n");

  /* Output YYTOKNUM. */
  if (toknumflag)
    {
      output_short_table (ftable, "yytoknum", user_toknums,
			  0, 1, ntokens + 1);
    }

  /* Output YYR1. */
  fputs ("\
/* YYR1[YYN]: Symbol number of symbol that rule YYN derives. */\n", ftable);

  output_short_table (ftable, "yyr1", rlhs,
		      0, 1, nrules + 1);
  FREE (rlhs + 1);

  putc ('\n', ftable);

  /* Output YYR2. */
  fputs ("\
/* YYR2[YYN]: Number of symbols composing right hand side of rule YYN. */\n\
static const short yyr2[] = {     0", ftable);
  j = 10;
  for (i = 1; i < nrules; i++)
    {
      putc (',', ftable);

      if (j >= 10)
	{
	  putc ('\n', ftable);
	  j = 1;
	}
      else
	{
	  j++;
	}

      fprintf (ftable, "%6d", rrhs[i + 1] - rrhs[i] - 1);
    }

  putc (',', ftable);
  if (j >= 10)
    putc ('\n', ftable);

  fprintf (ftable, "%6d\n};\n", nitems - rrhs[nrules] - 1);
  FREE (rrhs + 1);
}


static void
output_defines (void)
{
  fprintf (ftable, "\n\n#define\tYYFINAL\t\t%d\n", final_state);
  fprintf (ftable, "#define\tYYFLAG\t\t%d\n", MINSHORT);
  fprintf (ftable, "#define\tYYNTBASE\t%d\n", ntokens);
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

  froms[state] = sp1 = sp = NEW2 (count, short);
  tos[state] = sp2 = NEW2 (count, short);

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
  short *yydefact = NEW2 (nstates, short);

  actrow = NEW2 (ntokens, short);
  for (i = 0; i < nstates; ++i)
    {
      yydefact[i] = action_row (i);
      save_row (i);
    }
  FREE (actrow);

  output_short_table (ftable, "yydefact", yydefact,
		      yydefact[0], 1, nstates);
  FREE (yydefact);
}


static void
free_shifts (void)
{
  shifts *sp, *sptmp;	/* JF derefrenced freed ptr */

  FREE (shift_table);

  for (sp = first_shift; sp; sp = sptmp)
    {
      sptmp = sp->next;
      FREE (sp);
    }
}


static void
free_reductions (void)
{
  reductions *rp, *rptmp;	/* JF fixed freed ptr */

  FREE (reduction_table);

  for (rp = first_reduction; rp; rp = rptmp)
    {
      rptmp = rp->next;
      FREE (rp);
    }
}



static void
save_column (int symbol, int default_state)
{
  int i;
  int m;
  int n;
  short *sp;
  short *sp1;
  short *sp2;
  int count;
  int symno;

  m = goto_map[symbol];
  n = goto_map[symbol + 1];

  count = 0;
  for (i = m; i < n; i++)
    {
      if (to_state[i] != default_state)
	count++;
    }

  if (count == 0)
    return;

  symno = symbol - ntokens + nstates;

  froms[symno] = sp1 = sp = NEW2 (count, short);
  tos[symno] = sp2 = NEW2 (count, short);

  for (i = m; i < n; i++)
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
  int i, j, k;

  state_count = NEW2 (nstates, short);

  k = default_goto (ntokens);
  fprintf (ftable, "\nstatic const short yydefgoto[] = {%6d", k);
  save_column (ntokens, k);

  j = 10;
  for (i = ntokens + 1; i < nsyms; i++)
    {
      putc (',', ftable);

      if (j >= 10)
	{
	  putc ('\n', ftable);
	  j = 1;
	}
      else
	{
	  j++;
	}

      k = default_goto (i);
      fprintf (ftable, "%6d", k);
      save_column (i, k);
    }

  fprintf (ftable, "\n};\n");
  FREE (state_count);
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

  order = NEW2 (nvectors, short);
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

  if (t == 0)
    berror ("pack_vector");

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

  base = NEW2 (nvectors, short);
  pos = NEW2 (nentries, short);
  table = NEW2 (MAXTABLE, short);
  check = NEW2 (MAXTABLE, short);

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
	FREE (froms[i]);
      if (tos[i])
	FREE (tos[i]);
    }

  FREE (froms);
  FREE (tos);
  FREE (pos);
}

/* the following functions output yytable, yycheck
   and the vectors whose elements index the portion starts */

static void
output_base (void)
{
  output_short_table (ftable, "yypact", base,
		      base[0], 1, nstates);

  putc ('\n', ftable);

  output_short_table (ftable, "yypgoto", base,
		      base[nstates], nstates + 1, nvectors);

  FREE (base);
}


static void
output_table (void)
{
  fprintf (ftable, "\n\n#define\tYYLAST\t\t%d\n\n\n", high);
  output_short_table (ftable, "yytable", table,
		      table[0], 1, high + 1);
  FREE (table);
}


static void
output_check (void)
{
  output_short_table (ftable, "yycheck", check,
		      check[0], 1, high + 1);
  FREE (check);
}

/* compute and output yydefact, yydefgoto, yypact, yypgoto, yytable
   and yycheck.  */

static void
output_actions (void)
{
  nvectors = nstates + nvars;

  froms = NEW2 (nvectors, short *);
  tos = NEW2 (nvectors, short *);
  tally = NEW2 (nvectors, short);
  width = NEW2 (nvectors, short);

  token_actions ();
  free_shifts ();
  free_reductions ();
  FREE (lookaheads);
  FREE (LA);
  FREE (LAruleno);
  FREE (accessing_symbol);

  goto_actions ();
  FREE (goto_map + ntokens);
  FREE (from_state);
  FREE (to_state);

  sort_actions ();
  pack_table ();
  putc ('\n', ftable);
  output_base ();
  output_table ();
  putc ('\n', ftable);
  output_check ();
}

/* copy the parser code into the ftable file at the end.  */

static void
output_parser (void)
{
  int c;
#ifdef DONTDEF
  FILE *fpars;
#else
#define fpars fparser
#endif

  if (pure_parser)
    fprintf (ftable, "#define YYPURE 1\n\n");

#ifdef DONTDEF			/* JF no longer needed 'cuz open_extra_files changes the
				   currently open parser from bison.simple to bison.hairy */
  if (semantic_parser)
    fpars = fparser;
  else
    fpars = fparser1;
#endif

  /* Loop over lines in the standard parser file.  */

  while (1)
    {
      int write_line = 1;

      c = getc (fpars);

      /* See if the line starts with `#line.
         If so, set write_line to 0.  */
      if (nolinesflag)
	if (c == '#')
	  {
	    c = getc (fpars);
	    if (c == 'l')
	      {
		c = getc (fpars);
		if (c == 'i')
		  {
		    c = getc (fpars);
		    if (c == 'n')
		      {
			c = getc (fpars);
			if (c == 'e')
			  write_line = 0;
			else
			  fprintf (ftable, "#lin");
		      }
		    else
		      fprintf (ftable, "#li");
		  }
		else
		  fprintf (ftable, "#l");
	      }
	    else
	      fprintf (ftable, "#");
	  }

      /* now write out the line... */
      for (; c != '\n' && c != EOF; c = getc (fpars))
	if (write_line)
	  {
	    if (c == '$')
	      {
		/* `$' in the parser file indicates where to put the actions.
		   Copy them in at this point.  */
		rewind (faction);
		for (c = getc (faction); c != EOF; c = getc (faction))
		  putc (c, ftable);
	      }
	    else
	      putc (c, ftable);
	  }
      if (c == EOF)
	break;
      putc (c, ftable);
    }
}

static void
output_program (void)
{
  int c;

  if (!nolinesflag)
    fprintf (ftable, "#line %d \"%s\"\n", lineno, infile);

  c = getc (finput);
  while (c != EOF)
    {
      putc (c, ftable);
      c = getc (finput);
    }
}


static void
free_itemsets (void)
{
  core *cp, *cptmp;

  FREE (state_table);

  for (cp = first_state; cp; cp = cptmp)
    {
      cptmp = cp->next;
      FREE (cp);
    }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

void
output (void)
{
  int c;

  /* output_token_defines(ftable);      / * JF put out token defines FIRST */
  if (!semantic_parser)		/* JF Put out other stuff */
    {
      rewind (fattrs);
      while ((c = getc (fattrs)) != EOF)
	putc (c, ftable);
    }
  reader_output_yylsp (ftable);
  if (debugflag)
    fputs ("\
#ifndef YYDEBUG\n\
#define YYDEBUG 1\n\
#endif\n\
\n",
	   ftable);

  if (semantic_parser)
    fprintf (ftable, "#include \"%s\"\n", attrsfile);

  if (!noparserflag)
    fprintf (ftable, "#include <stdio.h>\n\n");

  /* Make "const" do nothing if not in ANSI C.  */
  fputs ("\
#ifndef __cplusplus\n\
# ifndef __STDC__\n\
#  define const\n\
# endif\n\
#endif\n\
\n",
	 ftable);

  free_itemsets ();
  output_defines ();
  output_token_translations ();
/*   if (semantic_parser) */
  /* This is now unconditional because debugging printouts can use it.  */
  output_gram ();
  FREE (ritem);
  if (semantic_parser)
    output_stos ();
  output_rule_data ();
  output_actions ();
  if (!noparserflag)
    output_parser ();
  output_program ();
}
