/* A Bison parser, made from parse-gram.y
   by GNU bison 1.49b.  */

/* Skeleton output parser for Bison,
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
   It was written by Richard Stallman by simplifying the original so
   called ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse gram_parse
#define yylex   gram_lex
#define yyerror gram_error
#define yylval  gram_lval
#define yychar  gram_char
#define yydebug gram_debug
#define yynerrs gram_nerrs
#if YYLSP_NEEDED
# define yylloc gram_lloc
#endif


/* Copy the first part of user declarations.  */
#line 31 "parse-gram.y"

#include "system.h"
#include "muscle_tab.h"
#include "files.h"
#include "getargs.h"
#include "output.h"
#include "symlist.h"
#include "gram.h"
#include "reader.h"
#include "conflicts.h"

/* Produce verbose parse errors.  */
#define YYERROR_VERBOSE 1
#define YYLLOC_DEFAULT(Current, Rhs, N)			\
do {							\
  if (N)						\
  {							\
    Current.first_column  = Rhs[1].first_column;	\
    Current.first_line    = Rhs[1].first_line;		\
    Current.last_column   = Rhs[N].last_column;		\
    Current.last_line     = Rhs[N].last_line;		\
  }							\
  else							\
  {							\
    Current = Rhs[0];					\
  }							\
} while (0)

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
static void yyprint (FILE *file, const location_t *loc,
                     int type, const yystype *value);

symbol_class current_class = unknown_sym;
char *current_type = 0;
symbol_t *current_lhs;
location_t current_lhs_location;
associativity current_assoc;
int current_prec = 0;


/* Tokens.  */
#ifndef YYTOKENTYPE
# if defined (__STDC__) || defined (__cplusplus)
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     GRAM_EOF = 0,
     STRING = 258,
     CHARACTER = 259,
     INT = 260,
     PERCENT_TOKEN = 261,
     PERCENT_NTERM = 262,
     PERCENT_TYPE = 263,
     PERCENT_UNION = 264,
     PERCENT_LEFT = 265,
     PERCENT_RIGHT = 266,
     PERCENT_NONASSOC = 267,
     PERCENT_EXPECT = 268,
     PERCENT_START = 269,
     PERCENT_PREC = 270,
     PERCENT_VERBOSE = 271,
     PERCENT_ERROR_VERBOSE = 272,
     PERCENT_OUTPUT = 273,
     PERCENT_FILE_PREFIX = 274,
     PERCENT_NAME_PREFIX = 275,
     PERCENT_DEFINE = 276,
     PERCENT_PURE_PARSER = 277,
     PERCENT_DEFINES = 278,
     PERCENT_YACC = 279,
     PERCENT_DEBUG = 280,
     PERCENT_LOCATIONS = 281,
     PERCENT_NO_LINES = 282,
     PERCENT_SKELETON = 283,
     PERCENT_TOKEN_TABLE = 284,
     TYPE = 285,
     EQUAL = 286,
     SEMICOLON = 287,
     COLON = 288,
     PIPE = 289,
     ID = 290,
     PERCENT_PERCENT = 291,
     PROLOGUE = 292,
     EPILOGUE = 293,
     BRACED_CODE = 294
   };
# endif
  /* POSIX requires `int' for tokens in interfaces.  */
# define YYTOKENTYPE int
#endif /* !YYTOKENTYPE */
#define GRAM_EOF 0
#define STRING 258
#define CHARACTER 259
#define INT 260
#define PERCENT_TOKEN 261
#define PERCENT_NTERM 262
#define PERCENT_TYPE 263
#define PERCENT_UNION 264
#define PERCENT_LEFT 265
#define PERCENT_RIGHT 266
#define PERCENT_NONASSOC 267
#define PERCENT_EXPECT 268
#define PERCENT_START 269
#define PERCENT_PREC 270
#define PERCENT_VERBOSE 271
#define PERCENT_ERROR_VERBOSE 272
#define PERCENT_OUTPUT 273
#define PERCENT_FILE_PREFIX 274
#define PERCENT_NAME_PREFIX 275
#define PERCENT_DEFINE 276
#define PERCENT_PURE_PARSER 277
#define PERCENT_DEFINES 278
#define PERCENT_YACC 279
#define PERCENT_DEBUG 280
#define PERCENT_LOCATIONS 281
#define PERCENT_NO_LINES 282
#define PERCENT_SKELETON 283
#define PERCENT_TOKEN_TABLE 284
#define TYPE 285
#define EQUAL 286
#define SEMICOLON 287
#define COLON 288
#define PIPE 289
#define ID 290
#define PERCENT_PERCENT 291
#define PROLOGUE 292
#define EPILOGUE 293
#define BRACED_CODE 294




