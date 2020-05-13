%code top {
  #include <ctype.h>  /* isdigit. */
  #include <stdbool.h>
  #include <stdio.h>  /* For printf, etc. */
  #include <string.h> /* strcmp. */
}

%code {
  int yylex (YYSTYPE *yylval);
  void yyerror (char const *);
}

%define api.header.include {"calc.h"}

/* Generate YYSTYPE from the types used in %token and %type.  */
%define api.value.type union
%token <double> NUM "number"
%type  <double> expr term fact

/* Don't share global variables between the scanner and the parser.  */
%define api.pure full
/* Generate a push parser.  */
%define api.push-pull push

/* Nice error messages with details. */
%define parse.error detailed

/* Generate the parser description file (calc.output).  */
%verbose

 /* Enable run-time traces (yydebug).  */
%define parse.trace

/* Formatting semantic values in debug traces.  */
%printer { fprintf (yyo, "%g", $$); } <double>;

%% /* The grammar follows.  */
input:
  %empty
| input line
;

line:
  '\n'
| expr '\n'  { printf ("%.10g\n", $1); }
| error '\n' { yyerrok; }
;

expr:
  expr '+' term { $$ = $1 + $3; }
| expr '-' term { $$ = $1 - $3; }
| term
;

term:
  term '*' fact { $$ = $1 * $3; }
| term '/' fact { $$ = $1 / $3; }
| fact
;

fact:
  "number"
| '(' expr ')' { $$ = $expr; }
;

%%

int
yylex (YYSTYPE *yylval)
{
  int c;

  /* Ignore white space, get first nonwhite character.  */
  while ((c = getchar ()) == ' ' || c == '\t')
    continue;

  if (c == EOF)
    return 0;

  /* Char starts a number => parse the number.         */
  if (c == '.' || isdigit (c))
    {
      ungetc (c, stdin);
      scanf ("%lf", &yylval->NUM);
      return NUM;
    }

  /* Any other character is a token by itself.        */
  return c;
}

/* Called by yyparse on error.  */
void
yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}

int
main (int argc, char const* argv[])
{
  /* Enable parse traces on option -p.  */
  for (int i = 1; i < argc; ++i)
    if (!strcmp (argv[i], "-p"))
      yydebug = 1;
  int status;
  yypstate *ps = yypstate_new ();
  do {
    YYSTYPE lval;
    status = yypush_parse (ps, yylex (&lval), &lval);
  } while (status == YYPUSH_MORE);
  yypstate_delete (ps);
  return status;
}
