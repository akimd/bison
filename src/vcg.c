/* VCG description handler for Bison.
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
#include "xalloc.h"
#include "vcg.h"
#include "vcg_defaults.h"

/* Initialize a graph with the default values. */
void
new_graph (graph_t *g)
{
  g->title = G_TITLE;
  g->label = G_LABEL;

  g->infos[0] = G_INFOS1;
  g->infos[1] = G_INFOS2;
  g->infos[2] = G_INFOS3;

  g->color = G_COLOR;
  g->textcolor = G_TEXTCOLOR;
  g->bordercolor = G_BORDERCOLOR;

  g->width = G_WIDTH;
  g->height = G_HEIGHT;
  g->borderwidth = G_BORDERWIDTH;
  g->x = G_X;
  g->y = G_Y;
  g->folding = G_FOLDING;
  g->shrink = G_SHRINK;
  g->stretch = G_STRETCH;

  g->textmode = G_TEXTMODE;
  g->shape = G_SHAPE;

  g->vertical_order = G_VERTICAL_ORDER;
  g->horizontal_order = G_HORIZONTAL_ORDER;

  g->xmax = G_XMAX; /* Not output. */
  g->ymax = G_YMAX; /* Not output. */

  g->xbase = G_XBASE;
  g->ybase = G_YBASE;

  g->xspace = G_XSPACE;
  g->yspace = G_YSPACE;
  g->xlspace = G_XLSPACE; /* Not output. */

  g->xraster = G_XRASTER;
  g->yraster = G_YRASTER;
  g->xlraster = G_XLRASTER;

  g->hidden = G_HIDDEN; /* No default value. */

  g->classname = G_CLASSNAME; /* No class name association. */

  g->layoutalgorithm = G_LAYOUTALGORITHM;
  g->layout_downfactor = G_LAYOUT_DOWNFACTOR;
  g->layout_upfactor = G_LAYOUT_UPFACTOR;
  g->layout_nearfactor = G_LAYOUT_NEARFACTOR;
  g->layout_splinefactor = G_LAYOUT_SPLINEFACTOR;

  g->late_edge_labels = G_LATE_EDGE_LABELS;
  g->display_edge_labels = G_DISPLAY_EDGE_LABELS;
  g->dirty_edge_labels = G_DIRTY_EDGE_LABELS;
  g->finetuning = G_FINETUNING;
  g->ignore_singles = G_IGNORE_SINGLES;
  g->straight_phase = G_STRAIGHT_PHASE;
  g->priority_phase = G_PRIORITY_PHASE;
  g->manhattan_edges = G_MANHATTAN_EDGES;
  g->smanhattan_edges = G_SMANHATTAN_EDGES;
  g->near_edges = G_NEAR_EDGES;

  g->orientation = G_ORIENTATION;
  g->node_alignement = G_NODE_ALIGNEMENT;
  g->port_sharing = G_PORT_SHARING;
  g->arrow_mode = G_ARROW_MODE;
  g->treefactor = G_TREEFACTOR;
  g->spreadlevel = G_SPREADLEVEL;
  g->crossing_weight = G_CROSSING_WEIGHT;
  g->crossing_phase2 = G_CROSSING_PHASE2;
  g->crossing_optimization = G_CROSSING_OPTIMIZATION;
  g->view = G_VIEW;

  g->edges = G_EDGES;
  g->nodes = G_NODES;
  g->splines = G_SPLINES;

  g->bmax = G_BMAX;
  g->cmin = G_CMIN;
  g->cmax = G_CMAX;
  g->pmin = G_PMIN;
  g->pmax = G_PMAX;
  g->rmin = G_RMIN;
  g->rmax = G_RMAX;
  g->smax = G_SMAX;

  g->node_list = G_NODE_LIST;
  g->edge_list = G_EDGE_LIST;

  new_edge(&g->edge);
  new_node(&g->node);
}

