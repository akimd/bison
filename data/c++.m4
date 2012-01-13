                                                            -*- Autoconf -*-

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

m4_include(b4_pkgdatadir/[c.m4])

## ---------------- ##
## Default values.  ##
## ---------------- ##

b4_percent_define_default([[parser_class_name]], [[parser]])

# Don't do that so that we remember whether we're using a user
# request, or the default value.
#
# b4_percent_define_default([[location_type]], [[location]])

b4_percent_define_default([[filename_type]], [[std::string]])
b4_percent_define_default([[api.namespace]], m4_defn([b4_prefix]))

b4_percent_define_default([[global_tokens_and_yystype]], [[false]])
b4_percent_define_default([[define_location_comparison]],
                          [m4_if(b4_percent_define_get([[filename_type]]),
                                 [std::string], [[true]], [[false]])])



## ----------- ##
## Namespace.  ##
## ----------- ##

m4_define([b4_namespace_ref], [b4_percent_define_get([[api.namespace]])])


# Don't permit an empty b4_namespace_ref.  Any `::parser::foo' appended to it
# would compile as an absolute reference with `parser' in the global namespace.
# b4_namespace_open would open an anonymous namespace and thus establish
# internal linkage.  This would compile.  However, it's cryptic, and internal
# linkage for the parser would be specified in all translation units that
# include the header, which is always generated.  If we ever need to permit
# internal linkage somehow, surely we can find a cleaner approach.
m4_if(m4_bregexp(b4_namespace_ref, [^[	 ]*$]), [-1], [],
[b4_complain_at(b4_percent_define_get_loc([[api.namespace]]),
                [[namespace reference is empty]])])

# Instead of assuming the C++ compiler will do it, Bison should reject any
# invalid b4_namepsace_ref that would be converted to a valid
# b4_namespace_open.  The problem is that Bison doesn't always output
# b4_namespace_ref to uncommented code but should reserve the ability to do so
# in future releases without risking breaking any existing user grammars.
# Specifically, don't allow empty names as b4_namespace_open would just convert
# those into anonymous namespaces, and that might tempt some users.
m4_if(m4_bregexp(b4_namespace_ref, [::[	 ]*::]), [-1], [],
[b4_complain_at(b4_percent_define_get_loc([[api.namespace]]),
                [[namespace reference has consecutive "::"]])])
m4_if(m4_bregexp(b4_namespace_ref, [::[	 ]*$]), [-1], [],
[b4_complain_at(b4_percent_define_get_loc([[api.namespace]]),
                [[namespace reference has a trailing "::"]])])

