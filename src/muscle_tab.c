/* Muscle table manager for Bison.

   Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 Free Software
   Foundation, Inc.

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

#include <hash.h>
#include <quotearg.h>

#include "complain.h"
#include "files.h"
#include "muscle_tab.h"
#include "getargs.h"

/* A key-value pair, along with storage that can be reclaimed when
   this pair is no longer needed.  */
typedef struct
{
  char const *key;
  char const *value;
  char *storage;
} muscle_entry;

/* An obstack used to create some entries.  */
struct obstack muscle_obstack;

/* Initial capacity of muscles hash table.  */
#define HT_INITIAL_CAPACITY 257

static struct hash_table *muscle_table = NULL;

static bool
hash_compare_muscles (void const *x, void const *y)
{
  muscle_entry const *m1 = x;
  muscle_entry const *m2 = y;
  return strcmp (m1->key, m2->key) == 0;
}

static size_t
hash_muscle (const void *x, size_t tablesize)
{
  muscle_entry const *m = x;
  return hash_string (m->key, tablesize);
}

/*-----------------------------------------------------------------.
| Create the MUSCLE_TABLE, and initialize it with default values.  |
| Also set up the MUSCLE_OBSTACK.                                  |
`-----------------------------------------------------------------*/

static void
muscle_entry_free (void *entry)
{
  muscle_entry *mentry = entry;
  free (mentry->storage);
  free (mentry);
}

void
muscle_init (void)
{
  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  muscle_table = hash_initialize (HT_INITIAL_CAPACITY, NULL, hash_muscle,
				  hash_compare_muscles, muscle_entry_free);

  /* Version and input file.  */
  MUSCLE_INSERT_STRING ("version", VERSION);
  MUSCLE_INSERT_C_STRING ("file_name", grammar_file);
}


/*------------------------------------------------------------.
| Free all the memory consumed by the muscle machinery only.  |
`------------------------------------------------------------*/

void
muscle_free (void)
{
  hash_free (muscle_table);
  obstack_free (&muscle_obstack, NULL);
}



/*------------------------------------------------------------.
| Insert (KEY, VALUE).  If KEY already existed, overwrite the |
| previous value.                                             |
`------------------------------------------------------------*/

void
muscle_insert (char const *key, char const *value)
{
  muscle_entry probe;
  muscle_entry *entry;

  probe.key = key;
  entry = hash_lookup (muscle_table, &probe);

  if (!entry)
    {
      /* First insertion in the hash. */
      entry = xmalloc (sizeof *entry);
      entry->key = key;
      hash_insert (muscle_table, entry);
    }
  else
    free (entry->storage);
  entry->value = value;
  entry->storage = NULL;
}


/*-------------------------------------------------------------------.
| Append VALUE to the current value of KEY.  If KEY did not already  |
| exist, create it.  Use MUSCLE_OBSTACK.  De-allocate the previously |
| associated value.  Copy VALUE and SEPARATOR.                       |
`-------------------------------------------------------------------*/

void
muscle_grow (const char *key, const char *val, const char *separator)
{
  muscle_entry probe;
  muscle_entry *entry = NULL;

  probe.key = key;
  entry = hash_lookup (muscle_table, &probe);

  if (!entry)
    {
      /* First insertion in the hash. */
      entry = xmalloc (sizeof *entry);
      entry->key = key;
      hash_insert (muscle_table, entry);
      entry->value = entry->storage = xstrdup (val);
    }
  else
    {
      /* Grow the current value. */
      char *new_val;
      obstack_sgrow (&muscle_obstack, entry->value);
      free (entry->storage);
      obstack_sgrow (&muscle_obstack, separator);
      obstack_sgrow (&muscle_obstack, val);
      obstack_1grow (&muscle_obstack, 0);
      new_val = obstack_finish (&muscle_obstack);
      entry->value = entry->storage = xstrdup (new_val);
      obstack_free (&muscle_obstack, new_val);
    }
}


/*------------------------------------------------------------------.
| Append VALUE to the current value of KEY, using muscle_grow.  But |
| in addition, issue a synchronization line for the location LOC.   |
`------------------------------------------------------------------*/

void
muscle_code_grow (const char *key, const char *val, location loc)
{
  char *extension = NULL;
  obstack_fgrow1 (&muscle_obstack, "]b4_syncline(%d, [[", loc.start.line);
  MUSCLE_OBSTACK_SGROW (&muscle_obstack,
			quotearg_style (c_quoting_style, loc.start.file));
  obstack_sgrow (&muscle_obstack, "]])[\n");
  obstack_sgrow (&muscle_obstack, val);
  obstack_1grow (&muscle_obstack, 0);
  extension = obstack_finish (&muscle_obstack);
  muscle_grow (key, extension, "");
  obstack_free (&muscle_obstack, extension);
}


