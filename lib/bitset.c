/* General bitsets.
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

#include <stdlib.h>
#include "bitset.h"
#include "sbitset.h"
#include "lbitset.h"
#include "ebitset.h"
#include "obstack.h"

static void bitset_print PARAMS ((FILE *, bitset, int));

#if BITSET_STATS
#define BITSET_STATS_FILE "bitset.dat"

#define BITSET_LOG_COUNT_BINS 10
#define BITSET_LOG_SIZE_BINS  16
#define BITSET_DENSITY_BINS  20

struct bitset_type_stats_struct
{
  unsigned int xmallocs;
  unsigned int xfrees;
  unsigned int oballocs;
  unsigned int obfrees;
  unsigned int lists;
  unsigned int list_counts[BITSET_LOG_COUNT_BINS];
  unsigned int list_sizes[BITSET_LOG_SIZE_BINS];
  unsigned int list_density[BITSET_DENSITY_BINS];
};

struct bitset_stats_struct
{
  unsigned int runs;
  struct bitset_type_stats_struct types[BITSET_TYPE_NUM];
};

struct bitset_stats_struct bitset_stats_data;
struct bitset_stats_struct *bitset_stats;

static void bitset_percent_histogram_print PARAMS ((FILE *, const char *,
						    const char *,
						    unsigned int,
						    unsigned int *));
static void bitset_log_histogram_print PARAMS ((FILE *, const char *,
						const char *,
						unsigned int,
						unsigned int *));
static void bitset_stats_print_1
PARAMS ((FILE *, const char *, struct bitset_type_stats_struct *));
static void bitset_stats_print PARAMS ((FILE *, int));
static void bitset_stats_read PARAMS ((void));
static void bitset_stats_write PARAMS ((void));

#define BITSET_STATS_XMALLOCS_INC(TYPE) 		\
  if (bitset_stats)					\
    bitset_stats->types[(TYPE)].xmallocs++

#define BITSET_STATS_XFREES_INC(BSET) 			\
  if (bitset_stats)					\
    bitset_stats->types[BITSET_TYPE_ (BSET)].xfrees++

#define BITSET_STATS_OBALLOCS_INC(TYPE) 		\
  if (bitset_stats)					\
    bitset_stats->types[(TYPE)].oballocs++

#define BITSET_STATS_OBFREES_INC(BSET)	 		\
  if (bitset_stats)					\
    bitset_stats->types[BITSET_TYPE_ (BSET)].obfrees++

#define BITSET_STATS_LISTS_INC(BSET) 			\
  if (bitset_stats)					\
    bitset_stats->types[BITSET_TYPE_ (BSET)].lists++

#define BITSET_STATS_LIST_COUNTS_INC(BSET, I) 		\
  if (bitset_stats)					\
    bitset_stats->types[BITSET_TYPE_ (BSET)].list_counts[(I)]++

#define BITSET_STATS_LIST_SIZES_INC(BSET, I) 		\
  if (bitset_stats)					\
    bitset_stats->types[BITSET_TYPE_ (BSET)].list_sizes[(I)]++

#define BITSET_STATS_LIST_DENSITY_INC(BSET, I) 		\
  if (bitset_stats)					\
    bitset_stats->types[BITSET_TYPE_ (BSET)].list_density[(I)]++

#else
#define BITSET_STATS_XMALLOCS_INC(TYPE)

#define BITSET_STATS_XFREES_INC(BSET)

#define BITSET_STATS_OBALLOCS_INC(TYPE)

#define BITSET_STATS_OBFREES_INC(BSET)

#define BITSET_STATS_LISTS_INC(BSET)

#define BITSET_STATS_LIST_COUNTS_INC(BSET, I)

#define BITSET_STATS_LIST_SIZES_INC(BSET, I)

#define BITSET_STATS_LIST_DENSITY_INC(BSET, I)
#endif /* BITSET_STATS  */


