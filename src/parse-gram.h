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
     PERCENT_VERBOSE = 273,
     PERCENT_ERROR_VERBOSE = 274,
     PERCENT_OUTPUT = 275,
     PERCENT_FILE_PREFIX = 276,
     PERCENT_NAME_PREFIX = 277,
     PERCENT_DEFINE = 278,
     PERCENT_PURE_PARSER = 279,
     PERCENT_DEFINES = 280,
     PERCENT_YACC = 281,
     PERCENT_DEBUG = 282,
     PERCENT_LOCATIONS = 283,
     PERCENT_NO_LINES = 284,
     PERCENT_SKELETON = 285,
     PERCENT_TOKEN_TABLE = 286,
     TYPE = 287,
     EQUAL = 288,
     SEMICOLON = 289,
     COLON = 290,
     PIPE = 291,
     ID = 292,
     PERCENT_PERCENT = 293,
     PROLOGUE = 294,
     EPILOGUE = 295,
     BRACED_CODE = 296
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
#define PERCENT_VERBOSE 273
#define PERCENT_ERROR_VERBOSE 274
#define PERCENT_OUTPUT 275
#define PERCENT_FILE_PREFIX 276
#define PERCENT_NAME_PREFIX 277
#define PERCENT_DEFINE 278
#define PERCENT_PURE_PARSER 279
#define PERCENT_DEFINES 280
#define PERCENT_YACC 281
#define PERCENT_DEBUG 282
#define PERCENT_LOCATIONS 283
#define PERCENT_NO_LINES 284
#define PERCENT_SKELETON 285
#define PERCENT_TOKEN_TABLE 286
#define TYPE 287
#define EQUAL 288
#define SEMICOLON 289
#define COLON 290
#define PIPE 291
#define ID 292
#define PERCENT_PERCENT 293
#define PROLOGUE 294
#define EPILOGUE 295
#define BRACED_CODE 296




#ifndef YYSTYPE
#line 91 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  symbol_list_t *list;
  int integer;
  char *string;
  associativity assoc;
} yystype;
/* Line 1313 of /usr/local/share/bison/bison.simple.  */
#line 110 "y.tab.h"
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