/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#ifndef YYSTYPE
#line 90 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  symbol_list_t *list;
  int integer;
  char *string;
  associativity assoc;
} yystype;
/* Line 199 of /usr/local/share/bison/bison.simple.  */
#line 236 "parse-gram.c"
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

/* Copy the second part of user declarations.  */


/* Line 219 of /usr/local/share/bison/bison.simple.  */
#line 257 "parse-gram.c"

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
	    (To)[yyi] = (From)[yyi];	\
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
#define YYFINAL  3
#define YYFLAG	 -32768
#define YYLAST   111

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  26
/* YYNRULES -- Number of rules. */
#define YYNRULES  67
/* YYNRULES -- Number of states. */
#define YYNSTATES  93

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     8,     9,    13,    15,    17,    19,    23,
      25,    27,    30,    34,    36,    40,    42,    46,    48,    51,
      53,    55,    57,    59,    61,    64,    67,    68,    72,    73,
      77,    81,    85,    87,    89,    91,    92,    94,    96,    99,
     101,   103,   106,   109,   113,   115,   118,   120,   123,   125,
     128,   129,   135,   137,   141,   142,   145,   148,   152,   154,
     156,   158,   160,   162,   164,   165,   168,   169
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const signed char yyrhs[] =
{
      41,     0,    -1,    42,    36,    54,    64,    -1,    -1,    42,
      43,    65,    -1,    44,    -1,    37,    -1,    25,    -1,    21,
      63,    63,    -1,    23,    -1,    17,    -1,    13,     5,    -1,
      19,    31,    63,    -1,    26,    -1,    20,    31,    63,    -1,
      27,    -1,    18,    31,    63,    -1,    22,    -1,    28,    63,
      -1,    29,    -1,    16,    -1,    24,    -1,    48,    -1,    45,
      -1,    14,    60,    -1,     9,    39,    -1,    -1,     7,    46,
      53,    -1,    -1,     6,    47,    53,    -1,     8,    30,    51,
      -1,    49,    50,    51,    -1,    10,    -1,    11,    -1,    12,
      -1,    -1,    30,    -1,    60,    -1,    51,    60,    -1,    30,
      -1,    35,    -1,    35,     5,    -1,    35,    62,    -1,    35,
       5,    62,    -1,    52,    -1,    53,    52,    -1,    55,    -1,
      54,    55,    -1,    56,    -1,    44,    32,    -1,    -1,    35,
      33,    57,    58,    32,    -1,    59,    -1,    58,    34,    59,
      -1,    -1,    59,    60,    -1,    59,    61,    -1,    59,    15,
      60,    -1,    35,    -1,    62,    -1,     4,    -1,    39,    -1,
       3,    -1,     3,    -1,    -1,    36,    38,    -1,    -1,    32,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   152,   152,   165,   167,   170,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   190,   192,   193,   197,   205,   205,   211,   211,
     216,   225,   240,   242,   243,   246,   248,   253,   255,   259,
     264,   269,   275,   281,   291,   294,   303,   305,   311,   313,
     316,   316,   321,   323,   326,   329,   331,   333,   337,   339,
     340,   343,   349,   358,   366,   371,   377,   379
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "\"end of string\"", "error", "$undefined.", "STRING", "CHARACTER", "INT", 
  "\"%token\"", "\"%nterm\"", "\"%type\"", "\"%union\"", "\"%left\"", 
  "\"%right\"", "\"%nonassoc\"", "\"%expect\"", "\"%start\"", "\"%prec\"", 
  "\"%verbose\"", "\"%error-verbose\"", "\"%output\"", "\"%file-prefix\"", 
  "\"%name-prefix\"", "\"%define\"", "\"%pure-parser\"", "\"%defines\"", 
  "\"%yacc\"", "\"%debug\"", "\"%locations\"", "\"%no-lines\"", 
  "\"%skeleton\"", "\"%token-table\"", "TYPE", "\"=\"", "\";\"", "\":\"", 
  "\"|\"", "\"identifier\"", "\"%%\"", "PROLOGUE", "EPILOGUE", 
  "BRACED_CODE", "$axiom", "input", "declarations", "declaration", 
  "grammar_declaration", "symbol_declaration", "@1", "@2", 
  "precedence_declaration", "precedence_declarator", "type.opt", 
  "symbols.1", "symbol_def", "symbol_defs.1", "grammar", 
  "rules_or_grammar_declaration", "rules", "@3", "rhses.1", "rhs", 
  "symbol", "action", "string_as_id", "string_content", "epilogue.opt", 
  "semi_colon.opt", 0
};
#endif

