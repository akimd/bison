%%{section} %%{output-prefix} %%{output-infix} %%".hh"
/* -*- C++ -*- */

/* A Bison parser, made from %%filename,
   by GNU bison %%version.  */

/* Skeleton output parser for bison,
   Copyright 1984, 1989, 1990, 2000, 2001 Free Software Foundation, Inc.

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
/* #include "parser.hh" */
#include "location.hh"

namespace yy
{
  class %%{name};
 
  template < typename P >
  struct Traits
  {
  };

  template < >
  struct Traits< %%{name} >
  {
    typedef int       StateType;
    typedef %%{stype} SemanticType;
    typedef Location  LocationType;
  };
}

%%{prologue}

namespace yy
{
  class %%{name}
  {
  public:
    
    typedef Traits< %%{name} >::StateType StateType;
    typedef Traits< %%{name} >::SemanticType SemanticType;
    typedef Traits< %%{name} >::LocationType LocationType;

    typedef Stack< StateType >    StateStack;
    typedef Stack< SemanticType > SemanticStack;
    typedef Stack< LocationType > LocationStack;

    %%{name} () : debug (1)
    {
      looka = empty;
    }

    virtual int parse ();

  private:
    
    /* Call to lexical analyser.  */
    virtual 
    void 
    lex () 
    { 
      looka = yylex (&value);
    }

    /* Stacks.  */
    StateStack    state_stack;
    SemanticStack semantic_stack;
    LocationStack location_stack;

    /* Tables.  */
    static const short pact_[];
    static const short defact_[];
    static const short pgoto_[];
    static const short defgoto_[];
    static const short table_[];
    static const short check_[];
    static const short line_[];
    static const short r1_[];
    static const short r2_[];
    static const char* const name_[];

    /* More tables, for debugging.  */
    /* FIXME: These tables should not be printed when not in debug 
       mode.  %%ifdef?  */
    static const short rhs_[];
    static const short prhs_[];
    static const short rline_[];

    /* Even more tables.  */
    static inline char translate (int token);

    /* Eof and empty.  */
    static const int eof;
    static const int empty;

    /* State.  */
    int n;
    int len;
    int debug;
    int state;
    
    /* Lookahead.  */
    int looka;
    int ilooka;
    
    /* @$ and $$.  */
    SemanticType value;
    LocationType location;
  };
}

%%{section} %%{output-prefix} %%{output-infix} %%".cc"
#include "%%{output-prefix}%%{output-infix}.hh"

/* Enable debugging if requested.  */
#if %%{debug}

# include <cstdio>
# define YYFPRINTF std::fprintf

