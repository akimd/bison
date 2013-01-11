# C++ skeleton for Bison

# Copyright (C) 2002-2012 Free Software Foundation, Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


## --------- ##
## variant.  ##
## --------- ##

# b4_symbol_variant(YYTYPE, YYVAL, ACTION, [ARGS])
# ------------------------------------------------
# Run some ACTION ("build", or "destroy") on YYVAL of symbol type
# YYTYPE.
m4_define([b4_symbol_variant],
[m4_pushdef([b4_dollar_dollar],
            [$2.$3< $][3 >(m4_shift3($@))])dnl
  switch ($1)
    {
b4_type_foreach([b4_type_action_])[]dnl
      default:
        break;
    }
m4_popdef([b4_dollar_dollar])dnl
])


# _b4_char_sizeof_counter
# -----------------------
# A counter used by _b4_char_sizeof_dummy to create fresh symbols.
m4_define([_b4_char_sizeof_counter],
[0])

# _b4_char_sizeof_dummy
# ---------------------
# At each call return a new C++ identifier.
m4_define([_b4_char_sizeof_dummy],
[m4_define([_b4_char_sizeof_counter], m4_incr(_b4_char_sizeof_counter))dnl
dummy[]_b4_char_sizeof_counter])


# b4_char_sizeof(SYMBOL-NUMS)
# ---------------------------
# To be mapped on the list of type names to produce:
#
#    char dummy1[sizeof(type_name_1)];
#    char dummy2[sizeof(type_name_2)];
#
# for defined type names.
m4_define([b4_char_sizeof],
[b4_symbol_if([$1], [has_type],
[
m4_map([      b4_symbol_tag_comment], [$@])dnl
      char _b4_char_sizeof_dummy@{sizeof([b4_symbol([$1], [type])])@};
])])


