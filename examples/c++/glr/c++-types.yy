/*                                                       -*- C++ -*-
  Copyright (C) 2020 Free Software Foundation, Inc.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Simplified C++ Type and Expression Grammar.
   Written by Paul Hilfinger for Bison's test suite.  */

%require "3.8"
%glr-parser
%skeleton "glr2.cc"
%define parse.assert
%header
%locations
%debug

/* Nice error messages with details. */
%define parse.error detailed

%code requires
{
  #include "ast.hh"
}

%define api.value.type variant

%code
{
  #include <cassert>
  #include <cctype>
  #include <fstream>
  #include <cstring>

  #if __cplusplus < 201103L
  # define nullptr 0
  #endif

  static Node
  stmtMerge (const Node& x0, const Node& x1);

  static int
  yylex (yy::parser::value_type* val, yy::parser::location_type* loc);
}

%expect-rr 1

%type <Node> TYPENAME ID stmt expr decl declarator
%printer { yyo << $$; } <Node>

%token
  TYPENAME "typename"
  ID "identifier"

%right '='
%left '+'

%%

prog : %empty
     | prog stmt   { std::cout << @2 << ": " << $2 << '\n'; }
     ;

stmt : expr ';'  %merge <stmtMerge>   { $$ = $1; }
     | decl      %merge <stmtMerge>
     | error ';'                      { $$ = Nterm ("<error>"); }
     ;

expr : ID
     | TYPENAME '(' expr ')'  { $$ = Nterm ("<cast>", $3, $1); }
     | expr '+' expr          { $$ = Nterm ("+", $1, $3); }
     | expr '=' expr          { $$ = Nterm ("=", $1, $3); }
     ;

decl : TYPENAME declarator ';'
                        { $$ = Nterm ("<declare>", $1, $2); }
     | TYPENAME declarator '=' expr ';'
                        { $$ = Nterm ("<init-declare>", $1, $2, $4); }
     ;

declarator
     : ID
     | '(' declarator ')' { $$ = $2; }
     ;

%%
std::istream* input = nullptr;

/* A C error reporting function.  */
void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

static int
yylex (yy::parser::value_type* lvalp, yy::parser::location_type* llocp)
{
  static int lineNum = 1;
  static int colNum = 0;

  while (1)
    {
      assert (!input->eof ());
      int c = input->get ();
      switch (c)
        {
        case EOF:
          return 0;
        case '\t':
          colNum = (colNum + 7) & ~7;
          break;
        case ' ': case '\f':
          colNum += 1;
          break;
        case '\n':
          lineNum += 1;
          colNum = 0;
          break;
        default:
          {
            llocp->begin.line = llocp->end.line = lineNum;
            llocp->begin.column = colNum;
            int tok;
            if (isalpha (c))
              {
                std::string form;
                do
                  {
                    form += static_cast<char> (c);
                    colNum += 1;
                    c = input->get ();
                  }
                while (isalnum (c) || c == '_');

                input->unget ();
                tok
                  = isupper (static_cast <unsigned char> (form[0]))
                  ? yy::parser::token::TYPENAME
                  : yy::parser::token::ID;
                lvalp->emplace<Node> (Term (form));
              }
            else
              {
                colNum += 1;
                tok = c;
                lvalp = nullptr;
              }
            llocp->end.column = colNum-1;
            return tok;
          }
        }
    }
}

static Node
stmtMerge (const Node& x0, const Node& x1)
{
  return Nterm ("<OR>", x0, x1);
}

int
process (yy::parser& parse, const std::string& file)
{
  bool is_stdin = file == "-" || file.empty ();
  if (is_stdin)
    input = &std::cin;
  else
    input = new std::ifstream (file.c_str ());
  int status = parse ();
  if (!is_stdin)
    delete input;
  return status;
}

int
main (int argc, char **argv)
{
  yy::parser parse;

  if (getenv ("YYDEBUG"))
    parse.set_debug_level (1);

  bool ran = false;
  for (int i = 1; i < argc; ++i)
    // Enable parse traces on option -p.
    if (strcmp (argv[i], "-p") == 0)
      parse.set_debug_level (1);
    else
      {
        int status = process (parse, argv[i]);
        ran = true;
        if (!status)
          return status;
      }

  if (!ran)
    {
      int status = process (parse, "");
      if (!status)
        return status;
    }
  return 0;
}
