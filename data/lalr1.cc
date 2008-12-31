# C++ skeleton for Bison

# Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008
# Free Software Foundation, Inc.

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

m4_include(b4_pkgdatadir/[c++.m4])


# b4_integral_parser_table_declare(TABLE-NAME, CONTENT, COMMENT)
# --------------------------------------------------------------
# Declare "parser::yy<TABLE-NAME>_" which contents is CONTENT.
m4_define([b4_integral_parser_table_declare],
[m4_ifval([$3], [b4_c_comment([$3], [  ])
])dnl
  static const b4_int_type_for([$2]) yy$1_[[]];dnl
])

# b4_integral_parser_table_define(TABLE-NAME, CONTENT, COMMENT)
# ---------------------------------------------
# Define "parser::yy<TABLE-NAME>_" which contents is CONTENT.
m4_define([b4_integral_parser_table_define],
[  const b4_int_type_for([$2])
  b4_parser_class_name::yy$1_[[]] =
  {
  $2
  };dnl
])


# b4_symbol_value_template(VAL, [TYPE])
# -------------------------------------
# Same as b4_symbol_value, but used in a template method.  It makes
# a difference when using variants.
m4_copy([b4_symbol_value], [b4_symbol_value_template])


# b4_lex_symbol_if([IF-YYLEX-RETURNS-A-COMPLETE-SYMBOL], [IF-NOT])
# ----------------------------------------------------------------
m4_define([b4_lex_symbol_if],
[b4_percent_define_ifdef([[lex_symbol]], [$1], [$2])])


# b4_assert_if([IF-ASSERTIONS-ARE-USED], [IF-NOT])
# ------------------------------------------------
m4_define([b4_assert_if],
[b4_percent_define_ifdef([[assert]], [$1], [$2])])


# b4_lhs_value([TYPE])
# --------------------
# Expansion of $<TYPE>$.
m4_define([b4_lhs_value],
          [b4_symbol_value([yylhs.value], [$1])])


# b4_lhs_location()
# -----------------
# Expansion of @$.
m4_define([b4_lhs_location],
          [yylhs.location])


# b4_rhs_data(RULE-LENGTH, NUM)
# -----------------------------
# Return the data corresponding to the symbol #NUM, where the current
# rule has RULE-LENGTH symbols on RHS.
m4_define([b4_rhs_data],
          [yystack_@{b4_subtract($@)@}])


# b4_rhs_state(RULE-LENGTH, NUM)
# ------------------------------
# The state corresponding to the symbol #NUM, where the current
# rule has RULE-LENGTH symbols on RHS.
m4_define([b4_rhs_state],
          [b4_rhs_data([$1], [$2]).state])


# b4_rhs_value(RULE-LENGTH, NUM, [TYPE])
# --------------------------------------
# Expansion of $<TYPE>NUM, where the current rule has RULE-LENGTH
# symbols on RHS.
m4_define([b4_rhs_value],
          [b4_symbol_value([b4_rhs_data([$1], [$2]).value], [$3])])


# b4_rhs_location(RULE-LENGTH, NUM)
# ---------------------------------
# Expansion of @NUM, where the current rule has RULE-LENGTH symbols
# on RHS.
m4_define([b4_rhs_location],
          [b4_rhs_data([$1], [$2]).location])


# b4_symbol_action(SYMBOL-NUM, KIND)
# ----------------------------------
# Run the action KIND (destructor or printer) for SYMBOL-NUM.
# Same as in C, but using references instead of pointers.
m4_define([b4_symbol_action],
[b4_symbol_if([$1], [has_$2],
[m4_pushdef([b4_dollar_dollar],
    [b4_symbol_value_template([yysym.value],
                              b4_symbol_if([$1], [has_type],
                                           [b4_symbol([$1], [type])]))])dnl
m4_pushdef([b4_at_dollar], [yysym.location])dnl
      b4_symbol_case_([$1])
b4_syncline([b4_symbol([$1], [$2_line])], ["b4_symbol([$1], [$2_file])"])
        b4_symbol([$1], [$2])
b4_syncline([@oline@], [@ofile@])
        break;

m4_popdef([b4_at_dollar])dnl
m4_popdef([b4_dollar_dollar])dnl
])])

