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
     PERCENT_UNION = 265,
     PERCENT_LEFT = 266,
     PERCENT_RIGHT = 267,
     PERCENT_NONASSOC = 268,
     PERCENT_EXPECT = 269,
     PERCENT_START = 270,
     PERCENT_PREC = 271,
     PERCENT_VERBOSE = 272,
     PERCENT_ERROR_VERBOSE = 273,
     PERCENT_OUTPUT = 274,
     PERCENT_FILE_PREFIX = 275,
     PERCENT_NAME_PREFIX = 276,
     PERCENT_DEFINE = 277,
     PERCENT_PURE_PARSER = 278,
     PERCENT_DEFINES = 279,
     PERCENT_YACC = 280,
     PERCENT_DEBUG = 281,
     PERCENT_LOCATIONS = 282,
     PERCENT_NO_LINES = 283,
     PERCENT_SKELETON = 284,
     PERCENT_TOKEN_TABLE = 285,
     TYPE = 286,
     EQUAL = 287,
     SEMICOLON = 288,
     COLON = 289,
     PIPE = 290,
     ID = 291,
     PERCENT_PERCENT = 292,
     PROLOGUE = 293,
     EPILOGUE = 294,
     BRACED_CODE = 295
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
#define PERCENT_UNION 265
#define PERCENT_LEFT 266
#define PERCENT_RIGHT 267
#define PERCENT_NONASSOC 268
#define PERCENT_EXPECT 269
#define PERCENT_START 270
#define PERCENT_PREC 271
#define PERCENT_VERBOSE 272
#define PERCENT_ERROR_VERBOSE 273
#define PERCENT_OUTPUT 274
#define PERCENT_FILE_PREFIX 275
#define PERCENT_NAME_PREFIX 276
#define PERCENT_DEFINE 277
#define PERCENT_PURE_PARSER 278
#define PERCENT_DEFINES 279
#define PERCENT_YACC 280
#define PERCENT_DEBUG 281
#define PERCENT_LOCATIONS 282
#define PERCENT_NO_LINES 283
#define PERCENT_SKELETON 284
#define PERCENT_TOKEN_TABLE 285
#define TYPE 286
#define EQUAL 287
#define SEMICOLON 288
#define COLON 289
#define PIPE 290
#define ID 291
#define PERCENT_PERCENT 292
#define PROLOGUE 293
#define EPILOGUE 294
#define BRACED_CODE 295




#ifndef YYSTYPE
#line 91 "parse-gram.y"
typedef union {
  symbol_t *symbol;
  symbol_list_t *list;
  int integer;
  char *string;
  associativity assoc;
} yystype;
/* Line 1271 of /usr/local/share/bison/bison.simple.  */
#line 108 "y.tab.h"
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

