/* Prologue (directives).   -*- C -*- */

/* Disable Flex features we don't need, to avoid warnings. */
%option nodefault noinput nounput noyywrap

%{
#include <errno.h> /* errno, ERANGE */
#include <limits.h> /* INT_MIN */
#include <stdlib.h> /* strtol */

#include "parse.h"

  // Each time a rule is matched, advance the end cursor/position.
#define YY_USER_ACTION                          \
  yylloc->last_column += yyleng;
%}

%%
%{
  // Each time yylex is called, move the head position to the end one.
  yylloc->first_line = yylloc->last_line;
  yylloc->first_column = yylloc->last_column;
%}
 /* Rules.  */

"+"      return TOK_PLUS;
"-"      return TOK_MINUS;
"*"      return TOK_STAR;
"/"      return TOK_SLASH;
"^"      return TOK_CARET;

"("      return TOK_LPAREN;
")"      return TOK_RPAREN;

"="      return TOK_EQUAL;

 /* Scan an identifier.  */
[a-z]+   {
  symrec *s = getsym (yytext);
  if (!s)
    s = putsym (yytext, TOK_VAR);
  yylval->TOK_VAR = s;
  return s->type;
}

 /* Scan a double precision number.  */
[0-9]+(\.[0-9]*)?|(\.[0-9]+)   {
  sscanf (yytext, "%lf", &yylval->TOK_NUM);
  return TOK_NUM;
}

"\n"     yylloc->last_line++; yylloc->last_column = 1; return TOK_EOL;

 /* Ignore white spaces. */
[ \t]+   continue;

<<EOF>>  return TOK_EOF;

.        yyerror (yylloc, "syntax error, invalid character");
%%
/* Epilogue (C code). */
