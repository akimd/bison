/* Counterexample derivation trees

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

#include <config.h>

#include "system.h"

#include "derivation.h"

static derivation d_dot = { -1, NULL };

const derivation *
derivation_dot (void)
{
  return &d_dot;
}

derivation *
derivation_new (symbol_number sym, gl_list_t children)
{
  derivation *deriv = xmalloc (sizeof (derivation));
  deriv->sym = sym;
  deriv->children = children;
  return deriv;
}

void
derivation_free (derivation *d)
{
  if (d && d != &d_dot)
    {
      if (d->children)
        gl_list_free (d->children);
      free (d);
    }
}

size_t
derivation_size (const derivation *deriv)
{
  if (!deriv->children)
    return 1;
  int size = 1;
  gl_list_iterator_t it = gl_list_iterator (deriv->children);
  derivation *child;
  while (gl_list_iterator_next (&it, (const void **) &child, NULL))
    size += derivation_size (child);
  return size;
}

// these are used rarely enough that I don't think they should be interned.
void
derivation_print (const derivation *deriv, FILE *f)
{
  if (deriv == &d_dot)
    {
      fputs (" •", f);
      return;
    }
  symbol *sym = symbols[deriv->sym];
  if (!deriv->children)
    {
      fprintf (f, " %s", sym->tag);
      return;
    }
  gl_list_iterator_t it = gl_list_iterator (deriv->children);
  derivation *child;
  fprintf (f, " %s ::=[", sym->tag);
  while (gl_list_iterator_next (&it, (const void **) &child, NULL))
    {
      derivation_print (child, f);
      fputs (" ", f);
    }
  fputs ("]", f);
}

void
derivation_print_leaves (const derivation *deriv, FILE *f)
{
  if (deriv == &d_dot)
    {
      fputs ("•", f);
      return;
    }
  if (!deriv->children)
    {
      symbol *sym = symbols[deriv->sym];
      fprintf (f, "%s", sym->tag);
      return;
    }

  gl_list_iterator_t it = gl_list_iterator (deriv->children);
  const char *sep = "";
  derivation *child;
  while (gl_list_iterator_next (&it, (const void **) &child, NULL))
    {
      fputs (sep, f);
      sep = "  ";
      derivation_print_leaves (child, f);
    }
}
