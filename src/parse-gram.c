/* A Bison parser, made by GNU Bison 2.4.483-4ad39-dirty.  */

/* Implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2010 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "2.4.483-4ad39-dirty"

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
/* Line 252 of yacc.c  */
#line 1 "src/parse-gram.y"
/* Bison Grammar Parser                             -*- C -*-

   Copyright (C) 2002-2010 Free Software Foundation, Inc.

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
#include "muscle-tab.h"
#include "named-ref.h"
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

/* Line 252 of yacc.c  */
#line 131 "src/parse-gram.c"

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

/* "%code requires" blocks.  */
/* Line 272 of yacc.c  */
#line 201 "src/parse-gram.y"

# ifndef PARAM_TYPE
#  define PARAM_TYPE
  typedef enum
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;
# endif


/* Line 272 of yacc.c  */
#line 168 "src/parse-gram.c"

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
     PERCENT_NAME_PREFIX = 284,
     PERCENT_NO_DEFAULT_PREC = 285,
     PERCENT_NO_LINES = 286,
     PERCENT_NONDETERMINISTIC_PARSER = 287,
     PERCENT_OUTPUT = 288,
     PERCENT_REQUIRE = 289,
     PERCENT_SKELETON = 290,
     PERCENT_START = 291,
     PERCENT_TOKEN_TABLE = 292,
     PERCENT_VERBOSE = 293,
     PERCENT_YACC = 294,
     BRACED_CODE = 295,
     BRACKETED_ID = 296,
     CHAR = 297,
     EPILOGUE = 298,
     EQUAL = 299,
     ID = 300,
     ID_COLON = 301,
     PERCENT_PERCENT = 302,
     PIPE = 303,
     PROLOGUE = 304,
     SEMICOLON = 305,
     TAG = 306,
     TAG_ANY = 307,
     TAG_NONE = 308,
     PERCENT_PARAM = 309,
     PERCENT_UNION = 310
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
#define PERCENT_NAME_PREFIX 284
#define PERCENT_NO_DEFAULT_PREC 285
#define PERCENT_NO_LINES 286
#define PERCENT_NONDETERMINISTIC_PARSER 287
#define PERCENT_OUTPUT 288
#define PERCENT_REQUIRE 289
#define PERCENT_SKELETON 290
#define PERCENT_START 291
#define PERCENT_TOKEN_TABLE 292
#define PERCENT_VERBOSE 293
#define PERCENT_YACC 294
#define BRACED_CODE 295
#define BRACKETED_ID 296
#define CHAR 297
#define EPILOGUE 298
#define EQUAL 299
#define ID 300
#define ID_COLON 301
#define PERCENT_PERCENT 302
#define PIPE 303
#define PROLOGUE 304
#define SEMICOLON 305
#define TAG 306
#define TAG_ANY 307
#define TAG_NONE 308
#define PERCENT_PARAM 309
#define PERCENT_UNION 310




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 277 of yacc.c  */
#line 87 "src/parse-gram.y"

  assoc assoc;
  char *code;
  char const *chars;
  int integer;
  named_ref *named_ref;
  symbol *symbol;
  symbol_list *list;
  uniqstr uniqstr;
  unsigned char character;

/* Line 277 of yacc.c  */
#line 225 "src/parse-gram.y"

  param_type param;


/* Line 277 of yacc.c  */
#line 314 "src/parse-gram.c"
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

/* Line 327 of yacc.c  */
#line 338 "src/parse-gram.c"
/* Unqualified %code blocks.  */
/* Line 328 of yacc.c  */
#line 55 "src/parse-gram.y"

  static int current_prec = 0;
  static location current_lhs_location;
  static named_ref *current_lhs_named_ref;
  static symbol *current_lhs;
  static symbol_class current_class = unknown_sym;
  static uniqstr current_type = NULL;

  #define YYTYPE_INT16 int_fast16_t
  #define YYTYPE_INT8 int_fast8_t
  #define YYTYPE_UINT16 uint_fast16_t
  #define YYTYPE_UINT8 uint_fast8_t

/* Line 328 of yacc.c  */
#line 214 "src/parse-gram.y"

  /** Add a lex-param and/or a parse-param.
   *
   * \param type  where to push this formal argument.
   * \param decl  the formal argument.  Destroyed.
   * \param loc   the location in the source.
   */
  static void add_param (param_type type, char *decl, location loc);
  static param_type current_param = param_none;


/* Line 328 of yacc.c  */
#line 369 "src/parse-gram.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#define YYLAST   162

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  148

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

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
      55
};

#if YYDEBUG
  /* YYRLINEYYN -- Source line where rule number YYN was defined.    */
static const yytype_uint16 yyrline[] =
{
       0,   250,   250,   258,   260,   264,   265,   275,   279,   284,
     285,   290,   295,   296,   297,   298,   299,   304,   313,   314,
     315,   316,   317,   318,   319,   320,   320,   321,   322,   346,
     347,   348,   349,   353,   354,   363,   364,   365,   369,   376,
     383,   387,   391,   398,   413,   414,   418,   430,   430,   435,
     435,   440,   451,   466,   467,   468,   469,   473,   474,   479,
     481,   486,   487,   492,   494,   499,   500,   504,   505,   506,
     507,   512,   517,   522,   528,   534,   545,   546,   555,   556,
     562,   563,   564,   571,   571,   576,   577,   578,   583,   585,
     587,   589,   591,   593,   598,   600,   611,   612,   617,   618,
     619,   628,   648,   650,   659,   664,   665,   670,   677,   679
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
  "\"%name-prefix\"", "\"%no-default-prec\"", "\"%no-lines\"",
  "\"%nondeterministic-parser\"", "\"%output\"", "\"%require\"",
  "\"%skeleton\"", "\"%start\"", "\"%token-table\"", "\"%verbose\"",
  "\"%yacc\"", "\"{...}\"", "\"[identifier]\"", "\"char\"", "\"epilogue\"",
  "\"=\"", "\"identifier\"", "\"identifier:\"", "\"%%\"", "\"|\"",
  "\"%{...%}\"", "\";\"", "\"<tag>\"", "\"<*>\"", "\"<>\"", "\"%param\"",
  "\"%union\"", "$accept", "input", "prologue_declarations",
  "prologue_declaration", "$@1", "params", "grammar_declaration",
  "union_name", "symbol_declaration", "$@2", "$@3",
  "precedence_declaration", "precedence_declarator", "tag.opt",
  "symbols.prec", "symbol.prec", "symbols.1", "generic_symlist",
  "generic_symlist_item", "symbol_def", "symbol_defs.1", "grammar",
  "rules_or_grammar_declaration", "rules", "$@4", "rhses.1", "rhs",
  "named_ref.opt", "variable", "content.opt", "braceless", "id",
  "id_colon", "symbol", "string_as_id", "epilogue.opt", 0
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
     305,   306,   307,   308,   309,   310
};
# endif

