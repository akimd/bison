m4_divert(-1)                                                       -*- C -*-
m4_include([c.m4])

# Yacc compatible skeleton for Bison
# Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002
# Free Software Foundation, Inc.

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
[yyvsp@<:@m4_eval([$2 - $1])@:>@m4_ifval([$3], [.$3])])



## ----------- ##
## Locations.  ##
## ----------- ##

# b4_location_if(IF-TRUE, IF-FALSE)
# ---------------------------------
# Expand IF-TRUE, if locations are used, IF-FALSE otherwise.
m4_define([b4_location_if],
[m4_if(b4_locations_flag, [1],
       [$1],
       [$2])])


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
[yylsp@<:@m4_eval([$2 - $1])@:>@])



## -------------- ##
## %pure-parser.  ##
## -------------- ##

# b4_pure_if(IF-TRUE, IF-FALSE)
# -----------------------------
# Expand IF-TRUE, if %pure-parser, IF-FALSE otherwise.
m4_define([b4_pure_if],
[m4_if(b4_pure, [1],
       [$1],
       [$2])])


## ------------------- ##
## Output file names.  ##
## ------------------- ##

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



## --------------------------------------------------------- ##
## Defining symbol actions, e.g., printers and destructors.  ##
## --------------------------------------------------------- ##

# b4_symbol_actions(FILENAME, LINENO,
#                   SYMBOL-TAG, SYMBOL-NUM,
#                   SYMBOL-ACTION, SYMBOL-TYPENAME)
# -------------------------------------------------
m4_define([b4_symbol_actions],
[m4_pushdef([b4_dollar_dollar], [yyvalue.$6])dnl
m4_pushdef([b4_at_dollar], [yylocation])dnl
      case $4: /* $3 */
#line $2 "$1"
        $5;
#line __oline__ "__ofile__"
        break;
m4_popdef([b4_at_dollar])dnl
m4_popdef([b4_dollar_dollar])dnl
])



m4_divert(0)dnl
#output "b4_output_parser_name"
b4_copyright([Skeleton parser for Yacc-like parsing with Bison],
             [1984, 1989, 1990, 2000, 2001, 2002])

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	b4_pure

/* Using locations.  */
#define YYLSP_NEEDED b4_locations_flag

