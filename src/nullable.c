/* Part of the bison parser generator,
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


/* Set up NULLABLE, a vector saying which nonterminals can expand into
   the null string.  NULLABLE[I - NTOKENS] is nonzero if symbol I can
   do so.  */

#include "system.h"
#include "types.h"
#include "gram.h"
#include "xalloc.h"
#include "nullable.h"

char *nullable = NULL;

void
set_nullable (void)
{
  short *r;
  short *s1;
  short *s2;
  int ruleno;
  int symbol;
  shorts *p;

  short *squeue;
  short *rcount;
  shorts **rsets;
  shorts *relts;
  char any_tokens;
  short *r1;

#ifdef	TRACE
  fprintf (stderr, _("Entering set_nullable"));
#endif

  nullable = XCALLOC (char, nvars) - ntokens;

  squeue = XCALLOC (short, nvars);
  s1 = s2 = squeue;

  rcount = XCALLOC (short, nrules + 1);
  rsets = XCALLOC (shorts *, nvars) - ntokens;
  /* This is said to be more elements than we actually use.
     Supposedly nitems - nrules is enough.
     But why take the risk?  */
  relts = XCALLOC (shorts, nitems + nvars + 1);
  p = relts;

  r = ritem;
  while (*r)
    {
      if (*r < 0)
	{
	  symbol = rlhs[-(*r++)];
	  if (symbol >= 0 && !nullable[symbol])
	    {
	      nullable[symbol] = 1;
	      *s2++ = symbol;
	    }
	}
      else
	{
	  r1 = r;
	  any_tokens = 0;
	  for (symbol = *r++; symbol > 0; symbol = *r++)
	    {
	      if (ISTOKEN (symbol))
		any_tokens = 1;
	    }

	  if (!any_tokens)
	    {
	      ruleno = -symbol;
	      r = r1;
	      for (symbol = *r++; symbol > 0; symbol = *r++)
		{
		  rcount[ruleno]++;
		  p->next = rsets[symbol];
		  p->value = ruleno;
		  rsets[symbol] = p;
		  p++;
		}
	    }
	}
    }

  while (s1 < s2)
    {
      p = rsets[*s1++];
      while (p)
	{
	  ruleno = p->value;
	  p = p->next;
	  if (--rcount[ruleno] == 0)
	    {
	      symbol = rlhs[ruleno];
	      if (symbol >= 0 && !nullable[symbol])
		{
		  nullable[symbol] = 1;
		  *s2++ = symbol;
		}
	    }
	}
    }

  XFREE (squeue);
  XFREE (rcount);
  XFREE (rsets + ntokens);
  XFREE (relts);
}


void
free_nullable (void)
{
  XFREE (nullable + ntokens);
}