#define YYPACT_NINF -137

#define yypact_value_is_default(yystate) \
  ((yystate) == (-137))

#define YYTABLE_NINF -109

#define yytable_value_is_error(yytable_value) \
  YYID (0)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.    */
static const yytype_int16 yypact[] =
{
    -137,    20,   106,  -137,  -137,  -137,    19,    -1,    36,  -137,
    -137,  -137,  -137,    13,  -137,     0,    79,  -137,    89,    90,
    -137,    41,  -137,    55,    93,    46,  -137,  -137,  -137,    47,
      94,    95,    35,  -137,  -137,  -137,    16,  -137,  -137,  -137,
      54,  -137,  -137,  -137,  -137,    49,    33,    33,    35,    12,
      12,  -137,    61,  -137,  -137,  -137,    29,  -137,  -137,  -137,
    -137,   100,  -137,  -137,  -137,   101,  -137,   103,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,    57,  -137,    58,     1,
    -137,  -137,    68,    80,  -137,    61,  -137,    35,  -137,  -137,
      33,    56,    33,    35,  -137,  -137,  -137,  -137,    12,  -137,
    -137,    12,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
    -137,    78,  -137,  -137,  -137,  -137,  -137,    82,  -137,    35,
    -137,   142,  -137,   144,  -137,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,    18,    27,  -137,  -137,    35,   145,    97,
      68,    68,    27,  -137,  -137,  -137,  -137,  -137
};

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE does not specify something else to do.  Zero means the default
     is an error.    */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,    49,    47,     0,     0,     0,    53,
      54,    55,    56,     0,    40,     0,     9,    11,     0,     0,
       7,     0,    16,     0,     0,     0,    41,    21,    22,     0,
       0,     0,     0,    29,    30,    31,     0,     6,    32,    25,
      44,     4,     5,    36,    35,    57,     0,     0,     0,     0,
       0,   101,     0,    42,    97,    96,    98,    10,    12,    13,
      14,     0,    17,    18,    19,     0,    23,     0,    27,    28,
     107,   103,   102,   105,    37,   106,     0,   104,     0,     0,
      78,    80,    94,     0,    45,     0,    58,     0,    71,    76,
      50,    72,    48,    51,    63,    68,    69,    70,    38,    65,
      67,    39,    43,   100,    99,     8,    15,    20,    24,    82,
      81,     0,    79,     2,    95,    83,    34,    26,    46,    52,
      59,    61,    77,    73,    74,    64,    66,   109,    88,    33,
      60,    62,    75,    84,    85,    88,    87,     0,     0,     0,
      94,    94,    86,    91,    92,    93,    90,    89
};

  /* YYPGOTO[NTERM-NUM].    */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,  -137,  -137,  -137,  -137,   148,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,    32,  -137,   102,    -9,    -4,
     107,  -137,    83,  -137,  -137,  -137,    22,  -136,  -137,  -137,
     -12,   -11,  -137,   -32,   -40,  -137
};

  /* YYDEFGOTO[NTERM-NUM].    */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    41,    83,   117,    78,    85,    43,    47,
      46,    44,    45,    87,   119,   120,    93,    98,    99,    89,
      90,    79,    80,    81,   128,   133,   134,   115,    56,   105,
      53,    73,    82,   100,    75,   113
};

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF, syntax error.    */
static const yytype_int16 yytable[] =
{
      74,  -108,    76,    54,   146,   147,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    70,    94,    76,    13,    14,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      70,    26,   103,    13,    14,    91,    91,    32,    70,    49,
     102,   137,   138,   139,    60,    55,    26,    77,   111,    64,
      66,   124,    32,    51,    71,   121,    40,    72,    52,    70,
     123,   125,    77,    95,    96,    97,   135,   140,   136,    71,
      48,    40,    72,   118,   104,    71,    50,    71,    72,    91,
      72,    91,    57,   132,    88,    61,   122,   121,   122,   126,
      65,    67,   126,    58,    59,    62,    63,    68,    69,    84,
      86,    51,   141,   106,   107,   143,   108,   109,   110,   114,
     141,     4,     5,     6,     7,     8,     9,    10,    11,    12,
     116,   127,   129,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,   131,    70,   145,   144,
      42,   130,   101,    36,    92,    37,    38,   142,     0,     0,
      39,    40,   112
};

