m4_divert(-1)
# C++ skeleton for Bison
# Copyright (C) 2002 Free Software Foundation, Inc.

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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA

m4_include([c.m4])

# b4_lhs_value([TYPE])
# --------------------
# Expansion of $<TYPE>$.
m4_define([b4_lhs_value],
[yyval[]m4_ifval([$1], [.$1])])


# b4_rhs_value(RULE-LENGTH, NUM, [TYPE])
# --------------------------------------
# Expansion of $<TYPE>NUM, where the current rule has RULE-LENGTH
# symbols on RHS.
m4_define([b4_rhs_value],
[semantic_stack_@<:@m4_eval([$1 - $2])@:>@m4_ifval([$3], [.$3])])

m4_define_default([b4_ltype], [Location])

# b4_lhs_location()
# -----------------
# Expansion of @$.
m4_define([b4_lhs_location],
[yyloc])


# b4_rhs_location(RULE-LENGTH, NUM)
# ---------------------------------
# Expansion of @NUM, where the current rule has RULE-LENGTH symbols
# on RHS.
m4_define([b4_rhs_location],
[location_stack_@<:@m4_eval([$1 - $2])@:>@])


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

m4_define([b4_inherit],
          [m4_ifdef([b4_root],
		    [: public b4_root
],
		    [])])

m4_define([b4_param],
	  [m4_ifdef([b4_root],
	            [,
            const Param& param],
		    [])])

m4_define([b4_constructor],
	  [m4_ifdef([b4_root],
		    [b4_root (param),
      ],
		    [])])


m4_divert(0)dnl
#output "b4_output_header_name"
b4_copyright([C++ Skeleton parser for LALR(1) parsing with Bison],
             [2002])
#ifndef b4_header_guard
# define b4_header_guard

#include "stack.hh"
#include "location.hh"

#include <string>
#include <iostream>

/* Using locations.  */
#define YYLSP_NEEDED b4_locations_flag

b4_token_defines(b4_tokens)

/* Copy the first part of user declarations.  */
b4_pre_prologue

/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG b4_debug
#endif

/* Enabling verbose error message.  */
#ifndef YYERROR_VERBOSE
# define YYERROR_VERBOSE b4_error_verbose
#endif

#ifndef YYSTYPE
m4_ifdef([b4_stype],
[#line b4_stype_line "b4_filename"
typedef union b4_stype yystype;
/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"],
[typedef int yystype;])
# define YYSTYPE yystype
#endif

/* Copy the second part of user declarations.  */
b4_post_prologue

/* Line __line__ of __file__.  */
#line __oline__  "__ofile__"
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N) \
   Current.last_line = Rhs[[N]].last_line; \
   Current.last_column = Rhs[[N]].last_column;
#endif

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
    typedef b4_int_type_for([b4_translate]) TokenNumberType;
    typedef b4_int_type_for([b4_rhs])       RhsNumberType;
    typedef int      StateType;
    typedef yystype  SemanticType;
    typedef b4_ltype LocationType;
  };
}

namespace yy
{
  class b4_name b4_inherit
  {
  public:

    typedef Traits< b4_name >::TokenNumberType TokenNumberType;
    typedef Traits< b4_name >::RhsNumberType   RhsNumberType;
    typedef Traits< b4_name >::StateType       StateType;
    typedef Traits< b4_name >::SemanticType    SemanticType;
    typedef Traits< b4_name >::LocationType    LocationType;

    typedef Stack< StateType >    StateStack;
    typedef Stack< SemanticType > SemanticStack;
    typedef Stack< LocationType > LocationStack;

#if YYLSP_NEEDED
    b4_name (bool debug,
	    LocationType initlocation[]b4_param) :
      b4_constructor[]debug_ (debug),
      cdebug_ (std::cerr),
      initlocation_ (initlocation)
#else
    b4_name (bool debug[]b4_param) :
      b4_constructor[]debug_ (debug),
      cdebug_ (std::cerr)
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
    static const b4_int_type_for([b4_pact]) pact_[[]];
    static const b4_int_type_for([b4_pact]) pact_ninf_;
    static const b4_int_type_for([b4_defact]) defact_[[]];
    static const b4_int_type_for([b4_pgoto]) pgoto_[[]];
    static const b4_int_type_for([b4_defgoto]) defgoto_[[]];
    static const b4_int_type_for([b4_table]) table_[[]];
    static const b4_int_type_for([b4_table]) table_ninf_;
    static const b4_int_type_for([b4_check]) check_[[]];
    static const b4_int_type_for([b4_r1]) r1_[[]];
    static const b4_int_type_for([b4_r2]) r2_[[]];

#if YYDEBUG || YYERROR_VERBOSE
    static const char* const name_[[]];
#endif

