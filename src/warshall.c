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
#include "warshall.h"

/*-------------------------------------------------------------.
| Given n by n matrix of bits R, modify its contents to be the |
| transive closure of what was given.                          |
`-------------------------------------------------------------*/

#define R(Num) (unsigned *) ((char *) R + ((Num) * rowsize))

static void
TC (unsigned *R, int n)
{
  int rowsize = WORDSIZE (n) * sizeof (unsigned);
  int i, j, k;

  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
      if (BITISSET (R (i), j))
	for (k = 0; k < rowsize; ++k)
	  if (BITISSET (R (i), k))
	    SETBIT (R (j), k);
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
