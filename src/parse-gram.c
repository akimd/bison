/* A Bison parser, made from parse-gram.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse gram_parse
#define yylex gram_lex
#define yyerror gram_error
#define yylval gram_lval
#define yychar gram_char
#define yydebug gram_debug
#define yynerrs gram_nerrs
#define YYLSP_NEEDED 1

# define	GRAM_EOF	0
# define	STRING	257
# define	CHARACTER	258
# define	INT	259
# define	PERCENT_TOKEN	260
# define	PERCENT_NTERM	261
# define	PERCENT_TYPE	262
# define	PERCENT_UNION	263
# define	PERCENT_EXPECT	264
# define	PERCENT_START	265
# define	PERCENT_LEFT	266
# define	PERCENT_RIGHT	267
# define	PERCENT_NONASSOC	268
# define	PERCENT_PREC	269
# define	PERCENT_VERBOSE	270
# define	PERCENT_ERROR_VERBOSE	271
# define	PERCENT_OUTPUT	272
# define	PERCENT_FILE_PREFIX	273
# define	PERCENT_NAME_PREFIX	274
# define	PERCENT_DEFINE	275
# define	PERCENT_PURE_PARSER	276
# define	PERCENT_DEFINES	277
# define	PERCENT_YACC	278
# define	PERCENT_DEBUG	279
# define	PERCENT_LOCATIONS	280
# define	PERCENT_NO_LINES	281
# define	PERCENT_SKELETON	282
# define	PERCENT_TOKEN_TABLE	283
# define	TYPE	284
# define	EQUAL	285
# define	SEMICOLON	286
# define	COLON	287
# define	PIPE	288
# define	ID	289
# define	PERCENT_PERCENT	290
# define	PROLOGUE	291
# define	EPILOGUE	292
# define	BRACED_CODE	293

#line 31 "parse-gram.y"

#include "system.h"
#include "muscle_tab.h"
#include "files.h"
#include "getargs.h"
#include "output.h"
#include "gram.h"
#include "reader.h"
#include "conflicts.h"

#define YYERROR_VERBOSE 1

/* Pass the control structure to YYPARSE and YYLEX. */
#define YYPARSE_PARAM gram_control
#define YYLEX_PARAM gram_control
/* YYPARSE receives GRAM_CONTROL as a void *.  Provide a
   correctly typed access to it.  */
#define yycontrol ((gram_control_t *) gram_control)

/* Request detailed parse error messages, and pass them to
   GRAM_ERROR. */
#undef  yyerror
#define yyerror(Msg) \
        gram_error (yycontrol, &yylloc, Msg)

/* When debugging our pure parser, we want to see values and locations
   of the tokens.  */
#define YYPRINT(File, Type, Value) \
        yyprint (File, &yylloc, Type, &Value)
static void yyprint (FILE *file, const yyltype *loc,
                     int type, const yystype *value);

symbol_class current_class = unknown_sym;
char *current_type = 0;
symbol_t *current_lhs;
associativity current_assoc;
int current_prec = 0;

#line 72 "parse-gram.y"
#ifndef YYSTYPE
typedef union
{
  symbol_t *symbol;
  int integer;
  char *string;
  associativity assoc;
} yystype;
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

# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

#ifndef YYDEBUG
# define YYDEBUG 1
#endif



