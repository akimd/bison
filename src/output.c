/* Output the generated parsing program for bison,
   Copyright (C) 1984, 1986, 1989, 1992, 2000, 2001, 2002
   Free Software Foundation, Inc.

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


/* The parser tables consist of these tables.  Marked ones needed only
   for the semantic parser.  Double marked are output only if switches
   are set.

   YYTRANSLATE = vector mapping yylex's token numbers into bison's
   token numbers.

   ++ YYTNAME = vector of string-names indexed by bison token number.

   ++ YYTOKNUM = vector of yylex token numbers corresponding to
   entries in YYTNAME.

   YYRLINE = vector of line-numbers of all rules.  For yydebug
   printouts.

   YYRHS = vector of items of all rules.  This is exactly what RITEMS
   contains.  For yydebug and for semantic parser.

   YYPRHS[R] = index in YYRHS of first item for rule R.

   YYR1[R] = symbol number of symbol that rule R derives.

   YYR2[R] = number of symbols composing right hand side of rule R.

   + YYSTOS[S] = the symbol number of the symbol that leads to state
   S.

   YYDEFACT[S] = default rule to reduce with in state s, when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.

   YYDEFGOTO[I] = default state to go to after a reduction of a rule
   that generates variable NTOKENS + I, except when YYTABLE specifies
   something else to do.

   YYPACT[S] = index in YYTABLE of the portion describing state S.
   The lookahead token's type is used to index that portion to find
   out what to do.

   If the value in YYTABLE is positive, we shift the token and go to
   that state.

   If the value is negative, it is minus a rule number to reduce by.

   If the value is zero, the default action from YYDEFACT[S] is used.

   YYPGOTO[I] = the index in YYTABLE of the portion describing what to
   do after reducing a rule that derives variable I + NTOKENS.  This
   portion is indexed by the parser state number, S, as of before the
   text for this nonterminal was read.  The value from YYTABLE is the
   state to go to if the corresponding value in YYCHECK is S.

   YYTABLE = a vector filled with portions for different uses, found
   via YYPACT and YYPGOTO.

   YYCHECK = a vector indexed in parallel with YYTABLE.  It indicates,
   in a roundabout way, the bounds of the portion you are trying to
   examine.

   Suppose that the portion of yytable starts at index P and the index
   to be examined within the portion is I.  Then if YYCHECK[P+I] != I,
   I is outside the bounds of what is actually allocated, and the
   default (from YYDEFACT or YYDEFGOTO) should be used.  Otherwise,
   YYTABLE[P+I] should be used.

   YYFINAL = the state number of the termination state.  YYFLAG = most
   negative short int.  Used to flag ??  */

#include "system.h"
#include "bitsetv.h"
#include "quotearg.h"
#include "error.h"
#include "getargs.h"
#include "files.h"
#include "gram.h"
#include "LR0.h"
#include "complain.h"
#include "output.h"
#include "lalr.h"
#include "reader.h"
#include "symtab.h"
#include "conflicts.h"
#include "muscle_tab.h"

/* From lib/readpipe.h.  */
FILE *readpipe PARAMS ((const char *, ...));

/* From src/scan-skel.l. */
int skel_lex PARAMS ((void));
extern FILE *skel_in;

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

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.
   We start with the original hard-coded value: SHRT_MAX
   (yes, not USHRT_MAX). */
static size_t table_size = SHRT_MAX;
static short *table = NULL;
static short *check = NULL;
static int lowzero;
static int high;

struct obstack muscle_obstack;
static struct obstack format_obstack;

int error_verbose = 0;


/*----------------------------------------------------------------.
| If TABLE (and CHECK) appear to be small to be addressed at      |
| DESIRED, grow them.  Note that TABLE[DESIRED] is to be used, so |
| the desired size is at least DESIRED + 1.                       |
`----------------------------------------------------------------*/

static void
table_grow (size_t desired)
{
  size_t old_size = table_size;

  while (table_size <= desired)
    table_size *= 2;

  if (trace_flag)
    fprintf (stderr, "growing table and check from: %d to %d\n",
	     old_size, table_size);

  table = XREALLOC (table, short, table_size);
  check = XREALLOC (check, short, table_size);

  for (/* Nothing. */; old_size < table_size; ++old_size)
    {
      table[old_size] = 0;
      check[old_size] = -1;
    }
}


