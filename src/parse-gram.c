/* A Bison parser, made by GNU Bison 2.4.266-78f6.  */

/* Implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2007, 2008, 2009 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "2.4.266-78f6"

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

   Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009 Free Software
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
#line 152 "../../src/parse-gram.c"

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
     PERCENT_DEFAULT_PREC = 273,
     PERCENT_DEFINE = 274,
     PERCENT_DEFINES = 275,
     PERCENT_ERROR_VERBOSE = 276,
     PERCENT_EXPECT = 277,
     PERCENT_EXPECT_RR = 278,
     PERCENT_FLAG = 279,
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
     TAG = 309,
     TAG_ANY = 310,
     TAG_NONE = 311,
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
#define PERCENT_DEFAULT_PREC 273
#define PERCENT_DEFINE 274
#define PERCENT_DEFINES 275
#define PERCENT_ERROR_VERBOSE 276
#define PERCENT_EXPECT 277
#define PERCENT_EXPECT_RR 278
#define PERCENT_FLAG 279
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
#define TAG 309
#define TAG_ANY 310
#define TAG_NONE 311
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
#line 315 "../../src/parse-gram.c"
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
#line 339 "../../src/parse-gram.c"

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
#define YYNRULES  104
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  142

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   312

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
  /* YYRLINEYYN -- Source line where rule number YYN was defined.    */
static const yytype_uint16 yyrline[] =
{
       0,   204,   204,   212,   214,   218,   219,   229,   233,   237,
     238,   243,   244,   245,   246,   247,   252,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   296,
     297,   298,   299,   303,   304,   305,   309,   316,   323,   327,
     331,   338,   353,   354,   358,   370,   370,   375,   375,   380,
     391,   406,   407,   408,   409,   413,   414,   419,   421,   426,
     427,   432,   434,   439,   440,   444,   445,   446,   447,   452,
     457,   462,   468,   474,   485,   486,   495,   496,   502,   503,
     504,   511,   511,   515,   516,   517,   522,   523,   525,   527,
     529,   531,   541,   542,   548,   551,   560,   580,   582,   591,
     596,   597,   602,   609,   611
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
  "\"%default-prec\"", "\"%define\"", "\"%defines\"", "\"%error-verbose\"",
  "\"%expect\"", "\"%expect-rr\"", "\"%<flag>\"", "\"%file-prefix\"",
  "\"%glr-parser\"", "\"%initial-action\"", "\"%language\"",
  "\"%lex-param\"", "\"%locations\"", "\"%name-prefix\"",
  "\"%no-default-prec\"", "\"%no-lines\"", "\"%nondeterministic-parser\"",
  "\"%output\"", "\"%parse-param\"", "\"%pure-parser\"", "\"%require\"",
  "\"%skeleton\"", "\"%start\"", "\"%token-table\"", "\"%verbose\"",
  "\"%yacc\"", "\"{...}\"", "\"char\"", "\"epilogue\"", "\"=\"",
  "\"identifier\"", "\"identifier:\"", "\"%%\"", "\"|\"", "\"%{...%}\"",
  "\";\"", "\"<tag>\"", "\"<*>\"", "\"<>\"", "\"%union\"", "$accept",
  "input", "prologue_declarations", "prologue_declaration",
  "grammar_declaration", "union_name", "symbol_declaration", "$@1", "$@2",
  "precedence_declaration", "precedence_declarator", "tag.opt",
  "symbols.prec", "symbol.prec", "symbols.1", "generic_symlist",
  "generic_symlist_item", "symbol_def", "symbol_defs.1", "grammar",
  "rules_or_grammar_declaration", "rules", "$@3", "rhses.1", "rhs",
  "variable", "content.opt", "braceless", "id", "id_colon", "symbol",
  "string_as_id", "epilogue.opt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
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

#define YYPACT_NINF -81

#define YYTABLE_NINF -104

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.    */
static const yytype_int16 yypact[] =
{
     -81,    21,   111,   -81,   -81,   -81,   -14,    -8,     3,   -81,
     -81,   -81,   -81,   -18,   -81,    14,    41,    42,    49,   -81,
       2,   -81,    10,    53,    34,   -81,    13,   -81,   -81,   -81,
      26,    35,    54,    78,     0,   -81,   -81,   -81,    58,   -81,
     -81,    36,   -81,   -81,   -81,   -81,    28,   -23,   -23,     0,
      32,    32,   -81,    45,   -81,   -81,   -81,    82,   -81,   -81,
     -81,   -81,    88,   -81,   -81,   -81,   -81,    89,   -81,    91,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,    43,
     -81,    44,     1,   -81,   -81,   -81,   -81,    45,   -81,     0,
     -81,   -81,   -23,    39,   -23,     0,   -81,   -81,   -81,   -81,
      32,   -81,   -81,    32,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,    55,   -81,   -81,   -81,   -81,     0,   -81,    96,
     -81,    92,   -81,   -81,   -81,   -81,   -19,   155,   -81,   -81,
     -81,   -81,   -81,     0,    98,    51,   -81,   -81,   155,   -81,
     -81,   -81
};

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     does not specify something else to do.  Zero means the default is an
     error.    */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    47,    45,     0,     0,     0,    51,
      52,    53,    54,     0,    38,     0,     9,     0,     0,     7,
       0,    15,     0,     0,     0,    19,     0,    39,    22,    23,
       0,     0,     0,     0,     0,    29,    30,    31,     0,     6,
      32,    42,     4,     5,    34,    33,    55,     0,     0,     0,
       0,     0,    96,     0,    40,    93,    92,    94,    10,    11,
      12,    13,     0,    16,    17,    18,    20,     0,    24,     0,
      26,    27,    28,   102,    98,    97,   100,    35,   101,     0,
      99,     0,     0,    76,    78,    81,    43,     0,    56,     0,
      69,    74,    48,    70,    46,    49,    61,    66,    67,    68,
      36,    63,    65,    37,    41,    95,     8,    14,    21,    25,
      80,    79,     0,    77,     2,    86,    44,    50,    57,    59,
      75,    71,    72,    62,    64,   104,    82,    83,    58,    60,
      73,    86,    85,     0,     0,     0,    88,    87,    84,    89,
      90,    91
};

  /* YYPGOTO[NTERM-NUM].    */
static const yytype_int8 yypgoto[] =
{
     -81,   -81,   -81,   -81,   101,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -11,   -81,    57,   -80,   -55,    61,   -81,
      29,   -81,   -81,   -81,   -21,   -81,   -81,   -49,   -20,   -81,
     -34,   -69,   -81
};

  /* YYDEFGOTO[NTERM-NUM].    */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    42,    81,    87,    44,    48,    47,    45,
      46,    89,   117,   118,    95,   100,   101,    91,    92,    82,
      83,    84,   115,   126,   127,    57,   106,    54,    76,    85,
     102,    78,   114
};

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.
     If YYTABLE_NINF, syntax error.    */
static const yytype_int16 yytable[] =
{
      77,  -103,    79,    73,   104,    61,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    96,    66,    55,    13,    14,
     124,     3,    74,   124,   122,    75,    52,    93,    93,    68,
      53,    90,   131,    27,   132,    73,    50,   120,   116,   120,
      49,    34,    73,   121,    58,    74,    59,    51,    75,    62,
      80,   112,   130,    60,    63,   119,    64,    71,    41,    79,
      67,   123,    56,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    93,    69,    93,    13,    14,    74,    65,    70,
      75,    72,    88,   119,    86,   105,    97,    98,    99,    52,
      27,   107,   108,   137,   109,    73,   110,   111,    34,   139,
     129,   125,   140,    43,   137,   141,   128,    80,   103,    94,
     138,   113,     0,     0,     0,    41,     4,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,     0,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     0,    32,
      33,    34,    35,    36,    37,     0,     0,     0,    73,     0,
       0,    38,     0,    39,    40,     0,     0,     0,    41,   133,
     134,   135,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   136,
      74,     0,     0,    75
};

static const yytype_int16 yycheck[] =
{
      34,     0,     1,     3,    53,     3,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    49,     3,     3,    17,    18,
     100,     0,    45,   103,    93,    48,    44,    47,    48,     3,
      48,    54,    51,    32,    53,     3,    44,    92,    87,    94,
      54,    40,     3,     4,     3,    45,     4,    44,    48,    47,
      49,    50,   121,     4,    44,    89,     3,     3,    57,     1,
      47,    95,    48,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    92,    47,    94,    17,    18,    45,    44,    44,
      48,     3,    54,   117,    48,     3,    54,    55,    56,    44,
      32,     3,     3,   127,     3,     3,    53,    53,    40,   133,
       4,    46,     4,     2,   138,    54,   117,    49,    51,    48,
     131,    82,    -1,    -1,    -1,    57,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    -1,    -1,    -1,     3,    -1,
      -1,    50,    -1,    52,    53,    -1,    -1,    -1,    57,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    48
};

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.    */
static const yytype_uint8 yystos[] =
{
       0,    59,    60,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    17,    18,    19,    20,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    38,    39,    40,    41,    42,    43,    50,    52,
      53,    57,    61,    62,    64,    67,    68,    66,    65,    54,
      44,    44,    44,    48,    85,     3,    48,    83,     3,     4,
       4,     3,    47,    44,     3,    44,     3,    47,     3,    47,
      44,     3,     3,     3,    45,    48,    86,    88,    89,     1,
      49,    62,    77,    78,    79,    87,    48,    63,    54,    69,
      54,    75,    76,    86,    76,    72,    88,    54,    55,    56,
      73,    74,    88,    73,    85,     3,    84,     3,     3,     3,
      53,    53,    50,    78,    90,    80,    85,    70,    71,    88,
      75,     4,    89,    88,    74,    46,    81,    82,    71,     4,
      89,    51,    53,    14,    15,    16,    44,    88,    82,    88,
       4,    54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.    */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    60,    60,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    63,    63,    62,    65,    64,    66,    64,    64,
      67,    68,    68,    68,    68,    69,    69,    70,    70,    71,
      71,    72,    72,    73,    73,    74,    74,    74,    74,    75,
      75,    75,    75,    75,    76,    76,    77,    77,    78,    78,
      78,    80,    79,    81,    81,    81,    82,    82,    82,    82,
      82,    82,    83,    83,    84,    84,    85,    86,    86,    87,
      88,    88,    89,    90,    90
};

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.    */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     2,     1,     1,     1,     3,     1,
       2,     2,     2,     2,     3,     1,     2,     2,     2,     1,
       2,     3,     1,     1,     2,     3,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     3,     3,     1,     1,
       2,     3,     0,     1,     3,     0,     3,     0,     3,     3,
       3,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     1,     2,     1,     2,     1,     2,
       2,     0,     3,     1,     3,     2,     0,     2,     2,     3,
       3,     3,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     2
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
            case 3: // "string"

