/* Muscle table manager for Bison,
   Copyright (C) 2001, 2002, 2003, 2006 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#ifndef MUSCLE_TAB_H_
# define MUSCLE_TAB_H_

# include "location.h"

void muscle_init (void);
void muscle_insert (char const *key, char const *value);
char *muscle_find (char const *key);
void muscle_free (void);


/* An obstack dedicated to receive muscle keys and values.  */
extern struct obstack muscle_obstack;

#define MUSCLE_INSERT_BOOL(Key, Value)				\
do {								\
  int v = Value;						\
  MUSCLE_INSERT_INT (Key, v);					\
} while(0)

#define MUSCLE_INSERT_INT(Key, Value)				\
do {								\
  obstack_fgrow1 (&muscle_obstack, "%d", Value);		\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
} while(0)

#define MUSCLE_INSERT_LONG_INT(Key, Value)			\
do {								\
  obstack_fgrow1 (&muscle_obstack, "%ld", Value);		\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
} while(0)

#define MUSCLE_INSERT_STRING_RAW(Key, Value)			\
do {								\
  obstack_sgrow (&muscle_obstack, Value);			\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
} while(0)

#define MUSCLE_INSERT_STRING(Key, Value)			\
do {								\
  MUSCLE_OBSTACK_SGROW (&muscle_obstack, Value);                \
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
} while(0)

#define MUSCLE_OBSTACK_SGROW(Obstack, Value)			\
do {								\
  char const *p;						\
  for (p = Value; *p; p++)					\
    switch (*p)							\
      {								\
      case '$':	obstack_sgrow (Obstack, "$]["); break;		\
      case '@':	obstack_sgrow (Obstack, "@@" ); break;		\
      case '[':	obstack_sgrow (Obstack, "@{" ); break;		\
      case ']':	obstack_sgrow (Obstack, "@}" ); break;		\
      default: obstack_1grow (Obstack, *p); break;		\
      }								\
} while(0)

#define MUSCLE_INSERT_C_STRING(Key, Value)			\
do {								\
  MUSCLE_OBSTACK_SGROW (&muscle_obstack,			\
			quotearg_style (c_quoting_style,	\
					Value));		\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
} while(0)

/* Append VALUE to the current value of KEY.  If KEY did not already
   exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously
   associated value.  Copy VALUE and SEPARATOR.  */

void muscle_grow (const char *key, const char *value, const char *separator);


/* Append VALUE to the current value of KEY, using muscle_grow.  But
   in addition, issue a synchronization line for the location LOC.  */

void muscle_code_grow (const char *key, const char *value, location loc);


/* MUSCLE is an M4 list of pairs.  Create or extend it with the pair
   (A1, A2).  Note that because the muscle values are output *double*
   quoted, one needs to strip the first level of quotes to reach the
   list itself.  */

void muscle_pair_list_grow (const char *muscle,
			    const char *a1, const char *a2);

void muscles_m4_output (FILE *out);

/* In the format `file_name:line.column', append BOUND to MUSCLE.  Use digraphs
   for special characters in the file name.  */
void muscle_boundary_grow (char const *key, boundary bound);

#endif /* not MUSCLE_TAB_H_ */
