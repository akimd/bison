/* Bitset statistics.
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
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 
*/

/* This file is a wrapper bitset implementation for the other bitset
   implementations.  It provides bitset compatibility checking and
   statistics gathering without having to instrument the bitset
   implementations.  When statistics gathering is enabled, the bitset
   operations get vectored through here and we then call the appropriate
   routines.  
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bbitset.h"
#include "abitset.h"
#include "ebitset.h"
#include "lbitset.h"
#include "bitset_stats.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* Configuration macros.  */
#define BITSET_STATS_FILE "bitset.dat"
#define BITSET_LOG_COUNT_BINS 10
#define BITSET_LOG_SIZE_BINS  16
#define BITSET_DENSITY_BINS  20


/* Accessor macros.  */
#define BITSET_STATS_ALLOCS_INC(TYPE)	 		\
    bitset_stats_info->types[(TYPE)].allocs++
#define BITSET_STATS_FREES_INC(BSET) 			\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].frees++
#define BITSET_STATS_SETS_INC(BSET) 			\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].sets++
#define BITSET_STATS_CACHE_SETS_INC(BSET) 		\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].cache_sets++
#define BITSET_STATS_RESETS_INC(BSET) 			\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].resets++
#define BITSET_STATS_CACHE_RESETS_INC(BSET) 		\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].cache_resets++
#define BITSET_STATS_TESTS_INC(BSET) 			\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].tests++
#define BITSET_STATS_CACHE_TESTS_INC(BSET) 		\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].cache_tests++
#define BITSET_STATS_LISTS_INC(BSET) 			\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].lists++
#define BITSET_STATS_LIST_COUNTS_INC(BSET, I) 		\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].list_counts[(I)]++
#define BITSET_STATS_LIST_SIZES_INC(BSET, I) 		\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].list_sizes[(I)]++
#define BITSET_STATS_LIST_DENSITY_INC(BSET, I) 		\
    bitset_stats_info->types[BITSET_TYPE_ (BSET)].list_density[(I)]++


typedef struct bitset_stats_struct
{
  bitset bset;
} *bitset_stats;


struct bitset_struct
{
  struct bbitset_struct b;
  struct bitset_stats_struct s;
};


struct bitset_type_info_struct
{
  unsigned int allocs;
  unsigned int frees;
  unsigned int lists;
  unsigned int sets;
  unsigned int cache_sets;
  unsigned int resets;
  unsigned int cache_resets;
  unsigned int tests;
  unsigned int cache_tests;
  unsigned int list_counts[BITSET_LOG_COUNT_BINS];
  unsigned int list_sizes[BITSET_LOG_SIZE_BINS];
  unsigned int list_density[BITSET_DENSITY_BINS];
};

struct bitset_stats_info_struct
{
  unsigned int runs;
  struct bitset_type_info_struct types[BITSET_TYPE_NUM];
};


struct bitset_stats_info_struct bitset_stats_info_data;
struct bitset_stats_info_struct *bitset_stats_info;
int bitset_stats_enabled = 0;


static void bitset_stats_set PARAMS ((bitset, bitset_bindex));
static void bitset_stats_reset PARAMS ((bitset, bitset_bindex));
static int bitset_stats_test PARAMS ((bitset, bitset_bindex));
static int bitset_stats_size PARAMS ((bitset));
static int bitset_stats_op1 PARAMS ((bitset, enum bitset_ops));
static int bitset_stats_op2 PARAMS ((bitset, bitset, enum bitset_ops));
static int bitset_stats_op3 PARAMS ((bitset, bitset, bitset, enum bitset_ops));
static int bitset_stats_op4 PARAMS ((bitset, bitset, bitset, bitset,
				     enum bitset_ops));
static int bitset_stats_list PARAMS ((bitset, bitset_bindex *, bitset_bindex,
				      bitset_bindex *));
static int bitset_stats_reverse_list
PARAMS ((bitset, bitset_bindex *, bitset_bindex, bitset_bindex *));
static void bitset_stats_free PARAMS ((bitset));
static void bitset_percent_histogram_print PARAMS ((FILE *, const char *,
						    const char *,
						    unsigned int,
						    unsigned int *));
static void bitset_log_histogram_print PARAMS ((FILE *, const char *,
						const char *,
						unsigned int,
						unsigned int *));
static void bitset_stats_print_1
PARAMS ((FILE *, const char *, struct bitset_type_info_struct *));
static void bitset_stats_print PARAMS ((FILE *, int));


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
  unsigned int last_bin;

  total = 0;
  for (i = 0; i < n_bins; i++)
    total += bins[i];

  if (!total)
    return;

  for (i = n_bins; i > 3 && ! bins[i - 1]; i--)
     continue;
  last_bin = i - 1;

  /* 2 * ceil (log10 (2) * (N - 1)) + 1.  */
  max_width = 2 * (unsigned int) (0.30103 * (n_bins - 1) + 0.9999) + 1;

  fprintf (file, "%s %s", name, msg);
  for (i = 0; i < 2; i++)
    fprintf (file, "%*d\t%8d (%5.1f%%)\n",
	     max_width, i, bins[i], 100.0 * bins[i] / total);

  for (; i <= last_bin; i++)
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
     struct bitset_type_info_struct *stats;
{
  if (!stats)
    return;
  
