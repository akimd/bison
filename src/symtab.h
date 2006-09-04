/* Definitions for symtab.c and callers, part of Bison.

   Copyright (C) 1984, 1989, 1992, 2000, 2001, 2002, 2004, 2005, 2006
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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/**
 * \file symtab.h
 * \brief Manipulating ::symbol.
 */

#ifndef SYMTAB_H_
# define SYMTAB_H_

# include "assoc.h"
# include "location.h"
# include "uniqstr.h"

/*----------.
| Symbols.  |
`----------*/

/** Symbol classes.  */
typedef enum
{
  unknown_sym,          /**< Undefined.  */
  token_sym,		/**< Terminal. */
  nterm_sym		/**< Non-terminal. */
} symbol_class;


/** Internal token numbers. */
typedef int symbol_number;
#define SYMBOL_NUMBER_MAXIMUM INT_MAX


typedef struct symbol symbol;

/* When extending this structure, be sure to complete
   symbol_check_alias_consistency.  */
struct symbol
{
  /** The key, name of the symbol.  */
  uniqstr tag;
  /** The location of its first occurrence.  */
  location location;

  /** Its \c \%type.  */
  uniqstr type_name;
  /** Its \c \%type's location.  */
  location type_location;

  /** Any \c \%destructor declared specifically for this symbol.

     Access this field only through <tt>symbol</tt>'s interface functions.  For
     example, if <tt>symbol::destructor = NULL</tt>, the default
     \c \%destructor or a per-type \c \%destructor might be appropriate, and
     \c symbol_destructor_get will compute the correct one.  */
  const char *destructor;

  /** The location of \c symbol::destructor.

     Access this field only through <tt>symbol</tt>'s interface functions.
     \sa symbol::destructor  */
  location destructor_location;

  /** Any \c \%printer declared specifically for this symbol.

     Access this field only through <tt>symbol</tt>'s interface functions.
     \sa symbol::destructor  */
  const char *printer;

  /** The location of \c symbol::printer.

     Access this field only through <tt>symbol</tt>'s interface functions.
     \sa symbol::destructor  */
  location printer_location;

  symbol_number number;
  location prec_location;
  int prec;
  assoc assoc;
  int user_token_number;

  /* Points to the other in the identifier-symbol pair for an alias.
     Special value USER_NUMBER_ALIAS in the identifier half of the
     identifier-symbol pair for an alias.  */
  symbol *alias;
  symbol_class class;
  bool declared;
};

/** Undefined user number.  */
#define USER_NUMBER_UNDEFINED -1

/* `symbol->user_token_number == USER_NUMBER_ALIAS' means this symbol
   *has* (not is) a string literal alias.  For instance, `%token foo
   "foo"' has `"foo"' numbered regularly, and `foo' numbered as
   USER_NUMBER_ALIAS.  */
#define USER_NUMBER_ALIAS -9991

/* Undefined internal token number.  */
#define NUMBER_UNDEFINED (-1)

/** Print a symbol (for debugging). */
void symbol_print (symbol *s, FILE *f);

/** Fetch (or create) the symbol associated to KEY.  */
symbol *symbol_from_uniqstr (const uniqstr key, location loc);

/** Fetch (or create) the symbol associated to KEY.  */
symbol *symbol_get (const char *key, location loc);

/** Generate a dummy nonterminal.

   Its name cannot conflict with the user's names.  */
symbol *dummy_symbol_get (location loc);

/** Is this a dummy nonterminal?  */
bool symbol_is_dummy (const symbol *sym);

/** Declare the new symbol \c sym.  Make it an alias of \c symval.  */
void symbol_make_alias (symbol *sym, symbol *symval, location loc);

/** Set the \c type_name associated with \c sym.

    Do nothing if passed 0 as \c type_name.  */
void symbol_type_set (symbol *sym, uniqstr type_name, location loc);

/** Set the \c destructor associated with \c sym.  */
void symbol_destructor_set (symbol *sym, const char *destructor, location loc);

/** Get the computed \c \%destructor for \c sym, or \c NULL if none.   */
const char *symbol_destructor_get (symbol *sym);

/** Get the grammar location of the computed \c \%destructor for \c sym.
 
  \pre <tt>symbol_destructor_get (sym) != NULL</tt> */
location symbol_destructor_location_get (symbol *sym);

/** Set the \c printer associated with \c sym.  */
void symbol_printer_set (symbol *sym, const char *printer, location loc);

/** Get the computed \c \%printer for \c sym, or \c NULL if none.  */
const char *symbol_printer_get (symbol *sym);

/** Get the grammar location of the computed \c \%printer for \c sym.

  \pre <tt>symbol_printer_get (sym) != NULL</tt> */
location symbol_printer_location_get (symbol *sym);

/* Set the \c precedence associated with \c sym.

   Ensure that \a symbol is a terminal.
   Do nothing if invoked with \c undef_assoc as \c assoc.  */
void symbol_precedence_set (symbol *sym, int prec, assoc a, location loc);

/** Set the \c class associated with \c sym.  */
void symbol_class_set (symbol *sym, symbol_class class, location loc,
		       bool declaring);

/** Set the \c user_token_number associated with \c sym.  */
void symbol_user_token_number_set (symbol *sym, int user_number, location loc);


/** The error token. */
extern symbol *errtoken;
/** The token for unknown tokens.  */
extern symbol *undeftoken;
/** The end of input token.  */
extern symbol *endtoken;
/** The genuine start symbol.

   $accept: start-symbol $end */
extern symbol *accept;

/** The user start symbol. */
extern symbol *startsymbol;
/** The location of the \c \%start declaration.  */
extern location startsymbol_location;


/*-----------------.
| Semantic types.  |
`-----------------*/

/** A semantic type and its associated \c \%destructor and \c \%printer.
   
   Access the fields of this struct only through the interface functions in
   this file.  \sa symbol::destructor  */
typedef struct semantic_type {
  /** The key, name of the semantic type.  */
  uniqstr tag;

  /** Any \c %destructor declared for this semantic type.  */
  const char *destructor;
  /** The location of \c semantic_type::destructor.  */
  location destructor_location;

  /** Any \c %printer declared for this semantic type.  */
  const char *printer;
  /** The location of \c semantic_type::printer.  */
  location printer_location;
} semantic_type;

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_from_uniqstr (const uniqstr key);

/** Fetch (or create) the semantic type associated to KEY.  */
semantic_type *semantic_type_get (const char *key);

/** Set the \c destructor associated with \c type.  */
void semantic_type_destructor_set (semantic_type *type, const char *destructor,
                                   location loc);

/** Set the \c printer associated with \c type.  */
void semantic_type_printer_set (semantic_type *type, const char *printer,
                                location loc);

/*----------------------------------.
| Symbol and semantic type tables.  |
`----------------------------------*/

/** Create the symbol and semantic type tables.  */
void symbols_new (void);

/** Free all the memory allocated for symbols and semantic types.  */
void symbols_free (void);

/** Check that all the symbols are defined.

    Report any undefined symbols and consider them nonterminals.  */
void symbols_check_defined (void);

/** Sanity checks and #token_translations construction.

   Perform various sanity checks, assign symbol numbers, and set up
   #token_translations.  */
void symbols_pack (void);


/*-----------------------------------.
| Default %destructor and %printer.  |
`-----------------------------------*/

/** Set the default \c \%destructor.  */
void default_destructor_set (const char *destructor, location loc);

/** Set the default \c \%printer.  */
void default_printer_set (const char *printer, location loc);

#endif /* !SYMTAB_H_ */
