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
#include "files.h"
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

/* This part will construct the label of nodes. */
static void
print_core (state_t *state, struct obstack *node_obstack)
{
  int i;
  short *sitems = state->items;
  int snitems   = state->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snitems);
  sitems = itemset;
  snitems = nitemset;

  obstack_fgrow1 (node_obstack, "%2d: ", state->number);
  for (i = 0; i < snitems; i++)
    {
      short *sp;
      short *sp1;
      int rule;

      sp1 = sp = ritem + sitems[i];

      while (*sp > 0)
	sp++;

      rule = -(*sp);

      if (i)
	obstack_sgrow (node_obstack, "\n    ");
      obstack_fgrow1 (node_obstack, " %s -> ",
		      tags[rule_table[rule].lhs]);

      for (sp = ritem + rule_table[rule].rhs; sp < sp1; sp++)
	obstack_fgrow1 (node_obstack, "%s ", tags[*sp]);

      obstack_1grow (node_obstack, '.');

      for (/* Nothing */; *sp > 0; ++sp)
	obstack_fgrow1 (node_obstack, " %s", tags[*sp]);
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
	int state1 = shiftp->shifts[i];
	int symbol = state_table[state1]->accessing_symbol;

	new_edge (&edge);

	if (state->number > state1)
	  edge.type = back_edge;
	open_edge (&edge, fgraph);
	/* The edge source is the current node.  */
	edge.sourcename = node_name;
	sprintf (buff, "%d", state1);
	edge.targetname = buff;
	/* Shifts are blue, gotos are red. */
	edge.color = SHIFT_IS_SHIFT(shiftp, i) ? blue : red;
	edge.label = tags[symbol];
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
  print_core (state, &node_obstack);
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
  int i;

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
  new_closure (nitems);
  for (i = 0; i < nstates; i++)
    print_state (state_table[i]);
  free_closure ();

  /* Close graph. */
  close_graph (&graph, fgraph);
  xfclose (fgraph);
}
