/* Locations for Bison
   Copyright (C) 2002  Free Software Foundation, Inc.

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

#ifndef LOCATION_H_
# define LOCATION_H_

typedef struct location_s
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
}  location_t;
#define YYLTYPE location_t

/* Initialize LOC. */
# define LOCATION_RESET(Loc)                  \
  (Loc).first_column = (Loc).first_line = 1;  \
  (Loc).last_column =  (Loc).last_line = 1;

/* Advance of NUM columns. */
# define LOCATION_COLUMNS(Loc, Num)           \
  (Loc).last_column += Num;

/* Advance of NUM lines. */
# define LOCATION_LINES(Loc, Num)             \
  (Loc).last_column = 1;                      \
  (Loc).last_line += Num;

/* Restart: move the first cursor to the last position. */
# define LOCATION_STEP(Loc)                   \
  (Loc).first_column = (Loc).last_column;     \
  (Loc).first_line = (Loc).last_line;

/* Output LOC on the stream OUT. */
# define LOCATION_PRINT(Out, Loc)                               \
  fprintf (stderr, "%s:", infile);				\
  if ((Loc).first_line != (Loc).last_line)                      \
    fprintf (Out, "%d.%d-%d.%d",                                \
             (Loc).first_line, (Loc).first_column,              \
             (Loc).last_line, (Loc).last_column - 1);           \
  else if ((Loc).first_column < (Loc).last_column - 1)          \
    fprintf (Out, "%d.%d-%d", (Loc).first_line,                 \
             (Loc).first_column, (Loc).last_column - 1);        \
  else                                                          \
    fprintf (Out, "%d.%d", (Loc).first_line, (Loc).first_column)

#endif /* !LOCATION_H_ */