static const yytype_int16 yycheck[] =
{
      32,     0,     1,     3,   140,   141,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     3,    48,     1,    17,    18,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       3,    30,     3,    17,    18,    46,    47,    36,     3,    40,
      52,    14,    15,    16,     3,    45,    30,    46,    47,     3,
       3,    91,    36,    40,    42,    87,    55,    45,    45,     3,
       4,    93,    46,    51,    52,    53,    48,    40,    50,    42,
      51,    55,    45,    85,    45,    42,    40,    42,    45,    90,
      45,    92,     3,   123,    51,    44,    90,   119,    92,    98,
      44,    44,   101,     4,     4,    40,     3,     3,     3,    45,
      51,    40,   134,     3,     3,   137,     3,    50,    50,    41,
     142,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      40,    43,    40,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     4,     3,    51,     4,
       2,   119,    50,    47,    47,    49,    50,   135,    -1,    -1,
      54,    55,    79
};

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.    */
static const yytype_uint8 yystos[] =
{
       0,    57,    58,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    47,    49,    50,    54,
      55,    59,    62,    64,    67,    68,    66,    65,    51,    40,
      40,    40,    45,    86,     3,    45,    84,     3,     4,     4,
       3,    44,    40,     3,     3,    44,     3,    44,     3,     3,
       3,    42,    45,    87,    89,    90,     1,    46,    62,    77,
      78,    79,    88,    60,    45,    63,    51,    69,    51,    75,
      76,    87,    76,    72,    89,    51,    52,    53,    73,    74,
      89,    73,    86,     3,    45,    85,     3,     3,     3,    50,
      50,    47,    78,    91,    41,    83,    40,    61,    86,    70,
      71,    89,    75,     4,    90,    89,    74,    43,    80,    40,
      71,     4,    90,    81,    82,    48,    50,    14,    15,    16,
      40,    89,    82,    89,     4,    51,    83,    83
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.    */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    60,    59,    59,    59,    59,
      59,    59,    59,    61,    61,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    63,    62,    65,    64,    66,
      64,    64,    67,    68,    68,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    73,    73,    74,    74,    74,
      74,    75,    75,    75,    75,    75,    76,    76,    77,    77,
      78,    78,    78,    80,    79,    81,    81,    81,    82,    82,
      82,    82,    82,    82,    83,    83,    84,    84,    85,    85,
      85,    86,    87,    87,    88,    89,    89,    90,    91,    91
};

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.    */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     2,     1,     1,     1,     3,     1,
       2,     1,     2,     2,     2,     3,     1,     2,     2,     2,
       3,     1,     1,     2,     3,     0,     3,     2,     2,     1,
       1,     1,     1,     2,     1,     1,     1,     2,     3,     3,
       1,     1,     2,     3,     0,     1,     3,     0,     3,     0,
       3,     3,     3,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     1,     2,     1,     2,
       1,     2,     2,     0,     4,     1,     3,     2,     0,     3,
       3,     3,     3,     3,     0,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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

/* Line 729 of yacc.c  */
#line 174 "src/parse-gram.y"
        { fputs (quotearg_style (c_quoting_style, ((*yyvaluep).chars)), stderr); }
/* Line 729 of yacc.c  */
#line 1034 "src/parse-gram.c"
        break;

            case 4: // "integer"

/* Line 729 of yacc.c  */
#line 187 "src/parse-gram.y"
        { fprintf (stderr, "%d", ((*yyvaluep).integer)); }
/* Line 729 of yacc.c  */
#line 1043 "src/parse-gram.c"
        break;

            case 24: // "%<flag>"

/* Line 729 of yacc.c  */
#line 183 "src/parse-gram.y"
        { fprintf (stderr, "%%%s", ((*yyvaluep).uniqstr)); }
/* Line 729 of yacc.c  */
#line 1052 "src/parse-gram.c"
        break;

            case 40: // "{...}"

/* Line 729 of yacc.c  */
#line 176 "src/parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).code)); }
/* Line 729 of yacc.c  */
#line 1061 "src/parse-gram.c"
        break;

            case 41: // "[identifier]"

/* Line 729 of yacc.c  */
#line 181 "src/parse-gram.y"
        { fprintf (stderr, "[%s]", ((*yyvaluep).uniqstr)); }
/* Line 729 of yacc.c  */
#line 1070 "src/parse-gram.c"
        break;

            case 42: // "char"

/* Line 729 of yacc.c  */
#line 168 "src/parse-gram.y"
        { fputs (char_name (((*yyvaluep).character)), stderr); }
/* Line 729 of yacc.c  */
#line 1079 "src/parse-gram.c"
        break;

            case 43: // "epilogue"

/* Line 729 of yacc.c  */
#line 176 "src/parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 729 of yacc.c  */
#line 1088 "src/parse-gram.c"
        break;

            case 45: // "identifier"

/* Line 729 of yacc.c  */
#line 180 "src/parse-gram.y"
        { fputs (((*yyvaluep).uniqstr), stderr); }
/* Line 729 of yacc.c  */
#line 1097 "src/parse-gram.c"
        break;

            case 46: // "identifier:"

/* Line 729 of yacc.c  */
#line 182 "src/parse-gram.y"
        { fprintf (stderr, "%s:", ((*yyvaluep).uniqstr)); }
/* Line 729 of yacc.c  */
#line 1106 "src/parse-gram.c"
        break;

            case 49: // "%{...%}"

/* Line 729 of yacc.c  */
#line 176 "src/parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 729 of yacc.c  */
#line 1115 "src/parse-gram.c"
        break;

            case 51: // "<tag>"

/* Line 729 of yacc.c  */
#line 184 "src/parse-gram.y"
        { fprintf (stderr, "<%s>", ((*yyvaluep).uniqstr)); }
/* Line 729 of yacc.c  */
#line 1124 "src/parse-gram.c"
        break;

            case 54: // "%param"

/* Line 729 of yacc.c  */
#line 230 "src/parse-gram.y"
        {
  switch (((*yyvaluep).param))
    {
#define CASE(In, Out)                                           \
      case param_ ## In: fputs ("%" #Out, stderr); break
      CASE(lex,   lex-param);
      CASE(parse, parse-param);
      CASE(both,  param);
#undef CASE
      case param_none: aver (false); break;
    }
}
/* Line 729 of yacc.c  */
#line 1144 "src/parse-gram.c"
        break;

            case 71: // symbol.prec

/* Line 729 of yacc.c  */
#line 190 "src/parse-gram.y"
        { fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); }
/* Line 729 of yacc.c  */
#line 1153 "src/parse-gram.c"
        break;

            case 84: // variable

/* Line 729 of yacc.c  */
#line 180 "src/parse-gram.y"
        { fputs (((*yyvaluep).uniqstr), stderr); }
