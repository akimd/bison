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

#define	TABSIZE	1009

/* Associativity values for tokens and rules.  */
typedef enum
{
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
  /* The key, name of the symbol. */
  char *tag;
  /* Its type. */
  char *type_name;

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


/* A function to apply to each symbol. */
typedef bool (*symbol_processor) PARAMS ((symbol_t *));

symbol_t *getsym PARAMS ((const char *));

void symbols_new PARAMS ((void));
void symbols_do PARAMS ((symbol_processor processor, void *processor_data));
void symbols_free PARAMS ((void));

#endif /* !SYMTAB_H_ */
