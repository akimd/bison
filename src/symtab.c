/* Symbol table manager for Bison,
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


#include "system.h"
#include "symtab.h"
#include "gram.h"


bucket *firstsymbol;
static bucket *lastsymbol;
static bucket **symtab;

static int
hash (const char *key)
{
  const char *cp;
  int k;

  cp = key;
  k = 0;
  while (*cp)
    k = ((k << 1) ^ (*cp++)) & 0x3fff;

  return k % TABSIZE;
}

/*--------------------------------------------------------------.
| Create a new symbol, named TAG, which hash value is HASHVAL.  |
`--------------------------------------------------------------*/

static bucket *
bucket_new (const char *tag, int hashval)
{
  bucket *res = XMALLOC (bucket, 1);

  res->link = symtab[hashval];
  res->next = NULL;
  res->tag = xstrdup (tag);
  res->type_name = NULL;
  res->value = 0;
  res->prec = 0;
  res->assoc = right_assoc;
  res->user_token_number = 0;
  res->alias = NULL;
  res->class = unknown_sym;

  nsyms++;

  return res;
}


void
tabinit (void)
{
  symtab = XCALLOC (bucket *, TABSIZE);

  firstsymbol = NULL;
  lastsymbol = NULL;
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

bucket *
getsym (const char *key)
{
  int hashval;
  bucket *bp;
  int found;

  hashval = hash (key);
  bp = symtab[hashval];

  found = 0;
  while (bp != NULL && found == 0)
    {
      if (strcmp (key, bp->tag) == 0)
	found = 1;
      else
	bp = bp->link;
    }

  if (found == 0)
    {
      bp = bucket_new (key, hashval);

      if (firstsymbol == NULL)
	{
	  firstsymbol = bp;
	  lastsymbol = bp;
	}
      else
	{
	  lastsymbol->next = bp;
	  lastsymbol = bp;
	}

      symtab[hashval] = bp;
    }

  return bp;
}


void
free_symtab (void)
{
  int i;
  bucket *bp, *bptmp;		/* JF don't use ptr after free */

  for (i = 0; i < TABSIZE; i++)
    {
      bp = symtab[i];
      while (bp)
	{
	  bptmp = bp->link;
#if 0
	  /* This causes crashes because one string can appear more
	     than once.  */
	  if (bp->type_name)
	    XFREE (bp->type_name);
#endif
	  XFREE (bp->tag);
	  XFREE (bp);
	  bp = bptmp;
	}
    }
  XFREE (symtab);
}
