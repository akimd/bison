/* Keeping a unique copy of strings.
   Copyright (C) 2002 Free Software Foundation, Inc.

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

#include <error.h>
#include <hash.h>
#include <quotearg.h>

#include "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

static struct hash_table *uniqstrs_table = NULL;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (const char *s)
{
  uniqstr res = hash_lookup (uniqstrs_table, s);
  if (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (s);
      hash_insert (uniqstrs_table, res);
    }
  return res;
}


/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

void
uniqstr_assert (const char *s)
{
  if (!hash_lookup (uniqstrs_table, s))
    {
      error (0, 0, "not a uniqstr: %s", quotearg (s));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

static bool
uniqstr_print (const uniqstr s)
{
  fprintf (stderr, "%s\n", s);
  return true;
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

static bool
hash_compare_uniqstr (const uniqstr m1, const uniqstr m2)
{
  return strcmp (m1, m2) == 0;
}

static unsigned int
hash_uniqstr (const uniqstr m, unsigned int tablesize)
{
  return hash_string (m, tablesize);
}

/* A function to apply to each symbol. */
typedef bool (*uniqstr_processor) (const uniqstr);

/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

void
uniqstrs_new (void)
{
  uniqstrs_table = hash_initialize (HT_INITIAL_CAPACITY,
				    NULL,
				    (Hash_hasher) hash_uniqstr,
				    (Hash_comparator) hash_compare_uniqstr,
				    (Hash_data_freer) free);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

static void
uniqstrs_do (uniqstr_processor processor, void *processor_data)
{
  hash_do_for_each (uniqstrs_table,
		    (Hash_processor) processor,
		    processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

void
uniqstrs_print (void)
{
  uniqstrs_do (uniqstr_print, NULL);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

void
uniqstrs_free (void)
{
  hash_free (uniqstrs_table);
}
