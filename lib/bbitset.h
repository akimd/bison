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
   BITSET_ARRAY:  Array of bits (fixed size, fast for dense bitsets).
   BITSET_LIST:   Linked list of array of bits (variable size, least storage
		  for large very sparse sets).
   BITSET_TABLE:  Expandable table of pointers to array of bits
		  (variable size, less storage for large sparse sets).

   BITSET_STATS:  Wrapper bitset for internal use only.
*/
enum bitset_type {BITSET_ARRAY, BITSET_LIST, BITSET_TABLE, BITSET_TYPE_NUM,
		  BITSET_STATS};
#define BITSET_TYPE_NAMES {"abitset", "lbitset", "ebitset"}

extern const char * const bitset_type_names[];

enum bitset_alloc_type {BITSET_MALLOC, BITSET_OBALLOC};

/* Data type used to store a word of bits.  */
typedef unsigned long bitset_word;
#define BITSET_WORD_BITS ((unsigned) (CHAR_BIT * sizeof (bitset_word)))

/* Bit index.  In theory we might need a type wider than size_t, but
   in practice we lose at most a factor of CHAR_BIT by going with
   size_t, and that is good enough.  If this type is changed to be
   wider than size_t, the code needs to be modified to check for
   overflow when converting bit counts to byte or word counts.
   The bit and word index types must be unsigned.  */
typedef size_t bitset_bindex;

/* Word index.  */
typedef size_t bitset_windex;

/* Maximum values for commonly-used unsigned types.  BITSET_SIZE_MAX
   always equals SIZE_MAX, but some older systems lack SIZE_MAX.  */
#define BITSET_BINDEX_MAX ((bitset_bindex) -1)

/* Limit max word index to the maximum value of a signed integer
   to simplify cache disabling.  */
#define BITSET_WINDEX_MAX (((bitset_windex) -1) >> 1)
#define BITSET_SIZE_MAX ((size_t) -1)

#define BITSET_MSB ((bitset_word) 1 << (BITSET_WORD_BITS - 1))

#define BITSET_LIST_SIZE 1024

enum bitset_ops {BITSET_OP_ZERO, BITSET_OP_ONES,
		 BITSET_OP_COPY, BITSET_OP_NOT,
		 BITSET_OP_EMPTY_P, BITSET_OP_EQUAL_P,
		 BITSET_OP_SUBSET_P, BITSET_OP_DISJOINT_P,
		 BITSET_OP_AND, BITSET_OP_OR, BITSET_OP_XOR, BITSET_OP_ANDN,
		 BITSET_OP_OR_AND, BITSET_OP_AND_OR, BITSET_OP_ANDN_OR};

struct bbitset_struct
{
  const struct bitset_vtable * vtable;
  bitset_windex cindex;		/* Cache word index.  */
  bitset_windex csize;		/* Cache size in words.  */
  bitset_word *cdata;		/* Cache data pointer.  */
  /* Perhaps we could sacrifice another word to indicate
     that the bitset is known to be zero, that a bit has been set
     in the cache, and that a bit has been cleared in the cache.
     This would speed up some of the searches but slightly slow down
     bit set/reset operations of cached bits.  */
};


typedef union bitset_union *bitset;


/* The contents of this structure should be considered private.  */
struct bitset_vtable
{
  void (*set) PARAMS ((bitset, bitset_bindex));
  void (*reset) PARAMS ((bitset, bitset_bindex));
  int (*toggle) PARAMS ((bitset, bitset_bindex));
  int (*test) PARAMS ((bitset, bitset_bindex));
  bitset_bindex (*size) PARAMS ((bitset));
  bitset_bindex (*count) PARAMS ((bitset));

  int (*empty_p) PARAMS ((bitset));
  void (*ones) PARAMS ((bitset));
  void (*zero) PARAMS ((bitset));

  void (*copy) PARAMS ((bitset, bitset));
  int (*disjoint_p) PARAMS ((bitset, bitset));
  int (*equal_p) PARAMS ((bitset, bitset));
  void (*not) PARAMS ((bitset, bitset));
  int (*subset_p) PARAMS ((bitset, bitset));

