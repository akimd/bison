/* A Bison parser, made by GNU Bison 1.875.  */

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
     PIPE = 294,
     ID = 295,
     ID_COLON = 296,
     PERCENT_PERCENT = 297,
     PROLOGUE = 298,
     EPILOGUE = 299,
     BRACED_CODE = 300
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
#define PIPE 294
#define ID 295
#define ID_COLON 296
#define PERCENT_PERCENT 297
#define PROLOGUE 298
#define EPILOGUE 299
#define BRACED_CODE 300




/* Copy the first part of user declarations.  */
#line 31 "parse-gram.y"

#include "system.h"

#include "complain.h"
#include "conflicts.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "muscle_tab.h"
#include "output.h"
#include "reader.h"
#include "symlist.h"

/* Produce verbose syntax errors.  */
#define YYERROR_VERBOSE 1

#define YYLLOC_DEFAULT(Current, Rhs, N)  (Current) = lloc_default (Rhs, N)
static YYLTYPE lloc_default (YYLTYPE const *, int);

/* Request detailed syntax error messages, and pass them to GRAM_ERROR.
   FIXME: depends on the undocumented availability of YYLLOC.  */
#undef  yyerror
#define yyerror(Msg) \
        gram_error (&yylloc, Msg)
static void gram_error (location const *, char const *);

#define YYPRINT(File, Type, Value) \
	print_token_value (File, Type, &Value)
static void print_token_value (FILE *, int, YYSTYPE const *);

static void add_param (char const *, char *, location);