/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      -1
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    44,    44,    44,    44,    46,    45,    47,    45,
      45,    48,    49,    49,    49,    50,    50,    51,    51,    52,
      52,    52,    52,    52,    53,    53,    54,    54,    55,    55,
      57,    56,    58,    58,    59,    59,    59,    59,    60,    60,
      60,    61,    62,    63,    64,    64,    65,    65
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     0,     3,     1,     1,     1,     3,     1,
       1,     2,     3,     1,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     2,     2,     0,     3,     0,     3,
       3,     3,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     2,     2,     3,     1,     2,     1,     2,     1,     2,
       0,     5,     1,     3,     0,     2,     2,     3,     1,     1,
       1,     1,     1,     1,     0,     2,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const short yydefact[] =
{
       3,     0,     0,     0,    28,    26,     0,     0,    32,    33,
      34,     0,     0,    20,    10,     0,     0,     0,     0,    17,
       9,    21,     7,    13,    15,     0,    19,     0,     6,    66,
       5,    23,    22,    35,     0,     0,     0,    25,    11,    62,
      60,    58,    24,    59,     0,     0,     0,    63,     0,    18,
       0,     0,    64,    46,    48,    67,     4,    36,     0,    39,
      40,    44,    29,    27,    30,    37,    16,    12,    14,     8,
      50,    49,     0,    47,     2,    31,    41,    42,    45,    38,
      54,    65,    43,     0,    52,    51,    54,     0,    61,    55,
      56,    53,    57
};

/* YYPGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     1,     2,    29,    51,    31,    35,    34,    32,    33,
      58,    64,    61,    62,    52,    53,    54,    80,    83,    84,
      65,    90,    43,    48,    74,    56
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const short yypact[] =
{
  -32768,    23,    74,-32768,-32768,-32768,     1,   -31,-32768,-32768,
  -32768,    29,     0,-32768,-32768,     7,    10,    14,    39,-32768,
  -32768,-32768,-32768,-32768,-32768,    39,-32768,    18,-32768,    15,
  -32768,-32768,-32768,    16,   -24,   -24,     0,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,    39,    39,    39,-32768,    39,-32768,
      17,    19,     8,-32768,-32768,-32768,-32768,-32768,     0,-32768,
       4,-32768,   -24,   -24,     0,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    20,-32768,-32768,     0,    45,-32768,-32768,-32768,
  -32768,-32768,-32768,   -22,    -2,-32768,-32768,     0,-32768,-32768,
  -32768,    -2,-32768
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,    47,-32768,-32768,-32768,-32768,-32768,
  -32768,    -4,   -23,    25,-32768,     9,-32768,-32768,-32768,   -21,
     -12,-32768,   -55,    11,-32768,-32768
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
static const short yytable[] =
{
      42,    39,    40,    39,    40,    77,    59,    39,    37,    76,
      85,    60,    86,    87,     4,     5,     6,     7,     8,     9,
      10,    82,    12,     3,     4,     5,     6,     7,     8,     9,
      10,    36,    12,    41,    38,    41,    49,    88,    44,    78,
      78,    45,    47,    50,    72,    46,    57,    55,    39,    30,
      70,    71,    79,    50,    75,    66,    67,    68,    81,    69,
      63,    73,     0,    79,     0,    91,     0,     0,     0,     0,
       0,     0,    89,     0,     0,    92,     0,     0,     0,    89,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,     0,     0,     0,     0,     0,
      27,    28
};

static const short yycheck[] =
{
      12,     3,     4,     3,     4,    60,    30,     3,    39,     5,
      32,    35,    34,    15,     6,     7,     8,     9,    10,    11,
      12,    76,    14,     0,     6,     7,     8,     9,    10,    11,
      12,    30,    14,    35,     5,    35,    25,    39,    31,    62,
      63,    31,     3,    35,    36,    31,    30,    32,     3,     2,
      33,    32,    64,    35,    58,    44,    45,    46,    38,    48,
      35,    52,    -1,    75,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    87,    -1,    -1,    -1,    91,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    37
};

#if YYDEBUG
/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    41,    42,     0,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    36,    37,    43,
      44,    45,    48,    49,    47,    46,    30,    39,     5,     3,
       4,    35,    60,    62,    31,    31,    31,     3,    63,    63,
      35,    44,    54,    55,    56,    32,    65,    30,    50,    30,
      35,    52,    53,    53,    51,    60,    63,    63,    63,    63,
      33,    32,    36,    55,    64,    51,     5,    62,    52,    60,
      57,    38,    62,    58,    59,    32,    34,    15,    39,    60,
      61,    59,    60
};
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
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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
/* The semantic value of the lookahead symbol.  */	\
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

