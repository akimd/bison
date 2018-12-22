/* Prologue (directives).   -*- C++ -*- */

/* Disable Flex features we don't need, to avoid warnings. */
%option nodefault noinput nounput noyywrap

%{
#include <limits.h> /* INT_MIN */
#include <stdlib.h> /* strtol */

#include "parse.h"
%}

%%
 /* Rules.  */

"+"      return TOK_PLUS;
"-"      return TOK_MINUS;
"*"      return TOK_STAR;
"/"      return TOK_SLASH;

"("      return TOK_LPAREN;
")"      return TOK_RPAREN;

 /* Scan an integer.  */
[0-9]+   {
  errno = 0;
  long n = strtol (yytext, NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    yyerror (nerrs, "integer is out of range");
  yylval->TOK_NUM = (int) n;
  return TOK_NUM;
}

 /* Ignore white spaces. */
[ \t]+   continue;

"\n"     return TOK_EOL;

.        yyerror (nerrs, "syntax error, invalid character");

<<EOF>>  return TOK_EOF;
%%
/* Epilogue (C code). */