  fprintf (file, "%s:\n", name);
  fprintf (file, "%d bitset_allocs, %d freed (%.2f%%).\n",
	   stats->allocs, stats->frees,
	   stats->allocs ? 100.0 * stats->frees / stats->allocs : 0);
  fprintf (file, "%d bitset_sets, %d cached (%.2f%%)\n",
	   stats->sets, stats->cache_sets,
	   stats->sets ? 100.0 * stats->cache_sets / stats->sets : 0);
  fprintf (file, "%d bitset_resets, %d cached (%.2f%%)\n",
	   stats->resets, stats->cache_resets,
	   stats->resets ? 100.0 * stats->cache_resets / stats->resets : 0);
  fprintf (file, "%d bitset_tests, %d cached (%.2f%%)\n",
	   stats->tests, stats->cache_tests,
	   stats->tests ? 100.0 * stats->cache_tests / stats->tests : 0);

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

  if (!bitset_stats_info)
    return;

  fprintf (file, "Bitset statistics:\n\n");

  if (bitset_stats_info->runs > 1)
    fprintf (file, "Accumulated runs = %d\n", bitset_stats_info->runs);

  for (i = 0; i < BITSET_TYPE_NUM; i++)
    bitset_stats_print_1 (file, names[i], &bitset_stats_info->types[i]);
}


/* Initialise bitset statistics logging.  */
void
bitset_stats_enable ()
{
  if (!bitset_stats_info)
    bitset_stats_info = &bitset_stats_info_data;
  bitset_stats_enabled = 1;
}


void
bitset_stats_disable ()
{
  bitset_stats_enabled = 0;
}


/* Read bitset statistics file.  */
void
bitset_stats_read (filename)
     const char *filename;
{
  FILE *file;

  if (!bitset_stats_info)
    return;
  
  if (!filename)
    filename = BITSET_STATS_FILE;

  file = fopen (filename, "r");
  if (file)
    {
      if (fread (&bitset_stats_info_data, sizeof (bitset_stats_info_data),
		 1, file) != 1)
	{
	  if (ferror (file))
	    perror ("Could not read stats file.");
	  else
	    fprintf (stderr, "Bad stats file size.\n");
	}
      fclose (file);
    }
  bitset_stats_info_data.runs++;
}


