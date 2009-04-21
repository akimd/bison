/* IELR's inadequacy list.

   Copyright (C) 2009 Free Software Foundation, Inc.

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

#ifndef INADEQUACY_LIST_H_
# define INADEQUACY_LIST_H_

#include <bitset.h>
#include "gram.h"
#include "state.h"
#include "symtab.h"

/**
 * For a conflict, each rule in the grammar can have at most one contributing
 * reduction except that rule 0 cannot have any because the reduction on rule 0
 * cannot have lookaheads.  For a conflict, exactly one shift can contribute.
 * Thus the number of rules in the grammar is an upper bound on the number of
 * possible contributions to any conflict.  The maximum number of possible
 * items in a state is also an upper bound, but the \c nitems member of \c
 * state is currently a \c size_t and thus, if changed, risks becoming out of
 * sync with this type.  Whatever the type, it must support negatives for sake
 * of the special values below.
 */
typedef rule_number ContributionIndex;

/* Special \c ContributionIndex used to indicate null result when looking for a
   contribution.  */
extern ContributionIndex const ContributionIndex__none;

/* Special \c ContributionIndex used by
   \c AnnotationList__computeDominantContribution to signal when the action
   chosen in a conflict is a syntax error because of a %nonassoc.  */
extern ContributionIndex const ContributionIndex__error_action;

/**
 * The description of a conflict.  Don't break encapsulation by modifying the
 * fields directly.  Use the provided interface functions for
 * \c InadequacyList.
 */
typedef struct {
  /** The \c token passed to \c InadequacyList__new_conflict.  */
  symbol *token;
  /** The \c actions passed to \c InadequacyList__new_conflict.  */
  bitset actions;
} Conflict;

/**
 * A node in a list that describes all the inadequacies that manifest in a
 * particular state.  Don't break encapsulation by modifying the fields
 * directly.  Use the provided interface functions.
 */
typedef struct InadequacyList {
  struct InadequacyList *next;
  state *manifestingState;
  ContributionIndex contributionCount;
  union {
    Conflict conflict;
  } inadequacy;
} InadequacyList;

/**
 * \pre
 *   - <tt>manifesting_state != NULL</tt>.
 *   - \c token is a token.
 *   - The size of \c actions is
 *     <tt>manifesting_state->reductions->num + 1</tt>.
 * \post
 *   - \c result is a new \c InadequacyList with one node indicating that, in
 *     \c manifesting_state, the following actions are in conflict on \c token:
 *     - Shift iff
 *       <tt>bitset_test (actions, manifesting_state->reductions->num)</tt>.
 *     - For any \c i such that
 *       <tt>0 <= i < manifesting_state->reductions->num</tt>, the reduction
 *       for the rule <tt>manifesting_state->reductions->rules[i]</tt> iff
 *       <tt>actions[i]</tt> is set.
 *   - \c result assumes responsibility for the memory of \c actions.
 */
InadequacyList *InadequacyList__new_conflict (state *manifesting_state,
                                              symbol *token, bitset actions);

/**
 * \post
 *   - All memory associated with all nodes in the list \c self was freed.
 */
void InadequacyList__delete (InadequacyList *self);

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 * \post
 *   - \c result = either:
 *     - \c ContributionIndex__none iff there is no shift contribution in
 *       \c self (perhaps because \c self isn't a conflict).
 *     - The index of the shift contribution, otherwise.
 */
ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList const *self);

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= i < self->contributionCount</tt>.
 * \post
 *   - \c result = the token associated with contribution \c i in the
 *     inadequacy described by the node \c self.
 */
symbol *InadequacyList__getContributionToken (InadequacyList const *self,
                                              ContributionIndex i);

/**
 * \pre
 *   - \c self is a single node.
 *   - <tt>list != NULL</tt>.
 * \post
 *   - \c list now contains \c self as its first node.
 *   - \c list assumes responsibility for the memory of \c self.
 */
void InadequacyList__prependTo (InadequacyList *self, InadequacyList **list);

#endif /* !INADEQUACY_LIST_H_ */
