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
#include "files.h"
#include "gram.h"
#include "LR0.h"
#include "complain.h"
#include "output.h"
#include "lalr.h"
#include "reader.h"
#include "conflicts.h"

extern void berror PARAMS((const char *));



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



static inline void
output_short_or_char_table (struct obstack *oout,
			    const char *comment,
			    const char *type,
			    const char *table_name,
			    short *short_table,
			    short first_value,
			    short begin, short end)
{
  int i, j;

  if (comment)
    obstack_fgrow1 (oout, "/* %s. */\n", comment);

  obstack_fgrow3 (oout, "static const %s %s[] =\n{\n  %6d",
		  type, table_name, first_value);

  j = 1;
  for (i = begin; i < end; i++)
    {
      obstack_1grow (oout, ',');

      if (j >= 10)
	{
	  obstack_sgrow (oout, "\n  ");
	  j = 1;
	}
      else
	{
	  j++;
	}

      obstack_fgrow1 (oout, "%6d", short_table[i]);
    }

  obstack_sgrow (oout, "\n};\n");
}


static inline void
output_short_table (struct obstack *oout,
		    const char *comment,
		    const char *table_name,
		    short *short_table,
		    short first_value,
		    short begin, short end)
{
  output_short_or_char_table (oout, comment, "short", table_name, short_table,
			      first_value, begin, end);
}


/*--------------------------------------------------------------.
| output_headers -- Output constant strings to the beginning of |
| certain files.                                                |
`--------------------------------------------------------------*/

/* Don't put the `%s' insides quotes, since it quotearg puts them. */

#define	GUARDSTR	\
"\n\
#include %s\n\
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
#include %s\n\
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
  char *attrsfile_quoted = 0;

  if (semantic_parser)
    {
      /* FIXME: This is *buggy*.  ATTRSFILE is not computed yet, since
	 we are waiting for the full input file to have been read to
	 be sure of the output file name.  So basically, here, a SEGV
	 is guaranteed.  OTOH, currently semantic parsers are not
	 supported.  */
      attrsfile_quoted = quotearg_style (c_quoting_style, attrsfile);
      obstack_fgrow1 (&guard_obstack, GUARDSTR, attrsfile_quoted);
    }

  if (no_parser_flag)
    return;

  if (semantic_parser)
    obstack_fgrow1 (&action_obstack, ACTSTR, attrsfile_quoted);
  else
    obstack_sgrow (&action_obstack, ACTSTR_SIMPLE);

  /* Rename certain symbols if -p was specified.  */
  if (spec_name_prefix)
    {
      obstack_fgrow1 (&table_obstack,
		      "#define yyparse %sparse\n", spec_name_prefix);
      obstack_fgrow1 (&table_obstack,
		      "#define yylex %slex\n", spec_name_prefix);
      obstack_fgrow1 (&table_obstack,
		      "#define yyerror %serror\n", spec_name_prefix);
      obstack_fgrow1 (&table_obstack,
		      "#define yylval %slval\n", spec_name_prefix);
      obstack_fgrow1 (&table_obstack,
		      "#define yychar %schar\n", spec_name_prefix);
      obstack_fgrow1 (&table_obstack,
		      "#define yydebug %sdebug\n", spec_name_prefix);
      obstack_fgrow1 (&table_obstack,
		      "#define yynerrs %snerrs\n", spec_name_prefix);
    }
}


/*-------------------------------------------------------.
| Output constant strings to the ends of certain files.  |
`-------------------------------------------------------*/

void
output_trailers (void)
{
  if (semantic_parser)
    obstack_sgrow (&guard_obstack, "\n    }\n}\n");

  obstack_1grow (&action_obstack, '\n');

  if (no_parser_flag)
    return;

  if (semantic_parser)
    obstack_sgrow (&action_obstack, "    }\n");

  obstack_sgrow (&action_obstack, "}\n");
}



static void
output_token_translations (void)
{
  obstack_sgrow (&table_obstack, "\
\n\
/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */\n");

  obstack_fgrow2 (&table_obstack,
      "#define YYTRANSLATE(x) ((unsigned)(x) <= %d ? yytranslate[x] : %d)\
\n\
\n",
		  max_user_token_number, nsyms);

  output_short_or_char_table (&table_obstack,
	     "YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX",
			      ntokens < 127 ? "char" : "short",
			      "yytranslate", token_translations,
			      0, 1, max_user_token_number + 1);

  XFREE (token_translations);
}


