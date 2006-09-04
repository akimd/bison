/* A Bison parser, made by GNU Bison 2.3+.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3+"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse gram_parse
#define yylex   gram_lex
#define yyerror gram_error
#define yylval  gram_lval
#define yychar  gram_char
#define yydebug gram_debug
#define yynerrs gram_nerrs
#define yylloc gram_lloc

/* Copy the first part of user declarations.  */
/* Line 164 of yacc.c.  */
#line 1 "parse-gram.y"
/* Bison Grammar Parser                             -*- C -*-

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301  USA
*/

#include <config.h>
#include "system.h"

#include <strverscmp.h>

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
     PERCENT_SYMBOL_DEFAULT = 265,
     PERCENT_LEFT = 266,
     PERCENT_RIGHT = 267,
     PERCENT_NONASSOC = 268,
     PERCENT_PREC = 269,
     PERCENT_DPREC = 270,
     PERCENT_MERGE = 271,
     PERCENT_AFTER_HEADER = 272,
     PERCENT_BEFORE_HEADER = 273,
     PERCENT_DEBUG = 274,
     PERCENT_DEFAULT_PREC = 275,
     PERCENT_DEFINE = 276,
     PERCENT_DEFINES = 277,
     PERCENT_END_HEADER = 278,
     PERCENT_ERROR_VERBOSE = 279,
     PERCENT_EXPECT = 280,
     PERCENT_EXPECT_RR = 281,
     PERCENT_FILE_PREFIX = 282,
     PERCENT_GLR_PARSER = 283,
     PERCENT_INITIAL_ACTION = 284,
     PERCENT_LEX_PARAM = 285,
     PERCENT_LOCATIONS = 286,
     PERCENT_NAME_PREFIX = 287,
     PERCENT_NO_DEFAULT_PREC = 288,
     PERCENT_NO_LINES = 289,
     PERCENT_NONDETERMINISTIC_PARSER = 290,
     PERCENT_OUTPUT = 291,
     PERCENT_PARSE_PARAM = 292,
     PERCENT_PURE_PARSER = 293,
     PERCENT_REQUIRE = 294,
     PERCENT_SKELETON = 295,
     PERCENT_START = 296,
     PERCENT_START_HEADER = 297,
     PERCENT_TOKEN_TABLE = 298,
     PERCENT_VERBOSE = 299,
     PERCENT_YACC = 300,
     BRACED_CODE = 301,
     CHAR = 302,
     EPILOGUE = 303,
     EQUAL = 304,
     ID = 305,
     ID_COLON = 306,
     PERCENT_PERCENT = 307,
     PIPE = 308,
     PROLOGUE = 309,
     SEMICOLON = 310,
     TYPE = 311,
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
#define PERCENT_SYMBOL_DEFAULT 265
#define PERCENT_LEFT 266
#define PERCENT_RIGHT 267
#define PERCENT_NONASSOC 268
#define PERCENT_PREC 269
#define PERCENT_DPREC 270
#define PERCENT_MERGE 271
#define PERCENT_AFTER_HEADER 272
#define PERCENT_BEFORE_HEADER 273
#define PERCENT_DEBUG 274
#define PERCENT_DEFAULT_PREC 275
#define PERCENT_DEFINE 276
#define PERCENT_DEFINES 277
#define PERCENT_END_HEADER 278
#define PERCENT_ERROR_VERBOSE 279
#define PERCENT_EXPECT 280
#define PERCENT_EXPECT_RR 281
#define PERCENT_FILE_PREFIX 282
#define PERCENT_GLR_PARSER 283
#define PERCENT_INITIAL_ACTION 284
#define PERCENT_LEX_PARAM 285
#define PERCENT_LOCATIONS 286
#define PERCENT_NAME_PREFIX 287
#define PERCENT_NO_DEFAULT_PREC 288
#define PERCENT_NO_LINES 289
#define PERCENT_NONDETERMINISTIC_PARSER 290
#define PERCENT_OUTPUT 291
#define PERCENT_PARSE_PARAM 292
#define PERCENT_PURE_PARSER 293
#define PERCENT_REQUIRE 294
#define PERCENT_SKELETON 295
#define PERCENT_START 296
#define PERCENT_START_HEADER 297
#define PERCENT_TOKEN_TABLE 298
#define PERCENT_VERBOSE 299
#define PERCENT_YACC 300
#define BRACED_CODE 301
#define CHAR 302
#define EPILOGUE 303
#define EQUAL 304
#define ID 305
#define ID_COLON 306
#define PERCENT_PERCENT 307
#define PIPE 308
#define PROLOGUE 309
#define SEMICOLON 310
#define TYPE 311
#define PERCENT_UNION 312




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{/* Line 198 of yacc.c.  */
#line 98 "parse-gram.y"

  symbol *symbol;
  symbol_list *list;
  int integer;
  char *chars;
  assoc assoc;
  uniqstr uniqstr;
  unsigned char character;
}
/* Line 198 of yacc.c.  */
#line 310 "parse-gram.c"
	YYSTYPE;
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

