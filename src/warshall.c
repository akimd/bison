/* Generate transitive closure of a matrix,
   Copyright 1984, 1989, 2000, 2001 Free Software Foundation, Inc.

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
#include "warshall.h"

/*-------------------------------------------------------------.
| Given n by n matrix of bits R, modify its contents to be the |
| transive closure of what was given.                          |
`-------------------------------------------------------------*/

static void
bitmatrix_print (const char *title, unsigned *matrix, size_t size)
{
  size_t i, j;
  size_t rowsize = WORDSIZE (size) * sizeof (unsigned);
#define ROW(Num) ((unsigned *) ((char *) matrix + ((Num) * rowsize)))

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
	fputs (BITISSET (ROW (i), j) ? "1" : " ", stderr);
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

#define R(Num) (unsigned *) ((char *) R + ((Num) * rowsize))

static void
TC (unsigned *R, int n)
{
  int rowsize = WORDSIZE (n) * sizeof (unsigned);
  int i, j, k;

  if (trace_flag)
    bitmatrix_print ("TC: Input", R, n);

  /* R (J, I) && R (I, K) => R (J, K).
     I *must* be the outter loop. */

  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
      if (BITISSET (R (j), i))
	for (k = 0; k < n; ++k)
	  if (BITISSET (R (i), k))
	    SETBIT (R (j), k);

  if (trace_flag)
    bitmatrix_print ("TC: Output", R, n);
}


/*---------------------------------------------------------------.
| Reflexive Transitive Closure.  Same as TC and then set all the |
| bits on the diagonal of R.                                     |
`---------------------------------------------------------------*/

void
RTC (unsigned *R, int n)
{
  int rowsize = WORDSIZE (n) * sizeof (unsigned);
  int i;

  TC (R, n);
  for (i = 0; i < n; ++i)
    SETBIT (R (i), i);
}