/* Initialize a node with the defalut values. */
void
new_node (node_t *node)
{
  node->title = N_TITLE;
  node->label = N_LABEL;

  node->locx = N_LOCX; /* Default unspcified. */
  node->locy = N_LOCY; /* Default unspcified. */

  node->vertical_order = N_VERTICAL_ORDER;	/* Default unspcified. */
  node->horizontal_order = N_HORIZONTAL_ORDER;	/* Default unspcified. */

  node->width = N_WIDTH; /* We assume that we can't define it now. */
  node->height = N_HEIGHT; /* Also. */

  node->shrink = N_SHRINK;
  node->stretch = N_STRETCH;

  node->folding = N_FOLDING; /* No explicit default value. */

  node->shape = N_SHAPE;
  node->textmode = N_TEXTMODE;
  node->borderwidth = N_BORDERWIDTH;

  node->color = N_COLOR;
  node->textcolor = N_TEXTCOLOR;
  node->bordercolor = N_BORDERCOLOR;

  node->infos[0] = N_INFOS1;
  node->infos[1] = N_INFOS2;
  node->infos[2] = N_INFOS3;

  node->next = N_NEXT;
}

/* Initialize a edge with the defalut values. */
void
new_edge (edge_t *edge)
{
  edge->type = E_EDGE_TYPE;

  edge->sourcename = E_SOURCENAME;
  edge->targetname = E_TARGETNAME;
  edge->label = E_LABEL;

  edge->linestyle = E_LINESTYLE;
  edge->thickness = E_THICKNESS;

  edge->class = E_CLASS;

  edge->color = E_COLOR;
  edge->textcolor = E_TEXTCOLOR;
  edge->arrowcolor = E_ARROWCOLOR;
  edge->backarrowcolor = E_BACKARROWCOLOR;

  edge->arrowsize = E_ARROWSIZE;
  edge->backarrowsize = E_BACKARROWSIZE;
  edge->arrowstyle = E_ARROWSTYLE;

  edge->backarrowstyle = E_BACKARROWSTYLE;

  edge->priority = E_PRIORITY;

  edge->anchor = E_ANCHOR;

  edge->horizontal_order = E_HORIZONTAL_ORDER;

  edge->next = E_NEXT;
}

/*----------------------------------------------.
| Get functions.       	                        |
| Return string corresponding to an enum value. |
`----------------------------------------------*/

static const char *
get_color_str (enum color_e c)
{
  switch (c)
    {
    case white:		return "white";
    case blue:		return "blue";
    case red:		return "red";
    case green:		return "green";
    case yellow:	return "yellow";
    case magenta:	return "magenta";
    case cyan:		return "cyan";
    case darkgrey:	return "darkgrey";
    case darkblue:	return "darkblue";
    case darkred:	return "darkred";
    case darkgreen:	return "darkgreen";
    case darkyellow:	return "darkyellow";
    case darkmagenta:	return "darkmagenta";
    case darkcyan:	return "darkcyan";
    case gold:		return "gold";
    case lightgrey:	return "lightgrey";
    case lightblue:	return "lightblue";
    case lightred:	return "lightred";
    case lightgreen:	return "lightgreen";
    case lightyellow:	return "lightyellow";
    case lightmagenta:	return "lightmagenta";
    case lightcyan:	return "lightcyan";
    case lilac:		return "lilac";
    case turquoise:	return "turquoise";
    case aquamarine:	return "aquamarine";
    case khaki:		return "khaki";
    case purple:	return "purple";
    case yellowgreen:	return "yellowgreen";
    case pink:		return "pink";
    case orange:	return "orange";
    case orchid:	return "orchid";
    case black:		return "black";
    default:
      assert (!"Not a default color.");
    }
  return NULL;
}

static const char *
get_textmode_str (enum textmode_e t)
{
  switch (t)
    {
    case centered:	return "center";
    case left_justify:	return "left_justify";
    case right_justify:	return "right_justify";
    default:
      assert (!"Not a text mode.");
    }
  return NULL;
}

static const char *
get_shape_str (enum shape_e s)
{
  switch (s)
    {
    case box:		return "box";
    case rhomb:		return "rhomb";
    case ellipse:	return "ellipse";
    case triangle:	return "triangle";
    default:
      assert (!"Not a shape.");
    }
  return NULL;
}

