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
#define yylloc gram_lloc

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

#define YYPRINT(File, Type, Value) \
        yyprint (File, Type, &Value)
static void yyprint (FILE *file, int type, const yystype *value);

symbol_class current_class = unknown_sym;
char *current_type = 0;
symbol_t *current_lhs;
location_t current_lhs_location;
associativity current_assoc;
int current_prec = 0;
braced_code_t current_braced_code = action_braced_code;


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
     PERCENT_DESTRUCTOR = 264,
     PERCENT_PRINTER = 265,
     PERCENT_UNION = 266,
     PERCENT_LEFT = 267,
     PERCENT_RIGHT = 268,
     PERCENT_NONASSOC = 269,
     PERCENT_EXPECT = 270,
     PERCENT_START = 271,
     PERCENT_PREC = 272,
     PERCENT_DPREC = 273,
     PERCENT_MERGE = 274,
     PERCENT_VERBOSE = 275,
     PERCENT_ERROR_VERBOSE = 276,
     PERCENT_OUTPUT = 277,
     PERCENT_FILE_PREFIX = 278,
     PERCENT_NAME_PREFIX = 279,
     PERCENT_DEFINE = 280,
     PERCENT_PURE_PARSER = 281,
     PERCENT_GLR_PARSER = 282,
     PERCENT_DEFINES = 283,
     PERCENT_YACC = 284,
     PERCENT_DEBUG = 285,
     PERCENT_LOCATIONS = 286,
     PERCENT_NO_LINES = 287,
     PERCENT_SKELETON = 288,
     PERCENT_TOKEN_TABLE = 289,
     TYPE = 290,
     EQUAL = 291,
     SEMICOLON = 292,
     COLON = 293,
     PIPE = 294,
     ID = 295,
     PERCENT_PERCENT = 296,
     PROLOGUE = 297,
     EPILOGUE = 298,
     BRACED_CODE = 299
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
#define PERCENT_DESTRUCTOR 264
#define PERCENT_PRINTER 265
#define PERCENT_UNION 266
#define PERCENT_LEFT 267
#define PERCENT_RIGHT 268
#define PERCENT_NONASSOC 269
#define PERCENT_EXPECT 270
#define PERCENT_START 271
#define PERCENT_PREC 272
#define PERCENT_DPREC 273
#define PERCENT_MERGE 274
#define PERCENT_VERBOSE 275
#define PERCENT_ERROR_VERBOSE 276
#define PERCENT_OUTPUT 277
#define PERCENT_FILE_PREFIX 278
#define PERCENT_NAME_PREFIX 279
#define PERCENT_DEFINE 280
#define PERCENT_PURE_PARSER 281
#define PERCENT_GLR_PARSER 282
#define PERCENT_DEFINES 283
#define PERCENT_YACC 284
#define PERCENT_DEBUG 285
#define PERCENT_LOCATIONS 286
#define PERCENT_NO_LINES 287
#define PERCENT_SKELETON 288
#define PERCENT_TOKEN_TABLE 289
#define TYPE 290
#define EQUAL 291
#define SEMICOLON 292
#define COLON 293
#define PIPE 294
#define ID 295
#define PERCENT_PERCENT 296
#define PROLOGUE 297
#define EPILOGUE 298
#define BRACED_CODE 299




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
#line 88 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  symbol_list_t *list;
  int integer;
  char *string;
  associativity assoc;
} yystype;
/* Line 272 of /usr/local/share/bison/yacc.c.  */
#line 241 "parse-gram.c"
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