static void
output_gram (void)
{
  /* With the ordinary parser,
     yyprhs and yyrhs are needed only for yydebug. */
  /* With the no_parser option, all tables are generated */
  if (!semantic_parser && !no_parser_flag)
    obstack_sgrow (&table_obstack, "\n#if YYDEBUG != 0\n");

  output_short_table (&table_obstack, NULL, "yyprhs", rrhs,
		      0, 1, nrules + 1);

  {
    size_t yyrhs_size = 1;
    short *yyrhs, *sp;
    int i;

    for (sp = ritem + 1; *sp; sp++)
      ++yyrhs_size;
    yyrhs = XMALLOC (short, yyrhs_size);

    for (sp = ritem + 1, i = 1; *sp; ++sp, ++i)
      yyrhs[i] = *sp > 0 ? *sp : 0;

    output_short_table (&table_obstack, NULL, "yyrhs", yyrhs,
			ritem[0], 1, yyrhs_size);
    XFREE (yyrhs);
  }

  if (!semantic_parser && !no_parser_flag)
    obstack_sgrow (&table_obstack, "\n#endif\n");
}


static void
output_stos (void)
{
  output_short_table (&table_obstack, NULL, "yystos", accessing_symbol,
		      0, 1, nstates);
}


static void
output_rule_data (void)
{
  int i;
  int j;
  short *short_tab = NULL;

  obstack_sgrow (&table_obstack, "\n\
#if YYDEBUG != 0\n");

  output_short_table (&table_obstack,
           "YYRLINE[YYN] -- source line where rule number YYN was defined",
		      "yyrline", rline,
		      0, 1, nrules + 1);

  obstack_sgrow (&table_obstack, "#endif\n\n");

  if (token_table_flag || no_parser_flag)
    {
      obstack_fgrow1 (&table_obstack, "#define YYNTOKENS %d\n", ntokens);
      obstack_fgrow1 (&table_obstack, "#define YYNNTS %d\n", nvars);
      obstack_fgrow1 (&table_obstack, "#define YYNRULES %d\n", nrules);
      obstack_fgrow1 (&table_obstack, "#define YYNSTATES %d\n", nstates);
      obstack_fgrow1 (&table_obstack, "#define YYMAXUTOK %d\n\n",
		      max_user_token_number);
    }

  /* Output the table of symbol names.  */
  if (!token_table_flag && !no_parser_flag)
    obstack_sgrow (&table_obstack,
			 "\n#if YYDEBUG != 0 || defined YYERROR_VERBOSE\n\n");
  obstack_sgrow (&table_obstack, "\
/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */\n");
  obstack_sgrow (&table_obstack,
	   "static const char *const yytname[] =\n{\n  ");

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
	  obstack_sgrow (&table_obstack, "\n  ");
	  j = 2;
	}

      obstack_1grow (&table_obstack, '\"');
      for (p = tags[i]; p && *p; p++)
	{
	  if (*p == '"' || *p == '\\')
	    obstack_fgrow1 (&table_obstack, "\\%c", *p);
	  else if (*p == '\n')
	    obstack_sgrow (&table_obstack, "\\n");
	  else if (*p == '\t')
	    obstack_sgrow (&table_obstack, "\\t");
	  else if (*p == '\b')
	    obstack_sgrow (&table_obstack, "\\b");
	  else if (*p < 040 || *p >= 0177)
	    obstack_fgrow1 (&table_obstack, "\\%03o", *p);
	  else
	    obstack_1grow (&table_obstack, *p);
	}

      obstack_sgrow (&table_obstack, "\", ");
      j += strsize;
    }
  /* add a NULL entry to list of tokens */
  obstack_sgrow (&table_obstack, "NULL\n};\n");

  if (!token_table_flag && !no_parser_flag)
    obstack_sgrow (&table_obstack, "#endif\n\n");

  /* Output YYTOKNUM. */
  if (token_table_flag)
    {
      output_short_table (&table_obstack,
		  "YYTOKNUM[YYLEX] -- Index in YYTNAME corresponding to YYLEX",
			  "yytoknum", user_toknums,
			  0, 1, ntokens + 1);
    }

  /* Output YYR1. */
  output_short_table (&table_obstack,
	      "YYR1[YYN] -- Symbol number of symbol that rule YYN derives",
		      "yyr1", rlhs,
		      0, 1, nrules + 1);
  XFREE (rlhs + 1);

  obstack_1grow (&table_obstack, '\n');

  /* Output YYR2. */
  short_tab = XMALLOC (short, nrules + 1);
  for (i = 1; i < nrules; i++)
    short_tab[i] = rrhs[i + 1] - rrhs[i] - 1;
  short_tab[nrules] = nitems - rrhs[nrules] - 1;
  output_short_table (&table_obstack,
        "YYR2[YYN] -- Number of symbols composing right hand side of rule YYN",
		      "yyr2", short_tab,
		      0, 1, nrules + 1);
  obstack_1grow (&table_obstack, '\n');

  XFREE (short_tab);

  XFREE (rrhs + 1);
}


