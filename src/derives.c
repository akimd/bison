/* Match rules with nonterminals for bison,
   Copyright 1984, 1989, 2000, 2001  Free Software Foundation, Inc.

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
#include "getargs.h"
#include "symtab.h"
#include "types.h"
#include "reader.h"
#include "gram.h"
#include "derives.h"

short **derives = NULL;

static void
print_derives (void)
{
  int i;

  fputs ("DERIVES\n", stderr);

  for (i = ntokens; i < nsyms; i++)
    {
      short *sp;
      fprintf (stderr, "\t%s derives\n", symbols[i]->tag);
      for (sp = derives[i]; *sp > 0; sp++)
	{
	  item_number_t *rhsp;
	  fprintf (stderr, "\t\t%d:", *sp);
	  for (rhsp = rules[*sp].rhs; *rhsp >= 0; ++rhsp)
	    fprintf (stderr, " %s", symbols[*rhsp]->tag);
	  fprintf (stderr, " (rule %d)\n", -*rhsp - 1);
	}
    }

  fputs ("\n\n", stderr);
}


void
set_derives (void)
{
  int i;
  shorts *p;
  short *q;
  shorts **dset;
  shorts *delts;

  dset = XCALLOC (shorts *, nvars) - ntokens;
  delts = XCALLOC (shorts, nrules + 1);

  p = delts;
  for (i = nrules; i > 0; i--)
    {
      symbol_number_t lhs = rules[i].lhs->number;
      p->next = dset[lhs];
      p->value = i;
      dset[lhs] = p;
      p++;
    }

  derives = XCALLOC (short *, nvars) - ntokens;
  q = XCALLOC (short, nvars + nrules);

  for (i = ntokens; i < nsyms; i++)
    {
      derives[i] = q;
      p = dset[i];
      while (p)
	{
	  *q++ = p->value;
	  p = p->next;
	}
      *q++ = -1;
    }

  if (trace_flag)
    print_derives ();

  XFREE (dset + ntokens);
  XFREE (delts);
}

void
free_derives (void)
{
  XFREE (derives[ntokens]);
  XFREE (derives + ntokens);
}
