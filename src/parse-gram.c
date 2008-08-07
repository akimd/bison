/* A Bison parser, made by GNU Bison 2.4.79-c85be.  */

/* Implementation for Bison's Yacc-like parsers in C
   
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2007, 2008 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.79-c85be"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         gram_parse
#define yylex           gram_lex
#define yyerror         gram_error
#define yylval          gram_lval
#define yychar          gram_char
#define yydebug         gram_debug
#define yynerrs         gram_nerrs
#define yylloc          gram_lloc

/* Copy the first part of user declarations.  */

/* Line 191 of yacc.c  */
#line 1 "parse-gram.y"
/* Bison Grammar Parser                             -*- C -*-

   Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <config.h>
#include "system.h"

#include "complain.h"
#include "conflicts.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "muscle_tab.h"
#include "quotearg.h"
#include "reader.h"
#include "symlist.h"
#include "scan-gram.h"
#include "scan-code.h"

#define YYLLOC_DEFAULT(Current, Rhs, N)  (Current) = lloc_default (Rhs, N)
static YYLTYPE lloc_default (YYLTYPE const *, int);

#define YY_LOCATION_PRINT(File, Loc) \
	  location_print (File, Loc)

static void version_check (location const *loc, char const *version);

/* Request detailed syntax error messages, and pass them to GRAM_ERROR.
   FIXME: depends on the undocumented availability of YYLLOC.  */
#undef  yyerror
#define yyerror(Msg) \
	gram_error (&yylloc, Msg)
static void gram_error (location const *, char const *);

static char const *char_name (char);

/** Add a lex-param or a parse-param.
 *
 * \param type  \a lex_param or \a parse_param
 * \param decl  the formal argument
 * \param loc   the location in the source.
 */
static void add_param (char const *type, char *decl, location loc);


static symbol_class current_class = unknown_sym;
static uniqstr current_type = NULL;
static symbol *current_lhs;
static location current_lhs_location;
static int current_prec = 0;

#define YYTYPE_INT16 int_fast16_t
#define YYTYPE_INT8 int_fast8_t
#define YYTYPE_UINT16 uint_fast16_t
#define YYTYPE_UINT8 uint_fast8_t


/* Line 191 of yacc.c  */
#line 154 "../../../src/parse-gram.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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
     PERCENT_LEFT = 265,
     PERCENT_RIGHT = 266,
     PERCENT_NONASSOC = 267,
     PERCENT_PRECEDENCE = 268,
     PERCENT_PREC = 269,
     PERCENT_DPREC = 270,
     PERCENT_MERGE = 271,
     PERCENT_CODE = 272,
     PERCENT_DEBUG = 273,
     PERCENT_DEFAULT_PREC = 274,
     PERCENT_DEFINE = 275,
     PERCENT_DEFINES = 276,
     PERCENT_ERROR_VERBOSE = 277,
     PERCENT_EXPECT = 278,
     PERCENT_EXPECT_RR = 279,
     PERCENT_FILE_PREFIX = 280,
     PERCENT_GLR_PARSER = 281,
     PERCENT_INITIAL_ACTION = 282,
     PERCENT_LANGUAGE = 283,
     PERCENT_LEX_PARAM = 284,
     PERCENT_LOCATIONS = 285,
     PERCENT_NAME_PREFIX = 286,
     PERCENT_NO_DEFAULT_PREC = 287,
     PERCENT_NO_LINES = 288,
     PERCENT_NONDETERMINISTIC_PARSER = 289,
     PERCENT_OUTPUT = 290,
     PERCENT_PARSE_PARAM = 291,
     PERCENT_PURE_PARSER = 292,
     PERCENT_REQUIRE = 293,
     PERCENT_SKELETON = 294,
     PERCENT_START = 295,
     PERCENT_TOKEN_TABLE = 296,
     PERCENT_VERBOSE = 297,
     PERCENT_YACC = 298,
     BRACED_CODE = 299,
     CHAR = 300,
     EPILOGUE = 301,
     EQUAL = 302,
     ID = 303,
     ID_COLON = 304,
     PERCENT_PERCENT = 305,
     PIPE = 306,
     PROLOGUE = 307,
     SEMICOLON = 308,
     TYPE = 309,
     TYPE_TAG_ANY = 310,
     TYPE_TAG_NONE = 311,
     PERCENT_UNION = 312
   };
#endif
/* Tokens.  */
#define GRAM_EOF 0
#define STRING 258
#define INT 259
#define PERCENT_TOKEN 260
#define PERCENT_NTERM 261
#define PERCENT_TYPE 262
#define PERCENT_DESTRUCTOR 263
#define PERCENT_PRINTER 264
#define PERCENT_LEFT 265
#define PERCENT_RIGHT 266
#define PERCENT_NONASSOC 267
#define PERCENT_PRECEDENCE 268
#define PERCENT_PREC 269
#define PERCENT_DPREC 270
#define PERCENT_MERGE 271
#define PERCENT_CODE 272
#define PERCENT_DEBUG 273
#define PERCENT_DEFAULT_PREC 274
#define PERCENT_DEFINE 275
#define PERCENT_DEFINES 276
#define PERCENT_ERROR_VERBOSE 277
#define PERCENT_EXPECT 278
#define PERCENT_EXPECT_RR 279
#define PERCENT_FILE_PREFIX 280
#define PERCENT_GLR_PARSER 281
#define PERCENT_INITIAL_ACTION 282
#define PERCENT_LANGUAGE 283
#define PERCENT_LEX_PARAM 284
#define PERCENT_LOCATIONS 285
#define PERCENT_NAME_PREFIX 286
#define PERCENT_NO_DEFAULT_PREC 287
#define PERCENT_NO_LINES 288
#define PERCENT_NONDETERMINISTIC_PARSER 289
#define PERCENT_OUTPUT 290
#define PERCENT_PARSE_PARAM 291
#define PERCENT_PURE_PARSER 292
#define PERCENT_REQUIRE 293
#define PERCENT_SKELETON 294
#define PERCENT_START 295
#define PERCENT_TOKEN_TABLE 296
#define PERCENT_VERBOSE 297
#define PERCENT_YACC 298
#define BRACED_CODE 299
#define CHAR 300
#define EPILOGUE 301
#define EQUAL 302
#define ID 303
#define ID_COLON 304
#define PERCENT_PERCENT 305
#define PIPE 306
#define PROLOGUE 307
#define SEMICOLON 308
#define TYPE 309
#define TYPE_TAG_ANY 310
#define TYPE_TAG_NONE 311
#define PERCENT_UNION 312




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 216 of yacc.c  */
#line 92 "parse-gram.y"

  symbol *symbol;
  symbol_list *list;
  int integer;
  char const *chars;
  char *code;
  assoc assoc;
  uniqstr uniqstr;
  unsigned char character;