    /* More tables, for debugging.  */
#if YYDEBUG
    static const RhsNumberType rhs_[[]];
    static const b4_int_type_for([b4_prhs]) prhs_[[]];
    static const b4_int_type_for([b4_rline]) rline_[[]];
    static const b4_int_type_for([b4_stos]) stos_[[]];
    static const b4_int_type_for([b4_toknum]) token_number_[[]];
#endif

    /* Even more tables.  */
    static inline TokenNumberType translate_ (int token);

    /* Constants.  */
    static const int eof_;
    static const int last_;
    static const int nnts_;
    static const int empty_;
    static const int final_;
    static const int terror_;
    static const int errcode_;
    static const int ntokens_;
    static const int initdepth_;
    static const unsigned user_token_number_max_;
    static const TokenNumberType undef_token_;

    /* State.  */
    int n_;
    int len_;
    int state_;

    /* Debugging.  */
    int debug_;
    std::ostream &cdebug_;

    /* Lookahead and lookahead in internal form.  */
    int looka_;
    int ilooka_;

    /* Message.  */
    std::string message;

    /* Semantic value and location of lookahead token.  */
    SemanticType value;
    LocationType location;

    /* @$ and $$.  */
    SemanticType yyval;
    LocationType yyloc;

    /* Initial location.  */
    LocationType initlocation_;
  };
}

#endif /* not b4_header_guard */
dnl
#output "b4_output_prefix[]b4_output_infix[].cc"
b4_copyright([C++ Skeleton parser for LALR(1) parsing with Bison],
             [2002])

#include "b4_output_header_name"

/* Enable debugging if requested.  */
#if YYDEBUG
# define YYCDEBUG    if (debug_) cdebug_
#else /* !YYDEBUG */
# define YYCDEBUG    if (0) cdebug_
#endif /* !YYDEBUG */