#define	YYFINAL		94
#define	YYFLAG		-32768
#define	YYNTBASE	41

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 293 ? yytranslate[x] : 68)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     7,     8,    11,    13,    15,    17,    21,
      23,    25,    28,    32,    34,    38,    40,    44,    46,    49,
      51,    53,    55,    57,    58,    62,    65,    66,    70,    71,
      76,    80,    81,    86,    88,    90,    92,    93,    95,    97,
     100,   102,   105,   107,   109,   112,   115,   119,   121,   124,
     126,   129,   130,   136,   138,   142,   143,   146,   149,   153,
     155,   157,   159,   161,   163,   165,   166,   169,   170
};
static const short yyrhs[] =
{
      -1,    42,    43,    37,    57,    66,     0,     0,    43,    44,
       0,    45,     0,    38,     0,    26,     0,    22,    65,    65,
       0,    24,     0,    18,     0,    11,     6,     0,    20,    32,
      65,     0,    27,     0,    21,    32,    65,     0,    28,     0,
      19,    32,    65,     0,    23,     0,    29,    65,     0,    30,
       0,    17,     0,    25,     0,    49,     0,     0,     8,    46,
      56,     0,    12,    62,     0,     0,     7,    47,    56,     0,
       0,     9,    31,    48,    53,     0,    10,    40,    67,     0,
       0,    51,    52,    50,    54,     0,    13,     0,    14,     0,
      15,     0,     0,    31,     0,    36,     0,    53,    36,     0,
      62,     0,    54,    62,     0,    31,     0,    36,     0,    36,
       6,     0,    36,    64,     0,    36,     6,    64,     0,    55,
       0,    56,    55,     0,    58,     0,    57,    58,     0,     0,
      36,    34,    59,    60,    33,     0,    61,     0,    60,    35,
      61,     0,     0,    61,    62,     0,    61,    63,     0,    61,
      16,    62,     0,    36,     0,    64,     0,     5,     0,    40,
       0,     4,     0,     4,     0,     0,    37,    39,     0,     0,
      33,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   131,   131,   139,   141,   144,   146,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   167,   169,   169,   174,   178,   178,   183,   183,
     187,   195,   195,   202,   204,   205,   208,   210,   214,   216,
     220,   226,   235,   240,   245,   251,   257,   267,   270,   274,
     276,   279,   279,   284,   286,   289,   292,   294,   296,   300,
     302,   303,   306,   312,   321,   329,   334,   340,   342
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "\"end of string\"", "STRING", "CHARACTER", 
  "INT", "\"%token\"", "\"%nterm\"", "\"%type\"", "\"%union\"", 
  "\"%expect\"", "\"%start\"", "\"%left\"", "\"%right\"", "\"%nonassoc\"", 
  "\"%prec\"", "\"%verbose\"", "\"%error-verbose\"", "\"%output\"", 
  "\"%file-prefix\"", "\"%name-prefix\"", "\"%define\"", 
  "\"%pure-parser\"", "\"%defines\"", "\"%yacc\"", "\"%debug\"", 
  "\"%locations\"", "\"%no-lines\"", "\"%skeleton\"", "\"%token-table\"", 
  "TYPE", "\"=\"", "\";\"", "\":\"", "\"|\"", "\"identifier\"", "\"%%\"", 
  "PROLOGUE", "EPILOGUE", "BRACED_CODE", "input", "@1", "directives", 
  "directive", "grammar_directives", "@2", "@3", "@4", 
  "precedence_directives", "@5", "precedence_directive", "type.opt", 
  "nterms_to_type.1", "terms_to_prec.1", "symbol_def", "symbol_defs.1", 
  "gram", "rules", "@6", "rhses.1", "rhs", "symbol", "action", 
  "string_as_id", "string_content", "epilogue.opt", "semi_colon_opt", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    42,    41,    43,    43,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    45,    46,    45,    45,    47,    45,    48,    45,
      45,    50,    49,    51,    51,    51,    52,    52,    53,    53,
      54,    54,    55,    55,    55,    55,    55,    56,    56,    57,
      57,    59,    58,    60,    60,    61,    61,    61,    61,    62,
      62,    62,    63,    64,    65,    66,    66,    67,    67
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     5,     0,     2,     1,     1,     1,     3,     1,
       1,     2,     3,     1,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     0,     3,     2,     0,     3,     0,     4,
       3,     0,     4,     1,     1,     1,     0,     1,     1,     2,
       1,     2,     1,     1,     2,     2,     3,     1,     2,     1,
       2,     0,     5,     1,     3,     0,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     0,     2,     0,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     3,     0,    26,    23,     0,     0,     0,     0,    33,
      34,    35,    20,    10,     0,     0,     0,     0,    17,     9,
      21,     7,    13,    15,     0,    19,     0,     6,     4,     5,
      22,    36,     0,     0,    28,    67,    11,    63,    61,    59,
      25,    60,     0,     0,     0,    64,     0,    18,     0,    65,
      49,    37,    31,    42,    43,    47,    27,    24,     0,    68,
      30,    16,    12,    14,     8,    51,     0,    50,     2,     0,
      44,    45,    48,    38,    29,    55,    66,    32,    40,    46,
      39,     0,    53,    41,    52,    55,     0,    62,    56,    57,
      54,    58,     0,     0,     0
};

