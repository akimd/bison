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

%glr-parser
%skeleton "glr2.cc"
%header
%locations
%debug

/* Nice error messages with details. */
%define parse.error detailed

%code requires
{
  #include "ast.hh"
}

%define api.value.type {Node *}

%code
{

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if __cplusplus < 201103L
# define nullptr 0
#endif

  static YYSTYPE stmtMerge (YYSTYPE x0, YYSTYPE x1);

  static int yylex (YYSTYPE *lvalp, YYLTYPE *llocp);
}

%expect-rr 1

%token
  TYPENAME "typename"
  ID "identifier"

%right '='
%left '+'

%destructor { $$->free (); } stmt expr decl declarator TYPENAME ID

%%

prog : %empty
     | prog stmt   { std::cout << @2 << ": " << *$2 << '\n'; $2->free (); }
     ;

stmt : expr ';'  %merge <stmtMerge>     { $$ = $1; }
     | decl      %merge <stmtMerge>
     | error ';'        { $$ = new Nterm ("<error>"); }
     | '@'              { $$ = $1; YYACCEPT; }
     ;

expr : ID
     | TYPENAME '(' expr ')'
                        { $$ = new Nterm ("<cast>", $3, $1); }
     | expr '+' expr    { $$ = new Nterm ("+", $1, $3); }
     | expr '=' expr    { $$ = new Nterm ("=", $1, $3); }
     ;

decl : TYPENAME declarator ';'
                        { $$ = new Nterm ("<declare>", $1, $2); }
     | TYPENAME declarator '=' expr ';'
                        { $$ = new Nterm ("<init-declare>", $1,
                                          $2, $4); }
     ;

declarator
     : ID
     | '(' declarator ')' { $$ = $2; }
     ;

%%
/* A C error reporting function.  */
void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

int yylex (YYSTYPE *lvalp, YYLTYPE *llocp)
{
  static int lineNum = 1;
  static int colNum = 0;

  while (1)
    {
      int c;
      assert (!feof (stdin));
      c = getchar ();
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
            int tok;
            llocp->begin.line = llocp->end.line = lineNum;
            llocp->begin.column = colNum;
            if (isalpha (c))
              {
                std::string form;
                do
                  {
                    form += static_cast<char> (c);
                    colNum += 1;
                    c = getchar ();
                  }
                while (isalnum (c) || c == '_');

                ungetc (c, stdin);
                tok
                  = isupper (static_cast <unsigned char> (form[0]))
                  ? yy::parser::token::TYPENAME
                  : yy::parser::token::ID;
                *lvalp = new Term (form);
              }
            else
              {
                colNum += 1;
                tok = c;
                *lvalp = nullptr;
              }
            llocp->end.column = colNum-1;
            return tok;
          }
        }
    }
}

static YYSTYPE
stmtMerge (YYSTYPE x0, YYSTYPE x1)
{
  return new Nterm ("<OR>", x0, x1);
}

int
main (int argc, char **argv)
{
  // Enable parse traces on option -p.
  if (1 < argc && strcmp (argv[1], "-p") == 0)
    yydebug = 1;
  yy::parser parser;
  return !!parser.parse ();
}
