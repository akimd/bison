/*
  Copyright (C) 2008-2018 Free Software Foundation, Inc.

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

%require "3.2"
%debug
%language "c++"
%glr-parser
%defines
%define api.token.constructor
%define api.value.type variant
 // FIXME: does not work: %define parse.assert
%locations
%expect-rr 3

%code requires // *.hh
{
#include <vector>
#include <string>
typedef std::vector<std::string> strings_type;
}

%code // *.cc
{
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

  // Whether to issue debug traces.
  bool debug = false;

  namespace yy
  {
    // Prototype of the yylex function providing subsequent tokens.
    static parser::symbol_type yylex ();

    // Printing a vector of strings.
    std::ostream&
    operator<< (std::ostream& o, const strings_type& ss)
    {
      if (debug)
        o << '(' << &ss << ") ";
      o << '{';
      const char *sep = "";
      for (strings_type::const_iterator i = ss.begin(), end = ss.end();
           i != end; ++i)
        {
          o << sep << *i;
          sep = ", ";
        }
      return o << "}";
    }
  }

  // With glr.cc, the actions are not in yy::.
  using yy::operator<<;

  // Conversion to string.
  template <typename T>
  std::string
  string_cast (const T& t)
  {
    std::ostringstream o;
    o << t;
    return o.str ();
  }
}

%token <std::string> TEXT;
%token <int> NUMBER;
%printer { yyo << $$; } <*>;
%token END_OF_FILE 0;

%type <std::string> item;
%type <std::vector<std::string>> list;

%%

result:
  list  { std::cout << $1 << '\n'; }
;

list:
  %empty
  {
    /* Generates an empty string list */
    if (debug)
      std::cerr << "Empty:  This is $$: " << $$ << '\n';
  }
| list item
  {
    if (debug)
      std::cerr
        << "Pre:  This is $$: " << $$ << '\n'
        << "Pre:  This is $1: " << $1 << '\n'
        << "Pre:  This is $2: " << $2 << '\n';
    $$ = $1;
    $$.push_back ($2);
    if (debug)
        std::cerr << "Post: This is $$: " << $$ << '\n'
                  << "Post: This is $1: " << $1 << '\n'
                  << "Post: This is $2: " << $2 << '\n';
  }
;

item:
  TEXT %dprec 1 { $$ = $1; }
| TEXT %dprec 2 { $$ = $1; }
| NUMBER        { $$ = string_cast ($1); }
;
%%

namespace yy
{
  // The yylex function providing subsequent tokens:
  // TEXT         "I have three numbers for you."
  // NUMBER       1
  // NUMBER       2
  // NUMBER       3
  // TEXT         "And that's all!"
  // END_OF_FILE

  static
  parser::symbol_type
  yylex ()
  {
    static int stage = -1;
    ++stage;
    parser::location_type loc(YY_NULLPTR, unsigned(stage + 1), unsigned(stage + 1));
    switch (stage)
      {
      case 0:
        return parser::make_TEXT ("I have three numbers for you.", loc);
      case 1:
      case 2:
      case 3:
        return parser::make_NUMBER (stage, loc);
      case 4:
        return parser::make_TEXT ("And that's all!", loc);
      default:
        return parser::make_END_OF_FILE (loc);
      }
  }

  // Mandatory error function
  void
  parser::error (const parser::location_type& loc, const std::string& msg)
  {
    std::cerr << loc << ": " << msg << '\n';
  }
}

int
main ()
{
  debug = !!getenv ("YYDEBUG");
  yy::parser p;
  p.set_debug_level (debug);
  return p.parse ();
}

// Local Variables:
// mode: C++
// End:
