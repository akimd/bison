m4_divert(-1)

# m4_define_default(MACRO, VALUE)
# -------------------------------
# Define MACRO to VALUE, unless already defined.
m4_define([m4_define_default],
[m4_ifdef([$1], [], [m4_define($@)])])

m4_define_default([b4_input_suffix], [.y])

m4_define_default([b4_output_parser_suffix],
[m4_translit(b4_input_suffix, [yY], [cC])])

m4_define_default([b4_output_parser_name],
[b4_output_prefix[]b4_output_infix[]b4_output_parser_suffix[]])


m4_define_default([b4_output_header_suffix],
[m4_translit(b4_input_suffix, [yY], [hH])])

m4_define_default([b4_output_header_name],
[b4_output_prefix[]b4_output_infix[]b4_output_header_suffix[]])

m4_define_default([b4_header_guard],
                  [m4_bpatsubst(m4_toupper([BISON_]b4_output_header_name),
                                [[^ABCDEFGHIJKLMNOPQRSTUVWXYZ]], [_])])

m4_divert(0)dnl
#output "b4_output_prefix[]b4_output_infix[]-class.hh"
/* -*- C++ -*- */

/* A Bison parser, made from b4_filename,
   by GNU bison b4_version.  */

/* Skeleton output parser for bison,
   Copyright 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#include "stack.hh"
#include "location.hh"

#include <string>

/* Using locations.  */
#define YYLSP_NEEDED b4_locations_flag

b4_prologue

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG b4_debug
#endif

/* Enabling verbose error message.  */
#ifndef YYERROR_VERBOSE
# define YYERROR_VERBOSE b4_error_verbose
#endif

// FIXME: This should be defined in traits, not here.
typedef b4_stype yystype;

namespace yy
{
  class b4_name;

  template < typename P >
  struct Traits
  {
  };

  template < >
  struct Traits< b4_name >
  {
    typedef int      StateType;
    typedef yystype  SemanticType;
    typedef b4_ltype LocationType;
  };
}

namespace yy
{
  class b4_name
  {
  public:

    typedef Traits< b4_name >::StateType    StateType;
    typedef Traits< b4_name >::SemanticType SemanticType;
    typedef Traits< b4_name >::LocationType LocationType;

    typedef Stack< StateType >    StateStack;
    typedef Stack< SemanticType > SemanticStack;
    typedef Stack< LocationType > LocationStack;

#if YYLSP_NEEDED
    b4_name (bool debug,
	     LocationType initlocation) : debug_ (debug),
					  initlocation_ (initlocation)
#else
    b4_name (bool debug) : debug_ (debug)
#endif
    {
    }

    virtual ~b4_name ()
    {
    }

    virtual int parse ();

  private:

    virtual void lex_ ();
    virtual void error_ ();
    virtual void print_ ();

    /* Stacks.  */
    StateStack    state_stack_;
    SemanticStack semantic_stack_;
    LocationStack location_stack_;

    /* Tables.  */
    static const short pact_[[]];
    static const short defact_[[]];
    static const short pgoto_[[]];
    static const short defgoto_[[]];
    static const short table_[[]];
    static const short check_[[]];
    static const short r1_[[]];
    static const short r2_[[]];

#if YYDEBUG || YYERROR_VERBOSE
    static const char* const name_[[]];
#endif

    /* More tables, for debugging.  */
#if YYDEBUG
    static const short rhs_[[]];
    static const short prhs_[[]];
    static const short rline_[[]];
#endif

    /* Even more tables.  */
    static inline char translate_ (int token);

    /* Constants.  */
    static const int eof_;
    static const int last_;
    static const int flag_;
    static const int nnts_;
    static const int nsym_;
    static const int empty_;
    static const int final_;
    static const int terror_;
    static const int errcode_;
    static const int ntokens_;
    static const int initdepth_;
    static const unsigned maxtok_;

    /* State.  */
    int n_;
    int len_;
    int debug_;
    int state_;

    /* Lookahead and lookahead in internal form.  */
    int looka_;
    int ilooka_;

    /* Message.  */
    std::string message;

    /* @$ and $$.  */
    SemanticType value;
    LocationType location;

