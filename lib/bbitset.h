/* Base bitset stuff.
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

#ifndef _BBITSET_H
#define _BBITSET_H

#include "libiberty.h"

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

/* Currently we support three flavours of bitsets:
   BITSET_ARRAY:  Array of bits (fixed size, faster).
   BITSET_LIST:   Linked list of array of bits (variable size, least storage
   for large very sparse sets). 
   BITSET_TABLE:  Expandable table of pointers to array of bits 
   (variable size, less storage for large sparse sets). 

   BITSET_STATS:  Wrapper bitset for internal use only.
*/
enum bitset_type {BITSET_ARRAY, BITSET_LIST, BITSET_TABLE, BITSET_TYPE_NUM,
		  BITSET_STATS};
#define BITSET_TYPE_NAMES {"abitset", "lbitset", "ebitset"}

enum bitset_alloc_type {BITSET_MALLOC, BITSET_OBALLOC};

/* Non-zero to use inline functions instead of macros.  */
#define BITSET_INLINE 0

/* Data type used to store a word of bits.  */
typedef unsigned long bitset_word;
#define BITSET_WORD_BITS ((unsigned) CHAR_BIT * sizeof (bitset_word))

/* Bit index.  */
typedef unsigned long bitset_bindex;

/* Word index.  */
typedef unsigned long bitset_windex;

#define BITSET_INDEX_MAX ((1U << (BITSET_WORD_BITS - 1)))

#define BITSET_MSB (1U << (BITSET_WORD_BITS - 1))

#define BITSET_LIST_SIZE 1024

enum bitset_ops {BITSET_OP_ZERO, BITSET_OP_ONES, 
		 BITSET_OP_COPY, BITSET_OP_NOT, 
		 BITSET_OP_EMPTY_P, BITSET_OP_EQUAL_P,
		 BITSET_OP_SUBSET_P, BITSET_OP_DISJOINT_P,
		 BITSET_OP_AND, BITSET_OP_OR, BITSET_OP_XOR, BITSET_OP_ANDN, 
		 BITSET_OP_OR_AND, BITSET_OP_AND_OR, BITSET_OP_ANDN_OR};

/* Return size in bits of bitset SRC.  */
#define BITSET_SIZE_(SRC) (SRC)->b.ops->size (SRC) 

/* Return type of bitset SRC.  */
#define BITSET_TYPE_(DST) (DST)->b.ops->type 

/* Set bit BITNO in bitset DST.  */
#define BITSET_SET_(DST, BITNO) (DST)->b.ops->set (DST, BITNO) 

/* Reset bit BITNO in bitset DST.  */
#define BITSET_RESET_(DST, BITNO) (DST)->b.ops->reset (DST, BITNO) 

/* Return non-zero if bit BITNO in bitset SRC is set.  */
#define BITSET_TEST_(SRC, BITNO) (SRC)->b.ops->test (SRC, BITNO) 

/* Free bitset SRC.  */
#define BITSET_FREE_(SRC)\
 ((SRC)->b.ops->free ? (SRC)->b.ops->free (SRC) :(void)0)

/* Perform operation OP on DST.  */
#define BITSET_OP1_(DST, OP) (DST)->b.ops->op1 (DST, OP) 

/* Perform operation OP on SRC and store in DST.  */
#define BITSET_OP2_(DST, SRC, OP) (DST)->b.ops->op2 (DST, SRC, OP) 

/* DST = SRC1 OP SRC2.  */
#define BITSET_OP3_(DST, SRC1, SRC2, OP) \
(DST)->b.ops->op3 (DST, SRC1, SRC2, OP) 

/* DST = (SRC1 OP1 SRC2) OP2 SRC3.  */
#define BITSET_OP4_(DST, SRC1, SRC2, SRC3, OP) \
(DST)->b.ops->op4 (DST, SRC1, SRC2, SRC3, OP) 

/* DST = 0.  */
#define BITSET_ZERO_(DST) BITSET_OP1_ (DST, BITSET_OP_ZERO);

/* DST = ~0.  */
#define BITSET_ONES_(DST) BITSET_OP1_ (DST, BITSET_OP_ONES);

/* Return SRC == 0.  */
#define BITSET_EMPTY_P_(SRC) BITSET_OP1_ (SRC, BITSET_OP_EMPTY_P);

/* Return DST == SRC.  */
#define BITSET_EQUAL_P_(DST, SRC) BITSET_OP2_ (DST, SRC, BITSET_OP_EQUAL_P) 

/* Return DST == DST | SRC.  */
#define BITSET_SUBSET_P_(DST, SRC) BITSET_OP2_ (DST, SRC, BITSET_OP_SUBSET_P) 

/* Return DST & SRC == 0.  */
#define BITSET_DISJOINT_P_(DST, SRC)\
 BITSET_OP2_ (DST, SRC, BITSET_OP_DISJOINT_P) 

/* DST = SRC.  */
#define BITSET_COPY_(DST, SRC) BITSET_OP2_ (DST, SRC, BITSET_OP_COPY) 

/* DST = ~SRC.  */
#define BITSET_NOT_(DST, SRC) BITSET_OP2_ (DST, SRC, BITSET_OP_NOT) 