/* Line 292 of /usr/local/share/bison/yacc.c.  */
#line 262 "parse-gram.c"

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
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)

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
#define YYLAST   125

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  28
/* YYNRULES -- Number of rules. */
#define YYNRULES  74
/* YYNRULES -- Number of states. */
#define YYNSTATES  106

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     8,     9,    13,    15,    17,    19,    23,
      25,    27,    30,    34,    36,    40,    42,    46,    48,    50,
      53,    55,    57,    59,    61,    63,    66,    69,    70,    75,
      76,    81,    82,    86,    87,    91,    95,    99,   101,   103,
     105,   106,   108,   110,   113,   115,   117,   120,   123,   127,
     129,   132,   134,   137,   139,   142,   143,   149,   151,   155,
     156,   159,   162,   166,   170,   174,   176,   178,   180,   182,
     184,   186,   187,   190,   191
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const signed char yyrhs[] =
{
      46,     0,    -1,    47,    41,    61,    71,    -1,    -1,    47,
      48,    72,    -1,    49,    -1,    42,    -1,    30,    -1,    25,
      70,    70,    -1,    28,    -1,    21,    -1,    15,     5,    -1,
      23,    36,    70,    -1,    31,    -1,    24,    36,    70,    -1,
      32,    -1,    22,    36,    70,    -1,    26,    -1,    27,    -1,
      33,    70,    -1,    34,    -1,    20,    -1,    29,    -1,    55,
      -1,    52,    -1,    16,    67,    -1,    11,    44,    -1,    -1,
       9,    50,    44,    58,    -1,    -1,    10,    51,    44,    58,
      -1,    -1,     7,    53,    60,    -1,    -1,     6,    54,    60,
      -1,     8,    35,    58,    -1,    56,    57,    58,    -1,    12,
      -1,    13,    -1,    14,    -1,    -1,    35,    -1,    67,    -1,
      58,    67,    -1,    35,    -1,    40,    -1,    40,     5,    -1,
      40,    69,    -1,    40,     5,    69,    -1,    59,    -1,    60,
      59,    -1,    62,    -1,    61,    62,    -1,    63,    -1,    49,
      37,    -1,    -1,    40,    38,    64,    65,    37,    -1,    66,
      -1,    65,    39,    66,    -1,    -1,    66,    67,    -1,    66,
      68,    -1,    66,    17,    67,    -1,    66,    18,     5,    -1,
      66,    19,    35,    -1,    40,    -1,    69,    -1,     4,    -1,
      44,    -1,     3,    -1,     3,    -1,    -1,    41,    43,    -1,
      -1,    37,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   158,   158,   171,   173,   176,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   197,   199,   200,   204,   211,   210,   221,
     220,   233,   232,   238,   238,   243,   252,   267,   269,   270,
     273,   275,   280,   282,   286,   291,   296,   302,   308,   318,
     321,   330,   332,   338,   340,   344,   343,   348,   350,   353,
     356,   358,   360,   362,   364,   368,   370,   371,   374,   380,
     389,   397,   402,   408,   410
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "\"end of string\"", "error", "$undefined.", "STRING", "CHARACTER", "INT", 
  "\"%token\"", "\"%nterm\"", "\"%type\"", "\"%destructor\"", 
  "\"%printer\"", "\"%union\"", "\"%left\"", "\"%right\"", 
  "\"%nonassoc\"", "\"%expect\"", "\"%start\"", "\"%prec\"", "\"%dprec\"", 
  "\"%merge\"", "\"%verbose\"", "\"%error-verbose\"", "\"%output\"", 
  "\"%file-prefix\"", "\"%name-prefix\"", "\"%define\"", 
  "\"%pure-parser\"", "\"%glr-parser\"", "\"%defines\"", "\"%yacc\"", 
  "\"%debug\"", "\"%locations\"", "\"%no-lines\"", "\"%skeleton\"", 
  "\"%token-table\"", "TYPE", "\"=\"", "\";\"", "\":\"", "\"|\"", 
  "\"identifier\"", "\"%%\"", "PROLOGUE", "EPILOGUE", "BRACED_CODE", 
  "$axiom", "input", "declarations", "declaration", "grammar_declaration", 
  "@1", "@2", "symbol_declaration", "@3", "@4", "precedence_declaration", 
  "precedence_declarator", "type.opt", "symbols.1", "symbol_def", 
  "symbol_defs.1", "grammar", "rules_or_grammar_declaration", "rules", 
  "@5", "rhses.1", "rhs", "symbol", "action", "string_as_id", 
  "string_content", "epilogue.opt", "semi_colon.opt", 0
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
     295,   296,   297,   298,   299,    -1
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    49,    49,    49,    49,    50,    49,    51,
      49,    53,    52,    54,    52,    52,    55,    56,    56,    56,
      57,    57,    58,    58,    59,    59,    59,    59,    59,    60,
      60,    61,    61,    62,    62,    64,    63,    65,    65,    66,
      66,    66,    66,    66,    66,    67,    67,    67,    68,    69,
      70,    71,    71,    72,    72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     0,     3,     1,     1,     1,     3,     1,
       1,     2,     3,     1,     3,     1,     3,     1,     1,     2,
       1,     1,     1,     1,     1,     2,     2,     0,     4,     0,
       4,     0,     3,     0,     3,     3,     3,     1,     1,     1,
       0,     1,     1,     2,     1,     1,     2,     2,     3,     1,
       2,     1,     2,     1,     2,     0,     5,     1,     3,     0,
       2,     2,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     0,     2,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const short yydefact[] =
{
       3,     0,     0,     0,    33,    31,     0,    27,    29,     0,
      37,    38,    39,     0,     0,    21,    10,     0,     0,     0,
       0,    17,    18,     9,    22,     7,    13,    15,     0,    20,
       0,     6,    73,     5,    24,    23,    40,     0,     0,     0,
       0,     0,    26,    11,    69,    67,    65,    25,    66,     0,
       0,     0,    70,     0,    19,     0,     0,    71,    51,    53,
      74,     4,    41,     0,    44,    45,    49,    34,    32,    35,
      42,     0,     0,    16,    12,    14,     8,    55,    54,     0,
      52,     2,    36,    46,    47,    50,    43,    28,    30,    59,
      72,    48,     0,    57,    56,    59,     0,     0,     0,    68,
      60,    61,    58,    62,    63,    64
};

/* YYPGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     1,     2,    32,    56,    40,    41,    34,    38,    37,
      35,    36,    63,    69,    66,    67,    57,    58,    59,    89,
      92,    93,    70,   101,    48,    53,    81,    61
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const short yypact[] =
{
  -32768,     7,    83,-32768,-32768,-32768,   -23,-32768,-32768,     3,
  -32768,-32768,-32768,    43,     0,-32768,-32768,    13,    14,    15,
      51,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    51,-32768,
      23,-32768,    19,-32768,-32768,-32768,    22,   -29,   -29,     0,
      17,    18,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    51,
      51,    51,-32768,    51,-32768,    20,    27,    12,-32768,-32768,
  -32768,-32768,-32768,     0,-32768,     5,-32768,   -29,   -29,     0,
  -32768,     0,     0,-32768,-32768,-32768,-32768,-32768,-32768,    28,
  -32768,-32768,     0,    56,-32768,-32768,-32768,     0,     0,-32768,
  -32768,-32768,     4,    -2,-32768,-32768,     0,    55,    35,-32768,
  -32768,-32768,    -2,-32768,-32768,-32768
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,    70,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   -58,   -22,    37,-32768,    21,-32768,-32768,
  -32768,   -19,   -14,-32768,   -56,    16,-32768,-32768
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
static const short yytable[] =
{
      47,    44,    45,    44,    45,    82,    64,     3,    44,    84,
      83,    65,    39,    87,    88,    96,    97,    98,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    91,    14,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    46,    14,
      46,    94,    99,    95,    54,    85,    85,    42,    43,    49,
      50,    51,    55,    79,    52,    86,    60,    62,    77,    44,
     104,    71,    72,    55,    78,    73,    74,    75,    86,    76,
     105,    90,    33,    86,    86,    68,   102,     0,    80,   100,
       0,     0,   103,     0,     0,     0,     0,     0,   100,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       0,     0,     0,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     0,     0,
       0,     0,     0,     0,    30,    31
};

static const short yycheck[] =
{
      14,     3,     4,     3,     4,    63,    35,     0,     3,    65,
       5,    40,    35,    71,    72,    17,    18,    19,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    83,    16,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    40,    16,
      40,    37,    44,    39,    28,    67,    68,    44,     5,    36,
      36,    36,    40,    41,     3,    69,    37,    35,    38,     3,
       5,    44,    44,    40,    37,    49,    50,    51,    82,    53,
      35,    43,     2,    87,    88,    38,    95,    -1,    57,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,   102,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    46,    47,     0,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      41,    42,    48,    49,    52,    55,    56,    54,    53,    35,
      50,    51,    44,     5,     3,     4,    40,    67,    69,    36,
      36,    36,     3,    70,    70,    40,    49,    61,    62,    63,
      37,    72,    35,    57,    35,    40,    59,    60,    60,    58,
      67,    44,    44,    70,    70,    70,    70,    38,    37,    41,
      62,    71,    58,     5,    69,    59,    67,    58,    58,    64,
      43,    69,    65,    66,    37,    39,    17,    18,    19,    44,
      67,    68,    66,    67,     5,    35
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
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX	yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX	yylex (&yylval, &yylloc)
#endif

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

#if defined (__STDC__) || defined (__cplusplus)
static void yydestruct (int yytype,
			YYSTYPE yyvalue, YYLTYPE yylocation);
# if YYDEBUG
static void yysymprint (FILE* out, int yytype,
			YYSTYPE yyvalue, YYLTYPE yylocation);
# endif
#endif




int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of parse errors so far.  */
int yynerrs;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

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

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

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
  yylsp = yyls;
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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, yychar1, yylval, yyloc));
      YYDPRINTF ((stderr, "\n"));
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);

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
#line 160 "parse-gram.y"
    {
      yycontrol->errcode = 0;
      epilogue_set (yyvsp[0].string, yylsp[0]);
    }
    break;

  case 6:
#line 178 "parse-gram.y"
    { prologue_augment (yyvsp[0].string, yylsp[0]); }
    break;

  case 7:
#line 179 "parse-gram.y"
    { debug_flag = 1; }
    break;

  case 8:
#line 180 "parse-gram.y"
    { muscle_insert (yyvsp[-1].string, yyvsp[0].string); }
    break;

  case 9:
#line 181 "parse-gram.y"
    { defines_flag = 1; }
    break;

  case 10:
#line 182 "parse-gram.y"
    { error_verbose = 1; }
    break;

  case 11:
#line 183 "parse-gram.y"
    { expected_conflicts = yyvsp[0].integer; }
    break;

  case 12:
#line 184 "parse-gram.y"
    { spec_file_prefix = yyvsp[0].string; }
    break;

  case 13:
#line 185 "parse-gram.y"
    { locations_flag = 1; }
    break;

  case 14:
#line 186 "parse-gram.y"
    { spec_name_prefix = yyvsp[0].string; }
    break;

  case 15:
#line 187 "parse-gram.y"
    { no_lines_flag = 1; }
    break;

  case 16:
#line 188 "parse-gram.y"
    { spec_outfile = yyvsp[0].string; }
    break;

  case 17:
#line 189 "parse-gram.y"
    { pure_parser = 1; }
    break;

  case 18:
