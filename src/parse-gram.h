/* A Bison parser, made by GNU Bison 2.3a+.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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
     PERCENT_PREC = 268,
     PERCENT_DPREC = 269,
     PERCENT_MERGE = 270,
     PERCENT_CODE = 271,
     PERCENT_CODE_TOP = 272,
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
     PERCENT_PROVIDES = 292,
     PERCENT_PURE_PARSER = 293,
     PERCENT_PUSH_PARSER = 294,
     PERCENT_PUSH_PULL_PARSER = 295,
     PERCENT_REQUIRE = 296,
     PERCENT_REQUIRES = 297,
     PERCENT_SKELETON = 298,
     PERCENT_START = 299,
     PERCENT_TOKEN_TABLE = 300,
     PERCENT_VERBOSE = 301,
     PERCENT_YACC = 302,
     BRACED_CODE = 303,
     CHAR = 304,
     EPILOGUE = 305,
     EQUAL = 306,
     ID = 307,
     ID_COLON = 308,
     PERCENT_PERCENT = 309,
     PIPE = 310,
     PROLOGUE = 311,
     SEMICOLON = 312,
     TYPE = 313,
     TYPE_TAG_ANY = 314,
     TYPE_TAG_NONE = 315,
     PERCENT_UNION = 316
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
#define PERCENT_PREC 268
#define PERCENT_DPREC 269
#define PERCENT_MERGE 270
#define PERCENT_CODE 271
#define PERCENT_CODE_TOP 272
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
#define PERCENT_PROVIDES 292
#define PERCENT_PURE_PARSER 293
#define PERCENT_PUSH_PARSER 294
#define PERCENT_PUSH_PULL_PARSER 295
#define PERCENT_REQUIRE 296
#define PERCENT_REQUIRES 297
#define PERCENT_SKELETON 298
#define PERCENT_START 299
#define PERCENT_TOKEN_TABLE 300
#define PERCENT_VERBOSE 301
#define PERCENT_YACC 302
#define BRACED_CODE 303
#define CHAR 304
#define EPILOGUE 305
#define EQUAL 306
#define ID 307
#define ID_COLON 308
#define PERCENT_PERCENT 309
#define PIPE 310
#define PROLOGUE 311
#define SEMICOLON 312
#define TYPE 313
#define TYPE_TAG_ANY 314
#define TYPE_TAG_NONE 315
#define PERCENT_UNION 316




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 1538 of yacc.c  */
#line 98 "parse-gram.y"

  symbol *symbol;
  symbol_list *list;
  int integer;
  char const *chars;
  char *code;
  assoc assoc;
  uniqstr uniqstr;
  unsigned char character;
}

/* Line 1538 of yacc.c  */
#line 188 "parse-gram.h"
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