static const short yydefgoto[] =
{
      92,     1,     2,    28,    29,    33,    32,    58,    30,    69,
      31,    52,    74,    77,    55,    56,    49,    50,    75,    81,
      82,    88,    89,    41,    46,    68,    60
};

static const short yypact[] =
{
  -32768,-32768,    72,-32768,-32768,   -22,   -29,     8,    -1,-32768,
  -32768,-32768,-32768,-32768,   -16,   -14,   -11,    22,-32768,-32768,
  -32768,-32768,-32768,-32768,    22,-32768,    -6,-32768,-32768,-32768,
  -32768,     0,   -24,   -24,-32768,     1,-32768,-32768,-32768,-32768,
  -32768,-32768,    22,    22,    22,-32768,    22,-32768,    -2,   -17,
  -32768,-32768,-32768,-32768,     4,-32768,   -24,   -24,     2,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,     3,-32768,-32768,    -1,
      32,-32768,-32768,-32768,     5,-32768,-32768,    -1,-32768,-32768,
  -32768,   -18,    -3,-32768,-32768,-32768,    -1,-32768,-32768,-32768,
      -3,-32768,    39,    40,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   -28,    10,-32768,    -5,-32768,-32768,
     -40,    -8,-32768,   -48,   -19,-32768,-32768
};


#define	YYLAST		110


static const short yytable[] =
{
      40,    37,    38,    37,    38,    47,    71,    53,    37,    34,
      70,    35,    54,    86,    36,    84,    42,    85,    43,    48,
      66,    44,    79,    61,    62,    63,    45,    64,    72,    72,
      48,    51,    65,    39,    59,    39,    37,    87,    73,    93,
      94,    80,    76,    57,    67,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     3,
       4,     5,     6,     7,     8,     9,    10,    11,     0,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,    26,
      27
};

