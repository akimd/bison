/* Simple bitsets.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bitset.h"
#include <stdlib.h>

# if WITH_DMALLOC
#  define DMALLOC_FUNC_CHECK
#  include <dmalloc.h>
# endif /* WITH_DMALLOC */

/* This file implements fixed size bitsets stored as an array
   of words.  Any unused bits in the last word must be zero.  */

static void sbitset_unused_clear PARAMS((bitset));

static int sbitset_small_list PARAMS((bitset, bitset_bindex *, bitset_bindex,
				      bitset_bindex *));

static void sbitset_set PARAMS((bitset, bitset_bindex));
static void sbitset_reset PARAMS((bitset, bitset_bindex));
static int sbitset_test PARAMS((bitset, bitset_bindex));
static int sbitset_size PARAMS((bitset));
static int sbitset_op1 PARAMS((bitset, enum bitset_ops));
static int sbitset_op2 PARAMS((bitset, bitset, enum bitset_ops));
static int sbitset_op3 PARAMS((bitset, bitset, bitset, enum bitset_ops));
static int sbitset_op4 PARAMS((bitset, bitset, bitset, bitset,
			       enum bitset_ops));
static int sbitset_list PARAMS((bitset, bitset_bindex *, bitset_bindex,
				bitset_bindex *));
static int sbitset_reverse_list PARAMS((bitset, bitset_bindex *, bitset_bindex,
					bitset_bindex *));

#define SBITSET_N_WORDS(N) (((N) + BITSET_WORD_BITS - 1) / BITSET_WORD_BITS)
#define SBITSET_WORDS(X) ((X)->u.s.words)
#define SBITSET_N_BITS(X) ((X)->u.s.n_bits)


/* Return size in bits of bitset SRC.  */
static int
sbitset_size (src)
     bitset src;
{
    return SBITSET_N_BITS (src);
}


/* Find list of up to NUM bits set in BSET starting from and including
   *NEXT and store in array LIST.  Return with actual number of bits
   found and with *NEXT indicating where search stopped.  */
static int
sbitset_small_list (src, list, num, next)
     bitset src;
     bitset_bindex *list;
     bitset_bindex num;
     bitset_bindex *next;
{
  bitset_bindex bitno;
  bitset_bindex count;
  bitset_windex size;
  bitset_word word;

  word = SBITSET_WORDS (src)[0];

  /* Short circuit common case.  */
  if (!word)
    return 0;

  size = SBITSET_N_BITS (src);
  bitno = *next;
  if (bitno >= size)
    return 0;

  word >>= bitno;

  /* If num is 1, we could speed things up with a binary search
     of the word of interest.  */

  if (num >= BITSET_WORD_BITS)
  {
      for (count = 0; word; bitno++)
      {
	  if (word & 1)
	      list[count++] = bitno;
	  word >>= 1;
      }
  }
  else
  {
      for (count = 0; word; bitno++)
      {
	  if (word & 1)
	  {
	      list[count++] = bitno;
	      if (count >= num)
	      {
		  bitno++;
		  break;
	      }
	  }
	  word >>= 1;
      }
  }

  *next = bitno;
  return count;
}


/* Set bit BITNO in bitset DST.  */
static void
sbitset_set (dst, bitno)
     bitset dst ATTRIBUTE_UNUSED;
     bitset_bindex bitno ATTRIBUTE_UNUSED;
{
    /* This should never occur for sbitsets.  */
    abort ();
}


/* Reset bit BITNO in bitset DST.  */
static void
sbitset_reset (dst, bitno)
     bitset dst ATTRIBUTE_UNUSED;
     bitset_bindex bitno ATTRIBUTE_UNUSED;
{
    /* This should never occur for sbitsets.  */
    abort ();
}


/* Test bit BITNO in bitset SRC.  */
static int
sbitset_test (src, bitno)
     bitset src ATTRIBUTE_UNUSED;
     bitset_bindex bitno ATTRIBUTE_UNUSED;
{
    /* This should never occur for sbitsets.  */
    abort ();
    return 0;
}


/* Find list of up to NUM bits set in BSET in reverse order, starting
   from and including NEXT and store in array LIST.  Return with
   actual number of bits found and with *NEXT indicating where search
   stopped.  */
static int
sbitset_reverse_list (src, list, num, next)
     bitset src;
     bitset_bindex *list;
     bitset_bindex num;
     bitset_bindex *next;
{
  bitset_bindex bitno;
  bitset_bindex rbitno;
  bitset_bindex count;
  bitset_windex index;
  unsigned int bitcnt;
  bitset_bindex bitoff;
  bitset_word word;
  bitset_word *srcp = SBITSET_WORDS (src);
  bitset_bindex n_bits = SBITSET_N_BITS (src);

  rbitno = *next;

  /* If num is 1, we could speed things up with a binary search
     of the word of interest.  */

  if (rbitno >= n_bits)
      return 0;

  count = 0;

  bitno = n_bits - (rbitno + 1);

  index = bitno / BITSET_WORD_BITS;
  bitcnt = bitno % BITSET_WORD_BITS;
  bitoff = index * BITSET_WORD_BITS;

  for (; index != ~0U; index--, bitoff -= BITSET_WORD_BITS,
	   bitcnt = BITSET_WORD_BITS - 1)
    {
      word = srcp[index] << (BITSET_WORD_BITS - 1 - bitcnt);
      for (; word; bitcnt--)
	{
	  if (word & BITSET_MSB)
	    {
	      list[count++] = bitoff + bitcnt;
	      if (count >= num)
		{
		  *next = n_bits - (bitoff + bitcnt);
		  return count;
		}
	    }
	  word <<= 1;
	}
    }

  *next = n_bits - (bitoff + 1);
  return count;
}


