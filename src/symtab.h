/* Definitions for symtab.c and callers, part of bison,
   Copyright (C) 1984, 1989, 1992, 2000, 2001, 2002
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

#ifndef SYMTAB_H_
# define SYMTAB_H_

# include "location.h"

/*----------.
| Symbols.  |
`----------*/

/* Associativity values for tokens and rules.  */
typedef enum
{
  undef_assoc,
  right_assoc,
  left_assoc,
  non_assoc
} associativity;


/* Symbol classes.  */
typedef enum
{
  unknown_sym,
  token_sym,		/* terminal symbol */
  nterm_sym		/* non-terminal */
} symbol_class;


/* Internal token numbers. */
typedef short symbol_number_t;


typedef struct symbol_s symbol_t;
struct symbol_s
{
  /* The key, name of the symbol.  */
  char *tag;

  /* Its %type and associated destructor.  */
  char *type_name;
  char *destructor;

  /* The location of its first occurence.  */
  location_t location;

  symbol_number_t number;
  short prec;
  associativity assoc;
  int user_token_number;

  /* Points to the other in the identifier-symbol pair for an alias.
     Special value USER_NUMBER_ALIAS in the identifier half of the
     identifier-symbol pair for an alias.  */
  symbol_t *alias;
  symbol_class class;
};

/* Undefined user number.  */
#define USER_NUMBER_UNDEFINED -1

/* `symbol->user_token_number == USER_NUMBER_ALIAS' means this symbol
   *has* (not is) a string literal alias.  For instance, `%token foo
   "foo"' has `"foo"' numbered regularly, and `foo' numbered as
   USER_NUMBER_ALIAS.  */
#define USER_NUMBER_ALIAS -9991

/* Undefined internal token number.  */
#define NUMBER_UNDEFINED ((symbol_number_t) -1)


/* Return the tag of this SYMBOL in a printable form.  Warning: uses
   the QUOTEARG slot 0.  */
const char *symbol_tag_get PARAMS ((symbol_t *symbol));

/* Return the tag of this SYMBOL in a printable form.  Use the
   QUOTEARG slot number N.  */
const char *symbol_tag_get_n PARAMS ((symbol_t *symbol, int n));

/* Print the tag of this SYMBOL.  */
void symbol_tag_print PARAMS ((symbol_t *symbol, FILE *out));

/* Fetch (or create) the symbol associated to KEY.  */
symbol_t *getsym PARAMS ((const char *key, location_t location));

/* Declare the new SYMBOL.  Make it an alias of SYMVAL.  */
void symbol_make_alias PARAMS ((symbol_t *symbol, symbol_t *symval));

/* Set the TYPE_NAME associated to SYMBOL. Does nothing if passed 0 as
   TYPE_NAME.  */
void symbol_type_set PARAMS ((symbol_t *symbol, location_t location,
			      char *type_name));

/* Set the DESTRUCTOR associated to SYMBOL.  */
void symbol_destructor_set PARAMS ((symbol_t *symbol, location_t location,
				    char *destructor));

/* Set the PRECEDENCE associated to SYMBOL.  Ensures that SYMBOL is a
   terminal.  Does nothing if invoked with UNDEF_ASSOC as ASSOC.  */
void symbol_precedence_set PARAMS ((symbol_t *symbol, location_t location,
				    int prec, associativity assoc));

/* Set the CLASS associated to SYMBOL.  */
void symbol_class_set PARAMS ((symbol_t *symbol, symbol_class class));

/* Set the USER_TOKEN_NUMBER associated to SYMBOL.  */
void symbol_user_token_number_set PARAMS ((symbol_t *symbol, int user_number));


/* Distinguished symbols.  AXIOM is the real start symbol, that used
   by the automaton.  STARTSYMBOL is the one specified by the user.
   */
extern symbol_t *errtoken;
extern symbol_t *undeftoken;
extern symbol_t *eoftoken;
extern symbol_t *axiom;
extern symbol_t *startsymbol;
extern location_t startsymbol_location;


/*---------------.
| Symbol table.  |
`---------------*/


/* Create the symbol table.  */
void symbols_new PARAMS ((void));

/* A function to apply to each symbol. */
typedef bool (*symbol_processor) PARAMS ((symbol_t *));

/* Apply PROCESSOR to all the symbols.  PROCESSOR must return TRUE: on
   FALSE, the processing stops.  */
void symbols_do PARAMS ((symbol_processor processor, void *processor_data));

/* Free all the memory allocated for symbols.  */
void symbols_free PARAMS ((void));

/* Check that all the symbols are defined.  Report any undefined
   symbols and consider them nonterminals.  */
void symbols_check_defined PARAMS ((void));

/* Perform various sanity checks, assign symbol numbers, and set up
   TOKEN_TRANSLATIONS.  */
void symbols_pack PARAMS ((void));

#endif /* !SYMTAB_H_ */
