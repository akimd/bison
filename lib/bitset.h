/* Generic bitsets.
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

#ifndef _BITSET_H
#define _BITSET_H 

/* This file is the public interface to the bitset abstract data type.
   Only use the functions and macros defined in this file.  */

#include "bbitset.h"
#include "obstack.h"
#include <stdio.h>

/* Attributes used to select a bitset implementation.  */
enum bitset_attr {BITSET_FIXED = 1,    /* Bitset size fixed.  */
		  BITSET_VARIABLE = 2, /* Bitset size variable.  */
		  BITSET_DENSE = 4,    /* Bitset dense.  */
		  BITSET_SPARSE = 8,   /* Bitset sparse.  */
		  BITSET_FRUGAL = 16,  /* Prefer most compact.  */
		  BITSET_GREEDY = 32}; /* Prefer fastest at memory expense.  */

typedef unsigned int bitset_attrs;

/* The contents of the union should be considered to be private.
   While I would like to make this union opaque, it needs to be
   visible for the inline bit set/test functions, and for delegation
   to the proper implementation.  */
union bitset_union
{
  /* This must be the first member of every other structure that is a
     member of this union.  */
  struct bbitset_struct b;

  struct abitset_struct
  {
    struct bbitset_struct b;
    bitset_bindex n_bits;		/* Number of bits.  */
    bitset_word words[1];		/* The array of bits.  */
  } a;

  struct ebitset_struct
  {
    struct bbitset_struct b;
    bitset_windex size;			/* Number of elements.  */
    struct ebitset_elt_struct **elts;	/* Expanding array of ptrs to elts.  */
  } e;

  struct lbitset_struct
  {
    struct bbitset_struct b;
    struct lbitset_elt_struct *head;	/* First element in linked list.  */
    struct lbitset_elt_struct *tail;	/* Last element in linked list.  */
  } l;

  struct bitset_stats_struct
  {
    struct bbitset_struct b;
    bitset bset;
  } s;
};


/* The contents of this structure should be considered private.
   It is used for iterating over set bits.  */
typedef struct
{
  bitset_bindex list[BITSET_LIST_SIZE];
  bitset_bindex next;
  bitset_bindex num;
  bitset_bindex i;
} bitset_iterator;


/* Return bytes required for bitset of desired type and size.  */
extern size_t bitset_bytes PARAMS ((enum_bitset_type, bitset_bindex));

/* Initialise a bitset with desired type and size.  */
extern bitset bitset_init PARAMS ((bitset, bitset_bindex, enum_bitset_type));

/* Select an implementation type based on the desired bitset size
   and attributes.  */
extern enum bitset_type bitset_type_choose PARAMS ((bitset_bindex,
						    bitset_attrs));

/* Create a bitset of desired type and size.  The bitset is zeroed.  */
extern bitset bitset_alloc PARAMS ((bitset_bindex, enum_bitset_type));

/* Free bitset.  */
extern void bitset_free PARAMS ((bitset));

/* Create a bitset of desired type and size using an obstack.  The
   bitset is zeroed.  */
extern bitset bitset_obstack_alloc PARAMS ((struct obstack *bobstack,
					    bitset_bindex, enum_bitset_type));

/* Free bitset allocated on obstack.  */
extern void bitset_obstack_free PARAMS ((bitset));

/* Create a bitset of desired size and attributes.  The bitset is zeroed.  */
extern bitset bitset_create PARAMS ((bitset_bindex, bitset_attrs));

/* Return bitset type.  */
extern enum bitset_type bitset_type_get PARAMS ((bitset));

/* Return bitset type name.  */
extern const char *bitset_type_name_get PARAMS ((bitset));

#if BITSET_INLINE
static inline void bitset_set PARAMS ((bitset, bitset_bindex));
static inline void bitset_reset PARAMS ((bitset, bitset_bindex));
static inline int bitset_test PARAMS ((bitset, bitset_bindex));

/* Set bit BITNO in bitset BSET.  */
static inline void bitset_set (bset, bitno)
    bitset bset;
    bitset_bindex bitno;
{
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->b.cindex;
  
  if (offset < bset->b.csize)
    bset->b.cdata[offset] |= ((bitset_word) 1 << (bitno % BITSET_WORD_BITS));
  else
    BITSET_SET_ (bset, bitno);
}