# b4_variant_includes
# -------------------
# The needed includes for variants support.
m4_define([b4_variant_includes],
[b4_parse_assert_if([[#include <typeinfo>]])[
#include <cstdlib>  // abort
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif
]])

# b4_variant_define
# -----------------
# Define "variant".
m4_define([b4_variant_define],
[[  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    inline
    variant ()]b4_parse_assert_if([
      : built (false)
      , tname (YY_NULL)])[
    {}

    /// Instantiate a \a T in here.
    template <typename T>
    inline T&
    build ()
    {]b4_parse_assert_if([
      //YYASSERT (!built);
      //YYASSERT (!tname);
      YYASSERT (sizeof (T) <= S);
      built = true;
      tname = typeid (T).name ();])[
      return *new (buffer.raw) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    inline T&
    build (const T& t)
    {]b4_parse_assert_if([
      //YYASSERT (!built);
      //YYASSERT (!tname);
      YYASSERT (sizeof (T) <= S);
      built = true;
      tname = typeid (T).name ();])[
      return *new (buffer.raw) T (t);
    }

    /// Construct and fill.
    template <typename T>
    inline
    variant (const T& t)]b4_parse_assert_if([
      : built (true)
      , tname (typeid (T).name ())])[
    {
      YYASSERT (sizeof (T) <= S);
      new (buffer.raw) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    inline T&
    as ()
    {]b4_parse_assert_if([
      YYASSERT (built);
      YYASSERT (tname == typeid (T).name ());
      YYASSERT (sizeof (T) <= S);])[
      return reinterpret_cast<T&> (buffer.raw);
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    inline const T&
    as () const
    {]b4_parse_assert_if([
      YYASSERT (built);
      YYASSERT (tname == typeid (T).name ());
      YYASSERT (sizeof (T) <= S);])[
      return reinterpret_cast<const T&> (buffer.raw);
    }

    /// Swap the content with \a other, of same type.
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and ((possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    inline void
    swap (variant<S>& other)
    {]b4_parse_assert_if([
      YYASSERT (built);
      YYASSERT (other.built);
      YYASSERT (tname == other.tname);])[
      std::swap (as<T>(), other.as<T>());
    }

    /// Assign the content of \a other to this.
    /// Destroys \a other.
    template <typename T>
    inline void
    move (variant<S>& other)
    {]b4_parse_assert_if([
      YYASSERT (! built);])[
      build<T>();
      swap<T>(other);
      other.destroy<T>();
    }

    /// Copy the content of \a other to this.
    /// Destroys \a other.
    template <typename T>
    inline void
    copy (const variant<S>& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    inline void
    destroy ()
    {
      as<T> ().~T ();]b4_parse_assert_if([
      built = false;
      tname = YY_NULL;])[
    }

    /// Prohibit blind copies.
    private:
    self_type& operator=(const self_type&)
    {
      abort ();
    }

    variant (const self_type&)
    {
      abort ();
    }

  private:
    /// A buffer large enough to store any of the semantic values.
    /// Long double is chosen as it has the strongest alignment
    /// constraints.
    union
    {
      long double align_me;
      char raw[S];
    } buffer;]b4_parse_assert_if([
    /// Whether something is contained.
    bool built;
    /// If defined, the name of the stored type.
    const char* tname;])[
  };
]])


## -------------------------- ##
## Adjustments for variants.  ##
## -------------------------- ##


# b4_semantic_type_declare
# ------------------------
# Declare semantic_type.
m4_define([b4_semantic_type_declare],
[    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {]b4_type_foreach([b4_char_sizeof])[};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;dnl
])


# How the semantic value is extracted when using variants.

# b4_symbol_value(VAL, [TYPE])
# ----------------------------
m4_define([b4_symbol_value],
[m4_ifval([$2],
          [$1.as< $2 >()],
          [$1])])

# b4_symbol_value_template(VAL, [TYPE])
# -------------------------------------
# Same as b4_symbol_value, but used in a template method.
m4_define([b4_symbol_value_template],
[m4_ifval([$2],
          [$1.template as< $2 >()],
          [$1])])



## ------------- ##
## make_SYMBOL.  ##
## ------------- ##


# b4_symbol_constructor_declare_(SYMBOL-NUMBER)
# ---------------------------------------------
# Declare the overloaded version of make_symbol for the (common) type of
# these SYMBOL-NUMBERS.  Use at class-level.
m4_define([b4_symbol_constructor_declare_],
[b4_symbol_if([$1], [is_token], [b4_symbol_if([$1], [has_id],
[    static inline
    symbol_type
    make_[]b4_symbol_([$1], [id]) (dnl
b4_join(b4_symbol_if([$1], [has_type],
                     [const b4_symbol([$1], [type])& v]),
        b4_locations_if([const location_type& l])));

])])])


# b4_symbol_constructor_declare
# -----------------------------
# Declare symbol constructors for all the value types.
# Use at class-level.
m4_define([b4_symbol_constructor_declare],
[    // Symbol constructors declarations.
b4_symbol_foreach([b4_symbol_constructor_declare_])])



# b4_symbol_constructor_define_(SYMBOL-NUMBER)
# --------------------------------------------
# Define symbol constructor for this SYMBOL-NUMBER.
m4_define([b4_symbol_constructor_define_],
[b4_symbol_if([$1], [is_token], [b4_symbol_if([$1], [has_id],
[  b4_parser_class_name::symbol_type
  b4_parser_class_name::make_[]b4_symbol_([$1], [id]) (dnl
b4_join(b4_symbol_if([$1], [has_type],
                     [const b4_symbol([$1], [type])& v]),
        b4_locations_if([const location_type& l])))
  {
    return symbol_type (b4_join([token::b4_symbol([$1], [id])],
                                b4_symbol_if([$1], [has_type], [v]),
                                b4_locations_if([l])));
  }

])])])


# b4_symbol_constructor_define
# ----------------------------
# Define the overloaded versions of make_symbol for all the value types.
m4_define([b4_symbol_constructor_define],
[  // Implementation of make_symbol for each symbol type.
b4_symbol_foreach([b4_symbol_constructor_define_])])
