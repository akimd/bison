/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

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
     PERCENT_UNION = 265,
     PERCENT_LEFT = 266,
     PERCENT_RIGHT = 267,
     PERCENT_NONASSOC = 268,
     PERCENT_PREC = 269,
     PERCENT_DPREC = 270,
     PERCENT_MERGE = 271,
     PERCENT_DEBUG = 272,
     PERCENT_DEFAULT_PREC = 273,
     PERCENT_DEFINE = 274,
     PERCENT_DEFINES = 275,
     PERCENT_ERROR_VERBOSE = 276,
     PERCENT_EXPECT = 277,
     PERCENT_FILE_PREFIX = 278,
     PERCENT_GLR_PARSER = 279,
     PERCENT_INITIAL_ACTION = 280,
     PERCENT_LEX_PARAM = 281,
     PERCENT_LOCATIONS = 282,
     PERCENT_NAME_PREFIX = 283,
     PERCENT_NO_LINES = 284,
     PERCENT_NONDETERMINISTIC_PARSER = 285,
     PERCENT_OUTPUT = 286,
     PERCENT_PARSE_PARAM = 287,
     PERCENT_PURE_PARSER = 288,
     PERCENT_SKELETON = 289,
     PERCENT_START = 290,
     PERCENT_TOKEN_TABLE = 291,
     PERCENT_VERBOSE = 292,
     PERCENT_YACC = 293,
     TYPE = 294,
     EQUAL = 295,
     SEMICOLON = 296,
     PIPE = 297,
     ID = 298,
     ID_COLON = 299,
     PERCENT_PERCENT = 300,
     PROLOGUE = 301,
     EPILOGUE = 302,
     BRACED_CODE = 303
   };
#endif
#define GRAM_EOF 0
#define STRING 258
#define INT 259
#define PERCENT_TOKEN 260
#define PERCENT_NTERM 261
#define PERCENT_TYPE 262
#define PERCENT_DESTRUCTOR 263
#define PERCENT_PRINTER 264
#define PERCENT_UNION 265
#define PERCENT_LEFT 266
#define PERCENT_RIGHT 267
#define PERCENT_NONASSOC 268
#define PERCENT_PREC 269
#define PERCENT_DPREC 270
#define PERCENT_MERGE 271
#define PERCENT_DEBUG 272
#define PERCENT_DEFAULT_PREC 273
#define PERCENT_DEFINE 274
#define PERCENT_DEFINES 275
#define PERCENT_ERROR_VERBOSE 276
#define PERCENT_EXPECT 277
#define PERCENT_FILE_PREFIX 278
#define PERCENT_GLR_PARSER 279
#define PERCENT_INITIAL_ACTION 280
#define PERCENT_LEX_PARAM 281
#define PERCENT_LOCATIONS 282
#define PERCENT_NAME_PREFIX 283
#define PERCENT_NO_LINES 284
#define PERCENT_NONDETERMINISTIC_PARSER 285
#define PERCENT_OUTPUT 286
#define PERCENT_PARSE_PARAM 287
#define PERCENT_PURE_PARSER 288
#define PERCENT_SKELETON 289
#define PERCENT_START 290
#define PERCENT_TOKEN_TABLE 291
#define PERCENT_VERBOSE 292
#define PERCENT_YACC 293
#define TYPE 294
#define EQUAL 295
#define SEMICOLON 296
#define PIPE 297
#define ID 298
#define ID_COLON 299
#define PERCENT_PERCENT 300
#define PROLOGUE 301
#define EPILOGUE 302
#define BRACED_CODE 303




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 80 "parse-gram.y"
typedef union YYSTYPE {
  symbol *symbol;
  symbol_list *list;
  int integer;
  char *chars;
  assoc assoc;
  uniqstr uniqstr;
} YYSTYPE;
/* Line 1285 of yacc.c.  */
#line 144 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
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




