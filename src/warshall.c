/* Generate transitive closure of a matrix,
   Copyright 1984, 1989, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include "getargs.h"
#include "bitsetv.h"
#include "warshall.h"

/*--------------------------------------------------------.
| Display the MATRIX array of SIZE bitsets of size SIZE.  |
`--------------------------------------------------------*/

static void
bitmatrix_print (const char *title, bitsetv matrix)
{
  size_t i, j;
  size_t size = bitset_size (matrix[0]);

  /* Title. */
  fprintf (stderr, "%s BEGIN\n", title);

  /* Column numbers. */
  fputs ("   ", stderr);
  for (i = 0; i < size; ++i)
    putc (i / 10 ? '0' + i / 10 : ' ', stderr);
  putc ('\n', stderr);
  fputs ("   ", stderr);
  for (i = 0; i < size; ++i)
    fprintf (stderr, "%d", i % 10);
  putc ('\n', stderr);

  /* Bar. */
  fputs ("  .", stderr);
  for (i = 0; i < size; ++i)
    putc ('-', stderr);
  fputs (".\n", stderr);

  /* Contents. */
  for (i = 0; i < size; ++i)
    {
      fprintf (stderr, "%2d|", i);
      for (j = 0; j < size; ++j)
	fputs (bitset_test (matrix[i], j) ? "1" : " ", stderr);
      fputs ("|\n", stderr);
    }

  /* Bar. */
  fputs ("  `", stderr);
  for (i = 0; i < size; ++i)
    putc ('-', stderr);
  fputs ("'\n", stderr);

  /* End title. */
  fprintf (stderr, "%s END\n\n", title);
}

/*-------------------------------------------------------------------.
| Given the MATRIX array of N bitsets of size N, modify its contents |
| to be the transive closure of what was given.                      |
`-------------------------------------------------------------------*/

static void
TC (bitsetv matrix)
{
  int i, j;

  if (trace_flag)
    bitmatrix_print ("TC: Input", matrix);

  /* R (J, I) && R (I, K) => R (J, K).
     I *must* be the outter loop. */

  for (i = 0; matrix[i]; ++i)
    for (j = 0; matrix[j]; ++j)
      if (bitset_test (matrix[j], i))
	bitset_or (matrix[j], matrix[j], matrix[i]);

  if (trace_flag)
    bitmatrix_print ("TC: Output", matrix);
}


/*---------------------------------------------------------------.
| Reflexive Transitive Closure.  Same as TC and then set all the |
| bits on the diagonal of MATRIX.                                |
`---------------------------------------------------------------*/

void
RTC (bitsetv matrix)
{
  int i;

  TC (matrix);
  for (i = 0; matrix[i]; ++i)
    bitset_set (matrix[i], i);
}
