/* Conflict counterexample generation
 
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
#include <time.h>
#include <hash.h>
#include <stdlib.h>
#include <gl_linked_list.h>
#include <gl_xlist.h>
#include <gl_rbtreehash_list.h>
#include "counterexample.h"
#include "derivation.h"
#include "gram.h"
#include "complain.h"
#include "closure.h"
#include "nullable.h"
#include "getargs.h"
#include "lalr.h"
#include "parse-simulation.h"
#include "lssi.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_SEARCH false

/* costs for making various steps in a search */
#define PRODUCTION_COST 50
#define REDUCE_COST 1
#define SHIFT_COST 1
#define UNSHIFT_COST 1
#define EXTENDED_COST 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#define ASSURANCE_LIMIT 2.0f

/* The time limit before giving up looking for unifying counterexample. */
#define TIME_LIMIT 5.0f

#define CUMULATIVE_TIME_LIMIT 120.0f

typedef struct
{
  const derivation *d1;
  const derivation *d2;
  bool unifying;
  bool timeout;
} counterexample;

counterexample *
new_counterexample (const derivation *d1, const derivation *d2,
                    bool u, bool t)
{
  counterexample *ret = xmalloc (sizeof (counterexample));
  ret->d1 = d1;
  ret->d2 = d2;
  ret->unifying = u;
  ret->timeout = t;
  return ret;
}