# b4_symbol_constructor_declaration_(SYMBOL-NUMBER)
# -------------------------------------------------
# Declare the overloaded version of make_symbol for the (common) type of
# these SYMBOL-NUMBERS.  Use at class-level.
m4_define([b4_symbol_constructor_declaration_],
[b4_symbol_if([$1], [is_token], [b4_symbol_if([$1], [has_id],
[    static inline
    symbol_type
    make_[]b4_symbol_([$1], [id]) (dnl
b4_args(b4_symbol_if([$1], [has_type],
                     [const b4_symbol([$1], [type])& v]),
        b4_locations_if([const location_type& l])));

])])])


# b4_symbol_constructor_declarations
# ----------------------------------
# Declare symbol constructors for all the value types.
# Use at class-level.
m4_define([b4_symbol_constructor_declarations],
[b4_variant_if([
    // Symbol constructors declarations.
b4_symbol_foreach([b4_symbol_constructor_declaration_])])])



# b4_symbol_constructor_definition_(SYMBOL-NUMBER)
# ------------------------------------------------
# Define symbol constructor for this SYMBOL-NUMBER.
m4_define([b4_symbol_constructor_definition_],
[b4_symbol_if([$1], [is_token], [b4_symbol_if([$1], [has_id],
[  b4_parser_class_name::symbol_type
  b4_parser_class_name::make_[]b4_symbol_([$1], [id]) (dnl
b4_args(b4_symbol_if([$1], [has_type],
                     [const b4_symbol([$1], [type])& v]),
        b4_locations_if([const location_type& l])))
  {
    return symbol_type (b4_args([yytranslate_ (token::b4_symbol([$1], [id]))],
                                b4_symbol_if([$1], [has_type], [v]),
                                b4_locations_if([l])));
  }

])])])


# b4_symbol_constructor_definitions
# ----------------------------------
# Define the overloaded versions of make_symbol for all the value types.
m4_define([b4_symbol_constructor_definitions],
[[  // symbol_base_type.
  template <typename Exact>
  ]b4_parser_class_name[::symbol_base_type<Exact>::symbol_base_type ()
    : value()]b4_locations_if([
    , location()])[
  {
  }]b4_locations_if([[

  template <typename Exact>
  ]b4_parser_class_name[::symbol_base_type<Exact>::symbol_base_type (const location_type& l)
    : value()
    , location(l)
  {
  }]])[

  template <typename Exact>
  ]b4_parser_class_name[::symbol_base_type<Exact>::symbol_base_type (]b4_args(
          [const semantic_type& v],
          b4_locations_if([const location_type& l]))[)
    : value(v)]b4_locations_if([
    , location(l)])[
  {
  }

  template <typename Exact>
  const Exact&
  ]b4_parser_class_name[::symbol_base_type<Exact>::self () const
  {
    return static_cast<const Exact&>(*this);
  }

  template <typename Exact>
  Exact&
  ]b4_parser_class_name[::symbol_base_type<Exact>::self ()
  {
    return static_cast<Exact&>(*this);
  }

  template <typename Exact>
  int
  ]b4_parser_class_name[::symbol_base_type<Exact>::type_get () const
  {
    return self ().type_get_ ();
  }

  // symbol_type.
  ]b4_parser_class_name[::symbol_type::symbol_type ()
    : super_type ()
    , type ()
  {
  }

  ]b4_parser_class_name[::symbol_type::symbol_type (]b4_args(
                [int t],
                b4_locations_if([const location_type& l]))[)
    : super_type (]b4_locations_if([l])[)
    , type (t)
  {
  }

  ]b4_parser_class_name[::symbol_type::symbol_type (]b4_args(
                 [int t],
                 [const semantic_type& v],
                 b4_locations_if([const location_type& l]))[)
    : super_type (v]b4_locations_if([, l])[)
    , type (t)
  {
  }

  int
  ]b4_parser_class_name[::symbol_type::type_get_ () const
  {
    return type;
  }
]b4_lex_symbol_if([[
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
]])[

]b4_variant_if(
[  // Implementation of make_symbol for each symbol type.
b4_symbol_foreach([b4_symbol_constructor_definition_])])])


