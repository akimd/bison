%require "3.6"

%code top {
  #include <ctype.h>  // isdigit
  #include <math.h>   // cos, sin, etc.
  #include <stddef.h> // ptrdiff_t
  #include <stdio.h>  // printf
  #include <string.h> // strcmp
}

%code requires {
  // Function type.
  typedef double (func_t) (double);

  // Data type for links in the chain of symbols.
  typedef struct symrec symrec;
  struct symrec
  {
    char *name;  // name of symbol
    int type;    // type of symbol: either VAR or FUN
    union
    {
      double var;    // value of a VAR
      func_t *fun;   // value of a FUN
    } value;
    symrec *next;  // link field
  };

  symrec *putsym (char const *name, int sym_type);
  symrec *getsym (char const *name);
}

%code provides {
  int yylex (YYSTYPE *yylval, YYLTYPE *yylloc);
  void yyerror (YYLTYPE *yylloc, char const *);
}

%code {
#define N_
#define _
}

// Don't share global variables between the scanner and the parser.
%define api.pure full

// To avoid name clashes (e.g., with C's EOF) prefix token definitions
// with TOK_ (e.g., TOK_EOF).
%define api.token.prefix {TOK_}

// Customized syntax error messages (see yyreport_syntax_error).
%define parse.error custom

// with locations.
%locations

// and acurate list of expected tokens.
%define parse.lac full

// Generate the parser description file (calc.output).
%verbose

// Generate YYSTYPE from the types assigned to symbols.
%define api.value.type union
%token
    PLUS   "+"
    MINUS  "-"
    STAR   "*"
    SLASH  "/"
    CARET  "^"
    LPAREN "("
    RPAREN ")"
    EQUAL  "="
    EOL    _("end of line")
    EOF 0  _("end of file")
  <double>
    NUM _("double precision number")
  <symrec*>
    FUN _("function")
    VAR _("variable")

%nterm <double>  exp

// Enable run-time traces (yydebug).
%define parse.trace

// Formatting semantic values in debug traces.
%printer { fprintf (yyo, "%s", $$->name); } VAR;
%printer { fprintf (yyo, "%s()", $$->name); } FUN;
%printer { fprintf (yyo, "%g", $$); } <double>;


// Precedence (from lowest to highest) and associativity.
%precedence "="
%left "+" "-"
%left "*" "/"
%precedence NEG // negation--unary minus
%right "^"      // exponentiation

%% // The grammar follows.
input:
  %empty
| input line
;

line:
  EOL
| exp EOL   { printf ("%.10g\n", $exp); }
| error EOL { yyerrok; }
;

exp:
  NUM
| VAR               { $$ = $VAR->value.var; }
| VAR "=" exp       { $$ = $3; $VAR->value.var = $3; }
| FUN "(" exp ")"   { $$ = $FUN->value.fun ($3); }
| exp[l] "+" exp[r] { $$ = $l + $r; }
| exp[l] "-" exp[r] { $$ = $l - $r; }
| exp[l] "*" exp[r] { $$ = $l * $r; }
| exp[l] "/" exp[r]
  {
    if ($r == 0)
      {
        yyerror (&@$, "error: division by zero");
        YYERROR;
      }
    else
      $$ = $l / $r;
  }
| "-" exp  %prec NEG { $$ = -$2; }
| exp[l] "^" exp[r]  { $$ = pow ($l, $r); }
| "(" exp ")"        { $$ = $2; }
;

// End of grammar.
%%

/*------------.
| Functions.  |
`------------*/

struct init
{
  char const *name;
  func_t *fun;
};

static struct init const funs[] =
{
  { "atan", atan },
  { "cos",  cos  },
  { "exp",  exp  },
  { "ln",   log  },
  { "sin",  sin  },
  { "sqrt", sqrt },
  { 0, 0 },
};

// The symbol table: a chain of 'struct symrec'.
static symrec *sym_table;

// Put functions in table.
static void
init_table (void)
{
  for (int i = 0; funs[i].name; i++)
    {
      symrec *ptr = putsym (funs[i].name, TOK_FUN);
      ptr->value.fun = funs[i].fun;
    }
}

symrec *
putsym (char const *name, int sym_type)
{
  symrec *res = (symrec *) malloc (sizeof (symrec));
  res->name = strdup (name);
  res->type = sym_type;
  res->value.var = 0; // Set value to 0 even if fun.
  res->next = sym_table;
  sym_table = res;
  return res;
}

symrec *
getsym (char const *name)
{
  for (symrec *p = sym_table; p; p = p->next)
    if (strcmp (p->name, name) == 0)
      return p;
  return NULL;
}


/*----------.
| Scanner.  |
`----------*/

int
yylex (YYSTYPE *yylval, YYLTYPE *yylloc)
{
  int c;

  // Ignore white space, get first nonwhite character.
  do {
    // Move the first position onto the last.
    yylloc->first_line = yylloc->last_line;
    yylloc->first_column = yylloc->last_column;

    yylloc->last_column += 1;
    c = getchar ();
  } while (c == ' ' || c == '\t');

  switch (c)
    {
    case '+': return TOK_PLUS;
    case '-': return TOK_MINUS;
    case '*': return TOK_STAR;
    case '/': return TOK_SLASH;
    case '^': return TOK_CARET;
    case '=': return TOK_EQUAL;
    case '(': return TOK_LPAREN;
    case ')': return TOK_RPAREN;

    case '\n':
      yylloc->last_column = 1;
      yylloc->last_line += 1;
      return TOK_EOL;

    case EOF: return TOK_EOF;

      // Any other character is a token by itself.
    default:
      if (c == '.' || isdigit (c))
        {
          ungetc (c, stdin);
          int nchars = 0;
          scanf ("%lf%n", &yylval->TOK_NUM, &nchars);
          yylloc->last_column += nchars - 1;
          return TOK_NUM;
        }
      else if (islower (c))
        {
          ungetc (c, stdin);
          int nchars = 0;
          char buf[100];
          scanf ("%99[a-z]%n", buf, &nchars);
          symrec *s = getsym (buf);
          if (!s)
            s = putsym (buf, TOK_VAR);
          yylval->TOK_VAR = s;
          yylloc->last_column += nchars - 1;
          return s->type;
        }
      else
        {
          yyerror (yylloc, "error: invalid character");
          return yylex (yylval, yylloc);
        }
    }
}


/*---------.
| Parser.  |
`---------*/

int
yyreport_syntax_error (const yyparse_context_t *ctx)
{
  enum { ARGMAX = 10 };
  int arg[ARGMAX];
  int n = yysyntax_error_arguments (ctx, arg, ARGMAX);
  if (n == -2)
    return 2;
  YY_LOCATION_PRINT (stderr, *yyparse_context_location (ctx));
  fprintf (stderr, ": syntax error");
  for (int i = 1; i < n; ++i)
    fprintf (stderr, "%s %s",
             i == 1 ? ": expected" : " or", yysymbol_name (arg[i]));
  if (n)
    fprintf (stderr, " before %s", yysymbol_name (arg[0]));
  fprintf (stderr, "\n");
  return 0;
}

// Called by yyparse on error.
void yyerror (YYLTYPE *loc, char const *msg)
{
  YY_LOCATION_PRINT (stderr, *loc);
  fprintf (stderr, ": %s\n", msg);
}


/*-------.
| Main.  |
`-------*/

int main (int argc, char const* argv[])
{
  // Enable parse traces on option -p.
  if (argc == 2 && strcmp(argv[1], "-p") == 0)
    yydebug = 1;
  init_table ();
  return yyparse ();
}