/* Return number of bytes required to create a N_BIT bitset
   of TYPE.  The bitset may grow to require more bytes than this.  */
int
bitset_bytes (type, n_bits)
     enum bitset_type type;
     bitset_bindex n_bits;
{
  unsigned int bytes;

  switch (type)
    {
    case BITSET_ARRAY:
      bytes = sbitset_bytes (n_bits);
      break;

    case BITSET_LIST:
      bytes = lbitset_bytes (n_bits);
      break;

    case BITSET_TABLE:
      bytes = ebitset_bytes (n_bits);
      break;

    default:
      abort ();
    }

  return bytes;
}


/* Initialise bitset BSET of TYPE for N_BITS.  */
bitset
bitset_init (bset, n_bits, type)
     bitset bset;
     bitset_bindex n_bits;
     enum bitset_type type;
{
  switch (type)
    {
    case BITSET_ARRAY:
      return sbitset_init (bset, n_bits);

    case BITSET_LIST:
      return lbitset_init (bset, n_bits);

    case BITSET_TABLE:
      return ebitset_init (bset, n_bits);

    default:
      abort ();
    }
}


/* Select a bitset type for a set of N_BITS and with attribute hints
   specified by ATTR.  For variable size bitsets, N_BITS is only a
   hint and may be zero.  */
enum bitset_type
bitset_type_choose (n_bits, attr)
     bitset_bindex n_bits ATTRIBUTE_UNUSED;
     unsigned int attr;
{
  enum bitset_type type;

#if BITSET_CHECK
  /* Check attributes.  */
  if (attr & BITSET_FIXED && attr & BITSET_VARIABLE)
    abort ();
  if (attr & BITSET_SPARSE && attr & BITSET_DENSE)
    abort ();

  /* Note that sometimes we will be asked for a zero length
     fixed size bitset.  */
#endif

  /* Choose the type of bitset.  */

  type = BITSET_ARRAY;
  /* Currently, the simple bitsets do not support a variable size.  */
  if (attr & BITSET_VARIABLE || attr & BITSET_SPARSE)
    {
      type = BITSET_LIST;
      if (attr & BITSET_DENSE || attr & BITSET_GREEDY)
	type = BITSET_TABLE;
    }

  return type;
}


/* Create a bitset of N_BITS of type TYPE.  */
bitset
bitset_alloc (n_bits, type)
     bitset_bindex n_bits;
     enum bitset_type type;
{
  unsigned int bytes;
  bitset bset;

  BITSET_STATS_XMALLOCS_INC (type);

  bytes = bitset_bytes (type, n_bits);

  bset = (bitset) xcalloc (1, bytes);

  /* The cache is disabled until some elements are allocated.  If we
     have variable length arrays, then we may need to allocate dummy
     element.  */

  return bitset_init (bset, n_bits, type);
}


/* Create a bitset of N_BITS of type TYPE.  */
bitset
bitset_obstack_alloc (bobstack, n_bits, type)
     struct obstack *bobstack;
     bitset_bindex n_bits;
     enum bitset_type type;
{
  unsigned int bytes;
  bitset bset;

  BITSET_STATS_OBALLOCS_INC (type);

  bytes = bitset_bytes (type, n_bits);

  bset = obstack_alloc (bobstack, bytes);
  memset (bset, 0, bytes);

  return bitset_init (bset, n_bits, type);
}


/* Create a bitset of N_BITS and with attribute hints specified by
   ATTR.  */
bitset
bitset_create (n_bits, attr)
     bitset_bindex n_bits;
     unsigned int attr;
{
  enum bitset_type type;

  type = bitset_type_choose (n_bits, attr);

  return bitset_alloc (n_bits, type);
}


/* Free bitset BSET.  */
void
bitset_free (bset)
     bitset bset;
{
  BITSET_STATS_XFREES_INC (bset);

  BITSET_FREE_ (bset);
  free (bset);
}