static void
output_defines (void)
{
  obstack_fgrow1 (&table_obstack, "\n\n#define\tYYFINAL\t\t%d\n", final_state);
  obstack_fgrow1 (&table_obstack, "#define\tYYFLAG\t\t%d\n", MINSHORT);
  obstack_fgrow1 (&table_obstack, "#define\tYYNTBASE\t%d\n", ntokens);
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
  XFREE (actrow);

  output_short_table (&table_obstack,
  "YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE\n\
   doesn't specify something else to do.  Zero means the default is an\n\
   error",
		      "yydefact", yydefact,
		      yydefact[0], 1, nstates);
  obstack_1grow (&table_obstack, '\n');
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

  output_short_table (&table_obstack, NULL, "yydefgoto", yydefgoto,
		      yydefgoto[0], 1, nsyms - ntokens);

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
  output_short_table (&table_obstack, NULL, "yypact", base,
		      base[0], 1, nstates);

  obstack_1grow (&table_obstack, '\n');

  output_short_table (&table_obstack, NULL, "yypgoto", base,
		      base[nstates], nstates + 1, nvectors);

  XFREE (base);
}


static void
output_table (void)
{
  obstack_fgrow1 (&table_obstack, "\n\n#define\tYYLAST\t\t%d\n\n\n", high);
  output_short_table (&table_obstack, NULL, "yytable", table,
		      table[0], 1, high + 1);
  XFREE (table);
}