    /* Initial location.  */
    LocationType initlocation_;
  };
}

#output "b4_output_prefix[]b4_output_infix[].cc"
#include "b4_output_prefix[]b4_output_infix-class.hh"

/* Enable debugging if requested.  */
#if YYDEBUG
# include <cstdio>
# define YYFPRINTF std::fprintf
# define YYDPRINTF(Args)			\
do {						\
  if (debug_)					\
    YYFPRINTF Args;				\
} while (0)
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* Tokens.  */
b4_tokendef

int
yy::b4_name::parse ()
{
  int nerrs = 0;
  int errstatus = 0;

  /* Initialize stack.  */
  state_stack_ = StateStack (0);
  semantic_stack_ = SemanticStack (1);
  location_stack_ = LocationStack (1);

  /* Reserve initial space.  The C parser needed that, but is it really
     useful here?  */
  state_stack_.reserve (initdepth_);
  semantic_stack_.reserve (initdepth_);
  location_stack_.reserve (initdepth_);

  /* Start.  */
  state_ = 0;
  looka_ = empty_;
#if YYLSP_NEEDED
  location = initlocation_;
#endif
  YYDPRINTF ((stderr, "Starting parse\n"));

  /* New state.  */
 yynewstate:
  state_stack_.push (state_);
  YYDPRINTF ((stderr, "Entering state %d\n", state_));
  goto yybackup;

  /* Backup.  */
 yybackup:

  /* Try to take a decision without lookahead.  */
  n_ = pact_[[state_]];
  if (n_ == flag_)
    goto yydefault;

  /* Read a lookahead token.  */
  if (looka_ == empty_)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      lex_ ();
    }

  /* Convert token to internal form.  */
  if (looka_ <= 0)
    {
      looka_ = eof_;
      ilooka_ = 0;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      ilooka_ = translate_ (looka_);
#if YYDEBUG
      if (debug_)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s", looka_, name_[[ilooka_]]);
	  print_ ();
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  n_ += ilooka_;
  if (n_ < 0 || n_ > last_ || check_[[n_]] != ilooka_)
    goto yydefault;

  /* Reduce or error.  */
  n_ = table_[[n_]];
  if (n_ < 0)
    {
      if (n_ == flag_)
	goto yyerrlab;
      else
	{
	  n_ = -n_;
	  goto yyreduce;
	}
    }
  else if (n_ == 0)
    goto yyerrlab;

  /* Accept?  */
  if (n_ == final_)
    goto yyacceptlab;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ", looka_, name_[[ilooka_]]));

  /* Discard the token being shifted unless it is eof.  */
  if (looka_ != eof_)
    looka_ = empty_;

  semantic_stack_.push (value);
  location_stack_.push (location);

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (errstatus)
    --errstatus;

  state_ = n_;
  goto yynewstate;

  /* Default action.  */
 yydefault:
  n_ = defact_[[state_]];
  if (n_ == 0)
    goto yyerrlab;
  goto yyreduce;

  /* Reduce.  */
 yyreduce:
  len_ = r2_[[n_]];
  if (len_)
    {
      value = semantic_stack_[[1 - len_]];
      location = location_stack_[[1 - len_]];
    }
  else
    {
      value = semantic_stack_[[0]];
      location = location_stack_[[0]];
    }

#if YYDEBUG
  if (debug_)
    {
      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ", n_ - 1, rline_[[n_]]);
      for (unsigned i = prhs_[[n_]];
	   rhs_[[i]] >= 0; ++i)
	YYFPRINTF (stderr, "%s ", name_[[rhs_[i]]]);
      YYFPRINTF (stderr, "-> %s\n", name_[[r1_[n_]]]);
    }
#endif

  {
    SemanticType& yyval (value);
    LocationType& yyloc (location);
    SemanticStack& yyvsp (semantic_stack_);
    LocationStack& yylsp (location_stack_);

    switch (n_)
      {
	b4_actions
      }
  }

  state_stack_.pop (len_);
  semantic_stack_.pop (len_);
  location_stack_.pop (len_);

#if YYDEBUG
  if (debug_)
    {
      YYFPRINTF (stderr, "state stack now");
      for (StateStack::ConstIterator i = state_stack_.begin ();
	   i != state_stack_.end (); ++i)
	YYFPRINTF (stderr, " %d", *i);
      YYFPRINTF (stderr, "\n");
    }
#endif

  semantic_stack_.push (value);
  location_stack_.push (location);

  /* Shift the result of the reduction.  */
  n_ = r1_[[n_]];
  state_ = pgoto_[[n_ - ntokens_]] + state_stack_[[0]];
  if (state_ >= 0 && state_ <= last_ && check_[[state_]] == state_stack_[[0]])
    state_ = table_[[state_]];
  else
    state_ = defgoto_[[n_ - ntokens_]];
  goto yynewstate;

  /* Report and recover from errors.  This is very incomplete.  */
 yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!errstatus)
    {
      ++nerrs;

#if YYERROR_VERBOSE
      n_ = pact_[[state_]];
      if (n_ > flag_ && n_ < last_)
	{
	  message = "parse error, unexpected ";
	  message += name_[[ilooka_]];
	  {
	    int count = 0;
	    for (int x = (n_ < 0 ? -n_ : 0); x < ntokens_ + nnts_; ++x)
	      if (check_[[x + n_]] == x)
		++count;
	    if (count < 5)
	      {
		count = 0;
		for (int x = (n_ < 0 ? -n_ : 0); x < ntokens_ + nnts_; ++x)
		  if (check_[[x + n_]] == x)
		    {
		      message += (!count++) ? ", expecting " : " or ";
		      message += name_[[x]];
		    }
	      }
	  }
	}
      else
#endif
	message = "parse error";
      error_ ();
    }
  goto yyerrlab1;

  /* Error raised explicitly by an action.  */
 yyerrlab1:
  if (errstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (looka_ == eof_)
	goto yyabortlab;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n", looka_, name_[[ilooka_]]));
      looka_ = empty_;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  errstatus = 3;
  goto yyerrhandle;

  /* Pop the current state because it cannot handle the error token.  */
 yyerrdefault:
 yyerrpop:
  if (!state_stack_.height ())
    goto yyabortlab;
  state_ = (state_stack_.pop (), state_stack_[[0]]);
  semantic_stack_.pop ();
  location_stack_.pop ();;

#if YYDEBUG
  if (debug_)
    {
      YYFPRINTF (stderr, "Error: state stack now");
      for (StateStack::ConstIterator i = state_stack_.begin ();
	   i != state_stack_.end (); ++i)
	YYFPRINTF (stderr, " %d", *i);
      YYFPRINTF (stderr, "\n");
    }
#endif

 yyerrhandle:
  n_ = pact_[[state_]];
  if (n_ == flag_)
    goto yyerrdefault;

  n_ += terror_;
  if (n_ < 0 || n_ > last_ || check_[[n_]] != terror_)
    goto yyerrdefault;

  n_ = table_[[n_]];
  if (n_ < 0)
    {
      if (n_ == flag_)
	goto yyerrpop;
      n_ = -n_;
      goto yyreduce;
    }
  else if (!n_)
    goto yyerrpop;

  if (n_ == final_)
    goto yyacceptlab;

  YYDPRINTF ((stderr, "Shifting error token, "));

  semantic_stack_.push (value);
  location_stack_.push (location);

  state_ = n_;
  goto yynewstate;

  /* Accept.  */
 yyacceptlab:
  return 0;

  /* Abort.  */
 yyabortlab:
  return 1;
}

