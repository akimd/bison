// Prologue (directives).
%expect 0

// Emitted in the header file, before the definition of YYSTYPE.
%code requires
{
  typedef void* yyscan_t;
  typedef struct
  {
    // Whether to print the intermediate results.
    int verbose;
    // Value of the last computation.
    int value;
    // Number of errors.
    int nerrs;
  } result;
}

// Emitted in the header file, after the definition of YYSTYPE.
%code provides
{
  // Tell Flex the expected prototype of yylex.
  // The scanner argument must be named yyscanner.
#define YY_DECL                                 \
  enum yytokentype yylex (YYSTYPE* yylval, yyscan_t yyscanner, result *res)
  YY_DECL;

  void yyerror (yyscan_t scanner, result *res, const char *msg);
}

// Emitted on top of the implementation file.
%code top
{
#include <stdio.h>  /* printf. */
#include <stdlib.h> /* getenv. */
}

%code
{
  result parse_string (const char* cp);
  result parse (void);
}

%define api.pure full
%define api.token.prefix {TOK_}
%define api.value.type union
%define parse.error verbose
%define parse.trace
 // Scanner and error count are exchanged between main, yyparse and yylex.
%param {yyscan_t scanner}{result *res}

%token
  PLUS   "+"
  MINUS  "-"
  STAR   "*"
  SLASH  "/"
  EOL    "end-of-line"
  EOF 0  "end-of-file"
;

%token <int> NUM "number"
%type <int> exp line
%printer { fprintf (yyo, "%d", $$); } <int>

%token <char*> STR "string"
%printer { fprintf (yyo, "\"%s\"", $$); } <char*>

// Precedence (from lowest to highest) and associativity.
%left "+" "-"
%left "*" "/"

%%
// Rules.
input:
  %empty
| input line
  {
    res->value = $line;
    if (res->verbose)
      printf ("%d\n", $line);
  }
;

line:
  exp EOL   { $$ = $1; }
| exp       { $$ = $1; }
| error EOL { $$ = 0; yyerrok; }
;

exp:
  exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp
  {
    if ($3 == 0)
      {
        yyerror (scanner, res, "invalid division by zero");
        YYERROR;
      }
    else
      $$ = $1 / $3;
  }
| STR
  {
    result r = parse_string ($1);
    free ($1);
    if (r.nerrs)
      {
        res->nerrs += r.nerrs;
        YYERROR;
      }
    else
      $$ = r.value;
  }
| NUM      { $$ = $1; }
;
%%
#include "scan.h"

result
parse (void)
{
  result res = {1, 0, 0};
  yyscan_t scanner;
  yylex_init (&scanner);
  yyparse (scanner, &res);
  yylex_destroy (scanner);
  return res;
}

result
parse_string (const char *str)
{
  result res = {0, 0, 0};
  yyscan_t scanner;
  yylex_init (&scanner);
  YY_BUFFER_STATE buf = yy_scan_string (str, scanner);
  yyparse (scanner, &res);
  yy_delete_buffer(buf, scanner);
  yylex_destroy (scanner);
  return res;
}

// Epilogue (C code).
void yyerror (yyscan_t scanner, result *res, const char *msg)
{
  (void) scanner;
  fprintf (stderr, "%s\n", msg);
  res->nerrs += 1;
}

int main (void)
{
  // Possibly enable parser runtime debugging.
  yydebug = !!getenv ("YYDEBUG");
  result res = parse ();
  // Exit on failure if there were errors.
  return !!res.nerrs;
}