/* Find list of up to NUM bits set in BSET starting from and including
   *NEXT and store in array LIST.  Return with actual number of bits
   found and with *NEXT indicating where search stopped.  */
static int
sbitset_list (src, list, num, next)
     bitset src;
     bitset_bindex *list;
     bitset_bindex num;
     bitset_bindex *next;
{
  bitset_bindex bitno;
  bitset_bindex count;
  bitset_windex index;
  bitset_bindex bitoff;
  bitset_windex size = src->csize;
  bitset_word *srcp = SBITSET_WORDS (src);
  bitset_word word;

  bitno = *next;

  count = 0;
  if (! bitno)
    {
      /* Many bitsets are zero, so make this common case fast.  */
      for (index = 0; index < size && ! srcp[index]; index++)
	continue;
      if (index >= size)
	return 0;

      /* If num is 1, we could speed things up with a binary search
	 of the current word.  */

      bitoff = index * BITSET_WORD_BITS;
    }
  else
    {
      if (bitno >= SBITSET_N_BITS (src))
	return 0;

      index = bitno / BITSET_WORD_BITS;
      bitno = bitno % BITSET_WORD_BITS;

      if (bitno)
	{
	  /* Handle the case where we start within a word.
	     Most often, this is executed with large bitsets
	     with many set bits where we filled the array
	     on the previous call to this function.  */

	  bitoff = index * BITSET_WORD_BITS;
	  word = srcp[index] >> bitno;
	  for (bitno = bitoff + bitno; word; bitno++)
	    {
	      if (word & 1)
		{
		  list[count++] = bitno;
		  if (count >= num)
		    {
		      *next = bitno + 1;
		      return count;
		    }
		}
	      word >>= 1;
	    }
	  index++;
	}
      bitoff = index * BITSET_WORD_BITS;
    }

  for (; index < size; index++, bitoff += BITSET_WORD_BITS)
    {
      if (! (word = srcp[index]))
	continue;

      if ((count + BITSET_WORD_BITS) < num)
      {
	for (bitno = bitoff; word; bitno++)
	  {
	    if (word & 1)
	      list[count++] = bitno;
	    word >>= 1;
	  }
      }
      else
	{
	  for (bitno = bitoff; word; bitno++)
	    {
	      if (word & 1)
		{
		  list[count++] = bitno;
		  if (count >= num)
		    {
		      *next = bitno + 1;
		      return count;
		    }
		}
	      word >>= 1;
	    }
	}
    }

  *next = bitoff;
  return count;
}


/* Ensure that any unused bits within the last word are clear.  */
static inline void
sbitset_unused_clear (dst)
     bitset dst;
{
  unsigned int last_bit;

  last_bit = SBITSET_N_BITS (dst) % BITSET_WORD_BITS;
  if (last_bit)
    SBITSET_WORDS (dst)[dst->csize - 1] &= (bitset_word)((1 << last_bit) - 1);
}


static int
sbitset_op1 (dst, op)
     bitset dst;
     enum bitset_ops op;
{
  unsigned int i;
  bitset_word *dstp = SBITSET_WORDS (dst);
  unsigned int bytes;

  bytes = sizeof (bitset_word) * dst->csize;

  switch (op)
    {
    case BITSET_ZERO:
      memset (dstp, 0, bytes);
      break;

    case BITSET_ONES:
      memset (dstp, ~0, bytes);
      sbitset_unused_clear (dst);
      break;

    case BITSET_EMPTY_P:
      for (i = 0; i < dst->csize; i++)
	if (dstp[i])
	  return 0;
      break;

    default:
      abort ();
    }

  return 1;
}


static int
sbitset_op2 (dst, src, op)
     bitset dst;
     bitset src;
     enum bitset_ops op;
{
  unsigned int i;
  bitset_word *srcp = SBITSET_WORDS (src);
  bitset_word *dstp = SBITSET_WORDS (dst);
  bitset_windex size = dst->csize;

#ifdef ENABLE_CHECKING
  /* Check for compatiblity.  */
  if (src->ops != dst->ops || SBITSET_N_BITS (src) != SBITSET_N_BITS (dst))
    abort ();
#endif