/* Line 221 of yacc.c.  */
#line 334 "parse-gram.c"

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
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
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
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   195

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNRULES -- Number of states.  */
#define YYNSTATES  136

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
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     8,     9,    12,    14,    16,    19,    22,
      24,    28,    30,    33,    35,    38,    41,    45,    47,    50,
      53,    55,    59,    61,    63,    67,    70,    72,    75,    78,
      81,    83,    85,    87,    89,    91,    93,    96,   100,   104,
     106,   108,   109,   111,   115,   116,   120,   121,   125,   129,
     133,   135,   137,   139,   140,   142,   144,   147,   149,   152,
     154,   156,   158,   160,   162,   165,   168,   172,   174,   177,
     179,   182,   184,   187,   190,   191,   195,   197,   201,   204,
     205,   208,   211,   215,   219,   223,   225,   227,   228,   230,
     232,   234,   236,   238,   240,   242,   244,   245
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      59,     0,    -1,    60,    52,    75,    88,    -1,    -1,    60,
      61,    -1,    62,    -1,    54,    -1,    17,    83,    -1,    18,
      83,    -1,    19,    -1,    21,     3,    82,    -1,    22,    -1,
      23,    83,    -1,    24,    -1,    25,     4,    -1,    26,     4,
      -1,    27,    49,     3,    -1,    28,    -1,    29,    46,    -1,
      30,    46,    -1,    31,    -1,    32,    49,     3,    -1,    34,
      -1,    35,    -1,    36,    49,     3,    -1,    37,    46,    -1,
      38,    -1,    39,     3,    -1,    40,     3,    -1,    42,    83,
      -1,    43,    -1,    44,    -1,    45,    -1,    55,    -1,    67,
      -1,    64,    -1,    41,    86,    -1,     8,    46,    71,    -1,
       9,    46,    71,    -1,    20,    -1,    33,    -1,    -1,    50,
      -1,    57,    63,    46,    -1,    -1,     6,    65,    74,    -1,
      -1,     5,    66,    74,    -1,     7,    56,    70,    -1,    68,
      69,    70,    -1,    11,    -1,    12,    -1,    13,    -1,    -1,
      56,    -1,    86,    -1,    70,    86,    -1,    72,    -1,    71,
      72,    -1,    86,    -1,    56,    -1,    10,    -1,    56,    -1,
      84,    -1,    84,     4,    -1,    84,    87,    -1,    84,     4,
      87,    -1,    73,    -1,    74,    73,    -1,    76,    -1,    75,
      76,    -1,    77,    -1,    62,    55,    -1,     1,    55,    -1,
      -1,    85,    78,    79,    -1,    80,    -1,    79,    53,    80,
      -1,    79,    55,    -1,    -1,    80,    86,    -1,    80,    46,
      -1,    80,    14,    86,    -1,    80,    15,     4,    -1,    80,
      16,    56,    -1,     3,    -1,    83,    -1,    -1,    81,    -1,
      46,    -1,    50,    -1,    47,    -1,    51,    -1,    84,    -1,
      87,    -1,     3,    -1,    -1,    52,    48,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   208,   208,   216,   218,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   239,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   261,   262,   263,   267,   275,   283,
     287,   301,   302,   306,   334,   334,   339,   339,   344,   355,
     370,   371,   372,   376,   377,   382,   384,   389,   390,   394,
     395,   396,   401,   406,   411,   417,   423,   434,   435,   444,
     445,   451,   452,   453,   460,   460,   464,   465,   466,   471,
     472,   474,   476,   478,   480,   490,   491,   497,   501,   506,
     522,   524,   533,   538,   539,   544,   551,   553
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"string\"", "\"integer\"",
  "\"%token\"", "\"%nterm\"", "\"%type\"", "\"%destructor\"",
  "\"%printer\"", "\"%symbol-default\"", "\"%left\"", "\"%right\"",
  "\"%nonassoc\"", "\"%prec\"", "\"%dprec\"", "\"%merge\"",
  "\"%after-header\"", "\"%before-header\"", "\"%debug\"",
  "\"%default-prec\"", "\"%define\"", "\"%defines\"", "\"%end-header\"",
  "\"%error-verbose\"", "\"%expect\"", "\"%expect-rr\"",
  "\"%file-prefix\"", "\"%glr-parser\"", "\"%initial-action\"",
  "\"%lex-param\"", "\"%locations\"", "\"%name-prefix\"",
  "\"%no-default-prec\"", "\"%no-lines\"", "\"%nondeterministic-parser\"",
  "\"%output\"", "\"%parse-param\"", "\"%pure-parser\"", "\"%require\"",
  "\"%skeleton\"", "\"%start\"", "\"%start-header\"", "\"%token-table\"",
  "\"%verbose\"", "\"%yacc\"", "\"{...}\"", "\"char\"", "\"epilogue\"",
  "\"=\"", "\"identifier\"", "\"identifier:\"", "\"%%\"", "\"|\"",
  "\"%{...%}\"", "\";\"", "\"type\"", "\"%union\"", "$accept", "input",
  "prologue_declarations", "prologue_declaration", "grammar_declaration",
  "union_name", "symbol_declaration", "@1", "@2", "precedence_declaration",
  "precedence_declarator", "type.opt", "symbols.1", "generic_symlist",
  "generic_symlist_item", "symbol_def", "symbol_defs.1", "grammar",
  "rules_or_grammar_declaration", "rules", "@3", "rhses.1", "rhs",
  "content", "content.opt", "braceless", "id", "id_colon", "symbol",
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
      61,    61,    61,    61,    62,    62,    62,    62,    62,    62,
      62,    63,    63,    62,    65,    64,    66,    64,    64,    67,
      68,    68,    68,    69,    69,    70,    70,    71,    71,    72,
      72,    72,    73,    73,    73,    73,    73,    74,    74,    75,
      75,    76,    76,    76,    78,    77,    79,    79,    79,    80,
      80,    80,    80,    80,    80,    81,    81,    82,    82,    83,
      84,    84,    85,    86,    86,    87,    88,    88
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     2,     1,     1,     2,     2,     1,
       3,     1,     2,     1,     2,     2,     3,     1,     2,     2,
       1,     3,     1,     1,     3,     2,     1,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     3,     3,     1,
       1,     0,     1,     3,     0,     3,     0,     3,     3,     3,
       1,     1,     1,     0,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     2,     2,     3,     1,     2,     1,
       2,     1,     2,     2,     0,     3,     1,     3,     2,     0,
       2,     2,     3,     3,     3,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    46,    44,     0,     0,     0,    50,
      51,    52,     0,     0,     9,    39,     0,    11,     0,    13,
       0,     0,     0,    17,     0,     0,    20,     0,    40,    22,
      23,     0,     0,    26,     0,     0,     0,     0,    30,    31,
      32,     0,     6,    33,    41,     4,     5,    35,    34,    53,
       0,     0,     0,     0,     0,    89,     7,     8,    87,    12,
      14,    15,     0,    18,    19,     0,     0,    25,    27,    28,
      95,    91,    90,    93,    36,    94,    29,     0,    92,     0,
       0,    69,    71,    74,    42,     0,    54,     0,    62,    67,
      47,    63,    45,    48,    55,    61,    60,    37,    57,    59,
      38,    85,    88,    10,    86,    16,    21,    24,    73,    72,
       0,    70,     2,    79,    43,    49,    68,    64,    65,    56,
      58,    97,    75,    76,    66,    79,    78,     0,     0,     0,
      81,    80,    77,    82,    83,    84
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    45,    79,    85,    47,    51,    50,    48,
      49,    87,    93,    97,    98,    89,    90,    80,    81,    82,
     113,   122,   123,   102,   103,    56,    73,    83,    99,    75,
     112
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -75
static const yytype_int16 yypact[] =
{
     -75,    18,   101,   -75,   -75,   -75,   -21,   -20,   -10,   -75,
     -75,   -75,    -8,    -8,   -75,   -75,    37,   -75,    -8,   -75,
      41,    44,     6,   -75,    -5,     3,   -75,     7,   -75,   -75,
     -75,    10,    16,   -75,    61,    67,     0,    -8,   -75,   -75,
     -75,    60,   -75,   -75,    13,   -75,   -75,   -75,   -75,    19,
     -28,   -28,     0,    27,    27,   -75,   -75,   -75,     8,   -75,
     -75,   -75,    73,   -75,   -75,    75,    78,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   -75,   -75,   -75,    30,   -75,    31,
       1,   -75,   -75,   -75,   -75,    36,   -75,     0,   -75,   -75,
     -28,    29,   -28,     0,   -75,   -75,   -75,    27,   -75,   -75,
      27,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
      40,   -75,   -75,   -75,   -75,     0,   -75,    81,   -75,   -75,
     -75,   -75,   -30,   145,   -75,   -75,   -75,     0,    85,    34,
     -75,   -75,   145,   -75,   -75,   -75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -75,   -75,   -75,   -75,    90,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,    11,    43,   -73,   -61,    48,   -75,    14,   -75,
     -75,   -75,   -25,   -75,   -75,     2,   -46,   -75,   -36,   -74,
     -75
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -97
static const yytype_int16 yytable[] =
{
      74,   -96,    77,    70,    91,    91,     4,     5,     6,     7,
       8,   101,     9,    10,    11,    57,    94,   118,     3,    71,
      59,    15,    72,   125,   120,   126,    53,   120,    88,   116,
      70,   116,    70,   117,    28,    52,    54,    95,    55,    76,
      58,    63,    36,   124,    91,    60,    91,    71,    61,    64,
      72,    94,    78,   110,    55,    62,    65,   119,    44,    66,
     104,    77,    67,    84,    68,     4,     5,     6,     7,     8,
      69,     9,    10,    11,    71,    86,   105,    72,   106,   119,
      15,   107,   114,    96,    70,   108,   109,   131,   121,   134,
     135,   133,    46,    28,   111,     0,   131,   100,   115,    92,
     132,    36,     0,     0,     0,     0,     4,     5,     6,     7,
       8,    78,     9,    10,    11,     0,     0,    44,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    70,     0,
       0,     0,     0,    41,     0,    42,    43,     0,    44,   127,
     128,   129,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   130,    71,     0,     0,    72
};

static const yytype_int16 yycheck[] =
{
      36,     0,     1,     3,    50,    51,     5,     6,     7,     8,
       9,     3,    11,    12,    13,    13,    52,    91,     0,    47,
      18,    20,    50,    53,    97,    55,    46,   100,    56,    90,
       3,    92,     3,     4,    33,    56,    46,    10,    46,    37,
       3,    46,    41,   117,    90,     4,    92,    47,     4,    46,
      50,    87,    51,    52,    46,    49,    49,    93,    57,    49,
      58,     1,    46,    50,     3,     5,     6,     7,     8,     9,
       3,    11,    12,    13,    47,    56,     3,    50,     3,   115,
      20,     3,    46,    56,     3,    55,    55,   123,    48,     4,
      56,   127,     2,    33,    80,    -1,   132,    54,    87,    51,
     125,    41,    -1,    -1,    -1,    -1,     5,     6,     7,     8,
       9,    51,    11,    12,    13,    -1,    -1,    57,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,     3,    -1,
      -1,    -1,    -1,    52,    -1,    54,    55,    -1,    57,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    50
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    59,    60,     0,     5,     6,     7,     8,     9,    11,
      12,    13,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    52,    54,    55,    57,    61,    62,    64,    67,    68,
      66,    65,    56,    46,    46,    46,    83,    83,     3,    83,
       4,     4,    49,    46,    46,    49,    49,    46,     3,     3,
       3,    47,    50,    84,    86,    87,    83,     1,    51,    62,
      75,    76,    77,    85,    50,    63,    56,    69,    56,    73,
      74,    84,    74,    70,    86,    10,    56,    71,    72,    86,
      71,     3,    81,    82,    83,     3,     3,     3,    55,    55,
      52,    76,    88,    78,    46,    70,    73,     4,    87,    86,
      72,    48,    79,    80,    87,    53,    55,    14,    15,    16,
      46,    86,    80,    86,     4,    56
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
/* Line 670 of yacc.c.  */
#line 186 "parse-gram.y"
	{ fputs (quotearg_style (c_quoting_style, (yyvaluep->chars)), stderr); };
/* Line 670 of yacc.c.  */
#line 1026 "parse-gram.c"
	break;
      case 4: /* "\"integer\"" */
/* Line 670 of yacc.c.  */
#line 197 "parse-gram.y"
	{ fprintf (stderr, "%d", (yyvaluep->integer)); };
/* Line 670 of yacc.c.  */
#line 1033 "parse-gram.c"
	break;
      case 46: /* "\"{...}\"" */
/* Line 670 of yacc.c.  */
#line 188 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", (yyvaluep->chars)); };
/* Line 670 of yacc.c.  */
#line 1040 "parse-gram.c"
	break;
      case 47: /* "\"char\"" */
/* Line 670 of yacc.c.  */
#line 181 "parse-gram.y"
	{ fputs (char_name ((yyvaluep->character)), stderr); };
/* Line 670 of yacc.c.  */
#line 1047 "parse-gram.c"
	break;
      case 48: /* "\"epilogue\"" */
/* Line 670 of yacc.c.  */
#line 188 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", (yyvaluep->chars)); };
/* Line 670 of yacc.c.  */
#line 1054 "parse-gram.c"
	break;
      case 50: /* "\"identifier\"" */
/* Line 670 of yacc.c.  */
#line 193 "parse-gram.y"
	{ fputs ((yyvaluep->uniqstr), stderr); };
/* Line 670 of yacc.c.  */
#line 1061 "parse-gram.c"
	break;
      case 51: /* "\"identifier:\"" */
/* Line 670 of yacc.c.  */
#line 194 "parse-gram.y"
	{ fprintf (stderr, "%s:", (yyvaluep->uniqstr)); };
/* Line 670 of yacc.c.  */
#line 1068 "parse-gram.c"
	break;
      case 54: /* "\"%{...%}\"" */
/* Line 670 of yacc.c.  */
#line 188 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", (yyvaluep->chars)); };
/* Line 670 of yacc.c.  */
#line 1075 "parse-gram.c"
	break;
      case 56: /* "\"type\"" */
/* Line 670 of yacc.c.  */
#line 192 "parse-gram.y"
	{ fprintf (stderr, "<%s>", (yyvaluep->uniqstr)); };
/* Line 670 of yacc.c.  */
#line 1082 "parse-gram.c"
	break;
      case 81: /* "content" */
/* Line 670 of yacc.c.  */
#line 188 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", (yyvaluep->chars)); };
/* Line 670 of yacc.c.  */
#line 1089 "parse-gram.c"
	break;
      case 82: /* "content.opt" */
/* Line 670 of yacc.c.  */
#line 188 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", (yyvaluep->chars)); };
/* Line 670 of yacc.c.  */
#line 1096 "parse-gram.c"
	break;
      case 83: /* "braceless" */
/* Line 670 of yacc.c.  */
#line 188 "parse-gram.y"
	{ fprintf (stderr, "{\n%s\n}", (yyvaluep->chars)); };
/* Line 670 of yacc.c.  */
#line 1103 "parse-gram.c"
	break;
      case 84: /* "id" */
/* Line 670 of yacc.c.  */
#line 200 "parse-gram.y"
	{ fprintf (stderr, "%s", (yyvaluep->symbol)->tag); };
/* Line 670 of yacc.c.  */
#line 1110 "parse-gram.c"
	break;
      case 85: /* "id_colon" */
/* Line 670 of yacc.c.  */
#line 201 "parse-gram.y"
	{ fprintf (stderr, "%s:", (yyvaluep->symbol)->tag); };
/* Line 670 of yacc.c.  */
#line 1117 "parse-gram.c"
	break;
      case 86: /* "symbol" */
/* Line 670 of yacc.c.  */
#line 200 "parse-gram.y"
	{ fprintf (stderr, "%s", (yyvaluep->symbol)->tag); };
/* Line 670 of yacc.c.  */
#line 1124 "parse-gram.c"
	break;
      case 87: /* "string_as_id" */
/* Line 670 of yacc.c.  */
#line 200 "parse-gram.y"
	{ fprintf (stderr, "%s", (yyvaluep->symbol)->tag); };
/* Line 670 of yacc.c.  */
#line 1131 "parse-gram.c"
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
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






/*----------.
| yyparse.  |
`----------*/

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

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

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
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif


  /* User initialization code.  */
  /* Line 1082 of yacc.c.  */
#line 89 "parse-gram.y"
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_set (&yylloc.start, current_file, 1, 1);
  boundary_set (&yylloc.end, current_file, 1, 1);
}
/* Line 1082 of yacc.c.  */
#line 1653 "parse-gram.c"
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
/* Line 1269 of yacc.c.  */
#line 223 "parse-gram.y"
    { prologue_augment (translate_code ((yyvsp[(1) - (1)].chars), (yylsp[(1) - (1)])), (yylsp[(1) - (1)]), union_seen); }
    break;

  case 7:
/* Line 1269 of yacc.c.  */
#line 224 "parse-gram.y"
    { prologue_augment ((yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)]), true); }
    break;

  case 8:
