/* A Bison parser, made from parse-gram.y, by GNU bison 1.75b.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     GRAM_EOF = 0,
     STRING = 258,
     INT = 259,
     PERCENT_TOKEN = 260,
     PERCENT_NTERM = 261,
     PERCENT_TYPE = 262,
     PERCENT_DESTRUCTOR = 263,
     PERCENT_PRINTER = 264,
     PERCENT_UNION = 265,
     PERCENT_LEFT = 266,
     PERCENT_RIGHT = 267,
     PERCENT_NONASSOC = 268,
     PERCENT_PREC = 269,
     PERCENT_DPREC = 270,
     PERCENT_MERGE = 271,
     PERCENT_DEBUG = 272,
     PERCENT_DEFINE = 273,
     PERCENT_DEFINES = 274,
     PERCENT_ERROR_VERBOSE = 275,
     PERCENT_EXPECT = 276,
     PERCENT_FILE_PREFIX = 277,
     PERCENT_GLR_PARSER = 278,
     PERCENT_LEX_PARAM = 279,
     PERCENT_LOCATIONS = 280,
     PERCENT_NAME_PREFIX = 281,
     PERCENT_NO_LINES = 282,
     PERCENT_OUTPUT = 283,
     PERCENT_PARSE_PARAM = 284,
     PERCENT_PURE_PARSER = 285,
     PERCENT_SKELETON = 286,
     PERCENT_START = 287,
     PERCENT_TOKEN_TABLE = 288,
     PERCENT_VERBOSE = 289,
     PERCENT_YACC = 290,
     TYPE = 291,
     EQUAL = 292,
     SEMICOLON = 293,
     COLON = 294,
     COMMA = 295,
     PIPE = 296,
     ID = 297,
     PERCENT_PERCENT = 298,
     PROLOGUE = 299,
     EPILOGUE = 300,
     BRACED_CODE = 301
   };
#endif
#define GRAM_EOF 0
#define STRING 258
#define INT 259
#define PERCENT_TOKEN 260
#define PERCENT_NTERM 261
#define PERCENT_TYPE 262
#define PERCENT_DESTRUCTOR 263
#define PERCENT_PRINTER 264
#define PERCENT_UNION 265
#define PERCENT_LEFT 266
#define PERCENT_RIGHT 267
#define PERCENT_NONASSOC 268
#define PERCENT_PREC 269
#define PERCENT_DPREC 270
#define PERCENT_MERGE 271
#define PERCENT_DEBUG 272
#define PERCENT_DEFINE 273
#define PERCENT_DEFINES 274
#define PERCENT_ERROR_VERBOSE 275
#define PERCENT_EXPECT 276
#define PERCENT_FILE_PREFIX 277
#define PERCENT_GLR_PARSER 278
#define PERCENT_LEX_PARAM 279
#define PERCENT_LOCATIONS 280
#define PERCENT_NAME_PREFIX 281
#define PERCENT_NO_LINES 282
#define PERCENT_OUTPUT 283
#define PERCENT_PARSE_PARAM 284
#define PERCENT_PURE_PARSER 285
#define PERCENT_SKELETON 286
#define PERCENT_START 287
#define PERCENT_TOKEN_TABLE 288
#define PERCENT_VERBOSE 289
#define PERCENT_YACC 290
#define TYPE 291
#define EQUAL 292
#define SEMICOLON 293
#define COLON 294
#define COMMA 295
#define PIPE 296
#define ID 297
#define PERCENT_PERCENT 298
#define PROLOGUE 299
#define EPILOGUE 300
#define BRACED_CODE 301




/* Copy the first part of user declarations.  */
#line 31 "parse-gram.y"

#include "system.h"
#include "complain.h"
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

/* Request detailed parse error messages, and pass them to GRAM_ERROR.
   FIXME: depends on the undocumented availability of YYLLOC.t */
#undef  yyerror
#define yyerror(Msg) \
        gram_error (&yylloc, Msg)

#define YYPRINT(File, Type, Value) \
        yyprint (File, Type, &Value)
static void yyprint (FILE *file, int type, const yystype *value);

