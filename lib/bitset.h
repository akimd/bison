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

#if HAVE_CONFIG_H
# include <config.h>
#endif

#define BITSET_STATS 1

# ifndef PARAMS
#  if defined PROTOTYPES || (defined __STDC__ && __STDC__)
#   define PARAMS(Args) Args
#  else
#   define PARAMS(Args) ()
#  endif
# endif

# ifndef __attribute__
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 8) || __STRICT_ANSI__
#   define __attribute__(x)
#  endif
# endif

# ifndef ATTRIBUTE_NORETURN
#  define ATTRIBUTE_NORETURN __attribute__ ((__noreturn__))
# endif

# ifndef ATTRIBUTE_UNUSED
#  define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# endif

#include "bitset-int.h"
#include "sbitset.h"
#include "lbitset.h"
#include "ebitset.h"
#include "obstack.h"
#include <stdio.h>
#include "xalloc.h"

enum bitset_attr {BITSET_FIXED = 1,    /* Bitset size fixed.  */
		  BITSET_VARIABLE = 2, /* Bitset size variable.  */
		  BITSET_DENSE = 4,    /* Bitset dense.  */
		  BITSET_SPARSE = 8,   /* Bitset sparse.  */
		  BITSET_FRUGAL = 16,  /* Prefer most compact.  */
		  BITSET_GREEDY = 32}; /* Prefer fastest.  */

typedef unsigned int bitset_attrs;

/* The elements of these structures should be considered
   to be private.  */
typedef struct bitset_struct
{
  struct bitset_ops_struct *ops;
  bitset_windex cindex;		/* Cache word index.  */
  bitset_windex csize;		/* Cache size in words.  */
  bitset_word *cdata;		/* Cache data pointer.  */
  union bitset_data
  {
    struct sbitset_struct s;
    struct lbitset_struct l;
    struct ebitset_struct e;
  } u;
} *bitset;


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


/* Return bytes required for bitset of desired type and size.  */
extern int bitset_bytes PARAMS ((enum bitset_type, bitset_bindex));

/* Initialise a bitset with desired type and size.  */
extern bitset bitset_init PARAMS ((bitset, bitset_bindex, enum bitset_type));

extern enum bitset_type bitset_type_choose PARAMS ((bitset_bindex,
						    bitset_attrs));

/* Create a bitset of desired type and size.  */
extern bitset bitset_alloc PARAMS ((bitset_bindex, enum bitset_type));

/* Free bitset.  */
extern void bitset_free PARAMS ((bitset));

/* Create a bitset of desired type and size using obstack.  */
extern bitset bitset_obstack_alloc PARAMS ((struct obstack *bobstack,
					    bitset_bindex, enum bitset_type));

/* Free bitset allocated on obstack.  */
extern void bitset_obstack_free PARAMS ((bitset));

/* Create a bitset of desired size and attributes.  */
extern bitset bitset_create PARAMS ((bitset_bindex, bitset_attrs));

/* Return size in bits of bitset SRC.  */
extern int bitset_size PARAMS ((bitset));

#if BITSET_CACHE && BITSET_INLINE
static inline void bitset_set PARAMS ((bitset, bitset_bindex));
static inline void bitset_reset PARAMS ((bitset, bitset_bindex));
static inline int bitset_test PARAMS ((bitset, bitset_bindex));

/* Set bit BITNO in bitset BSET.  */
static inline void bitset_set (bset, bitno)
    bitset bset;
    bitset_bindex bitno;
{
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->cindex;

  if (offset < bset->csize)
    bset->cdata[offset] |= (1 << (bitno % BITSET_WORD_BITS));
  else
    BITSET__SET (bset, bitno);
}


/* Reset bit BITNO in bitset BSET.  */
static inline void bitset_reset (bset, bitno)
    bitset bset;
    bitset_bindex bitno;
{
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->cindex;

  if (offset < bset->csize)
    bset->cdata[offset] &= ~(1 << (bitno % BITSET_WORD_BITS));
  else
    BITSET__RESET (bset, bitno);
}


/* Test bit BITNO in bitset BSET.  */
static inline int bitset_test (bset, bitno)
    bitset bset;
    bitset_bindex bitno;
{
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->cindex;

  if (offset < bset->csize)
    return (bset->cdata[offset] >> (bitno % BITSET_WORD_BITS)) & 1;
  else
    return BITSET__TEST (bset, bitno);
}
#endif

