/* A Bison parser, made by GNU Bison 2.3a.  */

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
     PERCENT_PUSH_PARSER = 294,
     PERCENT_REQUIRE = 295,
     PERCENT_SKELETON = 296,
     PERCENT_START = 297,
     PERCENT_START_HEADER = 298,
     PERCENT_TOKEN_TABLE = 299,
     PERCENT_VERBOSE = 300,
     PERCENT_YACC = 301,
     BRACED_CODE = 302,
     CHAR = 303,
     EPILOGUE = 304,
     EQUAL = 305,
     ID = 306,
     ID_COLON = 307,
     PERCENT_PERCENT = 308,
     PIPE = 309,
     PROLOGUE = 310,
     SEMICOLON = 311,
     TYPE = 312,
     PERCENT_UNION = 313
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
#define PERCENT_PUSH_PARSER 294
#define PERCENT_REQUIRE 295
#define PERCENT_SKELETON 296
#define PERCENT_START 297
#define PERCENT_START_HEADER 298
#define PERCENT_TOKEN_TABLE 299
#define PERCENT_VERBOSE 300
#define PERCENT_YACC 301
#define BRACED_CODE 302
#define CHAR 303
#define EPILOGUE 304
#define EQUAL 305
#define ID 306
#define ID_COLON 307
#define PERCENT_PERCENT 308
#define PIPE 309
#define PROLOGUE 310
#define SEMICOLON 311
#define TYPE 312
#define PERCENT_UNION 313




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{/* Line 1535 of yacc.c.  */
#line 98 "parse-gram.y"

  symbol *symbol;
  symbol_list *list;
  int integer;
  char *chars;
  assoc assoc;
  uniqstr uniqstr;
  unsigned char character;
}
/* Line 1535 of yacc.c.  */
#line 179 "parse-gram.h"
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



