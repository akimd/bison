/* Copyright (C) 2001 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#if STDC_HEADERS || HAVE_STRING_H
# include <string.h>
#else
# include <strings.h>
#endif

#include <sys/types.h>

#include "xalloc.h"

#ifndef HAVE_DECL_STRNLEN
"this configure-time declaration test was not run"
#endif
#if !HAVE_DECL_STRNLEN
size_t strnlen ();
#endif

char *xstrndup (const char *s, size_t n);

char *
xstrndup (const char *s, size_t n)
{
  size_t len = strnlen (s, n);
  char *new = xmalloc (len + 1);

  if (new == NULL)
    return NULL;

  new[len] = '\0';
  return (char *) memcpy (new, s, len);
}
