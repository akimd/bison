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
#include "quotearg.h"

static graph_t graph;
static FILE *fgraph = NULL;

static size_t node_output_size = 0;

/* Return an unambiguous printable representated, allocated in slot 0,
   for NAME, suitable for C strings.  */
static char const *
quote (char const *name)
{
  return quotearg_n_style (0, escape_quoting_style, name);
}

/* This part will construct the label of nodes. */
static void
print_core (int state, struct obstack *node_obstack)
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

  for (i = 0; i < k; i++)
    {
      if (i)
	obstack_sgrow (node_obstack, "\\n");

      sp1 = sp = ritem + statep->items[i];

      while (*sp > 0)
	sp++;

      rule = -(*sp);

      obstack_fgrow1 (node_obstack, "%d: ", rule);
      obstack_fgrow1 (node_obstack, " %s  ->  ", quote (tags[rlhs[rule]]));

      for (sp = ritem + rrhs[rule]; sp < sp1; sp++)
	obstack_fgrow1 (node_obstack, "%s ", quote (tags[*sp]));

      obstack_1grow (node_obstack, '.');

      while (*sp > 0)
	obstack_fgrow1 (node_obstack, " %s", quote (tags[*sp++]));
    }
}

/* Output in graph_obstack edges specifications in incidence with current 
   node.  */
static void
print_actions (int state, const char *node_name, struct obstack *node_obstack)
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
	obstack_sgrow (node_obstack, "$default: accept");
      else
	obstack_sgrow (node_obstack, "NO ACTIONS");
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
	    open_edge (&edge, fgraph);
	    /* The edge source is the current node.  */
	    edge.sourcename = node_name;
	    sprintf (buff, "%d", state1);
	    edge.targetname = buff;
	    edge.color = (symbol == 0) ? red : blue;
	    /* FIXME: Be aware that quote uses static memory.  The string
	       must be output immediately (which is the case here). */
	    edge.label = tags[symbol] ? quote (tags[symbol]) : NULL;
	    output_edge (&edge, fgraph);
	    close_edge (fgraph);
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
	  /* If something has been added in the NODE_OBSTACK after
	     the declaration of the label, then we need a `\n'.  */
	  if (obstack_object_size (node_obstack) > node_output_size)
	    obstack_sgrow (node_obstack, "\\n");
	  obstack_fgrow1 (node_obstack, _("%-4s\terror (nonassociative)"),
			  tags[symbol]);
	}
      if (j > 0)
	obstack_sgrow (node_obstack, "\\n");
    }

  if (consistent[state] && redp)
    {
      rule = redp->rules[0];
      symbol = rlhs[rule];
      if (obstack_object_size (node_obstack) > node_output_size)
	obstack_sgrow (node_obstack, "\\n");
      obstack_fgrow2 (node_obstack, _("$default\treduce using rule %d (%s)"),
		      rule, tags[symbol]);
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
	  open_edge (&edge, fgraph);
	  edge.sourcename = node_name;
	  sprintf (buff, "%d", state1);
	  edge.targetname = buff;
	  edge.color = red;
	  edge.label = tags[symbol] ? quote (tags[symbol]) : NULL;
	  output_edge (&edge, fgraph);
	  close_edge (fgraph);
	}
    }
}

/* Output in GRAPH_OBSTACK the current node specifications and edges 
   which go out from that node.  */
static void
print_state (int state)
{
  static char name[10];
  struct obstack node_obstack;
  node_t node;

  obstack_init (&node_obstack);
  new_node (&node);	/* Set node attributs default value.  */
  sprintf (name, "%d", state);
  node.title = name;	/* Give a name to the node.  */
  
  { 
    /* Here we begin to compute the node label. */  
    obstack_sgrow (&node_obstack, "\t\tlabel:\t\"");	/* Open Label  */
    
    /* Keep the size of NODE_OBSTACK before computing the label. It is 
       useful to format the label.  */
    node_output_size = obstack_object_size (&node_obstack);
    
    /* Compute the labels of nodes on the fly.  */
    print_core (state, &node_obstack);
    /* Compute edges and additionnal parts of node label.  */
    print_actions (state, node.title, &node_obstack);
    
    obstack_sgrow (&node_obstack, "\"\n");		/* Close Label.  */
  }    

  open_node (fgraph);
  /* Output a VCG formatted attributs list.  */
  output_node (&node, fgraph);
  /* Save the node label.  */
  fwrite (obstack_base (&node_obstack), 
	  obstack_object_size (&node_obstack), 1, fgraph);
  close_node (fgraph);

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

  for (i = 0; i < nstates; i++)
    /* Output nodes & edges. */
    print_state (i);

  /* Close graph. */
  close_graph (&graph, fgraph);
  xfclose (fgraph);
}