/* Free bitset BSET allocated on obstack.  */
void
bitset_obstack_free (bset)
     bitset bset;
{
  BITSET_STATS_OBFREES_INC (bset);

  BITSET_FREE_ (bset);
}


/* Find next bit set in SRC starting from and including BITNO.
   Return -1 if SRC empty.  */
int
bitset_next (src, bitno)
     bitset src;
     bitset_bindex bitno;
{
  bitset_bindex val;
  bitset_bindex next = bitno;

  if (!bitset_list (src, &val, 1, &next))
    return -1;
  return val;
}


/* Find previous bit set in SRC starting from and including BITNO.
   Return -1 if SRC empty.  */
int
bitset_prev (src, bitno)
     bitset src;
     bitset_bindex bitno;
{
  bitset_bindex val;
  bitset_bindex next = bitno;

  if (!bitset_reverse_list (src, &val, 1, &next))
    return -1;
  return val;
}


/* Find first set bit.   */
int
bitset_first (src)
     bitset src;
{
  return bitset_next (src, 0);
}


/* Find last set bit.   */
int
bitset_last (src)
     bitset src;
{
  return bitset_prev (src, 0);
}


/* Print contents of bitset BSET to FILE.   */
static void
bitset_print (file, bset, verbose)
     FILE *file;
     bitset bset;
     int verbose;
{
  unsigned int i, pos;

  if (verbose)
    fprintf (file, "n_bits = %d, set = {", bitset_size (bset));

  pos = 30;
  BITSET_EXECUTE (bset, 0, i,
  {
    if (pos > 70)
      {
	fprintf (file, "\n");
	pos = 0;
      }

    fprintf (file, "%d ", i);
    pos += 1 + (i >= 10) + (i >= 100);
  });

  if (verbose)
    fprintf (file, "}\n");
}


/* DST = SRC.  Return non-zero if DST != SRC.  */
int
bitset_copy (dst, src)
     bitset dst;
     bitset src;
{
  unsigned int i;

  if (BITSET_COMPATIBLE_ (dst, src))
    return BITSET_COPY_ (dst, src);

  /* Convert bitset types.  We assume that the DST bitset
     is large enough to hold the SRC bitset.  */
  bitset_zero (dst);
  BITSET_EXECUTE (src, 0, i,
  {
     bitset_set (dst, i);
  });

  return 1;
}


/* Return size in bits of bitset SRC.  */
int
bitset_size (src)
     bitset src;
{
  return BITSET_SIZE_ (src);
}


/* Return number of bits set in bitset SRC.  */
int
bitset_count (src)
     bitset src;
{
  bitset_bindex list[BITSET_LIST_SIZE];
  bitset_bindex next;
  int num;
  int count;
  
  next = 0;
  for (count = 0; (num = bitset_list (src, list, BITSET_LIST_SIZE, &next));
       count += num)
    continue;
  
  return count;
}


/* DST = 0.  */
int
bitset_zero (dst)
     bitset dst;
{
  return BITSET_ZERO_ (dst);
}


/* DST = ~0.  */
int
bitset_ones (dst)
     bitset dst;
{
  return BITSET_ONES_ (dst);
}


/* Return SRC == 0.  */
int
bitset_empty_p (src)
     bitset src;
{
  return BITSET_EMPTY_P_ (src);
}


/* Return DST == DST | SRC.  */
int
bitset_subset_p (dst, src)
     bitset dst;
     bitset src;
{
  BITSET_CHECK2_ (dst, src);
  return BITSET_SUBSET_P_ (dst, src);
}


/* Return DST == SRC.  */
int
bitset_equal_p (dst, src)
     bitset dst;
     bitset src;
{
  BITSET_CHECK2_ (dst, src);
  return BITSET_EQUAL_P_ (dst, src);
}


/* Return DST & SRC == 0.  */
int
bitset_disjoint_p (dst, src)
     bitset dst;
     bitset src;
{
  BITSET_CHECK2_ (dst, src);
  return BITSET_DISJOINT_P_ (dst, src);
}