/* If nonreentrant, generate the variables here.  */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here.  */
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

  /* The state stack.  */
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
#  if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
#  endif
#  undef YYSTACK_RELOCATE
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
  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

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
    {
        case 2:
#line 154 "parse-gram.y"
    {
      yycontrol->errcode = 0;
      epilogue_set (yyvsp[0].string, yylsp[0]);
    }
    break;

  case 6:
#line 172 "parse-gram.y"
    { prologue_augment (yyvsp[0].string, yylsp[0]); }
    break;

  case 7:
#line 173 "parse-gram.y"
    { debug_flag = 1; }
    break;

  case 8:
#line 174 "parse-gram.y"
    { muscle_insert (yyvsp[-1].string, yyvsp[0].string); }
    break;

  case 9:
#line 175 "parse-gram.y"
    { defines_flag = 1; }
    break;

  case 10:
#line 176 "parse-gram.y"
    { error_verbose = 1; }
    break;

  case 11:
#line 177 "parse-gram.y"
    { expected_conflicts = yyvsp[0].integer; }
    break;

  case 12:
#line 178 "parse-gram.y"
    { spec_file_prefix = yyvsp[0].string; }
    break;

  case 13:
#line 179 "parse-gram.y"
    { locations_flag = 1; }
    break;

  case 14:
#line 180 "parse-gram.y"
    { spec_name_prefix = yyvsp[0].string; }
    break;

  case 15:
#line 181 "parse-gram.y"
    { no_lines_flag = 1; }
    break;

  case 16:
#line 182 "parse-gram.y"
    { spec_outfile = yyvsp[0].string; }
    break;

  case 17:
#line 183 "parse-gram.y"
    { pure_parser = 1; }
    break;

  case 18:
#line 184 "parse-gram.y"
    { skeleton = yyvsp[0].string; }
    break;

  case 19:
#line 185 "parse-gram.y"
    { token_table_flag = 1; }
    break;

  case 20:
#line 186 "parse-gram.y"
    { report_flag = 1; }
    break;

  case 21:
#line 187 "parse-gram.y"
    { yacc_flag = 1; }
    break;

  case 24:
#line 194 "parse-gram.y"
    {
      grammar_start_symbol_set (yyvsp[0].symbol, yylsp[0]);
    }
    break;

  case 25:
#line 198 "parse-gram.y"
    {
      typed = 1;
      MUSCLE_INSERT_INT ("stype_line", yylsp[0].first_line);
      muscle_insert ("stype", yyvsp[0].string);
    }
    break;

  case 26:
#line 206 "parse-gram.y"
    { current_class = nterm_sym; }
    break;

  case 27:
#line 207 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 28:
#line 211 "parse-gram.y"
    { current_class = token_sym; }
    break;

  case 29:
#line 212 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 30:
#line 217 "parse-gram.y"
    {
      symbol_list_t *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_type_set (list->sym, list->location, yyvsp[-1].string);
      symbol_list_free (yyvsp[0].list);
    }
    break;

  case 31:
#line 227 "parse-gram.y"
    {
      symbol_list_t *list;
      ++current_prec;
      for (list = yyvsp[0].list; list; list = list->next)
	{
	  symbol_type_set (list->sym, list->location, current_type);
	  symbol_precedence_set (list->sym, list->location, current_prec, yyvsp[-2].assoc);
	}
      symbol_list_free (yyvsp[0].list);
      current_type = NULL;
    }
    break;

  case 32:
#line 241 "parse-gram.y"
    { yyval.assoc = left_assoc; }
    break;

  case 33:
#line 242 "parse-gram.y"
    { yyval.assoc = right_assoc; }
    break;

  case 34:
#line 243 "parse-gram.y"
    { yyval.assoc = non_assoc; }
    break;

  case 35:
#line 247 "parse-gram.y"
    { current_type = NULL;}
    break;

  case 36:
#line 248 "parse-gram.y"
    { current_type = yyvsp[0].string; }
    break;

  case 37:
#line 254 "parse-gram.y"
    { yyval.list = symbol_list_new (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 38:
#line 255 "parse-gram.y"
    { yyval.list = symbol_list_prepend (yyvsp[-1].list, yyvsp[0].symbol, yylsp[0]); }
    break;

  case 39:
#line 261 "parse-gram.y"
    {
       current_type = yyvsp[0].string;
     }
    break;

  case 40:
#line 265 "parse-gram.y"
    {
       symbol_class_set (yyvsp[0].symbol, current_class);
       symbol_type_set (yyvsp[0].symbol, yylsp[0], current_type);
     }
    break;

  case 41:
#line 270 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class);
      symbol_type_set (yyvsp[-1].symbol, yylsp[-1], current_type);
      symbol_user_token_number_set (yyvsp[-1].symbol, yyvsp[0].integer);
    }
    break;

  case 42:
#line 276 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class);
      symbol_type_set (yyvsp[-1].symbol, yylsp[-1], current_type);
      symbol_make_alias (yyvsp[-1].symbol, yyvsp[0].symbol);
    }
    break;

  case 43:
#line 282 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-2].symbol, current_class);
      symbol_type_set (yyvsp[-2].symbol, yylsp[-2], current_type);
      symbol_user_token_number_set (yyvsp[-2].symbol, yyvsp[-1].integer);
      symbol_make_alias (yyvsp[-2].symbol, yyvsp[0].symbol);
    }
    break;

  case 44:
#line 293 "parse-gram.y"
    {;}
    break;

  case 45:
#line 295 "parse-gram.y"
    {;}
    break;

  case 50:
#line 317 "parse-gram.y"
    { current_lhs = yyvsp[-1].symbol; current_lhs_location = yylsp[-1]; }
    break;

  case 51:
#line 318 "parse-gram.y"
    {;}
    break;

  case 52:
#line 322 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 53:
#line 323 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 54:
#line 328 "parse-gram.y"
    { grammar_rule_begin (current_lhs, current_lhs_location); }
    break;

  case 55:
#line 330 "parse-gram.y"
    { grammar_current_rule_symbol_append (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 56:
#line 332 "parse-gram.y"
    { grammar_current_rule_action_append (yyvsp[0].string, yylsp[0]); }
    break;

  case 57:
#line 334 "parse-gram.y"
    { grammar_current_rule_prec_set (yyvsp[0].symbol); }
    break;

  case 58:
#line 338 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 59:
#line 339 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 60:
#line 340 "parse-gram.y"
    { yyval.symbol = getsym (yyvsp[0].string, yylsp[0]); }
    break;

  case 61:
#line 345 "parse-gram.y"
    { yyval.string = yyvsp[0].string; }
    break;

  case 62:
#line 351 "parse-gram.y"
    {
      yyval.symbol = getsym (yyvsp[0].string, yylsp[0]);
      symbol_class_set (yyval.symbol, token_sym);
    }
    break;

  case 63:
#line 360 "parse-gram.y"
    {
      yyval.string = yyvsp[0].string + 1;
      yyval.string[strlen (yyval.string) - 1] = '\0';
    }
    break;

  case 64:
#line 368 "parse-gram.y"
    {
      yyval.string = xstrdup ("");
    }
    break;

  case 65:
#line 372 "parse-gram.y"
    {
      yyval.string = yyvsp[0].string;
    }
    break;


    }

/* Line 1012 of /usr/local/share/bison/bison.simple.  */
#line 1524 "parse-gram.c"

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

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
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

#if YYERROR_VERBOSE
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
#endif /* YYERROR_VERBOSE */
        yyerror ("parse error");
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
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYFLAG)
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

#if YYDEBUG
      if (yydebug)
	{
	  if (yystos[yystate] < YYNTOKENS)
	    {
	      YYFPRINTF (stderr, "Error: popping token %d (%s",
			 yytoknum[yystos[yystate]], yytname[yystos[yystate]]);
# ifdef YYPRINT
	      YYPRINT (stderr, yytoknum[yystos[yystate]], *yyvsp);
# endif
	      YYFPRINTF (stderr, ")\n");
	    }
	  else
	    {
	      YYFPRINTF (stderr, "Error: popping nonterminal (%s)\n",
			 yytname[yystos[yystate]]);
	    }
	}
#endif

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
    }

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

#line 381 "parse-gram.y"

/*------------------------------------------------------------------.
| When debugging the parser, display tokens' locations and values.  |
`------------------------------------------------------------------*/

static void
yyprint (FILE *file,
         const location_t *loc, int type, const yystype *value)
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
	    location_t *yylloc, const char *msg)
{
  LOCATION_PRINT (stderr, *yylloc);
  fprintf (stderr, ": %s\n", msg);
}

