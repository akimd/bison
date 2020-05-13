%language "D"

%define api.parser.class {Calc}
%define parse.error verbose

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
import std.range.primitives;
import std.stdio;

auto calcLexer(R)(R range)
  if (isInputRange!R && is (ElementType!R : dchar))
{
  return new CalcLexer!R(range);
}

auto calcLexer (File f)
{
  import std.algorithm : map, joiner;
  import std.utf : byDchar;

  return f.byChunk(1024)        // avoid making a syscall roundtrip per char
          .map!(chunk => cast(char[]) chunk) // because byChunk returns ubyte[]
          .joiner               // combine chunks into a single virtual range of char
          .calcLexer;           // forward to other overload
}

class CalcLexer(R) : Lexer
  if (isInputRange!R && is (ElementType!R : dchar))
{
  R input;

  this(R r) { input = r; }

  // Should be a local in main, shared with %parse-param.
  int exit_status = 0;

  public void yyerror (string s)
  {
    exit_status = 1;
    stderr.writeln (s);
  }

  YYSemanticType semanticVal_;

  public final @property YYSemanticType semanticVal ()
  {
    return semanticVal_;
  }

  int yylex ()
  {
    import std.uni : isWhite, isNumber;

    // Skip initial spaces
    while (!input.empty && input.front != '\n' && isWhite (input.front))
      input.popFront;

    if (input.empty)
      return TokenKind.YYEOF;

    // Numbers.
    if (input.front.isNumber)
      {
        import std.conv : parse;
        semanticVal_.ival = input.parse!int;
        return TokenKind.NUM;
      }

    // Individual characters
    auto ch = input.front;
    input.popFront;
    switch (ch)
      {
      case '=':  return TokenKind.EQ;
      case '+':  return TokenKind.PLUS;
      case '-':  return TokenKind.MINUS;
      case '*':  return TokenKind.STAR;
      case '/':  return TokenKind.SLASH;
      case '(':  return TokenKind.LPAR;
      case ')':  return TokenKind.RPAR;
      case '\n': return TokenKind.EOL;
      default: assert(0);
      }
  }
}

int main ()
{
  auto l = calcLexer (stdin);
  auto p = new Calc (l);
  p.parse ();
  return l.exit_status;
}