/*------------------------------------------------------------------.
| Create a function NAME which Format the FIRST and then            |
| TABLE_DATA[BEGIN..END[ (of TYPE) into OOUT, and return the number |
| of bits needed for its longuest value.                            |
`------------------------------------------------------------------*/


#define GENERATE_OUTPUT_TABLE(Name, Type)				\
									\
static inline long int							\
Name (struct obstack *oout,						\
      Type *table_data,							\
      Type first,							\
      int begin,							\
      int end)								\
{									\
  long int max = first;							\
  int i;								\
  int j = 1;								\
									\
  obstack_fgrow1 (oout, "%6d", first);					\
  for (i = begin; i < end; ++i)						\
    {									\
      obstack_1grow (oout, ',');					\
      if (j >= 10)							\
	{								\
	  obstack_sgrow (oout, "\n  ");					\
	  j = 1;							\
	}								\
      else								\
	++j;								\
      obstack_fgrow1 (oout, "%6d", table_data[i]);			\
      if (table_data[i] > max)						\
	max = table_data[i];						\
    }									\
  obstack_1grow (oout, 0);						\
									\
  return max;								\
}

GENERATE_OUTPUT_TABLE(output_int_table, int)
GENERATE_OUTPUT_TABLE(output_unsigned_int_table, unsigned int)
GENERATE_OUTPUT_TABLE(output_short_table, short)
GENERATE_OUTPUT_TABLE(output_token_number_table, token_number_t)
GENERATE_OUTPUT_TABLE(output_item_number_table, item_number_t)


/*-----------------------------------------------------------------.
| Prepare the muscles related to the tokens: translate, tname, and |
| toknum.                                                          |
`-----------------------------------------------------------------*/

