/* Data definitions for internal representation of bison's input,
   Copyright 1984, 1986, 1989, 1992, 2001  Free Software Foundation, Inc.

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

/* representation of the grammar rules:

   ntokens is the number of tokens, and nvars is the number of
   variables (nonterminals).  nsyms is the total number, ntokens +
   nvars.

   (the true number of token values assigned is ntokens reduced by one
   for each alias declaration)

   Each symbol (either token or variable) receives a symbol number.
   Numbers 0 to ntokens-1 are for tokens, and ntokens to nsyms-1 are
   for variables.  Symbol number zero is the end-of-input token.  This
   token is counted in ntokens.

   The rules receive rule numbers 1 to nrules in the order they are
   written.  Actions and guards are accessed via the rule number.

   The rules themselves are described by several arrays: amongst which
   RITEM, and RULE_TABLE.

   RULE_TABLE is an array of struct rule_s, which members are:

   RULE_TABLE[R].lhs -- the symbol number of the left hand side of
   rule R.  If -1, the rule has been thrown out by reduce.c and should
   be ignored.

   RULE_TABLE[R].rhs -- the index in RITEM of the beginning of the
   portion for rule R.

   RULE_TABLE[R].prec -- the precedence level of R.

   RULE_TABLE[R].precsym -- the symbol-number of the symbol in %prec
   for R (if any).

   RULE_TABLE[R].assoc -- the associativity of R.

   RULE_TABLE[R].line -- the line where R was defined.

   RULE_TABLE[R].useful -- TRUE iff the rule is used.

   The right hand side is stored as symbol numbers in a portion of
   RITEM.

   The length of the portion is one greater than the number of symbols
   in the rule's right hand side.  The last element in the portion
   contains minus R, which identifies it as the end of a portion and
   says which rule it is for.

   The portions of RITEM come in order of increasing rule number and
   are followed by an element which is zero to mark the end.  nitems
   is the total length of ritem, not counting the final zero.  Each
   element of ritem is called an "item" and its index in ritem is an
   item number.

   Item numbers are used in the finite state machine to represent
   places that parsing can get to.

   SPREC records the precedence level of each symbol.

   Precedence levels are assigned in increasing order starting with 1
   so that numerically higher precedence values mean tighter binding
   as they ought to.  Zero as a symbol or rule's precedence means none
   is assigned.

   Associativities are recorded similarly in rassoc and sassoc.  */


#define	ISTOKEN(s)	((s) < ntokens)
#define	ISVAR(s)	((s) >= ntokens)

extern int nitems;
extern int nrules;
extern int nsyms;
extern int ntokens;
extern int nvars;

extern short *ritem;

extern short *sprec;
extern short *sassoc;

extern int start_symbol;

/* associativity values in elements of rassoc, sassoc.  */
typedef enum
{
  right_assoc,
  left_assoc,
  non_assoc
} associativity;


typedef struct rule_s
{
  short lhs;
  short rhs;
  short prec;
  short precsym;
  short assoc;
  short line;
  bool useful;
} rule_t;

extern struct rule_s *rule_table;

/* token translation table: indexed by a token number as returned by
   the user's yylex routine, it yields the internal token number used
   by the parser and throughout bison.  */

extern short *token_translations;
extern int max_user_token_number;

/* SEMANTIC_PARSER is nonzero if the input file says to use the hairy
   parser that provides for semantic error recovery.  If it is zero,
   the yacc-compatible simplified parser is used.  */

extern int semantic_parser;

/* PURE_PARSER is nonzero if should generate a parser that is all pure
   and reentrant.  */

extern int pure_parser;

/* ERROR_TOKEN_NUMBER is the token number of the error token.  */

extern int error_token_number;


/* Dump RITEM for traces. */
void ritem_print PARAMS ((FILE *out));

/* Return the size of the longest rule RHS.  */
size_t ritem_longest_rhs PARAMS ((void));

#endif /* !GRAM_H_ */