int
yy::b4_name::parse ()
{
  int nerrs = 0;
  int errstatus = 0;

  /* Initialize stack.  */
  state_stack_ = StateStack (0);
  semantic_stack_ = SemanticStack (1);
  location_stack_ = LocationStack (1);

  /* Start.  */
  state_ = 0;
  looka_ = empty_;
#if YYLSP_NEEDED
  location = initlocation_;
#endif
  YYCDEBUG << "Starting parse" << std::endl;

  /* New state.  */
 yynewstate:
  state_stack_.push (state_);
  YYCDEBUG << "Entering state " << state_ << std::endl;
  goto yybackup;

  /* Backup.  */
 yybackup:

  /* Try to take a decision without lookahead.  */
  n_ = pact_[[state_]];
  if (n_ == pact_ninf_)
    goto yydefault;

  /* Read a lookahead token.  */
  if (looka_ == empty_)
    {
      YYCDEBUG << "Reading a token: ";
      lex_ ();
    }

  /* Convert token to internal form.  */
  if (looka_ <= 0)
    {
      looka_ = eof_;
      ilooka_ = 0;
      YYCDEBUG << "Now at end of input." << std::endl;
    }
  else
    {
      ilooka_ = translate_ (looka_);
#if YYDEBUG
      if (debug_)
	{
	  YYCDEBUG << "Next token is " << looka_
		 << " (" << name_[[ilooka_]];
	  print_ ();
	  YYCDEBUG << ')' << std::endl;
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
      if (n_ == table_ninf_)
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
  YYCDEBUG << "Shifting token " << looka_
	 << " (" << name_[[ilooka_]] << "), ";

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
      yyval = semantic_stack_[[len_ - 1]];
      yyloc = location_stack_[[len_ - 1]];
    }
  else
    {
      yyval = semantic_stack_[[0]];
      yyloc = location_stack_[[0]];
    }

#if YYDEBUG
  if (debug_)
    {
      YYCDEBUG << "Reducing via rule " << n_ - 1
	     << " (line " << rline_[[n_]] << "), ";
      for (b4_int_type_for([b4_prhs]) i = prhs_[[n_]];
	   rhs_[[i]] >= 0; ++i)
	YYCDEBUG << name_[[rhs_[i]]] << ' ';
      YYCDEBUG << "-> " << name_[[r1_[n_]]] << std::endl;
    }
#endif

  if (len_)
    {
      Slice< LocationType, LocationStack > slice (location_stack_, len_);
      YYLLOC_DEFAULT (yyloc, slice, len_);
    }

  switch (n_)
    {
      b4_actions
    }

/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"

  state_stack_.pop (len_);
  semantic_stack_.pop (len_);
  location_stack_.pop (len_);

#if YYDEBUG
  if (debug_)
    {
      YYCDEBUG << "state stack now";
      for (StateStack::ConstIterator i = state_stack_.begin ();
	   i != state_stack_.end (); ++i)
	YYCDEBUG << ' ' << *i;
      YYCDEBUG << std::endl;
    }
#endif

  semantic_stack_.push (yyval);
  location_stack_.push (yyloc);

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
      if (pact_ninf_ < n_ && n_ < last_)
	{
	  message = "parse error, unexpected ";
	  message += name_[[ilooka_]];
	  {
	    int count = 0;
	    for (int x = (n_ < 0 ? -n_ : 0); x < ntokens_ + nnts_; ++x)
	      if (check_[[x + n_]] == x && x != terror_)
		++count;
	    if (count < 5)
	      {
		count = 0;
		for (int x = (n_ < 0 ? -n_ : 0); x < ntokens_ + nnts_; ++x)
		  if (check_[[x + n_]] == x && x != terror_)
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
      YYCDEBUG << "Discarding token " << looka_
	     << " (" << name_[[ilooka_]] << ")." << std::endl;
      looka_ = empty_;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  errstatus = 3;

  for (;;)
    {
      n_ = pact_[[state_]];
      if (n_ != pact_ninf_)
	{
	  n_ += terror_;
	  if (0 <= n_ && n_ <= last_ && check_[[n_]] == terror_)
	    {
	      n_ = table_[[n_]];
	      if (0 < n_)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (!state_stack_.height ())
	goto yyabortlab;

#if YYDEBUG
      if (debug_)
	{
	  if (stos_[[state_]] < ntokens_)
	    {
	      YYCDEBUG << "Error: popping token "
		     << token_number_[[stos_[state_]]]
		     << " (" << name_[[stos_[state_]]];
# ifdef YYPRINT
	      YYPRINT (stderr, token_number_[[stos_[state_]]],
		       semantic_stack_.top ());
# endif
	      YYCDEBUG << ')' << std::endl;
	    }
	  else
	    {
	      YYCDEBUG << "Error: popping nonterminal ("
		     << name_[[stos_[state_]]] << ')' << std::endl;
	    }
	}
#endif

      state_ = (state_stack_.pop (), state_stack_[[0]]);
      semantic_stack_.pop ();
      location_stack_.pop ();;

#if YYDEBUG
      if (debug_)
	{
	  YYCDEBUG << "Error: state stack now";
	  for (StateStack::ConstIterator i = state_stack_.begin ();
	       i != state_stack_.end (); ++i)
	    YYCDEBUG << ' ' << *i;
	  YYCDEBUG << std::endl;
	}
#endif
    }

  if (n_ == final_)
    goto yyacceptlab;

  YYCDEBUG << "Shifting error token, ";

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
const b4_int_type_for([b4_pact]) yy::b4_name::pact_ninf_ = b4_pact_ninf;
const b4_int_type_for([b4_pact])
yy::b4_name::pact_[[]] =
{
  b4_pact
};

/* YYDEFACT[[S]] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
const b4_int_type_for([b4_defact])
yy::b4_name::defact_[[]] =
{
  b4_defact
};

/* YYPGOTO[[NTERM-NUM]].  */
const b4_int_type_for([b4_pgoto])
yy::b4_name::pgoto_[[]] =
{
  b4_pgoto
};

/* YYDEFGOTO[[NTERM-NUM]].  */
const b4_int_type_for([b4_defgoto])
yy::b4_name::defgoto_[[]] =
{
  b4_defgoto
};

/* YYTABLE[[YYPACT[STATE-NUM]]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
const b4_int_type_for([b4_table]) yy::b4_name::table_ninf_ = b4_table_ninf;
const b4_int_type_for([b4_table])
yy::b4_name::table_[[]] =
{
  b4_table
};

/* YYCHECK.  */
const b4_int_type_for([b4_check])
yy::b4_name::check_[[]] =
{
  b4_check
};

#if YYDEBUG
/* STOS_[[STATE-NUM]] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
const b4_int_type_for([b4_stos])
yy::b4_name::stos_[[]] =
{
  b4_stos
};

/* TOKEN_NUMBER_[[YYLEX-NUM]] -- Internal token number corresponding
   to YYLEX-NUM.  */
const b4_int_type_for([b4_toknum])
yy::b4_name::token_number_[[]] =
{
  b4_toknum
};
#endif

/* YYR1[[YYN]] -- Symbol number of symbol that rule YYN derives.  */
const b4_int_type_for([b4_r1])
yy::b4_name::r1_[[]] =
{
  b4_r1
};

/* YYR2[[YYN]] -- Number of symbols composing right hand side of rule YYN.  */
const b4_int_type_for([b4_r2])
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
const yy::b4_name::RhsNumberType
yy::b4_name::rhs_[[]] =
{
  b4_rhs
};

/* YYPRHS[[YYN]] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
const b4_int_type_for([b4_prhs])
yy::b4_name::prhs_[[]] =
{
  b4_prhs
};

/* YYRLINE[[YYN]] -- source line where rule number YYN was defined.  */
const b4_int_type_for([b4_rline])
yy::b4_name::rline_[[]] =
{
  b4_rline
};
#endif

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
yy::b4_name::TokenNumberType
yy::b4_name::translate_ (int token)
{
  static
  const TokenNumberType
  translate_[[]] =
  {
    b4_translate
  };
  if ((unsigned) token <= user_token_number_max_)
    return translate_[[token]];
  else
    return undef_token_;
}

const int yy::b4_name::eof_ = 0;
const int yy::b4_name::last_ = b4_last;
const int yy::b4_name::nnts_ = b4_nterms_number;
const int yy::b4_name::empty_ = -2;
const int yy::b4_name::final_ = b4_final_state_number;
const int yy::b4_name::terror_ = 1;
const int yy::b4_name::errcode_ = 256;
const int yy::b4_name::ntokens_ = b4_tokens_number;
const int yy::b4_name::initdepth_ = b4_initdepth;

const unsigned yy::b4_name::user_token_number_max_ = b4_user_token_number_max;
const yy::b4_name::TokenNumberType yy::b4_name::undef_token_ = b4_undef_token_number;

b4_epilogue
dnl
#output "stack.hh"
b4_copyright([2002])

#ifndef BISON_STACK_HH
# define BISON_STACK_HH

#include <deque>

namespace yy
{
  template < class T, class S = std::deque< T > >
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
    operator [[]] (unsigned index)
    {
      return seq_[[index]];
    }

    inline
    const T&
    operator [[]] (unsigned index) const
    {
      return seq_[[index]];
    }

    inline
    void
    push (const T& t)
    {
      seq_.push_front (t);
    }

    inline
    void
    pop (unsigned n = 1)
    {
      for (; n; --n)
	seq_.pop_front ();
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

  template < class T, class S = Stack< T > >
  class Slice
  {
  public:

    Slice (const S& stack,
	   unsigned range) : stack_ (stack),
			     range_ (range)
    {
    }

    inline
    const T&
    operator [[]] (unsigned index) const
    {
      return stack_[[range_ - index]];
    }

  private:

    const S& stack_;
    unsigned range_;
  };
}

#endif // not BISON_STACK_HH
dnl
#output "location.hh"
b4_copyright([2002])

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
