/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
# define YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED
/* Debug traces.  */
#ifndef GRAM_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define GRAM_DEBUG 1
#  else
#   define GRAM_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define GRAM_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined GRAM_DEBUG */
#if GRAM_DEBUG
extern int gram_debug;
#endif
/* "%code requires" blocks.  */
#line 21 "src/parse-gram.y" /* yacc.c:1909  */

  #include "symlist.h"
  #include "symtab.h"
#line 233 "src/parse-gram.y" /* yacc.c:1909  */

  typedef enum
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;
#line 723 "src/parse-gram.y" /* yacc.c:1909  */
#include "muscle-tab.h"

#line 68 "src/parse-gram.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef GRAM_TOKENTYPE
# define GRAM_TOKENTYPE
  enum gram_tokentype
  {
    GRAM_EOF = 0,
    STRING = 258,
    PERCENT_TOKEN = 259,
    PERCENT_NTERM = 260,
    PERCENT_TYPE = 261,
    PERCENT_DESTRUCTOR = 262,
    PERCENT_PRINTER = 263,
    PERCENT_LEFT = 264,
    PERCENT_RIGHT = 265,
    PERCENT_NONASSOC = 266,
    PERCENT_PRECEDENCE = 267,
    PERCENT_PREC = 268,
    PERCENT_DPREC = 269,
    PERCENT_GPREC = 270,
    PERCENT_PRECR = 271,
    PERCENT_MERGE = 272,
    PERCENT_CODE = 273,
    PERCENT_DEFAULT_PREC = 274,
    PERCENT_DEFINE = 275,
    PERCENT_DEFINES = 276,
    PERCENT_ERROR_VERBOSE = 277,
    PERCENT_EXPECT = 278,
    PERCENT_EXPECT_RR = 279,
    PERCENT_FLAG = 280,
    PERCENT_FILE_PREFIX = 281,
    PERCENT_GLR_PARSER = 282,
    PERCENT_INITIAL_ACTION = 283,
    PERCENT_LANGUAGE = 284,
    PERCENT_NAME_PREFIX = 285,
    PERCENT_NO_DEFAULT_PREC = 286,
    PERCENT_NO_LINES = 287,
    PERCENT_NONDETERMINISTIC_PARSER = 288,
    PERCENT_OUTPUT = 289,
    PERCENT_REQUIRE = 290,
    PERCENT_SKELETON = 291,
    PERCENT_START = 292,
    PERCENT_TOKEN_TABLE = 293,
    PERCENT_VERBOSE = 294,
    PERCENT_YACC = 295,
    BRACED_CODE = 296,
    BRACED_PREDICATE = 297,
    BRACKETED_ID = 298,
    CHAR = 299,
    EPILOGUE = 300,
    EQUAL = 301,
    ID = 302,
    ID_COLON = 303,
    PERCENT_PERCENT = 304,
    PIPE = 305,
    PROLOGUE = 306,
    SEMICOLON = 307,
    GT = 308,
    TAG = 309,
    TAG_ANY = 310,
    TAG_NONE = 311,
    LBRACE = 312,
    RBRACE = 313,
    INT = 314,
    PERCENT_PARAM = 315,
    PERCENT_UNION = 316,
    PERCENT_EMPTY = 317
  };
#endif

/* Value type.  */
#if ! defined GRAM_STYPE && ! defined GRAM_STYPE_IS_DECLARED
typedef union GRAM_STYPE GRAM_STYPE;
union GRAM_STYPE
{
#line 187 "src/parse-gram.y" /* yacc.c:1909  */
unsigned char character;
#line 191 "src/parse-gram.y" /* yacc.c:1909  */
char *code;
#line 196 "src/parse-gram.y" /* yacc.c:1909  */
uniqstr uniqstr;
#line 204 "src/parse-gram.y" /* yacc.c:1909  */
int integer;
#line 208 "src/parse-gram.y" /* yacc.c:1909  */
symbol *symbol;
#line 213 "src/parse-gram.y" /* yacc.c:1909  */
assoc assoc;
#line 216 "src/parse-gram.y" /* yacc.c:1909  */
symbol_list *list;
#line 219 "src/parse-gram.y" /* yacc.c:1909  */
named_ref *named_ref;
#line 224 "src/parse-gram.y" /* yacc.c:1909  */
prec_rel_comparator prec_rel_comparator;
#line 253 "src/parse-gram.y" /* yacc.c:1909  */
param_type param;
#line 423 "src/parse-gram.y" /* yacc.c:1909  */
code_props_type code_type;
#line 725 "src/parse-gram.y" /* yacc.c:1909  */

  struct
  {
    char const *chars;
    muscle_kind kind;
  } value;

#line 174 "src/parse-gram.h" /* yacc.c:1909  */
};
# define GRAM_STYPE_IS_TRIVIAL 1
# define GRAM_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined GRAM_LTYPE && ! defined GRAM_LTYPE_IS_DECLARED
typedef struct GRAM_LTYPE GRAM_LTYPE;
struct GRAM_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define GRAM_LTYPE_IS_DECLARED 1
# define GRAM_LTYPE_IS_TRIVIAL 1
#endif



int gram_parse (void);

#endif /* !YY_GRAM_SRC_PARSE_GRAM_H_INCLUDED  */
