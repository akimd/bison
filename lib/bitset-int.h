/* Internal bitset definitions.
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

#ifndef _BITSET_INT_H
#define _BITSET_INT_H

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

/* Currently we support three flavours of bitsets:
   BITSET_ARRAY:  Array of bits (fixed size, faster).
   BITSET_LIST:   Linked list of array of bits (variable size, least storage
   for large very sparse sets).
   BITSET_TABLE:  Expandable table of pointers to array of bits
   (variable size, less storage for large sparse sets).
*/
enum bitset_type {BITSET_ARRAY, BITSET_LIST, BITSET_TABLE, BITSET_TYPE_NUM};
#define BITSET__TYPE_NAMES {"sbitset", "lbitset", "ebitset"}

/* Non-zero to enable bitset caching.  */
#define BITSET_CACHE 1

/* Non-zero to use inline functions instead of macros.  */
#define BITSET_INLINE 0

/* Non-zero to enable bitset statistics gathering.  */
#define BITSET_STATS 1

/* Non-zero to enable bitset type checking.  */
#define BITSET_CHECK 0

typedef unsigned long bitset_word;
#define BITSET_WORD_BITS ((unsigned) CHAR_BIT * sizeof (bitset_word))

/* Bit index.  */
typedef unsigned long bitset_bindex;

/* Word index.  */
typedef unsigned long bitset_windex;

#define BITSET_INDEX_MAX ((1U << (BITSET_WORD_BITS - 1)))

#define BITSET_MSB (1U << (BITSET_WORD_BITS - 1))

#define BITSET_LIST_SIZE 1024

enum bitset_ops {BITSET_ZERO, BITSET_ONES, BITSET_EMPTY_P,
		 BITSET_COPY, BITSET_EQUAL_P, BITSET_SUBSET_P, BITSET_NOT,
		 BITSET_AND, BITSET_OR, BITSET_XOR, BITSET_ANDN, BITSET_ORN,
		 BITSET_OR_AND, BITSET_AND_OR, BITSET_ANDN_OR};

/* Return size in bits of bitset SRC.  */
#define BITSET__SIZE(SRC) (SRC)->ops->size (SRC)

/* Set bit BITNO in bitset DST.  */
#define BITSET__SET(DST, BITNO) (DST)->ops->set (DST, BITNO)

/* Reset bit BITNO in bitset DST.  */
#define BITSET__RESET(DST, BITNO) (DST)->ops->reset (DST, BITNO)

/* Return non-zero if bit BITNO in bitset SRC is set.  */
#define BITSET__TEST(SRC, BITNO) (SRC)->ops->test (SRC, BITNO)

/* Free bitset SRC.  */
#define BITSET__FREE(SRC) ((SRC)->ops->free) (SRC)

/* Perform operation OP on DST.  */
#define BITSET__OP1(DST, OP) (DST)->ops->op1 (DST, OP)

/* Perform operation OP on SRC and store in DST.  */
#define BITSET__OP2(DST, SRC, OP) (DST)->ops->op2 (DST, SRC, OP)

/* DST = SRC1 OP SRC2.  */
#define BITSET__OP3(DST, SRC1, SRC2, OP) \
(DST)->ops->op3 (DST, SRC1, SRC2, OP)

/* DST = (SRC1 OP1 SRC2) OP2 SRC3.  */
#define BITSET__OP4(DST, SRC1, SRC2, SRC3, OP) \
(DST)->ops->op4 (DST, SRC1, SRC2, SRC3, OP)

/* DST = SRC.  */
#define BITSET__COPY(DST, SRC) BITSET__OP2 (DST, SRC, BITSET_COPY)

/* Find list of up to NUM bits set in BSET starting from and including
   *NEXT.  Return with actual number of bits found and with *NEXT
   indicating where search stopped.  */
#define BITSET__LIST(BSET, LIST, NUM, NEXT) \
(BSET)->ops->list (BSET, LIST, NUM, NEXT)

/* Find reverse list of up to NUM bits set in BSET starting from and
   including NEXT.  Return with actual number of bits found and with
   *NEXT indicating where search stopped.  */
#define BITSET__REVERSE_LIST(BSET, LIST, NUM, NEXT) \
(BSET)->ops->reverse_list (BSET, LIST, NUM, NEXT)

#endif /* _BITSET_INT_H  */
