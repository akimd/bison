/* Parser for lexcalc.   -*- C -*-

   Copyright (C) 2018-2020 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

// Prologue (directives).
%expect 0

// Emitted in the header file, after the definition of YYSTYPE.
%code provides
{
  // Tell Flex the expected prototype of yylex.
#define YY_DECL                                 \
  yytoken_kind_t yylex (YYSTYPE* yylval, YYLTYPE *yylloc)
  YY_DECL;

  void yyerror (YYLTYPE *loc, const char *msg);
}

// Emitted on top of the implementation file.
%code top
{
#include <stdio.h>  // printf.
#include <stdlib.h> // getenv.
#include <string.h> // strcmp.
}

// Include the header in the implementation rather than duplicating it.
%define api.header.include {"parse.h"}

// Don't share global variables between the scanner and the parser.
%define api.pure full

// To avoid name clashes (e.g., with C's EOF) prefix token definitions
// with TOK_ (e.g., TOK_EOF).
%define api.token.prefix {TOK_}

// %token and %type use genuine types (e.g., "%token <int>").  Let
// %bison define YYSTYPE as a union of all these types.
%define api.value.type union

// Generate detailed error messages.
%define parse.error detailed

// with locations.
%locations

// Enable debug traces (see yydebug in main).
%define parse.trace

%token
  PLUS   "+"
  MINUS  "-"
  STAR   "*"
  SLASH  "/"
  LPAREN "("
  RPAREN ")"
  EOL    "end of line"
;

%token <int> NUM "number"
%type <int> exp expression line
%printer { fprintf (yyo, "%d", $$); } <int>

%start input expression NUM

// Precedence (from lowest to highest) and associativity.
%left "+" "-"
%left "*" "/"

%%
// Rules.
input:
  %empty
| input line
;

line:
  exp EOL   { $$ = $exp; printf ("%d\n", $$); }
| error EOL { $$ = 0; yyerrok; }
;

expression:
  exp EOL  { $$ = $exp; }
;

exp:
  exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp
  {
    if ($3 == 0)
      {
        yyerror (&@$, "error: division by zero");
        YYERROR;
      }
    else
      $$ = $1 / $3;
  }
| "(" exp ")"   { $$ = $2; }
| NUM           { $$ = $1; }
;
%%
// Epilogue (C code).

void yyerror (YYLTYPE *loc, const char *msg)
{
  YY_LOCATION_PRINT (stderr, *loc);
  fprintf (stderr, ": %s\n", msg);
}

int main (int argc, const char *argv[])
{
  // Possibly enable parser runtime debugging.
  yydebug = !!getenv ("YYDEBUG");
  int parse_expression_p = 0;
  int nerrs = 0;

  // Enable parse traces on option -p.
  for (int i = 0; i < argc; ++i)
    if (1 < argc && strcmp (argv[1], "-p") == 0)
      yydebug = 1;
    else if (strcmp (argv[i], "-e") == 0)
      parse_expression_p = 1;

  if (parse_expression_p)
    {
      yyparse_expression_t res = yyparse_expression ();
      nerrs = res.yynerrs;
      if (res.yystatus == 0)
        printf ("expression: %d\n", res.yyvalue);
      else
        printf ("expression: failure\n");
    }
  else
    nerrs = yyparse_input ().yynerrs;

  if (nerrs)
    fprintf (stderr, "errors: %d\n", nerrs);

  // Exit on failure if there were errors.
  return !!nerrs;
}
