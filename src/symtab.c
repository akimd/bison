/* Symbol table manager for Bison,
   Copyright (C) 1984, 1989, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include "symtab.h"
#include "gram.h"

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

static bucket *
bucket_new (const char *tag)
{
  bucket *res = XMALLOC (bucket, 1);

  res->tag = xstrdup (tag);
  res->type_name = NULL;
  res->number = -1;
  res->prec = 0;
  res->assoc = right_assoc;
  res->user_token_number = SUNDEF;
  res->alias = NULL;
  res->class = unknown_sym;

  if (getenv ("DEBUG"))
    fprintf (stderr, "Creating: nsyms = %d, ntokens = %d: %s\n",
	     nsyms, ntokens, tag);
  nsyms++;

  return res;
}


/*------------.
| Free THIS.  |
`------------*/

static void
bucket_free (bucket *this)
{
#if 0
  /* This causes crashes because one string can appear more
     than once.  */
  XFREE (this->type_name);
#endif
  XFREE (this->tag);
  XFREE (this);
}



/*----------------------.
| A bucket hash table.  |
`----------------------*/

/* Initial capacity of buckets hash table.  */
#define HT_INITIAL_CAPACITY 257

static struct hash_table *bucket_table = NULL;

static bool
hash_compare_bucket (const bucket *m1, const bucket *m2)
{
  return strcmp (m1->tag, m2->tag) ? FALSE : TRUE;
}

static unsigned int
hash_bucket (const bucket *m, unsigned int tablesize)
{
  return hash_string (m->tag, tablesize);
}


/*-------------------------------.
| Create the bucket hash table.  |
`-------------------------------*/

void
buckets_new (void)
{
  bucket_table = hash_initialize (HT_INITIAL_CAPACITY,
				  NULL,
				  (Hash_hasher) hash_bucket,
				  (Hash_comparator) hash_compare_bucket,
				  (Hash_data_freer) bucket_free);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

bucket *
getsym (const char *key)
{
  bucket probe;
  bucket *entry;

  (const char *) probe.tag = key;
  entry = hash_lookup (bucket_table, &probe);

  if (!entry)
    {
      /* First insertion in the hash. */
      entry = bucket_new (key);
      hash_insert (bucket_table, entry);
    }
  return entry;
}


/*-------------------.
| Free the buckets.  |
`-------------------*/

void
buckets_free (void)
{
  hash_free (bucket_table);
}


/*---------------------------------------------------------------.
| Look for undefined buckets, report an error, and consider them |
| terminals.                                                     |
`---------------------------------------------------------------*/

void
buckets_do (bucket_processor processor, void *processor_data)
{
  hash_do_for_each (bucket_table,
		    (Hash_processor) processor,
		    processor_data);
}
