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
    {
      input.popFront;
    }

    // Handle EOF.
    if (input.empty)
      return YYTokenType.EOF;

    // Numbers.
    if (input.front == '.' || input.front.isNumber)
      {
        import std.conv : parse;
        semanticVal_.ival = input.parse!int;
        return YYTokenType.NUM;
      }

    // Individual characters
    auto ch = input.front;
    input.popFront;
    switch (ch)
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
  auto l = calcLexer (stdin);
  auto p = new Calc (l);
  p.parse ();
  return l.exit_status;
}