/* Reset bit BITNO in bitset BSET.  */
static inline void bitset_reset (bset, bitno)
    bitset bset;
    bitset_bindex bitno;
{
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->b.cindex;
  
  if (offset < bset->b.csize)
    bset->b.cdata[offset] &= ~((bitset_word) 1 << (bitno % BITSET_WORD_BITS));
  else
    BITSET_RESET_ (bset, bitno);
}


/* Test bit BITNO in bitset BSET.  */
static inline int bitset_test (bset, bitno)
    bitset bset;
    bitset_bindex bitno;
{
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->b.cindex;
  
  if (offset < bset->b.csize)
    return (bset->b.cdata[offset] >> (bitno % BITSET_WORD_BITS)) & 1;
  else
    return BITSET_TEST_ (bset, bitno);
}
#endif

#if ! BITSET_INLINE

/* Set bit BITNO in bitset BSET.  */
#define bitset_set(bset, bitno)					\
do   								\
{    								\
  bitset_bindex _bitno = (bitno);				\
  bitset_windex _index = _bitno / BITSET_WORD_BITS; 		\
  bitset_windex _offset = _index - (bset)->b.cindex;		\
  								\
  if (_offset < (bset)->b.csize)					\
    (bset)->b.cdata[_offset] |=					\
      ((bitset_word) 1 << (_bitno % BITSET_WORD_BITS)); 	\
  else  							\
    BITSET_SET_ ((bset), _bitno);				\
} while (0)


/* Reset bit BITNO in bitset BSET.  */
#define bitset_reset(bset, bitno)				\
do   								\
{    								\
  bitset_bindex _bitno = (bitno);				\
  bitset_windex _index = _bitno / BITSET_WORD_BITS; 		\
  bitset_windex _offset = _index - (bset)->b.cindex;		\
  								\
  if (_offset < (bset)->b.csize)				\
    (bset)->b.cdata[_offset] &=					\
       ~((bitset_word) 1 << (_bitno % BITSET_WORD_BITS)); 	\
  else  							\
    BITSET_RESET_ ((bset), _bitno);				\
} while (0)


/* Test bit BITNO in bitset BSET.  */
#define bitset_test(bset, bitno) \
(((((bitno) / BITSET_WORD_BITS) - (bset)->b.cindex) < (bset)->b.csize)  \
  ? ((bset)->b.cdata[(((bitno) / BITSET_WORD_BITS) - (bset)->b.cindex)] \
     >> ((bitno) % BITSET_WORD_BITS)) & 1 \
  : (unsigned int) BITSET_TEST_ ((bset), (bitno)))
#endif


/* Toggle bit BITNO in bitset BSET and return non-zero if now set.  */
#define bitset_toggle(bset, bitno) BITSET_TOGGLE_ (bset, bitno)

/* Return size in bits of bitset SRC.  */
#define bitset_size(SRC) BITSET_SIZE_ (SRC)

/* Return number of bits set in bitset SRC.  */
#define bitset_count(SRC) BITSET_COUNT_ (SRC)


/* Return SRC == 0.  */
#define bitset_empty_p(SRC) BITSET_EMPTY_P_ (SRC)

/* DST = ~0.  */
#define bitset_ones(DST) BITSET_ONES_ (DST)

/* DST = 0.  */
#define bitset_zero(DST) BITSET_ZERO_ (DST)



/* DST = SRC.  */
#define bitset_copy(DST, SRC) BITSET_COPY_ (DST, SRC)

/* Return DST & SRC == 0.  */
#define bitset_disjoint_p(DST, SRC) BITSET_DISJOINT_P_ (DST, SRC)

/* Return DST == SRC.  */
#define bitset_equal_p(DST, SRC) BITSET_EQUAL_P_ (DST, SRC)

/* DST = ~SRC.  */
#define bitset_not(DST, SRC) BITSET_NOT_ (DST, SRC)

/* Return DST == DST | SRC.  */
#define bitset_subset_p(DST, SRC) BITSET_SUBSET_P_ (DST, SRC)



/* DST = SRC1 & SRC2.  */
#define bitset_and(DST, SRC1, SRC2) BITSET_AND_ (DST, SRC1, SRC2)

