m4_divert(-1)                                                       -*- C -*-
m4_include([c.m4])

# GLR skeleton for Bison
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

# b4_lhs_value([TYPE])
# --------------------
# Expansion of $<TYPE>$.
m4_define([b4_lhs_value],
[(*yyvalp)[]m4_ifval([$1], [.$1])])


# b4_rhs_value(RULE-LENGTH, NUM, [TYPE])
# --------------------------------------
# Expansion of $<TYPE>NUM, where the current rule has RULE-LENGTH
# symbols on RHS.
m4_define([b4_rhs_value],
[yyvsp@<:@m4_eval([$2 - $1])@:>@.yystate.yysemantics.yysval[]m4_ifval([$3], [.$3])])



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
[(*yylocp)])


# b4_rhs_location(RULE-LENGTH, NUM)
# ---------------------------------
# Expansion of @NUM, where the current rule has RULE-LENGTH symbols
# on RHS.
m4_define([b4_rhs_location],
[yyvsp@<:@m4_eval([$2 - $1])@:>@.yystate.yyloc])



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


m4_divert(0)dnl
#output "b4_output_parser_name"
b4_copyright([Skeleton parser for GLR parsing with Bison], [2002])
[
/* This is the parser code for GLR (Generalized LR) parser. */

/* FIXME: minimize these */
#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Identify Bison output.  */
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	]b4_pure[

/* Using locations.  */
#define YYLSP_NEEDED ]b4_locations_flag[

]m4_if(b4_prefix[], [yy], [],
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

b4_token_defines(b4_tokens)[

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

#ifndef YYSTYPE
]m4_ifdef([b4_stype],
[#line b4_stype_line "b4_filename"
typedef union b4_stype yystype;
/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"],
[typedef int yystype;])[
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
]b4_location_if([
  int yyfirst_line;
  int yyfirst_column;
  int yylast_line;
  int yylast_column;])[
} yyltype;
# define YYLTYPE ]b4_ltype[
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Default (constant) values used for initialization for null
   right-hand sides.  Unlike the standard bison.simple template,
   here we set the default values of the $$ and $@ to zeroed-out
   values.  Since the default value of these quantities is undefined,
   this behavior is technically correct. */
static YYSTYPE yyval_default;
static YYLTYPE yyloc_default;

/* Copy the second part of user declarations.  */
]b4_post_prologue[

]/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"
[
#if ! defined (__cplusplus)
   typedef char bool;
#  define yytrue 1
#  define yyfalse 0
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if !defined (__GNUC__) || __GNUC__ < 2 || \
(__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef ATTRIBUTE_UNUSED
# define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif

#if ! defined (__GNUC__)
# define inline
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  ]b4_final_state_number[
#define YYFLAG	 ]b4_flag[
#define YYLAST   ]b4_last[

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  ]b4_tokens_number[
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  ]b4_nterms_number[
/* YYNRULES -- Number of rules. */
#define YYNRULES  ]b4_rules_number[
/* YYNRULES -- Number of states. */
#define YYNSTATES  ]b4_states_number[
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule. */
#define YYMAXRHS ]b4_r2_max[

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  ]b4_undef_token_number[
#define YYMAXUTOK   ]b4_user_token_number_max[

#define YYTRANSLATE(YYX) \
  ((unsigned)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const ]b4_int_type_for([b4_translate])[ yytranslate[] =
{
  ]b4_translate[
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const ]b4_int_type_for([b4_prhs])[ yyprhs[] =
{
  ]b4_prhs[
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const ]b4_int_type_for([b4_rhs])[ yyrhs[] =
{
  ]b4_rhs[
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const ]b4_int_type_for([b4_rline])[ yyrline[] =
{
  ]b4_rline[
};
#endif

#if (YYDEBUG) || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  ]b4_tname[
};

#define yytname_size ((int) (sizeof (yytname) / sizeof (yytname[0])))
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const ]b4_int_type_for([b4_r1])[ yyr1[] =
{
  ]b4_r1[
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const ]b4_int_type_for([b4_r2])[ yyr2[] =
{
  ]b4_r2[
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none). */
static const ]b4_int_type_for([b4_dprec])[ yydprec[] =
{
  ]b4_dprec[
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM. */
static const ]b4_int_type_for([b4_merger])[ yymerger[] =
{
  ]b4_merger[
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
static const ]b4_int_type_for([b4_defact])[ yydefact[] =
{
  ]b4_defact[
};

/* YYPDEFGOTO[NTERM-NUM]. */
static const ]b4_int_type_for([b4_defgoto])[ yydefgoto[] =
{
  ]b4_defgoto[
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF ]b4_pact_ninf[
static const ]b4_int_type_for([b4_pact])[ yypact[] =
{
  ]b4_pact[
};

/* YYPGOTO[NTERM-NUM].  */
static const ]b4_int_type_for([b4_pgoto])[ yypgoto[] =
{
  ]b4_pgoto[
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
#define YYTABLE_NINF ]b4_table_ninf[
static const ]b4_int_type_for([b4_table])[ yytable[] =
{
  ]b4_table[
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- pointer into yyconfl of start of list
   of conflicting reductions corresponding to action entry for state
   STATE-NUM in yytable.  0 means no conflicts.  The list in yyconfl
   is terminated by a rule number of 0. */
static const ]b4_int_type_for([b4_conflict_list_heads])[ yyconflp[] =
{
  ]b4_conflict_list_heads[
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated
   by 0, pointed into by YYCONFLP. */
]dnl Do not use b4_int_type_for here, since there are places where
dnl pointers onto yyconfl are taken, which type is "short *".
dnl We probably ought to introduce a type for confl.
[static const short yyconfl[] =
{
  ]b4_conflicting_rules[
};

static const ]b4_int_type_for([b4_check])[ yycheck[] =
{
  ]b4_check[
};


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG void
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* Error token number */
#define YYTERROR 1

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#define YYRHSLOC(yyRhs,YYK) (yyRhs[YYK].yystate.yyloc)

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(yyCurrent, yyRhs, YYN)			\
  yyCurrent.yyfirst_line   = YYRHSLOC(yyRhs,1).yyfirst_line;	\
  yyCurrent.yyfirst_column = YYRHSLOC(yyRhs,1).yyfirst_column;	\
  yyCurrent.yylast_line    = YYRHSLOC(yyRhs,YYN).yylast_line;	\
  yyCurrent.yylast_column  = YYRHSLOC(yyRhs,YYN).yylast_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

]b4_pure_if(
[
#ifdef YYLEX_PARAM
# define YYLEX	yylex (yylvalp, b4_location_if([yyllocp, ])YYLEX_PARAM)
#else
# define YYLEX	yylex (yylvalp[]b4_location_if([, yyllocp]))
#endif],
[#define YYLEX  yylex ()])

b4_pure_if(
[
#undef yynerrs
#define yynerrs (yystack->yyerrcnt)
#undef yychar
#define yychar (yystack->yyrawchar)],
[YYSTYPE yylval;

YYLTYPE yylloc;

int yynerrs;
int yychar;])[

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)							     \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; } 	     \
   while (0)

#if YYDEBUG

#if ! defined (YYFPRINTF)
#  define YYFPRINTF fprintf
#endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
  /* Avoid empty `if' bodies.  */
# define YYDPRINTF(Args)  {}
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH ]b4_initdepth[
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH ]b4_maxdepth[
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call expandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data. */
#define YYHEADROOM 2

#if ! defined (YYSTACKEXPANDABLE) \
    && (! defined (__cplusplus) || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
#define YYSTACKEXPANDABLE 1
#else
#define YYSTACKEXPANDABLE 0
#endif

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short yySymbol;

/** Item references, as in LALR(1) machine */
typedef short yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;
typedef struct yyGLRStateSet yyGLRStateSet;

struct yyGLRState {
  bool yyisState;
  bool yyresolved;
  yyStateNum yylrState;
  yyGLRState* yypred;
  size_t yyposn;
  union {
    yySemanticOption* yyfirstVal;
    YYSTYPE yysval;
  } yysemantics;
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  bool yyisState;
  yyRuleNum yyrule;
  yyGLRState* yystate;
  yySemanticOption* yynext;
};

union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrflag;
  int yyerrState;
]b4_pure_if(
[
  int yyerrcnt;
  int yyrawchar;
])[
  yySymbol* yytokenp;
  jmp_buf yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  int yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

static void yyinitGLRStack (yyGLRStack* yystack, size_t yysize);
static void yyexpandGLRStack (yyGLRStack* yystack);
static void yyfreeGLRStack (yyGLRStack* yystack);

static void
yyFail (yyGLRStack* yystack, const char* yyformat, ...)
{
  if (yyformat != NULL)
    {
      char yymsg[256];
      va_list yyap;
      va_start (yyap, yyformat);
      yystack->yyerrflag = 1;
      vsprintf (yymsg, yyformat, yyap);
      yyerror (yymsg);
    }
  longjmp (yystack->yyexception_buffer, 1);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  Valid until next call to
 *  tokenName. */
static inline const char*
yytokenName (yySymbol yytoken)
{
  return yytname[yytoken];
}
#endif

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$). Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT. */
static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
	      YYSTYPE* yyvalp, YYLTYPE* yylocp, yyGLRStack* yystack)
{
  /* Avoid `unused' warnings in there are no $n. */
  (void) yystack;

  if (yyrhslen == 0)
    {
      *yyvalp = yyval_default;
      *yylocp = yyloc_default;
    }
  else
    {
      *yyvalp = yyvsp[1-yyrhslen].yystate.yysemantics.yysval;
      *yylocp = yyvsp[1-yyrhslen].yystate.yyloc;
    }
# undef yyval
# define yyval (*yyvalp)
# undef yyerrok
# define yyerrok (yystack->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerr
# undef YYRECOVERING
# define YYRECOVERING (yystack->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = *(yystack->yytokenp) = YYEMPTY)
# undef YYBACKUP
# define YYBACKUP(Token, Value)						     \
  do {									     \
    yyerror ("syntax error: cannot back up");				     \
    YYERROR;								     \
  } while (0)

]
   switch (yyn)
     {
       b4_actions
     }

   return yyok;
# undef yyval
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
/* Line __line__ of __file__.  */
#line __oline__ "__ofile__"
}


static YYSTYPE
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYSTYPE yyval = *yy0;
  /* `Use' the arguments. */
  (void) yy0;
  (void) yy1;

  switch (yyn)
    {
      b4_mergers
    }
  return yyval;
}
[
				/* Bison grammar-table manipulation */

/** Number of symbols composing the right hand side of rule #RULE. */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

/** Left-hand-side symbol for rule #RULE. */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

/** True iff LR state STATE has only a default reduction (regardless
 *  of token). */
static inline bool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact[yystate] == YYPACT_NINF;
}

/** The default reduction for STATE, assuming it has one. */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

/** Set *ACTION to the action to take in STATE on seeing TOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
	        int* yyaction, const short** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yyindex < 0 || yyindex > YYLAST || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (yyr >= 0 && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline bool
yyisShiftAction (int yyaction)
{
  return yyaction > 0;
}

static inline bool
yyisErrorAction (int yyaction)
{
  return yyaction == 0 || yyaction == YYPACT_NINF;
}

				/* GLRStates */

/** True iff the semantic value of the edge leading to STATE is
 *  resolved. */
static inline bool
yyhasResolvedValue (yyGLRState* yystate)
{
  return yystate->yyresolved;
}

static void
yyaddDeferredAction (yyGLRStack* yystack, yyGLRState* yystate,
		     yyGLRState* yyrhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewItem;
  yynewItem = &yystack->yynextFree->yyoption;
  yystack->yyspaceLeft -= 1;
  yystack->yynextFree += 1;
  yynewItem->yyisState = yyfalse;
  yynewItem->yystate = yyrhs;
  yynewItem->yyrule = yyrule;
  yynewItem->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewItem;
  if (yystack->yyspaceLeft < YYHEADROOM)
    yyexpandGLRStack (yystack);
}

				/* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack. */
static void
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) malloc (16 * sizeof (yyset->yystates[0]));
  yyset->yystates[0] = NULL;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  free (yyset->yystates);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE. */
static void
yyinitGLRStack (yyGLRStack* yystack, size_t yysize)
{
  yystack->yyerrflag = 0;
  yystack->yyerrState = 0;
  yynerrs = 0;
  yystack->yyspaceLeft = yysize;
  yystack->yynextFree = yystack->yyitems =
    (yyGLRStackItem*) malloc (yysize * sizeof (yystack->yynextFree[0]));
  yystack->yysplitPoint = NULL;
  yystack->yylastDeleted = NULL;
  yyinitStateSet (&yystack->yytops);
}

#define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation. */
static void
yyexpandGLRStack (yyGLRStack* yystack)
{
#if YYSTACKEXPANDABLE
  yyGLRStack yynewStack;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yysize, yynewSize;
  size_t yyn;
  yysize = yystack->yynextFree - yystack->yyitems;
  if (yysize >= YYMAXDEPTH)
    yyFail (yystack, "parsing stack overflow (%d items)", yysize);
  yynewSize = 2*yysize;
  if (yynewSize > YYMAXDEPTH)
    yynewSize = YYMAXDEPTH;
  yyinitGLRStack (&yynewStack, yynewSize);
  for (yyp0 = yystack->yyitems, yyp1 = yynewStack.yyitems, yyn = yysize;
       yyn > 0;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(bool*) yyp0)
	{
	  yyGLRState* yys0 = &yyp0->yystate;
	  yyGLRState* yys1 = &yyp1->yystate;
	  if (yys0->yypred != NULL)
	    yys1->yypred =
	      YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
	  if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != NULL)
	    yys1->yysemantics.yyfirstVal =
	      YYRELOC(yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
	}
      else
	{
	  yySemanticOption* yyv0 = &yyp0->yyoption;
	  yySemanticOption* yyv1 = &yyp1->yyoption;
	  if (yyv0->yystate != NULL)
	    yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
	  if (yyv0->yynext != NULL)
	    yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
	}
    }
  if (yystack->yysplitPoint != NULL)
    yystack->yysplitPoint = YYRELOC (yystack->yyitems, yynewStack.yyitems,
				 yystack->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystack->yytops.yysize; yyn += 1)
    if (yystack->yytops.yystates[yyn] != NULL)
      yystack->yytops.yystates[yyn] =
	YYRELOC (yystack->yyitems, yynewStack.yyitems,
		 yystack->yytops.yystates[yyn], yystate);
  free (yystack->yyitems);
  yystack->yyitems = yynewStack.yyitems;
  yystack->yynextFree = yynewStack.yynextFree + yysize;
  yystack->yyspaceLeft = yynewStack.yyspaceLeft - yysize;

#else

  yyFail (yystack, "parsing stack overflow (%d items)", yysize);

#endif
}

static void
yyfreeGLRStack (yyGLRStack* yystack)
{
  free (yystack->yyitems);
  yyfreeStateSet (&yystack->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S. */
static inline void
yyupdateSplit (yyGLRStack* yystack, yyGLRState* yys)
{
  if (yystack->yysplitPoint != NULL && yystack->yysplitPoint > yys)
    yystack->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK. */
static inline void
yymarkStackDeleted (yyGLRStack* yystack, int yyk)
{
  if (yystack->yytops.yystates[yyk] != NULL)
    yystack->yylastDeleted = yystack->yytops.yystates[yyk];
  yystack->yytops.yystates[yyk] = NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted. */
static void
yyundeleteLastStack (yyGLRStack* yystack)
{
  if (yystack->yylastDeleted == NULL || yystack->yytops.yysize != 0)
    return;
  yystack->yytops.yystates[0] = yystack->yylastDeleted;
  yystack->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystack->yylastDeleted = NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystack)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystack->yytops.yysize)
    {
      if (yystack->yytops.yystates[yyi] == NULL)
	{
	  if (yyi == yyj)
	    YYDPRINTF ((stderr, "Removing dead stacks.\n"));
	  yystack->yytops.yysize -= 1;
	}
      else
	{
	  yystack->yytops.yystates[yyj] = yystack->yytops.yystates[yyi];
	  if (yyj != yyi)
	    YYDPRINTF ((stderr, "Rename stack %d -> %d.\n", yyi, yyj));
	  yyj += 1;
	}
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL. */
static inline void
yyglrShift (yyGLRStack* yystack, int yyk, yyStateNum yylrState, size_t yyposn,
	    YYSTYPE yysval, YYLTYPE* yylocp)
{
  yyGLRStackItem* yynewItem;

  yynewItem = yystack->yynextFree;
  yystack->yynextFree += 1;
  yystack->yyspaceLeft -= 1;
  yynewItem->yystate.yyisState = yytrue;
  yynewItem->yystate.yylrState = yylrState;
  yynewItem->yystate.yyposn = yyposn;
  yynewItem->yystate.yyresolved = yytrue;
  yynewItem->yystate.yypred = yystack->yytops.yystates[yyk];
  yystack->yytops.yystates[yyk] = &yynewItem->yystate;
  yynewItem->yystate.yysemantics.yysval = yysval;
  yynewItem->yystate.yyloc = *yylocp;
  if (yystack->yyspaceLeft < YYHEADROOM)
    yyexpandGLRStack (yystack);
}

/** Shift to a new state on stack #K of STACK, to a new state
 *  corresponding to LR state LRSTATE, at input position POSN, with
 * the (unresolved) semantic value of RHS under the action for RULE. */
static inline void
yyglrShiftDefer (yyGLRStack* yystack, int yyk, yyStateNum yylrState,
		 size_t yyposn, yyGLRState* yyrhs, yyRuleNum yyrule)
{
  yyGLRStackItem* yynewItem;

  yynewItem = yystack->yynextFree;
  yynewItem->yystate.yyisState = yytrue;
  yynewItem->yystate.yylrState = yylrState;
  yynewItem->yystate.yyposn = yyposn;
  yynewItem->yystate.yyresolved = yyfalse;
  yynewItem->yystate.yypred = yystack->yytops.yystates[yyk];
  yynewItem->yystate.yysemantics.yyfirstVal = NULL;
  yystack->yytops.yystates[yyk] = &yynewItem->yystate;
  yystack->yynextFree += 1;
  yystack->yyspaceLeft -= 1;
  yyaddDeferredAction (yystack, &yynewItem->yystate, yyrhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved. Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction. */
static inline int
yydoAction (yyGLRStack* yystack, int yyk, yyRuleNum yyrule,
 	    YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystack->yysplitPoint == NULL)
    {
      /* Standard special case: single stack. */
      yyGLRStackItem* yyrhs = (yyGLRStackItem*) yystack->yytops.yystates[yyk];
      assert (yyk == 0);
      yystack->yynextFree -= yynrhs;
      yystack->yyspaceLeft += yynrhs;
      yystack->yytops.yystates[0] = & yystack->yynextFree[-1].yystate;
      if (yynrhs == 0)
	{
	  *yyvalp = yyval_default;
	  *yylocp = yyloc_default;
	}
      else
	{
	  *yyvalp = yyrhs[1-yynrhs].yystate.yysemantics.yysval;
	  *yylocp = yyrhs[1-yynrhs].yystate.yyloc;
	}
      return yyuserAction (yyrule, yynrhs, yyrhs, yyvalp, yylocp, yystack);
    }
  else
    {
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS];
      for (yyi = yynrhs-1, yys = yystack->yytops.yystates[yyk]; yyi >= 0;
	   yyi -= 1, yys = yys->yypred)
	{
	  assert (yys->yypred != NULL);
	  yyrhsVals[yyi].yystate.yyresolved = yytrue;
	  yyrhsVals[yyi].yystate.yysemantics.yysval = yys->yysemantics.yysval;
	  yyrhsVals[yyi].yystate.yyloc = yys->yyloc;
	}
      yyupdateSplit (yystack, yys);
      yystack->yytops.yystates[yyk] = yys;
      if (yynrhs == 0)
	{
	  *yyvalp = yyval_default;
	  *yylocp = yyloc_default;
	}
      else
	{
	  *yyvalp = yyrhsVals[0].yystate.yysemantics.yysval;
	  *yylocp = yyrhsVals[0].yystate.yyloc;
	}
      return yyuserAction (yyrule, yynrhs, yyrhsVals + (yynrhs-1),
			   yyvalp, yylocp, yystack);
    }
}

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK. In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystack, size_t yyk, yyRuleNum yyrule,
             bool yyforceEval)
{
  size_t yyposn = yystack->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystack->yysplitPoint == NULL)
    {
      YYSTYPE yysval;
      YYLTYPE yyloc;

      YYCHK (yydoAction (yystack, yyk, yyrule, &yysval, &yyloc));
      yyglrShift (yystack, yyk,
		  yyLRgotoState (yystack->yytops.yystates[yyk]->yylrState,
				 yylhsNonterm (yyrule)),
		yyposn, yysval, &yyloc);
      YYDPRINTF ((stderr, "Reduced stack %d by rule #%d. Now in state %d.\n",
		  yyk, yyrule-1, yystack->yytops.yystates[yyk]->yylrState));
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystack->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystack->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
	   yyn > 0; yyn -= 1)
	{
	  yys = yys->yypred;
	  assert (yys != NULL);
	}
      yyupdateSplit (yystack, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
		  "Reduced stack %d by rule #%d; action deferred. "
		  "Now in state %d.\n",
		  yyk, yyrule-1, yynewLRState));
      for (yyi = 0; yyi < yystack->yytops.yysize; yyi += 1)
	if (yyi != yyk && yystack->yytops.yystates[yyi] != NULL)
	  {
	    yyGLRState* yyp, *yysplit = yystack->yysplitPoint;
	    yyp = yystack->yytops.yystates[yyi];
	    while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
	      {
		if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
		  {
		    yyaddDeferredAction (yystack, yyp, yys0, yyrule);
		    yymarkStackDeleted (yystack, yyk);
		    YYDPRINTF ((stderr, "Merging stack %d into stack %d.\n",
				yyk, yyi));
		    return 0;
		  }
		yyp = yyp->yypred;
	      }
	  }
      yystack->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystack, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return 0;
}

static int
yysplitStack (yyGLRStack* yystack, int yyk)
{
  if (yystack->yysplitPoint == NULL)
    {
      assert (yyk == 0);
      yystack->yysplitPoint = yystack->yytops.yystates[yyk];
    }
  if (yystack->yytops.yysize >= yystack->yytops.yycapacity)
    {
      yystack->yytops.yycapacity *= 2;
      yystack->yytops.yystates =
	(yyGLRState**) realloc (yystack->yytops.yystates,
				yystack->yytops.yycapacity
				* sizeof (yyGLRState*));
    }
  yystack->yytops.yystates[yystack->yytops.yysize]
    = yystack->yytops.yystates[yyk];
  yystack->yytops.yysize += 1;
  return yystack->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols. */
static bool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
	   yyn = yyrhsLength (yyy0->yyrule);
	   yyn > 0;
	   yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
	if (yys0->yyposn != yys1->yyposn)
	  return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), (destructively) merge the
 *  alternative semantic values for the RHS-symbols of Y1 into the
 *  corresponding semantic value sets of the symbols of Y0. */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    if (yys0 == yys1)
      break;
    else if (! yys0->yyresolved && ! yys1->yyresolved)
      {
	yySemanticOption* yyz;
	for (yyz = yys0->yysemantics.yyfirstVal; yyz->yynext != NULL;
	     yyz = yyz->yynext)
	  ;
	yyz->yynext = yys1->yysemantics.yyfirstVal;
      }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred. */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
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
  if (p0 > p1)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yySemanticOption* yyoptionList,
				   yyGLRStack* yystack, YYSTYPE* yyvalp,
				   YYLTYPE* yylocp);

static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn, yyGLRStack* yystack)
{
  YYRESULTTAG yyflag;
  if (yyn > 0)
    {
      assert (yys->yypred != NULL);
      yyflag = yyresolveStates (yys->yypred, yyn-1, yystack);
      if (yyflag != yyok)
	return yyflag;
      if (! yys->yyresolved)
	{
	  yyflag = yyresolveValue (yys->yysemantics.yyfirstVal, yystack,
			       &yys->yysemantics.yysval, &yys->yyloc);
	  if (yyflag != yyok)
	    return yyflag;
	  yys->yyresolved = yytrue;
	}
    }
  return yyok;
}

static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystack,
	         YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS];
  int yynrhs, yyi;
  yyGLRState* yys;

  yynrhs = yyrhsLength (yyopt->yyrule);
  YYCHK (yyresolveStates (yyopt->yystate, yynrhs, yystack));
  for (yyi = yynrhs-1, yys = yyopt->yystate; yyi >= 0;
       yyi -= 1, yys = yys->yypred)
    {
      assert (yys->yypred != NULL);
      yyrhsVals[yyi].yystate.yyresolved = yytrue;
      yyrhsVals[yyi].yystate.yysemantics.yysval = yys->yysemantics.yysval;
      yyrhsVals[yyi].yystate.yyloc = yys->yyloc;
    }
  return yyuserAction (yyopt->yyrule, yynrhs, yyrhsVals + (yynrhs-1),
		       yyvalp, yylocp, yystack);
}

#if YYDEBUG
static yyGLRState YYLEFTMOST_STATE = { 0, NULL, -1, 0, { NULL } };

static void yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[YYMAXRHS];

  for (yyi = yynrhs, yys = yyx->yystate; yyi > 0; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == NULL)
    yystates[0] = &YYLEFTMOST_STATE;
  else
    yystates[0] = yys;

  if (yys->yyposn+1 > yyx->yystate->yyposn)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %d .. %d>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule, yys->yyposn+1, yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
	{
	  if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
	    YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
	  else
	    YYFPRINTF (stderr, "%*s%s <tokens %d .. %d>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
		       yystates[yyi-1]->yyposn+1, yystates[yyi]->yyposn);
	}
      else
	yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

static void
yyreportAmbiguity (yySemanticOption* yyx0, yySemanticOption* yyx1,
		   yyGLRStack* yystack)
{
  /* `Unused' warnings.  */
  (void) yyx0;
  (void) yyx1;

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif
  yyFail (yystack, "ambiguity detected");
}


/** Resolve the ambiguity represented by OPTIONLIST, perform the indicated
 *  actions, and return the result. */
static YYRESULTTAG
yyresolveValue (yySemanticOption* yyoptionList, yyGLRStack* yystack,
		YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yySemanticOption* yybest;
  yySemanticOption* yyp;
  int yymerge;

  yybest = yyoptionList;
  yymerge = 0;
  for (yyp = yyoptionList->yynext; yyp != NULL; yyp = yyp->yynext)
    {
      if (yyidenticalOptions (yybest, yyp))
	yymergeOptionSets (yybest, yyp);
      else
	switch (yypreference (yybest, yyp))
	  {
	  case 0:
	    yyreportAmbiguity (yybest, yyp, yystack);
	    break;
	  case 1:
	    yymerge = 1;
	    break;
	  case 2:
	    break;
	  case 3:
	    yybest = yyp;
	    yymerge = 0;
	    break;
	  }
    }

  if (yymerge)
    {
      int yyprec = yydprec[yybest->yyrule];
      YYCHK (yyresolveAction (yybest, yystack, yyvalp, yylocp));
      for (yyp = yybest->yynext; yyp != NULL; yyp = yyp->yynext)
	{
	  if (yyprec == yydprec[yyp->yyrule])
	    {
	      YYSTYPE yyval1;
	      YYLTYPE yydummy;
	      YYCHK (yyresolveAction (yyp, yystack, &yyval1, &yydummy));
	      *yyvalp = yyuserMerge (yymerger[yyp->yyrule], yyvalp, &yyval1);
	    }
	}
      return yyok;
    }
  else
    return yyresolveAction (yybest, yystack, yyvalp, yylocp);
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystack)
{
  if (yystack->yysplitPoint != NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystack->yytops.yystates[0];
	   yys != yystack->yysplitPoint;
	   yys = yys->yypred, yyn += 1)
	;
      YYCHK (yyresolveStates (yystack->yytops.yystates[0], yyn, yystack));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystack)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystack->yytops.yysize != 1 || yystack->yysplitPoint == NULL)
    return;

  for (yyp = yystack->yytops.yystates[0], yyq = yyp->yypred, yyr = NULL;
       yyp != yystack->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystack->yyspaceLeft += yystack->yynextFree - yystack->yyitems;
  yystack->yynextFree = ((yyGLRStackItem*) yystack->yysplitPoint) + 1;
  yystack->yyspaceLeft -= yystack->yynextFree - yystack->yyitems;
  yystack->yysplitPoint = NULL;
  yystack->yylastDeleted = NULL;

  while (yyr != NULL)
    {
      yystack->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystack->yynextFree->yystate.yypred = & yystack->yynextFree[-1].yystate;
      yystack->yytops.yystates[0] = &yystack->yynextFree->yystate;
      yystack->yynextFree += 1;
      yystack->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystack, int yyk,
	           size_t yyposn, YYSTYPE* yylvalp, YYLTYPE* yyllocp)
{
  int yyaction;
  const short* yyconflicts;
  yyRuleNum yyrule;
  yySymbol* const yytokenp = yystack->yytokenp;

  while (yystack->yytops.yystates[yyk] != NULL)
    {
      yyStateNum yystate = yystack->yytops.yystates[yyk]->yylrState;

      assert (yystate != YYFINAL);
      if (yyisDefaultedState (yystate))
	{
	  yyrule = yydefaultAction (yystate);
	  if (yyrule == 0)
	    {
	      YYDPRINTF ((stderr, "Stack %d dies.\n", yyk));
	      yymarkStackDeleted (yystack, yyk);
	      return yyok;
	    }
	  YYCHK (yyglrReduce (yystack, yyk, yyrule, yyfalse));
	}
      else
	{
	  if (*yytokenp == YYEMPTY)
	    {
	      yychar = YYLEX;
	      *yytokenp = YYTRANSLATE(yychar);
	      YYDPRINTF ((stderr, "Read token %s\n", yytokenName (*yytokenp)));
	    }
	  yygetLRActions (yystate, *yytokenp, &yyaction, &yyconflicts);

	  while (*yyconflicts != 0)
	    {
	      int yynewStack = yysplitStack (yystack, yyk);
	      YYDPRINTF ((stderr, "Splitting off stack %d from %d.\n",
			  yynewStack, yyk));
	      YYCHK (yyglrReduce (yystack, yynewStack, *yyconflicts, yyfalse));
	      YYCHK (yyprocessOneStack (yystack, yynewStack, yyposn,
					yylvalp, yyllocp));
	      yyconflicts += 1;
	    }

	  if (yyisShiftAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "Shifted token %s on stack %d, ",
			  yytokenName (*yytokenp), yyk));
	      yyglrShift (yystack, yyk, yyaction, yyposn+1, *yylvalp, yyllocp);
	      YYDPRINTF ((stderr, "which is now in state #%d\n",
			  yystack->yytops.yystates[yyk]->yylrState));
	      break;
	    }
	  else if (yyisErrorAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "Stack %d dies.\n", yyk));
	      yymarkStackDeleted (yystack, yyk);
	      break;
	    }
	  else
	    YYCHK (yyglrReduce (yystack, yyk, -yyaction, yyfalse));
	}
    }
  return yyok;
}

static void
yyreportParseError (yyGLRStack* yystack, YYSTYPE* yylvalp, YYLTYPE* yyllocp)
{
  /* `Unused' warnings. */
  (void) yylvalp;
  (void) yyllocp;

  if (yystack->yyerrState == 0)
    {
#if YYERROR_VERBOSE
      yySymbol* const yytokenp = yystack->yytokenp;
      int yyn, yyx, yycount;
      size_t yysize;
      const char* yyprefix;
      char* yyp;
      char* yymsg;
      yyn = yypact[yystack->yytops.yystates[0]->yylrState];
      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  yysize = sizeof ("parse error, unexpected ")
	    + strlen (yytokenName (*yytokenp));
	  yyprefix = ", expecting ";
	  for (yyx = yyn < 0 ? -yyn : 0; yyx < yytname_size && yycount <= 5;
	       yyx += 1)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += strlen (yytokenName (yyx)) + strlen (yyprefix),
		yycount += 1, yyprefix = " or ";
	  yymsg = yyp = (char*) malloc (yysize);
	  yyp += sprintf (yyp, "parse error, unexpected %s",
			  yytokenName (*yytokenp));
	  if (yycount < 5)
	    {
	      yyprefix = ", expecting ";
	      for (yyx = yyn < 0 ? -yyn : 0; yyx < yytname_size; yyx += 1)
		if (yycheck[yyx + yyn] == yyx)
		  {
		    yyp += sprintf (yyp, "%s%s", yyprefix, yytokenName (yyx));
		    yyprefix = " or ";
		  }
	    }
	  yyerror (yymsg);
	  free (yymsg);
	}
      else
#endif
	yyerror ("parse error");
      yynerrs += 1;
    }
}

/* Recover from a syntax error on STACK, assuming that TOKENP,
   YYLVALP, and YYLLOCP point to the syntactic category, semantic
   value, and location of the lookahead.
   NOTE: This uses the panic-mode recovery algorithm described in the
   Bison documentation, which differs from what is in bison.simple.
   Specifically, this routine performs no reductions before shifting
   the error token. */
static void
yyrecoverParseError (yyGLRStack* yystack, YYSTYPE* yylvalp, YYLTYPE* yyllocp)
{
  yySymbol* const yytokenp = yystack->yytokenp;
  size_t yyk;
  int yyj;

  if (yystack->yyerrState == 0)
    yystack->yyerrState = 3;
  else if (yystack->yyerrState == 3)
    {
      /* We just shifted the error token and (perhaps) took some
	 reductions. Skip tokens until we can proceed. */
      do {
	if (*yytokenp == YYEOF)
	  yyFail (yystack, NULL);
	if (*yytokenp != YYEMPTY)
	  YYDPRINTF ((stderr, "Discarding token %s\n",
		      yytokenName (*yytokenp)));
	yychar = YYLEX;
	*yytokenp = YYTRANSLATE (yychar);
	YYDPRINTF ((stderr, "Read token %s\n", yytokenName (*yytokenp)));
	yyj = yypact[yystack->yytops.yystates[0]->yylrState];
	if (yyj == YYPACT_NINF)
	  /* Something's not right; we shouldn't be here */
	  yyFail (yystack, NULL);
	yyj += *yytokenp;
	if (yyj < 0 || yyj > YYLAST || yycheck[yyj] != *yytokenp)
	  {
	    if (yydefact[yystack->yytops.yystates[0]->yylrState] != 0)
	      return;
	  }
	else if (yytable[yyj] != 0 && yytable[yyj] != YYTABLE_NINF)
	  return;
      } while (yytrue);
    }

  /* Reduce to one stack */
  for (yyk = 0; yyk < yystack->yytops.yysize; yyk += 1)
    if (yystack->yytops.yystates[yyk] != NULL)
      break;
  if (yyk >= yystack->yytops.yysize)
    yyFail (yystack, NULL);
  for (yyk += 1; yyk < yystack->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystack, yyk);
  yyremoveDeletes (yystack);
  yycompressStack (yystack);

  /* Now pop stack until we find a state that shifts the error token. */
  while (yystack->yytops.yystates[0] != NULL)
    {
      yyj = yypact[yystack->yytops.yystates[0]->yylrState] + YYTERROR;
      if (yyj != YYPACT_NINF + YYTERROR && yyj >= 0 && yyj <= YYLAST &&
	  yycheck[yyj] == YYTERROR && yyisShiftAction (yytable[yyj]))
	{
	  yyglrShift (yystack, 0, yytable[yyj],
		      yystack->yytops.yystates[0]->yyposn, *yylvalp, yyllocp);
	  break;
	}
      yystack->yytops.yystates[0] = yystack->yytops.yystates[0]->yypred;
      yystack->yynextFree -= 1;
      yystack->yyspaceLeft += 1;
    }
  if (yystack->yytops.yystates[0] == NULL)
    yyFail (yystack, NULL);
}

#define YYCHK1(YYE)							     \
  do {									     \
    switch (YYE) {							     \
    default:								     \
      break;								     \
    case yyabort:							     \
      yystack.yyerrflag = 1;						     \
      goto yyDone;							     \
    case yyaccept:							     \
      yystack.yyerrflag = 0;						     \
      goto yyDone;							     \
    case yyerr:								     \
      goto yyuser_error;						     \
    }									     \
  } while (0)

int
yyparse (YYPARSE_PARAM_ARG)
{
  yySymbol yytoken;
  yyGLRStack yystack;
  size_t yyposn;
]b4_pure_if(
[
  YYSTYPE yylval;
  YYLTYPE yylloc;
  #undef yychar
  #define yychar (yystack.yyrawchar)
])[

  YYSTYPE* const yylvalp = &yylval;
  YYLTYPE* const yyllocp = &yylloc;

  yyinitGLRStack (&yystack, YYINITDEPTH);
  yystack.yytokenp = &yytoken;

  if (setjmp (yystack.yyexception_buffer) != 0)
    goto yyDone;

  yyglrShift (&yystack, 0, 0, 0, yyval_default, &yyloc_default);
  yytoken = YYEMPTY;
  yyposn = 0;

  while (yytrue)
    {
      /* For efficiency, we have two loops, of which the first of which
       * is specialized to deterministic operation (single stack, no
       * potential ambiguity). */

      /* Standard mode */
      while (yytrue)
	{
	  yyRuleNum yyrule;
	  int yyaction;
	  const short* yyconflicts;

	  yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
	  if (yystate == YYFINAL)
	    goto yyDone;
	  if (yyisDefaultedState (yystate))
	    {
	      yyrule = yydefaultAction (yystate);
	      if (yyrule == 0)
		{
		  yyreportParseError (&yystack, yylvalp, yyllocp);
		  goto yyuser_error;
		}
	      YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue));
	    }
	  else
	    {
	      if (yytoken == YYEMPTY)
		{
		  yychar = YYLEX;
		  yytoken = YYTRANSLATE (yychar);
		  YYDPRINTF ((stderr, "Read token %s\n",
			      yytokenName (yytoken)));
		}
	      yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
	      if (*yyconflicts != 0)
		break;
	      if (yyisShiftAction (yyaction))
		{
		  YYDPRINTF ((stderr, "Shifted token %s. ",
			      yytokenName (yytoken)));
		  if (yytoken != YYEOF)
		    yytoken = YYEMPTY;
		  yyposn += 1;
		  yyglrShift (&yystack, 0, yyaction, yyposn, yylval, yyllocp);
		  if (yystack.yyerrState > 0)
		    yystack.yyerrState -= 1;
		  YYDPRINTF ((stderr, "Now in state #%d\n",
			      yystack.yytops.yystates[0]->yylrState));
		}
	      else if (yyisErrorAction (yyaction))
		{
		  yyreportParseError (&yystack, yylvalp, yyllocp);
		  goto yyuser_error;
		}
	      else
		YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue));
	    }
	}

      while (yytrue)
	{
	  int yys;
	  int yyn = yystack.yytops.yysize;
	  for (yys = 0; yys < yyn; yys += 1)
	    YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn,
				       yylvalp, yyllocp));
	  yytoken = YYEMPTY;
	  yyposn += 1;
	  yyremoveDeletes (&yystack);
	  if (yystack.yytops.yysize == 0)
	    {
	      yyundeleteLastStack (&yystack);
	      if (yystack.yytops.yysize == 0)
		yyFail (&yystack, "parse error");
	      YYCHK1 (yyresolveStack (&yystack));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yyreportParseError (&yystack, yylvalp, yyllocp);
	      goto yyuser_error;
	    }
	  else if (yystack.yytops.yysize == 1)
	    {
	      YYCHK1 (yyresolveStack (&yystack));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yycompressStack (&yystack);
	      break;
	    }
	}
      continue;
    yyuser_error:
      yyrecoverParseError (&yystack, yylvalp, yyllocp);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }
 yyDone:
  ;

  yyfreeGLRStack (&yystack);
  return yystack.yyerrflag;
}

