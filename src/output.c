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


/* The parser tables consist of these tables.

   YYTRANSLATE = vector mapping yylex's token numbers into bison's
   token numbers.

   YYTNAME = vector of string-names indexed by bison token number.

   YYTOKNUM = vector of yylex token numbers corresponding to entries
   in YYTNAME.

   YYRLINE = vector of line-numbers of all rules.  For yydebug
   printouts.

   YYRHS = vector of items of all rules.  This is exactly what RITEMS
   contains.  For yydebug and for semantic parser.

   YYPRHS[R] = index in YYRHS of first item for rule R.

   YYR1[R] = symbol number of symbol that rule R derives.

   YYR2[R] = number of symbols composing right hand side of rule R.

   YYSTOS[S] = the symbol number of the symbol that leads to state S.

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

   Suppose that the portion of YYTABLE starts at index P and the index
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

/* From src/scan-skel.l. */
void m4_invoke PARAMS ((const char *definitions));


/* Several tables will be indexed both by state and nonterminal
   numbers.  We call `vector' such a thing (= either a state or a
   symbol number.

   Of course vector_number_t ought to be wide enough to contain
   state_number_t and symbol_number_t.  */
typedef short vector_number_t;
#define VECTOR_NUMBER_MAX ((vector_number_t) SHRT_MAX)
#define VECTOR_NUMBER_MIN ((vector_number_t) SHRT_MIN)
#define state_number_to_vector_number(State) \
   ((vector_number_t) State)
#define symbol_number_to_vector_number(Symbol) \
   ((vector_number_t) (state_number_as_int (nstates) + Symbol - ntokens))

static int nvectors;


/* FROMS and TOS are indexed by vector_number_t.

   If VECTOR is a nonterminal, (FROMS[VECTOR], TOS[VECTOR]) form an
   array of state numbers of the non defaulted GOTO on VECTOR.

   If VECTOR is a state, TOS[VECTOR] is the array of actions to do on
   the (array of) symbols FROMS[VECTOR].

   In both cases, TALLY[VECTOR] is the size of the arrays
   FROMS[VECTOR], TOS[VECTOR]; and WIDTH[VECTOR] =
   (FROMS[VECTOR][SIZE] - FROMS[VECTOR][0] + 1) where SIZE =
   TALLY[VECTOR].

   FROMS therefore contains symbol_number_t and action_number_t,
   TOS state_number_t and action_number_t,
   TALLY sizes,
   WIDTH differences of FROMS.

   Let base_t be the type of FROMS, TOS, and WIDTH.  */
typedef int base_t;
#define BASE_MAX ((base_t) INT_MAX)
#define BASE_MIN ((base_t) INT_MIN)

static base_t **froms = NULL;
static base_t **tos = NULL;
static unsigned int **conflict_tos = NULL;
static short *tally = NULL;
static base_t *width = NULL;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for `run the default action'.
   If N = MIN, stands for `raise a parse error'.
   If N > 0, stands for `shift SYMBOL and go to n'.
   If N < 0, stands for `reduce -N'.  */
typedef short action_t;
#define ACTION_MAX ((action_t) SHRT_MAX)
#define ACTION_MIN ((action_t) SHRT_MIN)

static action_t *actrow = NULL;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip `empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these `entries'.  */
static vector_number_t *order = NULL;
static int nentries;

static base_t *base = NULL;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MIN, later mapped to BASE_NINF to
   keep parser tables small.  */
base_t base_ninf = 0;
static base_t *pos = NULL;

static unsigned int *conflrow = NULL;
static unsigned int *conflict_table = NULL;
static unsigned int *conflict_list = NULL;
static int conflict_list_cnt;
static int conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.
   We start with the original hard-coded value: SHRT_MAX
   (yes, not USHRT_MAX). */