/* DST = ~SRC.  */
int
bitset_not (dst, src)
     bitset dst;
     bitset src;
{
  BITSET_CHECK2_ (dst, src);
  return BITSET_NOT_ (dst, src);
}


/* DST = SRC1 | SRC2.  Return non-zero if DST != SRC1 | SRC2.  */
int
bitset_or (dst, src1, src2)
     bitset dst;
     bitset src1;
     bitset src2;
{
  BITSET_CHECK3_ (dst, src1, src2);
  return BITSET_OR_ (dst, src1, src2);
}


/* DST = SRC1 & SRC2.  Return non-zero if DST != SRC1 & SRC2.  */
int
bitset_and (dst, src1, src2)
     bitset dst;
     bitset src1;
     bitset src2;
{
  BITSET_CHECK3_ (dst, src1, src2);
  return BITSET_AND_ (dst, src1, src2);
}


/* DST = SRC1 ^ SRC2.  Return non-zero if DST != SRC1 ^ SRC2.  */
int
bitset_xor (dst, src1, src2)
     bitset dst;
     bitset src1;
     bitset src2;
{
  BITSET_CHECK3_ (dst, src1, src2);
  return BITSET_XOR_ (dst, src1, src2);
}


/* DST = SRC1 & ~SRC2.  Return non-zero if DST != SRC1 & ~SRC2.  */
int
bitset_andn (dst, src1, src2)
     bitset dst;
     bitset src1;
     bitset src2;
{
  BITSET_CHECK3_ (dst, src1, src2);
  return BITSET_ANDN_ (dst, src1, src2);
}


/* DST = SRC1 | ~SRC2.  Return non-zero if DST != SRC1 | ~SRC2.  */
int
bitset_orn (dst, src1, src2)
     bitset dst;
     bitset src1;
     bitset src2;
{
  BITSET_CHECK3_ (dst, src1, src2);
  return BITSET_ORN_ (dst, src1, src2);
}


int
bitset_op4 (dst, src1, src2, src3, op)
     bitset dst;
     bitset src1;
     bitset src2;
     bitset src3;
     enum bitset_ops op;
{
  int changed = 0;
  bitset tmp;

  /* Create temporary bitset.  */
  tmp = bitset_alloc (BITSET_TYPE_ (dst), 0);

  switch (op)
    {
    case BITSET_OP_OR_AND:
      BITSET_OR_ (tmp, src1, src2);
      changed = BITSET_AND_ (dst, src3, tmp);
      break;

    case BITSET_OP_AND_OR:
      BITSET_AND_ (tmp, src1, src2);
      changed = BITSET_OR_ (dst, src3, tmp);
      break;

    case BITSET_OP_ANDN_OR:
      BITSET_ANDN_ (tmp, src1, src2);
      changed = BITSET_OR_ (dst, src3, tmp);
      break;

    default:
      abort ();
    }

  bitset_free (tmp);
  return changed;
}


/* DST = (SRC1 | SRC2) & SRC3.  Return non-zero if
   DST != (SRC1 | SRC2) & SRC3.  */
int
bitset_or_and (dst, src1, src2, src3)
     bitset dst;
     bitset src1;
     bitset src2;
     bitset src3;
{
  BITSET_CHECK4_ (dst, src1, src2, src3);
  return BITSET_OR_AND_ (dst, src1, src2, src3);
}


/* DST = (SRC1 & SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & SRC2) | SRC3.  */
int
bitset_and_or (dst, src1, src2, src3)
     bitset dst;
     bitset src1;
     bitset src2;
     bitset src3;
{
  BITSET_CHECK4_ (dst, src1, src2, src3);
  return BITSET_AND_OR_ (dst, src1, src2, src3);
}


/* DST = (SRC1 & ~SRC2) | SRC3.  Return non-zero if
   DST != (SRC1 & ~SRC2) | SRC3.  */