  void (*and) PARAMS ((bitset, bitset, bitset));
  int (*and_cmp) PARAMS ((bitset, bitset, bitset));
  void (*andn) PARAMS ((bitset, bitset, bitset));
  int (*andn_cmp) PARAMS ((bitset, bitset, bitset));
  void (*or) PARAMS ((bitset, bitset, bitset));
  int (*or_cmp) PARAMS ((bitset, bitset, bitset));
  void (*xor) PARAMS ((bitset, bitset, bitset));
  int (*xor_cmp) PARAMS ((bitset, bitset, bitset));

  void (*and_or) PARAMS ((bitset, bitset, bitset, bitset));
  int (*and_or_cmp) PARAMS ((bitset, bitset, bitset, bitset));
  void (*andn_or) PARAMS ((bitset, bitset, bitset, bitset));
  int (*andn_or_cmp) PARAMS ((bitset, bitset, bitset, bitset));
  void (*or_and) PARAMS ((bitset, bitset, bitset, bitset));
  int (*or_and_cmp) PARAMS ((bitset, bitset, bitset, bitset));

  bitset_bindex (*list) PARAMS ((bitset, bitset_bindex *,
				 bitset_bindex, bitset_bindex *));
  bitset_bindex (*list_reverse) PARAMS ((bitset,
					 bitset_bindex *, bitset_bindex,
					 bitset_bindex *));
  void (*free) PARAMS ((bitset));
  enum bitset_type type;
};

#define BITSET_COMPATIBLE_(BSET1, BSET2) ((BSET1)->b.vtable == (BSET2)->b.vtable)

#define BITSET_CHECK2_(DST, SRC) \
if (!BITSET_COMPATIBLE_ (DST, SRC)) abort ();

#define BITSET_CHECK3_(DST, SRC1, SRC2) \
if (!BITSET_COMPATIBLE_ (DST, SRC1) \
    || !BITSET_COMPATIBLE_ (DST, SRC2)) abort ();

#define BITSET_CHECK4_(DST, SRC1, SRC2, SRC3) \
if (!BITSET_COMPATIBLE_ (DST, SRC1) || !BITSET_COMPATIBLE_ (DST, SRC2) \
    || !BITSET_COMPATIBLE_ (DST, SRC3)) abort ();


/* Return size in bits of bitset SRC.  */
#define BITSET_SIZE_(SRC) (SRC)->b.vtable->size (SRC)

/* Return number of bits set in bitset SRC.  */
#define BITSET_COUNT_(SRC) (SRC)->b.vtable->count (SRC)

/* Return type of bitset SRC.  */
#define BITSET_TYPE_(DST) (DST)->b.vtable->type

/* Set bit BITNO in bitset DST.  */
#define BITSET_SET_(DST, BITNO) (DST)->b.vtable->set (DST, BITNO)

/* Reset bit BITNO in bitset DST.  */
#define BITSET_RESET_(DST, BITNO) (DST)->b.vtable->reset (DST, BITNO)

/* Toggle bit BITNO in bitset DST.  */
#define BITSET_TOGGLE_(DST, BITNO) (DST)->b.vtable->toggle (DST, BITNO)

/* Return non-zero if bit BITNO in bitset SRC is set.  */
#define BITSET_TEST_(SRC, BITNO) (SRC)->b.vtable->test (SRC, BITNO)

/* Free bitset SRC.  */
#define BITSET_FREE_(SRC)\
 ((SRC)->b.vtable->free ? (SRC)->b.vtable->free (SRC) :(void)0)


/* Return SRC == 0.  */
#define BITSET_EMPTY_P_(SRC) (SRC)->b.vtable->empty_p (SRC)

/* DST = ~0.  */
#define BITSET_ONES_(DST) (DST)->b.vtable->ones (DST)

/* DST = 0.  */
#define BITSET_ZERO_(DST) (DST)->b.vtable->zero (DST)



/* DST = SRC.  */
#define BITSET_COPY_(DST, SRC) (SRC)->b.vtable->copy (DST, SRC)

/* Return DST & SRC == 0.  */
#define BITSET_DISJOINT_P_(DST, SRC) (SRC)->b.vtable->disjoint_p (DST, SRC)

/* Return DST == SRC.  */
#define BITSET_EQUAL_P_(DST, SRC) (SRC)->b.vtable->equal_p (DST, SRC)

/* DST = ~SRC.  */
#define BITSET_NOT_(DST, SRC) (SRC)->b.vtable->not (DST, SRC)

/* Return DST == DST | SRC.  */
#define BITSET_SUBSET_P_(DST, SRC) (SRC)->b.vtable->subset_p (DST, SRC)


