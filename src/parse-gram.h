/* A Bison parser, made from parse-gram.y, by GNU bison 1.49c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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

#ifndef BISON_PARSE_GRAM_H
# define BISON_PARSE_GRAM_H

/* Tokens.  */
#ifndef YYTOKENTYPE
# if defined (__STDC__) || defined (__cplusplus)
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     GRAM_EOF = 0,
     STRING = 258,
     CHARACTER = 259,
     INT = 260,
     PERCENT_TOKEN = 261,
     PERCENT_NTERM = 262,
     PERCENT_TYPE = 263,
     PERCENT_DESTRUCTOR = 264,
     PERCENT_PRINTER = 265,
     PERCENT_UNION = 266,
     PERCENT_LEFT = 267,
     PERCENT_RIGHT = 268,
     PERCENT_NONASSOC = 269,
     PERCENT_EXPECT = 270,
     PERCENT_START = 271,
     PERCENT_PREC = 272,
     PERCENT_DPREC = 273,
     PERCENT_MERGE = 274,
     PERCENT_VERBOSE = 275,
     PERCENT_ERROR_VERBOSE = 276,
     PERCENT_OUTPUT = 277,
     PERCENT_FILE_PREFIX = 278,
     PERCENT_NAME_PREFIX = 279,
     PERCENT_DEFINE = 280,
     PERCENT_PURE_PARSER = 281,
     PERCENT_GLR_PARSER = 282,
     PERCENT_DEFINES = 283,
     PERCENT_YACC = 284,
     PERCENT_DEBUG = 285,
     PERCENT_LOCATIONS = 286,
     PERCENT_NO_LINES = 287,
     PERCENT_SKELETON = 288,
     PERCENT_TOKEN_TABLE = 289,
     TYPE = 290,
     EQUAL = 291,
     SEMICOLON = 292,
     COLON = 293,
     PIPE = 294,
     ID = 295,
     PERCENT_PERCENT = 296,
     PROLOGUE = 297,
     EPILOGUE = 298,
     BRACED_CODE = 299
   };
# endif
  /* POSIX requires `int' for tokens in interfaces.  */
# define YYTOKENTYPE int
#endif /* !YYTOKENTYPE */
#define GRAM_EOF 0
#define STRING 258
#define CHARACTER 259
#define INT 260
#define PERCENT_TOKEN 261
#define PERCENT_NTERM 262
#define PERCENT_TYPE 263
#define PERCENT_DESTRUCTOR 264
#define PERCENT_PRINTER 265
#define PERCENT_UNION 266
#define PERCENT_LEFT 267
#define PERCENT_RIGHT 268
#define PERCENT_NONASSOC 269
#define PERCENT_EXPECT 270
#define PERCENT_START 271
#define PERCENT_PREC 272
#define PERCENT_DPREC 273
#define PERCENT_MERGE 274
#define PERCENT_VERBOSE 275
#define PERCENT_ERROR_VERBOSE 276
#define PERCENT_OUTPUT 277
#define PERCENT_FILE_PREFIX 278
#define PERCENT_NAME_PREFIX 279
#define PERCENT_DEFINE 280
#define PERCENT_PURE_PARSER 281
#define PERCENT_GLR_PARSER 282
#define PERCENT_DEFINES 283
#define PERCENT_YACC 284
#define PERCENT_DEBUG 285
#define PERCENT_LOCATIONS 286
#define PERCENT_NO_LINES 287
#define PERCENT_SKELETON 288
#define PERCENT_TOKEN_TABLE 289
#define TYPE 290
#define EQUAL 291
#define SEMICOLON 292
#define COLON 293
#define PIPE 294
#define ID 295
#define PERCENT_PERCENT 296
#define PROLOGUE 297
#define EPILOGUE 298
#define BRACED_CODE 299




#ifndef YYSTYPE
#line 89 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  symbol_list_t *list;
  int integer;
  char *string;
  assoc_t assoc;
} yystype;
/* Line 1294 of /usr/local/share/bison/yacc.c.  */
#line 141 "y.tab.h"
# define YYSTYPE yystype
#endif



#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
#endif



#endif /* not BISON_PARSE_GRAM_H */