void muscle_pair_list_grow (const char *muscle,
			    const char *a1, const char *a2)
{
  char *pair;
  obstack_sgrow (&muscle_obstack, "[[[");
  MUSCLE_OBSTACK_SGROW (&muscle_obstack, a1);
  obstack_sgrow (&muscle_obstack, "]], [[");
  MUSCLE_OBSTACK_SGROW (&muscle_obstack, a2);
  obstack_sgrow (&muscle_obstack, "]]]");
  obstack_1grow (&muscle_obstack, 0);
  pair = obstack_finish (&muscle_obstack);
  muscle_grow (muscle, pair, ",\n");
  obstack_free (&muscle_obstack, pair);
}


/*----------------------------------------------------------------------------.
| Find the value of muscle KEY.  Unlike MUSCLE_FIND, this is always reliable  |
| to determine whether KEY has a value.                                       |
`----------------------------------------------------------------------------*/

char const *
muscle_find_const (char const *key)
{
  muscle_entry probe;
  muscle_entry *result = NULL;

  probe.key = key;
  result = hash_lookup (muscle_table, &probe);
  if (result)
    return result->value;
  return NULL;
}


/*----------------------------------------------------------------------------.
| Find the value of muscle KEY.  Abort if muscle_insert was invoked more      |
| recently than muscle_grow for KEY since muscle_find can't return a          |
| char const *.                                                               |
`----------------------------------------------------------------------------*/

char *
muscle_find (char const *key)
{
  muscle_entry probe;
  muscle_entry *result = NULL;

  probe.key = key;
  result = hash_lookup (muscle_table, &probe);
  if (result)
    {
      aver (result->value == result->storage);
      return result->storage;
    }
  return NULL;
}


void
muscle_boundary_grow (char const *key, boundary bound)
{
  char *extension;
  MUSCLE_OBSTACK_SGROW (&muscle_obstack, bound.file);
  obstack_1grow (&muscle_obstack, ':');
  obstack_fgrow1 (&muscle_obstack, "%d", bound.line);
  obstack_1grow (&muscle_obstack, '.');
  obstack_fgrow1 (&muscle_obstack, "%d", bound.column);
  obstack_1grow (&muscle_obstack, '\0');
  extension = obstack_finish (&muscle_obstack);
  muscle_grow (key, extension, "");
  obstack_free (&muscle_obstack, extension);
}

void
muscle_location_grow (char const *key, location loc)
{
  muscle_grow (key, "[[", "");
  muscle_boundary_grow (key, loc.start);
  muscle_grow (key, "]], [[", "");
  muscle_boundary_grow (key, loc.end);
  muscle_grow (key, "]]", "");
}

/* Reverse of muscle_location_grow.  */
static location
muscle_location_decode (char const *key)
{
  location loc;
  char const *value = muscle_find_const (key);
  aver (value);
  aver (*value == '[');
  aver (*++value == '[');
  while (*++value)
    switch (*value)
      {
        case '$':
          aver (*++value == ']');
          aver (*++value == '[');
          obstack_sgrow (&muscle_obstack, "$");
          break;
        case '@':
          switch (*++value)
            {
              case '@': obstack_sgrow (&muscle_obstack, "@" ); break;
              case '{': obstack_sgrow (&muscle_obstack, "[" ); break;
              case '}': obstack_sgrow (&muscle_obstack, "]" ); break;
              default: aver (false); break;
            }
          break;
        case '[':
          aver (false);
          break;
        case ']':
          {
            char *boundary_str;
            aver (*++value == ']');
            obstack_1grow (&muscle_obstack, '\0');
            boundary_str = obstack_finish (&muscle_obstack);
            switch (*++value)
              {
                case ',':
                  boundary_set_from_string (&loc.start, boundary_str);
                  obstack_free (&muscle_obstack, boundary_str);
                  aver (*++value == ' ');
                  aver (*++value == '[');
                  aver (*++value == '[');
                  break;
                case '\0':
                  boundary_set_from_string (&loc.end, boundary_str);
                  obstack_free (&muscle_obstack, boundary_str);
                  return loc;
                  break;
                default:
                  aver (false);
                  break;
              }
          }
          break;
        default:
          obstack_1grow (&muscle_obstack, *value);
          break;
      }
  aver (false);
  return loc;
}

