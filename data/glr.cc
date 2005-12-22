m4_divert(-1)                                                       -*- C -*-

# C++ GLR skeleton for Bison
# Copyright (C) 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301  USA

# We require a pure interface using locations.
m4_define([b4_location_flag], [1])
m4_define([b4_pure],          [1])

m4_include(b4_pkgdatadir/[c++.m4])
m4_include(b4_pkgdatadir/[location.cc])


# b4_yysymprint_generate(FUNCTION-DECLARATOR)
# -------------------------------------------
# Generate the "yysymprint" function, which declaration is issued using
# FUNCTION-DECLARATOR, which may be "b4_c_ansi_function_def" for ISO C
# or "b4_c_function_def" for K&R.
m4_define([b4_yysymprint_generate],
[b4_c_ansi_function_decl([yyerror],
    [static void],
    [[yy::b4_parser_class_name::location_type *yylocationp], [yylocationp]],
    b4_parse_param,
    [[const char* msg], [msg]])[

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

]$1([yysymprint],
    [static void],
    [[FILE *yyoutput],       [yyoutput]],
    [[int yytype],           [yytype]],
    [[const yy::b4_parser_class_name::semantic_type *yyvaluep],
                             [yyvaluep]][]dnl
b4_location_if([,
    [[const yy::b4_parser_class_name::location_type *yylocationp],
                             [yylocationp]]])[]dnl
m4_ifset([b4_parse_param], [, b4_parse_param]))[
{
  YYUSE (yyoutput);
]b4_parse_param_use[]dnl
[
  yyparser.yysymprint_ (yytype, yyvaluep]b4_location_if([, yylocationp])[);
}
]])

m4_prepend([b4_epilogue],
[[
/*------------------.
| Report an error.  |
`------------------*/

]b4_c_ansi_function_def([yyerror],
    [static void],
    [[yy::b4_parser_class_name::location_type *yylocationp], [yylocationp]],
    b4_parse_param,
    [[const char* msg], [msg]])[
{
]b4_parse_param_use[]dnl
[
  yyparser.error (*yylocationp, msg);
}


namespace yy
{
]dnl Restore the actual parser params.
m4_popdef([b4_parse_param])dnl
[
  /// Build a parser object.
  ]b4_parser_class_name::b4_parser_class_name[ (]b4_parse_param_decl[)
    : yycdebug_ (&std::cerr)]b4_parse_param_cons[
  {
  }

  ]b4_parser_class_name::~b4_parser_class_name[ ()
  {
  }

  int
  ]b4_parser_class_name[::parse ()
  {
    return ::yyparse (*this]b4_user_args[);
  }

#if YYDEBUG

  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  void
  ]b4_parser_class_name[::yysymprint_ (int yytype,
                           const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    /* Pacify ``unused variable'' warnings.  */
    YYUSE (yyvaluep);
    YYUSE (yylocationp);
    /* Backward compatibility, but should be removed eventually.  */
    std::ostream& cdebug_ = *yycdebug_;
    YYUSE (cdebug_);

    *yycdebug_ << (yytype < YYNTOKENS ? "token" : "nterm")
  	       << ' ' << yytname[yytype] << " ("
               << *yylocationp << ": ";
    switch (yytype)
      {
  ]m4_map([b4_symbol_actions], m4_defn([b4_symbol_printers]))dnl
[        default:
          break;
      }
    *yycdebug_ << ')';
  }

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
    return ::yydebug;
  }

  void
  ]b4_parser_class_name[::set_debug_level (debug_level_type l)
  {
    ::yydebug = l;
  }

#endif /* ! YYDEBUG */

} // namespace yy

]])

# Let glr.c believe that the user arguments are only the parser itself.
m4_pushdef([b4_parse_param],
	   [[yy::b4_parser_class_name& yyparser, yyparser],]
           m4_defn([b4_parse_param]))
m4_include(b4_pkgdatadir/[glr.c])
@output @output_header_name@
b4_copyright([C++ Skeleton parser for GLALR(1) parsing with Bison],
             [2002, 2003, 2004, 2005])[

/* C++ GLR parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H

#include <string>
#include <iostream>

/* Using locations.  */
#define YYLSP_NEEDED ]b4_locations_flag[

namespace yy
{
  class position;
  class location;
}

/* Copy the first part of user declarations.  */
]b4_pre_prologue[

]/* Line __line__ of glr.cc.  */
b4_syncline([@oline@], [@ofile@])[

#include "stack.hh"
#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG ]b4_debug[
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE ]b4_error_verbose[
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE ]b4_token_table[
#endif

#if 0
/* Copy the second part of user declarations.  */
]b4_post_prologue[

]/* Line __line__ of glr.cc.  */

b4_syncline([@oline@], [@ofile@])[
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).begin  = YYRHSLOC (Rhs, 1).begin;			\
	  (Current).end    = YYRHSLOC (Rhs, N).end;			\
	}								\
      else								\
	{								\
	  (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;	\
	}								\
    while (0)
#endif

namespace yy
{
  /// A Bison parser.
  class ]b4_parser_class_name[
  {
  public:
    /// Symbol semantic values.
#if ! defined (YYSTYPE)
]m4_ifdef([b4_stype],
[b4_syncline([b4_stype_line], [b4_file_name])
    union semantic_type b4_stype;
/* Line __line__ of lalr1.cc.  */
b4_syncline([@oline@], [@ofile@])],
[    typedef int semantic_type;])[
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef ]b4_location_type[ location_type;
    /// Tokens.
    struct token
    {
      ]b4_token_enums(b4_tokens)[
    };

    /// Build a parser object.
    ]b4_parser_class_name[ (]b4_parse_param_decl[);
    virtual ~]b4_parser_class_name[ ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

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

  private:

  public:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);
  private:

#if YYDEBUG
  public:
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yysymprint_ (int yytype,
			      const semantic_type* yyvaluep,
			      const location_type* yylocationp);
  private:
#endif /* ! YYDEBUG */


    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
                             int yytype,
                             semantic_type* yyvaluep,
                             location_type* yylocationp);

    /* Debugging.  */
    std::ostream* yycdebug_;
]b4_parse_param_vars[
  };

]dnl Redirections for glr.c.
[#define YYSTYPE yy::]b4_parser_class_name[::semantic_type
#define YYLTYPE yy::]b4_parser_class_name[::location_type

}

#endif /* ! defined PARSER_HEADER_H */]
