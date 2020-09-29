# C++ GLR skeleton for Bison

# Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

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

# b4_tname_if(TNAME-NEEDED, TNAME-NOT-NEEDED)
# -------------------------------------------
m4_define([b4_tname_if],
[m4_case(b4_percent_define_get([[parse.error]]),
         [verbose],         [$1],
         [b4_token_table_if([$1],
                            [$2])])])

# This skeleton produces a C++ class that encapsulates a C glr parser.
# This is in order to reduce the maintenance burden.  The glr.c
# skeleton is clean and pure enough so that there are no real
# problems.  The C++ interface is the same as that of lalr1.cc.  In
# fact, glr.c can replace yacc.c without the user noticing any
# difference, and similarly for glr.cc replacing lalr1.cc.
#
# The passing of parse-params
#
#   The additional arguments are stored as members of the parser
#   object, yyparser.  The C routines need to carry yyparser
#   throughout the C parser; that's easy: make yyparser an
#   additional parse-param.  But because the C++ skeleton needs to
#   know the "real" original parse-param, we save them
#   (b4_parse_param_orig).  Note that b4_parse_param is overquoted
#   (and c.m4 strips one level of quotes).  This is a PITA, and
#   explains why there are so many levels of quotes.
#
# The locations
#
#   We use location.cc just like lalr1.cc, but because glr.c stores
#   the locations in a union, the position and location classes
#   must not have a constructor.  Therefore, contrary to lalr1.cc, we
#   must not define "b4_location_constructors".  As a consequence the
#   user must initialize the first positions (in particular the
#   filename member).

# We require a pure interface.
m4_define([b4_pure_flag], [1])

m4_include(b4_skeletonsdir/[c++.m4])
b4_bison_locations_if([m4_include(b4_skeletonsdir/[location.cc])])

m4_define([b4_parser_class],
          [b4_percent_define_get([[api.parser.class]])])

# Save the parse parameters.
m4_define([b4_parse_param_orig], m4_defn([b4_parse_param]))

# Hijack the initial action to initialize the locations.
]b4_bison_locations_if([m4_define([b4_initial_action],
[yylloc.initialize ();]m4_ifdef([b4_initial_action], [
m4_defn([b4_initial_action])]))])[

]m4_define([b4_define_symbol_kind],
[m4_format([#define %-15s %s],
           b4_symbol($][1, kind_base),
           b4_namespace_ref[::]b4_parser_class[::symbol_kind::]b4_symbol($1, kind_base))
])

# b4_glr_cc_setup
# ---------------
# Setup redirections for glr.c: Map the names used in c.m4 to the ones used
# in c++.m4.
m4_define([b4_glr_cc_setup],
[[#ifndef ]b4_api_PREFIX[STYPE
# define ]b4_api_PREFIX[STYPE ]b4_namespace_ref[::]b4_parser_class[::semantic_type
#endif
#ifndef ]b4_api_PREFIX[LTYPE
# define ]b4_api_PREFIX[LTYPE ]b4_namespace_ref[::]b4_parser_class[::location_type
#endif
typedef ]b4_namespace_ref[::]b4_parser_class[::symbol_kind_type yysymbol_kind_t;
]])
# b4_glr_cc_cleanup
# -----------------
# Remove redirections for glr.c.
m4_define([b4_glr_cc_cleanup],
[[#undef ]b4_symbol(empty, [id])[
#undef ]b4_symbol(0, [id])[
#undef ]b4_symbol(1, [id])[
#undef ]b4_symbol_prefix[YYEMPTY
#undef ]b4_symbol_prefix[YYerror
#undef ]b4_symbol_prefix[YYEOF
#undef ]b4_symbol_prefix[YYUNDEF
]])

# b4_shared_declarations(hh|cc)
# -----------------------------
# Declaration that might either go into the header (if --header, $1 = hh)
# or in the implementation file.
m4_define([b4_shared_declarations],
[m4_pushdef([b4_parse_param], m4_defn([b4_parse_param_orig]))dnl
b4_percent_code_get([[requires]])[
#include <algorithm>
#include <cstddef> // ptrdiff_t
#include <cstring> // memcpy
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

]b4_cxx_portability[
]m4_ifdef([b4_location_include],
          [[# include ]b4_location_include])[
]b4_variant_if([b4_variant_includes])[

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

]b4_YYDEBUG_define[

]b4_namespace_open[

template <typename Parameter>
class strong_index_alias
{
 public:
  static strong_index_alias create(std::ptrdiff_t value) {
    strong_index_alias result;
    result.value_ = value;
    return result;
  }

  std::ptrdiff_t const& get() const { return value_; }

  size_t uget() const { return static_cast<size_t>(value_); }

  strong_index_alias operator+(std::ptrdiff_t other) const {
    return strong_index_alias(get() + other);
  }

  void operator+=(std::ptrdiff_t other) {
    value_ += other;
  }

  strong_index_alias operator-(std::ptrdiff_t other) {
    return strong_index_alias(get() - other);
  }

  void operator-=(std::ptrdiff_t other) {
    value_ -= other;
  }

  size_t operator-(strong_index_alias other) {
    return strong_index_alias(get() - other.get());
  }

  strong_index_alias& operator++() {
    ++value_;
    return *this;
  }

  bool isValid() const {
    return value_ != INVALID_INDEX;
  }

  void setInvalid() {
    value_ = INVALID_INDEX;
  }

  bool operator==(strong_index_alias other) {
    return get() == other.get();
  }

  bool operator!=(strong_index_alias other) {
    return get() != other.get();
  }

  bool operator<(strong_index_alias other) {
    return get() < other.get();
  }

 private:
  static const std::ptrdiff_t INVALID_INDEX;

  // WARNING: 0-initialized.
  std::ptrdiff_t value_;
};

template<typename T>
const std::ptrdiff_t strong_index_alias<T>::INVALID_INDEX =
  std::numeric_limits<std::ptrdiff_t>::max();

]b4_bison_locations_if([m4_ifndef([b4_location_file],
                                  [b4_location_define])])[

  /// A Bison parser.
  class ]b4_parser_class[
  {
  public:
]b4_public_types_declare[
]b4_symbol_type_define[

    /// Build a parser object.
    ]b4_parser_class[ (]b4_parse_param_decl[);
    virtual ~]b4_parser_class[ ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if ]b4_api_PREFIX[DEBUG
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

    /// Report a syntax error.]b4_locations_if([[
    /// \param loc    where the syntax error is found.]])[
    /// \param msg    a description of the syntax error.
    virtual void error (]b4_locations_if([[const location_type& loc, ]])[const std::string& msg);

]b4_parse_error_bmatch(
[custom\|detailed],
[[    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);]],
[simple],
[[#if ]b4_api_PREFIX[DEBUG || ]b4_token_table_flag[
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if ]b4_api_PREFIX[DEBUG || ]b4_token_table_flag[
]],
[verbose],
[[    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);]])[

# if ]b4_api_PREFIX[DEBUG
  public:
    /// \brief Report a symbol value on the debug stream.
    /// \param yykind       The symbol kind.
    /// \param yyvaluep     Its semantic value.]b4_locations_if([[
    /// \param yylocationp  Its location.]])[
    virtual void yy_symbol_value_print_ (symbol_kind_type yykind,
                                         const semantic_type* yyvaluep]b4_locations_if([[,
                                         const location_type* yylocationp]])[) const;
    /// \brief Report a symbol on the debug stream.
    /// \param yykind       The symbol kind.
    /// \param yyvaluep     Its semantic value.]b4_locations_if([[
    /// \param yylocationp  Its location.]])[
    virtual void yy_symbol_print_ (symbol_kind_type yykind,
                                   const semantic_type* yyvaluep]b4_locations_if([[,
                                   const location_type* yylocationp]])[) const;
  private:
    /// Debug stream.
    std::ostream* yycdebug_;
#endif

  public: // FIXME: Private
]b4_parse_error_bmatch(
[simple],
[[#if ]b4_api_PREFIX[DEBUG || ]b4_token_table_flag[
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if ]b4_api_PREFIX[DEBUG || ]b4_token_table_flag[
]],
[verbose],
[[    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];
]])[

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yykind    The symbol kind.
    void yy_destroy_ (const char* yymsg, symbol_kind_type yykind,
                      const semantic_type* yyvaluep]b4_locations_if([[,
                      const location_type* yylocationp]])[);

]b4_parse_param_vars[
  };

]b4_namespace_close[

]b4_percent_code_get([[provides]])[
]m4_popdef([b4_parse_param])dnl
])[

]b4_header_if(
[b4_output_begin([b4_spec_header_file])
b4_copyright([Skeleton interface for Bison GLR parsers in C++],
             [2002-2015, 2018-2020])[
// C++ GLR parser skeleton written by Valentin Tolmer.

]b4_disclaimer[
]b4_cpp_guard_open([b4_spec_mapped_header_file])[
]b4_shared_declarations[
]b4_cpp_guard_close([b4_spec_mapped_header_file])[
]b4_output_end])

## ---------------- ##
## Default values.  ##
## ---------------- ##

# Stack parameters.
m4_define_default([b4_stack_depth_max], [10000])
m4_define_default([b4_stack_depth_init],  [200])



## ------------------------ ##
## Pure/impure interfaces.  ##
## ------------------------ ##

b4_define_flag_if([pure])

# b4_user_formals
# ---------------
# The possible parse-params formal arguments preceded by a comma.
#
# This is not shared with yacc.c in c.m4 because  GLR relies on ISO C
# formal argument declarations.
m4_define([b4_user_formals],
[m4_ifset([b4_parse_param], [, b4_formals(b4_parse_param)])])


# b4_symbol_kind(NUM)
# -------------------
m4_define([b4_symbol_kind],
[symbol_kind::b4_symbol_kind_base($@)])


## ----------------- ##
## Semantic Values.  ##
## ----------------- ##


# b4_lhs_value(SYMBOL-NUM, [TYPE])
# --------------------------------
# See README.
m4_define([b4_lhs_value],
[b4_symbol_value([(*yyvalp)], [$1], [$2])])


# b4_rhs_data(RULE-LENGTH, POS)
# -----------------------------
# See README.
m4_define([b4_rhs_data],
[(static_cast<glr_stack_item const *>(yyvsp))@{YYFILL (b4_subtract([$2], [$1]))@}.getState()])


# b4_rhs_value(RULE-LENGTH, POS, SYMBOL-NUM, [TYPE])
# --------------------------------------------------
# Expansion of $$ or $<TYPE>$, for symbol SYMBOL-NUM.
m4_define([b4_rhs_value],
[b4_symbol_value([b4_rhs_data([$1], [$2]).semanticVal()], [$3], [$4])])



## ----------- ##
## Locations.  ##
## ----------- ##

# b4_lhs_location()
# -----------------
# Expansion of @$.
m4_define([b4_lhs_location],
[(*yylocp)])


# b4_rhs_location(RULE-LENGTH, NUM)
# ---------------------------------
# Expansion of @NUM, where the current rule has RULE-LENGTH symbols
# on RHS.
m4_define([b4_rhs_location],
[(b4_rhs_data([$1], [$2]).yyloc)])


## -------------- ##
## Output files.  ##
## -------------- ##

# Unfortunately the order of generation between the header and the
# implementation file matters (for glr.c) because of the current
# implementation of api.value.type=union.  In that case we still use a
# union for YYSTYPE, but we generate the contents of this union when
# setting up YYSTYPE.  This is needed for other aspects, such as
# defining yy_symbol_value_print, since we need to now the name of the
# members of this union.
#
# To avoid this issue, just generate the header before the
# implementation file.  But we should also make them more independant.


# ------------------------- #
# The implementation file.  #
# ------------------------- #

