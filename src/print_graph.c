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
#include "obstack.h"
#include "print_graph.h"
#include "vcg.h"

static graph_t graph;
static FILE *fgraph = NULL;

/* This part will construct the label of nodes. */
static void
print_core (int state, struct obstack *node_obstack)
{
  int i;
  core *statep = state_table[state].state;

  if (!statep->nitems)
    return;

  for (i = 0; i < statep->nitems; i++)
    {
      short *sp;
      short *sp1;
      int rule;

      sp1 = sp = ritem + statep->items[i];

      while (*sp > 0)
	sp++;

      rule = -(*sp);

      if (i)
	obstack_1grow (node_obstack, '\n');
      obstack_fgrow1 (node_obstack, "%d: ", rule);
      obstack_fgrow1 (node_obstack, " %s  ->  ",
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
print_actions (int state, const char *node_name)
{
  int i;

  shifts   *shiftp = state_table[state].shifts;
  reductions *redp = state_table[state].reductions;
#if 0
  errs       *errp = err_table[state];
#endif

  static char buff[10];
  edge_t edge;

  if (!shiftp->nshifts && !redp)
    {
#if 0
      if (final_state == state)
	obstack_sgrow (node_obstack, "$default: accept");
      else
	obstack_sgrow (node_obstack, "NO ACTIONS");
#endif
      return;
    }

  for (i = 0; i < shiftp->nshifts; i++)
    if (!SHIFT_IS_DISABLED (shiftp, i))
      {
	int state1 = shiftp->shifts[i];
	int symbol = state_table[state1].accessing_symbol;

	new_edge (&edge);

	if (state > state1)
	  edge.type = back_edge;
	open_edge (&edge, fgraph);
	/* The edge source is the current node.  */
	edge.sourcename = node_name;
	sprintf (buff, "%d", state1);
	edge.targetname = buff;
	edge.color = (symbol == 0) ? red : blue;
	edge.label = tags[symbol];
	output_edge (&edge, fgraph);
	close_edge (fgraph);
      }

#if 0
  if (errp)
    {
      int j, nerrs;

      nerrs = errp->nerrs;

      for (j = 0; j < nerrs; j++)
	{
	  if (!errp->errs[j])
	    continue;
	  symbol = errp->errs[j];
	  /* If something has been added in the NODE_OBSTACK after
	     the declaration of the label, then we need a `\n'.
	  if (obstack_object_size (node_obstack) > node_output_size)
	    obstack_sgrow (node_obstack, "\n");
	    */
	  obstack_fgrow1 (node_obstack, _("%-4s\terror (nonassociative)"),
			  tags[symbol]);
	}
      if (j > 0)
	obstack_1grow (node_obstack, '\n');
    }

  if (state_table[state].consistent && redp)
    {
      rule = redp->rules[0];
      symbol = rule_table[rule].lhs;
      /*
      if (obstack_object_size (node_obstack) > node_output_size)
	obstack_sgrow (node_obstack, "\n");
	*/
      obstack_fgrow2 (node_obstack, _("$default\treduce using rule %d (%s)"),
		      rule, tags[symbol]);
    }
#endif

  if (i < shiftp->nshifts)
    for (; i < shiftp->nshifts; i++)
      if (!SHIFT_IS_DISABLED (shiftp, i))
	{
	  int state1 = shiftp->shifts[i];
	  int symbol = state_table[state1].accessing_symbol;

	  new_edge (&edge);
	  open_edge (&edge, fgraph);
	  edge.sourcename = node_name;
	  sprintf (buff, "%d", state1);
	  edge.targetname = buff;
	  edge.color = red;
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
print_state (int state)
{
  static char name[10];
  struct obstack node_obstack;
  node_t node;

  /* The labels of the nodes are their the items.  */
  obstack_init (&node_obstack);
  new_node (&node);
  sprintf (name, "%d", state);
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

  if (!graph_flag)
    return;

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
  for (i = 0; i < nstates; i++)
    print_state (i);

  /* Close graph. */
  close_graph (&graph, fgraph);
  xfclose (fgraph);
}