void
yy::b4_name::lex_ ()
{
#if YYLSP_NEEDED
  looka_ = yylex (&value, &location);
#else
  looka_ = yylex (&value);
#endif
}

/* YYPACT[[STATE-NUM]] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
const short
yy::b4_name::pact_[[]] =
{
  b4_pact
};

/* YYDEFACT[[S]] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
const short
yy::b4_name::defact_[[]] =
{
  b4_defact
};

/* YYPGOTO[[NTERM-NUM]].  */
const short
yy::b4_name::pgoto_[[]] =
{
  b4_pgoto
};

/* YYDEFGOTO[[NTERM-NUM]].  */
const short
yy::b4_name::defgoto_[[]] =
{
  b4_defgoto
};

/* YYTABLE[[YYPACT[STATE-NUM]]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
const short
yy::b4_name::table_[[]] =
{
  b4_table
};

/* YYCHECK.  */
const short
yy::b4_name::check_[[]] =
{
  b4_check
};

/* YYR1[[YYN]] -- Symbol number of symbol that rule YYN derives.  */
const short
yy::b4_name::r1_[[]] =
{
  b4_r1
};

/* YYR2[[YYN]] -- Number of symbols composing right hand side of rule YYN.  */
const short
yy::b4_name::r2_[[]] =
{
  b4_r2
};

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNAME[[SYMBOL-NUM]] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
const char*
const yy::b4_name::name_[[]] =
{
  b4_tname
};
#endif

