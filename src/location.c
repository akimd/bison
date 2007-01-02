/* Locations for Bison
   Copyright (C) 2002, 2005, 2006, 2007 Free Software Foundation, Inc.

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

#include <config.h>
#include "system.h"

#include <mbswidth.h>
#include <quotearg.h>

#include "complain.h"
#include "location.h"

location const empty_location = EMPTY_LOCATION_INIT;

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, or might occur but is undetectable,
   return INT_MAX.  Assume COLUMN is nonnegative.  */

static inline int
add_column_width (int column, char const *buf, size_t bufsize)
{
  size_t width;
  unsigned int remaining_columns = INT_MAX - column;

  if (buf)
    {
      if (INT_MAX / 2 <= bufsize)
	return INT_MAX;
      width = mbsnwidth (buf, bufsize, 0);
    }
  else
    width = bufsize;

  return width <= remaining_columns ? column + width : INT_MAX;
}

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

void
location_compute (location *loc, boundary *cur, char const *token, size_t size)
{
  int line = cur->line;
  int column = cur->column;
  char const *p0 = token;
  char const *p = token;
  char const *lim = token + size;

  loc->start = *cur;

  for (p = token; p < lim; p++)
    switch (*p)
      {
      case '\n':
	line += line < INT_MAX;
	column = 1;
	p0 = p + 1;
	break;

      case '\t':
	column = add_column_width (column, p0, p - p0);
	column = add_column_width (column, NULL, 8 - ((column - 1) & 7));
	p0 = p + 1;
	break;

      default:
	break;
      }

  cur->line = line;
  cur->column = column = add_column_width (column, p0, p - p0);

  loc->end = *cur;

  if (line == INT_MAX && loc->start.line != INT_MAX)
    warn_at (*loc, _("line number overflow"));
  if (column == INT_MAX && loc->start.column != INT_MAX)
    warn_at (*loc, _("column number overflow"));
}


/* Output to OUT the location LOC.
   Warning: it uses quotearg's slot 3.  */
void
location_print (FILE *out, location loc)
{
  int end_col = 0 < loc.end.column ? loc.end.column - 1 : 0;
  fprintf (out, "%s:%d.%d",
	   quotearg_n_style (3, escape_quoting_style, loc.start.file),
	   loc.start.line, loc.start.column);

  if (loc.start.file != loc.end.file)
    fprintf (out, "-%s:%d.%d",
	     quotearg_n_style (3, escape_quoting_style, loc.end.file),
	     loc.end.line, end_col);
  else if (loc.start.line < loc.end.line)
    fprintf (out, "-%d.%d", loc.end.line, end_col);
  else if (loc.start.column < end_col)
    fprintf (out, "-%d", end_col);
}
