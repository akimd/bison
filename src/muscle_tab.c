/* Macro table manager for Bison,
   Copyright 2001 Free Software Foundation, Inc.

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

#include "system.h"
#include "hash.h"
#include "files.h"
#include "muscle_tab.h"
#include "getargs.h"

struct hash_table muscle_table;

static unsigned long
mhash1 (const void *item)
{
  return_STRING_HASH_1 (((const muscle_entry_t *) item)->key);
}

static unsigned long
mhash2 (const void *item)
{
  return_STRING_HASH_2 (((const muscle_entry_t *) item)->key);
}

static int
mcmp (const void *x, const void *y)
{
  return strcmp (((const muscle_entry_t*) x)->key,
		 ((const muscle_entry_t *) y)->key);
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

  /* Tables.  */
  muscle_insert ("rhs", NULL);
  muscle_insert ("pact", NULL);
  muscle_insert ("prhs", NULL);
  muscle_insert ("stos", NULL);
  muscle_insert ("check", NULL);
  muscle_insert ("pgoto", NULL);
  muscle_insert ("table", NULL);
  muscle_insert ("tname", NULL);
  muscle_insert ("defact", NULL);
  muscle_insert ("toknum", NULL);
  muscle_insert ("defgoto", NULL);
  muscle_insert ("translate", NULL);

  /* Various macros.  */
  muscle_insert ("flag", NULL);
  muscle_insert ("last", NULL);
  muscle_insert ("pure", NULL);
  muscle_insert ("nsym", NULL);
  muscle_insert ("debug", NULL);
  muscle_insert ("final", NULL);
  muscle_insert ("maxtok", NULL);
  muscle_insert ("ntbase", NULL);
  muscle_insert ("error-verbose", NULL);
  muscle_insert ("prefix", NULL);
  /* Default #line formatting.  */
  muscle_insert ("linef", "#line %d %s\n");

  /* No parser macros.  */
  muscle_insert ("nnts", NULL);
  muscle_insert ("nrules", NULL);
  muscle_insert ("nstates", NULL);
  muscle_insert ("ntokens", NULL);

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
