/* Keeping a unique copy of strings.
   Copyright (C) 2002 Free Software Foundation, Inc.

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

#ifndef STRUNIQ_H_
# define STRUNIQ_H_

/*------------------------------------------------------.
| struniq_t -- pointers to unique copies of C strings.  |
`------------------------------------------------------*/

typedef char *struniq_t;

/* Return the struniq for S.  */
const struniq_t struniq_new (const char *s);

/*--------------------------------------.
| Initializing, destroying, debugging.  |
`--------------------------------------*/

/* Create the string table.  */
void struniqs_new (void);

/* Free all the memory allocated for symbols.  */
void struniqs_free (void);

/* Report them all.  */
void struniqs_print (void);

#endif /* !STRUNIQ_H_ */