symbol_class current_class = unknown_sym;
uniqstr current_type = 0;
symbol *current_lhs;
location current_lhs_location;
assoc current_assoc;
int current_prec = 0;


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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 74 "parse-gram.y"
typedef union YYSTYPE {
  symbol *symbol;
  symbol_list *list;
  int integer;
  char *chars;
  assoc assoc;
  uniqstr uniqstr;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 225 "parse-gram.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 249 "parse-gram.c"

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
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAXIMUM)

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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
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
#define YYLAST   157

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  25
/* YYNRULES -- Number of rules. */
#define YYNRULES  74
/* YYNRULES -- Number of states. */
#define YYNSTATES  101

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     8,     9,    12,    14,    16,    18,    22,
      24,    26,    29,    33,    35,    37,    39,    43,    45,    49,
      51,    53,    56,    58,    60,    62,    64,    66,    68,    71,
      73,    76,    79,    80,    84,    85,    89,    93,    97,    99,
     101,   103,   104,   106,   108,   111,   113,   115,   118,   121,
     125,   127,   130,   132,   135,   137,   139,   142,   144,   145,
     149,   151,   155,   156,   159,   162,   166,   170,   174,   176,
     178,   180,   182,   184,   185
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      47,     0,    -1,    48,    42,    60,    70,    -1,    -1,    48,
      49,    -1,    50,    -1,    43,    -1,    17,    -1,    18,    69,
      69,    -1,    19,    -1,    20,    -1,    21,     4,    -1,    22,
      37,    69,    -1,    23,    -1,    24,    -1,    25,    -1,    26,
      37,    69,    -1,    27,    -1,    28,    37,    69,    -1,    29,
      -1,    30,    -1,    31,    69,    -1,    33,    -1,    34,    -1,
      35,    -1,    38,    -1,    54,    -1,    51,    -1,    32,    66,
      -1,    10,    -1,     8,    57,    -1,     9,    57,    -1,    -1,
       6,    52,    59,    -1,    -1,     5,    53,    59,    -1,     7,
      36,    57,    -1,    55,    56,    57,    -1,    11,    -1,    12,
      -1,    13,    -1,    -1,    36,    -1,    66,    -1,    57,    66,
      -1,    36,    -1,    40,    -1,    40,     4,    -1,    40,    68,
      -1,    40,     4,    68,    -1,    58,    -1,    59,    58,    -1,
      61,    -1,    60,    61,    -1,    62,    -1,    50,    -1,     1,
      38,    -1,    38,    -1,    -1,    41,    63,    64,    -1,    65,
      -1,    64,    39,    65,    -1,    -1,    65,    66,    -1,    65,
      67,    -1,    65,    14,    66,    -1,    65,    15,     4,    -1,
      65,    16,    36,    -1,    40,    -1,    68,    -1,    45,    -1,
       3,    -1,     3,    -1,    -1,    42,    44,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   160,   160,   168,   170,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   198,   199,   200,   204,
     210,   217,   227,   227,   232,   232,   237,   247,   262,   263,
     264,   268,   269,   275,   276,   281,   285,   290,   296,   302,
     313,   314,   323,   324,   330,   331,   336,   340,   344,   344,
     348,   349,   354,   355,   357,   359,   361,   363,   368,   369,
     373,   379,   388,   395,   397
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"string\"", "\"integer\"", 
  "\"%token\"", "\"%nterm\"", "\"%type\"", "\"%destructor {...}\"", 
  "\"%printer {...}\"", "\"%union {...}\"", "\"%left\"", "\"%right\"", 
  "\"%nonassoc\"", "\"%prec\"", "\"%dprec\"", "\"%merge\"", "\"%debug\"", 
  "\"%define\"", "\"%defines\"", "\"%error-verbose\"", "\"%expect\"", 
  "\"%file-prefix\"", "\"%glr-parser\"", "\"%lex-param {...}\"", 
  "\"%locations\"", "\"%name-prefix\"", "\"%no-lines\"", "\"%output\"", 
  "\"%parse-param {...}\"", "\"%pure-parser\"", "\"%skeleton\"", 
  "\"%start\"", "\"%token-table\"", "\"%verbose\"", "\"%yacc\"", 
  "\"type\"", "\"=\"", "\";\"", "\"|\"", "\"identifier\"", 
  "\"identifier:\"", "\"%%\"", "\"%{...%}\"", "\"epilogue\"", "\"{...}\"", 
  "$accept", "input", "declarations", "declaration", 
  "grammar_declaration", "symbol_declaration", "@1", "@2", 
  "precedence_declaration", "precedence_declarator", "type.opt", 
  "symbols.1", "symbol_def", "symbol_defs.1", "grammar", 
  "rules_or_grammar_declaration", "rules", "@3", "rhses.1", "rhs", 
  "symbol", "action", "string_as_id", "string_content", "epilogue.opt", 0
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
     295,   296,   297,   298,   299,   300
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    46,    47,    48,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    50,    50,    50,    50,
      50,    50,    52,    51,    53,    51,    51,    54,    55,    55,
      55,    56,    56,    57,    57,    58,    58,    58,    58,    58,
      59,    59,    60,    60,    61,    61,    61,    61,    63,    62,
      64,    64,    65,    65,    65,    65,    65,    65,    66,    66,
      67,    68,    69,    70,    70
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     0,     2,     1,     1,     1,     3,     1,
       1,     2,     3,     1,     1,     1,     3,     1,     3,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     2,     0,     3,     0,     3,     3,     3,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     2,     2,     3,
       1,     2,     1,     2,     1,     1,     2,     1,     0,     3,
       1,     3,     0,     2,     2,     3,     3,     3,     1,     1,
       1,     1,     1,     0,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,     0,     0,     1,    34,    32,     0,     0,     0,    29,
      38,    39,    40,     7,     0,     9,    10,     0,     0,    13,
      14,    15,     0,    17,     0,    19,    20,     0,     0,    22,
      23,    24,    25,     0,     6,     4,     5,    27,    26,    41,
       0,     0,     0,    71,    68,    30,    43,    69,    31,    72,
       0,    11,     0,     0,     0,    21,    28,     0,    57,    58,
      55,     0,    52,    54,    42,     0,    45,    46,    50,    35,
      33,    36,    44,     8,    12,    16,    18,    56,    62,     0,
      53,     2,    37,    47,    48,    51,    59,    60,    74,    49,
      62,     0,     0,     0,    70,    63,    64,    61,    65,    66,
      67
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     1,     2,    35,    60,    37,    41,    40,    38,    39,
      65,    45,    68,    69,    61,    62,    63,    78,    86,    87,
      46,    96,    47,    50,    81
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -65
static const yysigned_char yypact[] =
{
     -65,     5,   114,   -65,   -65,   -65,   -24,     1,     1,   -65,
     -65,   -65,   -65,   -65,    10,   -65,   -65,    11,   -23,   -65,
     -65,   -65,   -21,   -65,   -15,   -65,   -65,    10,     1,   -65,
     -65,   -65,   -65,    77,   -65,   -65,   -65,   -65,   -65,   -13,
     -34,   -34,     1,   -65,   -65,     1,   -65,   -65,     1,   -65,
      10,   -65,    10,    10,    10,   -65,   -65,   -17,   -65,   -65,
     -65,    39,   -65,   -65,   -65,     1,   -65,     4,   -65,   -34,
     -34,     1,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -20,
     -65,   -65,     1,    22,   -65,   -65,   -12,    15,   -65,   -65,
     -65,     1,    24,   -10,   -65,   -65,   -65,    15,   -65,   -65,
     -65
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -65,   -65,   -65,   -65,    30,   -65,   -65,   -65,   -65,   -65,
     -65,    -7,   -60,    -8,   -65,   -19,   -65,   -65,   -65,   -37,
     -28,   -65,   -64,   -16,   -65
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -74
static const yysigned_char yytable[] =
{
      56,    48,    66,    84,    43,     3,    67,    43,    83,    85,
      85,    55,    42,    49,    52,    51,    53,    72,    43,    89,
      72,    77,    54,    64,    88,    43,   100,    90,    99,    91,
      92,    93,    36,    70,    73,    71,    74,    75,    76,   -73,
      57,    44,    80,    72,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    97,    72,    44,     0,     0,    82,    95,
      94,     0,     0,    98,     0,     0,     0,     0,     0,    95,
       0,    28,     0,     0,     0,     0,     0,    58,    57,     0,
      59,    79,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,     0,    58,     0,     0,    59,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       0,     0,    32,     0,     0,     0,    33,    34
};

static const yysigned_char yycheck[] =
{
      28,     8,    36,    67,     3,     0,    40,     3,     4,    69,
      70,    27,    36,     3,    37,     4,    37,    45,     3,    83,
      48,    38,    37,    36,    44,     3,    36,    39,     4,    14,
      15,    16,     2,    41,    50,    42,    52,    53,    54,     0,
       1,    40,    61,    71,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    90,    82,    40,    -1,    -1,    65,    87,
      45,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,     1,    -1,
      41,    42,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    42,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    47,    48,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    38,    42,    43,    49,    50,    51,    54,    55,
      53,    52,    36,     3,    40,    57,    66,    68,    57,     3,
      69,     4,    37,    37,    37,    69,    66,     1,    38,    41,
      50,    60,    61,    62,    36,    56,    36,    40,    58,    59,
      59,    57,    66,    69,    69,    69,    69,    38,    63,    42,
      61,    70,    57,     4,    68,    58,    64,    65,    44,    68,
      39,    14,    15,    16,    45,    66,    67,    65,    66,     4,
      36
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
#define YYEMPTY		(-2)
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
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
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

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value, Location);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
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
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yytype, yyvaluep, yylocationp)
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

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
#endif /* ! YYPARSE_PARAM */






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

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

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
  YYLTYPE *yylerrsp;

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
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
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

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
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:
#line 175 "parse-gram.y"
    { prologue_augment (yyvsp[0].chars, yylsp[0]); }
    break;

  case 7:
#line 176 "parse-gram.y"
    { debug_flag = 1; }
    break;

  case 8:
#line 177 "parse-gram.y"
    { muscle_insert (yyvsp[-1].chars, yyvsp[0].chars); }
    break;

  case 9:
#line 178 "parse-gram.y"
    { defines_flag = 1; }
    break;

  case 10:
#line 179 "parse-gram.y"
    { error_verbose = 1; }
    break;

  case 11:
#line 180 "parse-gram.y"
    { expected_conflicts = yyvsp[0].integer; }
    break;

  case 12:
#line 181 "parse-gram.y"
    { spec_file_prefix = yyvsp[0].chars; }
    break;

  case 13:
#line 182 "parse-gram.y"
    { glr_parser = 1; }
    break;

  case 14:
#line 183 "parse-gram.y"
    { add_param ("lex_param", yyvsp[0].chars, yylsp[0]); }
    break;

  case 15:
#line 184 "parse-gram.y"
    { locations_flag = 1; }
    break;

  case 16:
#line 185 "parse-gram.y"
    { spec_name_prefix = yyvsp[0].chars; }
    break;

  case 17:
#line 186 "parse-gram.y"
    { no_lines_flag = 1; }
    break;

  case 18:
#line 187 "parse-gram.y"
    { spec_outfile = yyvsp[0].chars; }
    break;

  case 19:
#line 188 "parse-gram.y"
    { add_param ("parse_param", yyvsp[0].chars, yylsp[0]); }
    break;

  case 20:
#line 189 "parse-gram.y"
    { pure_parser = 1; }
    break;

  case 21:
#line 190 "parse-gram.y"
    { skeleton = yyvsp[0].chars; }
    break;

  case 22:
#line 191 "parse-gram.y"
    { token_table_flag = 1; }
    break;

  case 23:
#line 192 "parse-gram.y"
    { report_flag = report_states; }
    break;

  case 24:
#line 193 "parse-gram.y"
    { yacc_flag = 1; }
    break;

  case 28:
#line 201 "parse-gram.y"
    {
      grammar_start_symbol_set (yyvsp[0].symbol, yylsp[0]);
    }
    break;

  case 29:
#line 205 "parse-gram.y"
    {
      typed = 1;
      MUSCLE_INSERT_INT ("stype_line", yylsp[0].start.line);
      muscle_insert ("stype", yyvsp[0].chars);
    }
    break;

  case 30:
#line 211 "parse-gram.y"
    {
      symbol_list *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_destructor_set (list->sym, yyvsp[-1].chars, yylsp[-1]);
      symbol_list_free (yyvsp[0].list);
    }
    break;

  case 31:
#line 218 "parse-gram.y"
    {
      symbol_list *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_printer_set (list->sym, yyvsp[-1].chars, list->location);
      symbol_list_free (yyvsp[0].list);
    }
    break;

  case 32:
#line 227 "parse-gram.y"
    { current_class = nterm_sym; }
    break;

  case 33:
#line 228 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 34:
#line 232 "parse-gram.y"
    { current_class = token_sym; }
    break;

  case 35:
#line 233 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 36:
#line 238 "parse-gram.y"
    {
      symbol_list *list;
      for (list = yyvsp[0].list; list; list = list->next)
	symbol_type_set (list->sym, yyvsp[-1].uniqstr, yylsp[-1]);
      symbol_list_free (yyvsp[0].list);
    }
    break;

  case 37:
#line 248 "parse-gram.y"
    {
      symbol_list *list;
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

  case 38:
#line 262 "parse-gram.y"
    { yyval.assoc = left_assoc; }
    break;

  case 39:
#line 263 "parse-gram.y"
    { yyval.assoc = right_assoc; }
    break;

  case 40:
#line 264 "parse-gram.y"
    { yyval.assoc = non_assoc; }
    break;

  case 41:
#line 268 "parse-gram.y"
    { current_type = NULL; }
    break;

  case 42:
#line 269 "parse-gram.y"
    { current_type = yyvsp[0].uniqstr; }
    break;

  case 43:
#line 275 "parse-gram.y"
    { yyval.list = symbol_list_new (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 44:
#line 276 "parse-gram.y"
    { yyval.list = symbol_list_prepend (yyvsp[-1].list, yyvsp[0].symbol, yylsp[0]); }
    break;

  case 45:
#line 282 "parse-gram.y"
    {
       current_type = yyvsp[0].uniqstr;
     }
    break;

  case 46:
#line 286 "parse-gram.y"
    {
       symbol_class_set (yyvsp[0].symbol, current_class, yylsp[0]);
       symbol_type_set (yyvsp[0].symbol, current_type, yylsp[0]);
     }
    break;

  case 47:
#line 291 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class, yylsp[-1]);
      symbol_type_set (yyvsp[-1].symbol, current_type, yylsp[-1]);
      symbol_user_token_number_set (yyvsp[-1].symbol, yyvsp[0].integer, yylsp[0]);
    }
    break;

  case 48:
#line 297 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-1].symbol, current_class, yylsp[-1]);
      symbol_type_set (yyvsp[-1].symbol, current_type, yylsp[-1]);
      symbol_make_alias (yyvsp[-1].symbol, yyvsp[0].symbol, yyloc);
    }
    break;

  case 49:
#line 303 "parse-gram.y"
    {
      symbol_class_set (yyvsp[-2].symbol, current_class, yylsp[-2]);
      symbol_type_set (yyvsp[-2].symbol, current_type, yylsp[-2]);
      symbol_user_token_number_set (yyvsp[-2].symbol, yyvsp[-1].integer, yylsp[-1]);
      symbol_make_alias (yyvsp[-2].symbol, yyvsp[0].symbol, yyloc);
    }
    break;

  case 55:
#line 332 "parse-gram.y"
    {
      if (yacc_flag)
	complain_at (yyloc, _("POSIX forbids declarations in the grammar"));
    }
    break;

  case 56:
#line 337 "parse-gram.y"
    {
      yyerrok;
    }
    break;

  case 58:
#line 344 "parse-gram.y"
    { current_lhs = yyvsp[0].symbol; current_lhs_location = yylsp[0]; }
    break;

  case 60:
#line 348 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 61:
#line 349 "parse-gram.y"
    { grammar_rule_end (yylsp[0]); }
    break;

  case 62:
#line 354 "parse-gram.y"
    { grammar_rule_begin (current_lhs, current_lhs_location); }
    break;

  case 63:
#line 356 "parse-gram.y"
    { grammar_current_rule_symbol_append (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 64:
#line 358 "parse-gram.y"
    { grammar_current_rule_action_append (yyvsp[0].chars, yylsp[0]); }
    break;

  case 65:
#line 360 "parse-gram.y"
    { grammar_current_rule_prec_set (yyvsp[0].symbol, yylsp[0]); }
    break;

  case 66:
#line 362 "parse-gram.y"
    { grammar_current_rule_dprec_set (yyvsp[0].integer, yylsp[0]); }
    break;

  case 67:
#line 364 "parse-gram.y"
    { grammar_current_rule_merge_set (yyvsp[0].uniqstr, yylsp[0]); }
    break;

  case 68:
#line 368 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 69:
#line 369 "parse-gram.y"
    { yyval.symbol = yyvsp[0].symbol; }
    break;

  case 70:
#line 374 "parse-gram.y"
    { yyval.chars = yyvsp[0].chars; }
    break;

  case 71:
#line 380 "parse-gram.y"
    {
      yyval.symbol = symbol_get (yyvsp[0].chars, yylsp[0]);
      symbol_class_set (yyval.symbol, token_sym, yylsp[0]);
    }
    break;

  case 72:
#line 389 "parse-gram.y"
    {
      yyval.chars = yyvsp[0].chars + 1;
      yyval.chars[strlen (yyval.chars) - 1] = '\0';
    }
    break;

  case 74:
#line 398 "parse-gram.y"
    {
      epilogue_augment (yyvsp[0].chars, yylsp[0]);
      scanner_last_string_free ();
    }
    break;


    }

/* Line 991 of yacc.c.  */
#line 1614 "parse-gram.c"

  yyvsp -= yylen;
  yyssp -= yylen;
  yylsp -= yylen;

  YY_STACK_PRINT (yyss, yyssp);

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
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
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
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }

  yylerrsp = yylsp;

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
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp, yylsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval, &yylloc);
      yychar = YYEMPTY;
      *++yylerrsp = yylloc;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:

  /* Suppress GCC warning that yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif

  yylerrsp = yylsp;
  *++yylerrsp = yyloc;
  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp, yylsp);
      yyvsp--;
      yystate = *--yyssp;
      yylsp--;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
  YYLLOC_DEFAULT (yyloc, yylsp, (yylerrsp - yylsp));
  *++yylsp = yyloc;

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


#line 404 "parse-gram.y"



/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

static YYLTYPE
lloc_default (YYLTYPE const *rhs, int n)
{
  int i;
  YYLTYPE r;
  r.start = r.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  for (i = 1; i <= n; i++)
    if (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
	r.start = rhs[i].start;
	break;
      }

  return r;
}


/* Add a lex-param or a parse-param (depending on TYPE) with
   declaration DECL and location LOC.  */