# define YYDPRINTF(Args)			\
do {						\
  if (debug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace. [The following comment makes no
   sense to me.  Could someone clarify it?  --akim] Since this is
   uninitialized, it does not stop multiple parsers from coexisting.
   */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* Tokens.  */
%%{tokendef}

int
yy::%%{name}::parse ()
{
  /* Initialize stack.  */
  state_stack = StateStack (0);
  semantic_stack = SemanticStack (1);
  location_stack = LocationStack (1);

  /* Reserve initial space.  The C parser needed that, but is it really
     useful here?  */
  state_stack.reserve (%%{initdepth});
  semantic_stack.reserve (%%{initdepth});
  location_stack.reserve (%%{initdepth});

  /* Start.  */
  state = 0;
  YYDPRINTF ((stderr, "Starting parse\n"));

  /* New state.  */
 yynewstate:
  state_stack.push (state);
  YYDPRINTF ((stderr, "Entering state %d\n", state));

  /* Backup.  */
 yybackup:

  /* Try to take a decision without lookahead.  */
  n = pact_[state];
  if (n == %%{flag})
    goto yydefault;

  /* Read a lookahead token.  */
  if (looka == empty)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      lex ();
    }

  /* Convert token to internal form.  */
  if (looka <= 0)
    {
      looka = eof;
      ilooka = 0;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      ilooka = translate (looka);
#if %%{debug}
      YYFPRINTF (stderr, "Next token is %d (%s", looka, name_[ilooka]);
      YYFPRINTF (stderr, ")\n");
#endif
    }

  n += ilooka;
  if (n < 0 || n > %%{last} || check_[n] != ilooka)
    goto yydefault;

  /* Reduce or error.  */
  n = table_[n];
  if (n < 0)
    {
      if (n == %%{flag})
	goto yyerrlab;
      else
	{
	  n = -n;
	  goto yyreduce;
	}
    }
  else if (n == 0)
    goto yyerrlab;
  
  /* Accept?  */
  if (n == %%{final})
    goto yyacceptlab;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ", looka, name_[ilooka]));

  /* Discard the token being shifted unless it is eof.  */
  if (looka != eof)
    looka = empty;

  semantic_stack.push (value);
  location_stack.push (location);
  state = n;
  goto yynewstate;

  /* Default action.  */
 yydefault:
  n = defact_[state];
  if (n == 0)
    goto yyerrlab;

  /* Reduce.  */
 yyreduce:
  len = r2_[n];
  value = semantic_stack[1 - len];
  location = location_stack[1 - len];

#if %%{debug}
  if (debug)
    {
      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ", n - 1, rline_[n]);
      for (unsigned i = prhs_[n];
	   rhs_[i] >= 0; ++i)
	YYFPRINTF (stderr, "%s ", name_[rhs_[i]]);
      YYFPRINTF (stderr, "-> %s\n", name_[r1_[n]]);
    }
#endif
  
  {
    SemanticType& yyval (value);
    SemanticStack& yyvsp (semantic_stack);

    switch (n)
      {
	%%{actions}
      }
  }
  
  state_stack.pop (len);
  semantic_stack.pop (len);
  location_stack.pop (len);

#if %%{debug}
  if (debug)
    {
      YYFPRINTF (stderr, "state stack now");
      for (StateStack::ConstIterator i = state_stack.begin (); 
	   i != state_stack.end (); ++i)
	YYFPRINTF (stderr, " %d", *i);
      YYFPRINTF (stderr, "\n");
    }
#endif
  
  semantic_stack.push (value);
  location_stack.push (location);

  /* Shift the result of the reduction.  */
  n = r1_[n];
  state = pgoto_[n - %%{ntokens}] + state_stack[0];
  if (state >= 0 && state <= %%{last} && check_[state] == state_stack[0])
    state = table_[state];
  else
    state = defgoto_[n - %%{ntokens}];
  goto yynewstate;

  /* Report and recover from errors.  This is very incomplete.  */
 yyerrlab:
  std::cerr << "Parse error." << std::endl; // FIXME: Need something like yyerror?
  return 1;
  
  /* Accept.  */
 yyacceptlab:
  return 0;
}

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
const short 
yy::%%{name}::pact_[] =
{
  %%{pact}
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
const short 
yy::%%{name}::defact_[] =
{
  %%{defact}
};

/* YYPGOTO[NTERM-NUM].  */
const short 
yy::%%{name}::pgoto_[] =
{
  %%{pgoto}
};

/* YYDEFGOTO[NTERM-NUM].  */
const short 
yy::%%{name}::defgoto_[] =
{
  %%{defgoto}
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
const short 
yy::%%{name}::table_[] =
{
  %%{table}
};

/* YYCHECK.  */
const short 
yy::%%{name}::check_[] =
{
  %%{check}
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
const short
yy::%%{name}::line_[] =
{
  %%{line}
};

/*YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
const short 
yy::%%{name}::r1_[] =
{
  %%{r1}
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
const short 
yy::%%{name}::r2_[] =
{
  %%{r2}
};

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
const char*
const yy::%%{name}::name_[] =
{
  %%{tname}
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
const short 
yy::%%{name}::rhs_[] =
{
  %%{rhs}
};

/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
const short 
yy::%%{name}::prhs_[] =
{
  %%{prhs}
};

/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
const short 
yy::%%{name}::rline_[] =
{
  %%{rline}
};

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
char
yy::%%{name}::translate (int token)
{
  static 
  const char 
  translate_[] =
  {
    %%{translate}
  };
  return ((unsigned)(token) <= %%{maxtok} ? translate_[token] : %%{nsym});
}

const int yy::%%{name}::eof = 0;
const int yy::%%{name}::empty = -2;

%%{epilogue}

%%{section} %%"stack.hh"
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
    operator [] (int index)
    {
      return seq_[seq_.size () - 1 + index];
    }

    inline
    const T&
    operator [] (int index) const
    {
      return seq_[seq_.size () - 1 + index];
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

    inline ConstIterator begin () const { return seq_.begin (); }
    inline ConstIterator end () const { return seq_.end (); }

  private:

    S seq_;
  };
}

#endif // not BISON_STACK_HH

%%{section} %%"location.hh"
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
