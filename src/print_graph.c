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
  if (report_flag & report_itemsets)
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
      rule_number_t rule;

      sp1 = sp = ritem + sitems[i];

      while (*sp >= 0)
	sp++;

      rule = item_number_as_rule_number (*sp);

      if (i)
	obstack_1grow (oout, '\n');
      obstack_fgrow1 (oout, " %s -> ",
		      rules[rule].lhs->tag);

      for (sp = rules[rule].rhs; sp < sp1; sp++)
	obstack_fgrow1 (oout, "%s ", symbols[*sp]->tag);

      obstack_1grow (oout, '.');

      for (/* Nothing */; *sp >= 0; ++sp)
	obstack_fgrow1 (oout, " %s", symbols[*sp]->tag);

      /* Experimental feature: display the lookaheads. */
      if (report_flag & report_lookaheads)
	{
	  /* Find the reduction we are handling.  */
	  reductions_t *reds = state->reductions;
	  int redno = state_reduction_find (state, &rules[rule]);

	  /* Print them if there are.  */
	  if (reds->lookaheads && redno != -1)
	    {
	      bitset_iterator biter;
	      int k;
	      int not_first = 0;
	      obstack_sgrow (oout, "[");
	      BITSET_FOR_EACH (biter, reds->lookaheads[redno], k, 0)
		obstack_fgrow2 (oout, "%s%s",
				not_first++ ? ", " : "",
				symbols[k]->tag);
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

  transitions_t     *transitions = state->transitions;
  reductions_t *redp = state->reductions;

  static char buff[10];
  edge_t edge;

  if (!transitions->num && !redp)
    return;

  for (i = 0; i < transitions->num; i++)
    if (!TRANSITION_IS_DISABLED (transitions, i))
      {
	state_t *state1 = transitions->states[i];
	symbol_number_t symbol = state1->accessing_symbol;

	new_edge (&edge);

	if (state->number > state1->number)
	  edge.type = back_edge;
	open_edge (&edge, fgraph);
	/* The edge source is the current node.  */
	edge.sourcename = node_name;
	sprintf (buff, "%d", state1->number);
	edge.targetname = buff;
	/* Shifts are blue, gotos are green, and error is red. */
	if (TRANSITION_IS_ERROR (transitions, i))
	  edge.color = red;
	else
	  edge.color = TRANSITION_IS_SHIFT(transitions, i) ? blue : green;
	edge.label = symbols[symbol]->tag;
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