#line 190 "parse-gram.y"
    { glr_parser = 1; }
    break;

  case 19:
#line 191 "parse-gram.y"
    { skeleton = yyvsp[0].string; }
    break;

  case 20:
#line 192 "parse-gram.y"
    { token_table_flag = 1; }
    break;

  case 21:
#line 193 "parse-gram.y"
    { report_flag = 1; }
    break;

  case 22:
#line 194 "parse-gram.y"
    { yacc_flag = 1; }
    break;

  case 25:
#line 201 "parse-gram.y"
    {
      grammar_start_symbol_set (yyvsp[0].symbol, yylsp[0]);
    }
    break;

  case 26:
#line 205 "parse-gram.y"
    {
      typed = 1;
      MUSCLE_INSERT_INT ("stype_line", yylsp[0].first_line);
      muscle_insert ("stype", yyvsp[0].string);
    }
    break;

  case 27:
#line 211 "parse-gram.y"
    { current_braced_code = destructor_braced_code; }
    break;

  case 28:
#line 213 "parse-gram.y"
    {
      symbol_list_t *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_destructor_set (list->sym, yyvsp[-1].string, yylsp[-1]);
      symbol_list_free (yyvsp[0].list);
      current_braced_code = action_braced_code;
    }
    break;

  case 29:
#line 221 "parse-gram.y"
    { current_braced_code = printer_braced_code; }
    break;

  case 30:
#line 223 "parse-gram.y"
    {
      symbol_list_t *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_printer_set (list->sym, yyvsp[-1].string, list->location);
      symbol_list_free (yyvsp[0].list);
      current_braced_code = action_braced_code;
    }
    break;

  case 31:
#line 233 "parse-gram.y"
    { current_class = nterm_sym; }
    break;

  case 32:
#line 234 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 33:
#line 238 "parse-gram.y"
    { current_class = token_sym; }
    break;

  case 34:
#line 239 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 35:
#line 244 "parse-gram.y"
    {
      symbol_list_t *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_type_set (list->sym, yyvsp[-1].string, yylsp[-1]);
      symbol_list_free (yyvsp[0].list);
    }
    break;

  case 36:
#line 254 "parse-gram.y"
    {
      symbol_list_t *list;
      ++current_prec;
      for (list = yyvsp[0].list; list; list = list->next)
	{
	  symbol_type_set (list->sym, current_type, yylsp[-1]);
	  symbol_precedence_set (list->sym, current_prec, yyvsp[-2].assoc, yylsp[-2]);
	}
      symbol_list_free (yyvsp[0].list);
      current_type = NULL;
    }
    break;

  case 37:
#line 268 "parse-gram.y"
    { yyval.assoc = left_assoc; }
    break;

  case 38:
#line 269 "parse-gram.y"
    { yyval.assoc = right_assoc; }
    break;

  case 39:
#line 270 "parse-gram.y"
    { yyval.assoc = non_assoc; }
    break;

  case 40:
#line 274 "parse-gram.y"
    { current_type = NULL;}
    break;

  case 41:
#line 275 "parse-gram.y"
    { current_type = yyvsp[0].string; }
    break;

  case 42:
#line 281 "parse-gram.y"
    { yyval.list = symbol_list_new (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 43:
#line 282 "parse-gram.y"
    { yyval.list = symbol_list_prepend (yyvsp[-1].list, yyvsp[0].symbol, yylsp[0]); }
    break;

  case 44:
#line 288 "parse-gram.y"
    {
       current_type = yyvsp[0].string;
     }
    break;

  case 45:
#line 292 "parse-gram.y"
    {
       symbol_class_set (yyvsp[0].symbol, current_class, yylsp[0]);
       symbol_type_set (yyvsp[0].symbol, current_type, yylsp[0]);
     }
    break;

  case 46:
#line 297 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class, yylsp[-1]);
      symbol_type_set (yyvsp[-1].symbol, current_type, yylsp[-1]);
      symbol_user_token_number_set (yyvsp[-1].symbol, yyvsp[0].integer, yylsp[0]);
    }
    break;

  case 47:
#line 303 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class, yylsp[-1]);
      symbol_type_set (yyvsp[-1].symbol, current_type, yylsp[-1]);
      symbol_make_alias (yyvsp[-1].symbol, yyvsp[0].symbol);
    }
    break;

  case 48:
