/* Grammar reduction for Bison.
   Copyright (C) 2002  Free Software Foundation, Inc.

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

/* Reduce the grammar: Look for injections.  Akim Demaille.  */

#include "system.h"
#include "quotearg.h"
#include "bitsetv.h"
#include "bitsetv-print.h"
#include "complain.h"
#include "gram.h"
#include "getargs.h"
#include "derives.h"
#include "injections.h"

/* Set of all nonterminals which are not useless.  */
static bitsetv injects;

#define   INJECTS(Var)   injects[(Var) - ntokens]

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

void
injections_new (void)
{
  if (!injects)
    injects = bitsetv_create (nvars, nvars, BITSET_FIXED);
  else
    bitsetv_zero (injects);
}

void
injections_free (void)
{
  bitsetv_free (injects);
  injects = NULL;
}


/*------------------------------.
| Dump the list of injections.  |
`------------------------------*/

static void
injections_print (const char *title)
{
  int i, j;

  fprintf (stderr, "%s\n", title);
  for (i = ntokens; i < nsyms; i++)
    if (bitset_count (INJECTS (i)))
    {
      fprintf (stderr, "\t%s injects\n",
	       quotearg_style (escape_quoting_style, symbols[i]->tag));
      for (j = 0; j < nvars; j++)
	if (bitset_test (INJECTS (i), j))
	  fprintf (stderr, "\t\t%s\n",
		   quotearg_style (escape_quoting_style,
				   symbols[j + ntokens]->tag));
    }
  fprintf (stderr, "\n\n");
}


/*-----------------------------------------------------------------.
| Set INJECTS[i, j] if the nonterminal j derives the nonterminal j |
| (typically, `i -> j' is a rule, plus transitive closure).        |
`-----------------------------------------------------------------*/

static void
injections_compute (void)
{
  int i, j;

  injections_new ();
  for (i = ntokens; i < nsyms; i++)
    for (j = 0; derives[i][j] >= 0; ++j)
      {
	int symbol = rules[derives[i][j]].rhs[0];
	if (ISVAR (symbol) && rules[derives[i][j]].rhs[1] < 0)
	  bitset_set (INJECTS (i), symbol - ntokens);
      }

  if (trace_flag & trace_sets)
    injections_print ("syntactic direct injections");
  bitsetv_transitive_closure (injects);
  if (trace_flag & trace_sets)
    injections_print ("syntactic injections");
}



/*--------------------------------------------------------------.
| Look for infinitely ambiguous grammars: they contain cycles.  |
`--------------------------------------------------------------*/

void
injections_check_cycles (void)
{
  int i;

  injections_compute ();
  for (i = ntokens; i < nsyms; i++)
    if (bitset_test (INJECTS (i), i - ntokens))
      complain (_("infinitely ambiguous grammar: %s derives itself"),
		symbols[i]->tag);
}
