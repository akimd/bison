/* Bison Code Data Structure and Scanner.

   Copyright (C) 2006 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301  USA
*/

#ifndef SCAN_CODE_H_
# define SCAN_CODE_H_

# include "location.h"

struct symbol_list;

/**
 * \brief
 *   Keeps track of the maximum number of semantic values to the left of a
 *   handle (those referenced by \c $0, \c $-1, etc.) that are required by the
 *   semantic actions of this grammar.
 */
extern int max_left_semantic_context;

/**
 * \brief
 *   A code passage captured from the grammar file and possibly translated,
 *   and/or properties associated with such a code passage.
 * \note
 *   - Don't break encapsulation by accessing the fields directly.  Use the
 *     provided interface functions.
 */
typedef struct code_props {
  /**
   * \brief
   *   What kind of \c code_props this is.
   * \sa
   *   - \c code_props_none_init
   *   - \c code_props_plain_init
   *   - \c code_props_symbol_action_init
   *   - \c code_props_rule_action_init
   */
  enum {
    CODE_PROPS_NONE, CODE_PROPS_PLAIN,
    CODE_PROPS_SYMBOL_ACTION, CODE_PROPS_RULE_ACTION
  } kind;

  /**
   * \brief
   *   The code passage contained within this \c code_props.
   * \invariant
   *   - <tt>code_props::code = NULL</tt> iff
   *     <tt>code_props::kind = CODE_PROPS_NONE</tt>.
   */
  char const *code;
  /**
   * \brief
   *   The grammar file location of \c code_props::code.
   * \invariant
   *   - \c code_props::location is undefined iff
   *     <tt>code_props::code = NULL</tt>.
   */
  location location;
  /**
   * \brief
   *   The value returned by \c code_props_is_value_used for this
   *   \c code_props.
   */
  bool is_value_used;

  /**
   * \brief
   *   The \c symbol_list node associated with this code passage.
   * \invariant
   *   - <tt>code_props::rule != NULL</tt> iff \c code_props::kind is
   *     \c CODE_PROPS_RULE_ACTION.
   */
  struct symbol_list *rule;
} code_props;

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 * \post
 *   - \c self has been overwritten to contain no code.  (However, \c self may
 *     still be conceptually associated with some passage of code contained
 *     elsewhere.  Thus, a call on <tt>code_props_is_value_used (*self)</tt>,
 *     for example, is still reasonable.)
 */
void code_props_none_init (code_props *self);

/**
 * \brief A \c code_props initializer equivalent to \c code_props_none_init.
 */
#define CODE_PROPS_NONE_INIT \
  {CODE_PROPS_NONE, NULL, EMPTY_LOCATION_INIT, false, NULL}

/**
 * \brief
 *   A \c code_props initialized by \c CODE_PROPS_NONE_INIT with no further
 *   modification.
 */
extern code_props const code_props_none;

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>code != NULL</tt>.
 *   - \c code is an untranslated code passage containing no Bison escapes.
 *   - \c code was extracted from the grammar file at \c code_loc.
 * \post
 *   - \c self has been overwritten to represent the specified plain code
 *     passage.
 *   - \c self does not claim responsibility for the memory of \c code.
 */
void code_props_plain_init (code_props *self, char const *code,
                            location code_loc);

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>code != NULL</tt>.
 *   - \c code is an untranslated code passage.  The only Bison escapes it
 *     might contain are \c $$ and \c \@$, referring to a single symbol.
 *   - \c code was extracted from the grammar file at \c code_loc.
 * \post
 *   - \c self has been overwritten to represent the specified symbol action.
 *   - \c self does not claim responsibility for the memory of \c code.
 */
void code_props_symbol_action_init (code_props *self, char const *code,
                                    location code_loc);

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>code != NULL</tt>.
 *   - <tt>rule != NULL</tt>.
 *   - \c code is the untranslated action of the rule for which \c rule is the
 *     LHS node.  Thus, \c code possibly contains Bison escapes such as \c $$,
 *     \c $1, \c $2, etc referring to the values of the rule.
 * \post
 *   - \c self has been overwritten to represent the specified rule action.
 *   - \c self does not claim responsibility for the memory of \c code or
 *     \c rule.
 */
void code_props_rule_action_init (code_props *self, char const *code,
                                  location code_loc, struct symbol_list *rule);

/**
 * \pre
 *   - If there's a code passage contained in \c self and it contains Bison
 *     escapes, all grammar declarations have already been parsed as they may
 *     affect warnings and complaints issued here.
 * \post
 *   - All M4 special symbols and Bison escapes have been translated in
 *     <tt>code_props_code_get (*self)</tt> iff
 *     <tt>code_props_code_get (*self \@pre) != NULL</tt>.
 */
void code_props_translate_code (code_props *self);

/**
 * \pre
 *   - None.
 * \post
 *   - \c result = either:
 *     - The code passage contained with \c self.
 *     - \c NULL if none.
 */
char const *code_props_code_get (code_props const self);

/**
 * \pre
 *   - <tt>code_props_code_get (self) != NULL</tt>.
 * \post
 *   - \c result = the grammar file location of
 *     <tt>code_props_code_get (self)</tt>.
 */
location code_props_location_get (code_props const self);

/**
 * \pre
 *   - \c self was not previously initialized with \c code_props_plain_init.
 * \post
 *   - \c result = either:
 *     - \c false if either:
 *       - \c code_props_translate_code has never previously been invoked for
 *         the \c code_props that would contain the code passage associated
 *         with \c self.  (If \c self is for a RHS \c symbol_list node, that
 *         \c code_props is not \c self.  Instead, it's the \c code_props for
 *         the LHS symbol of the same rule.)
 *       - \c code_props_translate_code has been invoked for that
 *         \c code_props, but the symbol value associated with \c self was not
 *         referenced in the code passage.
 *     - \c true otherwise.
 */
bool code_props_is_value_used (code_props const self);

/**
 * \pre
 *   - None.
 * \post
 *   - The dynamic memory allocated by the previous invocation of
 *     \c code_props_translate_code (if any) was freed.  The \c code_props
 *     instance for which that \c code_props_translate_code was invoked is now
 *     invalid.
 */
void code_scanner_last_string_free (void);

/**
 * \pre
 *   - None.
 * \post
 *   - All dynamic memory allocated during invocations of
 *     \c code_props_translate_code (if any) has been freed.  All
 *     \c code_props instances and all pointers returned by
 *     \c code_props_code_get may now be invalid.
 */
void code_scanner_free (void);

#endif /* !SCAN_CODE_H_ */
