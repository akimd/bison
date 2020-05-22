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
#include <gl_linked_list.h>

#include "system.h"

#include "derivation.h"

struct derivation
{
  symbol_number sym;
  gl_list_t children;
  int reference_count;
};

static derivation d_dot = { -1, NULL, -1 };

derivation *
derivation_dot (void)
{
  return &d_dot;
}

void
derivation_list_append (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_last (dl, d);
}

void
derivation_list_prepend (derivation_list dl, derivation *d)
{
  derivation_retain (d);
  gl_list_add_first (dl, d);
}

void derivation_list_free (derivation_list dl)
{
  gl_list_iterator_t it = gl_list_iterator (dl);
  derivation *d;
  while (gl_list_iterator_next (&it, (const void **) &d, NULL))
    if (d != &d_dot)
      derivation_free (d);
  gl_list_free (dl);
}

derivation *
derivation_new (symbol_number sym, derivation_list children)
{
  derivation *deriv = xmalloc (sizeof (derivation));
  deriv->sym = sym;
  deriv->children = children;
  deriv->reference_count = 0;
  return deriv;
}

void
derivation_retain (derivation *d)
{
  ++d->reference_count;
}

void
derivation_free (derivation *d)
{
  if (!d)
    return;
  derivation_list free_queue =
    gl_list_create (GL_LINKED_LIST, NULL, NULL, NULL, true,
                    1, (const void **)&d);
  while (gl_list_size (free_queue) > 0)
    {
      derivation *deriv = (derivation *) gl_list_get_at (free_queue, 0);
      if (--deriv->reference_count == 0)
        {
          if (deriv->children)
            {
              gl_list_iterator_t it = gl_list_iterator (deriv->children);
              derivation *child;
              while (gl_list_iterator_next (&it, (const void **) &child, NULL))
                if (child != &d_dot)
                  gl_list_add_last (free_queue, child);
              gl_list_free (deriv->children);
            }
          free (deriv);
        }
      gl_list_remove_at (free_queue, 0);
    }
  gl_list_free (free_queue);
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