/* Line 729 of yacc.c  */
#line 1162 "src/parse-gram.c"
        break;

            case 85: // content.opt

/* Line 729 of yacc.c  */
#line 176 "src/parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 729 of yacc.c  */
#line 1171 "src/parse-gram.c"
        break;

            case 86: // braceless

/* Line 729 of yacc.c  */
#line 176 "src/parse-gram.y"
        { fprintf (stderr, "{\n%s\n}", ((*yyvaluep).chars)); }
/* Line 729 of yacc.c  */
#line 1180 "src/parse-gram.c"
        break;

            case 87: // id

/* Line 729 of yacc.c  */
#line 190 "src/parse-gram.y"
        { fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); }
/* Line 729 of yacc.c  */
#line 1189 "src/parse-gram.c"
        break;

            case 88: // id_colon

/* Line 729 of yacc.c  */
#line 191 "src/parse-gram.y"
        { fprintf (stderr, "%s:", ((*yyvaluep).symbol)->tag); }
/* Line 729 of yacc.c  */
#line 1198 "src/parse-gram.c"
        break;

            case 89: // symbol

/* Line 729 of yacc.c  */
#line 190 "src/parse-gram.y"
        { fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); }
/* Line 729 of yacc.c  */
#line 1207 "src/parse-gram.c"
        break;

            case 90: // string_as_id

/* Line 729 of yacc.c  */
#line 190 "src/parse-gram.y"
        { fprintf (stderr, "%s", ((*yyvaluep).symbol)->tag); }