static const char *
get_layoutalgorithm_str (enum layoutalgorithm_e l)
{
  switch (l)
    {
    case normal:       	return "normal";
    case maxdepth:	return "maxdepth";
    case mindepth:	return "mindepth";
    case maxdepthslow:	return "maxdepthslow";
    case mindepthslow:	return "mindepthslow";
    case maxdegree:	return "maxdegree";
    case mindegree:	return "mindegree";
    case maxindegree:	return "maxindegree";
    case minindegree:	return "minindegree";
    case maxoutdegree:	return "maxoutdegree";
    case minoutdegree:	return "minoutdegree";
    case minbackward:	return "minbackward";
    case dfs:		return "dfs";
    case tree:		return "tree";
    default:
      assert (!"Not a layout algorithm.");
    }
  return NULL;
}

static const char *
get_decision_str (enum decision_e d)
{
  switch (d)
    {
    case no:	return "no";
    case yes:	return "yes";
    default:
      assert (!"Either yes nor no.");
    }
  return NULL;
}

static const char *
get_orientation_str (enum orientation_e o)
{
  switch (o)
    {
    case top_to_bottom:	return "top_to_bottom";
    case bottom_to_top: return "bottom_to_top";
    case left_to_right: return "left_to_right";
    case right_to_left: return "right_to_left";
    default:
      assert (!"Not an orientation.");
    }
  return NULL;
}

static const char *
get_node_alignement_str (enum alignement_e a)
{
  switch (a)
    {
    case center:	return "center";
    case top:		return "top";
    case bottom:	return "bottom";
    default:
      assert (!"Not an alignement.");
    }
  return NULL;
}

static const char *
get_arrow_mode_str (enum arrow_mode_e a)
{
  switch (a)
    {
    case fixed:		return "fixed";
    case free_a:	return "free";
    default:
      assert (!"Not an arrow mode.");
    }
  return NULL;
}

static const char *
get_crossing_type_str (enum crossing_type_e c)
{
  switch (c)
    {
    case bary:		return "bary";
    case median:	return "median";
    case barymedian:	return "barymedian";
    case medianbary:	return "medianbary";
    default:
      assert (!"Not a crossing type.");
    }
  return NULL;
}

static const char *
get_view_str (enum view_e v)
{
  switch (v)
    {
    case normal_view:	return "normal_view";
    case cfish:		return "cfish";
    case pfish:		return "pfish";
    case fcfish:	return "fcfish";
    case fpfish:	return "fpfish";
    default:
      assert (!"Not a view.");
    }
  return NULL;
}

static const char *
get_linestyle_str (enum linestyle_e l)
{
  switch (l)
    {
    case continuous:	return "continuous";
    case dashed:	return "dashed";
    case dotted:	return "dotted";
    case invisible:	return "invisible";
    default:
      assert (!"Not a line style.");
    }
  return NULL;
}

static const char *
get_arrowstyle_str (enum arrowstyle_e a)
{
  switch (a)
    {
    case solid:	return "solid";
    case line:	return "line";
    case none:	return "none";
    default:
      assert (!"Not an arrow style.");
    }
  return NULL;
}

/*----------------------------.
| Add functions.       	      |
| Edge and Nodes int a graph. |
`----------------------------*/

void
add_node (graph_t *graph, node_t *node)
{
  node->next = graph->node_list;
  graph->node_list = node;
}

void
add_edge (graph_t *graph, edge_t *edge)
{
  edge->next = graph->edge_list;
  graph->edge_list = edge;
}

void
add_classname (graph_t *g, int val, const char *name)
{
  struct classname_s *classname;
  
  classname = XMALLOC (struct classname_s, 1);
  classname->no = val;
  classname->name = name;
  classname->next = g->classname;
  g->classname = classname;
}

void
add_infoname (graph_t *g, int integer, const char *string)
{
  struct infoname_s *infoname;
  
  infoname = XMALLOC (struct infoname_s, 1);
  infoname->integer = integer;
  infoname->string = string;
  infoname->next = g->infoname;
  g->infoname = infoname;
}

