/* Macro table manager for Bison,
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.

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


/* An obstack used to create some entries.  */
struct obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

struct hash_table *muscle_table = NULL;

static bool
hash_compare_muscles (void const *x, void const *y)
{
  const muscle_entry_t *m1 = x;
  const muscle_entry_t *m2 = y;
  return strcmp (m1->key, m2->key) ? FALSE : TRUE;
}

static unsigned int
hash_muscle (const void *x, unsigned int tablesize)
{
  const muscle_entry_t *m = x;
  return hash_string (m->key, tablesize);
}

/*-----------------------------------------------------------------.
| Create the MUSCLE_TABLE, and initialize it with default values.  |
| Also set up the MUSCLE_OBSTACK.                                  |
`-----------------------------------------------------------------*/

void
muscle_init (void)
{
  muscle_table = hash_initialize (HT_INITIAL_CAPACITY, NULL, hash_muscle,
				  hash_compare_muscles, free);

  /* Version and input file.  */
  muscle_insert ("version", VERSION);
  muscle_insert ("filename", infile);

  /* FIXME: there should probably be no default here, only in the
     skeletons.  */

  /* Types.  */
  muscle_insert ("ltype", "yyltype");

  /* Default #line formatting.  */
  muscle_insert ("linef", "#line %d %s\n");

  /* Stack parameters.  */
  muscle_insert ("maxdepth", "10000");
  muscle_insert ("initdepth", "200");

  /* C++ macros.  */
  muscle_insert ("name", "Parser");

  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);
}


/*------------------------------------------------------------.
| Free all the memory consumed by the muscle machinery only.  |
`------------------------------------------------------------*/

void
muscle_free (void)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, NULL);
}



void
muscle_insert (const char *key, const char *value)
{
  muscle_entry_t pair;
  muscle_entry_t *entry = NULL;

  pair.key = key;
  entry = hash_lookup (muscle_table, &pair);

  if (!entry)
    {
      /* First insertion in the hash. */
      entry = XMALLOC (muscle_entry_t, 1);
      entry->key = key;
      hash_insert (muscle_table, entry);
    }
  entry->value = value;
}

const char*
muscle_find (const char *key)
{
  muscle_entry_t pair;
  muscle_entry_t *result = NULL;

  pair.key = key;
  result = hash_lookup (muscle_table, &pair);
  return result ? result->value : NULL;
}


/* Output the definition of all the current muscles into a list of
   m4_defines.  */

static int
muscle_m4_output (muscle_entry_t *entry, FILE *out)
{
  fprintf (out, "m4_define([b4_%s],\n", entry->key);
  fprintf (out, "          [[%s]])\n\n\n", entry->value);
  return 1;
}


/* Output the definition of all the current muscles into a list of
   m4_defines.  */

void
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table,
		    (Hash_processor) muscle_m4_output,
		    out);
}
