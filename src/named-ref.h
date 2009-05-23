/* Named symbol references for Bison

   Copyright 2009 Free Software Foundation, Inc.

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

#ifndef NAMED_REF_H_
# define NAMED_REF_H_

#include "uniqstr.h"
#include "location.h"

typedef struct named_ref named_ref;

struct named_ref
{
  uniqstr id;
  location loc;
};

named_ref *named_ref_new (uniqstr id, location loc);

void named_ref_free (named_ref *r);

#endif /* !NAMED_REF_H_ */