/* Build a colorentry struct and add it to the list.  */
void
add_colorentry (graph_t *g, int color_idx, int red_cp, 
		int green_cp, int blue_cp)
{
  struct colorentry_s *ce;
  
  ce = XMALLOC (struct colorentry_s, 1);
  ce->color_index = color_idx;
  ce->red_cp = red_cp;
  ce->green_cp = green_cp;
  ce->blue_cp = blue_cp;
  ce->next = g->colorentry;
  g->colorentry = ce;
}

/*-------------------------------------.
| Open and close functions (formatted) |
`-------------------------------------*/

void
open_edge(edge_t *edge, struct obstack *os)
{
  switch (edge->type)
    {
    case normal_edge:
      obstack_sgrow (os, "\tedge: {\n");
      break;
    case back_edge:
      obstack_sgrow (os, "\tbackedge: {\n");
      break;
    case near_edge:
      obstack_sgrow (os, "\tnearedge: {\n");
      break;
    case bent_near_edge:
      obstack_sgrow (os, "\tbentnearedge: {\n");
      break;
    default:
      obstack_sgrow (os, "\tedge: {\n");
    }
}

void
close_edge(struct obstack *os)
{
  obstack_sgrow (os, "\t}\n");
}

void
open_node(struct obstack *os)
{
  obstack_sgrow (os, "\tnode: {\n");
}

void
close_node(struct obstack *os)
{
  obstack_sgrow (os, "\t}\n");
}

void
open_graph(struct obstack *os)
{
  obstack_sgrow (os, "graph: {\n");
}

void
close_graph(graph_t *graph, struct obstack *os)
{
  obstack_1grow (os, '\n');

  /* FIXME: Unallocate nodes and edges if required.  */
  {
    node_t *node;

    for (node = graph->node_list; node; node = node->next)
      {
	open_node (os);
	output_node (node, os);
	close_node (os);
      }
  }

  obstack_1grow (os, '\n');

  {
    edge_t *edge;

    for (edge = graph->edge_list; edge; edge = edge->next)
      {
	open_edge (edge, os);
	output_edge (edge, os);
	close_edge (os);
      }
  }

  obstack_sgrow (os, "}\n");
}

/*-------------------------------------------.
| Output functions (formatted) in obstack os |
`-------------------------------------------*/

void
output_node (node_t *node, struct obstack *os)
{
  if (node->title != N_TITLE)
    obstack_fgrow1 (os, "\t\ttitle:\t\"%s\"\n", node->title);
  if (node->label != N_LABEL)
    obstack_fgrow1 (os, "\t\tlabel:\t\"%s\"\n", node->label);

  if ((node->locx != N_LOCX) && (node->locy != N_LOCY))
    obstack_fgrow2 (os, "\t\tloc { x: %d  y: %d }\t\n", node->locx, node->locy);

  if (node->vertical_order != N_VERTICAL_ORDER)
    obstack_fgrow1 (os, "\t\tvertical_order:\t%d\n", node->vertical_order);
  if (node->horizontal_order != N_HORIZONTAL_ORDER)
    obstack_fgrow1 (os, "\t\thorizontal_order:\t%d\n", node->horizontal_order);

  if (node->width != N_WIDTH)
    obstack_fgrow1 (os, "\t\twidth:\t%d\n", node->width);
  if (node->height != N_HEIGHT)
    obstack_fgrow1 (os, "\t\theight:\t%d\n", node->height);

  if (node->shrink != N_SHRINK)
    obstack_fgrow1 (os, "\t\tshrink:\t%d\n", node->shrink);
  if (node->stretch != N_STRETCH)
    obstack_fgrow1 (os, "\t\tstretch:\t%d\n", node->stretch);

  if (node->folding != N_FOLDING)
    obstack_fgrow1 (os, "\t\tfolding:\t%d\n", node->folding);

  if (node->textmode != N_TEXTMODE)
    obstack_fgrow1 (os, "\t\ttextmode:\t%s\n",
		    get_textmode_str (node->textmode));

  if (node->shape != N_SHAPE)
    obstack_fgrow1 (os, "\t\tshape:\t%s\n", get_shape_str (node->shape));

  if (node->borderwidth != N_BORDERWIDTH)
    obstack_fgrow1 (os, "\t\tborderwidth:\t%d\n", node->borderwidth);

  if (node->color != N_COLOR)
    obstack_fgrow1 (os, "\t\tcolor:\t%s\n", get_color_str (node->color));
  if (node->textcolor != N_TEXTCOLOR)
    obstack_fgrow1 (os, "\t\ttextcolor:\t%s\n",
		    get_color_str (node->textcolor));
  if (node->bordercolor != N_BORDERCOLOR)
    obstack_fgrow1 (os, "\t\tbordercolor:\t%s\n",
		    get_color_str (node->bordercolor));

  if (node->infos[0])
    obstack_fgrow1 (os, "\t\tinfo1:\t\"%s\"\n", node->infos[0]);
  if (node->infos[1])
    obstack_fgrow1 (os, "\t\tinfo2:\t\"%s\"\n", node->infos[1]);
  if (node->infos[2])
    obstack_fgrow1 (os, "\t\tinfo3:\t\"%s\"\n", node->infos[2]);
}

