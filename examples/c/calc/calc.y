%code top {
  #include <ctype.h>  /* isdigit. */
  #include <stdbool.h>
  #include <stdio.h>  /* For printf, etc. */
  #include <string.h> /* strcmp. */

  int yylex (void);
  void yyerror (char const *);

  bool show_expected = false;

#define PRINT_EXPECTED_TOKENS()                                         \
  do {                                                                  \
    if (show_expected)                                                  \
      {                                                                 \
        yyparse_context_t ctx                                           \
          = {yyssp, yytoken, yyesa, &yyes, &yyes_capacity};             \
        int tokens[YYNTOKENS];                                          \
        int cnt = yyexpected_tokens (&ctx, tokens, YYNTOKENS);          \
        fprintf (stderr, "expected tokens in state %d rule %d (%d):",   \
                 *yyssp, yyn - 1, cnt);                                 \
        for (int i = 0; i < cnt; ++i)                                   \
          fprintf (stderr, " %s", yysymbol_name(tokens[i]));            \
        fprintf (stderr, "\n");                                         \
      }                                                                 \
  } while (0)
}

%define api.header.include {"calc.h"}
%define api.value.type union /* Generate YYSTYPE from these types:  */
%define parse.error custom
%define parse.lac full
%token <double> NUM "number"
%type  <double> expr term fact

/* Generate the parser description file.  */
%verbose
/* Enable run-time traces (yydebug).  */
%define parse.trace

/* Formatting semantic values.  */
%printer { fprintf (yyo, "%g", $$); } <double>;

%% /* The grammar follows.  */
input:
  %empty       { PRINT_EXPECTED_TOKENS (); }
| input line   { PRINT_EXPECTED_TOKENS (); }
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
  "number"                                   { PRINT_EXPECTED_TOKENS (); }
| '(' expr { PRINT_EXPECTED_TOKENS (); } ')' { $$ = $expr; }
;

%%

int
yyreport_syntax_error (const yyparse_context_t *ctx)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 10 };
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  int arg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  int n = yysyntax_error_arguments (ctx, arg, sizeof arg / sizeof *arg);
  if (n == -2)
    return 2;
  fprintf (stderr, "SYNTAX ERROR on token [%s]", yysymbol_name (arg[0]));
  if (1 < n)
    {
      fprintf (stderr, " (expected:");
      for (int i = 1; i < n; ++i)
        fprintf (stderr, " [%s]", yysymbol_name (arg[i]));
      fprintf (stderr, ")");
    }
  fprintf (stderr, "\n");
  return 0;
}

int
yylex (void)
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
      scanf ("%lf", &yylval.NUM);
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
    if (!strcmp (argv[i], "-e"))
      show_expected = 1;
    else if (!strcmp (argv[i], "-p"))
      yydebug = 1;
  return yyparse ();
}
