/* Symbol table manager for Bison,
   Copyright (C) 1984, 1989 Free Software Foundation, Inc.

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
#include "xalloc.h"
#include "symtab.h"
#include "gram.h"


bucket *firstsymbol;
static bucket *lastsymbol;
static bucket **symtab;

static int
hash (const char *key)
{
  register const char *cp;
  register int k;

  cp = key;
  k = 0;
  while (*cp)
    k = ((k << 1) ^ (*cp++)) & 0x3fff;

  return k % TABSIZE;
}



static char *
copys (const char *s)
{
  register int i;
  register const char *cp;
  register char *result;

  i = 1;
  for (cp = s; *cp; cp++)
    i++;

  result = XMALLOC(char, i);
  strcpy(result, s);
  return result;
}


void
tabinit (void)
{
/*   register int i; JF unused */

  symtab = XCALLOC (bucket *, TABSIZE);

  firstsymbol = NULL;
  lastsymbol = NULL;
}


bucket *
getsym (const char *key)
{
  register int hashval;
  register bucket *bp;
  register int found;

  hashval = hash(key);
  bp = symtab[hashval];

  found = 0;
  while (bp != NULL && found == 0)
    {
      if (strcmp(key, bp->tag) == 0)
	found = 1;
      else
	bp = bp->link;
    }

  if (found == 0)
    {
      nsyms++;

      bp = XCALLOC (bucket, 1);
      bp->link = symtab[hashval];
      bp->next = NULL;
      bp->tag = copys(key);
      bp->class = unknown_sym;

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
  register int i;
  register bucket *bp,*bptmp;/* JF don't use ptr after free */

  for (i = 0; i < TABSIZE; i++)
    {
      bp = symtab[i];
      while (bp)
	{
	  bptmp = bp->link;
#if 0 /* This causes crashes because one string can appear more than once.  */
	  if (bp->type_name)
	    XFREE(bp->type_name);
#endif
	  XFREE(bp);
	  bp = bptmp;
	}
    }
  XFREE(symtab);
}
