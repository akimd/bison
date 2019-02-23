%language "D"

%define api.parser.class {Calc}
%define parse.error verbose

%code imports {
  import std.ascii;
  import std.stdio;
}

%union {
  int ival;
}

/* Bison Declarations */
%token EQ     "="
       PLUS   "+"
       MINUS  "-"
       STAR   "*"
       SLASH  "/"
       LPAR   "("
       RPAR   ")"
       EOL    "end of line"
%token <ival> NUM "number"
%type  <ival> exp

%left "-" "+"
%left "*" "/"
%precedence UNARY   /* unary operators */

/* Grammar follows */
%%
input:
  line
| input line
;

line:
  EOL
| exp EOL           { writeln ($exp); }
| error EOL
;

exp:
  NUM                  { $$ = $1; }
| exp "+" exp          { $$ = $1 + $3; }
| exp "-" exp          { $$ = $1 - $3; }
| exp "*" exp          { $$ = $1 * $3; }
| exp "/" exp          { $$ = $1 / $3; }
| "+" exp  %prec UNARY { $$ = -$2; }
| "-" exp  %prec UNARY { $$ = -$2; }
| "(" exp ")"          { $$ = $2; }
;

%%
class CalcLexer : Lexer {

  // Should be a local in main, shared with %parse-param.
  int exit_status = 0;

  int
  get_char ()
  {
    import stdc = core.stdc.stdio;
    return stdc.getc (stdc.stdin);
  }

  void
  unget_char (int c)
  {
    import stdc = core.stdc.stdio;
    stdc.ungetc (c, stdc.stdin);
  }

  public void yyerror (string s)
  {
    exit_status = 1;
    stderr.writeln (s);
  }

  int
  read_signed_integer ()
  {
    int c = get_char ();
    int sign = 1;
    int n = 0;

    if (c == '-')
      {
        c = get_char ();
        sign = -1;
      }

    while (isDigit (c))
      {
        n = 10 * n + (c - '0');
        c = get_char ();
      }

    unget_char (c);
    return sign * n;
  }

  YYSemanticType semanticVal_;

  public final @property YYSemanticType semanticVal()
  {
    return semanticVal_;
  }

  YYTokenType yylex ()
  {
    int c;
    /* Skip white spaces.  */
    do
      {}
    while ((c = get_char ()) == ' ' || c == '\t');

    /* process numbers   */
    if (c == '.' || isDigit (c))
      {
        unget_char (c);
        semanticVal_.ival = read_signed_integer ();
        return YYTokenType.NUM;
      }

    switch (c)
      {
      case EOF: return YYTokenType.EOF;
      case '=': return YYTokenType.EQ;
      case '+': return YYTokenType.PLUS;
      case '-': return YYTokenType.MINUS;
      case '*': return YYTokenType.STAR;
      case '/': return YYTokenType.SLASH;
      case '(': return YYTokenType.LPAR;
      case ')': return YYTokenType.RPAR;
      case '\n': return YYTokenType.EOL;
      default:  assert(0);
      }
  }
}

int main ()
{
  CalcLexer l = new CalcLexer ();
  Calc p = new Calc (l);
  p.parse ();
  return l.exit_status;
}