# b4_yytranslate_definition
# -------------------------
# Define yytranslate_.  Sometimes we want it in the header file,
# sometimes the cc file suffices.
m4_define([b4_yytranslate_definition],
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


m4_pushdef([b4_copyright_years],
           [2002, 2003, 2004, 2005, 2006, 2007, 2008])

m4_define([b4_parser_class_name],
          [b4_percent_define_get([[parser_class_name]])])

# The header is mandatory.
b4_defines_if([],
              [b4_fatal([b4_skeleton[: using %%defines is mandatory]])])

b4_locations_if(
[# Backward compatibility.
m4_define([b4_location_constructors])
m4_include(b4_pkgdatadir/[location.cc])])
m4_include(b4_pkgdatadir/[stack.hh])
b4_variant_if([m4_include(b4_pkgdatadir/[variant.hh])])

# We do want M4 expansion after # for CPP macros.
m4_changecom()
m4_divert_push(0)dnl
@output(b4_spec_defines_file@)@
b4_copyright([Skeleton interface for Bison LALR(1) parsers in C++])
dnl FIXME: This is wrong, we want computed header guards.
[
/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H

]b4_percent_code_get([[requires]])[

]b4_assert_if([#include <cassert>])[
#include <string>
#include <iostream>
#include "stack.hh"

]b4_namespace_open[
]b4_locations_if([  class position;
  class location;])[
]b4_variant_if([b4_variant_definition])[
]b4_namespace_close[

]b4_locations_if([#include "location.hh"])[

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG ]b4_debug_flag[
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE ]b4_error_verbose_flag[
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE ]b4_token_table[
#endif

]b4_locations_if([dnl
[/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                        \
do {                                                            \
  if (N)                                                        \
    {                                                           \
      (Current).begin = (Rhs)[1].location.begin;                \
      (Current).end   = (Rhs)[N].location.end;			\
    }                                                           \
  else                                                          \
    {                                                           \
      (Current).begin = (Current).end = (Rhs)[0].location.end;	\
    }                                                           \
} while (false)
#endif]])[

]b4_namespace_open[

  /// A Bison parser.
  class ]b4_parser_class_name[
  {
  public:
#ifndef YYSTYPE
]b4_variant_if(
[    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {]b4_type_foreach([b4_char_sizeof])[};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;],
[    /// Symbol semantic values.
m4_ifdef([b4_stype],
[    union semantic_type
    {b4_user_stype
    };],
[m4_if(b4_tag_seen_flag, 0,
[[    typedef int semantic_type;]],
[[    typedef YYSTYPE semantic_type;]])])])[
#else
    typedef YYSTYPE semantic_type;
#endif]b4_locations_if([
    /// Symbol locations.
    typedef b4_percent_define_get([[location_type]]) location_type;])[
    /// Tokens.
    struct token
    {
      ]b4_token_enums(b4_tokens)[
    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    ]b4_parser_class_name[ (]b4_parse_param_decl[);
    virtual ~]b4_parser_class_name[ ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.]b4_locations_if([
    /// \param loc    where the syntax error is found.])[
    /// \param msg    a description of the syntax error.
    virtual void error (]b4_locations_if([const location_type& loc, ])[const std::string& msg);

  private:
    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

    /// State numbers.
    typedef int state_type;

    /// Internal symbol numbers.
    typedef ]b4_int_type_for([b4_translate])[ token_number_type;
    static const ]b4_int_type(b4_pact_ninf, b4_pact_ninf)[ yypact_ninf_;
    static const ]b4_int_type(b4_table_ninf, b4_table_ninf)[ yytable_ninf_;

    /* Tables.  */
]b4_parser_tables_declare[

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif]b4_error_verbose_if([

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);])[

#if YYDEBUG
]b4_integral_parser_table_declare([rline], [b4_rline],
     [YYRLINE[YYN] -- Source line where rule number YYN was defined.])[
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    static inline token_number_type yytranslate_ (]b4_lex_symbol_if([token_type], [int])[ t);

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

#if YYDEBUG
    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Exact>
    void yy_print_ (std::ostream& yyo,
                    const symbol_base_type<Exact>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param s         The symbol.
    template <typename Exact>
    inline void yy_destroy_ (const char* yymsg,
                             symbol_base_type<Exact>& yysym) const;

  public:
    /// External form of a symbol: its type and attributes.
    struct symbol_type : symbol_base_type<symbol_type>
    {
      /// The parent class.
      typedef symbol_base_type<symbol_type> super_type;

      /// Default constructor.
      inline symbol_type ();

      /// Constructor.
      inline symbol_type (]b4_args([int t],
                                   [const semantic_type& v],
                                   b4_locations_if([const location_type& l]))[);

      inline symbol_type (]b4_args([int t],
                                   b4_locations_if([const location_type& l]))[);

      /// The symbol type.
      int type;

      /// Return the type corresponding to this state.
      inline int type_get_ () const;

      /// Its token.
      inline token_type token () const;
    };

]b4_symbol_constructor_declarations[

  private:
    /// Element of the stack: a state and its attributes.
    struct stack_symbol_type : symbol_base_type<stack_symbol_type>
    {
      /// The parent class.
      typedef symbol_base_type<stack_symbol_type> super_type;

      /// Default constructor.
      inline stack_symbol_type ();

      /// Constructor.
      inline stack_symbol_type (]b4_args([state_type s],
                                         [const semantic_type& v],
                                         b4_locations_if([const location_type& l]))[);

      /// The state.
      state_type state;

      /// Return the type corresponding to this state.
      inline int type_get_ () const;
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    inline void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    inline void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    enum
    {
      yyeof_ = 0,
      yylast_ = ]b4_last[,           //< Last index in yytable_.
      yynnts_ = ]b4_nterms_number[,  //< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = ]b4_final_state_number[, //< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = ]b4_tokens_number[,   //< Number of tokens.
    };

]b4_parse_param_vars[
  };

]b4_lex_symbol_if([b4_yytranslate_definition])[
]b4_lex_symbol_if([b4_symbol_constructor_definitions])[
]b4_namespace_close[

]b4_percent_define_flag_if([[global_tokens_and_yystype]],
[b4_token_defines(b4_tokens)

#ifndef YYSTYPE
 /* Redirection for backward compatibility.  */
# define YYSTYPE b4_namespace_ref::b4_parser_class_name::semantic_type
#endif
])
b4_percent_code_get([[provides]])[]dnl

[#endif /* ! defined PARSER_HEADER_H */]
@output(b4_parser_file_name@)@
b4_copyright([Skeleton implementation for Bison LALR(1) parsers in C++])
b4_percent_code_get([[top]])[]dnl
m4_if(b4_prefix, [yy], [],
[
// Take the name prefix into account.
#define yylex   b4_prefix[]lex])[

/* First part of user declarations.  */
]b4_user_pre_prologue[

#include "@basename(]b4_spec_defines_file[@)"

/* User implementation prologue.  */
]b4_user_post_prologue
b4_percent_code_get[]dnl

[#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)		\
  do {					\
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);		\
  } while (false)

# define YY_STACK_PRINT()		\
  do {					\
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  static_cast<void>(0)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

]b4_namespace_open[]b4_error_verbose_if([[

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ]b4_parser_class_name[::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }
]])[

  /// Build a parser object.
  ]b4_parser_class_name::b4_parser_class_name[ (]b4_parse_param_decl[)]m4_ifset([b4_parse_param], [
    :])[
#if YYDEBUG
    ]m4_ifset([b4_parse_param], [  ], [ :])[yydebug_ (false),
      yycdebug_ (&std::cerr)]m4_ifset([b4_parse_param], [,])[
#endif]b4_parse_param_cons[
  {
  }

  ]b4_parser_class_name::~b4_parser_class_name[ ()
  {
  }


  /*---------------.
  | Symbol types.  |
  `---------------*/

]b4_lex_symbol_if([], [b4_symbol_constructor_definitions])[

  // stack_symbol_type.
  ]b4_parser_class_name[::stack_symbol_type::stack_symbol_type ()
    : super_type ()
    , state ()
  {
  }

  ]b4_parser_class_name[::stack_symbol_type::stack_symbol_type (]b4_args(
                 [state_type s],
                 [const semantic_type& v],
                 b4_locations_if([const location_type& l]))[)
    : super_type (v]b4_locations_if([, l])[)
    , state (s)
  {
  }

  int
  ]b4_parser_class_name[::stack_symbol_type::type_get_ () const
  {
    return yystos_[state];
  }


  template <typename Exact>
  void
  ]b4_parser_class_name[::yy_destroy_ (const char* yymsg,
                                       symbol_base_type<Exact>& yysym) const
  {
    int yytype = yysym.type_get ();
    YYUSE (yymsg);
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yytype)
      {
]b4_symbol_foreach([b4_symbol_destructor])dnl
[       default:
          break;
      }]b4_variant_if([

    // Type destructor.
  b4_symbol_variant([[yytype]], [[yysym.value]], [[template destroy]])])[
  }

#if YYDEBUG
  template <typename Exact>
  void
  ]b4_parser_class_name[::yy_print_ (std::ostream& yyo,
                                     const symbol_base_type<Exact>& yysym) const
  {
    int yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("]b4_locations_if([
        << yysym.location << ": "])[;
    switch (yytype)
      {
]b4_symbol_foreach([b4_symbol_printer])dnl
[       default:
	  break;
      }
    yyo << ')';
  }
#endif

  void
  ]b4_parser_class_name[::yypush_ (const char* m, state_type s,
                                   symbol_type& sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
]b4_variant_if(
[[    yystack_.push (stack_symbol_type (]b4_args(
                    [s],
                    [semantic_type()],
                    b4_locations_if([sym.location]))[));
    ]b4_symbol_variant([[yystos_[s]]], [[yystack_[0].value]],
                       [build], [sym.value])],
[[    yystack_.push (stack_symbol_type (]b4_args(
                      [s],
                      [sym.value],
                      b4_locations_if([sym.location]))[));]])[
  }

  void
  ]b4_parser_class_name[::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
]b4_variant_if(
[[    yystack_.push (stack_symbol_type (]b4_args(
                       [s.state],
                       [semantic_type()],
                       b4_locations_if([s.location]))[));
    ]b4_symbol_variant([[yystos_[s.state]]], [[yystack_[0].value]],
                       [build], [s.value])],
[    yystack_.push (s);])[
  }

  void
  ]b4_parser_class_name[::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  ]b4_parser_class_name[::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ]b4_parser_class_name[::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ]b4_parser_class_name[::debug_level_type
  ]b4_parser_class_name[::debug_level () const
  {
    return yydebug_;
  }

  void
  ]b4_parser_class_name[::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  ]b4_parser_class_name[::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;]b4_locations_if([[

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[2];]])[

    /// $$ and @@$.
    stack_symbol_type yylhs;

    /// The return value of parse().
    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;

]m4_ifdef([b4_initial_action], [
m4_pushdef([b4_at_dollar],     [yyla.location])dnl
m4_pushdef([b4_dollar_dollar], [yyla.value])dnl
    /* User initialization code.  */
    b4_user_initial_action
m4_popdef([b4_dollar_dollar])dnl
m4_popdef([b4_at_dollar])])dnl

  [  /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_ = stack_type (0);
    yypush_ (0, 0, yyla);

    // A new state was pushed on the stack.
    // Invariant: yystate == yystack_[0].state, i.e.,
    // yystate was just pushed onto the state stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
]b4_lex_symbol_if(
[        yyla = b4_c_function_call([yylex], [symbol_type],
                                   m4_ifdef([b4_lex_param], b4_lex_param));],
[        yyla.type = yytranslate_ (b4_c_function_call([yylex], [int],
				     [[YYSTYPE*], [&yyla.value]][]dnl
b4_locations_if([, [[location*], [&yyla.location]]])dnl
m4_ifdef([b4_lex_param], [, ]b4_lex_param)));])[
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Discard the token being shifted.  */
    yyempty = true;

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    /* Shift the lookahead token.  */
    yystate = yyn;
    yypush_ ("Shifting", yystate, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];]b4_variant_if([
    /* Variants are always initialized to an empty instance of the
       correct type. The default $$=$1 rule is NOT applied when using
       variants */
    ]b4_symbol_variant([[yyr1_@{yyn@}]], [yylhs.value], [build]),[
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYLHS.VALUE to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yylhs.value = yystack_@{yylen - 1@}.value;
    else
      yylhs.value = yystack_@{0@}.value;])[
]b4_locations_if([dnl
[
    // Compute the default @@$.
    {
      slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yylhs.location, slice, yylen);
    }]])[

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
]b4_user_actions[
	default:
          break;
      }
    // Compute post-reduction state.
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystack_[yylen].state;
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystack_[yylen].state)
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    yylhs.state = yystate;
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
]b4_variant_if([[
    // Destroy the rhs symbols.
    for (int i = 0; i < yylen; ++i)
      // Destroy a variant which value may have been swapped with
      // yylhs.value (for instance if the action was "std::swap($$,
      // $1)").  The value of yylhs.value (hence possibly one of these
      // rhs symbols) depends on the default contruction for this
      // type.  In the case of pointers for instance, no
      // initialization is done, so the value is junk.  Therefore do
      // not try to report the value of symbols about to be destroyed
      // in the debug trace, it's possibly junk.  Hence yymsg = 0.
      // Besides, that keeps exactly the same traces as with the other
      // Bison skeletons.
      yy_destroy_ (0, yystack_[i]);]])[

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    /* Shift the result of the reduction.  */
    yypush_ (0, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (]b4_args(b4_locations_if([yyla.location]),
                        [yysyntax_error_ (yystate, yyla.type)])[);
      }

]b4_locations_if([[
    yyerror_range[0].location = yyla.location;]])[
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        /* Return failure if at end of input.  */
        if (yyla.type == yyeof_)
          YYABORT;
        else
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

]b4_locations_if([[
    yyerror_range[0].location = yystack_[yylen - 1].location;]])[
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystack_[0].state;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystate];
          if (yyn != yypact_ninf_)
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;
]b4_locations_if([[
          yyerror_range[0].location = yystack_[0].location;]])[
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          yystate = yystack_[0].state;
          YY_STACK_PRINT ();
        }
]b4_locations_if([[
      yyerror_range[1].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, (yyerror_range - 1), 2);]])[

      /* Shift the error token.  */
      error_token.state = yystate = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystack_.size () != 1)
      {
	yy_destroy_ ("Cleanup: popping", yystack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  ]b4_parser_class_name[::yysyntax_error_ (]dnl
b4_error_verbose_if([int yystate, int yytoken],
                    [int, int])[)
  {
    std::string yyres;]b4_error_verbose_if([[
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;

        // Number of reported tokens (one for the "unexpected", one per
        // "expected").
	size_t yycount = 0;
        // Its maximum.
        enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
        // Arguments of yyformat.
        char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
        yyarg[yycount++] = yytname_[yytoken];
	for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
	  if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_)
          {
            if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
            {
              yycount = 1;
              break;
            }
            else
              yyarg[yycount++] = yytname_[yyx];
          }

        char const* yyformat = 0;
        switch (yycount)
        {
#define YYCASE_(N, S)                           \
          case N:                               \
            yyformat = S;                       \
          break
          YYCASE_(1, YY_("syntax error, unexpected %s"));
          YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
          YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
          YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
          YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
        }
        // Argument number.
        size_t yyi = 0;
        for (char const* yyp = yyformat; *yyp; ++yyp)
          if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
          {
            yyres += yytnamerr_ (yyarg[yyi++]);
            ++yyp;
          }
          else
            yyres += *yyp;
      }
    else
  ]])dnl
[    yyres = YY_("syntax error");
    return yyres;
  }


  const ]b4_int_type(b4_pact_ninf, b4_pact_ninf) b4_parser_class_name::yypact_ninf_ = b4_pact_ninf[;

  const ]b4_int_type(b4_table_ninf, b4_table_ninf) b4_parser_class_name::yytable_ninf_ = b4_table_ninf[;

]b4_parser_tables_define[

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const ]b4_parser_class_name[::yytname_[] =
  {
  ]b4_tname[
  };
#endif

#if YYDEBUG
]b4_integral_parser_table_define([rline], [b4_rline])[

  // Print the state stack on the debug stream.
  void
  ]b4_parser_class_name[::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
	 i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  ]b4_parser_class_name[::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       ]b4_rhs_data(yynrhs, yyi + 1)[);
  }
#endif // YYDEBUG

]b4_lex_symbol_if([], [b4_yytranslate_definition])[
]b4_namespace_close[
]b4_epilogue[]dnl
m4_divert_pop(0)
m4_popdef([b4_copyright_years])dnl