void
output_edge (edge_t *edge, struct obstack *os)
{
  /* FIXME: SOURCENAME and TARGETNAME are mandatory
     so it has to be fatal not to give these informations.  */
  if (edge->sourcename != E_SOURCENAME)
    obstack_fgrow1 (os, "\t\tsourcename:\t\"%s\"\n", edge->sourcename);
  if (edge->targetname != E_TARGETNAME)
    obstack_fgrow1 (os, "\t\ttargetname:\t\"%s\"\n", edge->targetname);

  if (edge->label != E_LABEL)
    obstack_fgrow1 (os, "\t\tlabel:\t\"%s\"\n", edge->label);

  if (edge->linestyle != E_LINESTYLE)
    obstack_fgrow1 (os, "\t\tlinestyle:\t\"%s\"\n",
		    get_linestyle_str(edge->linestyle));

  if (edge->thickness != E_THICKNESS)
    obstack_fgrow1 (os, "\t\tthickness:\t%d\n", edge->thickness);
  if (edge->class != E_CLASS)
    obstack_fgrow1 (os, "\t\tclass:\t%d\n", edge->class);

  if (edge->color != E_COLOR)
    obstack_fgrow1 (os, "\t\tcolor:\t%s\n", get_color_str (edge->color));
  if (edge->color != E_TEXTCOLOR)
    obstack_fgrow1 (os, "\t\ttextcolor:\t%s\n",
		    get_color_str (edge->textcolor));
  if (edge->arrowcolor != E_ARROWCOLOR)
    obstack_fgrow1 (os, "\t\tarrowcolor:\t%s\n",
		    get_color_str (edge->arrowcolor));
  if (edge->backarrowcolor != E_BACKARROWCOLOR)
    obstack_fgrow1 (os, "\t\tbackarrowcolor:\t%s\n",
		    get_color_str (edge->backarrowcolor));

  if (edge->arrowsize != E_ARROWSIZE)
    obstack_fgrow1 (os, "\t\tarrowsize:\t%d\n", edge->arrowsize);
  if (edge->backarrowsize != E_BACKARROWSIZE)
    obstack_fgrow1 (os, "\t\tbackarrowsize:\t%d\n", edge->backarrowsize);

  if (edge->arrowstyle != E_ARROWSTYLE)
    obstack_fgrow1 (os, "\t\tarrowstyle:\t%s\n",
		    get_arrowstyle_str(edge->arrowstyle));
  if (edge->backarrowstyle != E_BACKARROWSTYLE)
    obstack_fgrow1 (os, "\t\tbackarrowstyle:\t%s\n",
		    get_arrowstyle_str(edge->backarrowstyle));

  if (edge->priority != E_PRIORITY)
    obstack_fgrow1 (os, "\t\tpriority:\t%d\n", edge->priority);
  if (edge->anchor != E_ANCHOR)
    obstack_fgrow1 (os, "\t\tanchor:\t%d\n", edge->anchor);
  if (edge->horizontal_order != E_HORIZONTAL_ORDER)
    obstack_fgrow1 (os, "\t\thorizontal_order:\t%d\n", edge->horizontal_order);
}