/* DST = SRC1 | SRC2.  */
#define BITSET_OR_(DST, SRC1, SRC2) BITSET_OP3_ (DST, SRC1, SRC2,\
  BITSET_OP_OR) 

/* DST = SRC1 ^ SRC2.  */
#define BITSET_XOR_(DST, SRC1, SRC2) BITSET_OP3_ (DST, SRC1, SRC2,\
  BITSET_OP_XOR) 

/* DST = SRC1 & SRC2.  */
#define BITSET_AND_(DST, SRC1, SRC2) BITSET_OP3_ (DST, SRC1, SRC2, \
  BITSET_OP_AND) 

/* DST = SRC1 & ~SRC2.  */
#define BITSET_ANDN_(DST, SRC1, SRC2) BITSET_OP3_ (DST, SRC1, SRC2, \
  BITSET_OP_ANDN) 

/* DST = (SRC1 & SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & SRC2) | SRC3.  */
#define BITSET_AND_OR_(DST, SRC1, SRC2, SRC3)\
  BITSET_OP4_ (DST, SRC1, SRC2, SRC3, BITSET_OP_AND_OR) 

/* DST = (SRC1 | SRC2) & SRC3.  Return non-zero if
   DST != (SRC1 | SRC2) & SRC3.  */
#define BITSET_OR_AND_(DST, SRC1, SRC2, SRC3)\
  BITSET_OP4_ (DST, SRC1, SRC2, SRC3, BITSET_OP_OR_AND) 

/* DST = (SRC1 & ~SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & ~SRC2) | SRC3.  */
#define BITSET_ANDN_OR_(DST, SRC1, SRC2, SRC3)\
  BITSET_OP4_ (DST, SRC1, SRC2, SRC3, BITSET_OP_ANDN_OR) 

/* Find list of up to NUM bits set in BSET starting from and including 
   *NEXT.  Return with actual number of bits found and with *NEXT
   indicating where search stopped.  */
#define BITSET_LIST_(BSET, LIST, NUM, NEXT) \
(BSET)->b.ops->list (BSET, LIST, NUM, NEXT) 

/* Find reverse list of up to NUM bits set in BSET starting from and
   including NEXT.  Return with actual number of bits found and with
   *NEXT indicating where search stopped.  */
#define BITSET_REVERSE_LIST_(BSET, LIST, NUM, NEXT) \
(BSET)->b.ops->reverse_list (BSET, LIST, NUM, NEXT) 


struct bbitset_struct
{
  struct bitset_ops_struct *ops;
  bitset_windex cindex;		/* Cache word index.  */
  bitset_windex csize;		/* Cache size in words.  */
  bitset_word *cdata;		/* Cache data pointer.  */
  /* Perhaps we could sacrifice another word to indicate
     that the bitset is known to be zero, that a bit has been set
     in the cache, and that a bit has been cleared in the cache.
     This would speed up some of the searches but slightly slow down
     bit set/reset operations of cached bits.  */
};


typedef struct bitset_struct *bitset;


/* The contents of this structure should be considered private.  */
struct bitset_ops_struct
{
  void (*set) PARAMS ((struct bitset_struct *, bitset_bindex));
  void (*reset) PARAMS ((struct bitset_struct *, bitset_bindex));
  int (*test) PARAMS ((struct bitset_struct *, bitset_bindex));
  int (*size) PARAMS ((struct bitset_struct *));
  int (*op1) PARAMS ((struct bitset_struct *, enum bitset_ops));
  int (*op2) PARAMS ((struct bitset_struct *, struct bitset_struct *,
		      enum bitset_ops));
  int (*op3) PARAMS ((struct bitset_struct *, struct bitset_struct *,
		      struct bitset_struct *, enum bitset_ops));
  int (*op4) PARAMS ((struct bitset_struct *, struct bitset_struct *,
		      struct bitset_struct *, struct bitset_struct *,
		      enum bitset_ops));
  int (*list) PARAMS ((struct bitset_struct *, bitset_bindex *,
		       bitset_bindex, bitset_bindex *));
  int (*reverse_list) PARAMS ((struct bitset_struct *, bitset_bindex *,
			       bitset_bindex, bitset_bindex *));
  void (*free) PARAMS ((struct bitset_struct *));
  enum bitset_type type;
};

#define BITSET_COMPATIBLE_(BSET1, BSET2) ((BSET1)->b.ops == (BSET2)->b.ops)

#define BITSET_CHECK2_(DST, SRC) \
if (!BITSET_COMPATIBLE_ (DST, SRC)) abort ();

#define BITSET_CHECK3_(DST, SRC1, SRC2) \
if (!BITSET_COMPATIBLE_ (DST, SRC1) \
    || !BITSET_COMPATIBLE_ (DST, SRC2)) abort ();

#define BITSET_CHECK4_(DST, SRC1, SRC2, SRC3) \
if (!BITSET_COMPATIBLE_ (DST, SRC1) || !BITSET_COMPATIBLE_ (DST, SRC2) \
    || !BITSET_COMPATIBLE_ (DST, SRC3)) abort ();


extern int bitset_op4 PARAMS ((bitset, bitset, bitset, bitset, 
			       enum bitset_ops op));

#endif /* _BBITSET_H  */