symbol_class current_class = unknown_sym;
struniq_t current_type = 0;
symbol_t *current_lhs;
location_t current_lhs_location;
assoc_t current_assoc;
int current_prec = 0;
braced_code_t current_braced_code = action_braced_code;


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
#line 89 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  symbol_list_t *list;
  int integer;
  char *string;
  assoc_t assoc;
  struniq_t struniq;
} yystype;
/* Line 237 of /usr/local/share/bison/yacc.c.  */
#line 243 "parse-gram.c"
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


/* Line 257 of /usr/local/share/bison/yacc.c.  */
#line 264 "parse-gram.c"

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

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   125

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  28
/* YYNRULES -- Number of rules. */
#define YYNRULES  76
/* YYNRULES -- Number of states. */
#define YYNSTATES  115

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     8,     9,    13,    15,    17,    19,    23,
      25,    27,    30,    34,    36,    41,    43,    47,    49,    53,
      58,    60,    63,    65,    67,    69,    71,    73,    76,    79,
      80,    85,    86,    91,    92,    96,    97,   101,   105,   109,
     111,   113,   115,   116,   118,   120,   123,   125,   127,   130,
     133,   137,   139,   142,   144,   147,   149,   152,   155,   156,
     162,   164,   168,   169,   172,   175,   179,   183,   187,   189,
     191,   193,   195,   197,   198,   201,   202
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      48,     0,    -1,    49,    43,    63,    73,    -1,    -1,    49,
      50,    74,    -1,    51,    -1,    44,    -1,    17,    -1,    18,
      72,    72,    -1,    19,    -1,    20,    -1,    21,     4,    -1,
      22,    37,    72,    -1,    23,    -1,    24,    72,    40,    72,
      -1,    25,    -1,    26,    37,    72,    -1,    27,    -1,    28,
      37,    72,    -1,    29,    72,    40,    72,    -1,    30,    -1,
      31,    72,    -1,    33,    -1,    34,    -1,    35,    -1,    57,
      -1,    54,    -1,    32,    69,    -1,    10,    46,    -1,    -1,
       8,    52,    46,    60,    -1,    -1,     9,    53,    46,    60,
      -1,    -1,     6,    55,    62,    -1,    -1,     5,    56,    62,
      -1,     7,    36,    60,    -1,    58,    59,    60,    -1,    11,
      -1,    12,    -1,    13,    -1,    -1,    36,    -1,    69,    -1,
      60,    69,    -1,    36,    -1,    42,    -1,    42,     4,    -1,
      42,    71,    -1,    42,     4,    71,    -1,    61,    -1,    62,
      61,    -1,    64,    -1,    63,    64,    -1,    65,    -1,    51,
      38,    -1,     1,    38,    -1,    -1,    42,    39,    66,    67,
      38,    -1,    68,    -1,    67,    41,    68,    -1,    -1,    68,
      69,    -1,    68,    70,    -1,    68,    14,    69,    -1,    68,
      15,     4,    -1,    68,    16,    36,    -1,    42,    -1,    71,
      -1,    46,    -1,     3,    -1,     3,    -1,    -1,    43,    45,
      -1,    -1,    38,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   170,   170,   182,   184,   187,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   199,   200,   201,   202,   203,
     205,   206,   207,   208,   209,   212,   214,   215,   219,   226,
     225,   236,   235,   248,   247,   253,   253,   258,   267,   282,
     284,   285,   288,   290,   295,   297,   301,   306,   311,   317,
     323,   333,   336,   345,   347,   353,   355,   360,   367,   366,
     371,   373,   376,   379,   381,   383,   385,   387,   391,   393,
     396,   402,   411,   419,   421,   428,   430
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"string\"", "\"integer\"", 
  "\"%token\"", "\"%nterm\"", "\"%type\"", "\"%destructor\"", 
  "\"%printer\"", "\"%union\"", "\"%left\"", "\"%right\"", 
  "\"%nonassoc\"", "\"%prec\"", "\"%dprec\"", "\"%merge\"", "\"%debug\"", 
  "\"%define\"", "\"%defines\"", "\"%error-verbose\"", "\"%expect\"", 
  "\"%file-prefix\"", "\"%glr-parser\"", "\"%lex-param\"", 
  "\"%locations\"", "\"%name-prefix\"", "\"%no-lines\"", "\"%output\"", 
  "\"%parse-param\"", "\"%pure-parser\"", "\"%skeleton\"", "\"%start\"", 
  "\"%token-table\"", "\"%verbose\"", "\"%yacc\"", "\"type\"", "\"=\"", 
  "\";\"", "\":\"", "\",\"", "\"|\"", "\"identifier\"", "\"%%\"", 
  "\"%{...%}\"", "\"epilogue\"", "\"{...}\"", "$accept", "input", 
  "declarations", "declaration", "grammar_declaration", "@1", "@2", 
  "symbol_declaration", "@3", "@4", "precedence_declaration", 
  "precedence_declarator", "type.opt", "symbols.1", "symbol_def", 
  "symbol_defs.1", "grammar", "rules_or_grammar_declaration", "rules", 
  "@5", "rhses.1", "rhs", "symbol", "action", "string_as_id", 
  "string_content", "epilogue.opt", "semi_colon.opt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    47,    48,    49,    49,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    51,    51,    51,    51,    52,
      51,    53,    51,    55,    54,    56,    54,    54,    57,    58,
      58,    58,    59,    59,    60,    60,    61,    61,    61,    61,
      61,    62,    62,    63,    63,    64,    64,    64,    66,    65,
      67,    67,    68,    68,    68,    68,    68,    68,    69,    69,
      70,    71,    72,    73,    73,    74,    74
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     0,     3,     1,     1,     1,     3,     1,
       1,     2,     3,     1,     4,     1,     3,     1,     3,     4,
       1,     2,     1,     1,     1,     1,     1,     2,     2,     0,
       4,     0,     4,     0,     3,     0,     3,     3,     3,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     2,     2,
       3,     1,     2,     1,     2,     1,     2,     2,     0,     5,
       1,     3,     0,     2,     2,     3,     3,     3,     1,     1,
       1,     1,     1,     0,     2,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,     0,     0,     1,    35,    33,     0,    29,    31,     0,
      39,    40,    41,     7,     0,     9,    10,     0,     0,    13,
       0,    15,     0,    17,     0,     0,    20,     0,     0,    22,
      23,    24,     0,     6,    75,     5,    26,    25,    42,     0,
       0,     0,     0,     0,    28,    72,     0,    11,     0,     0,
       0,     0,     0,    21,    71,    68,    27,    69,     0,     0,
       0,     0,    53,    55,    76,     4,    43,     0,    46,    47,
      51,    36,    34,    37,    44,     0,     0,     8,    12,     0,
      16,    18,     0,    57,    58,    56,     0,    54,     2,    38,
      48,    49,    52,    45,    30,    32,    14,    19,    62,    74,
      50,     0,    60,    59,    62,     0,     0,     0,    70,    63,
      64,    61,    65,    66,    67
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     1,     2,    34,    60,    42,    43,    36,    40,    39,
      37,    38,    67,    73,    70,    71,    61,    62,    63,    98,
     101,   102,    74,   110,    57,    46,    88,    65
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -67
static const yysigned_char yypact[] =
{
     -67,     7,    81,   -67,   -67,   -67,   -13,   -67,   -67,   -16,
     -67,   -67,   -67,   -67,    32,   -67,   -67,    36,    20,   -67,
      32,   -67,    21,   -67,    22,    32,   -67,    32,    -1,   -67,
     -67,   -67,    43,   -67,    24,   -67,   -67,   -67,    28,   -14,
     -14,    -1,    25,    26,   -67,   -67,    32,   -67,    32,    30,
      32,    32,    33,   -67,   -67,   -67,   -67,   -67,    38,    29,
      40,     4,   -67,   -67,   -67,   -67,   -67,    -1,   -67,    16,
     -67,   -14,   -14,    -1,   -67,    -1,    -1,   -67,   -67,    32,
     -67,   -67,    32,   -67,   -67,   -67,    34,   -67,   -67,    -1,
      77,   -67,   -67,   -67,    -1,    -1,   -67,   -67,   -67,   -67,
     -67,   -20,    23,   -67,   -67,    -1,    78,    45,   -67,   -67,
     -67,    23,   -67,   -67,   -67
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -67,   -67,   -67,   -67,    82,   -67,   -67,   -67,   -67,   -67,
     -67,   -67,   -67,   -42,   -29,    55,   -67,    35,   -67,   -67,
     -67,    -7,   -28,   -67,   -66,   -19,   -67,   -67
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -74
static const yysigned_char yytable[] =
{
      56,    49,    54,    91,   -73,    58,    52,     3,    53,     4,
       5,     6,     7,     8,     9,    10,    11,    12,   103,    54,
      90,   104,    68,    41,   100,    89,    54,    77,    69,    78,
      44,    80,    81,    94,    95,    45,    28,   105,   106,   107,
      47,    55,    92,    92,    58,    93,    59,    86,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    48,    50,    51,
      96,    93,    64,    97,    66,    55,    93,    93,    84,   108,
      79,    75,    76,    82,   109,    28,    83,   112,    85,    99,
      54,   114,   113,   109,    35,    59,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    72,    87,   111,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,    32,    33
};

static const yysigned_char yycheck[] =
{
      28,    20,     3,    69,     0,     1,    25,     0,    27,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    38,     3,
       4,    41,    36,    36,    90,    67,     3,    46,    42,    48,
      46,    50,    51,    75,    76,     3,    32,    14,    15,    16,
       4,    42,    71,    72,     1,    73,    42,    43,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    37,    37,    37,
      79,    89,    38,    82,    36,    42,    94,    95,    39,    46,
      40,    46,    46,    40,   102,    32,    38,   105,    38,    45,
       3,    36,     4,   111,     2,    42,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    40,    61,   104,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    48,    49,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    43,    44,    50,    51,    54,    57,    58,    56,
      55,    36,    52,    53,    46,     3,    72,     4,    37,    72,
      37,    37,    72,    72,     3,    42,    69,    71,     1,    42,
      51,    63,    64,    65,    38,    74,    36,    59,    36,    42,
      61,    62,    62,    60,    69,    46,    46,    72,    72,    40,
      72,    72,    40,    38,    39,    38,    43,    64,    73,    60,
       4,    71,    61,    69,    60,    60,    72,    72,    66,    45,
      71,    67,    68,    38,    41,    14,    15,    16,    46,    69,
      70,    68,    69,     4,    36
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
      yyerror ("syntax error: cannot back up");	\
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
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc)
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



#if YYDEBUG
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyout, int yytype, YYSTYPE yyvalue, YYLTYPE yylocation)
#else
static void
yysymprint (yyout, yytype, yyvalue, yylocation)
    FILE *yyout;
    int yytype;
    YYSTYPE yyvalue;
    YYLTYPE yylocation;