m4_define([b4_namespace_open],
[b4_user_code([b4_percent_define_get_syncline([[api.namespace]])
[namespace ]m4_bpatsubst(m4_dquote(m4_bpatsubst(m4_dquote(b4_namespace_ref),
                                                [^\(.\)[	 ]*::], [\1])),
                         [::], [ { namespace ])[ {]])])

m4_define([b4_namespace_close],
[b4_user_code([b4_percent_define_get_syncline([[api.namespace]])
m4_bpatsubst(m4_dquote(m4_bpatsubst(m4_dquote(b4_namespace_ref[ ]),
                                    [^\(.\)[	 ]*\(::\)?\([^][:]\|:[^:]\)*],
                                    [\1])),
             [::\([^][:]\|:[^:]\)*], [} ])[} // ]b4_namespace_ref])])


# b4_token_enums(LIST-OF-PAIRS-TOKEN-NAME-TOKEN-NUMBER)
# -----------------------------------------------------
# Output the definition of the tokens as enums.
m4_define([b4_token_enums],
[/* Tokens.  */
   enum yytokentype {
m4_map_sep([     b4_token_enum], [,
],
           [$@])
   };
])


## ----------------- ##
## Semantic Values.  ##
## ----------------- ##

# b4_semantic_type_declare
# ------------------------
# Declare semantic_type.
m4_define([b4_semantic_type_declare],
[    /// Symbol semantic values.
m4_ifdef([b4_stype],
[    union semantic_type
    {b4_user_stype
    };],
[m4_if(b4_tag_seen_flag, 0,
[[    typedef int semantic_type;]],
[[    typedef YYSTYPE semantic_type;]])])])


# b4_public_types_declare
# -----------------------
# Define the public types: token, semantic value, location, and so forth.
# Depending on %define token_lex, may be output in the header or source file.
m4_define([b4_public_types_declare],
[[#ifndef YYSTYPE
]b4_semantic_type_declare[
#else
    typedef YYSTYPE semantic_type;
#endif]b4_locations_if([
    /// Symbol locations.
    typedef b4_percent_define_get([[location_type]],
                                  [[location]]) location_type;])[

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (]b4_locations_if([const location_type& l, ])[const std::string& m);]b4_locations_if([
      location_type location;])[
    };

    /// Tokens.
    struct token
    {
      ]b4_token_enums(b4_tokens)[
    };

    /// Token type.
    typedef token::yytokentype token_type;

    /// A complete symbol, with its type.
    template <typename Exact>
    struct symbol_base_type
    {
      /// Default constructor.
      inline symbol_base_type ();

      /// Constructor.]b4_locations_if([
      inline symbol_base_type (const location_type& l)])[;
      inline symbol_base_type (]b4_args(
        [const semantic_type& v],
        b4_locations_if([const location_type& l]))[);

      /// Return this with its exact type.
      const Exact& self () const;
      Exact& self ();

      /// Return the type of this symbol.
      int type_get () const;

      /// The semantic value.
      semantic_type value;]b4_locations_if([

      /// The location.
      location_type location;])[
    };

    /// External form of a symbol: its type and attributes.
    struct symbol_type : symbol_base_type<symbol_type>
    {
      /// The parent class.
      typedef symbol_base_type<symbol_type> super_type;

      /// Default constructor.
      inline symbol_type ();

      /// Constructor for tokens with semantic value.
      inline symbol_type (]b4_args([token_type t],
                                   [const semantic_type& v],
                                   b4_locations_if([const location_type& l]))[);

      /// Constructor for valueless tokens.
      inline symbol_type (]b4_args([token_type t],
                                   b4_locations_if([const location_type& l]))[);

      /// The symbol type.
      int type;

      /// The symbol type.
      inline int type_get_ () const;

      /// The token.
      inline token_type token () const;
    };
]b4_symbol_constructor_declare])


# b4_public_types_define
# ----------------------
# Provide the implementation needed by the public types.
m4_define([b4_public_types_define],
[[  inline
  ]b4_parser_class_name[::syntax_error::syntax_error (]b4_locations_if([const location_type& l, ])[const std::string& m)
    : std::runtime_error (m)]b4_locations_if([
    , location (l)])[
  {}

  // symbol_base_type.
  template <typename Exact>
  inline
  ]b4_parser_class_name[::symbol_base_type<Exact>::symbol_base_type ()
    : value()]b4_locations_if([
    , location()])[
  {
  }]b4_locations_if([[

  template <typename Exact>
  inline
  ]b4_parser_class_name[::symbol_base_type<Exact>::symbol_base_type (const location_type& l)
    : value()
    , location(l)
  {
  }]])[

  template <typename Exact>
  inline
  ]b4_parser_class_name[::symbol_base_type<Exact>::symbol_base_type (]b4_args(
          [const semantic_type& v],
          b4_locations_if([const location_type& l]))[)
    : value(v)]b4_locations_if([
    , location(l)])[
  {
  }

  template <typename Exact>
  inline
  const Exact&
  ]b4_parser_class_name[::symbol_base_type<Exact>::self () const
  {
    return static_cast<const Exact&>(*this);
  }

  template <typename Exact>
  inline
  Exact&
  ]b4_parser_class_name[::symbol_base_type<Exact>::self ()
  {
    return static_cast<Exact&>(*this);
  }

  template <typename Exact>
  inline
  int
  ]b4_parser_class_name[::symbol_base_type<Exact>::type_get () const
  {
    return self ().type_get_ ();
  }

  // symbol_type.
  inline
  ]b4_parser_class_name[::symbol_type::symbol_type ()
    : super_type ()
    , type ()
  {
  }

  inline
  ]b4_parser_class_name[::symbol_type::symbol_type (]b4_args(
                [token_type t],
                b4_locations_if([const location_type& l]))[)
    : super_type (]b4_locations_if([l])[)
    , type (yytranslate_ (t))
  {
  }

  inline
  ]b4_parser_class_name[::symbol_type::symbol_type (]b4_args(
                 [token_type t],
                 [const semantic_type& v],
                 b4_locations_if([const location_type& l]))[)
    : super_type (v]b4_locations_if([, l])[)
    , type (yytranslate_ (t))
  {
  }

  inline
  int
  ]b4_parser_class_name[::symbol_type::type_get_ () const
  {
    return type;
  }
]b4_lex_symbol_if([[
  inline
  ]b4_parser_class_name[::token_type
  ]b4_parser_class_name[::symbol_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const ]b4_int_type_for([b4_toknum])[
    yytoken_number_[] =
    {
  ]b4_toknum[
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
]])[]dnl
b4_symbol_constructor_define])


# b4_symbol_constructor_declare
# b4_symbol_constructor_define
# -----------------------------
# Declare/define symbol constructors for all the value types.
# Use at class-level.  Redefined in variant.hh.
m4_define([b4_symbol_constructor_declare], [])
m4_define([b4_symbol_constructor_define], [])


# b4_yytranslate_define
# ---------------------
# Define yytranslate_.  Sometimes used in the header file,
# sometimes in the cc file.
m4_define([b4_yytranslate_define],
[[  // Symbol number corresponding to token number t.
  ]b4_parser_class_name[::token_number_type
  ]b4_parser_class_name[::yytranslate_ (]b4_lex_symbol_if([token_type],
                                                          [int])[ t)
  {
    static
    const token_number_type
    translate_table[] =
    {
]b4_translate[
    };
    const unsigned int user_token_number_max_ = ]b4_user_token_number_max[;
    const token_number_type undef_token_ = ]b4_undef_token_number[;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }
]])


# b4_lhs_value([TYPE])
# --------------------
# Expansion of $<TYPE>$.
m4_define([b4_lhs_value],
[b4_symbol_value([yyval], [$1])])


# b4_rhs_value(RULE-LENGTH, NUM, [TYPE])
# --------------------------------------
# Expansion of $<TYPE>NUM, where the current rule has RULE-LENGTH
# symbols on RHS.
m4_define([b4_rhs_value],
[b4_symbol_value([yysemantic_stack_@{($1) - ($2)@}], [$3])])


# b4_lhs_location()
# -----------------
# Expansion of @$.
m4_define([b4_lhs_location],
[(yyloc)])


# b4_rhs_location(RULE-LENGTH, NUM)
# ---------------------------------
# Expansion of @NUM, where the current rule has RULE-LENGTH symbols
# on RHS.
m4_define([b4_rhs_location],
[(yylocation_stack_@{($1) - ($2)@})])


# b4_parse_param_decl
# -------------------
# Extra formal arguments of the constructor.
# Change the parameter names from "foo" into "foo_yyarg", so that
# there is no collision bw the user chosen attribute name, and the
# argument name in the constructor.
m4_define([b4_parse_param_decl],
[m4_ifset([b4_parse_param],
          [m4_map_sep([b4_parse_param_decl_1], [, ], [b4_parse_param])])])

m4_define([b4_parse_param_decl_1],
[$1_yyarg])



# b4_parse_param_cons
# -------------------
# Extra initialisations of the constructor.
m4_define([b4_parse_param_cons],
          [m4_ifset([b4_parse_param],
                    [
      b4_cc_constructor_calls(b4_parse_param)])])
m4_define([b4_cc_constructor_calls],
          [m4_map_sep([b4_cc_constructor_call], [,
      ], [$@])])
m4_define([b4_cc_constructor_call],
          [$2 ($2_yyarg)])

# b4_parse_param_vars
# -------------------
# Extra instance variables.
m4_define([b4_parse_param_vars],
          [m4_ifset([b4_parse_param],
                    [
    /* User arguments.  */
b4_cc_var_decls(b4_parse_param)])])
m4_define([b4_cc_var_decls],
          [m4_map_sep([b4_cc_var_decl], [
], [$@])])
m4_define([b4_cc_var_decl],
          [    $1;])
