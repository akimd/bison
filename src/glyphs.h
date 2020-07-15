/* Graphical symbols.

   Copyright (C) 2020 Free Software Foundation, Inc.

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

#ifndef GLYPHS_H
# define GLYPHS_H

/* Initialize the following variables.  */
void glyphs_init (void);

/* "→", separates the lhs of a rule from its rhs.  */
extern const char *arrow;
extern int arrow_width;

/* "•", a point in an item (aka, a dotted rule).  */
extern const char *dot;
extern int dot_width;

#endif /* GLYPHS_H */