/* Line 216 of yacc.c  */
#line 319 "../../../src/parse-gram.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
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


/* Line 266 of yacc.c  */
#line 344 "../../../src/parse-gram.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   203

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNRULES -- Number of states.  */
#define YYNSTATES  144

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   312

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,     9,    12,    14,    16,    18,    22,
      24,    27,    29,    32,    35,    38,    42,    44,    47,    50,
      53,    55,    58,    62,    64,    66,    69,    73,    76,    78,
      81,    84,    86,    88,    90,    92,    94,    96,    99,   103,
     107,   109,   111,   114,   118,   119,   121,   125,   126,   130,
     131,   135,   139,   143,   145,   147,   149,   151,   152,   154,
     156,   159,   161,   164,   166,   169,   171,   174,   176,   178,
     180,   182,   184,   186,   189,   192,   196,   198,   201,   203,
     206,   208,   211,   214,   215,   219,   221,   225,   228,   229,
     232,   235,   239,   243,   247,   249,   251,   252,   254,   256,
     258,   260,   262,   264,   266,   268,   269
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      59,     0,    -1,    60,    50,    77,    90,    -1,    -1,    60,
      61,    -1,    62,    -1,    52,    -1,    18,    -1,    20,    83,
      84,    -1,    21,    -1,    21,     3,    -1,    22,    -1,    23,
       4,    -1,    24,     4,    -1,    25,     3,    -1,    25,    47,
       3,    -1,    26,    -1,    27,    44,    -1,    28,     3,    -1,
      29,    44,    -1,    30,    -1,    31,     3,    -1,    31,    47,
       3,    -1,    33,    -1,    34,    -1,    35,     3,    -1,    35,
      47,     3,    -1,    36,    44,    -1,    37,    -1,    38,     3,
      -1,    39,     3,    -1,    41,    -1,    42,    -1,    43,    -1,
      53,    -1,    67,    -1,    64,    -1,    40,    88,    -1,     8,
      44,    73,    -1,     9,    44,    73,    -1,    19,    -1,    32,
      -1,    17,    85,    -1,    17,    48,    85,    -1,    -1,    48,
      -1,    57,    63,    85,    -1,    -1,     6,    65,    76,    -1,
      -1,     5,    66,    76,    -1,     7,    54,    72,    -1,    68,
      69,    70,    -1,    10,    -1,    11,    -1,    12,    -1,    13,
      -1,    -1,    54,    -1,    71,    -1,    70,    71,    -1,    88,
      -1,    88,     4,    -1,    88,    -1,    72,    88,    -1,    74,
      -1,    73,    74,    -1,    88,    -1,    54,    -1,    55,    -1,
      56,    -1,    54,    -1,    86,    -1,    86,     4,    -1,    86,
      89,    -1,    86,     4,    89,    -1,    75,    -1,    76,    75,
      -1,    78,    -1,    77,    78,    -1,    79,    -1,    62,    53,
      -1,     1,    53,    -1,    -1,    87,    80,    81,    -1,    82,
      -1,    81,    51,    82,    -1,    81,    53,    -1,    -1,    82,
      88,    -1,    82,    44,    -1,    82,    14,    88,    -1,    82,
      15,     4,    -1,    82,    16,    54,    -1,    48,    -1,     3,
      -1,    -1,     3,    -1,    44,    -1,    48,    -1,    45,    -1,
      49,    -1,    86,    -1,    89,    -1,     3,    -1,    -1,    50,
      46,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   203,   203,   211,   213,   217,   218,   228,   229,   233,
     234,   239,   240,   241,   242,   243,   244,   249,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   281,
     282,   306,   307,   308,   309,   313,   314,   315,   319,   326,
     333,   337,   341,   348,   363,   364,   368,   380,   380,   385,
     385,   390,   401,   416,   417,   418,   419,   423,   424,   429,
     431,   436,   437,   442,   444,   449,   450,   454,   455,   456,
     457,   462,   467,   472,   478,   484,   495,   496,   505,   506,
     512,   513,   514,   521,   521,   525,   526,   527,   532,   533,
     535,   537,   539,   541,   551,   552,   558,   561,   570,   590,
     592,   601,   606,   607,   612,   619,   621
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"string\"", "\"integer\"",
  "\"%token\"", "\"%nterm\"", "\"%type\"", "\"%destructor\"",
  "\"%printer\"", "\"%left\"", "\"%right\"", "\"%nonassoc\"",
  "\"%precedence\"", "\"%prec\"", "\"%dprec\"", "\"%merge\"", "\"%code\"",
  "\"%debug\"", "\"%default-prec\"", "\"%define\"", "\"%defines\"",
  "\"%error-verbose\"", "\"%expect\"", "\"%expect-rr\"",
  "\"%file-prefix\"", "\"%glr-parser\"", "\"%initial-action\"",
  "\"%language\"", "\"%lex-param\"", "\"%locations\"", "\"%name-prefix\"",
  "\"%no-default-prec\"", "\"%no-lines\"", "\"%nondeterministic-parser\"",
  "\"%output\"", "\"%parse-param\"", "\"%pure-parser\"", "\"%require\"",
  "\"%skeleton\"", "\"%start\"", "\"%token-table\"", "\"%verbose\"",
  "\"%yacc\"", "\"{...}\"", "\"char\"", "\"epilogue\"", "\"=\"",
  "\"identifier\"", "\"identifier:\"", "\"%%\"", "\"|\"", "\"%{...%}\"",
  "\";\"", "\"type\"", "\"<*>\"", "\"<>\"", "\"%union\"", "$accept",
  "input", "prologue_declarations", "prologue_declaration",
  "grammar_declaration", "union_name", "symbol_declaration", "$@1", "$@2",
  "precedence_declaration", "precedence_declarator", "type.opt",
  "symbols.prec", "symbol.prec", "symbols.1", "generic_symlist",
  "generic_symlist_item", "symbol_def", "symbol_defs.1", "grammar",
  "rules_or_grammar_declaration", "rules", "$@3", "rhses.1", "rhs",
  "variable", "content.opt", "braceless", "id", "id_colon", "symbol",
  "string_as_id", "epilogue.opt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    60,    60,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    63,    62,    65,    64,    66,
      64,    64,    67,    68,    68,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    73,    73,    74,    74,    74,
      74,    75,    75,    75,    75,    75,    76,    76,    77,    77,
      78,    78,    78,    80,    79,    81,    81,    81,    82,    82,
      82,    82,    82,    82,    83,    83,    84,    84,    85,    86,
      86,    87,    88,    88,    89,    90,    90
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     2,     1,     1,     1,     3,     1,
       2,     1,     2,     2,     2,     3,     1,     2,     2,     2,
       1,     2,     3,     1,     1,     2,     3,     2,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     3,     3,
       1,     1,     2,     3,     0,     1,     3,     0,     3,     0,
       3,     3,     3,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     1,     2,     1,     2,
       1,     2,     2,     0,     3,     1,     3,     2,     0,     2,
       2,     3,     3,     3,     1,     1,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    49,    47,     0,     0,     0,    53,
      54,    55,    56,     0,     7,    40,     0,     9,    11,     0,
       0,     0,    16,     0,     0,     0,    20,     0,    41,    23,
      24,     0,     0,    28,     0,     0,     0,    31,    32,    33,
       0,     6,    34,    44,     4,     5,    36,    35,    57,     0,
       0,     0,     0,     0,    98,     0,    42,    95,    94,    96,
      10,    12,    13,    14,     0,    17,    18,    19,    21,     0,
      25,     0,    27,    29,    30,   104,   100,    99,   102,    37,
     103,     0,   101,     0,     0,    78,    80,    83,    45,     0,
      58,     0,    71,    76,    50,    72,    48,    51,    63,    68,
      69,    70,    38,    65,    67,    39,    43,    97,     8,    15,
      22,    26,    82,    81,     0,    79,     2,    88,    46,    52,
      59,    61,    77,    73,    74,    64,    66,   106,    84,    85,
      60,    62,    75,    88,    87,     0,     0,     0,    90,    89,
      86,    91,    92,    93
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    44,    83,    89,    46,    50,    49,    47,
      48,    91,   119,   120,    97,   102,   103,    93,    94,    84,
      85,    86,   117,   128,   129,    59,   108,    56,    78,    87,
     104,    80,   116
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -81
static const yytype_int16 yypact[] =
{
     -81,    23,   111,   -81,   -81,   -81,   -28,   -12,    -8,   -81,
     -81,   -81,   -81,   -13,   -81,   -81,    14,    43,   -81,    48,
      49,     2,   -81,    10,    53,    34,   -81,    13,   -81,   -81,
     -81,    26,    36,   -81,    54,    78,     0,   -81,   -81,   -81,
      58,   -81,   -81,    41,   -81,   -81,   -81,   -81,    28,   -24,
     -24,     0,    31,    31,   -81,    44,   -81,   -81,   -81,    81,
     -81,   -81,   -81,   -81,    88,   -81,   -81,   -81,   -81,    89,
     -81,    91,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,    42,   -81,    47,     1,   -81,   -81,   -81,   -81,    44,
     -81,     0,   -81,   -81,   -24,    40,   -24,     0,   -81,   -81,
     -81,   -81,    31,   -81,   -81,    31,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,    50,   -81,   -81,   -81,   -81,     0,
     -81,    93,   -81,    98,   -81,   -81,   -81,   -81,   -14,   155,
     -81,   -81,   -81,   -81,   -81,     0,    99,    51,   -81,   -81,
     155,   -81,   -81,   -81
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -81,   -81,   -81,   -81,   100,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -11,   -81,    56,   -80,   -54,    60,   -81,
      22,   -81,   -81,   -81,   -21,   -81,   -81,   -51,   -22,   -81,
     -36,   -76,   -81
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -106
static const yytype_int16 yytable[] =
{
      79,  -105,    81,    75,   106,    63,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    98,    68,    57,    13,   124,
      15,    76,   126,     3,    77,   126,    51,    95,    95,    70,
      92,    54,    52,    28,    75,    55,    53,   133,   118,   134,
     122,    36,   122,    75,   123,    76,    60,   132,    77,    64,
      82,   114,    61,    62,    65,   121,    66,    73,    43,    81,
      69,   125,    58,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    95,    71,    95,    13,    76,    15,    67,    77,
      72,    74,    90,   121,   107,    99,   100,   101,    54,    88,
      28,   109,   110,   139,   111,   112,   127,   131,    36,   141,
     113,    75,    45,   142,   139,   143,   115,    82,   130,   105,
      96,     0,   140,     0,     0,    43,     4,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,     0,     0,    75,     0,
       0,    40,     0,    41,    42,     0,     0,     0,    43,   135,
     136,   137,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   138,
      76,     0,     0,    77
};

static const yytype_int16 yycheck[] =
{
      36,     0,     1,     3,    55,     3,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    51,     3,     3,    17,    95,
      19,    45,   102,     0,    48,   105,    54,    49,    50,     3,
      54,    44,    44,    32,     3,    48,    44,    51,    89,    53,
      94,    40,    96,     3,     4,    45,     3,   123,    48,    47,
      49,    50,     4,     4,    44,    91,     3,     3,    57,     1,
      47,    97,    48,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    94,    47,    96,    17,    45,    19,    44,    48,
      44,     3,    54,   119,     3,    54,    55,    56,    44,    48,
      32,     3,     3,   129,     3,    53,    46,     4,    40,   135,
      53,     3,     2,     4,   140,    54,    84,    49,   119,    53,
      50,    -1,   133,    -1,    -1,    57,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    -1,    -1,     3,    -1,
      -1,    50,    -1,    52,    53,    -1,    -1,    -1,    57,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    48
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    59,    60,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      50,    52,    53,    57,    61,    62,    64,    67,    68,    66,
      65,    54,    44,    44,    44,    48,    85,     3,    48,    83,
       3,     4,     4,     3,    47,    44,     3,    44,     3,    47,
       3,    47,    44,     3,     3,     3,    45,    48,    86,    88,
      89,     1,    49,    62,    77,    78,    79,    87,    48,    63,
      54,    69,    54,    75,    76,    86,    76,    72,    88,    54,
      55,    56,    73,    74,    88,    73,    85,     3,    84,     3,
       3,     3,    53,    53,    50,    78,    90,    80,    85,    70,
      71,    88,    75,     4,    89,    88,    74,    46,    81,    82,
      71,     4,    89,    51,    53,    14,    15,    16,    44,    88,
      82,    88,     4,    54
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


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
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      case 3: /* "\"string\"" */

/* Line 717 of yacc.c  */
#line 181 "parse-gram.y"
	{ fputs (quotearg_style (c_quoting_style, ((*yyvaluep).chars)), stderr); };

/* Line 717 of yacc.c  */
#line 1050 "../../../src/parse-gram.c"
	break;
      case 4: /* "\"integer\"" */

/* Line 717 of yacc.c  */
#line 192 "parse-gram.y"
	{ fprintf (stderr, "%d", ((*yyvaluep).integer)); };

/* Line 717 of yacc.c  */
#line 1059 "../../../src/parse-gram.c"
	break;
      case 44: /* "\"{...}\"" */

/* Line 717 of yacc.c  */
#line 183 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", ((*yyvaluep).code)); };

/* Line 717 of yacc.c  */
#line 1068 "../../../src/parse-gram.c"
	break;
      case 45: /* "\"char\"" */

/* Line 717 of yacc.c  */
#line 175 "parse-gram.y"
	{ fputs (char_name (((*yyvaluep).character)), stderr); };

/* Line 717 of yacc.c  */
#line 1077 "../../../src/parse-gram.c"
	break;
      case 46: /* "\"epilogue\"" */

/* Line 717 of yacc.c  */
#line 183 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); };

/* Line 717 of yacc.c  */
#line 1086 "../../../src/parse-gram.c"
	break;
      case 48: /* "\"identifier\"" */

/* Line 717 of yacc.c  */
#line 188 "parse-gram.y"
	{ fputs (((*yyvaluep).uniqstr), stderr); };

/* Line 717 of yacc.c  */
#line 1095 "../../../src/parse-gram.c"
	break;
      case 49: /* "\"identifier:\"" */

/* Line 717 of yacc.c  */
#line 189 "parse-gram.y"
	{ fprintf (stderr, "%s:", ((*yyvaluep).uniqstr)); };

/* Line 717 of yacc.c  */
#line 1104 "../../../src/parse-gram.c"
	break;
      case 52: /* "\"%{...%}\"" */

/* Line 717 of yacc.c  */
#line 183 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); };

/* Line 717 of yacc.c  */
#line 1113 "../../../src/parse-gram.c"
	break;
      case 54: /* "\"type\"" */

/* Line 717 of yacc.c  */
#line 187 "parse-gram.y"
	{ fprintf (stderr, "<%s>", ((*yyvaluep).uniqstr)); };

/* Line 717 of yacc.c  */
#line 1122 "../../../src/parse-gram.c"
	break;
      case 83: /* "variable" */

/* Line 717 of yacc.c  */
#line 188 "parse-gram.y"
	{ fputs (((*yyvaluep).uniqstr), stderr); };

/* Line 717 of yacc.c  */
#line 1131 "../../../src/parse-gram.c"
	break;
      case 84: /* "content.opt" */

/* Line 717 of yacc.c  */
#line 183 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); };

/* Line 717 of yacc.c  */
#line 1140 "../../../src/parse-gram.c"
	break;
      case 85: /* "braceless" */

/* Line 717 of yacc.c  */
#line 183 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); };

/* Line 717 of yacc.c  */
#line 1149 "../../../src/parse-gram.c"
	break;
      case 86: /* "id" */

/* Line 717 of yacc.c  */
#line 195 "parse-gram.y"
	{ fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); };

/* Line 717 of yacc.c  */
#line 1158 "../../../src/parse-gram.c"
	break;
      case 87: /* "id_colon" */

/* Line 717 of yacc.c  */
#line 196 "parse-gram.y"
	{ fprintf (stderr, "%s:", ((*yyvaluep).symbol)->tag); };

/* Line 717 of yacc.c  */
#line 1167 "../../../src/parse-gram.c"
	break;
      case 88: /* "symbol" */

/* Line 717 of yacc.c  */
#line 195 "parse-gram.y"
	{ fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); };

