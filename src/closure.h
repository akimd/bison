/* Subroutines for bison
   Copyright 1984, 1989, 2000, 2001  Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#ifndef CLOSURE_H_
# define CLOSURE_H_

/* Subroutines of file LR0.c. */

/* Allocates the itemset and ruleset vectors, and precomputes useful
   data so that closure can be called.  n is the number of elements to
   allocate for itemset.  */

void new_closure PARAMS ((int n));


/* Given the kernel (aka core) of a state (a vector of item numbers
   ITEMS, of length N), set up ruleset and itemset to indicate what
   rules could be run and which items could be accepted when those
   items are the active ones.

   ruleset contains a bit for each rule.  closure sets the bits for
   all rules which could potentially describe the next input to be
   read.

   ITEMSET is a vector of item numbers; NITEMSET is its size
   9actually, points to just beyond the end of the part of it that is
   significant).  closure places there the indices of all items which
   represent units of input that could arrive next.  */

void closure PARAMS ((short *items, int n));


/* Frees itemset, ruleset and internal data.  */

void free_closure PARAMS ((void));

extern short *itemset;
extern int nitemset;

#endif /* !CLOSURE_H_ */