/* Line 647 of yacc.c  */
#line 181 "parse-gram.y"
        { fputs (quotearg_style (c_quoting_style, ((*yyvaluep).chars)), stderr); }
/* Line 647 of yacc.c  */
#line 999 "../../src/parse-gram.c"
        break;

            case 4: // "integer"

/* Line 647 of yacc.c  */
#line 193 "parse-gram.y"
        { fprintf (stderr, "%d", ((*yyvaluep).integer)); }
/* Line 647 of yacc.c  */
#line 1008 "../../src/parse-gram.c"
        break;

            case 24: // "%<flag>"

/* Line 647 of yacc.c  */
#line 189 "parse-gram.y"
        { fprintf (stderr, "%%%s", ((*yyvaluep).uniqstr)); }
/* Line 647 of yacc.c  */
#line 1017 "../../src/parse-gram.c"
        break;

            case 44: // "{...}"

/* Line 647 of yacc.c  */
#line 183 "parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).code)); }
/* Line 647 of yacc.c  */
#line 1026 "../../src/parse-gram.c"
        break;

            case 45: // "char"

/* Line 647 of yacc.c  */
#line 175 "parse-gram.y"
        { fputs (char_name (((*yyvaluep).character)), stderr); }
/* Line 647 of yacc.c  */
#line 1035 "../../src/parse-gram.c"
        break;

            case 46: // "epilogue"

/* Line 647 of yacc.c  */
#line 183 "parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 647 of yacc.c  */
#line 1044 "../../src/parse-gram.c"
        break;

            case 48: // "identifier"

/* Line 647 of yacc.c  */
#line 187 "parse-gram.y"
        { fputs (((*yyvaluep).uniqstr), stderr); }
/* Line 647 of yacc.c  */
#line 1053 "../../src/parse-gram.c"
        break;

            case 49: // "identifier:"

/* Line 647 of yacc.c  */
#line 188 "parse-gram.y"
        { fprintf (stderr, "%s:", ((*yyvaluep).uniqstr)); }
/* Line 647 of yacc.c  */
#line 1062 "../../src/parse-gram.c"
        break;

            case 52: // "%{...%}"

/* Line 647 of yacc.c  */
#line 183 "parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 647 of yacc.c  */
#line 1071 "../../src/parse-gram.c"
        break;

            case 54: // "<tag>"

/* Line 647 of yacc.c  */
#line 190 "parse-gram.y"
        { fprintf (stderr, "<%s>", ((*yyvaluep).uniqstr)); }
/* Line 647 of yacc.c  */
#line 1080 "../../src/parse-gram.c"
        break;

            case 83: // variable

/* Line 647 of yacc.c  */
#line 187 "parse-gram.y"
        { fputs (((*yyvaluep).uniqstr), stderr); }
/* Line 647 of yacc.c  */
#line 1089 "../../src/parse-gram.c"
        break;

            case 84: // content.opt

/* Line 647 of yacc.c  */
#line 183 "parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 647 of yacc.c  */
#line 1098 "../../src/parse-gram.c"
        break;

            case 85: // braceless

/* Line 647 of yacc.c  */
#line 183 "parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 647 of yacc.c  */
#line 1107 "../../src/parse-gram.c"
        break;

            case 86: // id

/* Line 647 of yacc.c  */
#line 196 "parse-gram.y"
        { fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); }
/* Line 647 of yacc.c  */
#line 1116 "../../src/parse-gram.c"
        break;

            case 87: // id_colon

/* Line 647 of yacc.c  */
#line 197 "parse-gram.y"
        { fprintf (stderr, "%s:", ((*yyvaluep).symbol)->tag); }
/* Line 647 of yacc.c  */
#line 1125 "../../src/parse-gram.c"
        break;

            case 88: // symbol

/* Line 647 of yacc.c  */
#line 196 "parse-gram.y"
        { fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); }