/* Line 717 of yacc.c  */
#line 1176 "../../../src/parse-gram.c"
	break;
      case 89: /* "string_as_id" */

/* Line 717 of yacc.c  */
#line 195 "parse-gram.y"
	{ fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); };

/* Line 717 of yacc.c  */
#line 1185 "../../../src/parse-gram.c"
	break;
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

/* User initialization code.  */

/* Line 1244 of yacc.c  */
#line 84 "parse-gram.y"
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_set (&yylloc.start, current_file, 1, 1);
  boundary_set (&yylloc.end, current_file, 1, 1);
}

/* Line 1244 of yacc.c  */
#line 1714 "../../../src/parse-gram.c"
  yylsp[0] = yylloc;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
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
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

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
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:

/* Line 1457 of yacc.c  */
#line 219 "parse-gram.y"
    {
      code_props plain_code;
      code_props_plain_init (&plain_code, (yyvsp[(1) - (1)].chars), (yylsp[(1) - (1)]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      muscle_code_grow (union_seen ? "post_prologue" : "pre_prologue",
                        plain_code.code, (yylsp[(1) - (1)]));
      code_scanner_last_string_free ();
    }

/* Line 1457 of yacc.c  */
#line 1914 "../../../src/parse-gram.c"
    break;

  case 7:

/* Line 1457 of yacc.c  */
#line 228 "parse-gram.y"
    { debug_flag = true; }

/* Line 1457 of yacc.c  */
#line 1924 "../../../src/parse-gram.c"
    break;

  case 8:

/* Line 1457 of yacc.c  */
#line 230 "parse-gram.y"
    {
      muscle_percent_define_insert ((yyvsp[(2) - (3)].uniqstr), (yylsp[(2) - (3)]), (yyvsp[(3) - (3)].chars));
    }

/* Line 1457 of yacc.c  */
#line 1936 "../../../src/parse-gram.c"
    break;

  case 9:

/* Line 1457 of yacc.c  */
#line 233 "parse-gram.y"
    { defines_flag = true; }

/* Line 1457 of yacc.c  */
#line 1946 "../../../src/parse-gram.c"
    break;

  case 10:

/* Line 1457 of yacc.c  */
#line 235 "parse-gram.y"
    {
      defines_flag = true;
      spec_defines_file = xstrdup ((yyvsp[(2) - (2)].chars));
    }

/* Line 1457 of yacc.c  */
#line 1959 "../../../src/parse-gram.c"
    break;

  case 11:

/* Line 1457 of yacc.c  */
#line 239 "parse-gram.y"
    { error_verbose = true; }

/* Line 1457 of yacc.c  */
#line 1969 "../../../src/parse-gram.c"
    break;

  case 12:

/* Line 1457 of yacc.c  */
#line 240 "parse-gram.y"
    { expected_sr_conflicts = (yyvsp[(2) - (2)].integer); }

/* Line 1457 of yacc.c  */
#line 1979 "../../../src/parse-gram.c"
    break;

  case 13:

/* Line 1457 of yacc.c  */
#line 241 "parse-gram.y"
    { expected_rr_conflicts = (yyvsp[(2) - (2)].integer); }

/* Line 1457 of yacc.c  */
#line 1989 "../../../src/parse-gram.c"
    break;

  case 14:

/* Line 1457 of yacc.c  */
#line 242 "parse-gram.y"
    { spec_file_prefix = (yyvsp[(2) - (2)].chars); }

/* Line 1457 of yacc.c  */
#line 1999 "../../../src/parse-gram.c"
    break;

  case 15:

/* Line 1457 of yacc.c  */
#line 243 "parse-gram.y"
    { spec_file_prefix = (yyvsp[(3) - (3)].chars); }

/* Line 1457 of yacc.c  */
#line 2009 "../../../src/parse-gram.c"
    break;

  case 16:

/* Line 1457 of yacc.c  */
#line 245 "parse-gram.y"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }

/* Line 1457 of yacc.c  */
#line 2022 "../../../src/parse-gram.c"
    break;

  case 17:

/* Line 1457 of yacc.c  */
#line 250 "parse-gram.y"
    {
      code_props action;
      code_props_symbol_action_init (&action, (yyvsp[(2) - (2)].code), (yylsp[(2) - (2)]));
      code_props_translate_code (&action);
      gram_scanner_last_string_free ();
      muscle_code_grow ("initial_action", action.code, (yylsp[(2) - (2)]));
      code_scanner_last_string_free ();
    }

/* Line 1457 of yacc.c  */
#line 2039 "../../../src/parse-gram.c"
    break;

  case 18:

/* Line 1457 of yacc.c  */
#line 258 "parse-gram.y"
    { language_argmatch ((yyvsp[(2) - (2)].chars), grammar_prio, (yylsp[(1) - (2)])); }

/* Line 1457 of yacc.c  */
#line 2049 "../../../src/parse-gram.c"
    break;

  case 19:

/* Line 1457 of yacc.c  */
#line 259 "parse-gram.y"
    { add_param ("lex_param", (yyvsp[(2) - (2)].code), (yylsp[(2) - (2)])); }

/* Line 1457 of yacc.c  */
#line 2059 "../../../src/parse-gram.c"
    break;

  case 20:

/* Line 1457 of yacc.c  */
#line 260 "parse-gram.y"
    { locations_flag = true; }

/* Line 1457 of yacc.c  */
#line 2069 "../../../src/parse-gram.c"
    break;

  case 21:

/* Line 1457 of yacc.c  */
#line 261 "parse-gram.y"
    { spec_name_prefix = (yyvsp[(2) - (2)].chars); }

/* Line 1457 of yacc.c  */
#line 2079 "../../../src/parse-gram.c"
    break;

  case 22:

/* Line 1457 of yacc.c  */
#line 262 "parse-gram.y"
    { spec_name_prefix = (yyvsp[(3) - (3)].chars); }

/* Line 1457 of yacc.c  */
#line 2089 "../../../src/parse-gram.c"
    break;

  case 23:

/* Line 1457 of yacc.c  */
#line 263 "parse-gram.y"
    { no_lines_flag = true; }

/* Line 1457 of yacc.c  */
#line 2099 "../../../src/parse-gram.c"
    break;

  case 24:

/* Line 1457 of yacc.c  */
#line 264 "parse-gram.y"
    { nondeterministic_parser = true; }

/* Line 1457 of yacc.c  */
#line 2109 "../../../src/parse-gram.c"
    break;

  case 25:

/* Line 1457 of yacc.c  */
#line 265 "parse-gram.y"
    { spec_outfile = (yyvsp[(2) - (2)].chars); }

/* Line 1457 of yacc.c  */
#line 2119 "../../../src/parse-gram.c"
    break;

  case 26:

/* Line 1457 of yacc.c  */
#line 266 "parse-gram.y"
    { spec_outfile = (yyvsp[(3) - (3)].chars); }

/* Line 1457 of yacc.c  */
#line 2129 "../../../src/parse-gram.c"
    break;

  case 27:

/* Line 1457 of yacc.c  */
#line 267 "parse-gram.y"
    { add_param ("parse_param", (yyvsp[(2) - (2)].code), (yylsp[(2) - (2)])); }

/* Line 1457 of yacc.c  */
#line 2139 "../../../src/parse-gram.c"
    break;

  case 28:

/* Line 1457 of yacc.c  */
#line 269 "parse-gram.y"
    {
      /* %pure-parser is deprecated in favor of `%define api.pure', so use
         `%define api.pure' in a backward-compatible manner here.  First, don't
         complain if %pure-parser is specified multiple times.  */
      if (!muscle_find_const ("percent_define(api.pure)"))
        muscle_percent_define_insert ("api.pure", (yylsp[(1) - (1)]), "");
      /* In all cases, use api.pure now so that the backend doesn't complain if
         the skeleton ignores api.pure, but do warn now if there's a previous
         conflicting definition from an actual %define.  */
      if (!muscle_percent_define_flag_if ("api.pure"))
        muscle_percent_define_insert ("api.pure", (yylsp[(1) - (1)]), "");
    }

/* Line 1457 of yacc.c  */
#line 2160 "../../../src/parse-gram.c"
    break;

  case 29:

/* Line 1457 of yacc.c  */
#line 281 "parse-gram.y"
    { version_check (&(yylsp[(2) - (2)]), (yyvsp[(2) - (2)].chars)); }

/* Line 1457 of yacc.c  */
#line 2170 "../../../src/parse-gram.c"
    break;

  case 30:

/* Line 1457 of yacc.c  */
#line 283 "parse-gram.y"
    {
      char const *skeleton_user = (yyvsp[(2) - (2)].chars);
      if (strchr (skeleton_user, '/'))
        {
          size_t dir_length = strlen (current_file);
          char *skeleton_build;
          while (dir_length && current_file[dir_length - 1] != '/')
            --dir_length;
          while (dir_length && current_file[dir_length - 1] == '/')
            --dir_length;
          skeleton_build =
            xmalloc (dir_length + 1 + strlen (skeleton_user) + 1);
          if (dir_length > 0)
            {
              strncpy (skeleton_build, current_file, dir_length);
              skeleton_build[dir_length++] = '/';
            }
          strcpy (skeleton_build + dir_length, skeleton_user);
          skeleton_user = uniqstr_new (skeleton_build);
          free (skeleton_build);
        }
      skeleton_arg (skeleton_user, grammar_prio, (yylsp[(1) - (2)]));
    }

/* Line 1457 of yacc.c  */
#line 2202 "../../../src/parse-gram.c"
    break;

  case 31:

/* Line 1457 of yacc.c  */
#line 306 "parse-gram.y"
    { token_table_flag = true; }

/* Line 1457 of yacc.c  */
#line 2212 "../../../src/parse-gram.c"
    break;

  case 32:

/* Line 1457 of yacc.c  */
#line 307 "parse-gram.y"
    { report_flag |= report_states; }

/* Line 1457 of yacc.c  */
#line 2222 "../../../src/parse-gram.c"
    break;

  case 33:

/* Line 1457 of yacc.c  */
#line 308 "parse-gram.y"
    { yacc_flag = true; }

/* Line 1457 of yacc.c  */
#line 2232 "../../../src/parse-gram.c"
    break;

  case 37:

/* Line 1457 of yacc.c  */
#line 316 "parse-gram.y"
    {
      grammar_start_symbol_set ((yyvsp[(2) - (2)].symbol), (yylsp[(2) - (2)]));
    }

/* Line 1457 of yacc.c  */
#line 2244 "../../../src/parse-gram.c"
    break;

  case 38:

/* Line 1457 of yacc.c  */
#line 320 "parse-gram.y"
    {
      symbol_list *list;
      for (list = (yyvsp[(3) - (3)].list); list; list = list->next)
	symbol_list_destructor_set (list, (yyvsp[(2) - (3)].code), (yylsp[(2) - (3)]));
      symbol_list_free ((yyvsp[(3) - (3)].list));
    }

/* Line 1457 of yacc.c  */
#line 2259 "../../../src/parse-gram.c"
    break;

  case 39:

/* Line 1457 of yacc.c  */
#line 327 "parse-gram.y"
    {
      symbol_list *list;
      for (list = (yyvsp[(3) - (3)].list); list; list = list->next)
	symbol_list_printer_set (list, (yyvsp[(2) - (3)].code), (yylsp[(2) - (3)]));
      symbol_list_free ((yyvsp[(3) - (3)].list));
    }

/* Line 1457 of yacc.c  */
#line 2274 "../../../src/parse-gram.c"
    break;

  case 40:

/* Line 1457 of yacc.c  */
#line 334 "parse-gram.y"
    {
      default_prec = true;
    }

/* Line 1457 of yacc.c  */
#line 2286 "../../../src/parse-gram.c"
    break;

  case 41:

/* Line 1457 of yacc.c  */
#line 338 "parse-gram.y"
    {
      default_prec = false;
    }

/* Line 1457 of yacc.c  */
#line 2298 "../../../src/parse-gram.c"
    break;

  case 42:

/* Line 1457 of yacc.c  */
#line 342 "parse-gram.y"
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_code_grow ("percent_code()", (yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)]));
      code_scanner_last_string_free ();
    }