void
print_counterexample (counterexample *cex)
{
  if (cex->unifying)
    printf ("Example ");
  else
    printf ("First  Example");
  derivation_print_leaves (cex->d1, stdout);
  printf ("\nFirst  derivation ");
  derivation_print (cex->d1, stdout);
  if (!cex->unifying)
    {
      printf ("\nSecond Example ");
      derivation_print_leaves (cex->d2, stdout);
    }
  printf ("\nSecond derivation ");
  derivation_print (cex->d2, stdout);
  fputs ("\n\n", stdout);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
struct si_bfs_node;
typedef struct si_bfs_node
{
  state_item_number si;
  struct si_bfs_node *parent;
  int reference_count;
} si_bfs_node;

si_bfs_node *
si_bfs_new (state_item_number si, si_bfs_node *parent)
{
  si_bfs_node *ret = xcalloc (1, sizeof (si_bfs_node));
  ret->si = si;
  ret->parent = parent;
  ret->reference_count = 1;
  if (parent)
    ++parent->reference_count;
  return ret;
}

static bool
si_bfs_contains (const si_bfs_node *n, state_item_number sin)
{
  for (const si_bfs_node *search = n; search != NULL; search = search->parent)
    if (search->si == sin)
      return true;
  return false;
}

void
si_bfs_free (si_bfs_node *n)
{
  if (n == NULL)
    return;
  if (n->reference_count == 0)
    {
      if (n->parent)
        {
          --n->parent->reference_count;
          si_bfs_free (n->parent);
        }
      free (n);
    }
}

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * non-terminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the non-terminal and shifting over nullable non-terminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
static inline gl_list_t
expand_to_conflict (state_item_number start, symbol_number conflict_sym)
{
  si_bfs_node *init = si_bfs_new (start, NULL);

  gl_list_t queue = gl_list_create (GL_LINKED_LIST, NULL, NULL,
                                    (gl_listelement_dispose_fn) si_bfs_free,
                                    true, 1, (const void **) &init);
  si_bfs_node *node;
  // breadth-first search for a path of productions to the conflict symbol
  while (gl_list_size (queue) > 0)
    {
      node = (si_bfs_node *) gl_list_get_at (queue, 0);
      state_item *silast = state_items + node->si;
      symbol_number sym = item_number_as_symbol_number (*(silast->item));
      if (sym == conflict_sym)
        break;
      if (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_number sin;
          bitset sib = si_prods_lookup (node->si);
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              if (si_bfs_contains (node, sin))
                continue;
              si_bfs_node *next = si_bfs_new (sin, node);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          if (nullable[sym - ntokens])
            {
              si_bfs_node *next = si_bfs_new (si_trans[node->si], node);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  if (gl_list_size (queue) == 0)
    {
      gl_list_free (queue);
      fputs ("Error expanding derivation\n", stderr);
      exit (1);
    }

  derivation *dinit = derivation_new (conflict_sym, NULL);
  gl_list_t result =
    gl_list_create (GL_LINKED_LIST, NULL, NULL,
                    (gl_listelement_dispose_fn) derivation_free,
                    true, 1, (const void **) &dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  for (si_bfs_node *n = node; n != NULL; n = n->parent)
    {
      state_item *si = state_items + n->si;
      item_number *pos = si->item;
      if (SI_PRODUCTION (si))
        {
          item_number *i;
          for (i = pos + 1; !item_number_is_rule_number (*i); ++i)
            gl_list_add_last (result, derivation_new (*i, NULL));
          symbol_number lhs =
            rules[item_number_as_rule_number (*i)].lhs->number;
          derivation *deriv = derivation_new (lhs, result);
          result =
            gl_list_create (GL_LINKED_LIST, NULL, NULL,
                            (gl_listelement_dispose_fn) derivation_free,
                            true, 1, (const void **) &deriv);
        }
      else
        {
          symbol_number sym = item_number_as_symbol_number (*(pos - 1));
          derivation *deriv = derivation_new (sym, NULL);
          gl_list_add_first (result, deriv);
        }
    }
  gl_list_free (queue);
  gl_list_remove_at (result, 0);
  return result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ] 
 */
static derivation *
complete_diverging_example (symbol_number conflict_sym,
                            gl_list_t states, gl_list_t derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  gl_list_t result =
    gl_list_create_empty (GL_LINKED_LIST, NULL, NULL, NULL, true);
  bool lookahead_required = false;
  if (!derivs)
    {
      derivs = gl_list_create_empty (GL_LINKED_LIST, NULL, NULL, NULL, true);
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }
  // This is the fastest way to get the tail node from the gl_list API
  gl_list_node_t tmpd = gl_list_add_last (derivs, NULL);
  gl_list_node_t tmps = gl_list_add_last (states, NULL);
  gl_list_node_t deriv = gl_list_previous_node (derivs, tmpd);
  gl_list_node_t state = gl_list_previous_node (states, tmps);
  gl_list_remove_node (derivs, tmpd);
  gl_list_remove_node (states, tmps);
  // we go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the non-terminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  for (; state != NULL; state = gl_list_previous_node (states, state))
    {
      state_item *si = (state_item *) gl_list_node_value (states, state);
      item_number *item = si->item;
      item_number pos = *item;
      // symbols after dot
      if (gl_list_size (result) == 1 && !item_number_is_rule_number (pos)
          && gl_list_get_at (result, 0) == derivation_dot() )
        {
          gl_list_add_last (result,
                            derivation_new (item_number_as_symbol_number
                                            (pos), NULL));
          lookahead_required = false;
        }
      item_number *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      for (state_item_number nsi = si - state_items;
           !item_number_is_rule_number (*i); ++i, nsi = si_trans[nsi])
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          if (i == item)
            continue;
          symbol_number sym = item_number_as_symbol_number (*i);
          if (!lookahead_required || sym == conflict_sym)
            {
              gl_list_add_last (result, derivation_new (sym, NULL));
              lookahead_required = false;
              continue;
            }
          // Since states is a path to the conflict state-item,
          // for a reduce conflict item, we will want to have a derivation
          // that shows the conflict symbol from its lookahead set being used.
          //
          // Since reductions have the dot at the end of the item,
          // this loop will be first executed on the last item in the path
          // that's not a reduction. When that happens,
          // the symbol after the dot should be a non-terminal,
          // and we can look through successive nullable non-terminals
          // for one with the conflict symbol in its first set.
          if (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              gl_list_t next_derivs = expand_to_conflict (nsi,
                                                          conflict_sym);
              gl_list_iterator_t it = gl_list_iterator (next_derivs);
              derivation *d;
              while (gl_list_iterator_next (&it, (const void **) &d, NULL))
                gl_list_add_last (result, d);
              i += gl_list_size (next_derivs) - 1;
              // next_derivs is leaked to prevent the copied derivations from
              // being freed. It is somewhat difficult to for expand_to_conflict
              // to decide which derivation list will be the final one that is
              // returned, so each list frees its derivations when freed.
            }
          else if (nullable[sym - ntokens])
            {
              derivation *d = derivation_new (sym,
                                              gl_list_create_empty
                                              (GL_LINKED_LIST, NULL,
                                               NULL, NULL, 1));
              gl_list_add_last (result, d);
            }
          else
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              gl_list_add_last (result, derivation_new (1, NULL));
              lookahead_required = false;
            }
        }
      rule_number r = item_number_as_rule_number (*i);
      // add derivations for symbols before dot
      for (i = item - 1; !item_number_is_rule_number (*i) && i >= ritem; i--)
        {
          gl_list_node_t p = gl_list_previous_node (states, state);
          if (p)
            state = p;
          if (deriv)
            {
              const void *tmp_deriv = gl_list_node_value (derivs, deriv);
              deriv = gl_list_previous_node (derivs, deriv);
              gl_list_add_first (result, tmp_deriv);
            }
          else
            gl_list_add_first (result, derivation_new (*i, NULL));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (rules[r].lhs->number, result);
      result =
        gl_list_create (GL_LINKED_LIST, NULL, NULL, NULL, true, 1,
                        (const void **) &new_deriv);
    }
  derivation *ret = (derivation *) gl_list_get_at (result, 0);
  gl_list_free (result);
  return ret;
}

/* iterates backwards through the shifts of the path in the
  reduce conflict, and finds a path of shifts from the shift
  conflict that goes through the same states.
 */
gl_list_t
nonunifying_shift_path (gl_list_t reduce_path, state_item *shift_conflict)
{
  gl_list_node_t tmp = gl_list_add_last (reduce_path, NULL);
  gl_list_node_t next_node = gl_list_previous_node (reduce_path, tmp);
  gl_list_node_t node = gl_list_previous_node (reduce_path, next_node);
  gl_list_remove_node (reduce_path, tmp);
  state_item *si = shift_conflict;
  gl_list_t result =
    gl_list_create_empty (GL_LINKED_LIST, NULL, NULL, NULL, true);
  bool paths_merged;
  for (; node != NULL; next_node = node,
       node = gl_list_previous_node (reduce_path, node))
    {
      state_item *refsi =
        (state_item *) gl_list_node_value (reduce_path, node);
      state_item *nextrefsi =
        (state_item *) gl_list_node_value (reduce_path, next_node);
      if (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          continue;
        }
      // skip reduction items
      if (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        continue;

      // bfs to find a shift to the right state
      si_bfs_node *init = si_bfs_new (si - state_items, NULL);
      gl_list_t queue =
        gl_list_create (GL_LINKED_LIST, NULL, NULL,
                        (gl_listelement_dispose_fn) si_bfs_free,
                        true, 1, (const void **) &init);
      si_bfs_node *sis = NULL;
      state_item *prevsi = NULL;
      while (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_node *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          if (sis->si == 0)
            break;

          state_item *search_si = state_items + sis->si;
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = si_revs[sis->si];
          bitset_iterator biter;
          state_item_number sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = state_items + sin;
              if (SI_TRANSITION (search_si))
                {
                  if (prevsi->state == refsi->state)
                    goto search_end;
                }
              else if (!si_bfs_contains (sis, sin))
                {
                  si_bfs_node *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    search_end:
      // prepend path to shift we found
      if (sis)
        {
          gl_list_node_t ln = gl_list_add_first (result, state_items + sis->si);
          for (si_bfs_node *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, state_items + n->si);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  if (trace_flag & trace_cex)
    {
      puts ("SHIFT ITEM PATH:");
      gl_list_iterator_t it = gl_list_iterator (result);
      state_item *sip;
      while (gl_list_iterator_next (&it, (const void **) &sip, NULL))
        print_state_item (sip, stdout);
    }
  return result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
counterexample *
example_from_path (bool shift_reduce,
                   state_item_number itm2,
                   gl_list_t shortest_path, symbol_number next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, NULL);
  gl_list_t path_2 = shift_reduce ? nonunifying_shift_path (shortest_path,
                                                            &state_items
                                                            [itm2]) :
    shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, NULL);
  gl_list_free (path_2);
  return new_counterexample (deriv1, deriv2, false, true);
}

/*
 *
 * UNIFYING COUNTER EXAMPLES
 *
 */

/* A search state keeps track of two parser simulations,
 * one starting at each conflict. Complexity is a metric
 * which sums different parser actions with varying weights.
 */
typedef struct
{
  parse_state *states[2];
  int complexity;
} search_state;

static search_state *
initial_search_state (state_item *conflict1, state_item *conflict2)
{
  search_state *ret = xmalloc (sizeof (search_state));
  ret->states[0] = new_parse_state (conflict1);
  ret->states[1] = new_parse_state (conflict2);
  parse_state_retain (ret->states[0]);
  parse_state_retain (ret->states[1]);
  ret->complexity = 0;
  return ret;
}

static search_state *
new_search_state (parse_state *ps1, parse_state *ps2, int complexity)
{
  search_state *ret = xmalloc (sizeof (search_state));
  ret->states[0] = ps1;
  ret->states[1] = ps2;
  parse_state_retain (ret->states[0]);
  parse_state_retain (ret->states[1]);
  ret->complexity = complexity;
  return ret;
}

static search_state *
copy_search_state (search_state *parent)
{
  search_state *copy = xmalloc (sizeof (search_state));
  memcpy (copy, parent, sizeof (search_state));
  parse_state_retain (copy->states[0]);
  parse_state_retain (copy->states[1]);
  return copy;
}

static void
search_state_free_children (search_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

static void
search_state_free (search_state *ss)
{
  if (ss == NULL)
    return;
  search_state_free_children (ss);
  free (ss);
}

static void
search_state_print (search_state *ss)
{
  fputs ("CONFLICT 1 ", stdout);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stdout);
  print_parse_state (ss->states[1]);
  putc ('\n', stdout);
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
static inline void
ss_set_parse_state (search_state *ss, int index, parse_state *ps)
{
  free_parse_state (ss->states[index]);
  ss->states[index] = ps;
  parse_state_retain (ps);
}

/*
 * Construct a nonunifying example from a search state
 * which has its parse states unified at the beginning
 * but not the end of the example.
 */
counterexample *
complete_diverging_examples(search_state *ss,
                            symbol_number next_sym)
{
  derivation *new_derivs[2];
  for (int i = 0; i < 2; ++i)
      {
        gl_list_t state_items, derivs;
        parse_state_lists (ss->states[i], &state_items, &derivs);
        new_derivs[i] =
          complete_diverging_example (next_sym, state_items, derivs);
      }
    return new_counterexample (new_derivs[0], new_derivs[1], false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
typedef struct
{
  gl_list_t states;
  int complexity;
} search_state_bundle;
void
ssb_free (search_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  free(ssb);
}

static size_t
ssb_hasher (search_state_bundle *ssb)
{
  return ssb->complexity;
}

static int
ssb_comp (const search_state_bundle *s1, const search_state_bundle *s2)
{
  return s1->complexity - s2->complexity;
}

static bool
ssb_equals (const search_state_bundle *s1, const search_state_bundle *s2)
{
  return s1->complexity == s2->complexity;
}

static size_t
visited_hasher (const search_state *ss, size_t max)
{
  return (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

static bool
visited_comparator (const search_state *ss1, const search_state *ss2)
{
  return parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
static gl_list_t ssb_queue;
static Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
static bitset scp_set = NULL;
/* The set of parser states used for the conflict reduction rule. */
static bitset rpp_set = NULL;

static void
ssb_append (search_state *ss)
{
  if (hash_lookup (visited, ss))
    {
      search_state_free (ss);
      return;
    }
  if (!hash_insert (visited, ss))
    xalloc_die ();
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  search_state_bundle *ssb = xmalloc (sizeof (search_state_bundle));
  ssb->complexity = ss->complexity;
  gl_list_node_t n = gl_list_search (ssb_queue, ssb);
  if (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, NULL, NULL,
                              (gl_listelement_dispose_fn)search_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  else
    {
      free (ssb);
      ssb = (search_state_bundle *) gl_list_node_value (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
static void
production_step (search_state *ss, int parser_state)
{
  const state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  symbol_number other_sym = item_number_as_symbol_number (*other_si->item);
  gl_list_t prods = simulate_production (ss->states[parser_state], other_sym);
  int complexity = ss->complexity + PRODUCTION_COST;

  gl_list_iterator_t it = gl_list_iterator (prods);
  parse_state *ps;
  while (gl_list_iterator_next (&it, (const void **) &ps, NULL))
    {
      search_state *copy = copy_search_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
}

static inline int
reduction_cost (const parse_state *ps)
{
  int shifts;
  int productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  return SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

static gl_list_t
reduction_step (search_state *ss, const item_number *conflict_item,
                int parser_state, int rule_len)
{
  gl_list_t result =
    gl_list_create_empty (GL_LINKED_LIST, NULL, NULL, NULL, 1);

  parse_state *ps = ss->states[parser_state];
  const state_item *si = parse_state_tail (ps);
  bitset symbol_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  const state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  if (item_number_is_symbol_number (*other_si->item))
    {
      symbol_number other_sym =
        item_number_as_symbol_number (*other_si->item);
      if (!intersect_symbol (other_sym, symbol_set))
        return result;
      symbol_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (symbol_set, other_sym);
    }

  search_state *new_root = copy_search_state (ss);
  gl_list_t reduced = simulate_reduction (ps, rule_len, symbol_set);
  gl_list_iterator_t it = gl_list_iterator (reduced);
  parse_state *reduced_ps;
  while (gl_list_iterator_next (&it, (const void **) &reduced_ps, NULL))
    {
      search_state *copy = copy_search_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      int r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  return result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
void
search_state_prepend (search_state *ss, symbol_number sym, bitset guide)
{
  const state_item *si1src = parse_state_head (ss->states[0]);
  const state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  if (prod1 != SI_PRODUCTION (si2src))
    {
      int prod_state = prod1 ? 0 : 1;
      gl_list_t prev = parser_prepend (ss->states[prod_state]);
      gl_list_iterator_t iter = gl_list_iterator (prev);
      parse_state *ps;
      while (gl_list_iterator_next (&iter, (const void **)&ps, NULL))
        {
          const state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->number);
          if (!guided && !EXTENDED_SEARCH)
            continue;

          search_state *copy = copy_search_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          if (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      return;
    }
  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  int complexity_cost = prod1 ? PRODUCTION_COST : UNSHIFT_COST;
  complexity_cost *= 2;

  gl_list_t prev1 = parser_prepend (ss->states[0]);
  gl_list_t prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  gl_list_iterator_t iter1 = gl_list_iterator (prev1);
  parse_state *ps1;
  while (gl_list_iterator_next (&iter1, (const void **)&ps1, NULL))
    {
      const state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->number);
      if (!guided1 && !EXTENDED_SEARCH)
        continue;

      gl_list_iterator_t iter2 = gl_list_iterator (prev2);
      parse_state *ps2;
      while (gl_list_iterator_next (&iter2, (const void **)&ps2, NULL))
        {
          const state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->number);
          if (!guided2 && !EXTENDED_SEARCH)
            continue;
          // Only consider prepend state items that share the same state.
          if (psi1->state != psi2->state)
            continue;

          int complexity = ss->complexity;
          if (prod1)
            complexity += PRODUCTION_COST * 2;
          else
            complexity += UNSHIFT_COST * 2;
          // penalty for not being along the guide path
          if (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_search_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
static bool
has_common_prefix (const item_number *itm1, const item_number *itm2)
{
  int i = 0;
  for (; !item_number_is_rule_number (*(itm1 + i)); ++i)
    if (*(itm1 + i) != *(itm2 + i))
      return false;
  return item_number_is_rule_number (*(itm2 + i));
}

/*
 * Calculate the start and end locations of an item in ritem.
 */
static void
item_rule_bounds (const item_number *item, const item_number **start,
                  const item_number **end)
{
  const item_number *s, *e;
  for (s = item;
       s > ritem && item_number_is_symbol_number (*(s - 1));
       --s);
  *start = s;

  for (e = item; item_number_is_symbol_number (*e); ++e);
  *end = e;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
static inline void
generate_next_states (search_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  const state_item *si1 = parse_state_tail (ps1);
  const state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_number_is_rule_number (*si1->item);
  bool si2reduce = item_number_is_rule_number (*si2->item);
  if (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      if (*si1->item == *si2->item)
        {
          int complexity = ss->complexity + 2 * SHIFT_COST;
          gl_list_t trans1 = simulate_transition (ps1);
          gl_list_t trans2 = simulate_transition (ps2);
          gl_list_iterator_t it1 = gl_list_iterator (trans1);
          parse_state *tps1;
          while (gl_list_iterator_next (&it1, (const void **) &tps1, NULL))
            {
              gl_list_iterator_t it2 = gl_list_iterator (trans2);
              parse_state *tps2;
              while (gl_list_iterator_next
                     (&it2, (const void **) &tps2, NULL))
                ssb_append (new_search_state (tps1, tps2, complexity));
            }
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  else
    {
      const item_number *rhs1, *rhe1;
      item_rule_bounds (si1->item, &rhs1, &rhe1);
      int len1 = rhe1 - rhs1;
      int size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      const item_number *rhs2, *rhe2;
      item_rule_bounds (si2->item, &rhs2, &rhe2);
      int len2 = rhe2 - rhs2;
      int size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction
      // without being prepended further, reduce.
      if (ready1)
        {
          gl_list_t reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_iterator_t iter = gl_list_iterator (reduced1);
          search_state *red1;
          if (ready2)
            {
              gl_list_add_last (reduced1, ss);
              while (gl_list_iterator_next
                     (&iter, (const void **) &red1, NULL))
                {
                  gl_list_t reduced2 =
                    reduction_step (red1, conflict2->item, 1, len2);
                  gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                  search_state *red2;
                  while (gl_list_iterator_next
                         (&iter2, (const void **) &red2, NULL))
                    ssb_append (red2);
                  // avoid duplicates
                  if (red1 != ss)
                    ssb_append (red1);
                  gl_list_free (reduced2);
                }
            }
          else
            while (gl_list_iterator_next (&iter, (const void **) &red1, NULL))
              ssb_append (red1);
          gl_list_free (reduced1);
        }
      else if (ready2)
        {
          gl_list_t reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
          search_state *red2;
          while (gl_list_iterator_next (&iter2, (const void **) &red2, NULL))
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      else
        {
          symbol_number sym;
          if (si1reduce && !ready1)
            sym = *(rhe1 - size1);
          else
            sym = *(rhe2 - size2);
          search_state_prepend (ss, sym,
                                parse_state_depth (ss->states[0]) >= 0
                                ? rpp_set : scp_set);
        }
    }
}

/*
 * Perform the actual counter example search,
 * keeps track of what stage of the search algorithm
 * we are at and gives the appropriate counterexample
 * type based off of time constraints.
 */
counterexample *
unifying_example (state_item_number itm1,
                  state_item_number itm2,
                  bool shift_reduce,
                  gl_list_t reduce_path, symbol_number next_sym)
{
  state_item *conflict1 = state_items + itm1;
  state_item *conflict2 = state_items + itm2;
  search_state *initial = initial_search_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free, 
                                    false);
  visited =
    hash_initialize (32, NULL, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) search_state_free);
  ssb_append (initial);
  time_t start = time (NULL);
  bool assurance_printed = false;
  search_state *stage3result = NULL;
  counterexample *cex = NULL;
  while (gl_list_size (ssb_queue) > 0)
    {
      const search_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);
      gl_list_iterator_t it = gl_list_iterator (ssb->states);
      search_state *ss;
      while (gl_list_iterator_next (&it, (const void **) &ss, NULL))
        {
          if (trace_flag & trace_cex)
            search_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          if (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              const state_item *si1src = parse_state_head (ps1);
              const state_item *si2src = parse_state_head (ps2);
              if (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && has_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  const derivation *d1 = parse_state_derivation (ps1);
                  const derivation *d2 = parse_state_derivation (ps2);
                  if (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2)
                      && d1->sym == d2->sym)
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, true, false);
                      // prevent d1/d2 from being freed.
                      parse_state_retain_deriv (ps1);
                      parse_state_retain_deriv (ps2);
                      goto cex_search_end;
                    }
                  if (!stage3result)
                    stage3result = ss;
                }
            }
          if (TIME_LIMIT_ENFORCED)
            {
              float time_passed = difftime (time (NULL), start);
              if (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  puts
                    ("Productions leading up to the conflict state found.  Still finding a possible unifying counterexample...");
                  assurance_printed = true;
                }
              if (time_passed > TIME_LIMIT)
                {
                  printf ("time limit exceeded: %f\n", time_passed);
                  goto cex_search_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_search_end:;
  if (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      if (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      else
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  return cex;
}

static time_t cumulative_time;

void
counterexample_init (void)
{
  time (&cumulative_time);
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_items_init ();
}


void
counterexample_free (void)
{
  if (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_items_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
static void
counterexample_report (state_item_number itm1, state_item_number itm2,
                       symbol_number next_sym, bool shift_reduce)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  gl_list_t shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduceProdReached = false;
  const rule *reduce_rule = item_rule (state_items[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);
  gl_list_iterator_t it = gl_list_iterator (shortest_path);
  state_item *si;
  while (gl_list_iterator_next (&it, (const void **) &si, NULL))
    {
      bitset_set (scp_set, si->state->number);
      reduceProdReached = reduceProdReached
                          || item_rule (si->item) == reduce_rule;
      if (reduceProdReached)
        bitset_set (rpp_set, si->state->number);
    }
  time_t t = time (NULL);
  counterexample *cex = difftime (t,
                                  cumulative_time) <
    CUMULATIVE_TIME_LIMIT ? unifying_example (itm1, itm2, shift_reduce,
                                              shortest_path,
                                              next_sym) :
    example_from_path (shift_reduce, itm2, shortest_path, next_sym);

  gl_list_free (shortest_path);
  print_counterexample (cex);

}

void
counterexample_report_shift_reduce (state_item_number itm1, state_item_number itm2,
                                    symbol_number next_sym)
{
  puts ("Shift-Reduce Conflict:");
  print_state_item (&state_items[itm1], stdout);
  print_state_item (&state_items[itm2], stdout);
  printf ("On Symbol: %s\n", symbols[next_sym]->tag);
  counterexample_report (itm1, itm2, next_sym, true);
}

void
counterexample_report_reduce_reduce (state_item_number itm1, state_item_number itm2,
                                     bitset conflict_syms)
{
  puts ("Reduce-Reduce Conflict:");
  print_state_item (&state_items[itm1], stdout);
  print_state_item (&state_items[itm2], stdout);
  fputs ("On Symbols: {", stdout);
  bitset_iterator biter;
  state_item_number sym;
  BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
    {
      printf ("%s,", symbols[sym]->tag);
    }
  fputs ("}\n", stdout);
  counterexample_report (itm1, itm2, bitset_first (conflict_syms), false);
}