/* Line 729 of yacc.c  */
#line 1216 "src/parse-gram.c"
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN while in state YYSTATE.

   Return 0 if *YYMSG was successfully written.  Return 1 if an ordinary
   "syntax error" message will suffice instead.  Return 2 if *YYMSG is
   not large enough to hold the message.  In the last case, also set
   *YYMSG_ALLOC to either (a) the required number of bytes or (b) zero
   if the required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                int yystate, int yytoken)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 1;
  else
    {
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      /* Internationalized format string. */
      const char *yyformat = 0;
      /* Arguments of yyformat. */
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  In other words, skip the first -YYN actions for this
	 state because they are default actions.  */
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
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
	    && !yytable_value_is_error (yytable[yyx + yyn]))
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    if (! (yysize <= yysize1
		   && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
	      {
		/* Overflow.  */
		*yymsg_alloc = 0;
		return 2;
	      }
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
      if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
        {
          /* Overflow.  */
          *yymsg_alloc = 0;
          return 2;
        }
      yysize = yysize1;

      if (*yymsg_alloc < yysize)
        {
          *yymsg_alloc = 2 * yysize;
          if (! (yysize <= *yymsg_alloc
                 && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
            *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
          return 2;
        }

      /* Avoid sprintf, as that infringes on the user's name space.
         Don't have undefined behavior even if the translation
         produced a string with the wrong number of "%s"s.  */
      {
        char *yyp = *yymsg;
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
      return 0;
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
    YYLTYPE yyerror_range[3];

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

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

/* User initialization code.  */
/* Line 1214 of yacc.c  */
#line 79 "src/parse-gram.y"
{
  /* Bison's grammar can initial empty locations, hence a default
     location is needed. */
  boundary_set (&yylloc.start, current_file, 1, 1);
  boundary_set (&yylloc.end, current_file, 1, 1);
}
/* Line 1214 of yacc.c  */
#line 1755 "src/parse-gram.c"
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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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
/* Line 1427 of yacc.c  */
#line 266 "src/parse-gram.y"
    {
      code_props plain_code;
      code_props_plain_init (&plain_code, (yyvsp[0].chars), (yylsp[0]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      muscle_code_grow (union_seen ? "post_prologue" : "pre_prologue",
                        plain_code.code, (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1427 of yacc.c  */
#line 1953 "src/parse-gram.c"
    break;

  case 7:
/* Line 1427 of yacc.c  */
#line 276 "src/parse-gram.y"
    {
      muscle_percent_define_ensure ((yyvsp[0].uniqstr), (yylsp[0]), true);
    }
/* Line 1427 of yacc.c  */
#line 1963 "src/parse-gram.c"
    break;

  case 8:
/* Line 1427 of yacc.c  */
#line 280 "src/parse-gram.y"
    {
      muscle_percent_define_insert ((yyvsp[-1].uniqstr), (yylsp[-1]), (yyvsp[0].chars),
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
/* Line 1427 of yacc.c  */
#line 1974 "src/parse-gram.c"
    break;

  case 9:
/* Line 1427 of yacc.c  */
#line 284 "src/parse-gram.y"
    { defines_flag = true; }
/* Line 1427 of yacc.c  */
#line 1982 "src/parse-gram.c"
    break;

  case 10:
/* Line 1427 of yacc.c  */
#line 286 "src/parse-gram.y"
    {
      defines_flag = true;
      spec_defines_file = xstrdup ((yyvsp[0].chars));
    }
/* Line 1427 of yacc.c  */
#line 1993 "src/parse-gram.c"
    break;

  case 11:
/* Line 1427 of yacc.c  */
#line 291 "src/parse-gram.y"
    {
      muscle_percent_define_insert ("parse.error", (yylsp[0]), "verbose",
                                    MUSCLE_PERCENT_DEFINE_GRAMMAR_FILE);
    }
/* Line 1427 of yacc.c  */
#line 2004 "src/parse-gram.c"
    break;

  case 12:
/* Line 1427 of yacc.c  */
#line 295 "src/parse-gram.y"
    { expected_sr_conflicts = (yyvsp[0].integer); }
/* Line 1427 of yacc.c  */
#line 2012 "src/parse-gram.c"
    break;

  case 13:
/* Line 1427 of yacc.c  */
#line 296 "src/parse-gram.y"
    { expected_rr_conflicts = (yyvsp[0].integer); }
/* Line 1427 of yacc.c  */
#line 2020 "src/parse-gram.c"
    break;

  case 14:
/* Line 1427 of yacc.c  */
#line 297 "src/parse-gram.y"
    { spec_file_prefix = (yyvsp[0].chars); }
/* Line 1427 of yacc.c  */
#line 2028 "src/parse-gram.c"
    break;

  case 15:
/* Line 1427 of yacc.c  */
#line 298 "src/parse-gram.y"
    { spec_file_prefix = (yyvsp[0].chars); }
/* Line 1427 of yacc.c  */
#line 2036 "src/parse-gram.c"
    break;

  case 16:
/* Line 1427 of yacc.c  */
#line 300 "src/parse-gram.y"
    {
      nondeterministic_parser = true;
      glr_parser = true;
    }
/* Line 1427 of yacc.c  */
#line 2047 "src/parse-gram.c"
    break;

  case 17:
/* Line 1427 of yacc.c  */
#line 305 "src/parse-gram.y"
    {
      code_props action;
      code_props_symbol_action_init (&action, (yyvsp[0].code), (yylsp[0]));
      code_props_translate_code (&action);
      gram_scanner_last_string_free ();
      muscle_code_grow ("initial_action", action.code, (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1427 of yacc.c  */
#line 2062 "src/parse-gram.c"
    break;

  case 18:
/* Line 1427 of yacc.c  */
#line 313 "src/parse-gram.y"
    { language_argmatch ((yyvsp[0].chars), grammar_prio, (yylsp[-1])); }
/* Line 1427 of yacc.c  */
#line 2070 "src/parse-gram.c"
    break;

  case 19:
/* Line 1427 of yacc.c  */
#line 314 "src/parse-gram.y"
    { spec_name_prefix = (yyvsp[0].chars); }
/* Line 1427 of yacc.c  */
#line 2078 "src/parse-gram.c"
    break;

  case 20:
/* Line 1427 of yacc.c  */
#line 315 "src/parse-gram.y"
    { spec_name_prefix = (yyvsp[0].chars); }
/* Line 1427 of yacc.c  */
#line 2086 "src/parse-gram.c"
    break;

  case 21:
/* Line 1427 of yacc.c  */
#line 316 "src/parse-gram.y"
    { no_lines_flag = true; }
/* Line 1427 of yacc.c  */
#line 2094 "src/parse-gram.c"
    break;

  case 22:
/* Line 1427 of yacc.c  */
#line 317 "src/parse-gram.y"
    { nondeterministic_parser = true; }
/* Line 1427 of yacc.c  */
#line 2102 "src/parse-gram.c"
    break;

  case 23:
/* Line 1427 of yacc.c  */
#line 318 "src/parse-gram.y"
    { spec_outfile = (yyvsp[0].chars); }
/* Line 1427 of yacc.c  */
#line 2110 "src/parse-gram.c"
    break;

  case 24:
/* Line 1427 of yacc.c  */
#line 319 "src/parse-gram.y"
    { spec_outfile = (yyvsp[0].chars); }
/* Line 1427 of yacc.c  */
#line 2118 "src/parse-gram.c"
    break;

  case 25:
/* Line 1427 of yacc.c  */
#line 320 "src/parse-gram.y"
    { current_param = (yyvsp[0].param); }
/* Line 1427 of yacc.c  */
#line 2126 "src/parse-gram.c"
    break;

  case 26:
/* Line 1427 of yacc.c  */
#line 320 "src/parse-gram.y"
    { current_param = param_none; }
/* Line 1427 of yacc.c  */
#line 2134 "src/parse-gram.c"
    break;

  case 27:
/* Line 1427 of yacc.c  */
#line 321 "src/parse-gram.y"
    { version_check (&(yylsp[0]), (yyvsp[0].chars)); }
/* Line 1427 of yacc.c  */
#line 2142 "src/parse-gram.c"
    break;

  case 28:
/* Line 1427 of yacc.c  */
#line 323 "src/parse-gram.y"
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
/* Line 1427 of yacc.c  */
#line 2172 "src/parse-gram.c"
    break;

  case 29:
/* Line 1427 of yacc.c  */
#line 346 "src/parse-gram.y"
    { token_table_flag = true; }
/* Line 1427 of yacc.c  */
#line 2180 "src/parse-gram.c"
    break;

  case 30:
/* Line 1427 of yacc.c  */
#line 347 "src/parse-gram.y"
    { report_flag |= report_states; }
/* Line 1427 of yacc.c  */
#line 2188 "src/parse-gram.c"
    break;

  case 31:
/* Line 1427 of yacc.c  */
#line 348 "src/parse-gram.y"
    { yacc_flag = true; }
/* Line 1427 of yacc.c  */
#line 2196 "src/parse-gram.c"
    break;

  case 33:
/* Line 1427 of yacc.c  */
#line 353 "src/parse-gram.y"
    { add_param (current_param, (yyvsp[0].code), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2204 "src/parse-gram.c"
    break;

  case 34:
/* Line 1427 of yacc.c  */
#line 354 "src/parse-gram.y"
    { add_param (current_param, (yyvsp[0].code), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2212 "src/parse-gram.c"
    break;

  case 37:
/* Line 1427 of yacc.c  */
#line 366 "src/parse-gram.y"
    {
      grammar_start_symbol_set ((yyvsp[0].symbol), (yylsp[0]));
    }
/* Line 1427 of yacc.c  */
#line 2222 "src/parse-gram.c"
    break;

  case 38:
/* Line 1427 of yacc.c  */
#line 370 "src/parse-gram.y"
    {
      symbol_list *list;
      for (list = (yyvsp[0].list); list; list = list->next)
	symbol_list_destructor_set (list, (yyvsp[-1].code), (yylsp[-1]));
      symbol_list_free ((yyvsp[0].list));
    }
/* Line 1427 of yacc.c  */
#line 2235 "src/parse-gram.c"
    break;

  case 39:
/* Line 1427 of yacc.c  */
#line 377 "src/parse-gram.y"
    {
      symbol_list *list;
      for (list = (yyvsp[0].list); list; list = list->next)
	symbol_list_printer_set (list, (yyvsp[-1].code), (yylsp[-1]));
      symbol_list_free ((yyvsp[0].list));
    }
/* Line 1427 of yacc.c  */
#line 2248 "src/parse-gram.c"
    break;

  case 40:
/* Line 1427 of yacc.c  */
#line 384 "src/parse-gram.y"
    {
      default_prec = true;
    }
/* Line 1427 of yacc.c  */
#line 2258 "src/parse-gram.c"
    break;

  case 41:
/* Line 1427 of yacc.c  */
#line 388 "src/parse-gram.y"
    {
      default_prec = false;
    }
/* Line 1427 of yacc.c  */
#line 2268 "src/parse-gram.c"
    break;

  case 42:
/* Line 1427 of yacc.c  */
#line 392 "src/parse-gram.y"
    {
      /* Do not invoke muscle_percent_code_grow here since it invokes
         muscle_user_name_list_grow.  */
      muscle_code_grow ("percent_code()", (yyvsp[0].chars), (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1427 of yacc.c  */
#line 2281 "src/parse-gram.c"
    break;

  case 43:
/* Line 1427 of yacc.c  */
#line 399 "src/parse-gram.y"
    {
      muscle_percent_code_grow ((yyvsp[-1].uniqstr), (yylsp[-1]), (yyvsp[0].chars), (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1427 of yacc.c  */
#line 2292 "src/parse-gram.c"
    break;

  case 44:
/* Line 1427 of yacc.c  */
#line 413 "src/parse-gram.y"
    {}
/* Line 1427 of yacc.c  */
#line 2300 "src/parse-gram.c"
    break;

  case 45:
/* Line 1427 of yacc.c  */
#line 414 "src/parse-gram.y"
    { muscle_code_grow ("union_name", (yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2308 "src/parse-gram.c"
    break;

  case 46:
/* Line 1427 of yacc.c  */
#line 419 "src/parse-gram.y"
    {
      union_seen = true;
      muscle_code_grow ("stype", (yyvsp[0].chars), (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1427 of yacc.c  */
#line 2320 "src/parse-gram.c"
    break;

  case 47:
/* Line 1427 of yacc.c  */
#line 430 "src/parse-gram.y"
    { current_class = nterm_sym; }
/* Line 1427 of yacc.c  */
#line 2328 "src/parse-gram.c"
    break;

  case 48:
/* Line 1427 of yacc.c  */
#line 431 "src/parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
/* Line 1427 of yacc.c  */
#line 2339 "src/parse-gram.c"
    break;

  case 49:
/* Line 1427 of yacc.c  */
#line 435 "src/parse-gram.y"
    { current_class = token_sym; }
/* Line 1427 of yacc.c  */
#line 2347 "src/parse-gram.c"
    break;

  case 50:
/* Line 1427 of yacc.c  */
#line 436 "src/parse-gram.y"
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
/* Line 1427 of yacc.c  */
#line 2358 "src/parse-gram.c"
    break;

  case 51:
/* Line 1427 of yacc.c  */
#line 441 "src/parse-gram.y"
    {
      symbol_list *list;
      tag_seen = true;
      for (list = (yyvsp[0].list); list; list = list->next)
	symbol_type_set (list->content.sym, (yyvsp[-1].uniqstr), (yylsp[-1]));
      symbol_list_free ((yyvsp[0].list));
    }
/* Line 1427 of yacc.c  */
#line 2372 "src/parse-gram.c"
    break;

  case 52:
/* Line 1427 of yacc.c  */
#line 452 "src/parse-gram.y"
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
/* Line 1427 of yacc.c  */
#line 2390 "src/parse-gram.c"
    break;

  case 53:
/* Line 1427 of yacc.c  */
#line 466 "src/parse-gram.y"
    { (yyval.assoc) = left_assoc; }
/* Line 1427 of yacc.c  */
#line 2398 "src/parse-gram.c"
    break;

  case 54:
/* Line 1427 of yacc.c  */
#line 467 "src/parse-gram.y"
    { (yyval.assoc) = right_assoc; }
/* Line 1427 of yacc.c  */
#line 2406 "src/parse-gram.c"
    break;

  case 55:
/* Line 1427 of yacc.c  */
#line 468 "src/parse-gram.y"
    { (yyval.assoc) = non_assoc; }
/* Line 1427 of yacc.c  */
#line 2414 "src/parse-gram.c"
    break;

  case 56:
/* Line 1427 of yacc.c  */
#line 469 "src/parse-gram.y"
    { (yyval.assoc) = precedence_assoc; }
/* Line 1427 of yacc.c  */
#line 2422 "src/parse-gram.c"
    break;

  case 57:
/* Line 1427 of yacc.c  */
#line 473 "src/parse-gram.y"
    { current_type = NULL; }
/* Line 1427 of yacc.c  */
#line 2430 "src/parse-gram.c"
    break;

  case 58:
/* Line 1427 of yacc.c  */
#line 474 "src/parse-gram.y"
    { current_type = (yyvsp[0].uniqstr); tag_seen = true; }
/* Line 1427 of yacc.c  */
#line 2438 "src/parse-gram.c"
    break;

  case 59:
/* Line 1427 of yacc.c  */
#line 480 "src/parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2446 "src/parse-gram.c"
    break;

  case 60:
/* Line 1427 of yacc.c  */
#line 482 "src/parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[-1].list), symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0]))); }
/* Line 1427 of yacc.c  */
#line 2454 "src/parse-gram.c"
    break;

  case 61:
/* Line 1427 of yacc.c  */
#line 486 "src/parse-gram.y"
    { (yyval.symbol) = (yyvsp[0].symbol); }
/* Line 1427 of yacc.c  */
#line 2462 "src/parse-gram.c"
    break;

  case 62:
/* Line 1427 of yacc.c  */
#line 487 "src/parse-gram.y"
    { (yyval.symbol) = (yyvsp[-1].symbol); symbol_user_token_number_set ((yyvsp[-1].symbol), (yyvsp[0].integer), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2470 "src/parse-gram.c"
    break;

  case 63:
/* Line 1427 of yacc.c  */
#line 493 "src/parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2478 "src/parse-gram.c"
    break;

  case 64:
/* Line 1427 of yacc.c  */
#line 495 "src/parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[-1].list), symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0]))); }
/* Line 1427 of yacc.c  */
#line 2486 "src/parse-gram.c"
    break;

  case 65:
/* Line 1427 of yacc.c  */
#line 499 "src/parse-gram.y"
    { (yyval.list) = (yyvsp[0].list); }
/* Line 1427 of yacc.c  */
#line 2494 "src/parse-gram.c"
    break;

  case 66:
/* Line 1427 of yacc.c  */
#line 500 "src/parse-gram.y"
    { (yyval.list) = symbol_list_prepend ((yyvsp[-1].list), (yyvsp[0].list)); }
/* Line 1427 of yacc.c  */
#line 2502 "src/parse-gram.c"
    break;

  case 67:
/* Line 1427 of yacc.c  */
#line 504 "src/parse-gram.y"
    { (yyval.list) = symbol_list_sym_new ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2510 "src/parse-gram.c"
    break;

  case 68:
/* Line 1427 of yacc.c  */
#line 505 "src/parse-gram.y"
    { (yyval.list) = symbol_list_type_new ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2518 "src/parse-gram.c"
    break;

  case 69:
/* Line 1427 of yacc.c  */
#line 506 "src/parse-gram.y"
    { (yyval.list) = symbol_list_default_tagged_new ((yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2526 "src/parse-gram.c"
    break;

  case 70:
/* Line 1427 of yacc.c  */
#line 507 "src/parse-gram.y"
    { (yyval.list) = symbol_list_default_tagless_new ((yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2534 "src/parse-gram.c"
    break;

  case 71:
/* Line 1427 of yacc.c  */
#line 513 "src/parse-gram.y"
    {
       current_type = (yyvsp[0].uniqstr);
       tag_seen = true;
     }
/* Line 1427 of yacc.c  */
#line 2545 "src/parse-gram.c"
    break;

  case 72:
/* Line 1427 of yacc.c  */
#line 518 "src/parse-gram.y"
    {
       symbol_class_set ((yyvsp[0].symbol), current_class, (yylsp[0]), true);
       symbol_type_set ((yyvsp[0].symbol), current_type, (yylsp[0]));
     }
/* Line 1427 of yacc.c  */
#line 2556 "src/parse-gram.c"
    break;

  case 73:
/* Line 1427 of yacc.c  */
#line 523 "src/parse-gram.y"
    {
      symbol_class_set ((yyvsp[-1].symbol), current_class, (yylsp[-1]), true);
      symbol_type_set ((yyvsp[-1].symbol), current_type, (yylsp[-1]));
      symbol_user_token_number_set ((yyvsp[-1].symbol), (yyvsp[0].integer), (yylsp[0]));
    }
/* Line 1427 of yacc.c  */
#line 2568 "src/parse-gram.c"
    break;

  case 74:
/* Line 1427 of yacc.c  */
#line 529 "src/parse-gram.y"
    {
      symbol_class_set ((yyvsp[-1].symbol), current_class, (yylsp[-1]), true);
      symbol_type_set ((yyvsp[-1].symbol), current_type, (yylsp[-1]));
      symbol_make_alias ((yyvsp[-1].symbol), (yyvsp[0].symbol), (yyloc));
    }
/* Line 1427 of yacc.c  */
#line 2580 "src/parse-gram.c"
    break;

  case 75:
/* Line 1427 of yacc.c  */
#line 535 "src/parse-gram.y"
    {
      symbol_class_set ((yyvsp[-2].symbol), current_class, (yylsp[-2]), true);
      symbol_type_set ((yyvsp[-2].symbol), current_type, (yylsp[-2]));
      symbol_user_token_number_set ((yyvsp[-2].symbol), (yyvsp[-1].integer), (yylsp[-1]));
      symbol_make_alias ((yyvsp[-2].symbol), (yyvsp[0].symbol), (yyloc));
    }
/* Line 1427 of yacc.c  */
#line 2593 "src/parse-gram.c"
    break;

  case 82:
/* Line 1427 of yacc.c  */
#line 565 "src/parse-gram.y"
    {
      yyerrok;
    }
/* Line 1427 of yacc.c  */
#line 2603 "src/parse-gram.c"
    break;

  case 83:
/* Line 1427 of yacc.c  */
#line 571 "src/parse-gram.y"
    { current_lhs = (yyvsp[-1].symbol); current_lhs_location = (yylsp[-1]);
    current_lhs_named_ref = (yyvsp[0].named_ref); }
/* Line 1427 of yacc.c  */
#line 2612 "src/parse-gram.c"
    break;

  case 85:
/* Line 1427 of yacc.c  */
#line 576 "src/parse-gram.y"
    { grammar_current_rule_end ((yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2620 "src/parse-gram.c"
    break;

  case 86:
/* Line 1427 of yacc.c  */
#line 577 "src/parse-gram.y"
    { grammar_current_rule_end ((yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2628 "src/parse-gram.c"
    break;

  case 88:
/* Line 1427 of yacc.c  */
#line 583 "src/parse-gram.y"
    { grammar_current_rule_begin (current_lhs, current_lhs_location,
				  current_lhs_named_ref); }
/* Line 1427 of yacc.c  */
#line 2637 "src/parse-gram.c"
    break;

  case 89:
/* Line 1427 of yacc.c  */
#line 586 "src/parse-gram.y"
    { grammar_current_rule_symbol_append ((yyvsp[-1].symbol), (yylsp[-1]), (yyvsp[0].named_ref)); }
/* Line 1427 of yacc.c  */
#line 2645 "src/parse-gram.c"
    break;

  case 90:
/* Line 1427 of yacc.c  */
#line 588 "src/parse-gram.y"
    { grammar_current_rule_action_append ((yyvsp[-1].code), (yylsp[-1]), (yyvsp[0].named_ref)); }
/* Line 1427 of yacc.c  */
#line 2653 "src/parse-gram.c"
    break;

  case 91:
/* Line 1427 of yacc.c  */
#line 590 "src/parse-gram.y"
    { grammar_current_rule_prec_set ((yyvsp[0].symbol), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2661 "src/parse-gram.c"
    break;

  case 92:
/* Line 1427 of yacc.c  */
#line 592 "src/parse-gram.y"
    { grammar_current_rule_dprec_set ((yyvsp[0].integer), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2669 "src/parse-gram.c"
    break;

  case 93:
/* Line 1427 of yacc.c  */
#line 594 "src/parse-gram.y"
    { grammar_current_rule_merge_set ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2677 "src/parse-gram.c"
    break;

  case 94:
/* Line 1427 of yacc.c  */
#line 598 "src/parse-gram.y"
    { (yyval.named_ref) = 0; }
/* Line 1427 of yacc.c  */
#line 2685 "src/parse-gram.c"
    break;

  case 95:
/* Line 1427 of yacc.c  */
#line 600 "src/parse-gram.y"
    { (yyval.named_ref) = named_ref_new((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2693 "src/parse-gram.c"
    break;

  case 97:
/* Line 1427 of yacc.c  */
#line 612 "src/parse-gram.y"
    { (yyval.uniqstr) = uniqstr_new ((yyvsp[0].chars)); }
/* Line 1427 of yacc.c  */
#line 2701 "src/parse-gram.c"
    break;

  case 98:
/* Line 1427 of yacc.c  */
#line 617 "src/parse-gram.y"
    { (yyval.chars) = ""; }
/* Line 1427 of yacc.c  */
#line 2709 "src/parse-gram.c"
    break;

  case 99:
/* Line 1427 of yacc.c  */
#line 618 "src/parse-gram.y"
    { (yyval.chars) = (yyvsp[0].uniqstr); }
/* Line 1427 of yacc.c  */
#line 2717 "src/parse-gram.c"
    break;

  case 101:
/* Line 1427 of yacc.c  */
#line 629 "src/parse-gram.y"
    {
      code_props plain_code;
      (yyvsp[0].code)[strlen ((yyvsp[0].code)) - 1] = '\n';
      code_props_plain_init (&plain_code, (yyvsp[0].code)+1, (yylsp[0]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      (yyval.chars) = plain_code.code;
    }
/* Line 1427 of yacc.c  */
#line 2732 "src/parse-gram.c"
    break;

  case 102:
/* Line 1427 of yacc.c  */
#line 649 "src/parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2740 "src/parse-gram.c"
    break;

  case 103:
/* Line 1427 of yacc.c  */
#line 651 "src/parse-gram.y"
    {
      (yyval.symbol) = symbol_get (char_name ((yyvsp[0].character)), (yylsp[0]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[0]), false);
      symbol_user_token_number_set ((yyval.symbol), (yyvsp[0].character), (yylsp[0]));
    }
/* Line 1427 of yacc.c  */
#line 2752 "src/parse-gram.c"
    break;

  case 104:
/* Line 1427 of yacc.c  */
#line 659 "src/parse-gram.y"
    { (yyval.symbol) = symbol_from_uniqstr ((yyvsp[0].uniqstr), (yylsp[0])); }
/* Line 1427 of yacc.c  */
#line 2760 "src/parse-gram.c"
    break;

  case 107:
/* Line 1427 of yacc.c  */
#line 671 "src/parse-gram.y"
    {
      (yyval.symbol) = symbol_get (quotearg_style (c_quoting_style, (yyvsp[0].chars)), (yylsp[0]));
      symbol_class_set ((yyval.symbol), token_sym, (yylsp[0]), false);
    }
/* Line 1427 of yacc.c  */
#line 2771 "src/parse-gram.c"
    break;

  case 109:
/* Line 1427 of yacc.c  */
#line 680 "src/parse-gram.y"
    {
      code_props plain_code;
      code_props_plain_init (&plain_code, (yyvsp[0].chars), (yylsp[0]));
      code_props_translate_code (&plain_code);
      gram_scanner_last_string_free ();
      muscle_code_grow ("epilogue", plain_code.code, (yylsp[0]));
      code_scanner_last_string_free ();
    }
/* Line 1427 of yacc.c  */
#line 2786 "src/parse-gram.c"
    break;


/* Line 1427 of yacc.c  */
#line 2791 "src/parse-gram.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      while (1)
        {
          int yysyntax_error_status =
            yysyntax_error (&yymsg_alloc, &yymsg, yystate, yytoken);
          if (yysyntax_error_status == 2 && 0 < yymsg_alloc)
            {
              if (yymsg != yymsgbuf)
                YYSTACK_FREE (yymsg);
              yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
              if (yymsg)
                continue;
              yymsg = yymsgbuf;
              yymsg_alloc = sizeof yymsgbuf;
            }
          if (yysyntax_error_status == 0)
            yyerror (yymsg);
          else
            yyerror (YY_("syntax error"));
          if (yysyntax_error_status == 2)
            goto yyexhaustedlab;
          break;
        }
#endif
    }

  yyerror_range[1] = yylloc;

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

  yyerror_range[1] = yylsp[1-yylen];
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
      if (!yypact_value_is_default (yyn))
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
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

/* Line 1657 of yacc.c  */
#line 690 "src/parse-gram.y"



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


static void
add_param (param_type type, char *decl, location loc)
{
  static char const alphanum[26 + 26 + 1 + 10] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_"
    "0123456789";

  char const *name_start = NULL;
  {
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
  }

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
      if (type & param_lex)
        muscle_pair_list_grow ("lex_param", decl, name);
      if (type & param_parse)
        muscle_pair_list_grow ("parse_param", decl, name);
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