static void
add_param (char const *type, char *decl, location loc)
{
  static char const alphanum[] =
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_";
  char const *alpha = alphanum + 10;
  char const *name_start = NULL;
  char *p;

  for (p = decl; *p; p++)
    if ((p == decl || ! strchr (alphanum, p[-1])) && strchr (alpha, p[0]))
      name_start = p;

  /* Strip the surrounding '{' and '}'.  */
  decl++;
  p[-1] = '\0';

  if (! name_start)
    complain_at (loc, _("missing identifier in parameter declaration"));
  else
    {
      char *name;
      size_t name_len;

      for (name_len = 1;
	   name_start[name_len] && strchr (alphanum, name_start[name_len]);
	   name_len++)
	continue;

      name = xmalloc (name_len + 1);
      memcpy (name, name_start, name_len);
      name[name_len] = '\0';
      muscle_pair_list_grow (type, decl, name);
      free (name);
    }

  scanner_last_string_free ();
}

/*----------------------------------------------------.
| When debugging the parser, display tokens' values.  |
`----------------------------------------------------*/

static void
print_token_value (FILE *file, int type, YYSTYPE const *value)
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
      fprintf (file, " = \"%s\"", value->chars);
      break;

    case TYPE:
      fprintf (file, " = <%s>", value->uniqstr);
      break;

    case BRACED_CODE:
    case PERCENT_DESTRUCTOR:
    case PERCENT_LEX_PARAM:
    case PERCENT_PARSE_PARAM:
    case PERCENT_PRINTER:
    case PERCENT_UNION:
    case PROLOGUE:
    case EPILOGUE:
      fprintf (file, " = {{ %s }}", value->chars);
      break;

    default:
      fprintf (file, "unknown token type");
      break;
    }
}

static void
gram_error (location const *loc, char const *msg)
{
  complain_at (*loc, "%s", msg);
}

char const *
token_name (int type)
{
  return yytname[type];
}