static size_t table_size = SHRT_MAX;
static base_t *table = NULL;
static base_t *check = NULL;
/* The value used in TABLE to denote explicit parse errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_MIN,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_t table_ninf = 0;
static int lowzero;
static int high;

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

  table = XREALLOC (table, base_t, table_size);
  check = XREALLOC (check, base_t, table_size);
  if (glr_parser)
    conflict_table = XREALLOC (conflict_table, unsigned int, table_size);

  for (/* Nothing. */; old_size < table_size; ++old_size)
    {
      table[old_size] = 0;
      check[old_size] = -1;
    }
}


/*-------------------------------------------------------------------.
| Create a function NAME which associates to the muscle NAME the     |
| result of formatting the FIRST and then TABLE_DATA[BEGIN..END[ (of |
| TYPE), and to the muscle NAME_max, the max value of the            |
| TABLE_DATA.                                                        |
`-------------------------------------------------------------------*/


#define GENERATE_MUSCLE_INSERT_TABLE(Name, Type)			\
									\
static void								\
Name (const char *name,							\
      Type *table_data,							\
      Type first,							\
      int begin,							\
      int end)								\
{									\
  Type min = first;							\
  Type max = first;							\
  int i;								\
  int j = 1;								\
									\
  obstack_fgrow1 (&format_obstack, "%6d", first);			\
  for (i = begin; i < end; ++i)						\
    {									\
      obstack_1grow (&format_obstack, ',');				\
      if (j >= 10)							\
	{								\
	  obstack_sgrow (&format_obstack, "\n  ");			\
	  j = 1;							\
	}								\
      else								\
	++j;								\
      obstack_fgrow1 (&format_obstack, "%6d", table_data[i]);		\
      if (table_data[i] < min)						\
	min = table_data[i];						\
      if (max < table_data[i])						\
	max = table_data[i];						\
    }									\
  obstack_1grow (&format_obstack, 0);					\
  muscle_insert (name, obstack_finish (&format_obstack));		\
									\
  /* Build `NAME_min' and `NAME_max' in the obstack. */			\
  obstack_fgrow1 (&format_obstack, "%s_min", name);			\
  obstack_1grow (&format_obstack, 0);					\
  MUSCLE_INSERT_LONG_INT (obstack_finish (&format_obstack),		\
			  (long int) min);				\
  obstack_fgrow1 (&format_obstack, "%s_max", name);			\
  obstack_1grow (&format_obstack, 0);					\
  MUSCLE_INSERT_LONG_INT (obstack_finish (&format_obstack),		\
			  (long int) max);				\
}

GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_unsigned_int_table, unsigned int)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_int_table, int)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_short_table, short)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_base_table, base_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_rule_number_table, rule_number_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_symbol_number_table, symbol_number_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_item_number_table, item_number_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_state_number_table, state_number_t)


/*-----------------------------------------------------------------.
| Prepare the muscles related to the tokens: translate, tname, and |
| toknum.                                                          |
`-----------------------------------------------------------------*/

