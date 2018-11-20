/* A Bison parser, made by GNU Bison 3.2.1.9-1e55d.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
#line 21 "src/parse-gram.y" /* yacc.c:1905  */

  #include "symlist.h"
  #include "symtab.h"
#line 218 "src/parse-gram.y" /* yacc.c:1905  */

  typedef enum
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;
#line 608 "src/parse-gram.y" /* yacc.c:1905  */

  #include "muscle-tab.h"
  typedef struct
  {
    char const *chars;
    muscle_kind kind;
  } value_type;

#line 77 "src/parse-gram.h" /* yacc.c:1905  */

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
    PERCENT_MERGE = 270,
    PERCENT_CODE = 271,
    PERCENT_DEFAULT_PREC = 272,
    PERCENT_DEFINE = 273,
    PERCENT_DEFINES = 274,
    PERCENT_EXPECT = 275,
    PERCENT_EXPECT_RR = 276,
    PERCENT_FLAG = 277,
    PERCENT_FILE_PREFIX = 278,
    PERCENT_GLR_PARSER = 279,
    PERCENT_INITIAL_ACTION = 280,
    PERCENT_LANGUAGE = 281,
    PERCENT_NAME_PREFIX = 282,
    PERCENT_NO_DEFAULT_PREC = 283,
    PERCENT_NO_LINES = 284,
    PERCENT_NONDETERMINISTIC_PARSER = 285,
    PERCENT_OUTPUT = 286,
    PERCENT_REQUIRE = 287,
    PERCENT_SKELETON = 288,
    PERCENT_START = 289,
    PERCENT_TOKEN_TABLE = 290,
    PERCENT_VERBOSE = 291,
    PERCENT_YACC = 292,
    BRACED_CODE = 293,
    BRACED_PREDICATE = 294,
    BRACKETED_ID = 295,
    CHAR = 296,
    EPILOGUE = 297,
    EQUAL = 298,
    ID = 299,
    ID_COLON = 300,
    PERCENT_PERCENT = 301,
    PIPE = 302,
    PROLOGUE = 303,
    SEMICOLON = 304,
    TAG = 305,
    TAG_ANY = 306,
    TAG_NONE = 307,
    INT = 308,
    PERCENT_PARAM = 309,
    PERCENT_UNION = 310,
    PERCENT_EMPTY = 311
  };
#endif

/* Value type.  */
#if ! defined GRAM_STYPE && ! defined GRAM_STYPE_IS_DECLARED

union GRAM_STYPE
{

  /* precedence_declarator  */
  assoc precedence_declarator;
  /* "string"  */
  char* STRING;
  /* "{...}"  */
  char* BRACED_CODE;
  /* "%?{...}"  */
  char* BRACED_PREDICATE;
  /* "epilogue"  */
  char* EPILOGUE;
  /* "%{...%}"  */
  char* PROLOGUE;
  /* code_props_type  */
  code_props_type code_props_type;
  /* "integer"  */
  int INT;
  /* named_ref.opt  */
  named_ref* yytype_86;
  /* "%param"  */
  param_type PERCENT_PARAM;
  /* symbol.prec  */
  symbol* yytype_73;
  /* id  */
  symbol* id;
  /* id_colon  */
  symbol* id_colon;
  /* symbol  */
  symbol* symbol;
  /* string_as_id  */
  symbol* string_as_id;
  /* symbols.prec  */
  symbol_list* yytype_72;
  /* symbols.1  */
  symbol_list* yytype_74;
  /* generic_symlist  */
  symbol_list* generic_symlist;
  /* generic_symlist_item  */
  symbol_list* generic_symlist_item;
  /* "%<flag>"  */
  uniqstr PERCENT_FLAG;
  /* "[identifier]"  */
  uniqstr BRACKETED_ID;
  /* "identifier"  */
  uniqstr ID;
  /* "identifier:"  */
  uniqstr ID_COLON;
  /* "<tag>"  */
  uniqstr TAG;
  /* tag  */
  uniqstr tag;
  /* variable  */
  uniqstr variable;
  /* "char"  */
  unsigned char CHAR;
  /* value  */
  value_type value;
#line 204 "src/parse-gram.h" /* yacc.c:1905  */
};

typedef union GRAM_STYPE GRAM_STYPE;
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
