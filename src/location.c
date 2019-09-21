/* Locations for Bison

   Copyright (C) 2002, 2005-2015, 2018-2019 Free Software Foundation,
   Inc.

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

#include <config.h>
#include "system.h"

#include <mbfile.h>
#include <mbswidth.h>
#include <quotearg.h>
#include <stdio.h>    /* fileno */
#include <sys/stat.h> /* fstat */

#include "complain.h"
#include "getargs.h"
#include "location.h"

location const empty_loc = EMPTY_LOCATION_INIT;

static int
min_int (int a, int b)
{
  return a < b ? a : b;
}

/* The terminal width.  */
static int
columns (void)
{
  const char *cp = getenv ("COLUMNS");
  int res = 80;
  if (cp && *cp)
    {
      unsigned long ul = strtoul (cp, NULL, 10);
      res = ul < INT_MAX ? ul : INT_MAX;
    }
  return res;
}

/* If BUF is null, add BUFSIZE (which in this case must be less than
   INT_MAX) to COLUMN; otherwise, add mbsnwidth (BUF, BUFSIZE, 0) to
   COLUMN.  If an overflow occurs, return INT_MAX.  */

static inline int
add_column_width (int column, char const *buf, size_t bufsize)
{
  int width
    = buf ? mbsnwidth (buf, bufsize, 0)
    : INT_MAX <= bufsize ? INT_MAX
    : bufsize;
  return column <= INT_MAX - width ? column + width : INT_MAX;
}

static void
boundary_compute (boundary *cur, char const *token, size_t size)
{
  int line = cur->line;
  int column = cur->column;
  int byte = cur->byte;
  char const *p0 = token;
  char const *p = token;
  char const *lim = token + size;

  for (p = token; p < lim; ++p)
    switch (*p)
      {
      case '\n':
        line += line < INT_MAX;
        column = 1;
        byte = 1;
        p0 = p + 1;
        break;

      case '\t':
        column = add_column_width (column, p0, p - p0);
        column = add_column_width (column, NULL, 8 - ((column - 1) & 7));
        p0 = p + 1;
        byte += byte < INT_MAX;
        break;

      default:
        byte += byte < INT_MAX;
        break;
      }
  column = add_column_width (column, p0, p - p0);

  cur->line = line;
  cur->column = column;
  cur->byte = byte;
}


/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */

void
location_compute (location *loc, boundary *cur, char const *token, size_t size)
{
  loc->start = *cur;
  boundary_compute (cur, token, size);
  loc->end = *cur;

  if (loc->end.line == INT_MAX && loc->start.line != INT_MAX)
    complain (loc, Wother, _("line number overflow"));
  if (loc->end.column == INT_MAX && loc->start.column != INT_MAX)
    complain (loc, Wother, _("column number overflow"));
  if (loc->end.byte == INT_MAX && loc->start.byte != INT_MAX)
    complain (loc, Wother, _("byte number overflow"));
}

static unsigned
boundary_print (boundary const *b, FILE *out)
{
  return fprintf (out, "%s:%d.%d@%d",
                  quotearg_n_style (3, escape_quoting_style, b->file),
                  b->line, b->column, b->byte);
}