/* DST = SRC1 & SRC2.  Return non-zero if DST != SRC1 & SRC2.  */
#define bitset_and_cmp(DST, SRC1, SRC2) BITSET_AND_CMP_ (DST, SRC1, SRC2)

/* DST = SRC1 & ~SRC2.  */
#define bitset_andn(DST, SRC1, SRC2) BITSET_ANDN_ (DST, SRC1, SRC2)

/* DST = SRC1 & ~SRC2.  Return non-zero if DST != SRC1 & ~SRC2.  */
#define bitset_andn_cmp(DST, SRC1, SRC2) BITSET_ANDN_CMP_ (DST, SRC1, SRC2)

/* DST = SRC1 | SRC2.  */
#define bitset_or(DST, SRC1, SRC2) BITSET_OR_ (DST, SRC1, SRC2)

/* DST = SRC1 | SRC2.  Return non-zero if DST != SRC1 | SRC2.  */
#define bitset_or_cmp(DST, SRC1, SRC2) BITSET_OR_CMP_ (DST, SRC1, SRC2)

/* DST = SRC1 ^ SRC2.  */
#define bitset_xor(DST, SRC1, SRC2) BITSET_XOR_ (DST, SRC1, SRC2)

/* DST = SRC1 ^ SRC2.  Return non-zero if DST != SRC1 ^ SRC2.  */
#define bitset_xor_cmp(DST, SRC1, SRC2) BITSET_XOR_CMP_ (DST, SRC1, SRC2)



/* DST = (SRC1 & SRC2) | SRC3.  */
#define bitset_and_or(DST, SRC1, SRC2, SRC3) \
 BITSET_AND_OR_ (DST, SRC1, SRC2, SRC3)

/* DST = (SRC1 & SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & SRC2) | SRC3.  */
#define bitset_and_or_cmp(DST, SRC1, SRC2, SRC3) \
 BITSET_AND_OR_CMP_ (DST, SRC1, SRC2, SRC3)

/* DST = (SRC1 & ~SRC2) | SRC3.  */
#define bitset_andn_or(DST, SRC1, SRC2, SRC3) \
 BITSET_ANDN_OR_ (DST, SRC1, SRC2, SRC3)

/* DST = (SRC1 & ~SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & ~SRC2) | SRC3.  */
#define bitset_andn_or_cmp(DST, SRC1, SRC2, SRC3) \
 BITSET_ANDN_OR_CMP_ (DST, SRC1, SRC2, SRC3)

/* DST = (SRC1 | SRC2) & SRC3.  */
#define bitset_or_and(DST, SRC1, SRC2, SRC3)\
 BITSET_OR_AND_ (DST, SRC1, SRC2, SRC3)

/* DST = (SRC1 | SRC2) & SRC3.  Return non-zero if
   DST != (SRC1 | SRC2) & SRC3.  */
#define bitset_or_and_cmp(DST, SRC1, SRC2, SRC3)\
 BITSET_OR_AND_CMP_ (DST, SRC1, SRC2, SRC3)

/* Find list of up to NUM bits set in BSET starting from and including 
   *NEXT.  Return with actual number of bits found and with *NEXT
   indicating where search stopped.  */
#define bitset_list(BSET, LIST, NUM, NEXT) \
 BITSET_LIST_ (BSET, LIST, NUM, NEXT) 

/* Find reverse list of up to NUM bits set in BSET starting from and
   including NEXT.  Return with actual number of bits found and with
   *NEXT indicating where search stopped.  */
#define bitset_list_reverse(BSET, LIST, NUM, NEXT) \
 BITSET_LIST_REVERSE_ (BSET, LIST, NUM, NEXT) 


/* Find next set bit.  */
extern bitset_bindex bitset_next PARAMS ((bitset, bitset_bindex));

/* Find previous set bit.  */
extern bitset_bindex bitset_prev PARAMS ((bitset, bitset_bindex));

/* Find first set bit.  */
extern bitset_bindex bitset_first PARAMS ((bitset));

/* Find last set bit.  */
extern bitset_bindex bitset_last PARAMS ((bitset));

/* Return nonzero if this is the only set bit.  */
extern int bitset_only_set_p PARAMS ((bitset, bitset_bindex));

