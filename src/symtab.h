/* Definitions for symtab.c and callers, part of bison,
   Copyright 1984, 1989, 1992, 2000 Free Software Foundation, Inc.

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
# include "gram.h"

#define	TABSIZE	1009

/*  symbol classes  */

typedef enum
{
  unknown_sym,
  token_sym,		/* terminal symbol */
  nterm_sym		/* non-terminal */
} symbol_class;

#define SALIAS	-9991		/* for symbol generated with an alias */

typedef struct bucket
{
  /* Needed for the hash table. */
  struct bucket *link;
  struct bucket *next;

  /* The key, name of the symbol. */
  char *tag;
  /* Its type. */
  char *type_name;
  short value;
  short prec;
  associativity assoc;
  short user_token_number;
  /* Points to the other in the identifier-symbol pair for an
     alias. Special value SALIAS in the identifier half of the
     identifier-symbol pair for an alias.  */
  struct bucket *alias;
  symbol_class class;
}
bucket;


extern bucket *firstsymbol;

bucket *getsym PARAMS ((const char *));

void tabinit PARAMS ((void));
void free_symtab PARAMS ((void));

#endif /* !SYMTAB_H_ */
