/* Bitset vectors.
   Copyright (C) 2001 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include "bitsetv.h"


/* Create a vector of N_VECS bitsets, each of N_BITS, and of
   type TYPE.  */
bitset *
bitsetv_alloc (n_vecs, n_bits, type)
    unsigned int n_vecs;
    unsigned int n_bits;
    enum bitset_type type;
{
    unsigned int vector_bytes;
    unsigned int bytes;
    bitset *bsetv;
    unsigned int i;

    /* Determine number of bytes for each set.  */
    bytes = bitset_bytes (type, n_bits);

    /* Allocate vector table at head of bitset array.  */
    vector_bytes = n_vecs * sizeof (bitset);
    bsetv = (bitset *) xmalloc (vector_bytes + bytes * n_vecs);

    for (i = 0; i < n_vecs; i++)
    {
      bsetv[i] = (bitset)((char *)bsetv + vector_bytes + i * bytes);

      bitset_init (bsetv[i], n_bits, type);
    }
    return bsetv;
}


/* Create a vector of N_VECS bitsets, each of N_BITS, and with
   attribute hints specified by ATTR.  */
bitset *
bitsetv_create (n_vecs, n_bits, attr)
    unsigned int n_vecs;
    unsigned int n_bits;
    unsigned int attr;
{
    enum bitset_type type;

    type = bitset_type_choose (n_bits, attr);
    return bitsetv_alloc (n_vecs, n_bits, type);
}


/* Free bitset vector BSETV.  */
void
bitsetv_free (bsetv)
    bitset *bsetv;
{
    free (bsetv);
}


/* Zero a vector of N_VECS bitsets.  */
void
bitsetv_zero (bsetv, n_vecs)
     struct bitset_struct **bsetv;
     unsigned int n_vecs;
{
  unsigned int i;

  for (i = 0; i < n_vecs; i++)
      bitset_zero (bsetv[i]);
}


/* Set a vector of N_VECS bitsets to ones.  */
void
bitsetv_ones (bsetv, n_vecs)
     bitset *bsetv;
     unsigned int n_vecs;
{
  unsigned int i;

  for (i = 0; i < n_vecs; i++)
    bitset_ones (bsetv[i]);
}


/* Dump the contents of a bitset vector BSETV with N_VECS elements to
   FILE.  */
void
bitsetv_dump (file, title, subtitle, bsetv, n_vecs)
     FILE *file;
     const char *title, *subtitle;
     bitset *bsetv;
     unsigned int n_vecs;
{
  unsigned int i;

  fprintf (file, "%s\n", title);
  for (i = 0; i < n_vecs; i++)
    {
      fprintf (file, "%s %d\n", subtitle, i);
      bitset_dump (file, bsetv[i]);
    }

  fprintf (file, "\n");
}
