// Prologue (directives).
%expect 0

// Emitted in the header file, after the definition of YYSTYPE.
%code provides
{
  // Tell Flex the expected prototype of yylex.
#define YY_DECL                                 \
  enum yytokentype yylex (YYSTYPE* yylval, int *nerrs)
  YY_DECL;

  void yyerror (int *nerrs, const char *msg);
}

// Emitted on top of the implementation file.
%code top
{
#include <stdio.h>  // printf.
#include <stdlib.h> // getenv.
}

%define api.pure full
%define api.token.prefix {TOK_}
%define api.value.type union
%define parse.error verbose
%define parse.trace
 // Error count, exchanged between main, yyparse and yylex.
%param {int *nerrs}

%token
  PLUS   "+"
  MINUS  "-"
  STAR   "*"
  SLASH  "/"
  LPAREN "("
  RPAREN ")"
  EOL    "end-of-line"
  EOF 0  "end-of-file"
;

%token <int> NUM "number"
%type <int> exp
%printer { fprintf (yyo, "%d", $$); } <int>

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
  exp EOL   { printf ("%d\n", $exp); }
| error EOL { yyerrok; }
;

exp:
  exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp
  {
    if ($3 == 0)
      {
        yyerror (nerrs, "invalid division by zero");
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
void yyerror (int *nerrs, const char *msg)
{
  fprintf (stderr, "%s\n", msg);
  ++*nerrs;
}

int main (void)
{
  int nerrs = 0;
  // Possibly enable parser runtime debugging.
  yydebug = !!getenv ("YYDEBUG");
  yyparse (&nerrs);
  // Exit on failure if there were errors.
  return !!nerrs;
}
