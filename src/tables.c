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
#include "getargs.h"
#include "files.h"
#include "gram.h"
#include "complain.h"
#include "lalr.h"
#include "reader.h"
#include "symtab.h"
#include "conflicts.h"
#include "tables.h"

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

int nvectors;


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

base_t *base = NULL;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MIN, later mapped to BASE_NINF to
   keep parser tables small.  */
base_t base_ninf = 0;
static base_t *pos = NULL;

static unsigned int *conflrow = NULL;
unsigned int *conflict_table = NULL;
unsigned int *conflict_list = NULL;
int conflict_list_cnt;
static int conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
static size_t table_size = 32768;
base_t *table = NULL;
base_t *check = NULL;
/* The value used in TABLE to denote explicit parse errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_MIN,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_t table_ninf = 0;
static int lowzero;
int high;

state_number_t *yydefgoto;
rule_number_t *yydefact;

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

  if (trace_flag & trace_resource)
    fprintf (stderr, "growing table and check from: %d to %d\n",
	     old_size, table_size);

  table = XREALLOC (table, base_t, table_size);
  check = XREALLOC (check, base_t, table_size);
  conflict_table = XREALLOC (conflict_table, unsigned int, table_size);

  for (/* Nothing. */; old_size < table_size; ++old_size)
    {
      table[old_size] = 0;
      check[old_size] = -1;
    }
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
  reductions_t *reds = state->reductions;

  if (! glr_parser)
    return;

  for (j = 0; j < ntokens; j += 1)
    if (conflrow[j])
      {
	conflrow[j] = conflict_list_cnt;

	/* Find all reductions for token J, and record all that do not
	   match ACTROW[J].  */
	for (i = 0; i < reds->num; i += 1)
	  if (bitset_test (reds->lookaheads[i], j)
	      && (actrow[j]
		  != rule_number_as_item_number (reds->rules[i]->number)))
	    {
	      assert (conflict_list_free > 0);
	      conflict_list[conflict_list_cnt] = reds->rules[i]->number + 1;
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

static rule_t *
action_row (state_t *state)
{
  int i;
  rule_t *default_rule = NULL;
  reductions_t *redp = state->reductions;
  transitions_t *transitions = state->transitions;
  errs_t *errp = state->errs;
  /* Set to nonzero to inhibit having any default reduction.  */
  int nodefault = 0;
  int conflicted = 0;

  for (i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  if (redp->lookaheads)
    {
      int j;
      bitset_iterator biter;
      /* loop over all the rules available here which require
	 lookahead (in reverse order to give precedence to the first
	 rule) */
      for (i = redp->num - 1; i >= 0; --i)
	/* and find each token which the rule finds acceptable
	   to come next */
	BITSET_FOR_EACH (biter, redp->lookaheads[i], j, 0)
	{
	  /* and record this rule as the rule to use if that
	     token follows.  */
	  if (actrow[j] != 0)
	    conflicted = conflrow[j] = 1;
	  actrow[j] = rule_number_as_item_number (redp->rules[i]->number);
	}
    }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  FOR_EACH_SHIFT (transitions, i)
    {
      symbol_number_t symbol = TRANSITION_SYMBOL (transitions, i);
      state_t *shift_state = transitions->states[i];

      if (actrow[symbol] != 0)
	conflicted = conflrow[symbol] = 1;
      actrow[symbol] = state_number_as_int (shift_state->number);

      /* Do not use any default reduction if there is a shift for
	 error */
      if (symbol == errtoken->number)
	nodefault = 1;
    }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_MIN as the action.  */
  for (i = 0; i < errp->num; i++)
    {
      symbol_t *symbol = errp->symbols[i];
      actrow[symbol->number] = ACTION_MIN;
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
	  for (i = 0; i < redp->num; i++)
	    {
	      int count = 0;
	      rule_t *rule = redp->rules[i];
	      symbol_number_t j;

	      for (j = 0; j < ntokens; j++)
		if (actrow[j] == rule_number_as_item_number (rule->number))
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
		if (actrow[j] == rule_number_as_item_number (default_rule->number)
		    && ! (glr_parser && conflrow[j]))
		  actrow[j] = 0;
	    }
	}
    }

  /* If have no default rule, the default is an error.
     So replace any action which says "error" with "use default".  */

  if (!default_rule)
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
  symbol_number_t j;
  rule_number_t r;

  int nconflict = glr_parser ? conflicts_total_count () : 0;

  yydefact = XCALLOC (rule_number_t, nstates);

  actrow = XCALLOC (action_t, ntokens);
  conflrow = XCALLOC (unsigned int, ntokens);

  conflict_list = XCALLOC (unsigned int, 1 + 2 * nconflict);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  if (!glr_parser)
    for (r = 0; r < nrules; ++r)
      rules[r].useful = FALSE;

  for (i = 0; i < nstates; ++i)
    {
      rule_t *default_rule = action_row (states[i]);
      yydefact[i] = default_rule ? default_rule->number + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
	 really reduced, and which are not because of SR or RR
	 conflicts.  */
      if (!glr_parser)
	{
	  for (j = 0; j < ntokens; ++j)
	    if (actrow[j] < 0 && actrow[j] != ACTION_MIN)
	      rules[item_number_as_rule_number (actrow[j])].useful = TRUE;
	  if (yydefact[i])
	    rules[yydefact[i] - 1].useful = TRUE;
	}
    }

  free (actrow);
  free (conflrow);
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
  short *state_count = XCALLOC (short, nstates);
  yydefgoto = XMALLOC (state_number_t, nvars);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  for (i = ntokens; i < nsyms; ++i)
    {
      state_number_t default_state = default_goto (i, state_count);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  free (state_count);
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
	  if (loc >= (int) table_size)
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
      res = tab[i];

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

  free (pos);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

void
tables_generate (void)
{
  int i;

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

  for (i = 0; i < nvectors; i++)
    {
      XFREE (froms[i]);
      XFREE (tos[i]);
      XFREE (conflict_tos[i]);
    }

  free (froms);
  free (tos);
  free (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

void
tables_free (void)
{
  free (base);
  free (conflict_table);
  free (conflict_list);
  free (table);
  free (check);
  free (yydefgoto);
  free (yydefact);
}
