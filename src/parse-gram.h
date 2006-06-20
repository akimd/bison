/* A Bison parser, made by GNU Bison 2.3+.  */

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
     PERCENT_DEBUG = 271,
     PERCENT_DEFAULT_PREC = 272,
     PERCENT_DEFINE = 273,
     PERCENT_DEFINES = 274,
     PERCENT_ERROR_VERBOSE = 275,
     PERCENT_EXPECT = 276,
     PERCENT_EXPECT_RR = 277,
     PERCENT_FILE_PREFIX = 278,
     PERCENT_GLR_PARSER = 279,
     PERCENT_INITIAL_ACTION = 280,
     PERCENT_LEX_PARAM = 281,
     PERCENT_LOCATIONS = 282,
     PERCENT_NAME_PREFIX = 283,
     PERCENT_NO_DEFAULT_PREC = 284,
     PERCENT_NO_LINES = 285,
     PERCENT_NONDETERMINISTIC_PARSER = 286,
     PERCENT_OUTPUT = 287,
     PERCENT_PARSE_PARAM = 288,
     PERCENT_PURE_PARSER = 289,
     PERCENT_REQUIRE = 290,
     PERCENT_SKELETON = 291,
     PERCENT_START = 292,
     PERCENT_TOKEN_TABLE = 293,
     PERCENT_VERBOSE = 294,
     PERCENT_YACC = 295,
     BRACED_CODE = 296,
     CHAR = 297,
     EPILOGUE = 298,
     EQUAL = 299,
     ID = 300,
     ID_COLON = 301,
     PERCENT_PERCENT = 302,
     PIPE = 303,
     PROLOGUE = 304,
     SEMICOLON = 305,
     TYPE = 306,
     PERCENT_UNION = 307
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
#define PERCENT_DEBUG 271
#define PERCENT_DEFAULT_PREC 272
#define PERCENT_DEFINE 273
#define PERCENT_DEFINES 274
#define PERCENT_ERROR_VERBOSE 275
#define PERCENT_EXPECT 276
#define PERCENT_EXPECT_RR 277
#define PERCENT_FILE_PREFIX 278
#define PERCENT_GLR_PARSER 279
#define PERCENT_INITIAL_ACTION 280
#define PERCENT_LEX_PARAM 281
#define PERCENT_LOCATIONS 282
#define PERCENT_NAME_PREFIX 283
#define PERCENT_NO_DEFAULT_PREC 284
#define PERCENT_NO_LINES 285
#define PERCENT_NONDETERMINISTIC_PARSER 286
#define PERCENT_OUTPUT 287
#define PERCENT_PARSE_PARAM 288
#define PERCENT_PURE_PARSER 289
#define PERCENT_REQUIRE 290
#define PERCENT_SKELETON 291
#define PERCENT_START 292
#define PERCENT_TOKEN_TABLE 293
#define PERCENT_VERBOSE 294
#define PERCENT_YACC 295
#define BRACED_CODE 296
#define CHAR 297
#define EPILOGUE 298
#define EQUAL 299
#define ID 300
#define ID_COLON 301
#define PERCENT_PERCENT 302
#define PIPE 303
#define PROLOGUE 304
#define SEMICOLON 305
#define TYPE 306
#define PERCENT_UNION 307




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 97 "../../src/parse-gram.y"
{
  symbol *symbol;
  symbol_list *list;
  int integer;
  char *chars;
  assoc assoc;
  uniqstr uniqstr;
  unsigned char character;
}
/* Line 1529 of yacc.c.  */
#line 165 "../../src/parse-gram.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
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