/* DST = SRC1 & SRC2.  */
#define BITSET_AND_(DST, SRC1, SRC2) (SRC1)->b.vtable->and (DST, SRC1, SRC2)
#define BITSET_AND_CMP_(DST, SRC1, SRC2) (SRC1)->b.vtable->and_cmp (DST, SRC1, SRC2)

/* DST = SRC1 & ~SRC2.  */
#define BITSET_ANDN_(DST, SRC1, SRC2) (SRC1)->b.vtable->andn (DST, SRC1, SRC2)
#define BITSET_ANDN_CMP_(DST, SRC1, SRC2) (SRC1)->b.vtable->andn_cmp (DST, SRC1, SRC2)

/* DST = SRC1 | SRC2.  */
#define BITSET_OR_(DST, SRC1, SRC2) (SRC1)->b.vtable->or (DST, SRC1, SRC2)
#define BITSET_OR_CMP_(DST, SRC1, SRC2) (SRC1)->b.vtable->or_cmp (DST, SRC1, SRC2)

/* DST = SRC1 ^ SRC2.  */
#define BITSET_XOR_(DST, SRC1, SRC2) (SRC1)->b.vtable->xor (DST, SRC1, SRC2)
#define BITSET_XOR_CMP_(DST, SRC1, SRC2) (SRC1)->b.vtable->xor_cmp (DST, SRC1, SRC2)



/* DST = (SRC1 & SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & SRC2) | SRC3.  */
#define BITSET_AND_OR_(DST, SRC1, SRC2, SRC3) \
 (SRC1)->b.vtable->and_or (DST, SRC1, SRC2, SRC3)
#define BITSET_AND_OR_CMP_(DST, SRC1, SRC2, SRC3) \
 (SRC1)->b.vtable->and_or_cmp (DST, SRC1, SRC2, SRC3)

/* DST = (SRC1 & ~SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & ~SRC2) | SRC3.  */
#define BITSET_ANDN_OR_(DST, SRC1, SRC2, SRC3) \
 (SRC1)->b.vtable->andn_or (DST, SRC1, SRC2, SRC3)
#define BITSET_ANDN_OR_CMP_(DST, SRC1, SRC2, SRC3) \
 (SRC1)->b.vtable->andn_or_cmp (DST, SRC1, SRC2, SRC3)

/* DST = (SRC1 | SRC2) & SRC3.  Return non-zero if
   DST != (SRC1 | SRC2) & SRC3.  */
#define BITSET_OR_AND_(DST, SRC1, SRC2, SRC3) \
 (SRC1)->b.vtable->or_and (DST, SRC1, SRC2, SRC3)
#define BITSET_OR_AND_CMP_(DST, SRC1, SRC2, SRC3) \
 (SRC1)->b.vtable->or_and_cmp (DST, SRC1, SRC2, SRC3)


/* Find list of up to NUM bits set in BSET starting from and including
   *NEXT.  Return with actual number of bits found and with *NEXT
   indicating where search stopped.  */
#define BITSET_LIST_(BSET, LIST, NUM, NEXT) \
 (BSET)->b.vtable->list (BSET, LIST, NUM, NEXT)

/* Find reverse list of up to NUM bits set in BSET starting from and
   including NEXT.  Return with actual number of bits found and with
   *NEXT indicating where search stopped.  */
#define BITSET_LIST_REVERSE_(BSET, LIST, NUM, NEXT) \
 (BSET)->b.vtable->list_reverse (BSET, LIST, NUM, NEXT)


/* Private functions for bitset implementations.  */

extern int bitset_toggle_ PARAMS ((bitset, bitset_bindex));

extern bitset_bindex bitset_count_ PARAMS ((bitset));

extern int bitset_copy_ PARAMS ((bitset, bitset));

extern void bitset_and_or_ PARAMS ((bitset, bitset, bitset, bitset));

extern int bitset_and_or_cmp_ PARAMS ((bitset, bitset, bitset, bitset));

extern void bitset_andn_or_ PARAMS ((bitset, bitset, bitset, bitset));

extern int bitset_andn_or_cmp_ PARAMS ((bitset, bitset, bitset, bitset));

extern void bitset_or_and_ PARAMS ((bitset, bitset, bitset, bitset));

extern int bitset_or_and_cmp_ PARAMS ((bitset, bitset, bitset, bitset));

#endif /* _BBITSET_H  */