/* DEBUGGING ONLY */
static void yypstack (yyGLRStack* yystack, int yyk) ATTRIBUTE_UNUSED;
static void yypdumpstack (yyGLRStack* yystack) ATTRIBUTE_UNUSED;

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      fprintf (stderr, " -> ");
    }
  fprintf (stderr, "%d@%lu", yys->yylrState, (unsigned long) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == NULL)
    fprintf (stderr, "<null>");
  else
    yy_yypstack (yyst);
  fprintf (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystack, int yyk)
{
  yypstates (yystack->yytops.yystates[yyk]);
}

#define YYINDEX(YYX) 							     \
    ((YYX) == NULL ? -1 : (yyGLRStackItem*) (YYX) - yystack->yyitems)


static void
yypdumpstack (yyGLRStack* yystack)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystack->yyitems; yyp < yystack->yynextFree; yyp += 1)
    {
      fprintf (stderr, "%3lu. ", (unsigned long) (yyp - yystack->yyitems));
      if (*(bool*) yyp)
	{
	  fprintf (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
		   yyp->yystate.yyresolved, yyp->yystate.yylrState,
		   (unsigned long) yyp->yystate.yyposn,
		   (long) YYINDEX (yyp->yystate.yypred));
	  if (! yyp->yystate.yyresolved)
	    fprintf (stderr, ", firstVal: %ld",
		     (long) YYINDEX (yyp->yystate.yysemantics.yyfirstVal));
	}
      else
	{
	  fprintf (stderr, "Option. rule: %d, state: %ld, next: %ld",
		   yyp->yyoption.yyrule,
		   (long) YYINDEX (yyp->yyoption.yystate),
		   (long) YYINDEX (yyp->yyoption.yynext));
	}
      fprintf (stderr, "\n");
    }
  fprintf (stderr, "Tops:");
  for (yyi = 0; yyi < yystack->yytops.yysize; yyi += 1)
    fprintf (stderr, "%lu: %ld; ", (unsigned long) yyi,
	     (long) YYINDEX (yystack->yytops.yystates[yyi]));
  fprintf (stderr, "\n");
}

]

b4_epilogue
m4_if(b4_defines_flag, 0, [],
[#output "b4_output_header_name"
b4_copyright([Skeleton parser for GLR parsing with Bison], [2002])
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
  int yyfirst_line;
  int yyfirst_column;
  int yylast_line;
  int yylast_column;
} yyltype;
# define YYLTYPE yyltype
#endif

m4_if(b4_pure, [0],
[extern YYLTYPE b4_prefix[]lloc;])
])
#endif /* not b4_header_guard */
])
