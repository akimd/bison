/* A Bison parser, made by GNU Bison 3.5.2.140-15e57-dirty.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

  #include "symlist.h"
  #include "symtab.h"

  typedef enum
  {
    param_none   = 0,
    param_lex    = 1 << 0,
    param_parse  = 1 << 1,
    param_both   = param_lex | param_parse
  } param_type;

  #include "muscle-tab.h"
  typedef struct
  {
    char const *chars;
    muscle_kind kind;
  } value_type;


/* Token type.  */
#ifndef GRAM_TOKENTYPE
# define GRAM_TOKENTYPE
  enum gram_tokentype
  {
    GRAM_EOF = 0,
    STRING = 3,
    TSTRING = 4,
    PERCENT_TOKEN = 5,
    PERCENT_NTERM = 6,
    PERCENT_TYPE = 7,
    PERCENT_DESTRUCTOR = 8,
    PERCENT_PRINTER = 9,
    PERCENT_LEFT = 10,
    PERCENT_RIGHT = 11,
    PERCENT_NONASSOC = 12,
    PERCENT_PRECEDENCE = 13,
    PERCENT_PREC = 14,
    PERCENT_DPREC = 15,
    PERCENT_MERGE = 16,
    PERCENT_CODE = 17,
    PERCENT_DEFAULT_PREC = 18,
    PERCENT_DEFINE = 19,
    PERCENT_DEFINES = 20,
    PERCENT_ERROR_VERBOSE = 21,
    PERCENT_EXPECT = 22,
    PERCENT_EXPECT_RR = 23,
    PERCENT_FLAG = 24,
    PERCENT_FILE_PREFIX = 25,
    PERCENT_GLR_PARSER = 26,
    PERCENT_INITIAL_ACTION = 27,
    PERCENT_LANGUAGE = 28,
    PERCENT_NAME_PREFIX = 29,
    PERCENT_NO_DEFAULT_PREC = 30,
    PERCENT_NO_LINES = 31,
    PERCENT_NONDETERMINISTIC_PARSER = 32,
    PERCENT_OUTPUT = 33,
    PERCENT_PURE_PARSER = 34,
    PERCENT_REQUIRE = 35,
    PERCENT_SKELETON = 36,
    PERCENT_START = 37,
    PERCENT_TOKEN_TABLE = 38,
    PERCENT_VERBOSE = 39,
    PERCENT_YACC = 40,
    BRACED_CODE = 41,
    BRACED_PREDICATE = 42,
    BRACKETED_ID = 43,
    CHAR = 44,
    COLON = 45,
    EPILOGUE = 46,
    EQUAL = 47,
    ID = 48,
    ID_COLON = 49,
    PERCENT_PERCENT = 50,
    PIPE = 51,
    PROLOGUE = 52,
    SEMICOLON = 53,
    TAG = 54,
    TAG_ANY = 55,
    TAG_NONE = 56,
    INT = 57,
    PERCENT_PARAM = 58,
    PERCENT_UNION = 59,
    PERCENT_EMPTY = 60
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
  /* "translatable string"  */
  char* TSTRING;
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
  /* "integer literal"  */
  int INT;
  /* int.opt  */
  int yytype_82;
  /* named_ref.opt  */
  named_ref* yytype_95;
  /* "%param"  */
  param_type PERCENT_PARAM;
  /* token_decl  */
  symbol* token_decl;
  /* alias  */
  symbol* alias;
  /* token_decl_for_prec  */
  symbol* token_decl_for_prec;
  /* id  */
  symbol* id;
  /* id_colon  */
  symbol* id_colon;
  /* symbol  */
  symbol* symbol;
  /* string_as_id  */
  symbol* string_as_id;
  /* generic_symlist  */
  symbol_list* generic_symlist;
  /* generic_symlist_item  */
  symbol_list* generic_symlist_item;
  /* nterm_decls  */
  symbol_list* nterm_decls;
  /* token_decls  */
  symbol_list* token_decls;
  /* token_decl.1  */
  symbol_list* yytype_80;
  /* token_decls_for_prec  */
  symbol_list* token_decls_for_prec;
  /* token_decl_for_prec.1  */
  symbol_list* yytype_85;
  /* symbol_decls  */
  symbol_list* symbol_decls;
  /* symbol_decl.1  */
  symbol_list* yytype_88;
  /* "%error-verbose"  */
  uniqstr PERCENT_ERROR_VERBOSE;
  /* "%<flag>"  */
  uniqstr PERCENT_FLAG;
  /* "%file-prefix"  */
  uniqstr PERCENT_FILE_PREFIX;
  /* "%name-prefix"  */
  uniqstr PERCENT_NAME_PREFIX;
  /* "%pure-parser"  */
  uniqstr PERCENT_PURE_PARSER;
  /* "[identifier]"  */
  uniqstr BRACKETED_ID;
  /* "identifier"  */
  uniqstr ID;
  /* "identifier:"  */
  uniqstr ID_COLON;
  /* "<tag>"  */
  uniqstr TAG;
  /* tag.opt  */
  uniqstr yytype_74;
  /* tag  */
  uniqstr tag;
  /* variable  */
  uniqstr variable;
  /* "character literal"  */
  unsigned char CHAR;
  /* value  */
  value_type value;

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
