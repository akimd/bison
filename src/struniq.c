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
#include "quotearg.h"
#include "hash.h"
#include "struniq.h"

/*-----------------------.
| A struniq hash table.  |
`-----------------------*/

/* Initial capacity of struniq hash table.  */
#define HT_INITIAL_CAPACITY 257

static struct hash_table *struniqs_table = NULL;

/*-------------------------------------.
| Create the struniq for S if needed.  |
`-------------------------------------*/

const struniq_t
struniq_new (const char *s)
{
  /* Keep the struniqs in a printable form.  */
  struniq_t res = hash_lookup (struniqs_table, 
			       quotearg_style (escape_quoting_style, s));

  if (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (quotearg_style (escape_quoting_style, s));
      hash_insert (struniqs_table, res);
    }
  return res;
}


/*--------------------.
| Print the struniq.  |
`--------------------*/

static bool
struniq_print (const struniq_t s)
{
  fprintf (stderr, "%s\n", s);
  return true;
}


/*-----------------------.
| A struniq hash table.  |
`-----------------------*/

static bool
hash_compare_struniq_t (const struniq_t m1, const struniq_t m2)
{
  return strcmp (m1, m2) == 0;
}

static unsigned int
hash_struniq_t (const struniq_t m, unsigned int tablesize)
{
  return hash_string (m, tablesize);
}

/* A function to apply to each symbol. */
typedef bool (*struniq_processor_t) (const struniq_t);

/*----------------------------.
| Create the struniqs table.  |
`----------------------------*/

void
struniqs_new (void)
{
  struniqs_table = hash_initialize (HT_INITIAL_CAPACITY,
				    NULL,
				    (Hash_hasher) hash_struniq_t,
				    (Hash_comparator) hash_compare_struniq_t,
				    (Hash_data_freer) free);
}


/*-------------------------------------.
| Perform a task on all the struniqs.  |
`-------------------------------------*/

static void
struniqs_do (struniq_processor_t processor, void *processor_data)
{
  hash_do_for_each (struniqs_table,
		    (Hash_processor) processor,
		    processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

void
struniqs_print (void)
{
  struniqs_do (struniq_print, NULL);
}


/*--------------------.
| Free the struniqs.  |
`--------------------*/

void
struniqs_free (void)
{
  hash_free (struniqs_table);
}
