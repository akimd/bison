/* Part of the bison parser generator,
   Copyright 1984, 1989, 2000, 2001 Free Software Foundation, Inc.

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
#include "getargs.h"
#include "reader.h"
#include "types.h"
#include "gram.h"
#include "reduce.h"
#include "nullable.h"

char *nullable = NULL;

static void
nullable_print (FILE *out)
{
  int i;
  fputs ("NULLABLE\n", out);
  for (i = ntokens; i < nsyms; i++)
    fprintf (out, "\t%s: %s\n", tags[i], nullable[i] ? "yes" : "no");
  fputs ("\n\n", out);
}

void
set_nullable (void)
{
  short *r;
  short *s1;
  short *s2;
  shorts *p;

  short *squeue;
  short *rcount;
  shorts **rsets;
  shorts *relts;

  if (trace_flag)
    fprintf (stderr, "Entering set_nullable\n");

  nullable = XCALLOC (char, nvars) - ntokens;

  squeue = XCALLOC (short, nvars);
  s1 = s2 = squeue;

  rcount = XCALLOC (short, nrules + 1);

  /* RITEM contains all the rules, including useless productions.
     Hence we must allocate room for useless nonterminals too.  */
  rsets = XCALLOC (shorts *, nvars + nuseless_nonterminals) - ntokens;
  /* This is said to be more elements than we actually use.
     Supposedly nitems - nrules is enough.
     But why take the risk?  */
  relts = XCALLOC (shorts, nitems + nvars + nuseless_nonterminals + 1);
  p = relts;

  for (r = ritem; *r; ++r)
    {
      /* Walk RITEM to find (i), if there are any tokens in the
	 RHS, and (ii), to find RULENO. */
      int ruleno;
      int any_tokens = 0;
      short *r1;
      for (r1 = r; *r1 > 0; ++r1)
	if (ISTOKEN (*r1))
	  any_tokens = 1;
      ruleno = -*r1;

      /* Examine the RHS of the rule.  */
      if (!any_tokens)
	for (/* Nothing. */; *r > 0; ++r)
	  {
	    rcount[ruleno]++;
	    p->next = rsets[*r];
	    p->value = ruleno;
	    rsets[*r] = p;
	    p++;
	  }

      /* Examine its LHS. */
      if (rule_table[ruleno].useful && !nullable[rule_table[ruleno].lhs])
	{
	  nullable[rule_table[ruleno].lhs] = 1;
	  *s2++ = rule_table[ruleno].lhs;
        }
    }

  while (s1 < s2)
    for (p = rsets[*s1++]; p; p = p->next)
      {
	int ruleno = p->value;
	if (--rcount[ruleno] == 0)
	  if (rule_table[ruleno].useful && !nullable[rule_table[ruleno].lhs])
	    {
	      nullable[rule_table[ruleno].lhs] = 1;
	      *s2++ = rule_table[ruleno].lhs;
	    }
      }

  XFREE (squeue);
  XFREE (rcount);
  XFREE (rsets + ntokens);
  XFREE (relts);

  if (trace_flag)
    nullable_print (stderr);
}


void
free_nullable (void)
{
  XFREE (nullable + ntokens);
}
