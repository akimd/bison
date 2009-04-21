/* A simple, memory-efficient bitset implementation.

   Copyright (C) 2009 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef SBITSET_H_
# define SBITSET_H_

typedef char *Sbitset;
typedef size_t Sbitset__Index;

#define Sbitset__nbytes(NBITS)            (((NBITS)+7)/8)
#define Sbitset__byteAddress(SELF, INDEX) (((SELF) + (INDEX)/8))
#define Sbitset__bit_mask(INDEX)          (0x1 << (7 - (INDEX)%8))
#define Sbitset__last_byte_mask(NBITS)    (0xff << (7 - ((NBITS)-1)%8))

/* nbits must not be 0.  */
Sbitset Sbitset__new (Sbitset__Index nbits);
Sbitset Sbitset__new_on_obstack (Sbitset__Index nbits,
                                 struct obstack *obstackp);
void Sbitset__delete (Sbitset self);

#define Sbitset__test(SELF, INDEX)                                            \
  ((*Sbitset__byteAddress ((SELF), (INDEX)) & Sbitset__bit_mask (INDEX)) != 0)

bool Sbitset__isEmpty (Sbitset self, Sbitset__Index nbits);

void Sbitset__fprint(Sbitset self, Sbitset__Index nbits, FILE *file);

#define Sbitset__set(SELF, INDEX)                                             \
do {                                                                          \
  *Sbitset__byteAddress ((SELF), (INDEX)) =                                   \
    *Sbitset__byteAddress ((SELF), (INDEX)) | Sbitset__bit_mask (INDEX);      \
} while(0)

#define Sbitset__reset(SELF, INDEX)                                           \
do {                                                                          \
  *Sbitset__byteAddress ((SELF), (INDEX)) =                                   \
    *Sbitset__byteAddress ((SELF), (INDEX)) & ~Sbitset__bit_mask (INDEX);     \
} while(0)

/* NBITS is the size of the bitset.  More than NBITS bits might be reset.  */
#define Sbitset__zero(SELF, NBITS)                                            \
do {                                                                          \
  memset (SELF, 0, Sbitset__nbytes (NBITS));                                  \
} while(0)

/* NBITS is the size of the bitset.  More than NBITS bits might be set.  */
#define Sbitset__ones(SELF, NBITS)                                            \
do {                                                                          \
  memset (SELF, 0xff, Sbitset__nbytes (NBITS));                               \
} while(0)

/* NBITS is the size of every bitset.  More than NBITS bits might be set.  */
#define Sbitset__or(SELF, OTHER1, OTHER2, NBITS)                              \
do {                                                                          \
  char *ptr_self = (SELF);                                                    \
  char *ptr_other1 = (OTHER1);                                                \
  char *ptr_other2 = (OTHER2);                                                \
  char *end_self = ptr_self + Sbitset__nbytes (NBITS);                        \
  for (; ptr_self < end_self; ++ptr_self, ++ptr_other1, ++ptr_other2)         \
    *ptr_self = *ptr_other1 | *ptr_other2;                                    \
} while(0)

#define SBITSET__FOR_EACH(SELF, NBITS, ITER, INDEX)                           \
  for ((ITER) = (SELF); (ITER) < (SELF) + Sbitset__nbytes (NBITS); ++(ITER))  \
    if (*(ITER) != 0)                                                         \
      for ((INDEX) = ((ITER)-(SELF))*8;                                       \
           (INDEX) < (NBITS) && (SELF)+(INDEX)/8 < (ITER)+1;                  \
           ++(INDEX))                                                         \
        if (((*ITER) & Sbitset__bit_mask (INDEX)) != 0)

#endif /* !SBITSET_H_ */