#if BITSET_CACHE && ! BITSET_INLINE

/* Set bit BITNO in bitset BSET.  */
#define bitset_set(bset, bitno)					\
do   								\
{    								\
  bitset_bindex _bitno = (bitno);				\
  bitset_windex _index = _bitno / BITSET_WORD_BITS; 		\
  bitset_windex _offset = _index - (bset)->cindex;		\
  								\
  if (_offset < (bset)->csize)					\
    (bset)->cdata[_offset] |= (1 << (_bitno % BITSET_WORD_BITS)); 	\
  else  							\
    BITSET__SET ((bset), _bitno);				\
} while (0)


/* Reset bit BITNO in bitset BSET.  */
#define bitset_reset(bset, bitno)				\
do   								\
{    								\
  bitset_bindex _bitno = (bitno);				\
  bitset_windex _index = _bitno / BITSET_WORD_BITS; 		\
  bitset_windex _offset = _index - (bset)->cindex;		\
  								\
  if (_offset < (bset)->csize)					\
    (bset)->cdata[_offset] &= ~(1 << (_bitno % BITSET_WORD_BITS)); 	\
  else  							\
    BITSET__RESET ((bset), _bitno);				\
} while (0)


/* Test bit BITNO in bitset BSET.  */
#define bitset_test(bset, bitno) \
(((((bitno) / BITSET_WORD_BITS) - (bset)->cindex) < (bset)->csize) \
  ? ((bset)->cdata[(((bitno) / BITSET_WORD_BITS) - (bset)->cindex)] \
     >> ((bitno) % BITSET_WORD_BITS)) & 1 \
  : (unsigned int) BITSET__TEST ((bset), (bitno)))
#endif

#if ! BITSET_CACHE
/* Set bit BITNO in bitset SRC.  */
#define bitset_set(SRC, BITNO) BITSET__SET (SRC, BITNO)

/* Reset bit BITNO in bitset SRC.  */
#define bitset_reset(SRC, BITNO) BITSET__RESET (SRC, BITNO)

/* Return non-zero if bit BITNO in bitset SRC is set.  */
#define bitset_test(SRC, BITNO) BITSET__TEST (SRC, BITNO)
#endif

/* DST = 0.  */
extern int bitset_zero PARAMS ((bitset));

/* DST = ~0.  */
extern int bitset_ones PARAMS ((bitset));

/* Return non-zero if all bits in bitset SRC are reset.  */
extern int bitset_empty_p PARAMS ((bitset));

/* Return DST == DST | SRC.  */
extern int bitset_subset_p PARAMS ((bitset, bitset));

/* Return DST == SRC.  */
extern int bitset_equal_p PARAMS ((bitset, bitset));

/* DST == SRC.  */
extern int bitset_copy PARAMS ((bitset, bitset));

/* DST = ~SRC.  */
extern int bitset_not PARAMS ((bitset, bitset));

/* DST = SRC1 | SRC2.  Return non-zero if DST != SRC1 | SRC2.  */
extern int bitset_or PARAMS ((bitset, bitset, bitset));

/* DST = SRC1 & SRC2.  Return non-zero if DST != SRC1 & SRC2.  */
extern int bitset_and PARAMS ((bitset, bitset, bitset));

/* DST = SRC1 ^ SRC2.  Return non-zero if DST != SRC1 ^ SRC2.  */
extern int bitset_xor PARAMS ((bitset, bitset, bitset));

/* DST = SRC1 & ~SRC2.  Return non-zero if DST != SRC1 & ~SRC2.  */
extern int bitset_andn PARAMS ((bitset, bitset, bitset));

/* DST = SRC1 | ~SRC2.  Return non-zero if DST != SRC1 | ~SRC2.  */
extern int bitset_orn PARAMS ((bitset, bitset, bitset));

/* DST = (SRC1 | SRC2) & SRC3.  Return non-zero if
   DST != (SRC1 | SRC2) & SRC3.  */
extern int bitset_or_and PARAMS ((bitset, bitset, bitset, bitset));

/* DST = (SRC1 & SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & SRC2) | SRC3.  */
extern int bitset_and_or PARAMS ((bitset, bitset, bitset, bitset));

