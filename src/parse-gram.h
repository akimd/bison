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
     PERCENT_UNION = 264,
     PERCENT_EXPECT = 265,
     PERCENT_START = 266,
     PERCENT_PREC = 267,
     PERCENT_VERBOSE = 268,
     PERCENT_ERROR_VERBOSE = 269,
     PERCENT_OUTPUT = 270,
     PERCENT_FILE_PREFIX = 271,
     PERCENT_NAME_PREFIX = 272,
     PERCENT_DEFINE = 273,
     PERCENT_PURE_PARSER = 274,
     PERCENT_DEFINES = 275,
     PERCENT_YACC = 276,
     PERCENT_DEBUG = 277,
     PERCENT_LOCATIONS = 278,
     PERCENT_NO_LINES = 279,
     PERCENT_SKELETON = 280,
     PERCENT_TOKEN_TABLE = 281,
     TYPE = 282,
     EQUAL = 283,
     SEMICOLON = 284,
     COLON = 285,
     PIPE = 286,
     ID = 287,
     PERCENT_PERCENT = 288,
     PROLOGUE = 289,
     EPILOGUE = 290,
     BRACED_CODE = 291,
     PERCENT_LEFT = 292,
     PERCENT_RIGHT = 293,
     PERCENT_NONASSOC = 294
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
#define PERCENT_UNION 264
#define PERCENT_EXPECT 265
#define PERCENT_START 266
#define PERCENT_PREC 267
#define PERCENT_VERBOSE 268
#define PERCENT_ERROR_VERBOSE 269
#define PERCENT_OUTPUT 270
#define PERCENT_FILE_PREFIX 271
#define PERCENT_NAME_PREFIX 272
#define PERCENT_DEFINE 273
#define PERCENT_PURE_PARSER 274
#define PERCENT_DEFINES 275
#define PERCENT_YACC 276
#define PERCENT_DEBUG 277
#define PERCENT_LOCATIONS 278
#define PERCENT_NO_LINES 279
#define PERCENT_SKELETON 280
#define PERCENT_TOKEN_TABLE 281
#define TYPE 282
#define EQUAL 283
#define SEMICOLON 284
#define COLON 285
#define PIPE 286
#define ID 287
#define PERCENT_PERCENT 288
#define PROLOGUE 289
#define EPILOGUE 290
#define BRACED_CODE 291
#define PERCENT_LEFT 292
#define PERCENT_RIGHT 293
#define PERCENT_NONASSOC 294




#ifndef YYSTYPE
#line 89 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  int integer;
  char *string;
  associativity assoc;
} yystype;
/* Line 1271 of /usr/local/share/bison/bison.simple.  */
#line 105 "y.tab.h"
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