static void
prepare_tokens (void)
{
  muscle_insert_symbol_number_table ("translate",
				    token_translations,
				    0, 1, max_user_token_number + 1);

  {
    int i;
    int j = 0;
    for (i = 0; i < nsyms; i++)
      {
	/* Be sure not to use twice the same QUOTEARG slot:
	   SYMBOL_TAG_GET uses slot 0.  */
	const char *cp =
	  quotearg_n_style (1, c_quoting_style,
			    symbols[i]->tag);
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
    int *values = XCALLOC (int, ntokens);
    for (i = 0; i < ntokens; ++i)
      values[i] = symbols[i]->user_token_number;
    muscle_insert_int_table ("toknum", values,
			     values[0], 1, ntokens);
    free (values);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger                                         |
`-------------------------------------------------------------*/

static void
prepare_rules (void)
{
  rule_number_t r;
  unsigned int i = 0;
  item_number_t *rhs = XMALLOC (item_number_t, nritems);
  unsigned int *prhs = XMALLOC (unsigned int, nrules);
  unsigned int *rline = XMALLOC (unsigned int, nrules);
  symbol_number_t *r1 = XMALLOC (symbol_number_t, nrules);
  unsigned int *r2 = XMALLOC (unsigned int, nrules);
  short *dprec = XMALLOC (short, nrules);
  short *merger = XMALLOC (short, nrules);

  for (r = 0; r < nrules; ++r)
    {
      item_number_t *rhsp = NULL;
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
      rline[r] = rules[r].location.first_line;
      /* Dynamic precedence (GLR) */
      dprec[r] = rules[r].dprec;
      /* Merger-function index (GLR) */
      merger[r] = rules[r].merger;
    }
  assert (i == nritems);

  muscle_insert_item_number_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_insert_unsigned_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_insert_unsigned_int_table ("rline", rline, 0, 0, nrules);
  muscle_insert_symbol_number_table ("r1", r1, 0, 0, nrules);
  muscle_insert_unsigned_int_table ("r2", r2, 0, 0, nrules);
  muscle_insert_short_table ("dprec", dprec, 0, 0, nrules);
  muscle_insert_short_table ("merger", merger, 0, 0, nrules);

  free (rhs);
  free (prhs);
  free (rline);
  free (r1);
  free (r2);
  free (dprec);
  free (merger);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

static void
prepare_states (void)
{
  state_number_t i;
  symbol_number_t *values =
    (symbol_number_t *) alloca (sizeof (symbol_number_t) * nstates);
  for (i = 0; i < nstates; ++i)
    values[i] = states[i]->accessing_symbol;
  muscle_insert_symbol_number_table ("stos", values,
				     0, 1, nstates);
}


/*-------------------------------------------------------------------.
| For GLR parsers, for each conflicted token in STATE, as indicated  |
| by non-zero entries in CONFLROW, create a list of possible 	     |
| reductions that are alternatives to the shift or reduction	     |
| currently recorded for that token in STATE.  Store the alternative |
| reductions followed by a 0 in CONFLICT_LIST, updating		     |
| CONFLICT_LIST_CNT, and storing an index to the start of the list   |
| back into CONFLROW.						     |
`-------------------------------------------------------------------*/

static void
conflict_row (state_t *state)
{
  int i, j;

  if (! glr_parser)
    return;

  for (j = 0; j < ntokens; j += 1)
    if (conflrow[j])
      {
	conflrow[j] = conflict_list_cnt;

	/* Find all reductions for token J, and record all that do not
	   match ACTROW[J].  */
	for (i = 0; i < state->nlookaheads; i += 1)
	  if (bitset_test (state->lookaheads[i], j)
	      && (actrow[j]
		  != rule_number_as_item_number (state->lookaheads_rule[i]->number)))
	    {
	      assert (conflict_list_free > 0);
	      conflict_list[conflict_list_cnt]
		= state->lookaheads_rule[i]->number + 1;
	      conflict_list_cnt += 1;
	      conflict_list_free -= 1;
	    }

	/* Leave a 0 at the end.  */
	assert (conflict_list_free > 0);
	conflict_list_cnt += 1;
	conflict_list_free -= 1;
      }
}


/*------------------------------------------------------------------.
| Decide what to do for each type of token if seen as the lookahead |
| token in specified state.  The value returned is used as the      |
| default action (yydefact) for the state.  In addition, ACTROW is  |
| filled with what to do for each kind of token, index by symbol    |
| number, with zero meaning do the default action.  The value       |
| ACTION_MIN, a very negative number, means this situation is an    |
| error.  The parser recognizes this value specially.               |
|                                                                   |
| This is where conflicts are resolved.  The loop over lookahead    |
| rules considered lower-numbered rules last, and the last rule     |
| considered that likes a token gets to handle it.                  |
|                                                                   |
| For GLR parsers, also sets CONFLROW[SYM] to an index into         |
| CONFLICT_LIST iff there is an unresolved conflict (s/r or r/r)    |
| with symbol SYM. The default reduction is not used for a symbol   |
| that has any such conflicts.                                      |
`------------------------------------------------------------------*/

static rule_number_t
action_row (state_t *state)
{
  int i;
  rule_number_t default_rule = -1;
  reductions_t *redp = state->reductions;
  transitions_t *transitions = state->transitions;
  errs_t *errp = state->errs;
  /* set nonzero to inhibit having any default reduction */
  int nodefault = 0;
  int conflicted = 0;

  for (i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  if (redp->num >= 1)
    {
      int j;
      bitset_iterator biter;
      /* loop over all the rules available here which require
	 lookahead */
      for (i = state->nlookaheads - 1; i >= 0; --i)
	/* and find each token which the rule finds acceptable
	   to come next */
	BITSET_FOR_EACH (biter, state->lookaheads[i], j, 0)
	{
	  /* and record this rule as the rule to use if that
	     token follows.  */
	  if (actrow[j] != 0)
	    conflicted = conflrow[j] = 1;
	  actrow[j] = rule_number_as_item_number (state->lookaheads_rule[i]->number);
	}
    }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  for (i = 0; i < transitions->num && TRANSITION_IS_SHIFT (transitions, i); i++)
    if (!TRANSITION_IS_DISABLED (transitions, i))
      {
	symbol_number_t symbol = TRANSITION_SYMBOL (transitions, i);
	state_number_t shift_state = transitions->states[i];

	if (actrow[symbol] != 0)
	  conflicted = conflrow[symbol] = 1;
	actrow[symbol] = state_number_as_int (shift_state);

	/* Do not use any default reduction if there is a shift for
	   error */
	if (symbol == errtoken->number)
	  nodefault = 1;
      }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_MIN as the action.  */
  for (i = 0; i < errp->num; i++)
    {
      symbol_number_t symbol = errp->symbols[i];
      actrow[symbol] = ACTION_MIN;
    }

  /* Now find the most common reduction and make it the default action
     for this state.  */

  if (redp->num >= 1 && !nodefault)
    {
      if (state->consistent)
	default_rule = redp->rules[0];
      else
	{
	  int max = 0;
	  for (i = 0; i < state->nlookaheads; i++)
	    {
	      int count = 0;
	      rule_number_t rule = state->lookaheads_rule[i]->number;
	      symbol_number_t j;

	      for (j = 0; j < ntokens; j++)
		if (actrow[j] == rule_number_as_item_number (rule))
		  count++;

	      if (count > max)
		{
		  max = count;
		  default_rule = rule;
		}
	    }

	  /* GLR parsers need space for conflict lists, so we can't
	     default conflicted entries.  For non-conflicted entries
	     or as long as we are not building a GLR parser,
	     actions that match the default are replaced with zero,
	     which means "use the default". */

	  if (max > 0)
	    {
	      int j;
	      for (j = 0; j < ntokens; j++)
		if (actrow[j] == rule_number_as_item_number (default_rule)
		    && ! (glr_parser && conflrow[j]))
		  actrow[j] = 0;
	    }
	}
    }

  /* If have no default rule, the default is an error.
     So replace any action which says "error" with "use default".  */

  if (default_rule == -1)
    for (i = 0; i < ntokens; i++)
      if (actrow[i] == ACTION_MIN)
	actrow[i] = 0;

  if (conflicted)
    conflict_row (state);

  return default_rule;
}


/*--------------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for STATE.  |
`--------------------------------------------*/

static void
save_row (state_number_t state)
{
  symbol_number_t i;
  int count;
  base_t *sp = NULL;
  base_t *sp1 = NULL;
  base_t *sp2 = NULL;
  unsigned int *sp3 = NULL;

  /* Number of non default actions in STATE.  */
  count = 0;
  for (i = 0; i < ntokens; i++)
    if (actrow[i] != 0)
      count++;

  if (count == 0)
    return;

  /* Allocate non defaulted actions.  */
  froms[state] = sp1 = sp = XCALLOC (base_t, count);
  tos[state] = sp2 = XCALLOC (base_t, count);
  if (glr_parser)
    conflict_tos[state] = sp3 = XCALLOC (unsigned int, count);
  else
    conflict_tos[state] = NULL;

  /* Store non defaulted actions.  */
  for (i = 0; i < ntokens; i++)
    if (actrow[i] != 0)
      {
	*sp1++ = i;
	*sp2++ = actrow[i];
	if (glr_parser)
	  *sp3++ = conflrow[i];
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
  state_number_t i;
  int nconflict = conflicts_total_count ();

  rule_number_t *yydefact = XCALLOC (rule_number_t, nstates);

  actrow = XCALLOC (action_t, ntokens);
  conflrow = XCALLOC (unsigned int, ntokens);

  if (glr_parser)
    {
      conflict_list = XCALLOC (unsigned int, 1 + 2 * nconflict);
      conflict_list_free = 2 * nconflict;
      conflict_list_cnt = 1;
    }
  else
    conflict_list_free = conflict_list_cnt = 0;

  for (i = 0; i < nstates; ++i)
    {
      yydefact[i] = action_row (states[i]) + 1;
      save_row (i);
    }

  muscle_insert_rule_number_table ("defact", yydefact,
				   yydefact[0], 1, nstates);
  XFREE (actrow);
  XFREE (conflrow);
  XFREE (yydefact);
}


/*-----------------------------.
| Output the actions to OOUT.  |
`-----------------------------*/

void
actions_output (FILE *out)
{
  rule_number_t r;

  fputs ("m4_define([b4_actions], \n[[", out);
  for (r = 0; r < nrules; ++r)
    if (rules[r].action)
      {
	fprintf (out, "  case %d:\n", r + 1);

	if (!no_lines_flag)
	  fprintf (out, muscle_find ("linef"),
		   rules[r].action_location.first_line,
		   quotearg_style (c_quoting_style,
				   muscle_find ("filename")));
	fprintf (out, "    %s\n    break;\n\n",
		 rules[r].action);
      }
  fputs ("]])\n\n", out);
}

/*--------------------------------------.
| Output the merge functions to OUT.   |
`--------------------------------------*/

static void
merger_output (FILE *out)
{
  int n;
  merger_list* p;

  fputs ("m4_define([b4_mergers], \n[[", out);
  for (n = 1, p = merge_functions; p != NULL; n += 1, p = p->next)
    {
      if (p->type[0] == '\0')
	fprintf (out, "  case %d: yyval = %s (*yy0, *yy1); break;\n",
		 n, p->name);
      else
	fprintf (out, "  case %d: yyval.%s = %s (*yy0, *yy1); break;\n",
		 n, p->type, p->name);
    }
  fputs ("]])\n\n", out);
}

/*---------------------------------------.
| Output the tokens definition to OOUT.  |
`---------------------------------------*/

void
token_definitions_output (FILE *out)
{
  int i;
  int first = 1;

  fputs ("m4_define([b4_tokens], \n[", out);
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
  fputs ("])\n\n", out);
}


/*----------------------------------------.
| Output the symbol destructors to OOUT.  |
`----------------------------------------*/

static void
symbol_destructors_output (FILE *out)
{
  int i;
  int first = 1;

  fputs ("m4_define([b4_symbol_destructors], \n[", out);
  for (i = 0; i < nsyms; ++i)
    if (symbols[i]->destructor)
      {
	symbol_t *symbol = symbols[i];

	/* Filename, lineno,
	   Symbol-name, Symbol-number,
	   destructor, typename. */
	fprintf (out, "%s[[[%s]], [[%d]], [[%s]], [[%d]], [[%s]], [[%s]]]",
		 first ? "" : ",\n",
		 infile, symbol->destructor_location.first_line,
		 symbol->tag,
		 symbol->number,
		 symbol->destructor,
		 symbol->type_name);

	first = 0;
      }
  fputs ("])\n\n", out);
}


/*-------------------------------------.
| Output the symbol printers to OOUT.  |
`-------------------------------------*/

static void
symbol_printers_output (FILE *out)
{
  int i;
  int first = 1;

  fputs ("m4_define([b4_symbol_printers], \n[", out);
  for (i = 0; i < nsyms; ++i)
    if (symbols[i]->destructor)
      {
	symbol_t *symbol = symbols[i];

	/* Filename, lineno,
	   Symbol-name, Symbol-number,
	   destructor, typename. */
	fprintf (out, "%s[[[%s]], [[%d]], [[%s]], [[%d]], [[%s]], [[%s]]]",
		 first ? "" : ",\n",
		 infile, symbol->printer_location.first_line,
		 symbol->tag,
		 symbol->number,
		 symbol->printer,
		 symbol->type_name);

	first = 0;
      }
  fputs ("])\n\n", out);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYMBOL.                                                           |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYMBOL, i.e., the   |
| default GOTO destination on SYMBOL.                               |
`------------------------------------------------------------------*/

static void
save_column (symbol_number_t symbol, state_number_t default_state)
{
  int i;
  base_t *sp;
  base_t *sp1;
  base_t *sp2;
  int count;
  vector_number_t symno = symbol_number_to_vector_number (symbol);

  goto_number_t begin = goto_map[symbol];
  goto_number_t end = goto_map[symbol + 1];

  /* Number of non default GOTO.  */
  count = 0;
  for (i = begin; i < end; i++)
    if (to_state[i] != default_state)
      count++;

  if (count == 0)
    return;

  /* Allocate room for non defaulted gotos.  */
  froms[symno] = sp1 = sp = XCALLOC (base_t, count);
  tos[symno] = sp2 = XCALLOC (base_t, count);

  /* Store the state numbers of the non defaulted gotos.  */
  for (i = begin; i < end; i++)
    if (to_state[i] != default_state)
      {
	*sp1++ = from_state[i];
	*sp2++ = to_state[i];
      }

  tally[symno] = count;
  width[symno] = sp1[-1] - sp[0] + 1;
}


/*----------------------------------------------------------------.
| Return `the' most common destination GOTO on SYMBOL (a nterm).  |
`----------------------------------------------------------------*/

static state_number_t
default_goto (symbol_number_t symbol, short state_count[])
{
  state_number_t s;
  int i;
  goto_number_t m = goto_map[symbol];
  goto_number_t n = goto_map[symbol + 1];
  state_number_t default_state = (state_number_t) -1;
  int max = 0;

  if (m == n)
    return (state_number_t) -1;

  for (s = 0; s < nstates; s++)
    state_count[s] = 0;

  for (i = m; i < n; i++)
    state_count[to_state[i]]++;

  for (s = 0; s < nstates; s++)
    if (state_count[s] > max)
      {
	max = state_count[s];
	default_state = s;
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
  symbol_number_t i;
  state_number_t *yydefgoto = XMALLOC (state_number_t, nvars);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  short *state_count = XCALLOC (short, nstates);
  for (i = ntokens; i < nsyms; ++i)
    {
      state_number_t default_state = default_goto (i, state_count);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }

  muscle_insert_state_number_table ("defgoto", yydefgoto,
				    yydefgoto[0], 1, nsyms - ntokens);
  XFREE (state_count);
  XFREE (yydefgoto);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

static void
sort_actions (void)
{
  int i;

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


/* If VECTOR is a state which actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

static state_number_t
matching_state (vector_number_t vector)
{
  vector_number_t i = order[vector];
  int t;
  int w;
  int prev;

  /* If VECTOR is a nterm, return -1.  */
  if (i >= (int) nstates)
    return -1;

  t = tally[i];
  w = width[i];

  for (prev = vector - 1; prev >= 0; prev--)
    {
      vector_number_t j = order[prev];
      int k;
      int match = 1;

      /* Given how ORDER was computed, if the WIDTH or TALLY is
	 different, there cannot be a matching state.  */
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


static base_t
pack_vector (vector_number_t vector)
{
  vector_number_t i = order[vector];
  int j;
  int t = tally[i];
  int loc = 0;
  base_t *from = froms[i];
  base_t *to = tos[i];
  unsigned int *conflict_to = conflict_tos[i];

  assert (t);

  for (j = lowzero - from[0]; j < (int) table_size; j++)
    {
      int k;
      int ok = 1;

      for (k = 0; ok && k < t; k++)
	{
	  loc = j + state_number_as_int (from[k]);
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
	      if (glr_parser && conflict_to != NULL)
		conflict_table[loc] = conflict_to[k];
	      check[loc] = from[k];
	    }

	  while (table[lowzero] != 0)
	    lowzero++;

	  if (loc > high)
	    high = loc;

	  if (j < BASE_MIN || BASE_MAX < j)
	    fatal ("base_t too small to hold %d\n", j);
	  return j;
	}
    }
#define pack_vector_succeeded 0
  assert (pack_vector_succeeded);
  return 0;
}


/*-------------------------------------------------------------.
| Remap the negative infinite in TAB from NINF to the greatest |
| possible smallest value.  Return it.                         |
|                                                              |
| In most case this allows us to use shorts instead of ints in |
| parsers.                                                     |
`-------------------------------------------------------------*/

static base_t
table_ninf_remap (base_t tab[], size_t size, base_t ninf)
{
  base_t res = 0;
  size_t i;

  for (i = 0; i < size; i++)
    if (tab[i] < res && tab[i] != ninf)
      res = base[i];

  --res;

  for (i = 0; i < size; i++)
    if (tab[i] == ninf)
      tab[i] = res;

  return res;
}

static void
pack_table (void)
{
  int i;

  base = XCALLOC (base_t, nvectors);
  pos = XCALLOC (base_t, nentries);
  table = XCALLOC (base_t, table_size);
  if (glr_parser)
    conflict_table = XCALLOC (unsigned int, table_size);
  check = XCALLOC (base_t, table_size);

  lowzero = 0;
  high = 0;

  for (i = 0; i < nvectors; i++)
    base[i] = BASE_MIN;

  for (i = 0; i < (int) table_size; i++)
    check[i] = -1;

  for (i = 0; i < nentries; i++)
    {
      state_number_t state = matching_state (i);
      base_t place;

      if (state < 0)
	/* A new set of state actions, or a nonterminal.  */
	place = pack_vector (i);
      else
	/* Action of I were already coded for STATE.  */
	place = base[state];

      pos[i] = place;
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MIN);
  table_ninf = table_ninf_remap (table, high + 1, ACTION_MIN);

  for (i = 0; i < nvectors; i++)
    {
      XFREE (froms[i]);
      XFREE (tos[i]);
      XFREE (conflict_tos[i]);
    }

  free (froms);
  free (tos);
  free (conflict_tos);
  free (pos);
}


/* the following functions output yytable, yycheck, yyconflp, yyconfl,
   and the vectors whose elements index the portion starts.  */

static void
output_base (void)
{
  /* Output PACT. */
  muscle_insert_base_table ("pact", base,
			     base[0], 1, nstates);
  MUSCLE_INSERT_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_insert_base_table ("pgoto", base,
			     base[nstates], nstates + 1, nvectors);
  XFREE (base);
}


static void
output_table (void)
{
  muscle_insert_base_table ("table", table,
			    table[0], 1, high + 1);
  MUSCLE_INSERT_INT ("table_ninf", table_ninf);
  XFREE (table);
}


static void
output_conflicts (void)
{
  /* GLR parsing slightly modifies yytable and yycheck
     (and thus yypact) so that in states with unresolved conflicts,
     the default reduction is not used in the conflicted entries, so
     that there is a place to put a conflict pointer.  This means that
     yyconflp and yyconfl are nonsense for a non-GLR parser, so we
     avoid accidents by not writing them out in that case. */
  if (! glr_parser)
    return;

  muscle_insert_unsigned_int_table ("conflict_list_heads", conflict_table,
				    conflict_table[0], 1, high+1);
  muscle_insert_unsigned_int_table ("conflicting_rules", conflict_list,
			     conflict_list[0], 1, conflict_list_cnt);

  XFREE (conflict_table);
  XFREE (conflict_list);
}


static void
output_check (void)
{
  muscle_insert_base_table ("check", check,
			    check[0], 1, high + 1);
  XFREE (check);
}

/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

static void
prepare_actions (void)
{
  /* That's a poor way to make sure the sizes are properly corelated,
     in particular the signedness is not taking into account, but it's
     not useless.  */
  assert (sizeof (nvectors) >= sizeof (nstates));
  assert (sizeof (nvectors) >= sizeof (nvars));

  nvectors = state_number_as_int (nstates) + nvars;

  froms = XCALLOC (base_t *, nvectors);
  tos = XCALLOC (base_t *, nvectors);
  conflict_tos = XCALLOC (unsigned int *, nvectors);
  tally = XCALLOC (short, nvectors);
  width = XCALLOC (base_t, nvectors);

  token_actions ();
  bitsetv_free (LA);
  free (LArule);

  goto_actions ();
  XFREE (goto_map + ntokens);
  XFREE (from_state);
  XFREE (to_state);

  order = XCALLOC (vector_number_t, nvectors);
  sort_actions ();
  pack_table ();
  free (order);

  free (tally);
  free (width);

  output_base ();
  output_table ();
  output_conflicts ();

  output_check ();
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

  actions_output (out);
  merger_output (out);
  token_definitions_output (out);
  symbol_destructors_output (out);
  symbol_printers_output (out);

  muscles_m4_output (out);

  fputs ("m4_wrap([m4_divert_pop(0)])\n", out);
  fputs ("m4_divert_push(0)dnl\n", out);
  xfclose (out);

  m4_invoke (tempfile);

  /* If `debugging', keep this file alive. */
  if (!trace_flag)
    unlink (tempfile);

  free (tempfile);
}

static void
prepare (void)
{
  /* Flags. */
  MUSCLE_INSERT_INT ("locations_flag", locations_flag);
  MUSCLE_INSERT_INT ("defines_flag", defines_flag);
  MUSCLE_INSERT_INT ("error_verbose", error_verbose);
  MUSCLE_INSERT_INT ("pure", pure_parser);
  MUSCLE_INSERT_INT ("debug", debug_flag);

  /* FIXME: This is wrong: the muscles should decide whether they hold
     a copy or not, but the situation is too obscure currently.  */
  MUSCLE_INSERT_STRING ("prefix", spec_name_prefix ? spec_name_prefix : "yy");
  MUSCLE_INSERT_STRING ("output_infix", output_infix ? output_infix : "");
  MUSCLE_INSERT_STRING ("output_prefix", short_base_name);
  MUSCLE_INSERT_STRING ("output_parser_name", parser_file_name);
  MUSCLE_INSERT_STRING ("output_header_name", spec_defines_file);

  /* Symbols. */
  MUSCLE_INSERT_INT ("tokens_number", ntokens);
  MUSCLE_INSERT_INT ("nterms_number", nvars);
  MUSCLE_INSERT_INT ("undef_token_number", undeftoken->number);
  MUSCLE_INSERT_INT ("user_token_number_max", max_user_token_number);

  /* Rules. */
  MUSCLE_INSERT_INT ("rules_number", nrules);

  /* States. */
  MUSCLE_INSERT_INT ("last", high);
  MUSCLE_INSERT_INT ("final_state_number", final_state->number);
  MUSCLE_INSERT_INT ("states_number", nstates);

  /* User Code.  */
  obstack_1grow (&pre_prologue_obstack, 0);
  obstack_1grow (&post_prologue_obstack, 0);
  muscle_insert ("pre_prologue", obstack_finish (&pre_prologue_obstack));
  muscle_insert ("post_prologue", obstack_finish (&post_prologue_obstack));

  /* Find the right skeleton file.  */
  if (!skeleton)
    {
      if (glr_parser)
	skeleton = "glr.c";
      else
	skeleton = "yacc.c";
    }

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
  prepare_actions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  obstack_free (&format_obstack, NULL);
  obstack_free (&pre_prologue_obstack, NULL);
  obstack_free (&post_prologue_obstack, NULL);
}
