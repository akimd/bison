/* Functions to support lbitsets.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Michael Hayes (m.hayes@elec.canterbury.ac.nz).

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _LBITSET_H
#define _LBITSET_H

#include "bitset-int.h"

/* Number of words to use for each element.  The larger the value the
   greater the size of the cache and the shorter the time to find a given bit
   but the more memory wasted for sparse bitsets and the longer the time
   to search for set bits.  */
#ifndef LBITSET_ELT_WORDS
#define LBITSET_ELT_WORDS 2
#endif

typedef bitset_word lbitset_word;
#define LBITSET_WORD_BITS BITSET_WORD_BITS

/* Number of bits stored in each element.  */
#define LBITSET_ELT_BITS \
  ((unsigned) (LBITSET_ELT_WORDS * LBITSET_WORD_BITS))

/* Lbitset element.   We use an array of bits for each element.
   These are linked together in a doubly-linked list.  */
typedef struct lbitset_elt_struct
{
  struct lbitset_elt_struct *next; /* Next element.  */
  struct lbitset_elt_struct *prev; /* Previous element.  */
  bitset_windex index;		/* bitno / BITSET_WORD_BITS.  */
  bitset_word words[LBITSET_ELT_WORDS]; /* Bits that are set.  */
} lbitset_elt;


/* Head of lbitset linked list.  */
typedef struct lbitset_struct
{
  lbitset_elt *head;		/* First element in linked list.  */
  lbitset_elt *tail;		/* Last element in linked list.  */
} *lbitset;


extern int lbitset_bytes PARAMS ((bitset_bindex));

extern void lbitset_release_memory PARAMS ((void));

#endif