m4_if(b4_prefix[], [yy], [],
[/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse b4_prefix[]parse
#define yylex   b4_prefix[]lex
#define yyerror b4_prefix[]error
#define yylval  b4_prefix[]lval
#define yychar  b4_prefix[]char
#define yydebug b4_prefix[]debug
#define yynerrs b4_prefix[]nerrs
b4_location_if([#define yylloc b4_prefix[]lloc])])

/* Copy the first part of user declarations.  */
b4_pre_prologue

b4_token_defines(b4_tokens)

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG b4_debug
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
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
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE b4_ltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Copy the second part of user declarations.  */
b4_post_prologue

/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  b4_location_if([  YYLTYPE yyls;
])dnl
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
b4_location_if(
[# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)],
[# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)])

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[[yyi]] = (From)[[yyi]];	\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  b4_final_state_number
#define YYLAST   b4_last

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  b4_tokens_number
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  b4_nterms_number
/* YYNRULES -- Number of rules. */
#define YYNRULES  b4_rules_number
/* YYNRULES -- Number of states. */
#define YYNSTATES  b4_states_number

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  b4_undef_token_number
#define YYMAXUTOK   b4_user_token_number_max

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[[X]] : YYUNDEFTOK)

/* YYTRANSLATE[[YYLEX]] -- Bison symbol number corresponding to YYLEX.  */
static const b4_int_type_for([b4_translate]) yytranslate[[]] =
{
  b4_translate
};

#if YYDEBUG
/* YYPRHS[[YYN]] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const b4_int_type_for([b4_prhs]) yyprhs[[]] =
{
  b4_prhs
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const b4_int_type_for([b4_rhs]) yyrhs[[]] =
{
  b4_rhs
};

/* YYRLINE[[YYN]] -- source line where rule number YYN was defined.  */
static const b4_int_type_for([b4_rline]) yyrline[[]] =
{
  b4_rline
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[[SYMBOL-NUM]] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[[]] =
{
  b4_tname
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[[YYLEX-NUM]] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const b4_int_type_for([b4_toknum]) yytoknum[[]] =
{
  b4_toknum
};
# endif

/* YYR1[[YYN]] -- Symbol number of symbol that rule YYN derives.  */
static const b4_int_type_for([b4_r1]) yyr1[[]] =
{
  b4_r1
};

/* YYR2[[YYN]] -- Number of symbols composing right hand side of rule YYN.  */
static const b4_int_type_for([b4_r2]) yyr2[[]] =
{
  b4_r2
};

/* YYDEFACT[[STATE-NAME]] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const b4_int_type_for([b4_defact]) yydefact[[]] =
{
  b4_defact
};

/* YYDEFGOTO[[NTERM-NUM]]. */
static const b4_int_type_for([b4_defgoto]) yydefgoto[[]] =
{
  b4_defgoto
};

/* YYPACT[[STATE-NUM]] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF b4_pact_ninf
static const b4_int_type_for([b4_pact]) yypact[[]] =
{
  b4_pact
};

/* YYPGOTO[[NTERM-NUM]].  */
static const b4_int_type_for([b4_pgoto]) yypgoto[[]] =
{
  b4_pgoto
};

/* YYTABLE[[YYPACT[STATE-NUM]]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
#define YYTABLE_NINF b4_table_ninf
static const b4_int_type_for([b4_table]) yytable[[]] =
{
  b4_table
};

static const b4_int_type_for([b4_check]) yycheck[[]] =
{
  b4_check
};

/* YYSTOS[[STATE-NUM]] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const b4_int_type_for([b4_stos]) yystos[[]] =
{
  b4_stos
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
  Current.first_line   = Rhs[[1]].first_line;      \
  Current.first_column = Rhs[[1]].first_column;    \
  Current.last_line    = Rhs[[N]].last_line;       \
  Current.last_column  = Rhs[[N]].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

b4_pure_if(
[#ifdef YYLEX_PARAM
# define YYLEX	yylex (&yylval[]b4_location_if([, &yylloc]), YYLEX_PARAM)
#else
# define YYLEX	yylex (&yylval[]b4_location_if([, &yylloc]))
#endif],
[#define YYLEX	yylex ()])

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH b4_initdepth
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH b4_maxdepth
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

b4_c_function([yysymprint],
	      [static void],
	      [[FILE*],   [yyout]],
	      [[int],     [yytype]],
	      [[YYSTYPE], [yyvalue]]b4_location_if([,
	      [[YYLTYPE], [yylocation]]]))
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;
b4_location_if([  (void) yylocation;
])dnl

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[[yytype]]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[[yytype]], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[[yytype]]);

  switch (yytype)
    {
m4_map([b4_symbol_actions], m4_defn([b4_symbol_printers]))dnl
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */


/*----------------------------------------------------------.
| yyreport_parse_error -- report a parse error in YYSTATE.  |
`----------------------------------------------------------*/

b4_c_function([yyreport_parse_error],
	      [static void],
	      [[int],     [yystate]],
	      [[int],     [yychar]],
	      [[YYSTYPE], [yyvalue]]b4_location_if([,
	      [[YYLTYPE], [yylloc]]]))
[{
#if YYERROR_VERBOSE
  int yyn = yypact[yystate];

  if (YYPACT_NINF < yyn && yyn < YYLAST)
    {
      YYSIZE_T yysize = 0;
      int yytype = YYTRANSLATE (yychar);
      char *yymsg;
      int yyx, yycount;

      yycount = 0;
      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      for (yyx = yyn < 0 ? -yyn : 0;
	   yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  yysize += yystrlen (yytname[yyx]) + 15, yycount++;
      yysize += yystrlen ("parse error, unexpected ") + 1;
      yysize += yystrlen (yytname[yytype]);
      yymsg = (char *) YYSTACK_ALLOC (yysize);
      if (yymsg != 0)
	{
	  char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	  yyp = yystpcpy (yyp, yytname[yytype]);

	  if (yycount < 5)
	    {
	      yycount = 0;
	      for (yyx = yyn < 0 ? -yyn : 0;
		   yyx < (int) (sizeof (yytname) / sizeof (char *));
		   yyx++)
		if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		  {
		    const char *yyq = ! yycount ? ", expecting " : " or ";
		    yyp = yystpcpy (yyp, yyq);
		    yyp = yystpcpy (yyp, yytname[yyx]);
		    yycount++;
		  }
	    }
	  yyerror (yymsg);
	  YYSTACK_FREE (yymsg);
	}
      else
	yyerror ("parse error; also virtual memory exhausted");
    }
  else
#endif /* YYERROR_VERBOSE */
    yyerror ("parse error");

  /* Pacify ``unused variable'' warnings.  */
  (void) yystate;
  (void) yychar;
  (void) yyvalue;
  ]b4_location_if([(void) yylloc;])[
}]


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

b4_c_function([yydestruct],
	      [static void],
	      [[int],     [yytype]],
	      [[YYSTYPE], [yyvalue]]b4_location_if([,
	      [[YYLTYPE], [yylocation]]]))
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;
b4_location_if([  (void) yylocation;
])dnl

  switch (yytype)
    {
m4_map([b4_symbol_actions], m4_defn([b4_symbol_destructors]))dnl
      default:
        break;
    }
}



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

m4_divert_push([KILL])# ======================== M4 code.
# b4_declare_parser_variables
# ---------------------------
# Declare the variables that are global, or local to YYPARSE if
#  pure-parser
m4_define([b4_declare_parser_variables],
[/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of parse errors so far.  */
int yynerrs;b4_location_if([
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;])
])
m4_divert_pop([KILL])dnl# ====================== End of M4 code.

b4_pure_if([],
           [b4_declare_parser_variables])

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{[
  ]b4_pure_if([b4_declare_parser_variables])[
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

]b4_location_if(
[[  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;]])[

#define YYPOPSTACK   (yyvsp--, yyssp--]b4_location_if([, yylsp--])[)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
]b4_location_if([  YYLTYPE yyloc;])[

  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
]b4_location_if([  yylsp = yyls;])[
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;
]b4_location_if([	YYLTYPE *yyls1 = yyls;])[

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
]b4_location_if([		    &yyls1, yysize * sizeof (*yylsp),])[
		    &yystacksize);
]b4_location_if([	yyls = yyls1;])[
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
]b4_location_if([	YYSTACK_RELOCATE (yyls);])[
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
]b4_location_if([      yylsp = yyls + yysize - 1;])[

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with.  */

  if (yychar <= 0)		/* This means end of input.  */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more.  */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, yychar1, yylval]b4_location_if([, yyloc])[));
      YYDPRINTF ((stderr, "\n"));
    }

  yyn += yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
]b4_location_if([  *++yylsp = yylloc;])[

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

]b4_location_if(
[  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);])[

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn - 1, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] >= 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    ]{
      b4_actions
    }

/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"

[  yyvsp -= yylen;
  yyssp -= yylen;
]b4_location_if([  yylsp -= yylen;])[

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
]b4_location_if([  *++yylsp = yyloc;])[

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyreport_parse_error (yystate, yychar, yylval]b4_location_if([, yylloc])[);
    }
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyssp > yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*yyssp],
			    *yyvsp]b4_location_if([, *yylsp])[));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*yyssp], *yyvsp]b4_location_if([, *yylsp])[);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yydestruct (yychar1, yylval]b4_location_if([, yylloc])[);
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*yyssp], *yyvsp]b4_location_if([, *yylsp])[));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp]b4_location_if([, *yylsp])[);
      yyvsp--;
      yystate = *--yyssp;
]b4_location_if([      yylsp--;])[

#if YYDEBUG
      if (yydebug)
	{
	  short *yyssp1 = yyss - 1;
	  YYFPRINTF (stderr, "Error: state stack now");
	  while (yyssp1 != yyssp)
	    YYFPRINTF (stderr, " %d", *++yyssp1);
	  YYFPRINTF (stderr, "\n");
	}
#endif
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
]b4_location_if([  *++yylsp = yylloc;])[

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
]}


b4_epilogue
m4_if(b4_defines_flag, 0, [],
[#output "b4_output_header_name"
b4_copyright([Skeleton parser for Yacc-like parsing with Bison],
             [1984, 1989, 1990, 2000, 2001, 2002])

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef b4_header_guard
# define b4_header_guard

b4_token_defines(b4_tokens)

#ifndef YYSTYPE
m4_ifdef([b4_stype],
[#line b4_stype_line "b4_filename"
typedef union b4_stype yystype;
/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"],
[typedef int yystype;])
# define YYSTYPE yystype
#endif

b4_pure_if([],
[extern YYSTYPE b4_prefix[]lval;])

b4_location_if(
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