/* Line 647 of yacc.c  */
#line 1134 "../../src/parse-gram.c"
        break;

            case 89: // string_as_id

/* Line 647 of yacc.c  */
#line 196 "parse-gram.y"
        { fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); }
/* Line 647 of yacc.c  */
#line 1143 "../../src/parse-gram.c"
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyssp, yyvsp, yylsp, yyrule)
    yytype_int16 *yyssp;
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
   YYTOKEN while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yytoken)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      /* Internationalized format string. */
      const char *yyformat = 0;
      /* Arguments of yyformat. */
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      /* Number of reported tokens (one for the "unexpected", one per
         "expected"). */
      int yycount = 0;
      int yyx;

      yyarg[yycount++] = yytname[yytoken];

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	  }

        switch (yycount)
        {
#define YYCASE_(N, S)                           \
          case N:                               \
            yyformat = S;                       \
          break
          YYCASE_(1, YY_("syntax error, unexpected %s"));
          YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
          YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
          YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
          YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
        }

      yysize1 = yysize + yystrlen (yyformat);
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
	  while ((*yyp = *yyformat) != '\0')
            if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
              {
                yyp += yytnamerr (yyp, yyarg[yyi++]);
                yyformat += 2;
              }
            else
              {
                yyp++;
                yyformat++;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
/* Line 1167 of yacc.c  */
#line 84 "parse-gram.y"
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_set (&yylloc.start, current_file, 1, 1);
  boundary_set (&yylloc.end, current_file, 1, 1);
}
/* Line 1167 of yacc.c  */
#line 1665 "../../src/parse-gram.c"
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
/* Line 1380 of yacc.c  */
#line 220 "parse-gram.y"
    {
      code_props plain_code;
      code_props_plain_init (&plain_code, (yyvsp[0].chars), (yylsp[0]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      muscle_code_grow (union_seen ? "post_prologue" : "pre_prologue",
                        plain_code.code, (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1380 of yacc.c  */
#line 1863 "../../src/parse-gram.c"
    break;

  case 7:
/* Line 1380 of yacc.c  */
#line 230 "parse-gram.y"
    {
      muscle_percent_define_ensure ((yyvsp[0].uniqstr), (yylsp[0]), true);
    }
/* Line 1380 of yacc.c  */
#line 1873 "../../src/parse-gram.c"
    break;

  case 8:
/* Line 1380 of yacc.c  */
#line 234 "parse-gram.y"
    {
      muscle_percent_define_insert ((yyvsp[-1].uniqstr), (yylsp[-1]), (yyvsp[0].chars));
    }
/* Line 1380 of yacc.c  */
#line 1883 "../../src/parse-gram.c"
    break;

  case 9:
/* Line 1380 of yacc.c  */
#line 237 "parse-gram.y"
    { defines_flag = true; }
/* Line 1380 of yacc.c  */
#line 1891 "../../src/parse-gram.c"
    break;

  case 10:
/* Line 1380 of yacc.c  */
#line 239 "parse-gram.y"
    {
      defines_flag = true;
      spec_defines_file = xstrdup ((yyvsp[0].chars));
    }
/* Line 1380 of yacc.c  */
#line 1902 "../../src/parse-gram.c"
    break;

  case 11:
/* Line 1380 of yacc.c  */
#line 243 "parse-gram.y"
    { expected_sr_conflicts = (yyvsp[0].integer); }
/* Line 1380 of yacc.c  */
#line 1910 "../../src/parse-gram.c"
    break;

  case 12:
/* Line 1380 of yacc.c  */
#line 244 "parse-gram.y"
    { expected_rr_conflicts = (yyvsp[0].integer); }
/* Line 1380 of yacc.c  */
#line 1918 "../../src/parse-gram.c"
    break;

  case 13:
/* Line 1380 of yacc.c  */
#line 245 "parse-gram.y"
    { spec_file_prefix = (yyvsp[0].chars); }
/* Line 1380 of yacc.c  */
#line 1926 "../../src/parse-gram.c"
    break;

  case 14:
/* Line 1380 of yacc.c  */
#line 246 "parse-gram.y"
    { spec_file_prefix = (yyvsp[0].chars); }
/* Line 1380 of yacc.c  */
#line 1934 "../../src/parse-gram.c"
    break;

  case 15:
/* Line 1380 of yacc.c  */
#line 248 "parse-gram.y"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
/* Line 1380 of yacc.c  */
#line 1945 "../../src/parse-gram.c"
    break;

  case 16:
/* Line 1380 of yacc.c  */
#line 253 "parse-gram.y"
    {
      code_props action;
      code_props_symbol_action_init (&action, (yyvsp[0].code), (yylsp[0]));
      code_props_translate_code (&action);
      gram_scanner_last_string_free ();
      muscle_code_grow ("initial_action", action.code, (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1380 of yacc.c  */
#line 1960 "../../src/parse-gram.c"
    break;

  case 17:
/* Line 1380 of yacc.c  */
#line 261 "parse-gram.y"
    { language_argmatch ((yyvsp[0].chars), grammar_prio, (yylsp[-1])); }
/* Line 1380 of yacc.c  */
#line 1968 "../../src/parse-gram.c"
    break;

  case 18:
/* Line 1380 of yacc.c  */
#line 262 "parse-gram.y"
    { add_param ("lex_param", (yyvsp[0].code), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 1976 "../../src/parse-gram.c"
    break;

  case 19:
/* Line 1380 of yacc.c  */
#line 263 "parse-gram.y"
    { locations_flag = true; }
/* Line 1380 of yacc.c  */
#line 1984 "../../src/parse-gram.c"
    break;

  case 20:
/* Line 1380 of yacc.c  */
#line 264 "parse-gram.y"
    { spec_name_prefix = (yyvsp[0].chars); }
/* Line 1380 of yacc.c  */
#line 1992 "../../src/parse-gram.c"
    break;

  case 21:
/* Line 1380 of yacc.c  */
#line 265 "parse-gram.y"
    { spec_name_prefix = (yyvsp[0].chars); }
/* Line 1380 of yacc.c  */
#line 2000 "../../src/parse-gram.c"
    break;

  case 22:
/* Line 1380 of yacc.c  */
#line 266 "parse-gram.y"
    { no_lines_flag = true; }
/* Line 1380 of yacc.c  */
#line 2008 "../../src/parse-gram.c"
    break;

  case 23:
/* Line 1380 of yacc.c  */
#line 267 "parse-gram.y"
    { nondeterministic_parser = true; }
/* Line 1380 of yacc.c  */
#line 2016 "../../src/parse-gram.c"
    break;

  case 24:
/* Line 1380 of yacc.c  */
#line 268 "parse-gram.y"
    { spec_outfile = (yyvsp[0].chars); }
/* Line 1380 of yacc.c  */
#line 2024 "../../src/parse-gram.c"
    break;

  case 25:
/* Line 1380 of yacc.c  */
#line 269 "parse-gram.y"
    { spec_outfile = (yyvsp[0].chars); }
/* Line 1380 of yacc.c  */
#line 2032 "../../src/parse-gram.c"
    break;

  case 26:
/* Line 1380 of yacc.c  */
#line 270 "parse-gram.y"
    { add_param ("parse_param", (yyvsp[0].code), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2040 "../../src/parse-gram.c"
    break;

  case 27:
/* Line 1380 of yacc.c  */
#line 271 "parse-gram.y"
    { version_check (&(yylsp[0]), (yyvsp[0].chars)); }
/* Line 1380 of yacc.c  */
#line 2048 "../../src/parse-gram.c"
    break;

  case 28:
/* Line 1380 of yacc.c  */
#line 273 "parse-gram.y"
    {
      char const *skeleton_user = (yyvsp[0].chars);
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
      skeleton_arg (skeleton_user, grammar_prio, (yylsp[-1]));
    }
/* Line 1380 of yacc.c  */
#line 2078 "../../src/parse-gram.c"
    break;

  case 29:
/* Line 1380 of yacc.c  */
#line 296 "parse-gram.y"
    { token_table_flag = true; }
/* Line 1380 of yacc.c  */
#line 2086 "../../src/parse-gram.c"
    break;

  case 30:
/* Line 1380 of yacc.c  */
#line 297 "parse-gram.y"
    { report_flag |= report_states; }
/* Line 1380 of yacc.c  */
#line 2094 "../../src/parse-gram.c"
    break;

  case 31:
/* Line 1380 of yacc.c  */
#line 298 "parse-gram.y"
    { yacc_flag = true; }
/* Line 1380 of yacc.c  */
#line 2102 "../../src/parse-gram.c"
    break;

  case 35:
/* Line 1380 of yacc.c  */
#line 306 "parse-gram.y"
    {
      grammar_start_symbol_set ((yyvsp[0].symbol), (yylsp[0]));
    }
/* Line 1380 of yacc.c  */
#line 2112 "../../src/parse-gram.c"
    break;

  case 36:
/* Line 1380 of yacc.c  */
#line 310 "parse-gram.y"
    {
      symbol_list *list;
      for (list = (yyvsp[0].list); list; list = list->next)
	symbol_list_destructor_set (list, (yyvsp[-1].code), (yylsp[-1]));
      symbol_list_free ((yyvsp[0].list));
    }
/* Line 1380 of yacc.c  */
#line 2125 "../../src/parse-gram.c"
    break;

  case 37:
/* Line 1380 of yacc.c  */
#line 317 "parse-gram.y"
    {
      symbol_list *list;
      for (list = (yyvsp[0].list); list; list = list->next)
	symbol_list_printer_set (list, (yyvsp[-1].code), (yylsp[-1]));
      symbol_list_free ((yyvsp[0].list));
    }
/* Line 1380 of yacc.c  */
#line 2138 "../../src/parse-gram.c"
    break;

  case 38:
/* Line 1380 of yacc.c  */
#line 324 "parse-gram.y"
    {
      default_prec = true;
    }
/* Line 1380 of yacc.c  */
#line 2148 "../../src/parse-gram.c"
    break;

  case 39:
/* Line 1380 of yacc.c  */
#line 328 "parse-gram.y"
    {
      default_prec = false;
    }
/* Line 1380 of yacc.c  */
#line 2158 "../../src/parse-gram.c"
    break;

  case 40:
/* Line 1380 of yacc.c  */
#line 332 "parse-gram.y"
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_code_grow ("percent_code()", (yyvsp[0].chars), (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1380 of yacc.c  */
#line 2171 "../../src/parse-gram.c"
    break;

  case 41:
/* Line 1380 of yacc.c  */
#line 339 "parse-gram.y"
    {
      muscle_percent_code_grow ((yyvsp[-1].uniqstr), (yylsp[-1]), (yyvsp[0].chars), (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1380 of yacc.c  */
#line 2182 "../../src/parse-gram.c"
    break;

  case 42:
/* Line 1380 of yacc.c  */
#line 353 "parse-gram.y"
    {}
/* Line 1380 of yacc.c  */
#line 2190 "../../src/parse-gram.c"
    break;

  case 43:
/* Line 1380 of yacc.c  */
#line 354 "parse-gram.y"
    { muscle_code_grow ("union_name", (yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2198 "../../src/parse-gram.c"
    break;

  case 44:
/* Line 1380 of yacc.c  */
#line 359 "parse-gram.y"
    {
      union_seen = true;
      muscle_code_grow ("stype", (yyvsp[0].chars), (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1380 of yacc.c  */
#line 2210 "../../src/parse-gram.c"
    break;

  case 45:
/* Line 1380 of yacc.c  */
#line 370 "parse-gram.y"
    { current_class = nterm_sym; }
/* Line 1380 of yacc.c  */
#line 2218 "../../src/parse-gram.c"
    break;

  case 46:
/* Line 1380 of yacc.c  */
#line 371 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
/* Line 1380 of yacc.c  */
#line 2229 "../../src/parse-gram.c"
    break;

  case 47:
/* Line 1380 of yacc.c  */
#line 375 "parse-gram.y"
    { current_class = token_sym; }
/* Line 1380 of yacc.c  */
#line 2237 "../../src/parse-gram.c"
    break;

  case 48:
/* Line 1380 of yacc.c  */
#line 376 "parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
/* Line 1380 of yacc.c  */
#line 2248 "../../src/parse-gram.c"
    break;

  case 49:
/* Line 1380 of yacc.c  */
#line 381 "parse-gram.y"
    {
      symbol_list *list;
      tag_seen = true;
      for (list = (yyvsp[0].list); list; list = list->next)
	symbol_type_set (list->content.sym, (yyvsp[-1].uniqstr), (yylsp[-1]));
      symbol_list_free ((yyvsp[0].list));
    }
/* Line 1380 of yacc.c  */
#line 2262 "../../src/parse-gram.c"
    break;

  case 50:
/* Line 1380 of yacc.c  */
#line 392 "parse-gram.y"
    {
      symbol_list *list;
      ++current_prec;
      for (list = (yyvsp[0].list); list; list = list->next)
	{
	  symbol_type_set (list->content.sym, current_type, (yylsp[-1]));
	  symbol_precedence_set (list->content.sym, current_prec, (yyvsp[-2].assoc), (yylsp[-2]));
	}
      symbol_list_free ((yyvsp[0].list));
      current_type = NULL;
    }
/* Line 1380 of yacc.c  */
#line 2280 "../../src/parse-gram.c"
    break;

  case 51:
/* Line 1380 of yacc.c  */
#line 406 "parse-gram.y"
    { (yyval.assoc) = left_assoc; }
/* Line 1380 of yacc.c  */
#line 2288 "../../src/parse-gram.c"
    break;

  case 52:
/* Line 1380 of yacc.c  */
#line 407 "parse-gram.y"
    { (yyval.assoc) = right_assoc; }
/* Line 1380 of yacc.c  */
#line 2296 "../../src/parse-gram.c"
    break;

  case 53:
/* Line 1380 of yacc.c  */
#line 408 "parse-gram.y"
    { (yyval.assoc) = non_assoc; }
/* Line 1380 of yacc.c  */
#line 2304 "../../src/parse-gram.c"
    break;

  case 54:
/* Line 1380 of yacc.c  */
#line 409 "parse-gram.y"
    { (yyval.assoc) = precedence_assoc; }
/* Line 1380 of yacc.c  */
#line 2312 "../../src/parse-gram.c"
    break;

  case 55:
/* Line 1380 of yacc.c  */
#line 413 "parse-gram.y"
    { current_type = NULL; }
/* Line 1380 of yacc.c  */
#line 2320 "../../src/parse-gram.c"
    break;

  case 56:
/* Line 1380 of yacc.c  */
#line 414 "parse-gram.y"
    { current_type = (yyvsp[0].uniqstr); tag_seen = true; }
/* Line 1380 of yacc.c  */
#line 2328 "../../src/parse-gram.c"
    break;

  case 57:
/* Line 1380 of yacc.c  */
#line 420 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2336 "../../src/parse-gram.c"
    break;

  case 58:
/* Line 1380 of yacc.c  */
#line 422 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[-1].list), symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0]))); }
/* Line 1380 of yacc.c  */
#line 2344 "../../src/parse-gram.c"
    break;

  case 59:
/* Line 1380 of yacc.c  */
#line 426 "parse-gram.y"
    { (yyval.symbol) = (yyvsp[0].symbol); }
/* Line 1380 of yacc.c  */
#line 2352 "../../src/parse-gram.c"
    break;

  case 60:
/* Line 1380 of yacc.c  */
#line 427 "parse-gram.y"
    { (yyval.symbol) = (yyvsp[-1].symbol); symbol_user_token_number_set ((yyvsp[-1].symbol), (yyvsp[0].integer), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2360 "../../src/parse-gram.c"
    break;

  case 61:
/* Line 1380 of yacc.c  */
#line 433 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2368 "../../src/parse-gram.c"
    break;

  case 62:
/* Line 1380 of yacc.c  */
#line 435 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[-1].list), symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0]))); }
/* Line 1380 of yacc.c  */
#line 2376 "../../src/parse-gram.c"
    break;

  case 63:
/* Line 1380 of yacc.c  */
#line 439 "parse-gram.y"
    { (yyval.list) = (yyvsp[0].list); }
/* Line 1380 of yacc.c  */
#line 2384 "../../src/parse-gram.c"
    break;

  case 64:
/* Line 1380 of yacc.c  */
#line 440 "parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[-1].list), (yyvsp[0].list)); }
/* Line 1380 of yacc.c  */
#line 2392 "../../src/parse-gram.c"
    break;

  case 65:
/* Line 1380 of yacc.c  */
#line 444 "parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2400 "../../src/parse-gram.c"
    break;

  case 66:
/* Line 1380 of yacc.c  */
#line 445 "parse-gram.y"
    { (yyval.list) = symbol_list_type_new ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2408 "../../src/parse-gram.c"
    break;

  case 67:
/* Line 1380 of yacc.c  */
#line 446 "parse-gram.y"
    { (yyval.list) = symbol_list_default_tagged_new ((yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2416 "../../src/parse-gram.c"
    break;

  case 68:
/* Line 1380 of yacc.c  */
#line 447 "parse-gram.y"
    { (yyval.list) = symbol_list_default_tagless_new ((yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2424 "../../src/parse-gram.c"
    break;

  case 69:
/* Line 1380 of yacc.c  */
#line 453 "parse-gram.y"
    {
       current_type = (yyvsp[0].uniqstr);
       tag_seen = true;
     }
/* Line 1380 of yacc.c  */
#line 2435 "../../src/parse-gram.c"
    break;

  case 70:
/* Line 1380 of yacc.c  */
#line 458 "parse-gram.y"
    {
       symbol_class_set ((yyvsp[0].symbol), current_class, (yylsp[0]), true);
       symbol_type_set ((yyvsp[0].symbol), current_type, (yylsp[0]));
     }
/* Line 1380 of yacc.c  */
#line 2446 "../../src/parse-gram.c"
    break;

  case 71:
/* Line 1380 of yacc.c  */
#line 463 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[-1].symbol), current_class, (yylsp[-1]), true);
      symbol_type_set ((yyvsp[-1].symbol), current_type, (yylsp[-1]));
      symbol_user_token_number_set ((yyvsp[-1].symbol), (yyvsp[0].integer), (yylsp[0]));
    }
/* Line 1380 of yacc.c  */
#line 2458 "../../src/parse-gram.c"
    break;

  case 72:
/* Line 1380 of yacc.c  */
#line 469 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[-1].symbol), current_class, (yylsp[-1]), true);
      symbol_type_set ((yyvsp[-1].symbol), current_type, (yylsp[-1]));
      symbol_make_alias ((yyvsp[-1].symbol), (yyvsp[0].symbol), (yyloc));
    }
/* Line 1380 of yacc.c  */
#line 2470 "../../src/parse-gram.c"
    break;

  case 73:
/* Line 1380 of yacc.c  */
#line 475 "parse-gram.y"
    {
      symbol_class_set ((yyvsp[-2].symbol), current_class, (yylsp[-2]), true);
      symbol_type_set ((yyvsp[-2].symbol), current_type, (yylsp[-2]));
      symbol_user_token_number_set ((yyvsp[-2].symbol), (yyvsp[-1].integer), (yylsp[-1]));
      symbol_make_alias ((yyvsp[-2].symbol), (yyvsp[0].symbol), (yyloc));
    }
/* Line 1380 of yacc.c  */
#line 2483 "../../src/parse-gram.c"
    break;

  case 80:
/* Line 1380 of yacc.c  */
#line 505 "parse-gram.y"
    {
      yyerrok;
    }
/* Line 1380 of yacc.c  */
#line 2493 "../../src/parse-gram.c"
    break;

  case 81:
/* Line 1380 of yacc.c  */
#line 511 "parse-gram.y"
    { current_lhs = (yyvsp[0].symbol); current_lhs_location = (yylsp[0]); }
/* Line 1380 of yacc.c  */
#line 2501 "../../src/parse-gram.c"
    break;

  case 83:
/* Line 1380 of yacc.c  */
#line 515 "parse-gram.y"
    { grammar_current_rule_end ((yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2509 "../../src/parse-gram.c"
    break;

  case 84:
/* Line 1380 of yacc.c  */
#line 516 "parse-gram.y"
    { grammar_current_rule_end ((yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2517 "../../src/parse-gram.c"
    break;

  case 86:
/* Line 1380 of yacc.c  */
#line 522 "parse-gram.y"
    { grammar_current_rule_begin (current_lhs, current_lhs_location); }
/* Line 1380 of yacc.c  */
#line 2525 "../../src/parse-gram.c"
    break;

  case 87:
/* Line 1380 of yacc.c  */
#line 524 "parse-gram.y"
    { grammar_current_rule_symbol_append ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2533 "../../src/parse-gram.c"
    break;

  case 88:
/* Line 1380 of yacc.c  */
#line 526 "parse-gram.y"
    { grammar_current_rule_action_append ((yyvsp[0].code), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2541 "../../src/parse-gram.c"
    break;

  case 89:
/* Line 1380 of yacc.c  */
#line 528 "parse-gram.y"
    { grammar_current_rule_prec_set ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2549 "../../src/parse-gram.c"
    break;

  case 90:
/* Line 1380 of yacc.c  */
#line 530 "parse-gram.y"
    { grammar_current_rule_dprec_set ((yyvsp[0].integer), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2557 "../../src/parse-gram.c"
    break;

  case 91:
/* Line 1380 of yacc.c  */
#line 532 "parse-gram.y"
    { grammar_current_rule_merge_set ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2565 "../../src/parse-gram.c"
    break;

  case 93:
/* Line 1380 of yacc.c  */
#line 542 "parse-gram.y"
    { (yyval.uniqstr) = uniqstr_new ((yyvsp[0].chars)); }
/* Line 1380 of yacc.c  */
#line 2573 "../../src/parse-gram.c"
    break;

  case 94:
/* Line 1380 of yacc.c  */
#line 548 "parse-gram.y"
    {
      (yyval.chars) = "";
    }
/* Line 1380 of yacc.c  */
#line 2583 "../../src/parse-gram.c"
    break;

  case 96:
/* Line 1380 of yacc.c  */
#line 561 "parse-gram.y"
    {
      code_props plain_code;
      (yyvsp[0].code)[strlen ((yyvsp[0].code)) - 1] = '\n';
      code_props_plain_init (&plain_code, (yyvsp[0].code)+1, (yylsp[0]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      (yyval.chars) = plain_code.code;
    }
/* Line 1380 of yacc.c  */
#line 2598 "../../src/parse-gram.c"
    break;

  case 97:
/* Line 1380 of yacc.c  */
#line 581 "parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2606 "../../src/parse-gram.c"
    break;

  case 98:
/* Line 1380 of yacc.c  */
#line 583 "parse-gram.y"
    {
      (yyval.symbol) = symbol_get (char_name ((yyvsp[0].character)), (yylsp[0]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[0]), false);
      symbol_user_token_number_set ((yyval.symbol), (yyvsp[0].character), (yylsp[0]));
    }
/* Line 1380 of yacc.c  */
#line 2618 "../../src/parse-gram.c"
    break;

  case 99:
/* Line 1380 of yacc.c  */
#line 591 "parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1380 of yacc.c  */
#line 2626 "../../src/parse-gram.c"
    break;

  case 102:
/* Line 1380 of yacc.c  */
#line 603 "parse-gram.y"
    {
      (yyval.symbol) = symbol_get (quotearg_style (c_quoting_style, (yyvsp[0].chars)), (yylsp[0]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[0]), false);
    }
/* Line 1380 of yacc.c  */
#line 2637 "../../src/parse-gram.c"
    break;

  case 104:
/* Line 1380 of yacc.c  */
#line 612 "parse-gram.y"
    {
      code_props plain_code;
      code_props_plain_init (&plain_code, (yyvsp[0].chars), (yylsp[0]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      muscle_code_grow ("epilogue", plain_code.code, (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1380 of yacc.c  */
#line 2652 "../../src/parse-gram.c"
    break;


/* Line 1380 of yacc.c  */
#line 2657 "../../src/parse-gram.c"
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
	YYSIZE_T yysize = yysyntax_error (0, yystate, yytoken);
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
	    (void) yysyntax_error (yymsg, yystate, yytoken);
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

/* Line 1599 of yacc.c  */
#line 622 "parse-gram.y"



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
