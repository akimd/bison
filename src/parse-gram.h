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
     PERCENT_LEFT = 265,
     PERCENT_RIGHT = 266,
     PERCENT_NONASSOC = 267,
     PERCENT_EXPECT = 268,
     PERCENT_START = 269,
     PERCENT_PREC = 270,
     PERCENT_VERBOSE = 271,
     PERCENT_ERROR_VERBOSE = 272,
     PERCENT_OUTPUT = 273,
     PERCENT_FILE_PREFIX = 274,
     PERCENT_NAME_PREFIX = 275,
     PERCENT_DEFINE = 276,
     PERCENT_PURE_PARSER = 277,
     PERCENT_DEFINES = 278,
     PERCENT_YACC = 279,
     PERCENT_DEBUG = 280,
     PERCENT_LOCATIONS = 281,
     PERCENT_NO_LINES = 282,
     PERCENT_SKELETON = 283,
     PERCENT_TOKEN_TABLE = 284,
     TYPE = 285,
     EQUAL = 286,
     SEMICOLON = 287,
     COLON = 288,
     PIPE = 289,
     ID = 290,
     PERCENT_PERCENT = 291,
     PROLOGUE = 292,
     EPILOGUE = 293,
     BRACED_CODE = 294
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
#define PERCENT_LEFT 265
#define PERCENT_RIGHT 266
#define PERCENT_NONASSOC 267
#define PERCENT_EXPECT 268
#define PERCENT_START 269
#define PERCENT_PREC 270
#define PERCENT_VERBOSE 271
#define PERCENT_ERROR_VERBOSE 272
#define PERCENT_OUTPUT 273
#define PERCENT_FILE_PREFIX 274
#define PERCENT_NAME_PREFIX 275
#define PERCENT_DEFINE 276
#define PERCENT_PURE_PARSER 277
#define PERCENT_DEFINES 278
#define PERCENT_YACC 279
#define PERCENT_DEBUG 280
#define PERCENT_LOCATIONS 281
#define PERCENT_NO_LINES 282
#define PERCENT_SKELETON 283
#define PERCENT_TOKEN_TABLE 284
#define TYPE 285
#define EQUAL 286
#define SEMICOLON 287
#define COLON 288
#define PIPE 289
#define ID 290
#define PERCENT_PERCENT 291
#define PROLOGUE 292
#define EPILOGUE 293
#define BRACED_CODE 294




#ifndef YYSTYPE
#line 89 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  symbol_list *list;
  int integer;
  char *string;
  associativity assoc;
} yystype;
/* Line 1271 of /usr/local/share/bison/bison.simple.  */
#line 106 "y.tab.h"
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

