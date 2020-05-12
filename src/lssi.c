/* Lookahead sensative state item searches for counterexample generation
 
 Copyright (C) 2019-2020 Free Software Foundation, Inc.
 
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
#include <gl_xlist.h>
#include <stdlib.h>
#include "lssi.h"
#include "nullable.h"
#include "getargs.h"

// lookahead sensative state item
struct lssi;
typedef struct lssi
{
  state_item_number si;
  struct lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

lssi *
new_lssi (state_item_number si, lssi *p, bitset l, bool free_l)
{
  lssi *ret = xmalloc (sizeof (lssi));
  ret->si = si;
  ret->parent = p;
  ret->lookahead = l;
  ret->free_lookahead = free_l;
  return ret;
}

void
lssi_free (lssi *sn)
{
  if (sn == NULL)
    return;
  if (sn->free_lookahead)
    bitset_free (sn->lookahead);
  free (sn);
}

static size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  symbol_number syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  return hash % max;
}

static bool
lssi_comparator (lssi *s1, lssi *s2)
{
  if (s1->si == s2->si)
    {
      if (s1->lookahead == s2->lookahead)
        return true;
      return bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  return false;
}

static inline bool
append_lssi (lssi *sn, Hash_table *visited, gl_list_t queue)
{
  if (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      return false;
    }
  if (!hash_insert (visited, sn))
    xalloc_die ();
  gl_list_add_last (queue, sn);
  return true;
}

static void
lssi_print (lssi *l)
{
  print_state_item (state_items + l->si, stdout);
  if (l->lookahead)
    {
      printf ("FOLLOWL = { ");
      bitset_iterator biter;
      symbol_number sin;
      BITSET_FOR_EACH (biter, l->lookahead, sin, 0)
        printf ("%s, \n", symbols[sin]->tag);
      puts ("}");
    }
}

/**
 * Compute the set of state-items that can reach the given conflict item via
 * a combination of transitions or production steps.
 */
bitset
eligible_state_items (state_item *target)
{
  bitset result = bitset_create (nstate_items, BITSET_FIXED);
  gl_list_t queue =
    gl_list_create (GL_LINKED_LIST, NULL, NULL, NULL, true, 1,
                    (const void **) &target);
  while (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      if (bitset_test (result, si - state_items))
        continue;
      bitset_set (result, si - state_items);
      // search all reverse edges.
      bitset rsi = si_revs[si - state_items];
      bitset_iterator biter;
      state_item_number sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_items[sin]);
    }
  gl_list_free (queue);
  return result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
gl_list_t
shortest_path_from_start (state_item_number target, symbol_number next_sym)
{
  bitset eligible = eligible_state_items (&state_items[target]);
  Hash_table *visited = hash_initialize (32,
                                         NULL,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, NULL, il, true);
  gl_list_t queue = gl_list_create (GL_LINKED_LIST, NULL, NULL,
                                    NULL,
                                    true, 1, (const void **) &init);
  // breadth-first search
  bool finished = false;
  lssi *n;
  while (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_number last = n->si;
      if (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          break;
        }
      // Transitions don't change follow_L
      if (si_trans[last] >= 0)
        {
          state_item_number nextSI = si_trans[last];
          if (bitset_test (eligible, nextSI))
            {
              lssi *next = new_lssi (nextSI, n, n->lookahead, false);
              append_lssi (next, visited, queue);
            }
        }
      // For production steps, follow_L is based on the symbol after the
      // non-terminal being produced.
      // if no such symbol exists, follow_L is unchanged
      // if the symbol is a terminal, follow_L only contains that terminal
      // if the symbol is not nullable, follow_L is its FIRSTS set
      // if the symbol is nullable, follow_L is its FIRSTS set unioned with
      // this logic applied to the next symbol in the rule
      bitset p = si_prods_lookup (last);
      if (p)
        {
          state_item si = state_items[last];
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_number *pos = si.item + 1;
          for (; !item_number_is_rule_number (*pos); ++pos)
            {
              item_number it = *pos;
              if (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  break;
                }
              else
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  if (!nullable[it - ntokens])
                    break;
                }
            }
          if (item_number_is_rule_number (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_number nextSI;
          BITSET_FOR_EACH (biter, p, nextSI, 0)
            {
              if (!bitset_test (eligible, nextSI))
                continue;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          if (!lookahead_used)
            bitset_free (lookahead);
        }
    }
  if (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      exit (1);
    }
  gl_list_t ret =
    gl_list_create_empty (GL_LINKED_LIST, NULL, NULL, NULL, true);
  for (lssi *sn = n; sn != NULL; sn = sn->parent)
    gl_list_add_first (ret, state_items + sn->si);

  hash_free (visited);
  gl_list_free (queue);

  if (trace_flag & trace_cex)
    {
      puts ("REDUCE ITEM PATH:");
      gl_list_iterator_t it = gl_list_iterator (ret);
      const void *sip;
      while (gl_list_iterator_next (&it, &sip, NULL))
        print_state_item ((state_item *) sip, stdout);
    }
  return ret;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_symbol (symbol_number sym, bitset syms)
{
  if (!syms)
    return true;
  bitset_iterator biter;
  symbol_number sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      if (sym == sn)
        return true;
      if (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        return true;
    }
  return false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  if (!syms || !ts)
    return true;
  bitset_iterator biter;
  symbol_number sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      if (bitset_test (ts, sn))
        return true;
      if (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        return true;
    }
  return false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
gl_list_t
lssi_reverse_production (const state_item *si, bitset lookahead)
{
  gl_list_t result =
    gl_list_create_empty (GL_LINKED_LIST, NULL, NULL, NULL, true);
  if (SI_TRANSITION (si))
    return result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_number sin;
  BITSET_FOR_EACH (biter, si_revs[si - state_items], sin, 0)
  {
    state_item *prevsi = state_items + sin;
    if (!production_allowed (prevsi, si))
      continue;
    bitset prev_lookahead = prevsi->lookahead;
    if (item_number_is_rule_number (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        if (!intersect (prev_lookahead, lookahead))
          continue;
      }
    else
      {
        // shift item
        if (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            for (item_number *pos = prevsi->item + 1;
                 !applicable && nlable && item_number_is_symbol_number (*pos);
                 ++pos)
              {
                symbol_number next_sym = item_number_as_symbol_number (*pos);
                if (ISTOKEN (next_sym))
                  {
                    applicable = intersect_symbol (next_sym, lookahead);
                    nlable = false;
                  }
                else
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    if (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            if (!applicable && !nlable)
              continue;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  return result;
}