#if YYDEBUG
/* YYRHS -- A `-1'-separated list of the rules' RHS. */
const short
yy::b4_name::rhs_[[]] =
{
  b4_rhs
};

/* YYPRHS[[YYN]] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
const short
yy::b4_name::prhs_[[]] =
{
  b4_prhs
};

/* YYRLINE[[YYN]] -- source line where rule number YYN was defined.  */
const short
yy::b4_name::rline_[[]] =
{
  b4_rline
};
#endif

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
char
yy::b4_name::translate_ (int token)
{
  static
  const char
  translate_[[]] =
  {
    b4_translate
  };
  return ((unsigned)(token) <= maxtok_ ? translate_[[token]] : nsym_);
}

const int yy::b4_name::eof_ = 0;
const int yy::b4_name::last_ = b4_last;
const int yy::b4_name::flag_ = b4_flag;
const int yy::b4_name::nnts_ = b4_nnts;
const int yy::b4_name::nsym_ = b4_nsym;
const int yy::b4_name::empty_ = -2;
const int yy::b4_name::final_ = b4_final;
const int yy::b4_name::terror_ = 1;
const int yy::b4_name::errcode_ = 256;
const int yy::b4_name::ntokens_ = b4_ntokens;
const int yy::b4_name::initdepth_ = b4_initdepth;

const unsigned yy::b4_name::maxtok_ = b4_maxtok;

b4_epilogue

#output "stack.hh"
#ifndef BISON_STACK_HH
# define BISON_STACK_HH

#include <vector>

namespace yy
{
  template < class T, class S = std::vector< T > >
  class Stack
  {
  public:

    typedef typename S::iterator Iterator;
    typedef typename S::const_iterator ConstIterator;

    Stack () : seq_ ()
    {
    }

    Stack (unsigned n) : seq_ (n)
    {
    }

    inline
    T&
    operator [[]] (int index)
    {
      return seq_[[seq_.size () - 1 + index]];
    }

    inline
    const T&
    operator [[]] (int index) const
    {
      return seq_[[seq_.size () - 1 + index]];
    }

    inline
    void
    push (const T& t)
    {
      seq_.push_back (t);
    }

    inline
    void
    pop (unsigned n = 1)
    {
      for (; n; --n)
	seq_.pop_back ();
    }

    inline
    void
    reserve (unsigned n)
    {
      seq_.reserve (n);
    }

    inline
    unsigned
    height () const
    {
      return seq_.size ();
    }

    inline ConstIterator begin () const { return seq_.begin (); }
    inline ConstIterator end () const { return seq_.end (); }

  private:

    S seq_;
  };
}

#endif // not BISON_STACK_HH

#output "location.hh"
#ifndef BISON_LOCATION_HH
# define BISON_LOCATION_HH

namespace yy
{
  struct Position
  {
    int line;
    int column;
  };

  struct Location
  {
    Position first;
    Position last;
  };
}

#endif // not BISON_LOCATION_HH

m4_if(b4_defines_flag, 0, [],
[#output "b4_output_header_name"
#ifndef b4_header_guard
# define b4_header_guard

b4_tokendef

#ifndef YYSTYPE
typedef b4_stype
yystype;
# define YYSTYPE yystype
#endif

m4_if(b4_pure, [0],
[extern YYSTYPE b4_prefix[]lval;])

m4_if(b4_locations_flag, [0], [],
[#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
#endif

m4_if(b4_pure, [0],
[extern YYLTYPE b4_prefix[]lloc;])
])
#endif /* not b4_header_guard */
])
