/* Binary relations.
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


#ifndef RELATION_H_
# define RELATION_H_

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation_t, then GRAPH[Node] is a list of adjacent
   nodes, ended with -1.  */

typedef short relation_node_t;
typedef relation_node_t *relation_nodes_t;
typedef relation_nodes_t *relation_t;


/* Report a RELATION that has SIZE vertices.  */
void relation_print PARAMS ((relation_t relation, size_t size,
			     FILE *out));

/* Compute the transitive closure of the FUNCTION on the RELATION with
   SIZE vertices.

   If RELATION (NODE-1, NODE-2) then on exit FUNCTION[NODE-1] was
   extended (unioned) with FUNCTION[NODE-2].  */
void relation_digraph PARAMS ((relation_t relation, size_t size,
			       bitsetv *function));

/* Destructively transpose *R_ARG, of size N.  */
void relation_transpose PARAMS ((relation_t *R_arg, int n));

#endif /* ! RELATION_H_ */