static void
prepare_tokens (void)
{
  long int max = output_token_number_table (&format_obstack,
					    token_translations,
					    0, 1, max_user_token_number + 1);
  muscle_insert ("translate", obstack_finish (&format_obstack));
  MUSCLE_INSERT_LONG_INT ("token_number_max", max);
  XFREE (token_translations);

  {
    int i;
    int j = 0;
    for (i = 0; i < nsyms; i++)
      {
	/* Be sure not to use twice the same quotearg slot. */
	const char *cp =
	  quotearg_n_style (1, c_quoting_style,
			    quotearg_style (escape_quoting_style,
					    symbols[i]->tag));
	/* Width of the next token, including the two quotes, the coma
	   and the space.  */
	int strsize = strlen (cp) + 2;

	if (j + strsize > 75)
	  {
	    obstack_sgrow (&format_obstack, "\n  ");
	    j = 2;
	  }

	obstack_sgrow (&format_obstack, cp);
	obstack_sgrow (&format_obstack, ", ");
	j += strsize;
      }
    /* Add a NULL entry to list of tokens (well, 0, as NULL might not be
       defined).  */
    obstack_sgrow (&format_obstack, "0");

    /* Finish table and store. */
    obstack_1grow (&format_obstack, 0);
    muscle_insert ("tname", obstack_finish (&format_obstack));
  }

    /* Output YYTOKNUM. */
  {
    int i;
    short *values = XCALLOC (short, ntokens + 1);
    for (i = 0; i < ntokens + 1; ++i)
      values[i] = symbols[i]->user_token_number;
    output_short_table (&format_obstack, values,
		       0, 1, ntokens + 1);
    muscle_insert ("toknum", obstack_finish (&format_obstack));
    free (values);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline.                                                       |
`-------------------------------------------------------------*/

static void
prepare_rules (void)
{
  long int max;
  int r;
  unsigned int i = 0;
  item_number_t *rhs = XMALLOC (item_number_t, nritems);
  unsigned int *prhs = XMALLOC (unsigned int, nrules + 1);
  unsigned int *rline = XMALLOC (unsigned int, nrules + 1);
  token_number_t *r1 = XMALLOC (token_number_t, nrules + 1);
  unsigned int *r2 = XMALLOC (unsigned int, nrules + 1);

  for (r = 1; r < nrules + 1; ++r)
    {
      item_number_t *rhsp;
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      for (rhsp = rules[r].rhs; *rhsp >= 0; ++rhsp)
	rhs[i++] = *rhsp;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->number;
      /* Length of rule R's RHS. */
      r2[r] = i - prhs[r];
      /* Separator in RHS. */
      rhs[i++] = -1;
      /* Line where rule was defined. */
      rline[r] = rules[r].line;
    }
  assert (i == nritems);

  max = output_item_number_table (&format_obstack, rhs, ritem[0], 1, nritems);
  muscle_insert ("rhs", obstack_finish (&format_obstack));
  MUSCLE_INSERT_LONG_INT ("rhs_max", max);

  max = output_unsigned_int_table (&format_obstack, prhs, 0, 1, nrules + 1);
  muscle_insert ("prhs", obstack_finish (&format_obstack));
  MUSCLE_INSERT_LONG_INT ("prhs_max", max);

  max = output_unsigned_int_table (&format_obstack, rline, 0, 1, nrules + 1);
  muscle_insert ("rline", obstack_finish (&format_obstack));
  MUSCLE_INSERT_LONG_INT ("rline_max", max);

  output_token_number_table (&format_obstack, r1, 0, 1, nrules + 1);
  muscle_insert ("r1", obstack_finish (&format_obstack));

  max = output_unsigned_int_table (&format_obstack, r2, 0, 1, nrules + 1);
  muscle_insert ("r2", obstack_finish (&format_obstack));
  MUSCLE_INSERT_LONG_INT ("r2_max", max);

  free (rhs);
  free (prhs);
  free (rline);
  free (r1);
  free (r2);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

static void
prepare_states (void)
{
  size_t i;
  token_number_t *values =
    (token_number_t *) alloca (sizeof (token_number_t) * nstates);
  for (i = 0; i < nstates; ++i)
    values[i] = states[i]->accessing_symbol;
  output_token_number_table (&format_obstack, values,
			     0, 1, nstates);
  muscle_insert ("stos", obstack_finish (&format_obstack));
}


/*------------------------------------------------------------------.
| Decide what to do for each type of token if seen as the lookahead |
| token in specified state.  The value returned is used as the      |
| default action (yydefact) for the state.  In addition, actrow is  |
| filled with what to do for each kind of token, index by symbol    |
| number, with zero meaning do the default action.  The value       |
| SHRT_MIN, a very negative number, means this situation is an      |
| error.  The parser recognizes this value specially.               |
|                                                                   |
| This is where conflicts are resolved.  The loop over lookahead    |
| rules considered lower-numbered rules last, and the last rule     |
| considered that likes a token gets to handle it.                  |
`------------------------------------------------------------------*/

static int
action_row (state_t *state)
{
  int i;
  int default_rule = 0;
  reductions *redp = state->reductions;
  shifts *shiftp = state->shifts;
  errs *errp = state->errs;
  /* set nonzero to inhibit having any default reduction */
  int nodefault = 0;

  for (i = 0; i < ntokens; i++)
    actrow[i] = 0;

  if (redp->nreds >= 1)
    {
      int j;
      /* loop over all the rules available here which require
	 lookahead */
      for (i = state->nlookaheads - 1; i >= 0; --i)
	/* and find each token which the rule finds acceptable
	   to come next */
	for (j = 0; j < ntokens; j++)
	  /* and record this rule as the rule to use if that
	     token follows.  */
	  if (bitset_test (LA[state->lookaheadsp + i], j))
	    actrow[j] = -LArule[state->lookaheadsp + i]->number;
    }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  for (i = 0; i < shiftp->nshifts; i++)
    {
      token_number_t symbol;
      int shift_state = shiftp->shifts[i];
      if (!shift_state)
	continue;

      symbol = states[shift_state]->accessing_symbol;

      if (ISVAR (symbol))
	break;

      actrow[symbol] = shift_state;

      /* Do not use any default reduction if there is a shift for
	 error */
      if (symbol == errtoken->number)
	nodefault = 1;
    }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record SHRT_MIN as the action.  */
  for (i = 0; i < errp->nerrs; i++)
    {
      int symbol = errp->errs[i];
      actrow[symbol] = SHRT_MIN;
    }

  /* Now find the most common reduction and make it the default action
     for this state.  */

  if (redp->nreds >= 1 && !nodefault)
    {
      if (state->consistent)
	default_rule = redp->rules[0];
      else
	{
	  int max = 0;
	  for (i = 0; i < state->nlookaheads; i++)
	    {
	      int count = 0;
	      int rule = -LArule[state->lookaheadsp + i]->number;
	      int j;

	      for (j = 0; j < ntokens; j++)
		if (actrow[j] == rule)
		  count++;

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
	      int j;
	      for (j = 0; j < ntokens; j++)
		if (actrow[j] == default_rule)
		  actrow[j] = 0;

	      default_rule = -default_rule;
	    }
	}
    }

  /* If have no default rule, the default is an error.
     So replace any action which says "error" with "use default".  */

  if (default_rule == 0)
    for (i = 0; i < ntokens; i++)
      if (actrow[i] == SHRT_MIN)
	actrow[i] = 0;

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
  size_t i;
  short *yydefact = XCALLOC (short, nstates);

  actrow = XCALLOC (short, ntokens);
  for (i = 0; i < nstates; ++i)
    {
      yydefact[i] = action_row (states[i]);
      save_row (i);
    }

  output_short_table (&format_obstack, yydefact,
		     yydefact[0], 1, nstates);
  muscle_insert ("defact", obstack_finish (&format_obstack));

  XFREE (actrow);
  XFREE (yydefact);
}


/*-----------------------------.
| Output the actions to OOUT.  |
`-----------------------------*/

void
actions_output (FILE *out)
{
  int rule;
  for (rule = 1; rule < nrules + 1; ++rule)
    if (rules[rule].action)
      {
	fprintf (out, "  case %d:\n", rule);

	if (!no_lines_flag)
	  fprintf (out, muscle_find ("linef"),
		   rules[rule].action_line,
		   quotearg_style (c_quoting_style,
				   muscle_find ("filename")));
	/* As a Bison extension, add the ending semicolon.  Since some
	   Yacc don't do that, help people using bison as a Yacc
	   finding their missing semicolons.  */
	fprintf (out, "{ %s%s }\n    break;\n\n",
		 rules[rule].action,
		 yacc_flag ? ";" : "");
      }
}


/*---------------------------------------.
| Output the tokens definition to OOUT.  |
`---------------------------------------*/

void
token_definitions_output (FILE *out)
{
  int i;
  int first = 1;
  for (i = 0; i < ntokens; ++i)
    {
      symbol_t *symbol = symbols[i];
      int number = symbol->user_token_number;

      /* At this stage, if there are literal aliases, they are part of
	 SYMBOLS, so we should not find symbols which are the aliases
	 here.  */
      assert (number != USER_NUMBER_ALIAS);

      /* Skip error token.  */
      if (symbol == errtoken)
	continue;

      /* If this string has an alias, then it is necessarily the alias
	 which is to be output.  */
      if (symbol->alias)
	symbol = symbol->alias;

      /* Don't output literal chars or strings (when defined only as a
	 string).  Note that must be done after the alias resolution:
	 think about `%token 'f' "f"'.  */
      if (symbol->tag[0] == '\'' || symbol->tag[0] == '\"')
	continue;

      /* Don't #define nonliteral tokens whose names contain periods
	 or '$' (as does the default value of the EOF token).  */
      if (strchr (symbol->tag, '.') || strchr (symbol->tag, '$'))
	continue;

      fprintf (out, "%s[[[%s]], [%d]]",
	       first ? "" : ",\n", symbol->tag, number);

      first = 0;
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
  int symno = symbol - ntokens + nstates;

  short begin = goto_map[symbol];
  short end = goto_map[symbol + 1];

  count = 0;
  for (i = begin; i < end; i++)
    if (to_state[i] != default_state)
      count++;

  if (count == 0)
    return;

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
  size_t i;
  size_t m = goto_map[symbol];
  size_t n = goto_map[symbol + 1];
  int default_state = -1;
  int max = 0;

  if (m == n)
    return -1;

  for (i = 0; i < nstates; i++)
    state_count[i] = 0;

  for (i = m; i < n; i++)
    state_count[to_state[i]]++;

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

  output_short_table (&format_obstack, yydefgoto,
		     yydefgoto[0], 1, nsyms - ntokens);
  muscle_insert ("defgoto", obstack_finish (&format_obstack));

  XFREE (state_count);
  XFREE (yydefgoto);
}


/* The next few functions decide how to pack the actions and gotos
   information into yytable. */

static void
sort_actions (void)
{
  int i;

  order = XCALLOC (short, nvectors);
  nentries = 0;

  for (i = 0; i < nvectors; i++)
    if (tally[i] > 0)
      {
	int k;
	int t = tally[i];
	int w = width[i];
	int j = nentries - 1;

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
  int i = order[vector];
  int t;
  int w;
  int prev;

  if (i >= (int) nstates)
    return -1;

  t = tally[i];
  w = width[i];

  for (prev = vector - 1; prev >= 0; prev--)
    {
      int j = order[prev];
      int k;
      int match = 1;

      if (width[j] != w || tally[j] != t)
	return -1;

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
  int i = order[vector];
  int j;
  int t = tally[i];
  int loc = 0;
  short *from = froms[i];
  short *to = tos[i];

  assert (t);

  for (j = lowzero - from[0]; j < (int) table_size; j++)
    {
      int k;
      int ok = 1;

      for (k = 0; ok && k < t; k++)
	{
	  loc = j + from[k];
	  if (loc > (int) table_size)
	    table_grow (loc);

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
#define pack_vector_succeeded 0
  assert (pack_vector_succeeded);
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
  table = XCALLOC (short, table_size);
  check = XCALLOC (short, table_size);

  lowzero = 0;
  high = 0;

  for (i = 0; i < nvectors; i++)
    base[i] = SHRT_MIN;

  for (i = 0; i < (int) table_size; i++)
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
  output_short_table (&format_obstack, base,
		     base[0], 1, nstates);
  muscle_insert ("pact", obstack_finish (&format_obstack));

  /* Output pgoto. */
  output_short_table (&format_obstack, base,
		     base[nstates], nstates + 1, nvectors);
  muscle_insert ("pgoto", obstack_finish (&format_obstack));

  XFREE (base);
}


static void
output_table (void)
{
  output_short_table (&format_obstack, table,
		     table[0], 1, high + 1);
  muscle_insert ("table", obstack_finish (&format_obstack));
  XFREE (table);
}


static void
output_check (void)
{
  output_short_table (&format_obstack, check,
		     check[0], 1, high + 1);
  muscle_insert ("check", obstack_finish (&format_obstack));
  XFREE (check);
}

/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

static void
output_actions (void)
{
  size_t i;
  nvectors = nstates + nvars;

  froms = XCALLOC (short *, nvectors);
  tos = XCALLOC (short *, nvectors);
  tally = XCALLOC (short, nvectors);
  width = XCALLOC (short, nvectors);

  token_actions ();
  bitsetv_free (LA);
  free (LArule);

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
      free (states[i]->shifts);
      XFREE (states[i]->reductions);
      free (states[i]->errs);
      free (states[i]);
    }
  XFREE (states);
}


/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

static void
output_skeleton (void)
{
  /* Store the definition of all the muscles. */
  const char *tempdir = getenv ("TMPDIR");
  char *tempfile = NULL;
  FILE *out = NULL;
  int fd;

  if (tempdir == NULL)
    tempdir = DEFAULT_TMPDIR;
  tempfile = xmalloc (strlen (tempdir) + 11);
  sprintf (tempfile, "%s/bsnXXXXXX", tempdir);
  fd = mkstemp (tempfile);
  if (fd == -1)
    error (EXIT_FAILURE, errno, "%s", tempfile);

  out = fdopen (fd, "w");
  if (out == NULL)
    error (EXIT_FAILURE, errno, "%s", tempfile);

  /* There are no comments, especially not `#': we do want M4 expansion
     after `#': think of CPP macros!  */
  fputs ("m4_changecom()\n", out);
  fputs ("m4_init()\n", out);

  fputs ("m4_define([b4_actions], \n[[", out);
  actions_output (out);
  fputs ("]])\n\n", out);

  fputs ("m4_define([b4_tokens], \n[", out);
  token_definitions_output (out);
  fputs ("])\n\n", out);

  muscles_m4_output (out);

  fputs ("m4_wrap([m4_divert_pop(0)])\n", out);
  fputs ("m4_divert_push(0)dnl\n", out);
  xfclose (out);

  /* Invoke m4 on the definition of the muscles, and the skeleton. */
  {
    const char *bison_pkgdatadir = getenv ("BISON_PKGDATADIR");
    const char *m4 = getenv ("M4");
    if (!m4)
      m4 = M4;
    if (!bison_pkgdatadir)
      bison_pkgdatadir = PKGDATADIR;
    if (trace_flag)
      fprintf (stderr,
	       "running: %s -I %s m4sugar/m4sugar.m4 %s %s\n",
	       m4, bison_pkgdatadir, tempfile, skeleton);
    skel_in = readpipe (m4,
			"-I", bison_pkgdatadir,
			"m4sugar/m4sugar.m4",
			tempfile,
			skeleton,
			NULL);
    if (!skel_in)
      error (EXIT_FAILURE, errno, "cannot run m4");
    skel_lex ();

    /* If `debugging', keep this file alive. */
    if (!trace_flag)
      unlink (tempfile);
  }
}

static void
prepare (void)
{
  MUSCLE_INSERT_INT ("last", high);
  MUSCLE_INSERT_INT ("flag", SHRT_MIN);
  MUSCLE_INSERT_INT ("pure", pure_parser);
  MUSCLE_INSERT_INT ("nsym", nsyms);
  MUSCLE_INSERT_INT ("debug", debug_flag);
  MUSCLE_INSERT_INT ("final", final_state);
  MUSCLE_INSERT_INT ("undef_token_number", undeftoken->number);
  MUSCLE_INSERT_INT ("user_token_number_max", max_user_token_number);
  MUSCLE_INSERT_INT ("error_verbose", error_verbose);
  MUSCLE_INSERT_STRING ("prefix", spec_name_prefix ? spec_name_prefix : "yy");

  /* FIXME: This is wrong: the muscles should decide whether they hold
     a copy or not, but the situation is too obscure currently.  */
  MUSCLE_INSERT_STRING ("output_infix", output_infix ? output_infix : "");
  MUSCLE_INSERT_STRING ("output_prefix", short_base_name);
  MUSCLE_INSERT_STRING ("output_parser_name", parser_file_name);
  MUSCLE_INSERT_STRING ("output_header_name", spec_defines_file);

  MUSCLE_INSERT_INT ("nnts", nvars);
  MUSCLE_INSERT_INT ("nrules", nrules);
  MUSCLE_INSERT_INT ("nstates", nstates);
  MUSCLE_INSERT_INT ("ntokens", ntokens);

  MUSCLE_INSERT_INT ("locations_flag", locations_flag);
  MUSCLE_INSERT_INT ("defines_flag", defines_flag);

  /* Copy definitions in directive.  */
  obstack_1grow (&pre_prologue_obstack, 0);
  obstack_1grow (&post_prologue_obstack, 0);
  muscle_insert ("pre_prologue", obstack_finish (&pre_prologue_obstack));
  muscle_insert ("post_prologue", obstack_finish (&post_prologue_obstack));

  /* Find the right skeleton file.  */
  if (!skeleton)
    skeleton = "bison.simple";

  /* Parse the skeleton file and output the needed parsers.  */
  muscle_insert ("skeleton", skeleton);
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

void
output (void)
{
  obstack_init (&format_obstack);

  prepare_tokens ();
  prepare_rules ();
  prepare_states ();
  output_actions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  obstack_free (&muscle_obstack, NULL);
  obstack_free (&format_obstack, NULL);
  obstack_free (&action_obstack, NULL);
  obstack_free (&pre_prologue_obstack, NULL);
  obstack_free (&post_prologue_obstack, NULL);
}
