/* Match rules with nonterminals for bison,
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


/* set_derives finds, for each variable (nonterminal), which rules can
   derive it.  It sets up the value of derives so that derives[i -
   ntokens] points to a vector of rule numbers, terminated with -1.
   */

#include "system.h"
#include "xalloc.h"
#include "types.h"
#include "gram.h"
#include "derives.h"

short **derives;

#if DEBUG

static void
print_derives (void)
{
  int i;
  short *sp;

  fputs ("\n\n\n", stdout);
  printf (_("DERIVES"));
  fputs ("\n\n", stdout);

  for (i = ntokens; i < nsyms; i++)
    {
      printf (_("%s derives"), tags[i]);
      for (sp = derives[i]; *sp > 0; sp++)
	{
	  printf ("  %d", *sp);
	}
      putchar ('\n');
    }

  putchar ('\n');
}

#endif

void
set_derives (void)
{
  int i;
  int lhs;
  shorts *p;
  short *q;
  shorts **dset;
  shorts *delts;

  dset = XCALLOC (shorts *, nvars) - ntokens;
  delts = XCALLOC (shorts, nrules + 1);

  p = delts;
  for (i = nrules; i > 0; i--)
    {
      lhs = rlhs[i];
      if (lhs >= 0)
	{
	  p->next = dset[lhs];
	  p->value = i;
	  dset[lhs] = p;
	  p++;
	}
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

#if DEBUG
  print_derives ();
#endif

  XFREE (dset + ntokens);
  XFREE (delts);
}

void
free_derives (void)
{
  XFREE (derives[ntokens]);
  XFREE (derives + ntokens);
}
