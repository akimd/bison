/* Find and resolve or report look-ahead conflicts for bison,
   Copyright (C) 2000, 2001, 2002 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#ifndef CONFLICTS_H_
# define CONFLICTS_H_
# include "state.h"

void conflicts_solve PARAMS ((void));
void conflicts_print PARAMS ((void));
int conflicts_total_count PARAMS ((void));
void conflicts_output PARAMS ((FILE *out));
void conflicts_free PARAMS ((void));

/* Were there conflicts? */
extern int expected_conflicts;
#endif /* !CONFLICTS_H_ */
