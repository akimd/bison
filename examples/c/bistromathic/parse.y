%require "3.6"

%code top {
  #include <ctype.h>  // isdigit
  #include <math.h>   // cos, sin, etc.
  #include <stddef.h> // ptrdiff_t
  #include <stdio.h>  // printf
  #include <stdlib.h> // calloc.
  #include <string.h> // strcmp

  #include <readline/readline.h>
  #include <readline/history.h>
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
  int yylex (const char **line, YYSTYPE *yylval, YYLTYPE *yylloc);
  void yyerror (YYLTYPE *yylloc, char const *msg);
}

%code {
#define N_
#define _

  // Whether to quit.
  int done = 0;
}

// Don't share global variables between the scanner and the parser.
%define api.pure full

// Generate a push parser.
%define api.push-pull push

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
    EXIT   "exit"
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
| exp     { printf ("%.10g\n", $exp); }
| "exit"  { done = 1; }
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

#undef yyssp
#undef yyesa
#undef yyes
#undef yyes_capacity

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

// How many symbols are registered.
int
symbol_count (void)
{
  int res = 0;
  for (symrec *p = sym_table; p; p = p->next)
    ++res;
  return res;
}


/*----------.
| Scanner.  |
`----------*/

int
yylex (const char **line, YYSTYPE *yylval, YYLTYPE *yylloc)
{
  int c;

  // Ignore white space, get first nonwhite character.
  do {
    // Move the first position onto the last.
    yylloc->first_line = yylloc->last_line;
    yylloc->first_column = yylloc->last_column;

    yylloc->last_column += 1;
    c = *((*line)++);
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

    case 0: return TOK_EOF;

    default:
      // Numbers.
      if (c == '.' || isdigit (c))
        {
          int nchars = 0;
          sscanf (*line - 1, "%lf%n", &yylval->TOK_NUM, &nchars);
          *line += nchars - 1;
          yylloc->last_column += nchars - 1;
          return TOK_NUM;
        }
      // Identifiers.
      else if (islower (c))
        {
          int nchars = 0;
          char buf[100];
          sscanf (*line - 1, "%99[a-z]%n", buf, &nchars);
          *line += nchars - 1;
          yylloc->last_column += nchars - 1;
          if (strcmp (buf, "exit") == 0)
            return TOK_EXIT;
          else
            {
              symrec *s = getsym (buf);
              if (!s)
                s = putsym (buf, TOK_VAR);
              yylval->TOK_VAR = s;
              return s->type;
            }
        }
      // Stray characters.
      else
        {
          yyerror (yylloc, "error: invalid character");
          return yylex (line, yylval, yylloc);
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


/*-----------.
| Readline.  |
`-----------*/

// Parse (and execute) this line.
int process_line (YYLTYPE *lloc, const char *line)
{
  yypstate *ps = yypstate_new ();
  int status = 0;
  do {
    YYSTYPE lval;
    status = yypush_parse (ps, yylex (&line, &lval, lloc), &lval, lloc);
  } while (status == YYPUSH_MORE);
  yypstate_delete (ps);
  lloc->last_line++;
  lloc->last_column = 1;
  return status;
}

// Get the list of possible tokens after INPUT was read.
int
expected_tokens (const char *input,
                 int *tokens, int ntokens)
{
  YYDPRINTF ((stderr, "expected_tokens(\"%s\")", input));

  // Parse the current state of the line.
  YYLTYPE lloc;
  yypstate *ps = yypstate_new ();
  int status = 0;
  do {
    if (!*input)
      break;
    YYSTYPE lval;
    int token = yylex (&input, &lval, &lloc);
    if (!token)
      break;
    status = yypush_parse (ps, token, &lval, &lloc);
  } while (status == YYPUSH_MORE);

  // Then query for the accepted tokens at this point.
  yyparse_context_t yyctx
    = {ps->yyssp, YYEMPTY, &lloc, ps->yyesa, &ps->yyes, &ps->yyes_capacity};
  return yyexpected_tokens (&yyctx, tokens, ntokens);
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
char **
completion (const char *text, int start, int end)
{
  YYDPRINTF ((stderr, "completion(\"%.*s[%.*s]%s\")\n",
              start, rl_line_buffer,
              end - start, rl_line_buffer + start,
              rl_line_buffer + end));

  // Get list of token numbers.
  int tokens[YYNTOKENS];
  char *line = strndup (rl_line_buffer, start);
  int ntokens = expected_tokens (line, tokens, YYNTOKENS);
  free (line);

  // Build MATCHES, the list of possible completions.
  const int len = strlen (text);
  // Need initial prefix and final NULL.
  char **matches = calloc (ntokens + symbol_count () + 2, sizeof *matches);
  int match = 0;
  matches[match++] = strdup (text);
  for (int i = 0; i < ntokens; ++i)
    if (tokens[i] == YYTRANSLATE (TOK_VAR))
      {
        for (symrec *s = sym_table; s; s = s->next)
          if (s->type == TOK_VAR && strncmp (text, s->name, len) == 0)
            matches[match++] = strdup (s->name);
      }
    else if (tokens[i] == YYTRANSLATE (TOK_FUN))
      {
        for (symrec *s = sym_table; s; s = s->next)
          if (s->type == TOK_FUN && strncmp (text, s->name, len) == 0)
            matches[match++] = strdup (s->name);
      }
    else
      {
        const char* token = yysymbol_name (tokens[i]);
        if (strncmp (token, text, strlen (text)) == 0)
          matches[match++] = strdup (token);
      }

  if (yydebug)
    {
      fprintf (stderr, "completion(\"%.*s[%.*s]%s\") = ",
               start, rl_line_buffer,
               end - start, rl_line_buffer + start,
               rl_line_buffer + end);
      for (int i = 1; matches[i]; ++i)
        fprintf (stderr, "%s%s",
                 i == 1 ? "{" : ", ",
                 matches[i]);
      fprintf (stderr, "}\n");
    }

  // Don't fall back to proposing file names.
  rl_attempted_completion_over = 1;
  return matches;
}

void init_readline (void)
{
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "pushcalc";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = completion;
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
  init_readline ();
  YYLTYPE lloc = {1, 1, 1, 1};
  while (!done)
    {
      char *line = readline ("> ");
      if (!line)
        return 0;
      if (*line)
        add_history (line);
      process_line (&lloc, line);
      free (line);
    }
}