static void
output_check (void)
{
  output_short_table (&table_obstack, NULL, "yycheck", check,
		      check[0], 1, high + 1);
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
  obstack_1grow (&table_obstack, '\n');
  output_base ();
  output_table ();
  obstack_1grow (&table_obstack, '\n');
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

  if (pure_parser)
    obstack_sgrow (&table_obstack, "#define YYPURE 1\n\n");

  /* Loop over lines in the standard parser file.  */
  if (!skeleton)
    {
      if (semantic_parser)
	skeleton = skeleton_find ("BISON_HAIRY", BISON_HAIRY);
      else
	skeleton = skeleton_find ("BISON_SIMPLE", BISON_SIMPLE);
    }
  assert (skeleton);
  fskel = xfopen (skeleton, "r");

  /* Set LINE to 2, not 1: `#line LINENUM' -- Here LINENUM is a
     decimal integer constant.  This specifies that the line number of
     the *following* line of input, in its original source file, was
     LINENUM.  */
  line = 2;

  while (1)
    {
      enum line_type_e
	{
	  regular_line,
	  sync_line,	/* #line. */
	  actions_line	/* %% actions. */
	};
      enum line_type_e line_type = regular_line;

      c = getc (fskel);

      /* Is this line special? */
      if (c == '#')
	{
	  /* See if it's a `#line' line. */
	  if ((c = getc (fskel)) == 'l')
	    if ((c = getc (fskel)) == 'i')
	      if ((c = getc (fskel)) == 'n')
		if ((c = getc (fskel)) == 'e')
		  line_type = sync_line;
		else
		  obstack_sgrow (&table_obstack, "#lin");
	      else
		obstack_sgrow (&table_obstack, "#li");
	    else
	      obstack_sgrow (&table_obstack, "#l");
	  else
	    obstack_sgrow (&table_obstack, "#");
	}
      else if (c == '%')
	{
	  /* See if it's a `%% actions' line. */
	  if ((c = getc (fskel)) == '%')
	    if ((c = getc (fskel)) == ' ')
	      if ((c = getc (fskel)) == 'a')
		if ((c = getc (fskel)) == 'c')
		  if ((c = getc (fskel)) == 't')
		    if ((c = getc (fskel)) == 'i')
		      if ((c = getc (fskel)) == 'o')
			if ((c = getc (fskel)) == 'n')
			  if ((c = getc (fskel)) == 's')
			    line_type = actions_line;
			  else
			    obstack_sgrow (&table_obstack, "%% action");
			else
			  obstack_sgrow (&table_obstack, "%% actio");
		      else
			obstack_sgrow (&table_obstack, "%% acti");
		    else
		      obstack_sgrow (&table_obstack, "%% act");
		  else
		    obstack_sgrow (&table_obstack, "%% ac");
		else
		  obstack_sgrow (&table_obstack, "%% a");
	      else
		obstack_sgrow (&table_obstack, "%% ");
	    else
	      obstack_sgrow (&table_obstack, "%%");
	  else
	    obstack_sgrow (&table_obstack, "%");
	}

      switch (line_type)
	{
	case sync_line:
	  if (!no_lines_flag)
	    obstack_fgrow2 (&table_obstack, "#line %d %s\n",
			    line, quotearg_style (c_quoting_style, skeleton));

	  /* Skip the end of line. */
	  for (; c != '\n' && c != EOF; c = getc (fskel))
	    /* nothing */;
	  break;

	case actions_line:
	  {
	    size_t size = obstack_object_size (&action_obstack);

	    actions_dumped++;
	    assert (actions_dumped == 1);
	    obstack_grow (&table_obstack,
			  obstack_finish (&action_obstack),
			  size);
	  }

	  /* Skip the end of line. */
	  for (; c != '\n' && c != EOF; c = getc (fskel))
	    /* nothing */;
	  break;

	case regular_line:
	  for (; c != '\n' && c != EOF; c = getc (fskel))
	    obstack_1grow (&table_obstack, c);
	}

      if (c == EOF)
	break;
      obstack_1grow (&table_obstack, c);
      line++;
    }
  assert (actions_dumped == 1);
  xfclose (fskel);
}

static void
output_program (void)
{
  int c;

  if (!no_lines_flag)
    obstack_fgrow2 (&table_obstack, "#line %d %s\n",
		    lineno, quotearg_style (c_quoting_style, infile));

  while ((c = getc (finput)) != EOF)
    obstack_1grow (&table_obstack, c);
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


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

void
output (void)
{
  /* output_token_defines(ftable);      / * JF put out token defines FIRST */

  /* If using a simple parser the definition of YYSTYPE are put into
     TABLE_OBSTACK.  */
  if (!semantic_parser)
    {
      size_t size = obstack_object_size (&attrs_obstack);
      obstack_grow (&table_obstack, obstack_finish (&attrs_obstack), size);
    }
  reader_output_yylsp (&table_obstack);
  if (debug_flag)
    obstack_sgrow (&table_obstack, "\
#ifndef YYDEBUG\n\
# define YYDEBUG 1\n\
#endif\n\
\n");

  if (semantic_parser)
    obstack_fgrow1 (&table_obstack, "#include %s\n",
		    quotearg_style (c_quoting_style, attrsfile));

  if (!no_parser_flag)
    obstack_sgrow (&table_obstack, "#include <stdio.h>\n\n");

  free_itemsets ();
  output_defines ();
  output_token_translations ();
/*   if (semantic_parser) */
  /* This is now unconditional because debugging printouts can use it.  */
  output_gram ();
  XFREE (ritem);
  if (semantic_parser)
    output_stos ();
  output_rule_data ();
  output_actions ();
  if (!no_parser_flag)
    output_parser ();
  output_program ();
}
