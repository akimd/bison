/* Output a VCG description on generated parser, for bison,
   Copyright 1984, 1986, 1989, 2000, 2001 Free Software Foundation, Inc.

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

/* This part will construct the label of nodes. */
static void
print_core (int state)
{
  int i;
  int k;
  int rule;
  core *statep;
  short *sp;
  short *sp1;
  
  statep = state_table[state];
  k = statep->nitems;

  if (k == 0)
    return;
  
  obstack_sgrow(&graph_obstack, "\t\tlabel:\t\"");

  for (i = 0; i < k; i++)
    {
      sp1 = sp = ritem + statep->items[i];

      while (*sp > 0)
	sp++;

      rule = -(*sp);

      obstack_fgrow1(&graph_obstack, _("%d: "), rule);
      obstack_fgrow1(&graph_obstack, " %s  ->  ", tags[rlhs[rule]]);  

      for (sp = ritem + rrhs[rule]; sp < sp1; sp++)
	obstack_fgrow1(&graph_obstack, "%s ", tags[*sp]);

      obstack_1grow(&graph_obstack, '.');
  
      while (*sp > 0)
	obstack_fgrow1(&graph_obstack, " %s", tags[*sp++]);
      
      obstack_sgrow(&graph_obstack, "\\n");
    }
  obstack_sgrow(&graph_obstack, "\"\n");
}

static void
print_actions (int state, node_t *node)
{
  int i;
  int k;
  int state1;
  int symbol;
  shifts *shiftp;
  errs *errp;
  reductions *redp;
  int rule;
  static char buff[10];
  edge_t edge;
  
  shiftp = shift_table[state];
  redp = reduction_table[state];
  errp = err_table[state];

  if (!shiftp && !redp)
    {
      if (final_state == state)
	;//fprintf(f, "    $default\taccept\n");
      else
	;//fprintf(f, "    NO ACTIONS\n");
      return;
    }

  if (shiftp)
    {
      k = shiftp->nshifts;

      for (i = 0; i < k; i++)
	{
	  if (!shiftp->shifts[i])
	    continue;
	  state1 = shiftp->shifts[i];
	  symbol = accessing_symbol[state1];

	  if (ISVAR (symbol))
	    break;
	  
	  {
	    new_edge (&edge);

	    if (state > state1)
	      edge.type = back_edge;
	    open_edge (&edge, &graph_obstack);
	    edge.sourcename = node->title;
	    edge.targetname = buff;
	    sprintf (edge.targetname, "%d", state1);
	    edge.color = (symbol == 0) ? blue : red; 
	    edge.label = tags[symbol];
	    output_edge (&edge, &graph_obstack);    
	    close_edge (&graph_obstack);
	  }	
	}
    }
  else
    {
      i = 0;
      k = 0;
    }

  if (errp)
    {
      int j, nerrs;

      nerrs = errp->nerrs;

      for (j = 0; j < nerrs; j++)
	{
	  if (!errp->errs[j])
	    continue;
	  symbol = errp->errs[j];
	}
    }
  
  if (consistent[state] && redp)
    {
      rule = redp->rules[0];
      symbol = rlhs[rule];
    }
  
  if (i < k)
    {
      for (; i < k; i++)
	{
	  if (!shiftp->shifts[i])
	    continue;
	  state1 = shiftp->shifts[i];
	  symbol = accessing_symbol[state1];

	  new_edge (&edge);
	  open_edge (&edge, &graph_obstack);
	  edge.sourcename = node->title;
	  edge.targetname = buff;
	  edge.color = red;
	  sprintf (edge.targetname, "%d", state1);
	  edge.label = tags[symbol];
	  output_edge (&edge, &graph_obstack);
	  close_edge (&graph_obstack);
	}
    }
}

static void
print_state (int state)
{
  static char name[10];
  node_t node;  

  new_node (&node);
  open_node (&graph_obstack);
  
  sprintf(name, "%d", state);
  node.title = name;  
  output_node (&node, &graph_obstack);
  
  print_core (state);   /* node label */
  
  close_node (&graph_obstack);
  
  print_actions (state, &node);	/* edges */
}


void
print_graph (void)
{
  int i;
  
  if (!graph_flag)
    return ;
  new_graph (&graph);
  
  /* graph.smanhattan_edges = yes;
     graph.manhattan_edges = yes; */
  
  graph.display_edge_labels = yes;
  graph.layoutalgorithm = 0;
  
  graph.port_sharing = no;
  graph.finetuning = yes;
  graph.straight_phase = yes;
  graph.priority_phase = yes;
  graph.splines = yes;
  
  graph.crossing_weight = median;

  /* Output graph options. */
  open_graph (&graph_obstack);
  output_graph (&graph, &graph_obstack);

  for (i = 0; i < nstates; i++)
      /* Output nodes & edges. */
      print_state (i);

  /* Close graph. */
  close_graph (&graph, &graph_obstack);
}

