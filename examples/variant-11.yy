/*
  Copyright (C) 2008-2015, 2018 Free Software Foundation, Inc.

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

%debug
%language "c++"
%define api.token.constructor
%define api.value.type variant
%define api.value.automove
%define api.location.file none
%define parse.assert
%locations

%code requires // *.hh
{
#include <memory> // std::unique_ptr
#include <string>
#include <vector>

  using string_uptr = std::unique_ptr<std::string>;
  using string_uptrs = std::vector<string_uptr>;
}

%code // *.cc
{
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

  namespace yy
  {
    // Prototype of the yylex function providing subsequent tokens.
    static parser::symbol_type yylex ();

    // Print a vector of strings.
    std::ostream&
    operator<< (std::ostream& o, const string_uptrs& ss)
    {
      o << '{';
      const char *sep = "";
      for (const auto& s: ss)
        {
          o << sep << *s;
          sep = ", ";
        }
      return o << '}';
    }
  }

  template <typename... Args>
  string_uptr
  make_string_uptr (Args&&... args)
  {
    // std::make_unique is C++14.
    return string_uptr (new std::string{std::forward<Args> (args)...});
  }

  // Convert to string.
  template <typename T>
    std::string
    to_string (const T& t)
  {
    auto&& o = std::ostringstream{};
    o << t;
    return o.str ();
  }
}

%token <string_uptr> TEXT;
%token <int> NUMBER;
%printer { yyo << '(' << &$$ << ") " << $$; } <*>;
%printer { yyo << *$$; } <string_uptr>;
%token END_OF_FILE 0;

%type <string_uptr> item;
%type <string_uptrs> list;

%%

result:
  list  { std::cout << $1 << '\n'; }
;

list:
  %empty     { /* Generates an empty string list */ }
| list item  { $$ = $1; $$.emplace_back ($2); }
;

item:
  TEXT    { $$ = $1; }
| NUMBER  { $$ = make_string_uptr (to_string ($1)); }
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
    static auto count = 0u;
    auto stage = count;
    ++count;
    auto loc = parser::location_type{nullptr, stage + 1, stage + 1};
    switch (stage)
      {
      case 0:
        return parser::make_TEXT (make_string_uptr ("I have three numbers for you."), std::move (loc));
      case 1:
      case 2:
      case 3:
        return parser::make_NUMBER (stage, std::move (loc));
      case 4:
        return parser::make_TEXT (make_string_uptr ("And that's all!"), std::move (loc));
      default:
        return parser::make_END_OF_FILE (std::move (loc));
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
  auto&& p = yy::parser{};
  p.set_debug_level (!!getenv ("YYDEBUG"));
  return p.parse ();
}

// Local Variables:
// mode: C++
// End:
