/* Data definitions for internal representation of bison's input,
   Copyright (C) 1984, 1986, 1989, 1992, 2001, 2002
   Free Software Foundation, Inc.

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

#ifndef GRAM_H_
# define GRAM_H_

/* Representation of the grammar rules:

   NTOKENS is the number of tokens, and NVARS is the number of
   variables (nonterminals).  NSYMS is the total number, ntokens +
   nvars.

   Each symbol (either token or variable) receives a symbol number.
   Numbers 0 to NTOKENS - 1 are for tokens, and NTOKENS to NSYMS - 1
   are for variables.  Symbol number zero is the end-of-input token.
   This token is counted in ntokens.  The true number of token values
   assigned is NTOKENS reduced by one for each alias declaration.

   The rules receive rule numbers 1 to NRULES in the order they are
   written.  More precisely Bison augments the grammar with the
   initial rule, `$accept: START-SYMBOL $end', which is numbered 1,
   all the user rules are 2, 3 etc.  Each time a rule number is
   presented to the user, we subtract 1, so *displayed* rule numbers
   are 0, 1, 2...

   Internally, we cannot use the number 0 for a rule because for
   instance RITEM stores both symbol (the RHS) and rule numbers: the
   symbols are shorts >= 0, and rule number are stored negative.
   Therefore 0 cannot be used, since it would be both the rule number
   0, and the token $end).

   Actions are accessed via the rule number.

   The rules themselves are described by several arrays: amongst which
   RITEM, and RULES.

   RULES is an array of struct rule_s, which members are:

   RULES[R].lhs -- the symbol of the left hand side of rule R.

   RULES[R].rhs -- the index in RITEM of the beginning of the portion
   for rule R.

   RULES[R].prec -- the symbol providing the precedence level of R.

   RULES[R].precsym -- the symbol attached (via %prec) to give its
   precedence to R.  Of course, if set, it is equal to `prec', but we
   need to distinguish one from the other when reducing: a symbol used
   in a %prec is not useless.

   RULES[R].assoc -- the associativity of R.

   RULES[R].dprec -- the dynamic precedence level of R (for GLR
   parsing).

   RULES[R].merger -- index of merging function for R (for GLR
   parsing).

   RULES[R].line -- the line where R was defined.

   RULES[R].useful -- TRUE iff the rule is used (i.e., FALSE if thrown
   away by reduce).

   The right hand side is stored as symbol numbers in a portion of
   RITEM.

   The length of the portion is one greater than the number of symbols
   in the rule's right hand side.  The last element in the portion
   contains minus R, which identifies it as the end of a portion and
   says which rule it is for.

   The portions of RITEM come in order of increasing rule number.
   NRITEMS is the total length of RITEM.  Each element of RITEM is
   called an "item" and its index in RITEM is an item number.

   Item numbers are used in the finite state machine to represent
   places that parsing can get to.

   SYMBOLS[I]->prec records the precedence level of each symbol.

   Precedence levels are assigned in increasing order starting with 1
   so that numerically higher precedence values mean tighter binding
   as they ought to.  Zero as a symbol or rule's precedence means none
   is assigned.

   Associativities are recorded similarly in SYMBOLS[I]->assoc.  */

# include "location.h"
# include "symtab.h"

# define ISTOKEN(s)	((s) < ntokens)
# define ISVAR(s)	((s) >= ntokens)

extern int nsyms;
extern int ntokens;
extern int nvars;

typedef int item_number_t;
# define ITEM_NUMBER_MAX ((item_number_t) INT_MAX)
# define ITEM_NUMBER_MIN ((item_number_t) INT_MIN)
extern item_number_t *ritem;
extern unsigned int nritems;

/* There is weird relationship between OT1H item_number_t and OTOH
   symbol_number_t and rule_number_t: we store the latter in
   item_number_t.  symbol_number_t are stored as are, while
   the negation of (rule_number_t + 1) are stored.

   Therefore, an symbol_number_t must be a valid item_number_t, and we
   sometimes have to perform the converse transformation.  */
# define symbol_number_as_item_number(Tok) ((item_number_t) (Tok))
# define item_number_as_symbol_number(Ite) ((symbol_number_t) (Ite))

extern symbol_number_t start_symbol;

/* Rules numbers. */
typedef short rule_number_t;
# define RULE_NUMBER_MAX ((rule_number_t) SHRT_MAX)
extern rule_number_t nrules;
# define int_of_rule_number(RNum) ((int) (RNum))
# define rule_number_as_item_number(RNum) ((item_number_t) (- RNum - 1))
# define item_number_as_rule_number(INum) ((rule_number_t) (- INum - 1))


/*--------.
| Rules.  |
`--------*/

typedef struct rule_s
{
  /* The number of the rule in the source.  It is usually the index in
     RULES too, except if there are useless rules.  */
  rule_number_t user_number;

  /* The index in RULES.  Usually the rule number in the source,
     except if some rules are useless.  */
  rule_number_t number;

  symbol_t *lhs;
  item_number_t *rhs;

  /* This symbol provides both the associativity, and the precedence. */
  symbol_t *prec;

  short dprec;
  short merger;

  /* This symbol was attached to the rule via %prec. */
  symbol_t *precsym;

  location_t location;
  bool useful;

  const char *action;
  location_t action_location;
} rule_t;

extern struct rule_s *rules;

/* A function that selects a rule.  */
typedef bool (*rule_filter_t) PARAMS ((rule_t *r));

/* Return true IFF the rule has a `number' smaller than NRULES.  */
bool rule_useful_p PARAMS ((rule_t *r));

/* Return true IFF the rule has a `number' higher than NRULES.  */
bool rule_useless_p PARAMS ((rule_t *r));

/* Return true IFF the rule is not flagged as useful *and* is useful.
   In other words, it was discarded because of conflicts.  */
bool rule_never_reduced_p PARAMS ((rule_t *r));

/* Print this RULE's number and lhs on OUT.  If a PREVIOUS_LHS was
   already displayed (by a previous call for another rule), avoid
   useless repetitions.  */
void rule_lhs_print PARAMS ((rule_t *rule, symbol_t *previous_lhs, FILE *out));

/* Return the length of the RHS.  */
int rule_rhs_length PARAMS ((rule_t *rule));

/* Print this RULE's RHS on OUT.  */
void rule_rhs_print PARAMS ((rule_t *rule, FILE *out));

/* Print this RULE on OUT.  */
void rule_print PARAMS ((rule_t *rule, FILE *out));




/* Table of the symbols, indexed by the symbol number. */
extern symbol_t **symbols;

/* TOKEN_TRANSLATION -- a table indexed by a token number as returned
   by the user's yylex routine, it yields the internal token number
   used by the parser and throughout bison.  */
extern symbol_number_t *token_translations;
extern int max_user_token_number;



/* GLR_PARSER is nonzero if the input file says to use the GLR
   (Generalized LR) parser, and to output some additional
   information used by the GLR algorithm. */

extern int glr_parser;

/* PURE_PARSER is nonzero if should generate a parser that is all pure
   and reentrant.  */

extern int pure_parser;

/* Dump RITEM for traces. */
void ritem_print PARAMS ((FILE *out));

/* Return the size of the longest rule RHS.  */
size_t ritem_longest_rhs PARAMS ((void));

/* Print the grammar's rules numbers from BEGIN (inclusive) to END
   (exclusive) on OUT under TITLE.  */
void grammar_rules_partial_print PARAMS ((FILE *out, const char *title,
					  rule_filter_t filter));

/* Print the grammar's rules on OUT.  */
void grammar_rules_print PARAMS ((FILE *out));

/* Dump the grammar. */
void grammar_dump PARAMS ((FILE *out, const char *title));

/* Report on STDERR the rules that are not flagged USEFUL, using the
   MESSAGE (which can be `useless rule' when invoked after grammar
   reduction, or `never reduced' after conflicts were taken into
   account).  */
void grammar_rules_never_reduced_report PARAMS ((const char *message));

/* Free the packed grammar. */
void grammar_free PARAMS ((void));

#endif /* !GRAM_H_ */
