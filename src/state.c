/* Type definitions for nondeterministic finite state machine for bison,
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


#include "system.h"
#include "state.h"

/*---------------------------------.
| Create a new array of N shitfs.  |
`---------------------------------*/

#define SHIFTS_ALLOC(Nshifts)						\
  (shifts *) xcalloc ((unsigned) (sizeof (shifts) 			\
                                  + (Nshifts - 1) * sizeof (short)), 1)

shifts *
shifts_new (int n)
{
  shifts *res = SHIFTS_ALLOC (n);
  res->nshifts = n;
  return res;
}


/*-------------------------------.
| Create a new array of N errs.  |
`-------------------------------*/

#define ERRS_ALLOC(Nerrs)						\
  (errs *) xcalloc ((unsigned) (sizeof (errs) 				\
                                  + (Nerrs - 1) * sizeof (short)), 1)


errs *
errs_new (int n)
{
  errs *res = ERRS_ALLOC (n);
  res->nerrs = n;
  return res;
}


errs *
errs_dup (errs *src)
{
  errs *res = errs_new (src->nerrs);
  memcpy (res->errs, src->errs, src->nerrs);
  return res;
}

/*-------------------------------------.
| Create a new array of N reductions.  |
`-------------------------------------*/

#define REDUCTIONS_ALLOC(Nreductions)					\
  (reductions *) xcalloc ((unsigned) (sizeof (reductions)		\
                                  + (Nreductions - 1) * sizeof (short)), 1)

reductions *
reductions_new (int n)
{
  reductions *res = REDUCTIONS_ALLOC (n);
  res->nreds = n;
  return res;
}
