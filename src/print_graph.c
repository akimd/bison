/* Output a VCG description on generated parser, for Bison,
   Copyright 2001 Free Software Foundation, Inc.

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
#include "files.h"
#include "symtab.h"
#include "gram.h"
#include "LR0.h"
#include "lalr.h"
#include "conflicts.h"
#include "complain.h"
#include "getargs.h"
#include "state.h"
#include "reader.h"
#include "closure.h"
#include "obstack.h"
#include "print_graph.h"
#include "vcg.h"

static graph_t graph;
static FILE *fgraph = NULL;


/*----------------------------.
| Construct the node labels.  |
`----------------------------*/

static void
print_core (struct obstack *oout, state_t *state)
{
  int i;
  item_number_t *sitems = state->items;
  int snritems   = state->nitems;

  /* Output all the items of a state, not only its kernel.  */
  if (trace_flag)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nritemset;
    }

  obstack_fgrow1 (oout, "state %2d\n", state->number);
  for (i = 0; i < snritems; i++)
    {
      item_number_t *sp;
      item_number_t *sp1;
      int rule;

      sp1 = sp = ritem + sitems[i];

      while (*sp >= 0)
	sp++;

      rule = -(*sp);

      if (i)
	obstack_1grow (oout, '\n');
      obstack_fgrow1 (oout, " %s -> ",
		      symbol_tag_get (rules[rule].lhs));

      for (sp = rules[rule].rhs; sp < sp1; sp++)
	obstack_fgrow1 (oout, "%s ", symbol_tag_get (symbols[*sp]));

      obstack_1grow (oout, '.');

      for (/* Nothing */; *sp >= 0; ++sp)
	obstack_fgrow1 (oout, " %s", symbol_tag_get (symbols[*sp]));

      /* Experimental feature: display the lookaheads. */
      if (trace_flag && state->nlookaheads)
	{
	  int j, k;
	  int nlookaheads = 0;
	  /* Look for lookaheads corresponding to this rule. */
	  for (j = 0; j < state->nlookaheads; ++j)
	    for (k = 0; k < ntokens; ++k)
	      if (bitset_test (state->lookaheads[j], k)
		  && state->lookaheads_rule[j]->number == rule)
		nlookaheads++;
	  if (nlookaheads)
	    {
	      obstack_sgrow (oout, "  [");
	      for (j = 0; j < state->nlookaheads; ++j)
		for (k = 0; k < ntokens; ++k)
		  if (bitset_test (state->lookaheads[j], k)
		      && state->lookaheads_rule[j]->number == rule)
		    obstack_fgrow2 (oout, "%s%s",
				    symbol_tag_get (symbols[k]),
				    --nlookaheads ? ", " : "");
	      obstack_sgrow (oout, "]");
	    }
	}
    }
}


/*---------------------------------------------------------------.
| Output in graph_obstack edges specifications in incidence with |
| current node.                                                  |
`---------------------------------------------------------------*/

static void
print_actions (state_t *state, const char *node_name)
{
  int i;

  shifts   *shiftp = state->shifts;
  reductions *redp = state->reductions;

  static char buff[10];
  edge_t edge;

  if (!shiftp->nshifts && !redp)
    return;

  for (i = 0; i < shiftp->nshifts; i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	state_number_t state1 = shiftp->shifts[i];
	symbol_number_t symbol = states[state1]->accessing_symbol;

	new_edge (&edge);

	if (state->number > state1)
	  edge.type = back_edge;
	open_edge (&edge, fgraph);
	/* The edge source is the current node.  */
	edge.sourcename = node_name;
	sprintf (buff, "%d", state1);
	edge.targetname = buff;
	/* Shifts are blue, gotos are green, and error is red. */
	if (SHIFT_IS_ERROR (shiftp, i))
	  edge.color = red;
	else
	  edge.color = SHIFT_IS_SHIFT(shiftp, i) ? blue : green;
	edge.label = symbol_tag_get (symbols[symbol]);
	output_edge (&edge, fgraph);
	close_edge (fgraph);
      }
}


/*-------------------------------------------------------------.
| Output in FGRAPH the current node specifications and exiting |
| edges.                                                       |
`-------------------------------------------------------------*/

static void
print_state (state_t *state)
{
  static char name[10];
  struct obstack node_obstack;
  node_t node;

  /* The labels of the nodes are their the items.  */
  obstack_init (&node_obstack);
  new_node (&node);
  sprintf (name, "%d", state->number);
  node.title = name;
  print_core (&node_obstack, state);
  obstack_1grow (&node_obstack, '\0');
  node.label = obstack_finish (&node_obstack);

  open_node (fgraph);
  output_node (&node, fgraph);
  close_node (fgraph);

  /* Output the edges.  */
  print_actions (state, name);

  obstack_free (&node_obstack, 0);
}


void
print_graph (void)
{
  state_number_t i;

  /* Output file.  */
  fgraph = xfopen (spec_graph_file, "w");

  new_graph (&graph);

#if 0
  graph.smanhattan_edges = yes;
  graph.manhattan_edges = yes;
#endif

  graph.display_edge_labels = yes;
  graph.layoutalgorithm = normal;

  graph.port_sharing = no;
  graph.finetuning = yes;
  graph.straight_phase = yes;
  graph.priority_phase = yes;
  graph.splines = yes;

  graph.crossing_weight = median;

  /* Output graph options. */
  open_graph (fgraph);
  output_graph (&graph, fgraph);

  /* Output nodes and edges. */
  new_closure (nritems);
  for (i = 0; i < nstates; i++)
    print_state (states[i]);
  free_closure ();

  /* Close graph. */
  close_graph (&graph, fgraph);
  xfclose (fgraph);
}