b4_output_begin([b4_parser_file_name])
b4_copyright([Skeleton implementation for Bison GLR parsers in C],
             [2002-2015, 2018-2019])[
/* New C++ GLR parser skeleton written by Akim Demaille, adapted by Valentin
 * Tolmer.  */

]b4_disclaimer[
]b4_identification[

]b4_percent_code_get([[top]])[
]m4_if(b4_api_prefix, [yy], [],
[[/* Substitute the type names.  */
#define YYSTYPE ]b4_api_PREFIX[STYPE]b4_locations_if([[
#define YYLTYPE ]b4_api_PREFIX[LTYPE]])])[
]m4_if(b4_prefix, [yy], [],
[[/* Substitute the variable and function names.  */
#define yyparse ]b4_prefix[parse
#define yylex   ]b4_prefix[lex
#define yyerror ]b4_prefix[error
#define yydebug ]b4_prefix[debug]]b4_pure_if([], [[
#define yylval  ]b4_prefix[lval
#define yychar  ]b4_prefix[char
#define yynerrs ]b4_prefix[nerrs]b4_locations_if([[
#define yylloc  ]b4_prefix[lloc]])]))[

]b4_user_pre_prologue[

]b4_null_define[

]b4_header_if([[#include "@basename(]b4_spec_header_file[@)"]],
               [b4_shared_declarations])[

]b4_glr_cc_setup[

#ifndef ]b4_api_PREFIX[STYPE
# define ]b4_api_PREFIX[STYPE ]b4_namespace_ref[::]b4_parser_class[::semantic_type
#endif
#ifndef ]b4_api_PREFIX[LTYPE
# define ]b4_api_PREFIX[LTYPE ]b4_namespace_ref[::]b4_parser_class[::location_type
#endif

typedef ]b4_namespace_ref[::]b4_parser_class[::symbol_kind_type yysymbol_kind_t;

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;]b4_locations_if([[
static YYLTYPE yyloc_default][]b4_yyloc_default;])[

]b4_user_post_prologue[
]b4_percent_code_get[]dnl

[#include <cstdio>
#include <cstdlib>


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(Env) setjmp (Env)
/* Pacify Clang and ICC.  */
# define YYLONGJMP(Env, Val)                    \
 do {                                           \
   longjmp (Env, Val);                          \
   YYASSERT (0);                                \
 } while (false)
#endif

]b4_attribute_define([noreturn])[

#if defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_NULL_DEREFERENCE_BEGIN                               \
  _Pragma ("GCC diagnostic push")                                       \
  _Pragma ("GCC diagnostic ignored \"-Wnull-dereference\"")
# define YY_IGNORE_NULL_DEREFERENCE_END         \
  _Pragma ("GCC diagnostic pop")
#else
# define YY_IGNORE_NULL_DEREFERENCE_BEGIN
# define YY_IGNORE_NULL_DEREFERENCE_END
#endif

]b4_null_define[
]b4_cast_define[

#ifndef YYASSERT
# define YYASSERT(Condition) ((void) ((Condition) || (abort (), 0)))
#endif

#ifdef YYDEBUG
# define YYDASSERT(Condition) YYASSERT(Condition)
#else
# define YYDASSERT(Condition)
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  ]b4_final_state_number[
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   ]b4_last[

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  ]b4_tokens_number[
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  ]b4_nterms_number[
/* YYNRULES -- Number of rules.  */
#define YYNRULES  ]b4_rules_number[
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  ]b4_states_number[
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS ]b4_r2_max[
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT ]b4_max_left_semantic_context[

/* YYUNDEFTOK -- Symbol number (for yytoken) that denotes an unknown
   token.  */
#define YYUNDEFTOK  ]b4_undef_token_number[

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
]b4_api_token_raw_if(dnl
[[#define YYTRANSLATE(YYX) static_cast<yysymbol_kind_t>(YYX)]],
[[#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= ]b4_code_max[                     \
   ? static_cast<yysymbol_kind_t>(yytranslate[YYX])        \
   : ]b4_namespace_ref::b4_parser_class::b4_symbol(empty, kind)[)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const ]b4_int_type_for([b4_translate])[ yytranslate[] =
{
  ]b4_translate[
};]])[

#if ]b4_api_PREFIX[DEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const ]b4_int_type_for([b4_rline])[ yyrline[] =
{
  ]b4_rline[
};
#endif

#define YYPACT_NINF ]b4_pact_ninf[
#define YYTABLE_NINF ]b4_table_ninf[

]b4_parser_tables_define[

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const ]b4_int_type_for([b4_dprec])[ yydprec[] =
{
  ]b4_dprec[
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const ]b4_int_type_for([b4_merger])[ yymerger[] =
{
  ]b4_merger[
};

/* YYIMMEDIATE[RULE-NUM] -- True iff rule #RULE-NUM is not to be deferred, as
   in the case of predicates.  */
static const bool yyimmediate[] =
{
  ]b4_immediate[
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const ]b4_int_type_for([b4_conflict_list_heads])[ yyconflp[] =
{
  ]b4_conflict_list_heads[
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
]dnl Do not use b4_int_type_for here, since there are places where
dnl pointers onto yyconfl are taken, whose type is "short*".
dnl We probably ought to introduce a type for confl.
[static const short yyconfl[] =
{
  ]b4_conflicting_rules[
};

/* Error token number */
#define YYTERROR 1

]b4_locations_if([[
]b4_yylloc_default_define[
# define YYRHSLOC(Rhs, K) ((Rhs)[K].getState().yyloc)
]])[

]b4_pure_if(
[
#undef yynerrs
#define yynerrs (yystackp->yyerrcnt)
#undef yychar
#define yychar (yystackp->yyrawchar)
#undef yylval
#define yylval (yystackp->yyval)
#undef yylloc
#define yylloc (yystackp->yyloc)
m4_if(b4_prefix[], [yy], [],
[#define b4_prefix[]nerrs yynerrs
#define b4_prefix[]char yychar
#define b4_prefix[]lval yylval
#define b4_prefix[]lloc yylloc])],
[YYSTYPE yylval;]b4_locations_if([[
YYLTYPE yylloc;]])[

int yynerrs;
int yychar;])[

enum YYRESULTTAG { yyok, yyaccept, yyabort, yyerr };

#define YYCHK(YYE)                              \
  do {                                          \
    YYRESULTTAG yychk_flag = YYE;               \
    if (yychk_flag != yyok)                     \
      return yychk_flag;                        \
  } while (0)

#if ]b4_api_PREFIX[DEBUG

]b4_yy_location_print_define[

#define YY_DEBUG_STREAM if (!yydebug) {} else std::cerr

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                  \
  do {                                                                  \
    if (yydebug)                                                        \
      {                                                                 \
        std::cerr << Title << ' ';                                      \
        yyparser.yy_symbol_print_ (Type, Value]b4_locations_if([, Location])[); \
        std::cerr << '\n';                                              \
      }                                                                 \
  } while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

class glr_stack;
static void yypstack (glr_stack* yystackp, size_t yyk)
  YY_ATTRIBUTE_UNUSED;
static void yypdumpstack (glr_stack* yystackp)
  YY_ATTRIBUTE_UNUSED;

#else /* !]b4_api_PREFIX[DEBUG */

# define YY_DEBUG_STREAM if (true) {} else std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !]b4_api_PREFIX[DEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH ]b4_stack_depth_init[
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH ]b4_stack_depth_max[
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# define YYSTACKEXPANDABLE 1
#endif

/** State numbers, as in LALR(1) machine */
typedef int state_num;

/** Rule numbers, as in LALR(1) machine */
typedef int rule_num;

// Forward declarations.
class glr_state;
class semantic_option;
class glr_stack_item;
class glr_stack;

typedef ]b4_namespace_ref[::strong_index_alias<struct glr_state_set_tag> state_set_index;

state_set_index create_state_set_index(std::ptrdiff_t value) {
  return state_set_index::create(value);
}

#define yypact_value_is_default(Yystate) \
  ]b4_table_value_equals([[pact]], [[Yystate]], [b4_pact_ninf], [YYPACT_NINF])[

#define yytable_value_is_error(Yytable_value) \
  ]b4_table_value_equals([[table]], [[Yytable_value]], [b4_table_ninf], [YYTABLE_NINF])[

]m4_define([b4_yygetToken_call],
           [[yygetToken (&yychar, yyparser][]b4_pure_if([, yystackp])[]b4_user_args[)]])[

static inline yysymbol_kind_t
yygetToken (int *yycharp, ]b4_namespace_ref[::]b4_parser_class[& yyparser][]b4_pure_if([, glr_stack* yystackp])[]b4_user_formals[);

static inline bool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline bool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

static inline int
yygetLRActions (state_num yystate, yysymbol_kind_t yytoken, const short** yyconflicts);

/** True iff LR state YYSTATE has only a default reduction (regardless
 *  of token).  */
static inline bool
yyisDefaultedState (state_num yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for YYSTATE, assuming it has one.  */
static inline rule_num
yydefaultAction (state_num yystate)
{
  return yydefact[yystate];
}

static inline int
yyrhsLength (rule_num yyrule);


class glr_state
{
public:
  glr_state()
    : yyresolved(false)
    , yylrState(0)
    , yyposn(0)
    , yypred(0)
  {}

  /// Build with a semantic value.
  glr_state(state_num lrState, size_t posn, YYSTYPE sval]b4_locations_if([[, YYLTYPE loc]])[)
    : yyresolved(true)
    , yylrState(lrState)
    , yyposn(posn)
    , yypred(0)]b4_locations_if([[
    , yyloc(loc)]])[
  {
    semanticVal() = sval;
  }

  /// Build with a semantic option.
  glr_state(state_num lrState, size_t posn)
    : yyresolved(false)
    , yylrState(lrState)
    , yyposn(posn)
    , yypred(0)
    , yyfirstVal(0)
  {}

  void copyFrom(const glr_state& other) {
    *this = other;
    setPred(other.pred());
    setFirstVal(other.firstVal());
  }

  /** Type tag for   If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  bool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  state_num yylrState;
  /** Source position of the last token produced by my symbol */
  size_t yyposn;

  /// Only call pred() and setPred() on objects in yyitems, not temporaries.
  glr_state* pred();
  const glr_state* pred() const;
  void setPred(const glr_state* state);

  /// Only call firstVal() and setFirstVal() on objects in yyitems, not
  /// temporaries.
  semantic_option* firstVal();
  const semantic_option* firstVal() const;
  void setFirstVal(const semantic_option* option);

  YYSTYPE& semanticVal() {
    return yysval;
  }

  const YYSTYPE& semanticVal() const {
    return yysval;
  }

  void
  destroy (char const *yymsg, ]b4_namespace_ref[::]b4_parser_class[& yyparser]b4_user_formals[);

  /* DEBUGGING ONLY */
#if ]b4_api_PREFIX[DEBUG
  void yy_yypstack() const
  {
    if (pred() != YY_NULLPTR)
      {
        pred()->yy_yypstack();
        std::cerr << " -> ";
      }
    std::cerr << yylrState << "@@" << yyposn;
  }
#endif

  std::ptrdiff_t indexIn(glr_stack_item* array);

  glr_stack_item* asItem() {
    return asItem(this);
  }

 private:
  template <typename T>
  static const glr_stack_item* asItem(const T* state) {
    return reinterpret_cast<const glr_stack_item*>(state);
  }
  template <typename T>
  static glr_stack_item* asItem(T* state) {
    return reinterpret_cast<glr_stack_item*>(state);
  }
  /** Preceding state in this stack */
  std::ptrdiff_t yypred;
  union {
    /** First in a chain of alternative reductions producing the
     *  nonterminal corresponding to this state, threaded through
     *  yyfirstVal.  */
    std::ptrdiff_t yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  };]b4_locations_if([[
 public:
  /** Source location for this state.  */
  YYLTYPE yyloc;]])[
};

/** A stack of GLRState representing the different heads during
  * nondeterministic evaluation. */
class glr_state_set {
 public:
  /** Initialize YYSET to a singleton set containing an empty stack.  */
  glr_state_set()
    : yylastDeleted(YY_NULLPTR)
  {
    yystates.push_back(YY_NULLPTR);
    yylookaheadNeeds.push_back(false);
  }

  // Behave like a vector of states.
  glr_state*& operator[](state_set_index index) {
    return yystates[index.uget()];
  }

  glr_state* operator[](state_set_index index) const {
    return yystates[index.uget()];
  }

  size_t size() const {
    return yystates.size();
  }

  std::vector<glr_state*>::iterator begin() {
    return yystates.begin();
  }

  std::vector<glr_state*>::iterator end() {
    return yystates.end();
  }


  bool lookaheadNeeds(state_set_index index) const {
    return yylookaheadNeeds[index.uget()];
  }

  bool setLookaheadNeeds(state_set_index index, bool value) {
    return yylookaheadNeeds[index.uget()] = value;
  }

  /** Invalidate stack #YYK.  */
  inline void
  yymarkStackDeleted (state_set_index yyk)
  {
    size_t k = yyk.uget();
    if (yystates[k] != YY_NULLPTR)
      yylastDeleted = yystates[k];
    yystates[k] = YY_NULLPTR;
  }

  /** Undelete the last stack in *this that was marked as deleted.  Can
      only be done once after a deletion, and only when all other stacks have
      been deleted.  */
  void
  yyundeleteLastStack ()
  {
    if (yylastDeleted == YY_NULLPTR || !yystates.empty())
      return;
    yystates.push_back(yylastDeleted);
    YY_DEBUG_STREAM <<  "Restoring last deleted stack as stack #0.\n";
    clearLastDeleted();
  }

  /** Remove the dead stacks (yystates[i] == YY_NULLPTR) and shift the later
   * ones.  */
  inline void
  yyremoveDeletes ()
  {
    std::ptrdiff_t newsize = static_cast<std::ptrdiff_t>(yystates.size());
    /* j is the number of live stacks we have seen.  */
    for (size_t i = 0, j = 0; i < yystates.size(); ++i)
      {
        if (yystates[i] == YY_NULLPTR)
          {
            if (i == j)
              {
                YY_DEBUG_STREAM <<  "Removing dead stacks.\n";
              }
            newsize -= 1;
          }
        else
          {
            yystates[j] = yystates[i];
            /* In the current implementation, it's unnecessary to copy
               yylookaheadNeeds[i] since, after
               yyremoveDeletes returns, the parser immediately either enters
               deterministic operation or shifts a token.  However, it doesn't
               hurt, and the code might evolve to need it.  */
            yylookaheadNeeds[j] = yylookaheadNeeds[i];
            if (j != i)
              {
                YY_DEBUG_STREAM << "Rename stack " << i << " -> " << j << ".\n";
              }
            j += 1;
          }
        i += 1;
      }
    yystates.erase(yystates.begin() + newsize, yystates.end());
    yylookaheadNeeds.erase(yylookaheadNeeds.begin() + newsize,
                           yylookaheadNeeds.end());
  }


  state_set_index
  yysplitStack (state_set_index yyk)
  {
    size_t k = yyk.uget();
    yystates.push_back(yystates[k]);
    yylookaheadNeeds.push_back(yylookaheadNeeds[k]);
    return create_state_set_index(static_cast<std::ptrdiff_t>(yystates.size() - 1));
  }

  void clearLastDeleted() {
    yylastDeleted = YY_NULLPTR;
  }

 private:

  std::vector<glr_state*> yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != ]b4_symbol(empty, id)[.  */
  std::vector<bool> yylookaheadNeeds;

  /** The last stack we invalidated.  */
  glr_state* yylastDeleted;

  static const size_t INITIAL_NUMBER_STATES = 16;
};

class semantic_option {
 public:
  semantic_option()
    : yyrule(0)
    , yystate(0)
    , yynext(0)
    , yyrawchar(0)
  {}

  semantic_option(rule_num rule, int rawChar)
    : yyrule(rule)
    , yystate(0)
    , yynext(0)
    , yyrawchar(rawChar)
  {}

  /// Only call state() and setState() on objects in yyitems, not temporaries.
  glr_state* state();
  const glr_state* state() const;
  void setState(const glr_state* s);

  semantic_option* next();
  void setNext(const semantic_option* s);

  std::ptrdiff_t indexIn(glr_stack_item* array);

  /** True iff YYY0 and YYY1 represent identical options at the top level.
   *  That is, they represent the same rule applied to RHS symbols
   *  that produce the same terminal symbols.  */
  bool
  isIdenticalTo (semantic_option* yyy1)
  {
    if (this->yyrule == yyy1->yyrule)
      {
        glr_state *yys0, *yys1;
        int yyn;
        for (yys0 = this->state(),
             yys1 = yyy1->state(),
             yyn = yyrhsLength (this->yyrule);
             yyn > 0;
             yys0 = yys0->pred(),
             yys1 = yys1->pred(), yyn -= 1)
          if (yys0->yyposn != yys1->yyposn)
            return false;
        return true;
      }
    else
      return false;
  }

  /** Assuming identicalOptions (YYY0,YYY1), destructively merge the
   *  alternative semantic values for the RHS-symbols of YYY1 and YYY0.  */
  void
  mergeWith (semantic_option* yyy1)
  {
    glr_state *yys0 = this->state();
    glr_state *yys1 = yyy1->state();
    for (int yyn = yyrhsLength (this->yyrule);
         yyn > 0;
         yyn -= 1,
         yys0 = yys0->pred(),
         yys1 = yys1->pred())
      {
        if (yys0 == yys1)
          break;
        else if (yys0->yyresolved)
          {
            yys1->yyresolved = true;
            yys1->semanticVal() = yys0->semanticVal();
          }
        else if (yys1->yyresolved)
          {
            yys0->yyresolved = true;
            yys0->semanticVal() = yys1->semanticVal();
          }
        else
          {
            semantic_option* yyz0prev = YY_NULLPTR;
            semantic_option* yyz0 = yys0->firstVal();
            semantic_option* yyz1 = yys1->firstVal();
            while (true)
              {
                if (yyz1 == yyz0 || yyz1 == YY_NULLPTR)
                  break;
                else if (yyz0 == YY_NULLPTR)
                  {
                    if (yyz0prev != YY_NULLPTR) {
                      yyz0prev->setNext(yyz1);
                    } else {
                      yys0->setFirstVal(yyz1);
                    }
                    break;
                  }
                else if (yyz0 < yyz1)
                  {
                    semantic_option* yyz = yyz0;
                    if (yyz0prev != YY_NULLPTR) {
                      yyz0prev->setNext(yyz1);
                    } else {
                      yys0->setFirstVal(yyz1);
                    }
                    yyz1 = yyz1->next();
                    yyz0->setNext(yyz);
                  }
                yyz0prev = yyz0;
                yyz0 = yyz0->next();
              }
            yys1->setFirstVal(yys0->firstVal());
          }
      }
  }

  /** Rule number for this reduction */
  rule_num yyrule;

 private:
  template <typename T>
  static const glr_stack_item* asItem(const T* state) {
    return reinterpret_cast<const glr_stack_item*>(state);
  }
  template <typename T>
  static glr_stack_item* asItem(T* state) {
    return reinterpret_cast<glr_stack_item*>(state);
  }
  /** The last RHS state in the list of states to be reduced.  */
  std::ptrdiff_t yystate;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  std::ptrdiff_t yynext;
 public:
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;]b4_locations_if([[
  YYLTYPE yyloc;]])[
};

/** Type of the items in the GLR stack.  The is_state_ field
 *  indicates which item of the union is valid.  */
class glr_stack_item
{
public:
  glr_stack_item (bool state = true)
    : is_state_ (state)
  {
    if (is_state_)
      new (&raw_) glr_state;
    else
      new (&raw_) semantic_option;
  }

  glr_stack_item(const glr_stack_item& other) YY_NOEXCEPT YY_NOTHROW
    : is_state_(other.is_state_)
  {
    std::memcpy(raw_, other.raw_, union_size);
  }

  glr_stack_item& operator=(glr_stack_item other)
  {
    std::swap(is_state_, other.is_state_);
    std::swap(raw_, other.raw_);
    return *this;
  }

  ~glr_stack_item()
  {
    if (is_state())
      getState().~glr_state();
    else
      getOption().~semantic_option();
  }

  glr_state& getState ()
  {
    YYDASSERT (is_state ());
    void *yyp = raw_;
    return *static_cast<glr_state*> (yyp);
  }
  const glr_state& getState () const
  {
    YYDASSERT (is_state());
    const void *yyp = raw_;
    return *static_cast<const glr_state*> (yyp);
  }

  semantic_option& getOption ()
  {
    YYDASSERT (!is_state ());
    void *yyp = raw_;
    return *static_cast<semantic_option*> (yyp);
  }
  const semantic_option& getOption () const
  {
    YYDASSERT (!is_state ());
    const void *yyp = raw_;
    return *static_cast<const semantic_option*> (yyp);
  }
  bool is_state () const
  {
    return is_state_;
  }
 private:
  /// The possible contents of raw_. Since they have constructors, they cannot
  /// be directly included in the union.
  struct contents {
    union {
      char yystate[sizeof(glr_state)];
      char yyoption[sizeof(semantic_option)];
    };
  };
  enum { union_size = sizeof(contents) };
  union {
    /// Strongest alignment constraints.
    long double yyalign_me;
    /// A buffer large enough to store the contents.
    char raw_[union_size];
  };
  /** Type tag for the union. */
  bool is_state_;
};

glr_state* glr_state::pred ()
{
  YY_IGNORE_NULL_DEREFERENCE_BEGIN
  return yypred ? &(asItem (this) - yypred)->getState () : YY_NULLPTR;
  YY_IGNORE_NULL_DEREFERENCE_END
}

const glr_state* glr_state::pred () const
{
  YY_IGNORE_NULL_DEREFERENCE_BEGIN
  return yypred ? &(asItem (this) - yypred)->getState () : YY_NULLPTR;
  YY_IGNORE_NULL_DEREFERENCE_END
}

void glr_state::setPred(const glr_state* state) {
  yypred = state ? asItem(this) - asItem(state) : 0;
}

semantic_option* glr_state::firstVal() {
  return yyfirstVal ? &(asItem(this) - yyfirstVal)->getOption() : YY_NULLPTR;
}

const semantic_option* glr_state::firstVal() const {
  return yyfirstVal ? &(asItem(this) - yyfirstVal)->getOption() : YY_NULLPTR;
}

void glr_state::setFirstVal(const semantic_option* option) {
  yyfirstVal = option ? asItem(this) - asItem(option) : 0;
}

std::ptrdiff_t glr_state::indexIn(glr_stack_item* array) {
  return asItem(this) - array;
}

std::ptrdiff_t semantic_option::indexIn(glr_stack_item* array) {
  return asItem(this) - array;
}

glr_state* semantic_option::state() {
  return yystate ? &(asItem(this) - yystate)->getState() : YY_NULLPTR;
}

const glr_state* semantic_option::state() const {
  return yystate ? &(asItem(this) - yystate)->getState() : YY_NULLPTR;
}

void semantic_option::setState(const glr_state* s) {
  yystate = s ? asItem(this) - asItem(s) : 0;
}

semantic_option* semantic_option::next() {
  return yynext ? &(asItem(this) - yynext)->getOption() : YY_NULLPTR;
}

void semantic_option::setNext(const semantic_option* s) {
  yynext = s ? asItem(this) - asItem(s) : 0;
}

void glr_state::destroy (char const *yymsg, ]b4_namespace_ref[::]b4_parser_class[& yyparser]b4_user_formals[)
{
  if (yyresolved)
    yyparser.yy_destroy_ (yymsg, static_cast<yysymbol_kind_t>(yystos[yylrState]),
                &semanticVal()]b4_locations_if([, &yyloc])[);
  else
    {
      YY_SYMBOL_PRINT (yymsg << (firstVal() ? " unresolved" : " incomplete"),
                       static_cast<yysymbol_kind_t>(yystos[yylrState]), YY_NULLPTR, &yyloc);

      if (firstVal() != YY_NULLPTR)
        {
          semantic_option *yyoption = firstVal();
          glr_state *yyrh = yyoption->state();
          for (int yyn = yyrhsLength (yyoption->yyrule); yyn > 0; yyn -= 1)
            {
              yyrh->destroy (yymsg, yyparser]b4_user_args[);
              yyrh = yyrh->pred();
            }
        }
    }
}


static int
yypreference (semantic_option* y0, semantic_option* y1);

static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1);


/** Left-hand-side symbol for rule #YYRULE.  */
static inline yysymbol_kind_t
yylhsNonterm (rule_num yyrule)
{
  return static_cast<yysymbol_kind_t>(yyr1[yyrule]);
}

static inline state_num
yyLRgotoState (state_num yystate, yysymbol_kind_t yysym);

#undef YYFILL
#define YYFILL(N) yyfill (yyvsp, &yylow, (N), yynormal)

class state_stack {
 public:
  /** Initialize to a single empty stack, with total maximum
   *  capacity for all stacks of YYSIZE.  */
  state_stack (size_t yysize)
    : yysplitPoint(YY_NULLPTR)
  {
    yyitems.reserve(yysize);
  }

#if YYSTACKEXPANDABLE
  /** Returns false if it tried to expand but could not. */
  bool
  yyexpandGLRStackIfNeeded() {
    return spaceLeft() < YYHEADROOM && yyexpandGLRStack();
  }

 private:
  /** If *this is expandable, extend it.  WARNING: Pointers into the
      stack from outside should be considered invalid after this call.
      We always expand when there are 1 or fewer items left AFTER an
      allocation, so that we can avoid having external pointers exist
      across an allocation.  */
  bool
  yyexpandGLRStack()
  {
    if (YYMAXDEPTH - YYHEADROOM < yyitems.size())
      return false;
    const size_t yynewSize = YYMAXDEPTH < 2 * yyitems.size() ? YYMAXDEPTH : 2 * yyitems.size();
    yyitems.reserve(yynewSize);
    return true;
  }
 public:
#else
  bool yyexpandGLRStackIfNeeded() {
    return yystateStack.spaceLeft() < YYHEADROOM;
  }
#endif

  static bool yyGLRStateNotNull(glr_state* s) {
    return s != YY_NULLPTR;
  }

  bool
  reduceToOneStack() {
    const std::vector<glr_state*>::iterator yybegin = yytops.begin();
    const std::vector<glr_state*>::iterator yyend = yytops.end();
    std::vector<glr_state*>::iterator yyit =
      std::find_if(yybegin, yyend, yyGLRStateNotNull);
    if (yyit == yyend)
      return false;
    for (state_set_index yyk = create_state_set_index(yyit + 1 - yybegin);
         yyk.uget() != numTops(); ++yyk)
      yytops.yymarkStackDeleted (yyk);
    yytops.yyremoveDeletes ();
    yycompressStack ();
    return true;
  }

  void
  yycompressStack ()
  {
    if (yytops.size() != 1 || !isSplit())
      return;

    glr_state* yyr = YY_NULLPTR;
    for (glr_state *yyp = firstTop(), *yyq = yyp->pred();
         yyp != yysplitPoint;
         yyr = yyp, yyp = yyq, yyq = yyp->pred())
      yyp->setPred(yyr);

    glr_stack_item* nextFreeItem = yysplitPoint->asItem() + 1;
    yysplitPoint = YY_NULLPTR;
    yytops.clearLastDeleted();

    while (yyr != YY_NULLPTR)
      {
        glr_state& nextFreeState = nextFreeItem->getState();
        nextFreeState.copyFrom(*yyr);
        yyr = yyr->pred();
        nextFreeState.setPred(&(nextFreeItem - 1)->getState());
        setFirstTop(&nextFreeState);
        ++nextFreeItem;
      }
    yyitems.resize(static_cast<size_t>(nextFreeItem - yyitems.data()));
  }

  bool isSplit() const {
    return yysplitPoint != YY_NULLPTR;
  }

  // Present the interface of a vector of glr_stack_item.
  std::vector<glr_stack_item>::const_iterator begin() const {
    return yyitems.begin();
  }

  std::vector<glr_stack_item>::const_iterator end() const {
    return yyitems.end();
  }

  size_t size() const {
    return yyitems.size();
  }

  glr_stack_item& operator[](size_t i) {
    return yyitems[i];
  }

  glr_stack_item& stackItemAt(size_t index) {
    return yyitems[index];
  }

  size_t numTops() const {
    return yytops.size();
  }

  glr_state* firstTop() {
    return yytops[create_state_set_index(0)];
  }

  glr_state* topAt(state_set_index i) {
    return yytops[i];
  }

  void setFirstTop(glr_state* value) {
    yytops[create_state_set_index(0)] = value;
  }

  void setTopAt(state_set_index i, glr_state* value) {
    yytops[i] = value;
  }

  void pop_back() {
    yyitems.pop_back();
  }

  void pop_back(size_t n) {
    yyitems.resize(yyitems.size() - n);
  }

  state_set_index
  yysplitStack (state_set_index yyk)
  {
    if (!isSplit())
      {
        YYASSERT (yyk.get() == 0);
        yysplitPoint = topAt(yyk);
      }
    return yytops.yysplitStack(yyk);
  }

  /** Assuming that YYS is a GLRState somewhere on *this, update the
   *  splitpoint of *this, if needed, so that it is at least as deep as
   *  YYS.  */
  inline void
  yyupdateSplit (glr_state& yys)
  {
    if (isSplit() && &yys < yysplitPoint)
      yysplitPoint = &yys;
  }

  /** Return a fresh GLRState.
   * Callers should call yyreserveStack afterwards to make sure there is
   * sufficient headroom.  */
  glr_state& yynewGLRState(glr_state newState) {
    glr_state& state = yyitems[yynewGLRStackItem(true)].getState();
#if 201103L <= YY_CPLUSPLUS
    state = std::move(newState);
#else
    state = newState;
#endif
    return state;
  }

  /** Return a fresh SemanticOption.
   * Callers should call yyreserveStack afterwards to make sure there is
   * sufficient headroom.  */
  semantic_option& yynewSemanticOption(semantic_option newOption) {
    semantic_option& option = yyitems[yynewGLRStackItem(false)].getOption();
#if 201103L <= YY_CPLUSPLUS
    option = std::move(newOption);
#else
    option = newOption;
#endif
    return option;
  }

  /* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
   * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
   * For convenience, always return YYLOW1.  */
  inline int
  yyfill (glr_stack_item *yyvsp, int *yylow, int yylow1, bool yynormal)
  {
    if (!yynormal && yylow1 < *yylow)
      {
        yyfillin (yyvsp, *yylow, yylow1);
        *yylow = yylow1;
      }
    return yylow1;
  }

  /** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
   *  at YYVSP[YYLOW0].getState().pred().  Leaves YYVSP[YYLOW1].getState().pred()
   *  containing the pointer to the next state in the chain.  */
  void
  yyfillin (glr_stack_item *yyvsp, int yylow0, int yylow1)
  {
    glr_state* s = yyvsp[yylow0].getState().pred();
    YYASSERT(s != YY_NULLPTR);
    for (int i = yylow0-1; i >= yylow1; i -= 1, s = s->pred())
      {
        glr_state& yys = yyvsp[i].getState();
#if ]b4_api_PREFIX[DEBUG
        yys.yylrState = s->yylrState;
#endif
        yys.yyresolved = s->yyresolved;
        if (s->yyresolved)
          yys.semanticVal() = s->semanticVal();
        else
          /* The effect of using semanticVal or yyloc (in an immediate rule) is
           * undefined.  */
          yys.setFirstVal(YY_NULLPTR);]b4_locations_if([[
        yys.yyloc = s->yyloc;]])[
        yys.setPred(s->pred());
      }
  }

#if !]b4_api_PREFIX[DEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)          \
  do {                                  \
    if (yydebug)                        \
      yystateStack.yy_reduce_print Args;             \
  } while (0)

  /*----------------------------------------------------------------------.
  | Report that stack #YYK of *YYSTACKP is going to be reduced by YYRULE. |
  `----------------------------------------------------------------------*/

  inline void
  yy_reduce_print (bool yynormal, glr_stack_item* yyvsp, state_set_index yyk,
                   rule_num yyrule, ]b4_namespace_ref[::]b4_parser_class[& yyparser)
  {
    int yynrhs = yyrhsLength (yyrule);]b4_locations_if([
    int yylow = 1;])[
    int yyi;
    std::cerr << "Reducing stack " << yyk.get() << " by rule " << yyrule - 1 << " (line " << yyrline[yyrule] << "):\n";
    if (! yynormal)
      yyfillin (yyvsp, 1, -yynrhs);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++)
      {
        std::cerr << "   $" << yyi + 1 << " = ";
        yyparser.yy_symbol_print_
          (static_cast<yysymbol_kind_t>(yystos[yyvsp[yyi - yynrhs + 1].getState().yylrState]),
           &yyvsp[yyi - yynrhs + 1].getState().semanticVal()]b4_locations_if([[,
           &]b4_rhs_location(yynrhs, yyi + 1)])[);
        if (!yyvsp[yyi - yynrhs + 1].getState().yyresolved)
          std::cerr <<  " (unresolved)";
        std::cerr <<  '\n';
      }
  }


#define YYINDEX(YYX)                                                         \
    ((YYX) == YY_NULLPTR ? -1 : (YYX)->indexIn(yyitems.data()))

  void
  dumpStack ()
  {
    for (size_t yyi = 0; yyi < size(); ++yyi)
      {
        glr_stack_item& item = yyitems[yyi];
        std::cerr << std::setw(3) << yyi << ". ";
        if (item.is_state())
          {
            std::cerr << "Res: " << item.getState().yyresolved
                      << ", LR State: " << item.getState().yylrState
                      << ", posn: " << item.getState().yyposn
                      << ", pred: " << YYINDEX(item.getState().pred());
            if (! item.getState().yyresolved)
              std::cerr << ", firstVal: "
                        << YYINDEX(item.getState().firstVal());
          }
        else
          {
            std::cerr << "Option. rule: " << item.getOption().yyrule - 1
                      << ", state: " << YYINDEX(item.getOption().state())
                      << ", next: " << YYINDEX(item.getOption().next());
          }
        std::cerr << '\n';
      }
    std::cerr << "Tops:";
    for (state_set_index yyi = create_state_set_index(0); yyi.uget() < numTops(); ++yyi) {
      std::cerr << yyi.get() << ": " << YYINDEX(topAt(yyi)) << "; ";
    }
    std::cerr << '\n';
  }

#undef YYINDEX
#endif

  YYRESULTTAG
  yyreportAmbiguity (semantic_option* yyx0,
                     semantic_option* yyx1, ]b4_namespace_ref[::]b4_parser_class[& yyparser]b4_locations_if([, YYLTYPE *yylocp])[)
  {
    YYUSE (yyx0);
    YYUSE (yyx1);

#if ]b4_api_PREFIX[DEBUG
    std::cerr << "Ambiguity detected.\n";
    std::cerr << "Option 1,\n";
    yyreportTree (yyx0, 2);
    std::cerr << "\nOption 2,\n";
    yyreportTree (yyx1, 2);
    std::cerr << "\n";
#endif

    yyparser.error (]b4_locations_if([*yylocp, ])[YY_("syntax is ambiguous"));
    return yyabort;
  }

  /* DEBUGGING ONLY */
#if ]b4_api_PREFIX[DEBUG
  void
  yypstates (const glr_state* yyst)
  {
    if (yyst != YY_NULLPTR)
      yyst->yy_yypstack();
    else
      std::cerr << "<null>";
    std::cerr << "\n";
  }
#endif

 private:
  size_t spaceLeft() const {
    return yyitems.capacity() - yyitems.size();
  }

  /** Return a fresh GLRStackItem in this.  The item is an LR state
   *  if YYIS_STATE, and otherwise a semantic option.  Callers should call
   *  yyreserveStack afterwards to make sure there is sufficient
   *  headroom.  */
  inline size_t
  yynewGLRStackItem (bool yyis_state)
  {
    YYDASSERT(yyitems.size() < yyitems.capacity());
    yyitems.push_back(glr_stack_item(yyis_state));
    return yyitems.size() - 1;
  }


#if ]b4_api_PREFIX[DEBUG
  void
  yyreportTree (semantic_option* yyx, size_t yyindent)
  {
    int yynrhs = yyrhsLength (yyx->yyrule);
    glr_state* yystates[1 + YYMAXRHS];
    glr_state yyleftmost_state;

    {
      glr_state* yys = yyx->state();
      for (int yyi = yynrhs; 0 < yyi; yyi -= 1)
        {
          yystates[yyi] = yys;
          yys = yys->pred();
        }
      if (yys == YY_NULLPTR)
        {
          yyleftmost_state.yyposn = 0;
          yystates[0] = &yyleftmost_state;
        }
      else
        yystates[0] = yys;
    }

    std::string yylhs = ]b4_namespace_ref::b4_parser_class[::symbol_name (yylhsNonterm (yyx->yyrule));
    YYASSERT(yyx->state());
    if (yyx->state()->yyposn < yystates[0]->yyposn + 1)
      std::cerr << std::string(yyindent, ' ') << yylhs << " -> <Rule "
                << yyx->yyrule - 1 << ", empty>\n";
    else
      std::cerr << std::string(yyindent, ' ') << yylhs << " -> <Rule "
                << yyx->yyrule - 1 << ", tokens "
                << yystates[0]->yyposn + 1 << " .. "
                << yyx->state()->yyposn << ">\n";
    for (int yyi = 1; yyi <= yynrhs; yyi += 1)
      {
        if (yystates[yyi]->yyresolved)
          {
            std::string yysym = ]b4_namespace_ref::b4_parser_class[::symbol_name (static_cast<yysymbol_kind_t>(yystos[yystates[yyi]->yylrState]));
            if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
              std::cerr << std::string(yyindent + 2, ' ') << yysym
                        << " <empty>\n";
            else
              std::cerr << std::string(yyindent + 2, ' ') << yysym
                        << " <tokens " << yystates[yyi-1]->yyposn + 1
                        << " .. " << yystates[yyi]->yyposn << ">\n";
          }
        else
          yyreportTree (yystates[yyi]->firstVal(),
                        yyindent+2);
      }
  }
#endif

 public:

  std::vector<glr_stack_item> yyitems;
  glr_state* yysplitPoint;
  glr_state_set yytops;
};

#undef YYFILL
#define YYFILL(N) yystateStack.yyfill (yyvsp, &yylow, (N), yynormal)

#define yystackp this
class glr_stack
{
public:

  glr_stack (size_t yysize, ]b4_namespace_ref[::]b4_parser_class[& yyparser_yyarg]m4_ifset([b4_parse_param], [, b4_parse_param_decl])[)
    : yyerrState (0)
    , yystateStack (yysize)
    , yyerrcnt (0)
    , yyrawchar (0)
    , yyparser (yyparser_yyarg)]m4_ifset([b4_parse_param], [,b4_parse_param_cons])[
  {}

  ~glr_stack ()
  {
    if (yychar != ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[)
      yyparser.yy_destroy_ ("Cleanup: discarding lookahead",
                  YYTRANSLATE (yychar), &yylval]b4_locations_if([, &yylloc])[);
    popall();
  }

  int yyerrState;
]b4_locations_if([[  /* To compute the location of the error token.  */
  glr_stack_item yyerror_range[3];]])[
]b4_pure_if(
[
  state_stack yystateStack;
  int yyerrcnt;
  int yyrawchar;
  YYSTYPE yyval;]b4_locations_if([[
  YYLTYPE yyloc;]])[
])[
  YYJMP_BUF yyexception_buffer;
  ]b4_namespace_ref[::]b4_parser_class[& yyparser;

  void yyreserveGlrStack() {
    if (yystateStack.yyexpandGLRStackIfNeeded ())
      yyMemoryExhausted();
  }

  _Noreturn void
  yyMemoryExhausted ()
  {
    YYLONGJMP (yyexception_buffer, 2);
  }

  _Noreturn void
  yyFail (]b4_locations_if([YYLTYPE* yylocp, ])[const char* yymsg)
  {
    if (yymsg != YY_NULLPTR)
      yyparser.error (]b4_locations_if([*yylocp, ])[yymsg);
    YYLONGJMP (yyexception_buffer, 1);
  }

                                /* GLRStates */


  /** Add a new semantic action that will execute the action for rule
   *  YYRULE on the semantic values in YYRHS to the list of
   *  alternative actions for YYSTATE.  Assumes that YYRHS comes from
   *  stack #YYK of *this. */
  void
  yyaddDeferredAction (state_set_index yyk, glr_state* yystate,
                       glr_state* yyrhs, rule_num yyrule)
  {
    semantic_option& yynewOption =
      yystateStack.yynewSemanticOption(semantic_option(yyrule, ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[));
    yynewOption.setState(yyrhs);
    yynewOption.setNext(yystate->firstVal());
    if (yystateStack.yytops.lookaheadNeeds(yyk))
      {
        yynewOption.yyrawchar = yychar;
        yynewOption.yyval = yylval;]b4_locations_if([
        yynewOption.yyloc = yylloc;])[
      }
    yystate->setFirstVal(&yynewOption);

    yyreserveGlrStack();
  }

#if ]b4_api_PREFIX[DEBUG
  void yypdumpstack () {
    yystateStack.dumpStack();
  }
#endif
  void
  yyreportSyntaxError ()
  {
    if (yyerrState != 0)
      return;
]b4_parse_error_bmatch(
[simple],
[[    yyparser.error (]b4_locations_if([yylloc, ])[YY_("syntax error"));]],
[[    {
    yysymbol_kind_t yytoken
      = yychar == ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[
      ? ]b4_namespace_ref::b4_parser_class::b4_symbol(empty, kind)[
      : YYTRANSLATE (yychar);
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    /* Arguments of yyformat. */
    yysymbol_kind_t yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM]
      = { ]b4_namespace_ref::b4_parser_class::b4_symbol(empty, kind)[ };
    /* Number of reported tokens (one for the "unexpected", one per
       "expected").  */
    int yycount = 0;

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yychar) is if
         this state is a consistent state with a default action.  Thus,
         detecting the absence of a lookahead is sufficient to determine
         that there is no unexpected or expected token to report.  In that
         case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is a
         consistent state with a default action.  There might have been a
         previous inconsistent state, consistent state with a non-default
         action, or user semantic action that manipulated yychar.
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (yytoken != ]b4_namespace_ref::b4_parser_class::b4_symbol(empty, kind)[)
      {
        int yyn = yypact[firstTopState()->yylrState];
        yyarg[yycount++] = yytoken;
        if (!yypact_value_is_default (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for this
               state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = YYLAST - yyn + 1;
            int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
            int yyx;
            for (yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck[yyx + yyn] == yyx && yyx != ]b4_namespace_ref::b4_parser_class::b4_symbol(1, kind)[
                  && !yytable_value_is_error (yytable[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  yyarg[yycount++] = static_cast<yysymbol_kind_t>(yyx);
                }
          }
      }

    /* Internationalized format string. */
    const char *yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                   \
        case N:                           \
          yyformat = S;                   \
        break
      default: /* Avoid compiler warnings. */
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yymsg;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yymsg += ]b4_namespace_ref::b4_parser_class[::symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yymsg += *yyp;
     yyparser.error (]b4_locations_if([[yylloc, ]])[yymsg);
    }
]])[
    yynerrs += 1;
  }


  /* Recover from a syntax error on this, assuming that YYTOKENP,
     yylval, and yylloc are the syntactic category, semantic value, and location
     of the lookahead.  */
  void
  yyrecoverSyntaxError (]b4_locations_if([YYLTYPE* yylocp])[)
  {
    if (yyerrState == 3)
      /* We just shifted the error token and (perhaps) took some
         reductions.  Skip tokens until we can proceed.  */
      while (true)
        {
          yysymbol_kind_t yytoken;
          int yyj;
          if (yychar == ]b4_namespace_ref::b4_parser_class::token::b4_symbol(0, id)[)
            yyFail (]b4_locations_if([yylocp, ])[YY_NULLPTR);
          if (yychar != ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[)
            {]b4_locations_if([[
              /* We throw away the lookahead, but the error range
                 of the shifted error token must take it into account.  */
              glr_state *yys = firstTopState();
              yyerror_range[1].getState().yyloc = yys->yyloc;
              yyerror_range[2].getState().yyloc = yylloc;
              YYLLOC_DEFAULT ((yys->yyloc), yyerror_range, 2);]])[
              yytoken = YYTRANSLATE (yychar);
              yyparser.yy_destroy_ ("Error: discarding",
                          yytoken, &yylval]b4_locations_if([, &yylloc])[);
              yychar = ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[;
            }
          yytoken = ]b4_yygetToken_call[;
          yyj = yypact[firstTopState()->yylrState];
          if (yypact_value_is_default (yyj))
            return;
          yyj += yytoken;
          if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
            {
              if (yydefact[firstTopState()->yylrState] != 0)
                return;
            }
          else if (! yytable_value_is_error (yytable[yyj]))
            return;
        }

    if (!yystateStack.reduceToOneStack())
      yyFail (]b4_locations_if([yylocp, ])[YY_NULLPTR);

    /* Now pop stack until we find a state that shifts the error token.  */
    yyerrState = 3;
    while (firstTopState() != YY_NULLPTR)
      {
        glr_state *yys = firstTopState();
        int yyj = yypact[yys->yylrState];
        if (! yypact_value_is_default (yyj))
          {
            yyj += YYTERROR;
            if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
                && yyisShiftAction (yytable[yyj]))
              {
                /* Shift the error token.  */]b4_locations_if([[
                /* First adjust its location.*/
                YYLTYPE yyerrloc;
                yyerror_range[2].getState().yyloc = yylloc;
                YYLLOC_DEFAULT (yyerrloc, (yyerror_range), 2);]])[
                YY_SYMBOL_PRINT ("Shifting", static_cast<yysymbol_kind_t>(yystos[yytable[yyj]]),
                                 &yylval, &yyerrloc);
                yyglrShift (create_state_set_index(0), yytable[yyj],
                            yys->yyposn, &yylval]b4_locations_if([, &yyerrloc])[);
                yys = firstTopState();
                break;
              }
          }]b4_locations_if([[
        yyerror_range[1].getState().yyloc = yys->yyloc;]])[
        if (yys->pred() != YY_NULLPTR)
          yys->destroy ("Error: popping", yyparser]b4_user_args[);
        yystateStack.setFirstTop(yys->pred());
        yystateStack.pop_back();
      }
    if (firstTopState() == YY_NULLPTR)
      yyFail (]b4_locations_if([yylocp, ])[YY_NULLPTR);
  }

  YYRESULTTAG
  yyprocessOneStack (state_set_index yyk,
                     size_t yyposn]b4_locations_if([, YYLTYPE* yylocp])[)
  {
    while (yystateStack.topAt(yyk) != YY_NULLPTR)
      {
        state_num yystate = topState(yyk)->yylrState;
        YY_DEBUG_STREAM << "Stack " << yyk.get()
                        << " Entering state " << yystate << '\n';

        YYASSERT (yystate != YYFINAL);

        if (yyisDefaultedState (yystate))
          {
            rule_num yyrule = yydefaultAction (yystate);
            if (yyrule == 0)
              {
                YY_DEBUG_STREAM << "Stack " << yyk.get() << " dies.\n";
                yystateStack.yytops.yymarkStackDeleted (yyk);
                return yyok;
              }
            YYRESULTTAG yyflag = yyglrReduce (yyk, yyrule,
                                              yyimmediate[yyrule]);
            if (yyflag == yyerr)
              {
                YY_DEBUG_STREAM << "Stack " << yyk.get() << " dies "
                                "(predicate failure or explicit user error).\n";
                yystateStack.yytops.yymarkStackDeleted (yyk);
                return yyok;
              }
            if (yyflag != yyok)
              return yyflag;
          }
        else
          {
            yystateStack.yytops.setLookaheadNeeds(yyk, true);
            yysymbol_kind_t yytoken = ]b4_yygetToken_call[;
            const short* yyconflicts;
            int yyaction = yygetLRActions (yystate, yytoken, &yyconflicts);

            for (; *yyconflicts != 0; ++yyconflicts)
              {
                state_set_index yynewStack = yystateStack.yysplitStack (yyk);
                YY_DEBUG_STREAM << "Splitting off stack " << yynewStack.get()
                                << " from " << yyk.get() << '\n';
                YYRESULTTAG yyflag =
                  yyglrReduce (yynewStack, *yyconflicts,
                               yyimmediate[*yyconflicts]);
                if (yyflag == yyok)
                  YYCHK (yyprocessOneStack (yynewStack,
                                            yyposn]b4_locations_if([, yylocp])[));
                else if (yyflag == yyerr)
                  {
                    YY_DEBUG_STREAM << "Stack " << yynewStack.get() << " dies.\n";
                    yystateStack.yytops.yymarkStackDeleted (yynewStack);
                  }
                else
                  return yyflag;
              }

            if (yyisShiftAction (yyaction))
              break;
            else if (yyisErrorAction (yyaction))
              {
                YY_DEBUG_STREAM << "Stack " << yyk.get() << " dies\n";
                yystateStack.yytops.yymarkStackDeleted (yyk);
                break;
              }
            else
              {
                YYRESULTTAG yyflag = yyglrReduce (yyk, -yyaction,
                                                  yyimmediate[-yyaction]);
                if (yyflag == yyerr)
                  {
                    YY_DEBUG_STREAM << "Stack " << yyk.get() << " dies "
                                    "(predicate failure or explicit user error).\n";
                    yystateStack.yytops.yymarkStackDeleted (yyk);
                    break;
                  }
                else if (yyflag != yyok)
                  return yyflag;
              }
          }
      }
    return yyok;
  }

  /** Perform user action for rule number YYN, with RHS length YYRHSLEN,
   *  and top stack item YYVSP.  YYVALP points to place to put semantic
   *  value ($$), and yylocp points to place for location information
   *  (@@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
   *  yyerr for YYERROR, yyabort for YYABORT.  */
  YYRESULTTAG
  yyuserAction (rule_num yyn, int yyrhslen, glr_stack_item* yyvsp,
                YYSTYPE* yyvalp]b4_locations_if([, YYLTYPE* yylocp])[)
  {
    bool yynormal YY_ATTRIBUTE_UNUSED = !yystateStack.isSplit();
    int yylow;
  ]b4_parse_param_use([yyvalp], [yylocp])dnl
  [  YYUSE (yyrhslen);
  # undef yyerrok
  # define yyerrok (yyerrState = 0)
  # undef YYACCEPT
  # define YYACCEPT return yyaccept
  # undef YYABORT
  # define YYABORT return yyabort
  # undef YYERROR
  # define YYERROR return yyerrok, yyerr
  # undef YYRECOVERING
  # define YYRECOVERING() (yyerrState != 0)
  # undef yyclearin
  # define yyclearin (yychar = ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[)
  # undef YYBACKUP
  # define YYBACKUP(Token, Value)                                              \
    return yyparser.error (]b4_locations_if([*yylocp, ])[YY_("syntax error: cannot back up")),     \
           yyerrok, yyerr

    yylow = 1;
    if (yyrhslen == 0)
      *yyvalp = yyval_default;
    else
      *yyvalp = yyvsp[YYFILL (1-yyrhslen)].getState().semanticVal();]b4_locations_if([[
    /* Default location. */
    YYLLOC_DEFAULT ((*yylocp), (yyvsp - yyrhslen), yyrhslen);
    yyerror_range[1].getState().yyloc = *yylocp;
]])[
  #if YY_EXCEPTIONS
    typedef ]b4_namespace_ref[::]b4_parser_class[::syntax_error syntax_error;
    try
    {
  #endif // YY_EXCEPTIONS
    switch (yyn)
      {
  ]b4_user_actions[
        default: break;
      }
  #if YY_EXCEPTIONS
    }
    catch (const syntax_error& yyexc)
      {
        YY_DEBUG_STREAM  << "Caught exception: " << yyexc.what() << '\n';]b4_locations_if([
        *yylocp = yyexc.location;])[
        yyparser.error (]b4_locations_if([*yylocp, ])[yyexc.what ());
        YYERROR;
      }
  #endif // YY_EXCEPTIONS

    return yyok;
  # undef yyerrok
  # undef YYABORT
  # undef YYACCEPT
  # undef YYERROR
  # undef YYBACKUP
  # undef yyclearin
  # undef YYRECOVERING
  }

  YYRESULTTAG
  yyresolveStack ()
  {
    if (yystateStack.isSplit())
      {
        int yyn = 0;
        for (glr_state* yys = firstTopState();
             yys != yystateStack.yysplitPoint;
             yys = yys->pred())
          yyn += 1;
        YYCHK (yyresolveStates (firstTopState(), yyn));
      }
    return yyok;
  }

  /** Pop the symbols consumed by reduction #YYRULE from the top of stack
   *  #YYK of *YYSTACKP, and perform the appropriate semantic action on their
   *  semantic values.  Assumes that all ambiguities in semantic values
   *  have been previously resolved.  Set *YYVALP to the resulting value,
   *  and *YYLOCP to the computed location (if any).  Return value is as
   *  for userAction.  */
  inline YYRESULTTAG
  yydoAction (state_set_index yyk, rule_num yyrule,
              YYSTYPE* yyvalp]b4_locations_if([, YYLTYPE* yylocp])[)
  {
    int yynrhs = yyrhsLength (yyrule);

    if (!yystateStack.isSplit())
      {
        /* Standard special case: single stack.  */
        YYASSERT (yyk.get() == 0);
        glr_stack_item* yyrhs = yystateStack.firstTop()->asItem();
        YY_REDUCE_PRINT ((true, yyrhs, yyk, yyrule, yyparser));
        YYRESULTTAG res =  yyuserAction (yyrule, yynrhs, yyrhs,
                             yyvalp]b4_locations_if([, yylocp])[);
        yystateStack.pop_back(static_cast<size_t>(yynrhs));
        yystateStack.setFirstTop(&yystateStack[yystateStack.size() - 1].getState());
        return res;
      }
    else
      {
        glr_stack_item yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
        glr_state* yys = yystateStack.topAt(yyk);
        yyrhsVals[YYMAXRHS + YYMAXLEFT].getState().setPred(yys);]b4_locations_if([[
        if (yynrhs == 0)
          /* Set default location.  */
          yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].getState().yyloc = yys->yyloc;]])[
        for (int yyi = 0; yyi < yynrhs; yyi += 1)
          {
            yys = yys->pred();
            YYASSERT (yys != YY_NULLPTR);
          }
        yystateStack.yyupdateSplit (*yys);
        yystateStack.setTopAt(yyk, yys);
        YY_REDUCE_PRINT ((false, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1, yyk, yyrule, yyparser));
        return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                             yyvalp]b4_locations_if([, yylocp])[);
      }
  }

  /** Pop items off stack #YYK of *YYSTACKP according to grammar rule YYRULE,
   *  and push back on the resulting nonterminal symbol.  Perform the
   *  semantic action associated with YYRULE and store its value with the
   *  newly pushed state, if YYFORCEEVAL or if *YYSTACKP is currently
   *  unambiguous.  Otherwise, store the deferred semantic action with
   *  the new state.  If the new state would have an identical input
   *  position, LR state, and predecessor to an existing state on the stack,
   *  it is identified with that existing state, eliminating stack #YYK from
   *  *YYSTACKP.  In this case, the semantic value is
   *  added to the options for the existing state's semantic value.
   */
  inline YYRESULTTAG
  yyglrReduce (state_set_index yyk, rule_num yyrule,
               bool yyforceEval)
  {
    size_t yyposn = topState(yyk)->yyposn;

    if (yyforceEval || !yystateStack.isSplit())
      {
        YYSTYPE yysval;]b4_locations_if([[
        YYLTYPE loc;]])[

        YYRESULTTAG yyflag = yydoAction (yyk, yyrule, &yysval]b4_locations_if([, &loc])[);
        if (yyflag == yyerr && yystateStack.isSplit())
          {
            YY_DEBUG_STREAM << "Parse on stack " << yyk.get()
                            << " rejected by rule #" << yyrule - 1 << ".\n";
          }
        if (yyflag != yyok)
          return yyflag;
        YY_SYMBOL_PRINT ("-> $$ =", static_cast<yysymbol_kind_t>(yyr1[yyrule]), &yysval, &loc);
        yyglrShift (yyk,
                    yyLRgotoState (topState(yyk)->yylrState,
                                   yylhsNonterm (yyrule)),
                    yyposn, &yysval]b4_locations_if([, &loc])[);
      }
    else
      {
        glr_state *yys = yystateStack.topAt(yyk);
        glr_state *yys0 = yys;
        for (int yyn = yyrhsLength (yyrule); 0 < yyn; yyn -= 1)
          {
            yys = yys->pred();
            YYASSERT (yys != YY_NULLPTR);
          }
        yystateStack.yyupdateSplit (*yys);
        state_num yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
        YY_DEBUG_STREAM << "Reduced stack " << yyk.get()
                        << " by rule #" << yyrule - 1
                        << "; action deferred.  Now in state " << yynewLRState
                        << ".\n";
        for (state_set_index yyi = create_state_set_index(0); yyi.uget() < yystateStack.numTops(); ++yyi)
          if (yyi != yyk && yystateStack.topAt(yyi) != YY_NULLPTR)
            {
              glr_state* yysplit = yystateStack.yysplitPoint;
              glr_state* yyp = yystateStack.topAt(yyi);
              while (yyp != yys && yyp != yysplit
                     && yyp->yyposn >= yyposn)
                {
                  if (yyp->yylrState == yynewLRState
                      && yyp->pred() == yys)
                    {
                      yyaddDeferredAction (yyk, yyp, yys0, yyrule);
                      yystateStack.yytops.yymarkStackDeleted (yyk);
                      YY_DEBUG_STREAM  << "Merging stack " << yyk.get()
                                       << " into stack " << yyi.get() << ".\n";
                      return yyok;
                    }
                  yyp = yyp->pred();
                }
            }
        yystateStack.setTopAt(yyk, yys);
        yyglrShiftDefer (yyk, yynewLRState, yyposn, yys0, yyrule);
      }
    return yyok;
  }

  /** Shift stack #YYK of *YYSTACKP, to a new state corresponding to LR
   *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
   *  semantic value of YYRHS under the action for YYRULE.  */
  inline void
  yyglrShiftDefer (state_set_index yyk, state_num yylrState,
                   size_t yyposn, glr_state* yyrhs, rule_num yyrule)
  {
    glr_state& yynewState = yystateStack.yynewGLRState(
      glr_state(yylrState, yyposn));
    yynewState.setPred(yystateStack.topAt(yyk));
    yystateStack.setTopAt(yyk, &yynewState);

    /* Invokes yyreserveStack.  */
    yyaddDeferredAction (yyk, &yynewState, yyrhs, yyrule);
  }

  /** Shift to a new state on stack #YYK of *YYSTACKP, corresponding to LR
   * state YYLRSTATE, at input position YYPOSN, with (resolved) semantic
   * value *YYVALP and source location *YYLOCP.  */
  inline void
  yyglrShift (state_set_index yyk, state_num yylrState,
              size_t yyposn,
              YYSTYPE* yyvalp]b4_locations_if([, YYLTYPE* yylocp])[)
  {
    glr_state& yynewState = yystateStack.yynewGLRState(
      glr_state(yylrState, yyposn, *yyvalp
                 ]b4_locations_if([, *yylocp])[));
    yynewState.setPred(yystateStack.topAt(yyk));
    yystateStack.setTopAt(yyk, &yynewState);
    yyreserveGlrStack();
  }

#if ]b4_api_PREFIX[DEBUG
  void
  yypstack (state_set_index yyk)
  {
    yystateStack.yypstates (yystateStack.topAt(yyk));
  }
#endif

  glr_state* topState(state_set_index i) {
    return yystateStack.topAt(i);
  }

  glr_state* firstTopState() {
    return yystateStack.firstTop();
  }

 private:

  void popall() {
    /* If the stack is well-formed, pop the stack until it is empty,
       destroying its entries as we go.  But free the stack regardless
       of whether it is well-formed.  */
    for (state_set_index k = create_state_set_index(0); k.uget() < yystateStack.numTops(); k += 1)
      if (yystateStack.topAt(k) != YY_NULLPTR)
        {
          while (yystateStack.topAt(k) != YY_NULLPTR)
            {
              glr_state* state = topState(k);]b4_locations_if([[
                yyerror_range[1].getState().yyloc = state->yyloc;]])[
              if (state->pred() != YY_NULLPTR)
                state->destroy ("Cleanup: popping", yyparser]b4_user_args[);
              yystateStack.setTopAt(k, state->pred());
              yystateStack.pop_back();
            }
            break;
        }
  }

  /** Resolve the previous YYN states starting at and including state YYS
   *  on *YYSTACKP. If result != yyok, some states may have been left
   *  unresolved possibly with empty semantic option chains.  Regardless
   *  of whether result = yyok, each state has been left with consistent
   *  data so that destroy can be invoked if necessary.  */
  YYRESULTTAG
  yyresolveStates (glr_state* yys, int yyn)
  {
    if (0 < yyn)
      {
        YYASSERT (yys->pred() != YY_NULLPTR);
        YYCHK (yyresolveStates (yys->pred(), yyn-1));
        if (! yys->yyresolved)
          YYCHK (yyresolveValue (yys));
      }
    return yyok;
  }

  /** Resolve the ambiguity represented in state YYS in *YYSTACKP,
   *  perform the indicated actions, and set the semantic value of YYS.
   *  If result != yyok, the chain of semantic options in YYS has been
   *  cleared instead or it has been left unmodified except that
   *  redundant options may have been removed.  Regardless of whether
   *  result = yyok, YYS has been left with consistent data so that
   *  destroy can be invoked if necessary.  */
  YYRESULTTAG
  yyresolveValue (glr_state* yys)
  {
    semantic_option* yybest = yys->firstVal();
    YYASSERT(yybest != YY_NULLPTR);
    bool yymerge = false;
    YYSTYPE yysval;
    YYRESULTTAG yyflag;]b4_locations_if([
    YYLTYPE *yylocp = &yys->yyloc;])[

    semantic_option* yypPrev = yybest;
    for (semantic_option* yyp = yybest->next();
         yyp != YY_NULLPTR; )
      {
        if (yybest->isIdenticalTo (yyp))
          {
            yybest->mergeWith (yyp);
            yypPrev->setNext(yyp->next());
          }
        else
          {
            switch (yypreference (yybest, yyp))
              {
              case 0:]b4_locations_if([[
                yyresolveLocations (yys, 1);]])[
                return yystateStack.yyreportAmbiguity (yybest, yyp, yyparser]b4_locations_if([, yylocp])[);
                break;
              case 1:
                yymerge = true;
                break;
              case 2:
                break;
              case 3:
                yybest = yyp;
                yymerge = false;
                break;
              default:
                /* This cannot happen so it is not worth a YYASSERT (false),
                   but some compilers complain if the default case is
                   omitted.  */
                break;
              }
            yypPrev = yyp;
            yyp = yyp->next();
          }
      }

    if (yymerge)
      {
        int yyprec = yydprec[yybest->yyrule];
        yyflag = yyresolveAction (yybest, &yysval]b4_locations_if([, yylocp])[);
        if (yyflag == yyok)
          for (semantic_option* yyp = yybest->next();
               yyp != YY_NULLPTR;
               yyp = yyp->next())
            {
              if (yyprec == yydprec[yyp->yyrule])
                {
                  YYSTYPE yysval_other;]b4_locations_if([
                  YYLTYPE yydummy;])[
                  yyflag = yyresolveAction (yyp, &yysval_other]b4_locations_if([, &yydummy])[);
                  if (yyflag != yyok)
                    {
                      yyparser.yy_destroy_ ("Cleanup: discarding incompletely merged value for",
                                  static_cast<yysymbol_kind_t>(yystos[yys->yylrState]),
                                  &yysval]b4_locations_if([, yylocp])[);
                      break;
                    }
                  yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
                }
            }
      }
    else
      yyflag = yyresolveAction (yybest, &yysval]b4_locations_if([, yylocp])[);

    if (yyflag == yyok)
      {
        yys->yyresolved = true;
        YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
        yys->semanticVal() = yysval;
        YY_IGNORE_MAYBE_UNINITIALIZED_END
      }
    else
      yys->setFirstVal(YY_NULLPTR);
    return yyflag;
  }

  /** Resolve the states for the RHS of YYOPT on *YYSTACKP, perform its
   *  user action, and return the semantic value and location in *YYVALP
   *  and *YYLOCP.  Regardless of whether result = yyok, all RHS states
   *  have been destroyed (assuming the user action destroys all RHS
   *  semantic values if invoked).  */
  YYRESULTTAG
  yyresolveAction (semantic_option* yyopt, YYSTYPE* yyvalp]b4_locations_if([, YYLTYPE* yylocp])[)
  {
    glr_state* yyoptState = yyopt->state();
    glr_stack_item yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
    int yynrhs = yyrhsLength (yyopt->yyrule);
    YYRESULTTAG yyflag =
      yyresolveStates (yyoptState, yynrhs);
    if (yyflag != yyok)
      {
        for (glr_state *yys = yyoptState; yynrhs > 0; yys = yys->pred(), yynrhs -= 1)
          yys->destroy ("Cleanup: popping", yyparser]b4_user_args[);
        return yyflag;
      }

    yyrhsVals[YYMAXRHS + YYMAXLEFT].getState().setPred(yyopt->state());]b4_locations_if([[
    if (yynrhs == 0)
      /* Set default location.  */
      yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].getState().yyloc = yyoptState->yyloc;]])[
    {
      int yychar_current = yychar;
      YYSTYPE yylval_current = yylval;]b4_locations_if([
      YYLTYPE yylloc_current = yylloc;])[
      yychar = yyopt->yyrawchar;
      yylval = yyopt->yyval;]b4_locations_if([
      yylloc = yyopt->yyloc;])[
      yyflag = yyuserAction (yyopt->yyrule, yynrhs,
                             yyrhsVals + YYMAXRHS + YYMAXLEFT - 1, yyvalp]b4_locations_if([, yylocp])[);
      yychar = yychar_current;
      yylval = yylval_current;]b4_locations_if([
      yylloc = yylloc_current;])[
    }
    return yyflag;
  }]b4_locations_if([[

  /** Resolve the locations for each of the YYN1 states in *YYSTACKP,
   *  ending at YYS1.  Has no effect on previously resolved states.
   *  The first semantic option of a state is always chosen.  */
  void
  yyresolveLocations (glr_state *yys1, int yyn1)
  {
    if (0 < yyn1)
      {
        yyresolveLocations (yys1->pred(), yyn1 - 1);
        if (!yys1->yyresolved)
          {
            glr_stack_item yyrhsloc[1 + YYMAXRHS];
            int yynrhs;
            semantic_option* yyoption = yys1->firstVal();
            YYASSERT (yyoption != YY_NULLPTR);
            yynrhs = yyrhsLength (yyoption->yyrule);
            if (0 < yynrhs)
              {
                yyresolveLocations (yyoption->state(), yynrhs);
                glr_state *yys = yyoption->state();
                for (int yyn = yynrhs; yyn > 0; yyn -= 1)
                {
                  yyrhsloc[yyn].getState().yyloc = yys->yyloc;
                  yys = yys->pred();
                }
              }
            else
              {
                /* Both yyresolveAction and yyresolveLocations traverse the GSS
                   in reverse rightmost order.  It is only necessary to invoke
                   yyresolveLocations on a subforest for which yyresolveAction
                   would have been invoked next had an ambiguity not been
                   detected.  Thus the location of the previous state (but not
                   necessarily the previous state itself) is guaranteed to be
                   resolved already.  */
                YY_IGNORE_NULL_DEREFERENCE_BEGIN
                yyrhsloc[0].getState().yyloc = yyoption->state()->yyloc;
                YY_IGNORE_NULL_DEREFERENCE_END
              }
            YYLLOC_DEFAULT ((yys1->yyloc), yyrhsloc, yynrhs);
          }
      }
  }]])[

]b4_parse_param_vars[

};
#undef yystackp
#undef YYSTACKEXPANDABLE


/** If yychar is empty, fetch the next token.  */
static inline yysymbol_kind_t
yygetToken (int *yycharp, ]b4_namespace_ref[::]b4_parser_class[& yyparser][]b4_pure_if([, glr_stack* yystackp])[]b4_user_formals[)
{
  yysymbol_kind_t yytoken;
]b4_parse_param_use()dnl
[  if (*yycharp == ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[)
    {
      YY_DEBUG_STREAM <<  "Reading a token\n";
#if YY_EXCEPTIONS
      try
        {
#endif // YY_EXCEPTIONS
          *yycharp = ]b4_lex[;
#if YY_EXCEPTIONS
        }
      catch (const ]b4_namespace_ref[::]b4_parser_class[::syntax_error& yyexc)
        {
          YY_DEBUG_STREAM <<  "Caught exception: " << yyexc.what() << "\n";]b4_locations_if([
          yylloc = yyexc.location;])[
          yyparser.error (]b4_locations_if([yylloc, ])[yyexc.what ());
          // Map errors caught in the scanner to the error token, so that error
          // handling is started.
          *yycharp = ]b4_namespace_ref::b4_parser_class::token::b4_symbol(1, id)[;
        }
#endif // YY_EXCEPTIONS
    }
  if (*yycharp <= ]b4_namespace_ref::b4_parser_class::token::b4_symbol(0, id)[)
    {
      *yycharp = ]b4_namespace_ref::b4_parser_class::token::b4_symbol(0, id)[;
      yytoken = ]b4_namespace_ref::b4_parser_class::b4_symbol(0, kind)[;
      YY_DEBUG_STREAM << "Now at end of input.\n";
    }
  else
    {
      yytoken = YYTRANSLATE (*yycharp);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }
  return yytoken;
}



static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {
]b4_mergers[
      default: break;
    }
}

                              /* Bison grammar-table manipulation.  */

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (rule_num yyrule)
{
  return yyr2[yyrule];
}

/** The action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *YYCONFLICTS to a pointer into yyconfl to a 0-terminated list
 *  of conflicting reductions.
 */
static inline int
yygetLRActions (state_num yystate, yysymbol_kind_t yytoken, const short** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yytoken == ]b4_namespace_ref::b4_parser_class[::]b4_symbol(1, kind)[)
    {
      // This is the error token.
      *yyconflicts = yyconfl;
      return 0;
    }
  else if (yyisDefaultedState (yystate)
           || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyconflicts = yyconfl;
      return -yydefact[yystate];
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
    {
      *yyconflicts = yyconfl + yyconflp[yyindex];
      return yytable[yyindex];
    }
  else
    {
      *yyconflicts = yyconfl + yyconflp[yyindex];
      return 0;
    }
}

/** Compute post-reduction state.
 * \param yystate   the current state
 * \param yysym     the nonterminal to push on the stack
 */
static inline state_num
yyLRgotoState (state_num yystate, yysymbol_kind_t yysym)
{
  int yyr = yypgoto[yysym - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yysym - YYNTOKENS];
}

                                /* GLRStacks */

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (semantic_option* y0, semantic_option* y1)
{
  rule_num r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
        return 0;
      else
        return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (0)


/*----------.
| yyparse.  |
`----------*/

int
yyparse (]b4_namespace_ref[::]b4_parser_class[& yyparser]b4_user_formals[)
{
  int yyresult;
  glr_stack yystack(YYINITDEPTH, yyparser]b4_user_args[);
  glr_stack* const yystackp = &yystack;
  size_t yyposn;

  YY_DEBUG_STREAM << "Starting parse\n";

  yychar = ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[;
  yylval = yyval_default;]b4_locations_if([
  yylloc = yyloc_default;])[
]m4_ifdef([b4_initial_action], [
b4_dollar_pushdef([yylval], [], [], [yylloc])dnl
  b4_user_initial_action
b4_dollar_popdef])[]dnl
[
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yystack.yyglrShift (create_state_set_index(0), 0, 0, &yylval]b4_locations_if([, &yylloc])[);
  yyposn = 0;

  while (true)
    {
      /* For efficiency, we have two loops, the first of which is
         specialized to deterministic operation (single stack, no
         potential ambiguity).  */
      /* Standard mode */
      while (true)
        {
          state_num yystate = yystack.firstTopState()->yylrState;
          YY_DEBUG_STREAM << "Entering state " << yystate << "\n";
          if (yystate == YYFINAL)
            goto yyacceptlab;
          if (yyisDefaultedState (yystate))
            {
              rule_num yyrule = yydefaultAction (yystate);
              if (yyrule == 0)
                {]b4_locations_if([[
                  yystack.yyerror_range[1].getState().yyloc = yylloc;]])[
                  yystack.yyreportSyntaxError ();
                  goto yyuser_error;
                }
              YYCHK1 (yystack.yyglrReduce (create_state_set_index(0), yyrule, true));
            }
          else
            {
              yysymbol_kind_t yytoken = ]b4_yygetToken_call;[
              const short* yyconflicts;
              int yyaction = yygetLRActions (yystate, yytoken, &yyconflicts);
              if (*yyconflicts != 0)
                break;
              if (yyisShiftAction (yyaction))
                {
                  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
                  yychar = ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[;
                  yyposn += 1;
                  yystack.yyglrShift (create_state_set_index(0), yyaction, yyposn, &yylval]b4_locations_if([, &yylloc])[);
                  if (0 < yystack.yyerrState)
                    yystack.yyerrState -= 1;
                }
              else if (yyisErrorAction (yyaction))
                {]b4_locations_if([[
                  yystack.yyerror_range[1].getState().yyloc = yylloc;]])[
                  /* Don't issue an error message again for exceptions
                     thrown from the scanner.  */
                  if (yychar != ]b4_namespace_ref::b4_parser_class::token::b4_symbol(1, id)[)
                    yystack.yyreportSyntaxError ();
                  goto yyuser_error;
                }
              else
                YYCHK1 (yystack.yyglrReduce (create_state_set_index(0), -yyaction, true));
            }
        }

      while (true)
        {
          yysymbol_kind_t yytoken_to_shift;

          for (state_set_index yys = create_state_set_index(0); yys.uget() < yystack.yystateStack.numTops(); ++yys)
            yystackp->yystateStack.yytops.setLookaheadNeeds(yys, yychar != ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[);

          /* yyprocessOneStack returns one of three things:

              - An error flag.  If the caller is yyprocessOneStack, it
                immediately returns as well.  When the caller is finally
                yyparse, it jumps to an error label via YYCHK1.

              - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
                (yys), which sets the top state of yys to NULL.  Thus,
                yyparse's following invocation of yyremoveDeletes will remove
                the stack.

              - yyok, when ready to shift a token.

             Except in the first case, yyparse will invoke yyremoveDeletes and
             then shift the next token onto all remaining stacks.  This
             synchronization of the shift (that is, after all preceding
             reductions on all stacks) helps prevent double destructor calls
             on yylval in the event of memory exhaustion.  */

          for (state_set_index yys = create_state_set_index(0); yys.uget() < yystack.yystateStack.numTops(); ++yys)
            YYCHK1 (yystack.yyprocessOneStack (yys, yyposn]b4_pure_if([b4_locations_if([, &yylloc])])[));
          yystack.yystateStack.yytops.yyremoveDeletes ();
          if (yystack.yystateStack.yytops.size() == 0)
            {
              yystack.yystateStack.yytops.yyundeleteLastStack ();
              if (yystack.yystateStack.yytops.size() == 0)
                yystack.yyFail (]b4_pure_if([b4_locations_if([&yylloc, ])])[YY_("syntax error"));
              YYCHK1 (yystack.yyresolveStack ());
              YY_DEBUG_STREAM << "Returning to deterministic operation.\n";]b4_locations_if([[
              yystack.yyerror_range[1].getState().yyloc = yylloc;]])[
              yystack.yyreportSyntaxError ();
              goto yyuser_error;
            }

          /* If any yyglrShift call fails, it will fail after shifting.  Thus,
             a copy of yylval will already be on stack 0 in the event of a
             failure in the following loop.  Thus, yychar is set to ]b4_symbol(empty, id)[
             before the loop to make sure the user destructor for yylval isn't
             called twice.  */
          yytoken_to_shift = YYTRANSLATE (yychar);
          yychar = ]b4_namespace_ref::b4_parser_class::token::b4_symbol(empty, id)[;
          yyposn += 1;
          for (state_set_index yys = create_state_set_index(0); yys.uget() < yystack.yystateStack.numTops(); ++yys)
            {
              state_num yystate = yystack.topState(yys)->yylrState;
              const short* yyconflicts;
              int yyaction = yygetLRActions (yystate, yytoken_to_shift,
                              &yyconflicts);
              /* Note that yyconflicts were handled by yyprocessOneStack.  */
              YY_DEBUG_STREAM << "On stack " << yys.get() << ", ";
              YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
              yystack.yyglrShift (yys, yyaction, yyposn,
                          &yylval]b4_locations_if([, &yylloc])[);
              YY_DEBUG_STREAM << "Stack " << yys.get() << " now in state #"
                        << yystack.topState(yys)->yylrState << '\n';
            }

          if (yystack.yystateStack.yytops.size() == 1)
            {
              YYCHK1 (yystack.yyresolveStack ());
              YY_DEBUG_STREAM  << "Returning to deterministic operation.\n";
              yystack.yystateStack.yycompressStack ();
              break;
            }
        }
      continue;
    yyuser_error:
      yystack.yyrecoverSyntaxError (]b4_pure_if([b4_locations_if([&yylloc])])[);
      yyposn = yystack.firstTopState()->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (false);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyparser.error (]b4_locations_if([yylloc, ])[YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  return yyresult;
}


/* DEBUGGING ONLY */
#if ]b4_api_PREFIX[DEBUG
static void
yypstack (glr_stack* yystackp, size_t yyk)
{
  yystackp->yypstack(create_state_set_index(static_cast<std::ptrdiff_t>(yyk)));
}
static void yypdumpstack (glr_stack* yystackp) {
  yystackp->yypdumpstack();
}

#endif

#undef yylval
#undef yychar
#undef yynerrs]b4_locations_if([
#undef yylloc])

m4_if(b4_prefix, [yy], [],
[[/* Substitute the variable and function names.  */
#define yyparse ]b4_prefix[parse
#define yylex   ]b4_prefix[lex
#define yyerror ]b4_prefix[error
#define yylval  ]b4_prefix[lval
#define yychar  ]b4_prefix[char
#define yydebug ]b4_prefix[debug
#define yynerrs ]b4_prefix[nerrs]b4_locations_if([[
#define yylloc  ]b4_prefix[lloc]])])[


]b4_namespace_open[
]dnl In this section, the parse params are the original parse_params.
m4_pushdef([b4_parse_param], m4_defn([b4_parse_param_orig]))dnl
[  /// Build a parser object.
  ]b4_parser_class::b4_parser_class[ (]b4_parse_param_decl[)]m4_ifset([b4_parse_param], [
    :])[
#if ]b4_api_PREFIX[DEBUG
    ]m4_ifset([b4_parse_param], [  ], [ :])[yycdebug_ (&std::cerr)]m4_ifset([b4_parse_param], [,])[
#endif]b4_parse_param_cons[
  {}

  ]b4_parser_class::~b4_parser_class[ ()
  {}

  ]b4_parser_class[::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  int
  ]b4_parser_class[::operator() ()
  {
    return parse ();
  }

  int
  ]b4_parser_class[::parse ()
  {
    return ::yyparse (*this]b4_user_args[);
  }

]b4_parse_error_bmatch([custom\|detailed],
[[  const char *
  ]b4_parser_class[::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    ]b4_symbol_names[
    };]b4_has_translations_if([[
    /* YYTRANSLATABLE[SYMBOL-NUM] -- Whether YY_SNAME[SYMBOL-NUM] is
       internationalizable.  */
    static ]b4_int_type_for([b4_translatable])[ yytranslatable[] =
    {
    ]b4_translatable[
    };
    return (yysymbol < YYNTOKENS && yytranslatable[yysymbol]
            ? _(yy_sname[yysymbol])
            : yy_sname[yysymbol]);]], [[
    return yy_sname[yysymbol];]])[
  }
]],
[simple],
[[#if ]b4_api_PREFIX[DEBUG || ]b4_token_table_flag[
  const char *
  ]b4_parser_class[::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if ]b4_api_PREFIX[DEBUG || ]b4_token_table_flag[
]],
[verbose],
[[  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ]b4_parser_class[::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
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
              else
                goto append;

            append:
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

  std::string
  ]b4_parser_class[::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }
]])[

]b4_parse_error_bmatch([simple\|verbose],
[[#if ]b4_api_PREFIX[DEBUG]b4_tname_if([[ || 1]])[
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const ]b4_parser_class[::yytname_[] =
  {
  ]b4_tname[
  };
#endif
]])[

  void
  ]b4_parser_class[::yy_destroy_ (const char* yymsg, symbol_kind_type yykind,
                           const semantic_type* yyvaluep]b4_locations_if([[,
                           const location_type* yylocationp]])[)
  {
    YYUSE (yyvaluep);]b4_locations_if([[
    YYUSE (yylocationp);]])[
    if (!yymsg)
      yymsg = "Deleting";
    ]b4_namespace_ref::b4_parser_class[& yyparser = *this;
    YYUSE (yyparser);
    YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    ]b4_symbol_actions([destructor])[
    YY_IGNORE_MAYBE_UNINITIALIZED_END
  }

#if ]b4_api_PREFIX[DEBUG
  /*--------------------.
  | Print this symbol.  |
  `--------------------*/

  void
  ]b4_parser_class[::yy_symbol_value_print_ (symbol_kind_type yykind,
                           const semantic_type* yyvaluep]b4_locations_if([[,
                           const location_type* yylocationp]])[) const
  {]b4_locations_if([[
    YYUSE (yylocationp);]])[
    YYUSE (yyvaluep);
    std::ostream& yyo = debug_stream ();
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    ]b4_symbol_actions([printer])[
  }

  void
  ]b4_parser_class[::yy_symbol_print_ (symbol_kind_type yykind,
                           const semantic_type* yyvaluep]b4_locations_if([[,
                           const location_type* yylocationp]])[) const
  {
    *yycdebug_ << (yykind < YYNTOKENS ? "token" : "nterm")
               << ' ' << ]b4_namespace_ref::b4_parser_class[::symbol_name (yykind) << " ("]b4_locations_if([[
               << *yylocationp << ": "]])[;
    yy_symbol_value_print_ (yykind, yyvaluep]b4_locations_if([[, yylocationp]])[);
    *yycdebug_ << ')';
  }

  std::ostream&
  ]b4_parser_class[::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ]b4_parser_class[::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ]b4_parser_class[::debug_level_type
  ]b4_parser_class[::debug_level () const
  {
    return yydebug;
  }

  void
  ]b4_parser_class[::set_debug_level (debug_level_type l)
  {
    // Actually, it is yydebug which is really used.
    yydebug = l;
  }

#endif
]m4_popdef([b4_parse_param])dnl
b4_namespace_close[]dnl
b4_glr_cc_cleanup[]dnl
b4_epilogue[]dnl
b4_output_end


m4_popdef([b4_parse_param])