int
bitset_andn_or (dst, src1, src2, src3)
     bitset dst;
     bitset src1;
     bitset src2;
     bitset src3;
{
  BITSET_CHECK4_ (dst, src1, src2, src3);
  return BITSET_ANDN_OR_ (dst, src1, src2, src3);
}


/* Dump bitset BSET to FILE.  */
void
bitset_dump (file, bset)
     FILE *file;
     bitset bset;
{
  bitset_print (file, bset, 0);
}


/* Function to be called from debugger to print bitset.  */
void
debug_bitset (bset)
     bitset bset;
{
  if (bset)
    bitset_print (stderr, bset, 1);
}


/* Release memory associated with bitsets.  */
void
bitset_release_memory ()
{
  lbitset_release_memory ();
  ebitset_release_memory ();
}


#if BITSET_STATS
int
bitset_list (bset, list, num, next)
     bitset bset;
     bitset_bindex *list;
     bitset_bindex num;
     bitset_bindex *next;
{
  bitset_bindex count;

  count = BITSET_LIST_ (bset, list, num, next);

  if (bitset_stats)
    {
      bitset_bindex tmp;
      bitset_bindex size;
      bitset_bindex i;
      enum bitset_type type;

      type = BITSET_TYPE_ (bset);
      BITSET_STATS_LISTS_INC (bset);

      /* Log histogram of number of set bits.  */
      for (i = 0, tmp = count; tmp; tmp >>= 1, i++)
	continue;
      if (i >= BITSET_LOG_COUNT_BINS)
	i = BITSET_LOG_COUNT_BINS - 1;
      BITSET_STATS_LIST_COUNTS_INC (bset, i);

      /* Log histogram of number of bits in set.  */
      size = bitset_size (bset);
      for (i = 0, tmp = size; tmp; tmp >>= 1, i++)
	continue;
      if (i >= BITSET_LOG_SIZE_BINS)
	i = BITSET_LOG_SIZE_BINS - 1;
      BITSET_STATS_LIST_SIZES_INC (bset, i);

      /* Histogram of fraction of bits set.  */
      i = size ? (count * BITSET_DENSITY_BINS) / size : 0;
      if (i >= BITSET_DENSITY_BINS)
	i = BITSET_DENSITY_BINS - 1;
      BITSET_STATS_LIST_DENSITY_INC (bset, i);
    }
  return count;
}


/* Print a percentage histogram with message MSG to FILE.  */
static void
bitset_percent_histogram_print (file, name, msg, n_bins, bins)
     FILE *file;
     const char *name;
     const char *msg;
     unsigned int n_bins;
     unsigned int *bins;
{
  unsigned int i;
  unsigned int total;

  total = 0;
  for (i = 0; i < n_bins; i++)
    total += bins[i];

  if (!total)
    return;

  fprintf (file, "%s %s", name, msg);
  for (i = 0; i < n_bins; i++)
    fprintf (file, "%.0f-%.0f%%\t%8d (%5.1f%%)\n",
	     i * 100.0 / n_bins,
	     (i + 1) * 100.0 / n_bins, bins[i],
	     (100.0 * bins[i]) / total);
}


/* Print a log histogram with message MSG to FILE.  */
static void
bitset_log_histogram_print (file, name, msg, n_bins, bins)
     FILE *file;
     const char *name;
     const char *msg;
     unsigned int n_bins;
     unsigned int *bins;
{
  unsigned int i;
  unsigned int total;
  unsigned int max_width;

  total = 0;
  for (i = 0; i < n_bins; i++)
    total += bins[i];

  if (!total)
    return;

  /* 2 * ceil (log10(2) * (N - 1)) + 1  */
  max_width = 2 * (unsigned int) (0.30103 * (n_bins - 1) + 0.9999) + 1;

  fprintf (file, "%s %s", name, msg);
  for (i = 0; i < 2; i++)
    fprintf (file, "%*d\t%8d (%5.1f%%)\n",
	     max_width, i, bins[i], 100.0 * bins[i] / total);

  /* Perhaps we should bail out once the histogram goes to zero.  */
  for (; i < n_bins; i++)
    fprintf (file, "%*d-%d\t%8d (%5.1f%%)\n",
	     max_width - ((unsigned int) (0.30103 * (i) + 0.9999) + 1),
	     1 << (i - 1), (1 << i) - 1, bins[i],
	     (100.0 * bins[i]) / total);
}