/* Write bitset statistics file.  */
void
bitset_stats_write (filename)
     const char *filename;
{
  FILE *file;

  if (!bitset_stats_info)
    return;

  if (!filename)
    filename = BITSET_STATS_FILE;

  file = fopen (filename, "w");
  if (file)
    {
      if (fwrite (&bitset_stats_info_data, sizeof (bitset_stats_info_data),
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
  bitset_stats_print (file, 0);
}


/* Function to be called from debugger to print bitset stats.  */
void
debug_bitset_stats (void)
{
  bitset_stats_print (stderr, 1);
}


static void
bitset_stats_set (dst, bitno)
     bitset dst;
     bitset_bindex bitno;
{
  bitset bset = dst->s.bset;
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->b.cindex;
  
  BITSET_STATS_SETS_INC (bset);

  if (offset < bset->b.csize)
    {
      bset->b.cdata[offset] |= (1 << (bitno % BITSET_WORD_BITS));
      BITSET_STATS_CACHE_SETS_INC (bset);
    }
  else
    BITSET_SET_ (bset, bitno);
}


static void
bitset_stats_reset (dst, bitno)
     bitset dst;
     bitset_bindex bitno;
{
  bitset bset = dst->s.bset;
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->b.cindex;
  
  BITSET_STATS_RESETS_INC (bset);

  if (offset < bset->b.csize)
    {
      bset->b.cdata[offset] &= ~(1 << (bitno % BITSET_WORD_BITS));
      BITSET_STATS_CACHE_RESETS_INC (bset);
    }
  else
    BITSET_RESET_ (bset, bitno);
}


static int
bitset_stats_test (src, bitno)
     bitset src;
     bitset_bindex bitno;
{
  bitset bset = src->s.bset;
  bitset_windex index = bitno / BITSET_WORD_BITS;
  bitset_windex offset = index - bset->b.cindex;

  BITSET_STATS_TESTS_INC (bset);
  
  if (offset < bset->b.csize)
    {
      BITSET_STATS_CACHE_TESTS_INC (bset);
      return (bset->b.cdata[offset] >> (bitno % BITSET_WORD_BITS)) & 1;
    }
  else
    return BITSET_TEST_ (bset, bitno);
}


static int
bitset_stats_size (src)
     bitset src;
{
  return BITSET_SIZE_ (src->s.bset);
}


static int
bitset_stats_op1 (dst, op)
     bitset dst;
     enum bitset_ops op;
{
  return BITSET_OP1_ (dst->s.bset, op);
}


static int
bitset_stats_op2 (dst, src, op)
     bitset dst;
     bitset src;
     enum bitset_ops op;
{
  BITSET_CHECK2_ (dst, src);
  return BITSET_OP2_ (dst->s.bset, src->s.bset, op);
}


static int
bitset_stats_op3 (dst, src1, src2, op)
     bitset dst;
     bitset src1;
     bitset src2;
     enum bitset_ops op;
{
  BITSET_CHECK3_ (dst, src1, src2);
  return BITSET_OP3_ (dst->s.bset, src1->s.bset, src2->s.bset, op);
}


static int
bitset_stats_op4 (dst, src1, src2, src3, op)
     bitset dst;
     bitset src1;
     bitset src2;
     bitset src3;
     enum bitset_ops op;
{
  BITSET_CHECK4_ (dst, src1, src2, src3);

  /* This is a bit of a hack.  If the implementation handles
     a four operand operation then vector to it, passing
     the enclosed bitsets.  Otherwise use the fallback 
     bitset_op4 routine.  */
  if (dst->s.bset->b.ops->op4 != bitset_op4)
     return BITSET_OP4_ (dst->s.bset, src1->s.bset, src2->s.bset,
			 src3->s.bset, op);

  return bitset_op4 (dst, src1, src2, src3, op);
}


static int
bitset_stats_list (bset, list, num, next)
     bitset bset;
     bitset_bindex *list;
     bitset_bindex num;
     bitset_bindex *next;
{
  bitset_bindex count;
  bitset_bindex tmp;
  bitset_bindex size;
  bitset_bindex i;
  enum bitset_type type;

  count = BITSET_LIST_ (bset->s.bset, list, num, next);
  
  type = BITSET_TYPE_ (bset->s.bset);
  BITSET_STATS_LISTS_INC (bset->s.bset);
  
  /* Log histogram of number of set bits.  */
  for (i = 0, tmp = count; tmp; tmp >>= 1, i++)
     continue;
  if (i >= BITSET_LOG_COUNT_BINS)
     i = BITSET_LOG_COUNT_BINS - 1;
  BITSET_STATS_LIST_COUNTS_INC (bset->s.bset, i);
  
  /* Log histogram of number of bits in set.  */
  size = BITSET_SIZE_ (bset->s.bset);
  for (i = 0, tmp = size; tmp; tmp >>= 1, i++)
     continue;
  if (i >= BITSET_LOG_SIZE_BINS)
     i = BITSET_LOG_SIZE_BINS - 1;
  BITSET_STATS_LIST_SIZES_INC (bset->s.bset, i);
  
  /* Histogram of fraction of bits set.  */
  i = size ? (count * BITSET_DENSITY_BINS) / size : 0;
  if (i >= BITSET_DENSITY_BINS)
     i = BITSET_DENSITY_BINS - 1;
  BITSET_STATS_LIST_DENSITY_INC (bset->s.bset, i);
  return count;
}


static int
bitset_stats_reverse_list (bset, list, num, next)
     bitset bset;
     bitset_bindex *list;
     bitset_bindex num;
     bitset_bindex *next;
{
  return BITSET_REVERSE_LIST_ (bset->s.bset, list, num, next);
}


static void
bitset_stats_free (bset)
     bitset bset;
{
  BITSET_STATS_FREES_INC (bset->s.bset);
  BITSET_FREE_ (bset->s.bset);
}


struct bitset_ops_struct bitset_stats_ops = {
  bitset_stats_set,
  bitset_stats_reset,
  bitset_stats_test,
  bitset_stats_size,
  bitset_stats_op1,
  bitset_stats_op2,
  bitset_stats_op3,
  bitset_stats_op4,
  bitset_stats_list,
  bitset_stats_reverse_list,
  bitset_stats_free,
  BITSET_STATS
};


/* Return enclosed bitset type.  */
enum bitset_type
bitset_stats_type_get (bset)
   bitset bset;
{
   return BITSET_TYPE_ (bset->s.bset);
}


int bitset_stats_bytes (void)
{
  return sizeof (struct bitset_struct);
}


bitset
bitset_stats_init (bset, n_bits, type)
     bitset bset;
     bitset_bindex n_bits;
     enum bitset_type type;
{
  unsigned int bytes;
  bitset sbset;

  bset->b.ops = &bitset_stats_ops;

  /* Disable cache.  */
  bset->b.cindex = 0;
  bset->b.csize = 0;
  bset->b.cdata = 0;

  /* Set up the actual bitset implementation that
     we are a wrapper over.  */
  switch (type)
    {
    case BITSET_ARRAY:
      bytes = abitset_bytes (n_bits);
      sbset = (bitset) xcalloc (1, bytes);
      abitset_init (sbset, n_bits);
      break;

    case BITSET_LIST:
      bytes = lbitset_bytes (n_bits);
      sbset = (bitset) xcalloc (1, bytes);
      lbitset_init (sbset, n_bits);
      break;

    case BITSET_TABLE:
      bytes = ebitset_bytes (n_bits);
      sbset = (bitset) xcalloc (1, bytes);
      ebitset_init (sbset, n_bits);
      break;

    default:
      abort ();
    }

  bset->s.bset = sbset;

  BITSET_STATS_ALLOCS_INC (type);
  
  return bset;
}
