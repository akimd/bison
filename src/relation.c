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

#include "system.h"
#include "bitsetv.h"
#include "relation.h"
#include "getargs.h"

void
relation_print (relation_t relation, size_t size, FILE *out)
{
  unsigned i, j;

  for (i = 0; i < size; ++i)
    {
      fprintf (out, "%3d: ", i);
      if (relation[i])
	for (j = 0; relation[i][j] != -1; ++j)
	  fprintf (out, "%3d ", relation[i][j]);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}


/*---------------------------------------------------------------.
| digraph & traverse.                                            |
|                                                                |
| The following variables are used as common storage between the |
| two.                                                           |
`---------------------------------------------------------------*/

static relation_t R;
static relation_nodes_t INDEX;
static relation_nodes_t VERTICES;
static int top;
static int infinity;
static bitsetv F;

static void
traverse (int i)
{
  int j;
  int height;

  VERTICES[++top] = i;
  INDEX[i] = height = top;

  if (R[i])
    for (j = 0; R[i][j] >= 0; ++j)
      {
	if (INDEX[R[i][j]] == 0)
	  traverse (R[i][j]);

	if (INDEX[i] > INDEX[R[i][j]])
	  INDEX[i] = INDEX[R[i][j]];

	bitset_or (F[i], F[i], F[R[i][j]]);
      }

  if (INDEX[i] == height)
    for (;;)
      {
	j = VERTICES[top--];
	INDEX[j] = infinity;

	if (i == j)
	  break;

	bitset_copy (F[j], F[i]);
      }
}


void
relation_digraph (relation_t relation, size_t size,
		  bitsetv *function)
{
  unsigned i;

  infinity = size + 2;
  INDEX = XCALLOC (relation_node_t, size + 1);
  VERTICES = XCALLOC (relation_node_t, size + 1);
  top = 0;

  R = relation;
  F = *function;

  for (i = 0; i < size; i++)
    INDEX[i] = 0;

  for (i = 0; i < size; i++)
    if (INDEX[i] == 0 && R[i])
      traverse (i);

  XFREE (INDEX);
  XFREE (VERTICES);

  *function = F;
}


/*-------------------------------------------.
| Destructively transpose R_ARG, of size N.  |
`-------------------------------------------*/

void
relation_transpose (relation_t *R_arg, int n)
{
  /* The result. */
  relation_t new_R = XCALLOC (relation_nodes_t, n);
  /* END_R[I] -- next entry of NEW_R[I]. */
  relation_t end_R = XCALLOC (relation_nodes_t, n);
  /* NEDGES[I] -- total size of NEW_R[I]. */
  int *nedges = XCALLOC (int, n);
  int i, j;

  if (trace_flag & trace_sets)
    {
      fputs ("relation_transpose: input\n", stderr);
      relation_print (*R_arg, n, stderr);
    }

  /* Count. */
  for (i = 0; i < n; i++)
    if ((*R_arg)[i])
      for (j = 0; (*R_arg)[i][j] >= 0; ++j)
	++nedges[(*R_arg)[i][j]];

  /* Allocate. */
  for (i = 0; i < n; i++)
    if (nedges[i] > 0)
      {
	relation_node_t *sp = XCALLOC (relation_node_t, nedges[i] + 1);
	sp[nedges[i]] = -1;
	new_R[i] = sp;
	end_R[i] = sp;
      }

  /* Store. */
  for (i = 0; i < n; i++)
    if ((*R_arg)[i])
      for (j = 0; (*R_arg)[i][j] >= 0; ++j)
	{
	  *end_R[(*R_arg)[i][j]] = i;
	  ++end_R[(*R_arg)[i][j]];
	}

  free (nedges);
  free (end_R);

  /* Free the input: it is replaced with the result. */
  for (i = 0; i < n; i++)
    XFREE ((*R_arg)[i]);
  free (*R_arg);

  if (trace_flag & trace_sets)
    {
      fputs ("relation_transpose: output\n", stderr);
      relation_print (new_R, n, stderr);
    }

  *R_arg = new_R;
}