unsigned
location_print (location loc, FILE *out)
{
  unsigned res = 0;
  if (trace_flag & trace_locations)
    {
      res += boundary_print (&loc.start, out);
      res += fprintf (out, "-");
      res += boundary_print (&loc.end, out);
    }
  else
    {
      int end_col = 0 != loc.end.column ? loc.end.column - 1 : 0;
      res += fprintf (out, "%s",
                      quotearg_n_style (3, escape_quoting_style, loc.start.file));
      if (0 <= loc.start.line)
        {
          res += fprintf (out, ":%d", loc.start.line);
          if (0 <= loc.start.column)
            res += fprintf (out, ".%d", loc.start.column);
        }
      if (loc.start.file != loc.end.file)
        {
          res += fprintf (out, "-%s",
                          quotearg_n_style (3, escape_quoting_style,
                                            loc.end.file));
          if (0 <= loc.end.line)
            {
              res += fprintf (out, ":%d", loc.end.line);
              if (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
        }
      else if (0 <= loc.end.line)
        {
          if (loc.start.line < loc.end.line)
            {
              res += fprintf (out, "-%d", loc.end.line);
              if (0 <= end_col)
                res += fprintf (out, ".%d", end_col);
            }
          else if (0 <= end_col && loc.start.column < end_col)
            res += fprintf (out, "-%d", end_col);
        }
    }

  return res;
}


/* Persistent data used by location_caret to avoid reopening and rereading the
   same file all over for each error.  */
static struct
{
  /* Raw input file.  */
  FILE *file;
  /* Input file as a stream of multibyte characters.  */
  mb_file_t mbfile;
  /* The position within the last file we quoted.  If POS.FILE is non
     NULL, but FILE is NULL, it means this file is special and should
     not be quoted. */
  boundary pos;
  /* Offset in FILE where line POS.LINE starts.  */
  size_t offset;
} caret_info;


/* Open FILE for quoting, if needed, and if possible.  Return whether
   the file can quoted. */
static bool
caret_set_file (const char *file)
{
  /* If a different file than before, close and let the rest open
     the new one. */
  if (caret_info.pos.file && caret_info.pos.file != file)
    {
      caret_free ();
      caret_info.pos.file = NULL;
    }
  if (!caret_info.pos.file)
    {
      caret_info.pos.file = file;
      if ((caret_info.file = fopen (caret_info.pos.file, "r")))
        {
          /* If the file is not regular (imagine #line 1 "/dev/stdin"
             in the input file for instance), don't try to quote the
             file.  Keep caret_info.file set so that we don't try to
             open it again, but leave caret_info.file NULL so that we
             don't try to quote it. */
          struct stat buf;
          if (fstat (fileno (caret_info.file), &buf) == 0
              && buf.st_mode & S_IFREG)
            {
              caret_info.pos.line = 1;
              mbf_init (caret_info.mbfile, caret_info.file);
            }
          else
            caret_free ();
        }
    }
  return caret_info.file;
}

void
caret_free (void)
{
  if (caret_info.file)
    {
      fclose (caret_info.file);
      caret_info.file = NULL;
    }
}

/* Getc, but smash \r\n as \n.  */
static void
caret_getc_internal (mbchar_t *res)
{
  mbf_getc (*res, caret_info.mbfile);
  if (mb_iseq (*res, '\r'))
    {
      mbchar_t c;
      mbf_getc (c, caret_info.mbfile);
      if (mb_iseq (c, '\n'))
        mb_copy (res, &c);
      else
        mbf_ungetc (c, caret_info.mbfile);
    }
}

#define caret_getc(Var) caret_getc_internal(&Var)

void
location_caret (location loc, const char *style, FILE *out)
{
  if (loc.start.column == -1 || loc.start.line == -1)
    return;
  if (!caret_set_file (loc.start.file))
    return;


  /* If the line we want to quote is seekable (the same line as the previous
     location), just seek it. If it was a previous line, we lost track of it,
     so return to the start of file.  */
  if (loc.start.line < caret_info.pos.line)
    {
      caret_info.pos.line = 1;
      caret_info.offset = 0;
    }
  fseek (caret_info.file, caret_info.offset, SEEK_SET);

  /* Advance to the line's position, keeping track of the offset.  */
  while (caret_info.pos.line < loc.start.line)
    {
      mbchar_t c;
      caret_getc (c);
      if (mb_iseof (c))
        /* Something is wrong, that line number does not exist.  */
        return;
      caret_info.pos.line += mb_iseq (c, '\n');
    }
  caret_info.offset = ftell (caret_info.file);
  caret_info.pos.column = 1;
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;

  /* Find the number of columns of this line.  */
  while (true)
    {
      mbchar_t c;
      caret_getc (c);
      if (mb_iseof (c) || mb_iseq (c, '\n'))
        break;
      boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
    }
  int line_len = caret_info.pos.column;

  /* Available width.  Eight chars are consumed by the left-margin of
     the quoting lines.  */
  int width = columns () - 8;
  int skip = 0;
  if (width < line_len)
    {
      /* We cannot quote the whole line.  Make sure we can see the
         beginning of the location.  */
      skip = width < loc.start.column ? loc.start.column - 10 : 0;
    }
  /* If we skip the initial part, we insert "..." before.  */
  if (skip)
    width -= 3;
  /* If the end of line does not fit, we also need to truncate the
     end, and leave "..." there.  */
  if (width < line_len - skip)
    width -= 3;

  /* Go back to the beginning of line.  */
  fseek (caret_info.file, caret_info.offset, SEEK_SET);
  /* Reset mbf's internal state.
     FIXME: should be done in mbfile.  */
  caret_info.mbfile.eof_seen = 0;
  caret_info.pos.column = 1;

  /* Read the actual line.  Don't update the offset, so that we keep a pointer
     to the start of the line.  */
  {
    mbchar_t c;
    caret_getc (c);
    if (!mb_iseof (c))
      {
        /* The last column to highlight.  Only the first line of
           multiline locations are quoted, in which case the ending
           column is the end of line.  Single point locations (with
           equal boundaries) denote the character that they
           follow.  */
        int col_end
          = loc.start.line == loc.end.line
          ? loc.end.column + (loc.start.column == loc.end.column)
          : line_len;
        /* Quote the file (at most the first line in the case of
           multiline locations).  */
        {
          fprintf (out, "%5d | %s", loc.start.line, skip ? "..." : "");
          /* Whether we opened the style.  If the line is not as
             expected (maybe the file was changed since the scanner
             ran), we might reach the end before we actually saw the
             opening column.  */
          bool opened = false;
          while (!mb_iseof (c) && !mb_iseq (c, '\n'))
            {
              if (caret_info.pos.column == loc.start.column)
                {
                  begin_use_class (style, out);
                  opened = true;
                }
              if (skip < caret_info.pos.column)
                mb_putc (c, out);
              boundary_compute (&caret_info.pos, mb_ptr (c), mb_len (c));
              caret_getc (c);
              if (opened
                  && (caret_info.pos.column == col_end
                      || width < caret_info.pos.column - skip))
                {
                  end_use_class (style, out);
                  opened = false;
                }
              if (width < caret_info.pos.column - skip)
                {
                  fprintf (out, "...");
                  break;
                }
            }
          putc ('\n', out);
        }

        /* Print the carets with the same indentation as above.  */
        {
          fprintf (out, "      | %s%*s",
                   skip ? "..." : "",
                   loc.start.column - 1 - skip, "");
          begin_use_class (style, out);
          putc ('^', out);
          /* Underlining a multiline location ends with the first
             line.  */
          for (int i = loc.start.column - 1 - skip + 1,
                 i_end = min_int (col_end - 1 - skip, width);
               i < i_end; ++i)
            putc ('~', out);
          end_use_class (style, out);
          putc ('\n', out);
        }
      }
  }
}

bool
location_empty (location loc)
{
  return !loc.start.file && !loc.start.line && !loc.start.column
    && !loc.end.file && !loc.end.line && !loc.end.column;
}

void
boundary_set_from_string (boundary *bound, char *str)
{
  /* Must search in reverse since the file name field may contain '.'
     or ':'.  */
  char *at = strrchr (str, '@');
  if (at)
    {
      *at = '\0';
      bound->byte = atoi (at+1);
    }
  {
    char *dot = strrchr (str, '.');
    aver (dot);
    *dot = '\0';
    bound->column = atoi (dot+1);
    if (!at)
      bound->byte = bound->column;
  }
  {
    char *colon = strrchr (str, ':');
    aver (colon);
    *colon = '\0';
    bound->line = atoi (colon+1);
  }
  bound->file = uniqstr_new (str);
}
