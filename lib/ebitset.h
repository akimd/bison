/* Functions to support ebitsets.
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

#ifndef _EBITSET_H
#define _EBITSET_H

#include "bitset-int.h"

/* Number of words to use for each element.  */
#ifndef EBITSET_ELT_WORDS
#define EBITSET_ELT_WORDS 2
#endif

/* Number of bits stored in each element.  */
#define EBITSET_ELT_BITS \
  ((unsigned) (EBITSET_ELT_WORDS * BITSET_WORD_BITS))

/* Ebitset element.  We use an array of bits.  */
typedef struct ebitset_elt_struct
{
  union
  {
    bitset_word words[EBITSET_ELT_WORDS]; /* Bits that are set.  */
    struct ebitset_elt_struct *next;
  } u;
} ebitset_elt;


typedef ebitset_elt *ebitset_elts;

/* Head of ebitset linked list.  */
typedef struct ebitset_struct
{
  unsigned int size;		/* Number of elements.  */
  ebitset_elts *elts;		/* Expanding array of pointers to elements.  */
} *ebitset;


extern int ebitset_bytes PARAMS ((bitset_bindex));

extern void ebitset_release_memory PARAMS ((void));

#endif