/* Line 1457 of yacc.c  */
#line 2313 "../../../src/parse-gram.c"
    break;

  case 43:

/* Line 1457 of yacc.c  */
#line 349 "parse-gram.y"
    {
      muscle_percent_code_grow ((yyvsp[(2) - (3)].uniqstr), (yylsp[(2) - (3)]), (yyvsp[(3) - (3)].chars), (yylsp[(3) - (3)]));
      code_scanner_last_string_free ();
    }

/* Line 1457 of yacc.c  */
#line 2326 "../../../src/parse-gram.c"
    break;

  case 44:

/* Line 1457 of yacc.c  */
#line 363 "parse-gram.y"
    {}

/* Line 1457 of yacc.c  */
#line 2336 "../../../src/parse-gram.c"
    break;

  case 45:

/* Line 1457 of yacc.c  */
#line 364 "parse-gram.y"
    { muscle_code_grow ("union_name", (yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2346 "../../../src/parse-gram.c"
    break;

  case 46:

/* Line 1457 of yacc.c  */
#line 369 "parse-gram.y"
    {
      union_seen = true;
      muscle_code_grow ("stype", (yyvsp[(3) - (3)].chars), (yylsp[(3) - (3)]));
      code_scanner_last_string_free ();
    }

/* Line 1457 of yacc.c  */
#line 2360 "../../../src/parse-gram.c"
    break;

  case 47:

/* Line 1457 of yacc.c  */
#line 380 "parse-gram.y"
    { current_class = nterm_sym; }

/* Line 1457 of yacc.c  */
#line 2370 "../../../src/parse-gram.c"
    break;

  case 48:

/* Line 1457 of yacc.c  */
#line 381 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }

/* Line 1457 of yacc.c  */
#line 2383 "../../../src/parse-gram.c"
    break;

  case 49:

/* Line 1457 of yacc.c  */
#line 385 "parse-gram.y"
    { current_class = token_sym; }

/* Line 1457 of yacc.c  */
#line 2393 "../../../src/parse-gram.c"
    break;

  case 50:

/* Line 1457 of yacc.c  */
#line 386 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }

/* Line 1457 of yacc.c  */
#line 2406 "../../../src/parse-gram.c"
    break;

  case 51:

/* Line 1457 of yacc.c  */
#line 391 "parse-gram.y"
    {
      symbol_list *list;
      tag_seen = true;
      for (list = (yyvsp[(3) - (3)].list); list; list = list->next)
	symbol_type_set (list->content.sym, (yyvsp[(2) - (3)].uniqstr), (yylsp[(2) - (3)]));
      symbol_list_free ((yyvsp[(3) - (3)].list));
    }

/* Line 1457 of yacc.c  */
#line 2422 "../../../src/parse-gram.c"
    break;

  case 52:

/* Line 1457 of yacc.c  */
#line 402 "parse-gram.y"
    {
      symbol_list *list;
      ++current_prec;
      for (list = (yyvsp[(3) - (3)].list); list; list = list->next)
	{
	  symbol_type_set (list->content.sym, current_type, (yylsp[(2) - (3)]));
	  symbol_precedence_set (list->content.sym, current_prec, (yyvsp[(1) - (3)].assoc), (yylsp[(1) - (3)]));
	}
      symbol_list_free ((yyvsp[(3) - (3)].list));
      current_type = NULL;
    }

/* Line 1457 of yacc.c  */
#line 2442 "../../../src/parse-gram.c"
    break;

  case 53:

/* Line 1457 of yacc.c  */
#line 416 "parse-gram.y"
    { (yyval.assoc) = left_assoc; }

/* Line 1457 of yacc.c  */
#line 2452 "../../../src/parse-gram.c"
    break;

  case 54:

/* Line 1457 of yacc.c  */
#line 417 "parse-gram.y"
    { (yyval.assoc) = right_assoc; }

/* Line 1457 of yacc.c  */
#line 2462 "../../../src/parse-gram.c"
    break;

  case 55:

/* Line 1457 of yacc.c  */
#line 418 "parse-gram.y"
    { (yyval.assoc) = non_assoc; }

/* Line 1457 of yacc.c  */
#line 2472 "../../../src/parse-gram.c"
    break;

  case 56:

/* Line 1457 of yacc.c  */
#line 419 "parse-gram.y"
    { (yyval.assoc) = precedence_assoc; }

/* Line 1457 of yacc.c  */
#line 2482 "../../../src/parse-gram.c"
    break;

  case 57:

/* Line 1457 of yacc.c  */
#line 423 "parse-gram.y"
    { current_type = NULL; }

/* Line 1457 of yacc.c  */
#line 2492 "../../../src/parse-gram.c"
    break;

  case 58:

/* Line 1457 of yacc.c  */
#line 424 "parse-gram.y"
    { current_type = (yyvsp[(1) - (1)].uniqstr); tag_seen = true; }

/* Line 1457 of yacc.c  */
#line 2502 "../../../src/parse-gram.c"
    break;

  case 59:

/* Line 1457 of yacc.c  */
#line 430 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[(1) - (1)].symbol), (yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2512 "../../../src/parse-gram.c"
    break;

  case 60:

/* Line 1457 of yacc.c  */
#line 432 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[(1) - (2)].list), symbol_list_sym_new ((yyvsp[(2) - (2)].symbol), (yylsp[(2) - (2)]))); }

/* Line 1457 of yacc.c  */
#line 2522 "../../../src/parse-gram.c"
    break;

  case 61:

/* Line 1457 of yacc.c  */
#line 436 "parse-gram.y"
    { (yyval.symbol) = (yyvsp[(1) - (1)].symbol); }

/* Line 1457 of yacc.c  */
#line 2532 "../../../src/parse-gram.c"
    break;

  case 62:

/* Line 1457 of yacc.c  */
#line 437 "parse-gram.y"
    { (yyval.symbol) = (yyvsp[(1) - (2)].symbol); symbol_user_token_number_set ((yyvsp[(1) - (2)].symbol), (yyvsp[(2) - (2)].integer), (yylsp[(2) - (2)])); }

/* Line 1457 of yacc.c  */
#line 2542 "../../../src/parse-gram.c"
    break;

  case 63:

/* Line 1457 of yacc.c  */
#line 443 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[(1) - (1)].symbol), (yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2552 "../../../src/parse-gram.c"
    break;

  case 64:

/* Line 1457 of yacc.c  */
#line 445 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[(1) - (2)].list), symbol_list_sym_new ((yyvsp[(2) - (2)].symbol), (yylsp[(2) - (2)]))); }

/* Line 1457 of yacc.c  */
#line 2562 "../../../src/parse-gram.c"
    break;

  case 65:

/* Line 1457 of yacc.c  */
#line 449 "parse-gram.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }

/* Line 1457 of yacc.c  */
#line 2572 "../../../src/parse-gram.c"
    break;

  case 66:

/* Line 1457 of yacc.c  */
#line 450 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].list)); }

/* Line 1457 of yacc.c  */
#line 2582 "../../../src/parse-gram.c"
    break;

  case 67:

/* Line 1457 of yacc.c  */
#line 454 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[(1) - (1)].symbol), (yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2592 "../../../src/parse-gram.c"
    break;

  case 68:

/* Line 1457 of yacc.c  */
#line 455 "parse-gram.y"
    { (yyval.list) = symbol_list_type_new ((yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2602 "../../../src/parse-gram.c"
    break;

  case 69:

/* Line 1457 of yacc.c  */
#line 456 "parse-gram.y"
    { (yyval.list) = symbol_list_default_tagged_new ((yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2612 "../../../src/parse-gram.c"
    break;

  case 70:

/* Line 1457 of yacc.c  */
#line 457 "parse-gram.y"
    { (yyval.list) = symbol_list_default_tagless_new ((yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2622 "../../../src/parse-gram.c"
    break;

  case 71:

/* Line 1457 of yacc.c  */
#line 463 "parse-gram.y"
    {
       current_type = (yyvsp[(1) - (1)].uniqstr);
       tag_seen = true;
     }

/* Line 1457 of yacc.c  */
#line 2635 "../../../src/parse-gram.c"
    break;

  case 72:

/* Line 1457 of yacc.c  */
#line 468 "parse-gram.y"
    {
       symbol_class_set ((yyvsp[(1) - (1)].symbol), current_class, (yylsp[(1) - (1)]), true);
       symbol_type_set ((yyvsp[(1) - (1)].symbol), current_type, (yylsp[(1) - (1)]));
     }

/* Line 1457 of yacc.c  */
#line 2648 "../../../src/parse-gram.c"
    break;

  case 73:

/* Line 1457 of yacc.c  */
#line 473 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[(1) - (2)].symbol), current_class, (yylsp[(1) - (2)]), true);
      symbol_type_set ((yyvsp[(1) - (2)].symbol), current_type, (yylsp[(1) - (2)]));
      symbol_user_token_number_set ((yyvsp[(1) - (2)].symbol), (yyvsp[(2) - (2)].integer), (yylsp[(2) - (2)]));
    }

/* Line 1457 of yacc.c  */
#line 2662 "../../../src/parse-gram.c"
    break;

  case 74:

/* Line 1457 of yacc.c  */
#line 479 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[(1) - (2)].symbol), current_class, (yylsp[(1) - (2)]), true);
      symbol_type_set ((yyvsp[(1) - (2)].symbol), current_type, (yylsp[(1) - (2)]));
      symbol_make_alias ((yyvsp[(1) - (2)].symbol), (yyvsp[(2) - (2)].symbol), (yyloc));
    }

/* Line 1457 of yacc.c  */
#line 2676 "../../../src/parse-gram.c"
    break;

  case 75:

/* Line 1457 of yacc.c  */
#line 485 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[(1) - (3)].symbol), current_class, (yylsp[(1) - (3)]), true);
      symbol_type_set ((yyvsp[(1) - (3)].symbol), current_type, (yylsp[(1) - (3)]));
      symbol_user_token_number_set ((yyvsp[(1) - (3)].symbol), (yyvsp[(2) - (3)].integer), (yylsp[(2) - (3)]));
      symbol_make_alias ((yyvsp[(1) - (3)].symbol), (yyvsp[(3) - (3)].symbol), (yyloc));
    }

/* Line 1457 of yacc.c  */
#line 2691 "../../../src/parse-gram.c"
    break;

  case 82:

/* Line 1457 of yacc.c  */
#line 515 "parse-gram.y"
    {
      yyerrok;
    }

/* Line 1457 of yacc.c  */
#line 2703 "../../../src/parse-gram.c"
    break;

  case 83:

/* Line 1457 of yacc.c  */
#line 521 "parse-gram.y"
    { current_lhs = (yyvsp[(1) - (1)].symbol); current_lhs_location = (yylsp[(1) - (1)]); }

/* Line 1457 of yacc.c  */
#line 2713 "../../../src/parse-gram.c"
    break;

  case 85:

/* Line 1457 of yacc.c  */
#line 525 "parse-gram.y"
    { grammar_current_rule_end ((yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2723 "../../../src/parse-gram.c"
    break;

  case 86:

/* Line 1457 of yacc.c  */
#line 526 "parse-gram.y"
    { grammar_current_rule_end ((yylsp[(3) - (3)])); }

/* Line 1457 of yacc.c  */
#line 2733 "../../../src/parse-gram.c"
    break;

  case 88:

/* Line 1457 of yacc.c  */
#line 532 "parse-gram.y"
    { grammar_current_rule_begin (current_lhs, current_lhs_location); }

/* Line 1457 of yacc.c  */
#line 2743 "../../../src/parse-gram.c"
    break;

  case 89:

/* Line 1457 of yacc.c  */
#line 534 "parse-gram.y"
    { grammar_current_rule_symbol_append ((yyvsp[(2) - (2)].symbol), (yylsp[(2) - (2)])); }

/* Line 1457 of yacc.c  */
#line 2753 "../../../src/parse-gram.c"
    break;

  case 90:

/* Line 1457 of yacc.c  */
#line 536 "parse-gram.y"
    { grammar_current_rule_action_append ((yyvsp[(2) - (2)].code), (yylsp[(2) - (2)])); }

/* Line 1457 of yacc.c  */
#line 2763 "../../../src/parse-gram.c"
    break;

  case 91:

/* Line 1457 of yacc.c  */
#line 538 "parse-gram.y"
    { grammar_current_rule_prec_set ((yyvsp[(3) - (3)].symbol), (yylsp[(3) - (3)])); }

/* Line 1457 of yacc.c  */
#line 2773 "../../../src/parse-gram.c"
    break;

  case 92:

/* Line 1457 of yacc.c  */
#line 540 "parse-gram.y"
    { grammar_current_rule_dprec_set ((yyvsp[(3) - (3)].integer), (yylsp[(3) - (3)])); }

/* Line 1457 of yacc.c  */
#line 2783 "../../../src/parse-gram.c"
    break;

  case 93:

/* Line 1457 of yacc.c  */
#line 542 "parse-gram.y"
    { grammar_current_rule_merge_set ((yyvsp[(3) - (3)].uniqstr), (yylsp[(3) - (3)])); }

/* Line 1457 of yacc.c  */
#line 2793 "../../../src/parse-gram.c"
    break;

  case 95:

/* Line 1457 of yacc.c  */
#line 552 "parse-gram.y"
    { (yyval.uniqstr) = uniqstr_new ((yyvsp[(1) - (1)].chars)); }

/* Line 1457 of yacc.c  */
#line 2803 "../../../src/parse-gram.c"
    break;

  case 96:

/* Line 1457 of yacc.c  */
#line 558 "parse-gram.y"
    {
      (yyval.chars) = "";
    }

/* Line 1457 of yacc.c  */
#line 2815 "../../../src/parse-gram.c"
    break;

  case 98:

/* Line 1457 of yacc.c  */
#line 571 "parse-gram.y"
    {
      code_props plain_code;
      (yyvsp[(1) - (1)].code)[strlen ((yyvsp[(1) - (1)].code)) - 1] = '\n';
      code_props_plain_init (&plain_code, (yyvsp[(1) - (1)].code)+1, (yylsp[(1) - (1)]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      (yyval.chars) = plain_code.code;
    }

/* Line 1457 of yacc.c  */
#line 2832 "../../../src/parse-gram.c"
    break;

  case 99:

/* Line 1457 of yacc.c  */
#line 591 "parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2842 "../../../src/parse-gram.c"
    break;

  case 100:

/* Line 1457 of yacc.c  */
#line 593 "parse-gram.y"
    {
      (yyval.symbol) = symbol_get (char_name ((yyvsp[(1) - (1)].character)), (yylsp[(1) - (1)]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[(1) - (1)]), false);
      symbol_user_token_number_set ((yyval.symbol), (yyvsp[(1) - (1)].character), (yylsp[(1) - (1)]));
    }

/* Line 1457 of yacc.c  */
#line 2856 "../../../src/parse-gram.c"
    break;

  case 101:

/* Line 1457 of yacc.c  */
#line 601 "parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }

/* Line 1457 of yacc.c  */
#line 2866 "../../../src/parse-gram.c"
    break;

  case 104:

/* Line 1457 of yacc.c  */
#line 613 "parse-gram.y"
    {
      (yyval.symbol) = symbol_get (quotearg_style (c_quoting_style, (yyvsp[(1) - (1)].chars)), (yylsp[(1) - (1)]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[(1) - (1)]), false);
    }

/* Line 1457 of yacc.c  */
#line 2879 "../../../src/parse-gram.c"
    break;

  case 106:

/* Line 1457 of yacc.c  */
#line 622 "parse-gram.y"
    {
      code_props plain_code;
      code_props_plain_init (&plain_code, (yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      muscle_code_grow ("epilogue", plain_code.code, (yylsp[(2) - (2)]));
      code_scanner_last_string_free ();
    }

/* Line 1457 of yacc.c  */
#line 2896 "../../../src/parse-gram.c"
    break;



/* Line 1457 of yacc.c  */
#line 2902 "../../../src/parse-gram.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
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
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1677 of yacc.c  */
#line 632 "parse-gram.y"



/* Return the location of the left-hand side of a rule whose
   right-hand side is RHS[1] ... RHS[N].  Ignore empty nonterminals in
   the right-hand side, and return an empty location equal to the end
   boundary of RHS[0] if the right-hand side is empty.  */

static YYLTYPE
lloc_default (YYLTYPE const *rhs, int n)
{
  int i;
  YYLTYPE loc;

  /* SGI MIPSpro 7.4.1m miscompiles "loc.start = loc.end = rhs[n].end;".
     The bug is fixed in 7.4.2m, but play it safe for now.  */
  loc.start = rhs[n].end;
  loc.end = rhs[n].end;

  /* Ignore empty nonterminals the start of the the right-hand side.
     Do not bother to ignore them at the end of the right-hand side,
     since empty nonterminals have the same end as their predecessors.  */
  for (i = 1; i <= n; i++)
    if (! equal_boundaries (rhs[i].start, rhs[i].end))
      {
	loc.start = rhs[i].start;
	break;
      }

  return loc;
}


/* Add a lex-param or a parse-param (depending on TYPE) with
   declaration DECL and location LOC.  */

static void
add_param (char const *type, char *decl, location loc)
{
  static char const alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";
  char const *name_start = NULL;
  char *p;

  /* Stop on last actual character.  */
  for (p = decl; p[1]; p++)
    if ((p == decl
	 || ! memchr (alphanum, p[-1], sizeof alphanum))
	&& memchr (alphanum, p[0], sizeof alphanum - 10))
      name_start = p;

  /* Strip the surrounding '{' and '}', and any blanks just inside
     the braces.  */
  while (*--p == ' ' || *p == '\t')
    continue;
  p[1] = '\0';
  while (*++decl == ' ' || *decl == '\t')
    continue;

  if (! name_start)
    complain_at (loc, _("missing identifier in parameter declaration"));
  else
    {
      char *name;
      size_t name_len;

      for (name_len = 1;
	   memchr (alphanum, name_start[name_len], sizeof alphanum);
	   name_len++)
	continue;

      name = xmalloc (name_len + 1);
      memcpy (name, name_start, name_len);
      name[name_len] = '\0';
      muscle_pair_list_grow (type, decl, name);
      free (name);
    }

  gram_scanner_last_string_free ();
}


static void
version_check (location const *loc, char const *version)
{
  if (strverscmp (version, PACKAGE_VERSION) > 0)
    {
      complain_at (*loc, "require bison %s, but have %s",
		   version, PACKAGE_VERSION);
      exit (63);
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
  return yytname[YYTRANSLATE (type)];
}

static char const *
char_name (char c)
{
  if (c == '\'')
    return "'\\''";
  else
    {
      char buf[4];
      buf[0] = '\''; buf[1] = c; buf[2] = '\''; buf[3] = '\0';
      return quotearg_style (escape_quoting_style, buf);
    }
}