static const short yycheck[] =
{
       8,     4,     5,     4,     5,    24,    54,    31,     4,    31,
       6,    40,    36,    16,     6,    33,    32,    35,    32,    36,
      37,    32,    70,    42,    43,    44,     4,    46,    56,    57,
      36,    31,    34,    36,    33,    36,     4,    40,    36,     0,
       0,    36,    39,    33,    49,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

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
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

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
	    (To)[yyi] = (From)[yyi];		\
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
#define YYABORT 	goto yyabortlab
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
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


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
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
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
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

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
#endif

#line 315 "/usr/share/bison/bison.simple"


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

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

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

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
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
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
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
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

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
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
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
      if (yyn == YYFLAG)
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
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 1:
#line 131 "parse-gram.y"
{ LOCATION_RESET (yylloc); }
    break;
case 2:
#line 133 "parse-gram.y"
{
      yycontrol->errcode = 0;
      epilogue_set (yyvsp[0].string, yylsp[0].first_line);
    }
    break;
case 6:
#line 147 "parse-gram.y"
{
     prologue_augment (yyvsp[0].string, yylsp[0].first_line);
   }
    break;
case 7:
#line 150 "parse-gram.y"
{ debug_flag = 1; }
    break;
case 8:
#line 151 "parse-gram.y"
{ muscle_insert (yyvsp[-1].string, yyvsp[0].string); }
    break;
case 9:
#line 152 "parse-gram.y"
{ defines_flag = 1; }
    break;
case 10:
#line 153 "parse-gram.y"
{ error_verbose = 1; }
    break;
case 11:
#line 154 "parse-gram.y"
{ expected_conflicts = yyvsp[0].integer; }
    break;
case 12:
#line 155 "parse-gram.y"
{ spec_file_prefix = yyvsp[0].string; }
    break;
case 13:
#line 156 "parse-gram.y"
{ locations_flag = 1; }
    break;
case 14:
#line 157 "parse-gram.y"
{ spec_name_prefix = yyvsp[0].string; }
    break;
case 15:
#line 158 "parse-gram.y"
{ no_lines_flag = 1; }
    break;
case 16:
#line 159 "parse-gram.y"
{ spec_outfile = yyvsp[0].string; }
    break;
case 17:
#line 160 "parse-gram.y"
{ pure_parser = 1; }
    break;
case 18:
#line 161 "parse-gram.y"
{ skeleton = yyvsp[0].string; }
    break;
case 19:
#line 162 "parse-gram.y"
{ token_table_flag = 1; }
    break;
case 20:
#line 163 "parse-gram.y"
{ report_flag = 1; }
    break;
case 21:
#line 164 "parse-gram.y"
{ yacc_flag = 1; }
    break;
case 23:
#line 169 "parse-gram.y"
{ current_class = nterm_sym; }
    break;
case 24:
#line 170 "parse-gram.y"
{
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;
case 25:
#line 175 "parse-gram.y"
{
      grammar_start_symbol_set (yyvsp[0].symbol);
    }
    break;
case 26:
#line 178 "parse-gram.y"
{ current_class = token_sym; }
    break;
case 27:
#line 179 "parse-gram.y"
{
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;
case 28:
#line 183 "parse-gram.y"
{current_type = yyvsp[0].string; }
    break;
case 29:
#line 184 "parse-gram.y"
{
      current_type = NULL;
    }
    break;
case 30:
#line 188 "parse-gram.y"
{
      typed = 1;
      MUSCLE_INSERT_INT ("stype_line", yylsp[-1].first_line);
      muscle_insert ("stype", yyvsp[-1].string);
    }
    break;
case 31:
#line 197 "parse-gram.y"
{ current_assoc = yyvsp[-1].assoc; ++current_prec; }
    break;
case 32:
#line 199 "parse-gram.y"
{ current_assoc = non_assoc; current_type = NULL; }
    break;
case 33:
#line 203 "parse-gram.y"
{ yyval.assoc = left_assoc; }
    break;
case 34:
#line 204 "parse-gram.y"
{ yyval.assoc = right_assoc; }
    break;
case 35:
#line 205 "parse-gram.y"
{ yyval.assoc = non_assoc; }
    break;
case 36:
#line 209 "parse-gram.y"
{ current_type = NULL;}
    break;
case 37:
#line 210 "parse-gram.y"
{ current_type = yyvsp[0].string; }
    break;
case 38:
#line 215 "parse-gram.y"
{ symbol_type_set (yyvsp[0].symbol, current_type); }
    break;
case 39:
#line 216 "parse-gram.y"
{ symbol_type_set (yyvsp[0].symbol, current_type); }
    break;
case 40:
#line 222 "parse-gram.y"
{
      symbol_type_set (yyvsp[0].symbol, current_type);
      symbol_precedence_set (yyvsp[0].symbol, current_prec, current_assoc);
    }
    break;
case 41:
#line 227 "parse-gram.y"
{
      symbol_type_set (yyvsp[0].symbol, current_type);
      symbol_precedence_set (yyvsp[0].symbol, current_prec, current_assoc);
    }
    break;
case 42:
#line 237 "parse-gram.y"
{
       current_type = yyvsp[0].string;
     }
    break;
case 43:
#line 241 "parse-gram.y"
{
       symbol_class_set (yyvsp[0].symbol, current_class);
       symbol_type_set (yyvsp[0].symbol, current_type);
     }
    break;
case 44:
#line 246 "parse-gram.y"
{
      symbol_class_set (yyvsp[-1].symbol, current_class);
      symbol_type_set (yyvsp[-1].symbol, current_type);
      symbol_user_token_number_set (yyvsp[-1].symbol, yyvsp[0].integer);
    }
    break;
case 45:
#line 252 "parse-gram.y"
{
      symbol_class_set (yyvsp[-1].symbol, current_class);
      symbol_type_set (yyvsp[-1].symbol, current_type);
      symbol_make_alias (yyvsp[-1].symbol, yyvsp[0].symbol);
    }
    break;
case 46:
#line 258 "parse-gram.y"
{
      symbol_class_set (yyvsp[-2].symbol, current_class);
      symbol_type_set (yyvsp[-2].symbol, current_type);
      symbol_user_token_number_set (yyvsp[-2].symbol, yyvsp[-1].integer);
      symbol_make_alias (yyvsp[-2].symbol, yyvsp[0].symbol);
    }
    break;
case 47:
#line 269 "parse-gram.y"
{;}
    break;
case 48:
#line 271 "parse-gram.y"
{;}
    break;
case 51:
#line 280 "parse-gram.y"
{ current_lhs = yyvsp[-1].symbol; }
    break;
case 52:
#line 281 "parse-gram.y"
{;}
    break;
case 53:
#line 285 "parse-gram.y"
{ grammar_rule_end (); }
    break;
case 54:
#line 286 "parse-gram.y"
{ grammar_rule_end (); }
    break;
case 55:
#line 291 "parse-gram.y"
{ grammar_rule_begin (current_lhs); }
    break;
case 56:
#line 293 "parse-gram.y"
{ grammar_current_rule_symbol_append (yyvsp[0].symbol); }
    break;
case 57:
#line 295 "parse-gram.y"
{ grammar_current_rule_action_append (yyvsp[0].string, yylsp[0].first_line); }
    break;
case 58:
#line 297 "parse-gram.y"
{ grammar_current_rule_prec_set (yyvsp[0].symbol); }
    break;
case 59:
#line 301 "parse-gram.y"
{ yyval.symbol = yyvsp[0].symbol; }
    break;
case 60:
#line 302 "parse-gram.y"
{ yyval.symbol = yyvsp[0].symbol; }
    break;
case 61:
#line 303 "parse-gram.y"
{ yyval.symbol = getsym (yyvsp[0].string); }
    break;
case 62:
#line 308 "parse-gram.y"
{ yyval.string = yyvsp[0].string; }
    break;
case 63:
#line 314 "parse-gram.y"
{
      yyval.symbol = getsym (yyvsp[0].string);
      symbol_class_set (yyval.symbol, token_sym);
    }
    break;
case 64:
#line 323 "parse-gram.y"
{
      yyval.string = yyvsp[0].string + 1;
      yyval.string[strlen (yyval.string) - 1] = '\0';
    }
    break;
case 65:
#line 331 "parse-gram.y"
{
      yyval.string = xstrdup ("");
    }
    break;
case 66:
#line 335 "parse-gram.y"
{
      yyval.string = yyvsp[0].string;
    }
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

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
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
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
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

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

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 344 "parse-gram.y"

/*------------------------------------------------------------------.
| When debugging the parser, display tokens' locations and values.  |
`------------------------------------------------------------------*/

static void
yyprint (FILE *file,
         const yyltype *loc, int type, const yystype *value)
{
  fputs (" (", file);
  LOCATION_PRINT (file, *loc);
  fputs (")", file);
  switch (type)
    {
    case CHARACTER:
      fprintf (file, " = '%s'", value->string);
      break;

    case ID:
      fprintf (file, " = %s", value->symbol->tag);
      break;

    case INT:
      fprintf (file, " = %d", value->integer);
      break;

    case STRING:
      fprintf (file, " = \"%s\"", value->string);
      break;

    case TYPE:
      fprintf (file, " = <%s>", value->string);
      break;

    case BRACED_CODE:
    case PROLOGUE:
    case EPILOGUE:
      fprintf (file, " = {{ %s }}", value->string);
      break;
    }
}

void
gram_error (gram_control_t *control ATTRIBUTE_UNUSED,
	    yyltype *yylloc, const char *msg)
{
  LOCATION_PRINT (stderr, *yylloc);
  fprintf (stderr, ": %s\n", msg);
}