#line 309 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-2].symbol, current_class, yylsp[-2]);
      symbol_type_set (yyvsp[-2].symbol, current_type, yylsp[-2]);
      symbol_user_token_number_set (yyvsp[-2].symbol, yyvsp[-1].integer, yylsp[-1]);
      symbol_make_alias (yyvsp[-2].symbol, yyvsp[0].symbol);
    }
    break;

  case 49:
#line 320 "parse-gram.y"
    {;}
    break;

  case 50:
#line 322 "parse-gram.y"
    {;}
    break;

  case 55:
#line 344 "parse-gram.y"
    { current_lhs = yyvsp[-1].symbol; current_lhs_location = yylsp[-1]; }
    break;

  case 56:
#line 345 "parse-gram.y"
    {;}
    break;

  case 57:
#line 349 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 58:
#line 350 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 59:
#line 355 "parse-gram.y"
    { grammar_rule_begin (current_lhs, current_lhs_location); }
    break;

  case 60:
#line 357 "parse-gram.y"
    { grammar_current_rule_symbol_append (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 61:
#line 359 "parse-gram.y"
    { grammar_current_rule_action_append (yyvsp[0].string, yylsp[0]); }
    break;

  case 62:
#line 361 "parse-gram.y"
    { grammar_current_rule_prec_set (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 63:
#line 363 "parse-gram.y"
    { grammar_current_rule_dprec_set (yyvsp[0].integer, yylsp[0]); }
    break;

  case 64:
#line 365 "parse-gram.y"
    { grammar_current_rule_merge_set (yyvsp[0].string, yylsp[0]); }
    break;

  case 65:
#line 369 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 66:
#line 370 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 67:
#line 371 "parse-gram.y"
    { yyval.symbol = symbol_get (yyvsp[0].string, yylsp[0]); }
    break;

  case 68:
#line 376 "parse-gram.y"
    { yyval.string = yyvsp[0].string; }
    break;

  case 69:
#line 382 "parse-gram.y"
    {
      yyval.symbol = symbol_get (yyvsp[0].string, yylsp[0]);
      symbol_class_set (yyval.symbol, token_sym, yylsp[0]);
    }
    break;

  case 70:
#line 391 "parse-gram.y"
    {
      yyval.string = yyvsp[0].string + 1;
      yyval.string[strlen (yyval.string) - 1] = '\0';
    }
    break;

  case 71:
#line 399 "parse-gram.y"
    {
      yyval.string = xstrdup ("");
    }
    break;

  case 72:
#line 403 "parse-gram.y"
    {
      yyval.string = yyvsp[0].string;
    }
    break;


    }

/* Line 1040 of /usr/local/share/bison/yacc.c.  */
#line 1525 "parse-gram.c"

  yyvsp -= yylen;
  yyssp -= yylen;
  yylsp -= yylen;

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
  *++yylsp = yyloc;

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
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyssp > yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*yyssp],
			    *yyvsp, *yylsp));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*yyssp], *yyvsp, *yylsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yydestruct (yychar1, yylval, yylloc);
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

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*yyssp], *yyvsp, *yylsp));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp, *yylsp);
      yyvsp--;
      yystate = *--yyssp;
      yylsp--;

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
  *++yylsp = yylloc;

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
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (int yytype,
	    YYSTYPE yyvalue, YYLTYPE yylocation)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;
  (void) yylocation;

  switch (yytype)
    {
      default:
        break;
    }
}


#if YYDEBUG
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

static void
yysymprint (FILE* yyout, int yytype,
	    YYSTYPE yyvalue, YYLTYPE yylocation)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;
  (void) yylocation;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[yytype], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */

#line 412 "parse-gram.y"

/*------------------------------------------------------------------.
| When debugging the parser, display tokens' locations and values.  |
`------------------------------------------------------------------*/

static void
yyprint (FILE *file,
         int type, const yystype *value)
{
  fputc (' ', file);
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

