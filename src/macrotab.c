/* Macro table manager for Bison,
   Copyright 1984, 1989, 2000 Free Software Foundation, Inc.

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

#include <string.h>

#include "xalloc.h"
#include "system.h"
#include "hash.h"
#include "files.h"
#include "macrotab.h"

struct hash_table macro_table;

static unsigned long
mhash1 (const void *item)
{
  return_STRING_HASH_1 (((macro_entry_t *) item)->key);
}

static unsigned long
mhash2 (const void *item)
{
  return_STRING_HASH_2 (((macro_entry_t *) item)->key);
}

static int
mcmp (const void *x, const void *y)
{
  return strcmp (((macro_entry_t*) x)->key, ((macro_entry_t *) y)->key);
}

void
macro_init (void)
{
  hash_init (&macro_table, MTABSIZE, &mhash1, &mhash2, &mcmp);

  /* Version and input file.  */
  macro_insert ("version", VERSION);
  macro_insert ("filename", "a.y");

  /* Types.  */
  macro_insert ("stype", "int");
  macro_insert ("ltype", "yyltype");

  /* Tokens.  */
  macro_insert ("tokendef", "");

  /* Tables.  */
  macro_insert ("rhs", "0");
  macro_insert ("pact", "0");
  macro_insert ("prhs", "0");
  macro_insert ("stos", "0");
  macro_insert ("check", "0");
  macro_insert ("pgoto", "0");
  macro_insert ("table", "0");
  macro_insert ("tname", "0");
  macro_insert ("defact", "0");
  macro_insert ("toknum", "0");
  macro_insert ("defgoto", "0");
  macro_insert ("translate", "0");

  /* Various macros.  */
  macro_insert ("flag", "0");
  macro_insert ("last", "0");
  macro_insert ("pure", "0");
  macro_insert ("nsym", "0");
  macro_insert ("debug", "0");
  macro_insert ("final", "0");
  macro_insert ("maxtok", "0");
  macro_insert ("ntbase", "0");
  macro_insert ("verbose", "0");

  /* Variable prefix names.  */
  macro_insert ("yyparse", "yyparse");
  macro_insert ("yylex", "yylex");
  macro_insert ("yyerror", "yyerror");
  macro_insert ("yylval", "yylval");
  macro_insert ("yychar", "yychar");
  macro_insert ("yydebug", "yydebug");
  macro_insert ("yynerrs", "yynerrs");

  /* No parser macros.  */
  macro_insert ("nnts", "0");
  macro_insert ("nrules", "0");
  macro_insert ("nstates", "0");
  macro_insert ("ntokens", "0");

  /* Stack parameters.  */
  macro_insert ("maxdepth", "10000");
  macro_insert ("initdepth", "200");

  /* C++ macros.  */
  macro_insert ("name", "Parser");
}

void 
macro_insert (const char *key, const char *value)
{
  macro_entry_t *pair = XMALLOC (macro_entry_t, 1);
  pair->key = key;
  pair->value = value;
  hash_insert (&macro_table, pair);
}

const char*
macro_find (const char *key)
{
  macro_entry_t pair = { key, 0 };
  macro_entry_t *result = hash_find_item (&macro_table, &pair);
  return result ? result->value : 0;
}