void
output_graph (graph_t *graph, struct obstack *os)
{
  if (graph->title)
    obstack_fgrow1 (os, "\ttitle:\t\"%s\"\n", graph->title);
  if (graph->label)
    obstack_fgrow1 (os, "\tlabel:\t\"%s\"\n", graph->label);

  if (graph->infos[0])
    obstack_fgrow1 (os, "\tinfo1:\t\"%s\"\n", graph->infos[0]);
  if (graph->infos[1])
    obstack_fgrow1 (os, "\tinfo2:\t\"%s\"\n", graph->infos[1]);
  if (graph->infos[2])
    obstack_fgrow1 (os, "\tinfo3:\t\"%s\"\n", graph->infos[2]);

  if (graph->color != G_COLOR)
    obstack_fgrow1 (os, "\tcolor:\t%s\n", get_color_str (graph->color));
  if (graph->textcolor != G_TEXTCOLOR)
    obstack_fgrow1 (os, "\ttextcolor:\t%s\n", get_color_str (graph->textcolor));
  if (graph->bordercolor != G_BORDERCOLOR)
    obstack_fgrow1 (os, "\tbordercolor:\t%s\n",
		    get_color_str (graph->bordercolor));

  if (graph->width != G_WIDTH)
    obstack_fgrow1 (os, "\twidth:\t%d\n", graph->width);
  if (graph->height != G_HEIGHT)
    obstack_fgrow1 (os, "\theight:\t%d\n", graph->height);
  if (graph->borderwidth != G_BORDERWIDTH)
    obstack_fgrow1 (os, "\tborderwidth:\t%d\n", graph->borderwidth);

  if (graph->x != G_X)
    obstack_fgrow1 (os, "\tx:\t%d\n", graph->x);
  if (graph->y != G_Y)
    obstack_fgrow1 (os, "\ty:\t%d\n", graph->y);

  if (graph->folding != G_FOLDING)
    obstack_fgrow1 (os, "\tfolding:\t%d\n", graph->folding);

  if (graph->shrink != G_SHRINK)
    obstack_fgrow1 (os, "\tshrink:\t%d\n", graph->shrink);
  if (graph->stretch != G_STRETCH)
    obstack_fgrow1 (os, "\tstretch:\t%d\n", graph->stretch);

  if (graph->textmode != G_TEXTMODE)
    obstack_fgrow1 (os, "\ttextmode:\t%s\n",
		    get_textmode_str (graph->textmode));

  if (graph->shape != G_SHAPE)
    obstack_fgrow1 (os, "\tshape:\t%s\n", get_shape_str (graph->shape));
  
  if (graph->vertical_order != G_VERTICAL_ORDER)
    obstack_fgrow1 (os, "\tvertical_order:\t%d\n", graph->vertical_order);  
  if (graph->horizontal_order != G_HORIZONTAL_ORDER)
    obstack_fgrow1 (os, "\thorizontal_order:\t%d\n", graph->horizontal_order);  

  if (graph->xmax != G_XMAX)
    obstack_fgrow1 (os, "\txmax:\t%d\n", graph->xmax);
  if (graph->ymax != G_YMAX)
    obstack_fgrow1 (os, "\tymax:\t%d\n", graph->ymax);

  if (graph->xbase != G_XBASE)
    obstack_fgrow1 (os, "\txbase:\t%d\n", graph->xbase);
  if (graph->ybase != G_YBASE)
    obstack_fgrow1 (os, "\tybase:\t%d\n", graph->ybase);

  if (graph->xspace != G_XSPACE)
    obstack_fgrow1 (os, "\txspace:\t%d\n", graph->xspace);
  if (graph->yspace != G_YSPACE)
    obstack_fgrow1 (os, "\tyspace:\t%d\n", graph->yspace);
  if (graph->xlspace != G_XLSPACE)
    obstack_fgrow1 (os, "\txlspace:\t%d\n", graph->xlspace);

  if (graph->xraster != G_XRASTER)
    obstack_fgrow1 (os, "\txraster:\t%d\n", graph->xraster);
  if (graph->yraster != G_YRASTER)
    obstack_fgrow1 (os, "\tyraster:\t%d\n", graph->yraster);
  if (graph->xlraster != G_XLRASTER)
    obstack_fgrow1 (os, "\txlraster:\t%d\n", graph->xlraster);

  if (graph->hidden != G_HIDDEN)
    obstack_fgrow1 (os, "\thidden:\t%d\n", graph->hidden);
  
  /* FIXME: Unallocate struct list if required.  
     Maybe with a little function.  */
  if (graph->classname != G_CLASSNAME)
    {
      struct classname_s *ite;

      for (ite = graph->classname; ite; ite = ite->next)
	obstack_fgrow2 (os, "\tclassname %d :\t%s\n", ite->no, ite->name);
    }

  if (graph->infoname != G_INFONAME)
    {
      struct infoname_s *ite;

      for (ite = graph->infoname; ite; ite = ite->next)
	obstack_fgrow2 (os, "\tinfoname %d :\t%s\n", ite->integer, ite->string);
    }

  if (graph->colorentry != G_COLORENTRY)
    {
      struct colorentry_s *ite;
      char buff[64];
      
      for (ite = graph->colorentry; ite; ite = ite->next)
	{	
	  sprintf (buff, "\tcolorentry %d :\t%d %d %d\n", 
		   ite->color_index, 
		   ite->red_cp,
		   ite->green_cp,
		   ite->blue_cp);
	  obstack_sgrow (os, buff);
	}    
    }

  if (graph->layoutalgorithm != G_LAYOUTALGORITHM)
    obstack_fgrow1 (os, "\tlayoutalgorithm:\t%s\n",
		    get_layoutalgorithm_str(graph->layoutalgorithm));

  if (graph->layout_downfactor != G_LAYOUT_DOWNFACTOR)
    obstack_fgrow1 (os, "\tlayout_downfactor:\t%d\n", graph->layout_downfactor);
  if (graph->layout_upfactor != G_LAYOUT_UPFACTOR)
    obstack_fgrow1 (os, "\tlayout_upfactor:\t%d\n", graph->layout_upfactor);
  if (graph->layout_nearfactor != G_LAYOUT_NEARFACTOR)
    obstack_fgrow1 (os, "\tlayout_nearfactor:\t%d\n", graph->layout_nearfactor);
  if (graph->layout_splinefactor != G_LAYOUT_SPLINEFACTOR)
    obstack_fgrow1 (os, "\tlayout_splinefactor:\t%d\n",
		    graph->layout_splinefactor);

  if (graph->late_edge_labels != G_LATE_EDGE_LABELS)
    obstack_fgrow1 (os, "\tlate_edge_labels:\t%s\n",
		    get_decision_str(graph->late_edge_labels));
  if (graph->display_edge_labels != G_DISPLAY_EDGE_LABELS)
    obstack_fgrow1 (os, "\tdisplay_edge_labels:\t%s\n",
		    get_decision_str(graph->display_edge_labels));
  if (graph->dirty_edge_labels != G_DIRTY_EDGE_LABELS)
    obstack_fgrow1 (os, "\tdirty_edge_labels:\t%s\n",
		    get_decision_str(graph->dirty_edge_labels));
  if (graph->finetuning != G_FINETUNING)
    obstack_fgrow1 (os, "\tfinetuning:\t%s\n",
		    get_decision_str(graph->finetuning));
  if (graph->ignore_singles != G_IGNORE_SINGLES)
    obstack_fgrow1 (os, "\tignore_singles:\t%s\n",
		    get_decision_str(graph->ignore_singles));
  if (graph->straight_phase != G_STRAIGHT_PHASE)
    obstack_fgrow1 (os, "\tstraight_phase:\t%s\n",
		    get_decision_str(graph->straight_phase));
  if (graph->priority_phase != G_PRIORITY_PHASE)
    obstack_fgrow1 (os, "\tpriority_phase:\t%s\n",
		    get_decision_str(graph->priority_phase));
  if (graph->manhattan_edges != G_MANHATTAN_EDGES)
    obstack_fgrow1 (os,
		    "\tmanhattan_edges:\t%s\n",
		    get_decision_str(graph->manhattan_edges));
  if (graph->smanhattan_edges != G_SMANHATTAN_EDGES)
    obstack_fgrow1 (os,
		    "\tsmanhattan_edges:\t%s\n",
		    get_decision_str(graph->smanhattan_edges));
  if (graph->near_edges != G_NEAR_EDGES)
    obstack_fgrow1 (os, "\tnear_edges:\t%s\n",
		    get_decision_str(graph->near_edges));

  if (graph->orientation != G_ORIENTATION)
    obstack_fgrow1 (os, "\torientation:\t%s\n",
		    get_orientation_str(graph->orientation));

  if (graph->node_alignement != G_NODE_ALIGNEMENT)
    obstack_fgrow1 (os, "\tnode_alignement:\t%s\n",
		    get_node_alignement_str(graph->node_alignement));

  if (graph->port_sharing != G_PORT_SHARING)
    obstack_fgrow1 (os, "\tport_sharing:\t%s\n",
		    get_decision_str(graph->port_sharing));

  if (graph->arrow_mode != G_ARROW_MODE)
    obstack_fgrow1 (os, "\tarrow_mode:\t%s\n",
		    get_arrow_mode_str(graph->arrow_mode));

  if (graph->treefactor != G_TREEFACTOR)
    obstack_fgrow1 (os, "\ttreefactor:\t%f\n", graph->treefactor);
  if (graph->spreadlevel != G_SPREADLEVEL)
    obstack_fgrow1 (os, "\tspreadlevel:\t%d\n", graph->spreadlevel);

  if (graph->crossing_weight != G_CROSSING_WEIGHT)
    obstack_fgrow1 (os, "\tcrossing_weight:\t%s\n",
		    get_crossing_type_str(graph->crossing_weight));
  if (graph->crossing_phase2 != G_CROSSING_PHASE2)
    obstack_fgrow1 (os, "\tcrossing_phase2:\t%s\n",
		    get_decision_str(graph->crossing_phase2));
  if (graph->crossing_optimization != G_CROSSING_OPTIMIZATION)
    obstack_fgrow1 (os, "\tcrossing_optimization:\t%s\n",
		    get_decision_str(graph->crossing_optimization));

  if (graph->view != G_VIEW)
    obstack_fgrow1 (os, "\tview:\t%s\n", get_view_str(graph->view));

  if (graph->edges != G_EDGES)
    obstack_fgrow1 (os, "\tedges:\t%s\n", get_decision_str(graph->edges));

  if (graph->nodes != G_NODES)
    obstack_fgrow1 (os,"\tnodes:\t%s\n", get_decision_str(graph->nodes));

  if (graph->splines != G_SPLINES)
    obstack_fgrow1 (os, "\tsplines:\t%s\n", get_decision_str(graph->splines));

  if (graph->bmax != G_BMAX)
    obstack_fgrow1 (os, "\tbmax:\t%d\n", graph->bmax);
  if (graph->cmin != G_CMIN)
    obstack_fgrow1 (os, "\tcmin:\t%d\n", graph->cmin);
  if (graph->cmax != G_CMAX)
    obstack_fgrow1 (os, "\tcmax:\t%d\n", graph->cmax);
  if (graph->pmin != G_PMIN)
    obstack_fgrow1 (os, "\tpmin:\t%d\n", graph->pmin);
  if (graph->pmax != G_PMAX)
    obstack_fgrow1 (os, "\tpmax:\t%d\n", graph->pmax);
  if (graph->rmin != G_RMIN)
    obstack_fgrow1 (os, "\trmin:\t%d\n", graph->rmin);
  if (graph->rmax != G_RMAX)
    obstack_fgrow1 (os, "\trmax:\t%d\n", graph->rmax);
  if (graph->smax != G_SMAX)
    obstack_fgrow1 (os, "\tsmax:\t%d\n", graph->smax);
}