#endif
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


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE yyvalue, YYLTYPE yylocation)
#else
static void
yydestruct (yytype, yyvalue, yylocation)
    int yytype;
    YYSTYPE yyvalue;
    YYLTYPE yylocation;
#endif
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



/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
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

  if (yyss + yystacksize - 1 <= yyssp)
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
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
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

      if (yyss + yystacksize - 1 <= yyssp)
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
      YYDSYMPRINT ((stderr, yychar1, yylval, yyloc));
      YYDPRINTF ((stderr, "\n"));
    }

  /* If the proper action on seeing token YYCHAR1 is to reduce or to
     detect an error, take that action.  */
  yyn += yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yychar1)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

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

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
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
      for (yyi = yyprhs[yyn]; 0 <= yyrhs[yyi]; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    {
        case 2:
#line 172 "parse-gram.y"
    {
      yycontrol->errcode = 0;
    }
    break;

  case 6:
#line 189 "parse-gram.y"
    { prologue_augment (yyvsp[0].string, yylsp[0]); }
    break;

  case 7:
#line 190 "parse-gram.y"
    { debug_flag = 1; }
    break;

  case 8:
#line 191 "parse-gram.y"
    { muscle_insert (yyvsp[-1].string, yyvsp[0].string); }
    break;

  case 9:
#line 192 "parse-gram.y"
    { defines_flag = 1; }
    break;

  case 10:
#line 193 "parse-gram.y"
    { error_verbose = 1; }
    break;

  case 11:
#line 194 "parse-gram.y"
    { expected_conflicts = yyvsp[0].integer; }
    break;

  case 12:
#line 195 "parse-gram.y"
    { spec_file_prefix = yyvsp[0].string; }
    break;

  case 13:
#line 196 "parse-gram.y"
    { glr_parser = 1; }
    break;

  case 14:
#line 198 "parse-gram.y"
    { muscle_pair_list_grow ("lex_param", yyvsp[-2].string, yyvsp[0].string); }
    break;

  case 15:
#line 199 "parse-gram.y"
    { locations_flag = 1; }
    break;

  case 16:
#line 200 "parse-gram.y"
    { spec_name_prefix = yyvsp[0].string; }
    break;

  case 17:
#line 201 "parse-gram.y"
    { no_lines_flag = 1; }
    break;

  case 18:
#line 202 "parse-gram.y"
    { spec_outfile = yyvsp[0].string; }
    break;

  case 19:
#line 204 "parse-gram.y"
    { muscle_pair_list_grow ("parse_param", yyvsp[-2].string, yyvsp[0].string); }
    break;

  case 20:
#line 205 "parse-gram.y"
    { pure_parser = 1; }
    break;

  case 21:
#line 206 "parse-gram.y"
    { skeleton = yyvsp[0].string; }
    break;

  case 22:
#line 207 "parse-gram.y"
    { token_table_flag = 1; }
    break;

  case 23:
#line 208 "parse-gram.y"
    { report_flag = 1; }
    break;

  case 24:
#line 209 "parse-gram.y"
    { yacc_flag = 1; }
    break;

  case 27:
#line 216 "parse-gram.y"
    {
      grammar_start_symbol_set (yyvsp[0].symbol, yylsp[0]);
    }
    break;

  case 28:
#line 220 "parse-gram.y"
    {
      typed = 1;
      MUSCLE_INSERT_INT ("stype_line", yylsp[0].first_line);
      muscle_insert ("stype", yyvsp[0].string);
    }
    break;

  case 29:
#line 226 "parse-gram.y"
    { current_braced_code = destructor_braced_code; }
    break;

  case 30:
#line 228 "parse-gram.y"
    {
      symbol_list_t *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_destructor_set (list->sym, yyvsp[-1].string, yylsp[-1]);
      symbol_list_free (yyvsp[0].list);
      current_braced_code = action_braced_code;
    }
    break;

  case 31:
#line 236 "parse-gram.y"
    { current_braced_code = printer_braced_code; }
    break;

  case 32:
#line 238 "parse-gram.y"
    {
      symbol_list_t *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_printer_set (list->sym, yyvsp[-1].string, list->location);
      symbol_list_free (yyvsp[0].list);
      current_braced_code = action_braced_code;
    }
    break;

  case 33:
#line 248 "parse-gram.y"
    { current_class = nterm_sym; }
    break;

  case 34:
#line 249 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 35:
#line 253 "parse-gram.y"
    { current_class = token_sym; }
    break;

  case 36:
#line 254 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 37:
#line 259 "parse-gram.y"
    {
      symbol_list_t *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_type_set (list->sym, yyvsp[-1].struniq, yylsp[-1]);
      symbol_list_free (yyvsp[0].list);
    }
    break;

  case 38:
#line 269 "parse-gram.y"
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

  case 39:
#line 283 "parse-gram.y"
    { yyval.assoc = left_assoc; }
    break;

  case 40:
#line 284 "parse-gram.y"
    { yyval.assoc = right_assoc; }
    break;

  case 41:
#line 285 "parse-gram.y"
    { yyval.assoc = non_assoc; }
    break;

  case 42:
#line 289 "parse-gram.y"
    { current_type = NULL; }
    break;

  case 43:
#line 290 "parse-gram.y"
    { current_type = yyvsp[0].struniq; }
    break;

  case 44:
#line 296 "parse-gram.y"
    { yyval.list = symbol_list_new (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 45:
#line 297 "parse-gram.y"
    { yyval.list = symbol_list_prepend (yyvsp[-1].list, yyvsp[0].symbol, yylsp[0]); }
    break;

  case 46:
#line 303 "parse-gram.y"
    {
       current_type = yyvsp[0].struniq;
     }
    break;

  case 47:
#line 307 "parse-gram.y"
    {
       symbol_class_set (yyvsp[0].symbol, current_class, yylsp[0]);
       symbol_type_set (yyvsp[0].symbol, current_type, yylsp[0]);
     }
    break;

  case 48:
#line 312 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class, yylsp[-1]);
      symbol_type_set (yyvsp[-1].symbol, current_type, yylsp[-1]);
      symbol_user_token_number_set (yyvsp[-1].symbol, yyvsp[0].integer, yylsp[0]);
    }
    break;

  case 49:
#line 318 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class, yylsp[-1]);
      symbol_type_set (yyvsp[-1].symbol, current_type, yylsp[-1]);
      symbol_make_alias (yyvsp[-1].symbol, yyvsp[0].symbol, yyloc);
    }
    break;

  case 50:
#line 324 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-2].symbol, current_class, yylsp[-2]);
      symbol_type_set (yyvsp[-2].symbol, current_type, yylsp[-2]);
      symbol_user_token_number_set (yyvsp[-2].symbol, yyvsp[-1].integer, yylsp[-1]);
      symbol_make_alias (yyvsp[-2].symbol, yyvsp[0].symbol, yyloc);
    }
    break;

  case 51:
#line 335 "parse-gram.y"
    {;}
    break;

  case 52:
#line 337 "parse-gram.y"
    {;}
    break;

  case 56:
#line 356 "parse-gram.y"
    {
      if (yacc_flag)
	complain_at (yyloc, _("POSIX forbids declarations in the grammar"));
    }
    break;

  case 57:
#line 361 "parse-gram.y"
    {
      yyerrok;
    }
    break;

  case 58:
#line 367 "parse-gram.y"
    { current_lhs = yyvsp[-1].symbol; current_lhs_location = yylsp[-1]; }
    break;

  case 59:
#line 368 "parse-gram.y"
    {;}
    break;

  case 60:
#line 372 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 61:
#line 373 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 62:
#line 378 "parse-gram.y"
    { grammar_rule_begin (current_lhs, current_lhs_location); }
    break;

  case 63:
#line 380 "parse-gram.y"
    { grammar_current_rule_symbol_append (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 64:
#line 382 "parse-gram.y"
    { grammar_current_rule_action_append (yyvsp[0].string, yylsp[0]); }
    break;

  case 65:
#line 384 "parse-gram.y"
    { grammar_current_rule_prec_set (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 66:
#line 386 "parse-gram.y"
    { grammar_current_rule_dprec_set (yyvsp[0].integer, yylsp[0]); }
    break;

  case 67:
#line 388 "parse-gram.y"
    { grammar_current_rule_merge_set (yyvsp[0].struniq, yylsp[0]); }
    break;

  case 68:
#line 392 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 69:
#line 393 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 70:
#line 398 "parse-gram.y"
    { yyval.string = yyvsp[0].string; }
    break;

  case 71:
#line 404 "parse-gram.y"
    {
      yyval.symbol = symbol_get (yyvsp[0].string, yylsp[0]);
      symbol_class_set (yyval.symbol, token_sym, yylsp[0]);
    }
    break;

  case 72:
#line 413 "parse-gram.y"
    {
      yyval.string = yyvsp[0].string + 1;
      yyval.string[strlen (yyval.string) - 1] = '\0';
    }
    break;

  case 74:
#line 422 "parse-gram.y"
    {
      epilogue_augment (yyvsp[0].string, yylsp[0]);
      scanner_last_string_free ();
    }
    break;


    }

/* Line 1056 of /usr/local/share/bison/yacc.c.  */
#line 1616 "parse-gram.c"

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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

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
	  while (yyss < yyssp)
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


#line 432 "parse-gram.y"

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
      fprintf (file, " = <%s>", value->struniq);
      break;

    case BRACED_CODE:
    case PROLOGUE:
    case EPILOGUE:
      fprintf (file, " = {{ %s }}", value->string);
      break;

    default:
      fprintf (file, "unknown token type");
      break;
    }
}

void
gram_error (location_t *yylloc, const char *msg)
{
  complain_at (*yylloc, "%s", msg);
}

