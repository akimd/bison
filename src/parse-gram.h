#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef YYSTYPE
typedef union
{
  symbol_t *symbol;
  int integer;
  char *string;
  associativity assoc;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
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
# define YYLTYPE_IS_TRIVIAL 1
#endif

# define	GRAM_EOF	0
# define	STRING	257
# define	CHARACTER	258
# define	INT	259
# define	PERCENT_TOKEN	260
# define	PERCENT_NTERM	261
# define	PERCENT_TYPE	262
# define	PERCENT_UNION	263
# define	PERCENT_EXPECT	264
# define	PERCENT_START	265
# define	PERCENT_LEFT	266
# define	PERCENT_RIGHT	267
# define	PERCENT_NONASSOC	268
# define	PERCENT_PREC	269
# define	PERCENT_VERBOSE	270
# define	PERCENT_ERROR_VERBOSE	271
# define	PERCENT_OUTPUT	272
# define	PERCENT_FILE_PREFIX	273
# define	PERCENT_NAME_PREFIX	274
# define	PERCENT_DEFINE	275
# define	PERCENT_PURE_PARSER	276
# define	PERCENT_DEFINES	277
# define	PERCENT_YACC	278
# define	PERCENT_DEBUG	279
# define	PERCENT_LOCATIONS	280
# define	PERCENT_NO_LINES	281
# define	PERCENT_SKELETON	282
# define	PERCENT_TOKEN_TABLE	283
# define	TYPE	284
# define	EQUAL	285
# define	SEMICOLON	286
# define	COLON	287
# define	PIPE	288
# define	ID	289
# define	PERCENT_PERCENT	290
# define	PROLOGUE	291
# define	EPILOGUE	292
# define	BRACED_CODE	293


#endif /* not BISON_Y_TAB_H */