/* DST = (SRC1 & ~SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & ~SRC2) | SRC3.  */
#define bitset_andn_or(DST, SRC1, SRC2, SRC3) \

/* Find next bit set in BSET starting from and including BITNO.  */
extern int bitset_next PARAMS ((bitset, bitset_bindex));

/* Find previous bit set in BSET starting from and including BITNO.  */
extern int bitset_prev PARAMS ((bitset, bitset_bindex));

/* Find list of up to NUM bits set in BSET starting from and including
   *NEXT.  Return with actual number of bits found and with *NEXT
   indicating where search stopped.  */
#if BITSET_STATS
extern int bitset_list PARAMS ((bitset, bitset_bindex *, bitset_bindex,
				bitset_bindex *));
#else
#define bitset_list(BSET, LIST, NUM, NEXT) \
BITSET__LIST (BSET, LIST, NUM, NEXT)
#endif

/* Find reverse list of up to NUM bits set in BSET starting from and
   including NEXT.  Return with actual number of bits found and with
   *NEXT indicating where search stopped.  */
#define bitset_reverse_list(BSET, LIST, NUM, NEXT) \
BITSET__REVERSE_LIST (BSET, LIST, NUM, NEXT)

/* Find first set bit.  */
extern int bitset_first PARAMS ((bitset));

/* Find last set bit.  */
extern int bitset_last PARAMS ((bitset));

/* Dump bitset.  */
extern void bitset_dump PARAMS ((FILE *, bitset));

/* Loop over all elements of BSET, starting with MIN, executing CODE.  */
#define BITSET_EXECUTE(BSET, MIN, N, CODE)				\
do {									\
  bitset_bindex _list[BITSET_LIST_SIZE];				\
  bitset_bindex _next = (MIN);						\
  int _num;								\
    									\
  while ((_num = bitset_list ((BSET), _list, BITSET_LIST_SIZE, &_next)))\
    {									\
       int _i;								\
									\
       for (_i = 0; _i < _num; _i++)					\
	 {								\
	    (N) = _list[_i];						\
            CODE;							\
	 }								\
       if (_num < BITSET_LIST_SIZE)					\
         break;								\
    }									\
} while (0)


/* Loop over all elements of BSET, in reverse order starting with
   MIN, executing CODE.  */
#define BITSET_REVERSE_EXECUTE(BSET, MIN, N, CODE)			\
do {									\
  bitset_bindex _list[BITSET_LIST_SIZE];				\
  bitset_bindex _next = (MIN);						\
  int _num;								\
    									\
  while ((_num = bitset_reverse_list ((BSET), _list, 			\
                                      BITSET_LIST_SIZE, &_next)))	\
    {									\
       int _i;								\
									\
       for (_i = 0; _i < _num; _i++)					\
	 {								\
	    (N) = _list[_i];						\
            CODE;							\
	 }								\
       if (_num < BITSET_LIST_SIZE)					\
         break;								\
    }									\
} while (0)


/* Define set operations in terms of bit operations.  */

#define bitset_diff(DST, SRC1, SRC2)  bitset_andn (DST, SRC1, SRC2)

#define bitset_intersection(DST, SRC1, SRC2)  bitset_and (DST, SRC1, SRC2)

#define bitset_union(DST, SRC1, SRC2)  bitset_or (DST, SRC1, SRC2)

#define bitset_diff_union(DST, SRC1, SRC2, SRC3) \
bitset_andn_or (DST, SRC1, SRC2, SRC3)

/* Release any memory tied up with bitsets.  */
extern void bitset_release_memory PARAMS ((void));

/* Initialise bitset stats.  */
extern void bitset_stats_init PARAMS ((void));

/* Dump bitset stats.  */
extern void bitset_stats_dump PARAMS ((FILE *));

/* Function to debug bitset from debugger.  */
extern void debug_bitset PARAMS ((bitset));

/* Function to debug bitset stats from debugger.  */
extern void debug_bitset_stats PARAMS ((void));

extern bitset sbitset_init PARAMS ((bitset, bitset_bindex));

extern bitset lbitset_init PARAMS ((bitset, bitset_bindex));

extern bitset ebitset_init PARAMS ((bitset, bitset_bindex));

#endif /* _BITSET_H  */
