/* Definitions for macrotab.c and callers, part of bison,
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.

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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef MUSCLE_TAB_H_
# define MUSCLE_TAB_H

# define MTABSIZE 101

typedef struct muscle_entry_s
{
  const char *key;
  const char *value;
} muscle_entry_t;

void muscle_init PARAMS ((void));
void muscle_insert PARAMS ((const char *key, const char *value));
const char *muscle_find PARAMS ((const char *key));
void muscle_free PARAMS ((void));


/* An obstack dedicated to receive muscle keys and values.  */
extern struct obstack muscle_obstack;

#define MUSCLE_INSERT_INT(Key, Value)				\
{								\
  obstack_fgrow1 (&muscle_obstack, "%d", Value);	       	\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
}

#define MUSCLE_INSERT_LONG_INT(Key, Value)			\
{								\
  obstack_fgrow1 (&muscle_obstack, "%ld", Value);	       	\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
}

#define MUSCLE_INSERT_STRING(Key, Value)			\
{								\
  obstack_sgrow (&muscle_obstack, Value);			\
  obstack_1grow (&muscle_obstack, 0);				\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	\
}

#define MUSCLE_INSERT_PREFIX(Key, Value)	       			\
{									\
  obstack_fgrow2 (&muscle_obstack, "%s%s", 				\
		  spec_name_prefix ? spec_name_prefix : "yy", Value);	\
  obstack_1grow (&muscle_obstack, 0);					\
  muscle_insert (Key, obstack_finish (&muscle_obstack));	       	\
}


void muscles_m4_output PARAMS ((FILE *out));

#endif /* not MUSCLE_TAB_H_ */
