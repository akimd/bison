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
#include "muscle_tab.h"

struct hash_table muscle_table;

static unsigned long
mhash1 (const void *item)
{
  return_STRING_HASH_1 (((muscle_entry_t *) item)->key);
}

static unsigned long
mhash2 (const void *item)
{
  return_STRING_HASH_2 (((muscle_entry_t *) item)->key);
}

static int
mcmp (const void *x, const void *y)
{
  return strcmp (((muscle_entry_t*) x)->key, ((muscle_entry_t *) y)->key);
}

void
muscle_init (void)
{
  hash_init (&muscle_table, MTABSIZE, &mhash1, &mhash2, &mcmp);

  /* Version and input file.  */
  muscle_insert ("version", VERSION);
  muscle_insert ("filename", infile);

  /* Types.  */
  muscle_insert ("stype", "int");
  muscle_insert ("ltype", "yyltype");

  /* Tokens.  */
  muscle_insert ("tokendef", "");

  /* Tables.  */
  muscle_insert ("rhs", "0");
  muscle_insert ("pact", "0");
  muscle_insert ("prhs", "0");
  muscle_insert ("stos", "0");
  muscle_insert ("check", "0");
  muscle_insert ("pgoto", "0");
  muscle_insert ("table", "0");
  muscle_insert ("tname", "0");
  muscle_insert ("defact", "0");
  muscle_insert ("toknum", "0");
  muscle_insert ("defgoto", "0");
  muscle_insert ("translate", "0");

  /* Various macros.  */
  muscle_insert ("flag", "0");
  muscle_insert ("last", "0");
  muscle_insert ("pure", "0");
  muscle_insert ("nsym", "0");
  muscle_insert ("debug", "0");
  muscle_insert ("final", "0");
  muscle_insert ("maxtok", "0");
  muscle_insert ("ntbase", "0");
  muscle_insert ("verbose", "0");
  muscle_insert ("prefix", "yy");

  /* No parser macros.  */
  muscle_insert ("nnts", "0");
  muscle_insert ("nrules", "0");
  muscle_insert ("nstates", "0");
  muscle_insert ("ntokens", "0");

  /* Stack parameters.  */
  muscle_insert ("maxdepth", "10000");
  muscle_insert ("initdepth", "200");

  /* C++ macros.  */
  muscle_insert ("name", "Parser");
}

void 
muscle_insert (const char *key, const char *value)
{
  muscle_entry_t *pair = XMALLOC (muscle_entry_t, 1);
  pair->key = key;
  pair->value = value;
  hash_insert (&muscle_table, pair);
}

const char*
muscle_find (const char *key)
{
  muscle_entry_t pair = { key, 0 };
  muscle_entry_t *result = hash_find_item (&muscle_table, &pair);
  return result ? result->value : 0;
}
