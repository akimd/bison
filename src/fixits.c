/* Support for fixing grammar files.

   Copyright (C) 2019 Free Software Foundation, Inc.

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

#include "fixits.h"

#include "system.h"

#include "getargs.h"
#include "gl_xlist.h"
#include "gl_array_list.h"
#include "quotearg.h"

typedef struct
{
  location location;
  char *fix;
} fixit;

gl_list_t fixits = NULL;

static fixit *
fixit_new (location const *loc, char const* fix)
{
  fixit *res = xmalloc (sizeof *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  return res;
}


static void
fixit_free (fixit *f)
{
  free (f->fix);
  free (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   http://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
static void
fixit_print (fixit const *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.line, f->location.start.column,
           f->location.end.line, f->location.end.column,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


void
fixits_register (location const *loc, char const* fix)
{
  if (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ NULL,
                                   /* hashcode */ NULL,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_list_add_last (fixits, f);
  if (feature_flag & feature_fixit_parsable)
    fixit_print (f, stderr);
}


/* Free the registered fixits.  */
void fixits_free (void)
{
  if (fixits)
    {
      gl_list_free (fixits);
      fixits = NULL;
    }
}
