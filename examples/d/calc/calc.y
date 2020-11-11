/* Parser and scanner for calc in D.   -*- D -*-

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

%language "D"

%define api.parser.class {Calc}
%define parse.error verbose

%locations

%union {
  int ival;
}

/* Bison Declarations */
%token PLUS   "+"
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
| error EOL         { yyerrok(); }
;

exp:
  NUM                  { $$ = $1; }
| exp "+" exp          { $$ = $1 + $3; }
| exp "-" exp          { $$ = $1 - $3; }
| exp "*" exp          { $$ = $1 * $3; }
| exp "/" exp          { $$ = $1 / $3; }
| "+" exp  %prec UNARY { $$ = $2; }
| "-" exp  %prec UNARY { $$ = -$2; }
| "(" exp ")"          { $$ = $2; }
;

%%
import std.range.primitives;
import std.stdio;

auto calcLexer(R)(R range)
if (isInputRange!R && is(ElementType!R : dchar))
{
  return new CalcLexer!R(range);
}

auto calcLexer(File f)
{
  import std.algorithm : map, joiner;
  import std.utf : byDchar;

  return f.byChunk(1024)        // avoid making a syscall roundtrip per char
          .map!(chunk => cast(char[]) chunk) // because byChunk returns ubyte[]
          .joiner               // combine chunks into a single virtual range of char
          .calcLexer;           // forward to other overload
}

class CalcLexer(R) : Lexer
if (isInputRange!R && is(ElementType!R : dchar))
{
  R input;

  this(R r) { input = r; }

  YYPosition start;
  YYPosition end;

  // Should be a local in main, shared with %parse-param.
  int exit_status = 0;

  void yyerror(const YYLocation loc, string s)
  {
    exit_status = 1;
    stderr.writeln(loc.toString(), ": ", s);
  }

  YYSemanticType semanticVal_;

  public final YYSemanticType semanticVal()
  {
    return semanticVal_;
  }

  Calc.Symbol yylex()
  {
    import std.uni : isWhite, isNumber;

    // Skip initial spaces
    while (!input.empty && input.front != '\n' && isWhite(input.front))
    {
      start = end;
      end.column++;
      input.popFront;
    }

    if (input.empty)
      return Calc.Symbol(TokenKind.YYEOF, new YYLocation(startPos, endPos));

    // Numbers.
    if (input.front.isNumber)
    {
      int lenChars = 0;
      auto copy = input;
      import std.conv : parse;
      semanticVal_.ival = input.parse!int;
      while (!input.empty && copy.front != input.front)
      {
        lenChars++;
        copy.popFront;
      }
      start = end;
      end.column += lenChars;
      return Calc.Symbol(TokenKind.NUM, semanticVal_.ival, new YYLocation(startPos, endPos));
    }

    // Individual characters
    auto ch = input.front;
    input.popFront;
    start = end;
    end.column++;
    switch (ch)
    {
      case '+':  return Calc.Symbol(TokenKind.PLUS, new YYLocation(startPos, endPos));
      case '-':  return Calc.Symbol(TokenKind.MINUS, new YYLocation(startPos, endPos));
      case '*':  return Calc.Symbol(TokenKind.STAR, new YYLocation(startPos, endPos));
      case '/':  return Calc.Symbol(TokenKind.SLASH, new YYLocation(startPos, endPos));
      case '(':  return Calc.Symbol(TokenKind.LPAR, new YYLocation(startPos, endPos));
      case ')':  return Calc.Symbol(TokenKind.RPAR, new YYLocation(startPos, endPos));
      case '\n':
      {
        end.line++;
        end.column = 1;
        return Calc.Symbol(TokenKind.EOL, new YYLocation(startPos, endPos));
      }
      default: assert(0);
    }
  }

  YYPosition startPos() const
  {
    return start;
  }

  YYPosition endPos() const
  {
    return end;
  }
}

int main()
{
  auto l = calcLexer(stdin);
  auto p = new Calc(l);
  p.parse();
  return l.exit_status;
}