/* Dump bitset.  */
extern void bitset_dump PARAMS ((FILE *, bitset));

/* Loop over all elements of BSET, starting with MIN, setting BIT
   to the index of each set bit.  For example, the following will print
   the bits set in a bitset:

   bitset_bindex i;
   bitset_iterator iter;

   bitset_zero (dst);
   BITSET_FOR_EACH (iter, src, i, 0)
   {
      printf ("%ld ", i);
   };
*/
#define BITSET_FOR_EACH(ITER, BSET, BIT, MIN)				      \
  for (ITER.next = (MIN), ITER.num = BITSET_LIST_SIZE;			      \
       (ITER.num == BITSET_LIST_SIZE) 					      \
       && (ITER.num = bitset_list (BSET, ITER.list, 			      \
                                   BITSET_LIST_SIZE, &ITER.next));)	      \
    for (ITER.i = 0; (BIT) = ITER.list[ITER.i], ITER.i < ITER.num; ITER.i++)


/* Loop over all elements of BSET, in reverse order starting with
   MIN,  setting BIT to the index of each set bit. For example, the 
   following will print the bits set in a bitset in reverse order:

   bitset_bindex i;
   bitset_iterator iter;

   bitset_zero (dst);
   BITSET_FOR_EACH_REVERSE (iter, src, i, 0)
   {
      printf ("%ld ", i);
   };
*/
#define BITSET_FOR_EACH_REVERSE(ITER, BSET, BIT, MIN)			      \
  for (ITER.next = (MIN), ITER.num = BITSET_LIST_SIZE;			      \
       (ITER.num == BITSET_LIST_SIZE) 					      \
       && (ITER.num = bitset_list_reverse (BSET, ITER.list,		      \
                                          BITSET_LIST_SIZE, &ITER.next));)    \
    for (ITER.i = 0; (BIT) = ITER.list[ITER.i], ITER.i < ITER.num; ITER.i++)


/* Define set operations in terms of logical operations.  */

#define bitset_diff(DST, SRC1, SRC2) bitset_andn (DST, SRC1, SRC2) 
#define bitset_diff_cmp(DST, SRC1, SRC2) bitset_andn_cmp (DST, SRC1, SRC2) 

#define bitset_intersection(DST, SRC1, SRC2) bitset_and (DST, SRC1, SRC2) 
#define bitset_intersection_cmp(DST, SRC1, SRC2) bitset_and_cmp (DST, SRC1, SRC2) 

#define bitset_union(DST, SRC1, SRC2) bitset_or (DST, SRC1, SRC2) 
#define bitset_union_cmp(DST, SRC1, SRC2) bitset_or_cmp (DST, SRC1, SRC2) 

/* Symmetrical difference.  */
#define bitset_symdiff(DST, SRC1, SRC2) bitset_xor (DST, SRC1, SRC2) 
#define bitset_symdiff_cmp(DST, SRC1, SRC2) bitset_xor_cmp (DST, SRC1, SRC2) 

/* Union of difference.  */
#define bitset_diff_union(DST, SRC1, SRC2, SRC3) \
  bitset_andn_or (DST, SRC1, SRC2, SRC3) 
#define bitset_diff_union_cmp(DST, SRC1, SRC2, SRC3) \
  bitset_andn_or_cmp (DST, SRC1, SRC2, SRC3) 



/* Release any memory tied up with bitsets.  */
extern void bitset_release_memory PARAMS ((void));

/* Enable bitset stats gathering.  */
extern void bitset_stats_enable PARAMS ((void));

/* Disable bitset stats gathering.  */
extern void bitset_stats_disable PARAMS ((void));

/* Read bitset stats file of accummulated stats.  */
void bitset_stats_read PARAMS ((const char *filename));

/* Write bitset stats file of accummulated stats.  */
void bitset_stats_write PARAMS ((const char *filename));

/* Dump bitset stats.  */
extern void bitset_stats_dump PARAMS ((FILE *));

/* Function to debug bitset from debugger.  */
extern void debug_bitset PARAMS ((bitset));

/* Function to debug bitset stats from debugger.  */
extern void debug_bitset_stats PARAMS ((void));

#endif /* _BITSET_H  */

