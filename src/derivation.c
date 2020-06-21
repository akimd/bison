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

#include "derivation.h"

#include <gl_linked_list.h>

#include "system.h"
#include "complain.h"

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
  derivation *d = NULL;
  for (gl_list_iterator_t it = gl_list_iterator (dl);
       derivation_list_next (&it, &d);
       )
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
              derivation *child = NULL;
              for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
                   derivation_list_next (&it, &child);
                   )
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
  derivation *child = NULL;
  for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
       derivation_list_next (&it, &child);
       )
    size += derivation_size (child);
  return size;
}

/* Print DERIV, colored according to COUNTER.
   Return false if nothing is printed.  */
static bool
derivation_print_impl (const derivation *deriv, FILE *f,
                       bool leaves_only,
                       int *counter, const char *prefix)
{
  if (deriv->children)
    {
      const symbol *sym = symbols[deriv->sym];
      char style[20];
      snprintf (style, 20, "cex-%d", *counter);
      ++*counter;
      begin_use_class (style, f);

      if (!leaves_only)
        {
          fputs (prefix, f);
          begin_use_class ("cex-step", f);
          fprintf (f, "%s ::=[ ", sym->tag);
          end_use_class ("cex-step", f);
          prefix = "";
        }
      bool res = false;
      derivation *child;
      for (gl_list_iterator_t it = gl_list_iterator (deriv->children);
           derivation_list_next (&it, &child);
           )
        {
          if (derivation_print_impl (child, f, leaves_only, counter, prefix))
            {
              prefix = " ";
              res = true;
            }
          else if (!leaves_only)
            prefix = " ";
        }
      if (!leaves_only)
        {
          begin_use_class ("cex-step", f);
          if (res)
            fputs (" ]", f);
          else
            fputs ("]", f);
          end_use_class ("cex-step", f);
        }
      end_use_class (style, f);
      return res;
    }
  else if (deriv == &d_dot)
    {
      fputs (prefix, f);
      begin_use_class ("cex-dot", f);
      print_dot (f);
      end_use_class ("cex-dot", f);
    }
  else // leaf.
    {
      fputs (prefix, f);
      const symbol *sym = symbols[deriv->sym];
      begin_use_class ("cex-leaf", f);
      fprintf (f, "%s", sym->tag);
      end_use_class ("cex-leaf", f);
    }
  return true;
}

void
derivation_print (const derivation *deriv, FILE *out, const char *prefix)
{
  int counter = 0;
  fputs (prefix, out);
  derivation_print_impl (deriv, out, false, &counter, "");
  fputc ('\n', out);
}


void
derivation_print_leaves (const derivation *deriv, FILE *out, const char *prefix)
{
  int counter = 0;
  fputs (prefix, out);
  derivation_print_impl (deriv, out, true, &counter, "");
  fputc ('\n', out);
}
