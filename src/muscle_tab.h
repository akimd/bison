/* Definitions for macrotab.c and callers, part of bison,
   Copyright 2001 Free Software Foundation, Inc.

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

#endif /* not MUSCLE_TAB_H_ */