void
muscle_user_name_list_grow (char const *key, char const *user_name,
                            location loc)
{
  muscle_grow (key, "[[[[", ",");
  muscle_grow (key, user_name, "");
  muscle_grow (key, "]], ", "");
  muscle_location_grow (key, loc);
  muscle_grow (key, "]]", "");
}

#define MUSCLE_USER_NAME_CONVERT(NAME, PREFIX, USER_NAME, SUFFIX)    \
do {                                                                 \
  char *tmp;                                                         \
  size_t length = strlen ((USER_NAME));                              \
  tmp = xmalloc (sizeof (PREFIX) - 1 + length + sizeof (SUFFIX));    \
  strcpy (tmp, (PREFIX));                                            \
  strcpy (tmp + sizeof (PREFIX) - 1, (USER_NAME));                   \
  strcpy (tmp + sizeof (PREFIX) - 1 + length, (SUFFIX));             \
  (NAME) = uniqstr_new (tmp);                                        \
  free (tmp);                                                        \
} while (0)

void
muscle_percent_define_insert (char const *variable, location variable_loc,
                              char const *value)
{
  char const *name;
  char const *loc_name;

  MUSCLE_USER_NAME_CONVERT (name, "percent_define(", variable, ")");
  MUSCLE_USER_NAME_CONVERT (loc_name, "percent_define_loc(", variable, ")");

  if (muscle_find_const (name))
    {
      warn_at (variable_loc, _("%s `%s' redefined"),
               "%define variable", variable);
      warn_at (muscle_location_decode (loc_name), _("previous definition"));
    }
  MUSCLE_INSERT_STRING (name, value);

  muscle_insert (loc_name, "");
  muscle_location_grow (loc_name, variable_loc);
  muscle_user_name_list_grow ("percent_define_user_variables", variable,
                              variable_loc);
}

bool
muscle_percent_define_flag_if (char const *variable)
{
  char const *name;
  char const *loc_name;
  char const *usage_name;
  char const *value;
  bool result = false;

  MUSCLE_USER_NAME_CONVERT (name, "percent_define(", variable, ")");
  MUSCLE_USER_NAME_CONVERT (loc_name, "percent_define_loc(", variable, ")");
  MUSCLE_USER_NAME_CONVERT (usage_name, "percent_define_bison_variables(",
                            variable, ")");

  value = muscle_find_const (name);
  if (value)
    {
      if (value[0] == '\0' || 0 == strcmp (value, "true"))
        result = true;
      else if (0 == strcmp (value, "false"))
        result = false;
      else if (!muscle_find_const (usage_name))
        complain_at(muscle_location_decode (loc_name),
                    _("invalid value for %%define boolean variable `%s'"),
                    variable);
    }
  else
    fatal(_("undefined %%define variable `%s' passed to muscle_percent_define_flag_if"),
          variable);

  muscle_insert (usage_name, "");

  return result;
}

void
muscle_percent_define_default (char const *variable, char const *value)
{
  char const *name;
  char const *loc_name;
  MUSCLE_USER_NAME_CONVERT (name, "percent_define(", variable, ")");
  MUSCLE_USER_NAME_CONVERT (loc_name, "percent_define_loc(", variable, ")");
  if (!muscle_find_const (name))
    {
      location loc;
      MUSCLE_INSERT_STRING (name, value);
      loc.start.file = loc.end.file = "[Bison:muscle_percent_define_default]";
      loc.start.line = loc.start.column = 0;
      loc.end.line = loc.end.column = 0;
      muscle_insert (loc_name, "");
      muscle_location_grow (loc_name, loc);
    }
}

void
muscle_percent_code_grow (char const *qualifier, location qualifier_loc,
                          char const *code, location code_loc)
{
  char const *name;
  MUSCLE_USER_NAME_CONVERT (name, "percent_code(", qualifier, ")");
  muscle_code_grow (name, code, code_loc);
  muscle_user_name_list_grow ("percent_code_user_qualifiers", qualifier,
                               qualifier_loc);
}


/*------------------------------------------------.
| Output the definition of ENTRY as a m4_define.  |
`------------------------------------------------*/

static inline bool
muscle_m4_output (muscle_entry *entry, FILE *out)
{
  fprintf (out, "m4_define([b4_%s],\n", entry->key);
  fprintf (out, "[[%s]])\n\n\n", entry->value);
  return true;
}

static bool
muscle_m4_output_processor (void *entry, void *out)
{
  return muscle_m4_output (entry, out);
}


/*----------------------------------------------------------------.
| Output the definition of all the current muscles into a list of |
| m4_defines.                                                     |
`----------------------------------------------------------------*/

void
muscles_m4_output (FILE *out)
{
  hash_do_for_each (muscle_table, muscle_m4_output_processor, out);
}