/* Line 1269 of yacc.c.  */
#line 225 "parse-gram.y"
    { prologue_augment ((yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)]), false); }
    break;

  case 9:
/* Line 1269 of yacc.c.  */
#line 226 "parse-gram.y"
    { debug_flag = true; }
    break;

  case 10:
/* Line 1269 of yacc.c.  */
#line 227 "parse-gram.y"
    { muscle_insert ((yyvsp[(2) - (3)].chars), (yyvsp[(3) - (3)].chars)); }
    break;

  case 11:
/* Line 1269 of yacc.c.  */
#line 228 "parse-gram.y"
    { defines_flag = true; }
    break;

  case 12:
/* Line 1269 of yacc.c.  */
#line 229 "parse-gram.y"
    { muscle_code_grow ("end_header", (yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)])); }
    break;

  case 13:
/* Line 1269 of yacc.c.  */
#line 230 "parse-gram.y"
    { error_verbose = true; }
    break;

  case 14:
/* Line 1269 of yacc.c.  */
#line 231 "parse-gram.y"
    { expected_sr_conflicts = (yyvsp[(2) - (2)].integer); }
    break;

  case 15:
/* Line 1269 of yacc.c.  */
#line 232 "parse-gram.y"
    { expected_rr_conflicts = (yyvsp[(2) - (2)].integer); }
    break;

  case 16:
/* Line 1269 of yacc.c.  */
#line 233 "parse-gram.y"
    { spec_file_prefix = (yyvsp[(3) - (3)].chars); }
    break;

  case 17:
/* Line 1269 of yacc.c.  */
#line 235 "parse-gram.y"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
    break;

  case 18:
/* Line 1269 of yacc.c.  */
#line 240 "parse-gram.y"
    {
      muscle_code_grow ("initial_action", translate_symbol_action ((yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)])), (yylsp[(2) - (2)]));
    }
    break;

  case 19:
/* Line 1269 of yacc.c.  */
#line 243 "parse-gram.y"
    { add_param ("lex_param", (yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)])); }
    break;

  case 20:
/* Line 1269 of yacc.c.  */
#line 244 "parse-gram.y"
    { locations_flag = true; }
    break;

  case 21:
/* Line 1269 of yacc.c.  */
#line 245 "parse-gram.y"
    { spec_name_prefix = (yyvsp[(3) - (3)].chars); }
    break;

  case 22:
/* Line 1269 of yacc.c.  */
#line 246 "parse-gram.y"
    { no_lines_flag = true; }
    break;

  case 23:
/* Line 1269 of yacc.c.  */
#line 247 "parse-gram.y"
    { nondeterministic_parser = true; }
    break;

  case 24:
/* Line 1269 of yacc.c.  */
#line 248 "parse-gram.y"
    { spec_outfile = (yyvsp[(3) - (3)].chars); }
    break;

  case 25:
/* Line 1269 of yacc.c.  */
#line 249 "parse-gram.y"
    { add_param ("parse_param", (yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)])); }
    break;

  case 26:
/* Line 1269 of yacc.c.  */
#line 250 "parse-gram.y"
    { pure_parser = true; }
    break;

  case 27:
/* Line 1269 of yacc.c.  */
#line 251 "parse-gram.y"
    { version_check (&(yylsp[(2) - (2)]), (yyvsp[(2) - (2)].chars)); }
    break;

  case 28:
/* Line 1269 of yacc.c.  */
#line 252 "parse-gram.y"
    { skeleton = (yyvsp[(2) - (2)].chars); }
    break;

  case 29:
/* Line 1269 of yacc.c.  */
#line 253 "parse-gram.y"
    { muscle_code_grow ("start_header", (yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)])); }
    break;

  case 30:
/* Line 1269 of yacc.c.  */
#line 254 "parse-gram.y"
    { token_table_flag = true; }
    break;

  case 31:
/* Line 1269 of yacc.c.  */
#line 255 "parse-gram.y"
    { report_flag = report_states; }
    break;

  case 32:
/* Line 1269 of yacc.c.  */
#line 256 "parse-gram.y"
    { yacc_flag = true; }
    break;

  case 36:
/* Line 1269 of yacc.c.  */
#line 264 "parse-gram.y"
    {
      grammar_start_symbol_set ((yyvsp[(2) - (2)].symbol), (yylsp[(2) - (2)]));
    }
    break;

  case 37:
/* Line 1269 of yacc.c.  */
#line 268 "parse-gram.y"
    {
      symbol_list *list;
      const char *action = translate_symbol_action ((yyvsp[(2) - (3)].chars), (yylsp[(2) - (3)]));
      for (list = (yyvsp[(3) - (3)].list); list; list = list->next)
        symbol_list_destructor_set (list, action, (yylsp[(2) - (3)]));
      symbol_list_free ((yyvsp[(3) - (3)].list));
    }
    break;

  case 38:
/* Line 1269 of yacc.c.  */
#line 276 "parse-gram.y"
    {
      symbol_list *list;
      const char *action = translate_symbol_action ((yyvsp[(2) - (3)].chars), (yylsp[(2) - (3)]));
      for (list = (yyvsp[(3) - (3)].list); list; list = list->next)
        symbol_list_printer_set (list, action, (yylsp[(2) - (3)]));
      symbol_list_free ((yyvsp[(3) - (3)].list));
    }
    break;

  case 39:
/* Line 1269 of yacc.c.  */
#line 284 "parse-gram.y"
    {
      default_prec = true;
    }
    break;

  case 40:
/* Line 1269 of yacc.c.  */
#line 288 "parse-gram.y"
    {
      default_prec = false;
    }
    break;

  case 41:
/* Line 1269 of yacc.c.  */
#line 301 "parse-gram.y"
    {}
    break;

  case 42:
/* Line 1269 of yacc.c.  */
#line 302 "parse-gram.y"
    { muscle_code_grow ("union_name", (yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }
    break;

  case 43:
/* Line 1269 of yacc.c.  */
#line 307 "parse-gram.y"
    {
      char const *body = (yyvsp[(3) - (3)].chars);

      /* Concatenate the %union bodies.  If this is the first %union, make sure
	 the synchronization line appears after the opening '{' so as not to
	 confuse Doxygen.  Otherwise, turn the previous %union's trailing '}'
	 into '\n', and omit the new %union's leading '{'.  */
      if (!union_seen)
	{
	  muscle_grow ("stype", "{", "");
	}
      else
	{
	  char *code = muscle_find ("stype");
	  code[strlen (code) - 1] = '\n';
	}
      body++;

      union_seen = true;
      muscle_code_grow ("stype", body, (yylsp[(3) - (3)]));
    }
    break;

  case 44:
/* Line 1269 of yacc.c.  */
#line 334 "parse-gram.y"
    { current_class = nterm_sym; }
    break;

  case 45:
/* Line 1269 of yacc.c.  */
#line 335 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 46:
/* Line 1269 of yacc.c.  */
#line 339 "parse-gram.y"
    { current_class = token_sym; }
    break;

  case 47:
/* Line 1269 of yacc.c.  */
#line 340 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
    break;

  case 48:
/* Line 1269 of yacc.c.  */
#line 345 "parse-gram.y"
    {
      tag_seen = true;
      symbol_list *list;
      for (list = (yyvsp[(3) - (3)].list); list; list = list->next)
	symbol_type_set (list->content.sym, (yyvsp[(2) - (3)].uniqstr), (yylsp[(2) - (3)]));
      symbol_list_free ((yyvsp[(3) - (3)].list));
    }
    break;

  case 49:
/* Line 1269 of yacc.c.  */
#line 356 "parse-gram.y"
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
    break;

  case 50:
/* Line 1269 of yacc.c.  */
#line 370 "parse-gram.y"
    { (yyval.assoc) = left_assoc; }
    break;

  case 51:
/* Line 1269 of yacc.c.  */
#line 371 "parse-gram.y"
    { (yyval.assoc) = right_assoc; }
    break;

  case 52:
/* Line 1269 of yacc.c.  */
#line 372 "parse-gram.y"
    { (yyval.assoc) = non_assoc; }
    break;

  case 53:
/* Line 1269 of yacc.c.  */
#line 376 "parse-gram.y"
    { current_type = NULL; }
    break;

  case 54:
/* Line 1269 of yacc.c.  */
#line 377 "parse-gram.y"
    { current_type = (yyvsp[(1) - (1)].uniqstr); tag_seen = true; }
    break;

  case 55:
/* Line 1269 of yacc.c.  */
#line 383 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[(1) - (1)].symbol), (yylsp[(1) - (1)])); }
    break;

  case 56:
/* Line 1269 of yacc.c.  */
#line 385 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[(1) - (2)].list), symbol_list_sym_new ((yyvsp[(2) - (2)].symbol), (yylsp[(2) - (2)]))); }
    break;

  case 57:
/* Line 1269 of yacc.c.  */
#line 389 "parse-gram.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }
    break;

  case 58:
/* Line 1269 of yacc.c.  */
#line 390 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].list)); }
    break;

  case 59:
/* Line 1269 of yacc.c.  */
#line 394 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[(1) - (1)].symbol), (yylsp[(1) - (1)])); }
    break;

  case 60:
/* Line 1269 of yacc.c.  */
#line 395 "parse-gram.y"
    { (yyval.list) = symbol_list_type_new ((yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }
    break;

  case 61:
/* Line 1269 of yacc.c.  */
#line 396 "parse-gram.y"
    { (yyval.list) = symbol_list_default_new ((yylsp[(1) - (1)])); }
    break;

  case 62:
/* Line 1269 of yacc.c.  */
#line 402 "parse-gram.y"
    {
       current_type = (yyvsp[(1) - (1)].uniqstr);
       tag_seen = true;
     }
    break;

  case 63:
/* Line 1269 of yacc.c.  */
#line 407 "parse-gram.y"
    {
       symbol_class_set ((yyvsp[(1) - (1)].symbol), current_class, (yylsp[(1) - (1)]), true);
       symbol_type_set ((yyvsp[(1) - (1)].symbol), current_type, (yylsp[(1) - (1)]));
     }
    break;

  case 64:
/* Line 1269 of yacc.c.  */
#line 412 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[(1) - (2)].symbol), current_class, (yylsp[(1) - (2)]), true);
      symbol_type_set ((yyvsp[(1) - (2)].symbol), current_type, (yylsp[(1) - (2)]));
      symbol_user_token_number_set ((yyvsp[(1) - (2)].symbol), (yyvsp[(2) - (2)].integer), (yylsp[(2) - (2)]));
    }
    break;

  case 65:
/* Line 1269 of yacc.c.  */
#line 418 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[(1) - (2)].symbol), current_class, (yylsp[(1) - (2)]), true);
      symbol_type_set ((yyvsp[(1) - (2)].symbol), current_type, (yylsp[(1) - (2)]));
      symbol_make_alias ((yyvsp[(1) - (2)].symbol), (yyvsp[(2) - (2)].symbol), (yyloc));
    }
    break;

  case 66:
/* Line 1269 of yacc.c.  */
#line 424 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[(1) - (3)].symbol), current_class, (yylsp[(1) - (3)]), true);
      symbol_type_set ((yyvsp[(1) - (3)].symbol), current_type, (yylsp[(1) - (3)]));
      symbol_user_token_number_set ((yyvsp[(1) - (3)].symbol), (yyvsp[(2) - (3)].integer), (yylsp[(2) - (3)]));
      symbol_make_alias ((yyvsp[(1) - (3)].symbol), (yyvsp[(3) - (3)].symbol), (yyloc));
    }
    break;

  case 73:
/* Line 1269 of yacc.c.  */
#line 454 "parse-gram.y"
    {
      yyerrok;
    }
    break;

  case 74:
/* Line 1269 of yacc.c.  */
#line 460 "parse-gram.y"
    { current_lhs = (yyvsp[(1) - (1)].symbol); current_lhs_location = (yylsp[(1) - (1)]); }
    break;

  case 76:
/* Line 1269 of yacc.c.  */
#line 464 "parse-gram.y"
    { grammar_current_rule_end ((yylsp[(1) - (1)])); }
    break;

  case 77:
/* Line 1269 of yacc.c.  */
#line 465 "parse-gram.y"
    { grammar_current_rule_end ((yylsp[(3) - (3)])); }
    break;

  case 79:
/* Line 1269 of yacc.c.  */
#line 471 "parse-gram.y"
    { grammar_current_rule_begin (current_lhs, current_lhs_location); }
    break;

  case 80:
/* Line 1269 of yacc.c.  */
#line 473 "parse-gram.y"
    { grammar_current_rule_symbol_append ((yyvsp[(2) - (2)].symbol), (yylsp[(2) - (2)])); }
    break;

  case 81:
/* Line 1269 of yacc.c.  */
#line 475 "parse-gram.y"
    { grammar_current_rule_action_append ((yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)])); }
    break;

  case 82:
/* Line 1269 of yacc.c.  */
#line 477 "parse-gram.y"
    { grammar_current_rule_prec_set ((yyvsp[(3) - (3)].symbol), (yylsp[(3) - (3)])); }
    break;

  case 83:
/* Line 1269 of yacc.c.  */
#line 479 "parse-gram.y"
    { grammar_current_rule_dprec_set ((yyvsp[(3) - (3)].integer), (yylsp[(3) - (3)])); }
    break;

  case 84:
/* Line 1269 of yacc.c.  */
#line 481 "parse-gram.y"
    { grammar_current_rule_merge_set ((yyvsp[(3) - (3)].uniqstr), (yylsp[(3) - (3)])); }
    break;

  case 87:
/* Line 1269 of yacc.c.  */
#line 497 "parse-gram.y"
    {
      static char one[] = "1";
      (yyval.chars) = one;
    }
    break;

  case 89:
/* Line 1269 of yacc.c.  */
#line 507 "parse-gram.y"
    {
      (yyvsp[(1) - (1)].chars)[strlen ((yyvsp[(1) - (1)].chars)) - 1] = '\n';
      (yyval.chars) = translate_code ((yyvsp[(1) - (1)].chars) + 1, (yylsp[(1) - (1)]));
    }
    break;

  case 90:
/* Line 1269 of yacc.c.  */
#line 523 "parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }
    break;

  case 91:
/* Line 1269 of yacc.c.  */
#line 525 "parse-gram.y"
    {
      (yyval.symbol) = symbol_get (char_name ((yyvsp[(1) - (1)].character)), (yylsp[(1) - (1)]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[(1) - (1)]), false);
      symbol_user_token_number_set ((yyval.symbol), (yyvsp[(1) - (1)].character), (yylsp[(1) - (1)]));
    }
    break;

  case 92:
/* Line 1269 of yacc.c.  */
#line 533 "parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[(1) - (1)].uniqstr), (yylsp[(1) - (1)])); }
    break;

  case 95:
/* Line 1269 of yacc.c.  */
#line 545 "parse-gram.y"
    {
      (yyval.symbol) = symbol_get (quotearg_style (c_quoting_style, (yyvsp[(1) - (1)].chars)), (yylsp[(1) - (1)]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[(1) - (1)]), false);
    }
    break;

  case 97:
/* Line 1269 of yacc.c.  */
#line 554 "parse-gram.y"
    {
      muscle_code_grow ("epilogue", translate_code ((yyvsp[(2) - (2)].chars), (yylsp[(2) - (2)])), (yylsp[(2) - (2)]));
      gram_scanner_last_string_free ();
    }
    break;


/* Line 1269 of yacc.c.  */
#line 2390 "parse-gram.c"
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

#ifndef yyoverflow
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


/* Line 1486 of yacc.c.  */
#line 560 "parse-gram.y"



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