/* Print bitset statistics to FILE.  */
static void
bitset_stats_print_1 (file, name, stats)
     FILE *file;
     const char *name;
     struct bitset_type_stats_struct *stats;
{
  if (!stats)
    return;

  fprintf (file, "%d %ss xmalloced, %d freed.\n",
	   stats->xmallocs, name, stats->xfrees);
  fprintf (file, "%d %ss oballoced, %d freed.\n",
	   stats->oballocs, name, stats->obfrees);

  fprintf (file, "%d bitset_lists\n", stats->lists);

  bitset_log_histogram_print (file, name, "count log histogram\n",
			      BITSET_LOG_COUNT_BINS, stats->list_counts);

  bitset_log_histogram_print (file, name, "size log histogram\n",
			      BITSET_LOG_SIZE_BINS, stats->list_sizes);

  bitset_percent_histogram_print (file, name, "density histogram\n",
				  BITSET_DENSITY_BINS, stats->list_density);
}


/* Print all bitset statistics to FILE.  */
static void
bitset_stats_print (file, verbose)
     FILE *file;
     int verbose ATTRIBUTE_UNUSED;
{
  int i;
  static const char *names[] = BITSET_TYPE_NAMES;

  if (!bitset_stats)
    return;

  fprintf (file, "Bitset statistics:\n\n");

  if (bitset_stats->runs > 1)
    fprintf (file, "Accumulated runs = %d\n", bitset_stats->runs);

  for (i = 0; i < BITSET_TYPE_NUM; i++)
    bitset_stats_print_1 (file, names[i], &bitset_stats->types[i]);
}
#endif /* BITSET_STATS  */


/* Initialise bitset statistics logging.  */
void
bitset_stats_init ()
{
#if BITSET_STATS
  bitset_stats = &bitset_stats_data;
  bitset_stats_read ();
#endif /* BITSET_STATS  */
}


/* Read bitset statistics file.  */
static void
bitset_stats_read ()
{
  FILE *file;

  if (!bitset_stats)
    return;

  file = fopen (BITSET_STATS_FILE, "r");
  if (file)
    {
      if (fread (&bitset_stats_data, sizeof (bitset_stats_data),
		 1, file) != 1)
	{
	  if (ferror (file))
	    perror ("Could not read stats file.");
	  else
	    fprintf (stderr, "Bad stats file size.\n");
	}
      fclose (file);
    }
  bitset_stats_data.runs++;
}


/* Write bitset statistics file.  */
static void
bitset_stats_write ()
{
  FILE *file;

  if (!bitset_stats)
    return;

  file = fopen (BITSET_STATS_FILE, "w");
  if (file)
    {
      if (fwrite (&bitset_stats_data, sizeof (bitset_stats_data),
		  1, file) != 1)
	perror ("Could not write stats file.");
      fclose (file);
    }
  else
    perror ("Could not open stats file for writing.");
}


/* Dump bitset statistics to FILE.  */
void
bitset_stats_dump (file)
     FILE *file;
{
#if BITSET_STATS
  bitset_stats_print (file, 0);
  bitset_stats_write ();
#endif /* BITSET_STATS  */
}


/* Function to be called from debugger to print bitset stats.  */
void
debug_bitset_stats (void)
{
#if BITSET_STATS
  bitset_stats_print (stderr, 1);
#endif /* BITSET_STATS  */
}