  switch (op)
    {
    case BITSET_COPY:
      if (srcp == dstp)
	break;
      memcpy (dstp, srcp, sizeof (bitset_word) * size);
      break;

    case BITSET_NOT:
      for (i = 0; i < size; i++)
	*dstp++ = ~(*srcp++);
      sbitset_unused_clear (dst);
      break;

    case BITSET_EQUAL_P:
      for (i = 0; i < size; i++)
	if (*dstp++ != *srcp++)
	  return 0;
      break;

    case BITSET_SUBSET_P:
      for (i = 0; i < size; i++, dstp++, srcp++)
	{
	  if (*dstp != (*srcp | *dstp))
	    return 0;
	}
      break;

    default:
      abort ();
    }

  return 1;
}


static int
sbitset_op3 (dst, src1, src2, op)
     bitset dst;
     bitset src1;
     bitset src2;
     enum bitset_ops op;
{
  unsigned int i;
  int changed = 0;
  bitset_word *src1p = SBITSET_WORDS (src1);
  bitset_word *src2p = SBITSET_WORDS (src2);
  bitset_word *dstp = SBITSET_WORDS (dst);
  bitset_windex size = dst->csize;

#ifdef ENABLE_CHECKING
  /* Check for compatiblity.  */
  if (src1->ops != dst->ops || SBITSET_N_BITS (src1) != SBITSET_N_BITS (dst)
      || src2->ops != dst->ops || SBITSET_N_BITS (src2) != SBITSET_N_BITS (dst))
    abort ();
#endif

  switch (op)
    {
    case BITSET_OR:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = *src1p++ | *src2p++;

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      break;

    case BITSET_AND:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = *src1p++ & *src2p++;

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      break;

    case BITSET_XOR:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = *src1p++ ^ *src2p++;

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      break;

    case BITSET_ANDN:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = *src1p++ & ~(*src2p++);

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      break;

    case BITSET_ORN:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = *src1p++ | ~(*src2p++);

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      sbitset_unused_clear (dst);
      break;

    default:
      abort ();
    }

  return changed;
}


static int
sbitset_op4 (dst, src1, src2, src3, op)
     bitset dst;
     bitset src1;
     bitset src2;
     bitset src3;
     enum bitset_ops op;
{
  unsigned int i;
  int changed = 0;
  bitset_word *src1p = SBITSET_WORDS (src1);
  bitset_word *src2p = SBITSET_WORDS (src2);
  bitset_word *src3p = SBITSET_WORDS (src3);
  bitset_word *dstp = SBITSET_WORDS (dst);
  bitset_windex size = dst->csize;

#ifdef ENABLE_CHECKING
  /* Check for compatiblity.  */
  if (src1->ops != dst->ops || SBITSET_N_BITS (src1) != SBITSET_N_BITS (dst)
      || src2->ops != dst->ops || SBITSET_N_BITS (src2) != SBITSET_N_BITS (dst)
      || src3->ops != dst->ops || SBITSET_N_BITS (src3) != SBITSET_N_BITS (dst))
    abort ();
#endif

  switch (op)
    {
    case BITSET_OR_AND:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = (*src1p++ | *src2p++) & *src3p++;

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      break;

    case BITSET_AND_OR:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = (*src1p++ & *src2p++) | *src3p++;

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      break;

    case BITSET_ANDN_OR:
      for (i = 0; i < size; i++, dstp++)
	{
	  bitset_word tmp = (*src1p++ & ~(*src2p++)) | *src3p++;

	  if (*dstp != tmp)
	    {
	      changed = 1;
	      *dstp = tmp;
	    }
	}
      break;

    default:
      abort ();
    }

  return changed;
}


/* Vector of operations for single word bitsets.  */
struct bitset_ops_struct sbitset_small_ops =
  {
    sbitset_set,
    sbitset_reset,
    sbitset_test,
    sbitset_size,
    sbitset_op1,
    sbitset_op2,
    sbitset_op3,
    sbitset_op4,
    sbitset_small_list,
    sbitset_reverse_list,
    NULL,
    BITSET_ARRAY
  };


/* Vector of operations for multiple word bitsets.  */
struct bitset_ops_struct sbitset_ops =
  {
    sbitset_set,
    sbitset_reset,
    sbitset_test,
    sbitset_size,
    sbitset_op1,
    sbitset_op2,
    sbitset_op3,
    sbitset_op4,
    sbitset_list,
    sbitset_reverse_list,
    NULL,
    BITSET_ARRAY
};


int
sbitset_bytes (n_bits)
     bitset_bindex n_bits;
{
  unsigned int bytes, size;

  size = SBITSET_N_WORDS (n_bits);
  bytes = size * sizeof (bitset_word);
  return sizeof (struct bitset_struct) + bytes - sizeof (bitset_word);
}


bitset
sbitset_init (bset, n_bits)
     bitset bset;
     bitset_bindex n_bits;
{
  bitset_windex size;

  size = SBITSET_N_WORDS (n_bits);
  SBITSET_N_BITS (bset) = n_bits;

  /* Use optimized routines if bitset fits within a single word.
     There is probably little merit if using caching since
     the small bitset will always fit in the cache.  */
  if (size == 1)
    bset->ops = &sbitset_small_ops;
  else
    bset->ops = &sbitset_ops;

  bset->cindex = 0;
  bset->csize = size;
  bset->cdata = SBITSET_WORDS (bset);
  return bset;
}
