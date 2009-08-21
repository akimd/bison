%skeleton "lalr1.cc"
%defines
%define parse.assert
%define parse.trace
%define variant
%define lex_symbol
%locations

%code requires // *.hh
{
#include <list>
#include <string>
typedef std::list<std::string> strings_type;
}

%code // *.cc
{
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

  // Prototype of the yylex function providing subsequent tokens.
  static yy::parser::symbol_type yylex ();

  // Printing a list of strings.
  // Koening look up will look into std, since that's an std::list.
  namespace std
  {
    std::ostream&
    operator<< (std::ostream& o, const strings_type& s)
    {
      std::copy (s.begin (), s.end (),
                 std::ostream_iterator<strings_type::value_type> (o, "\n"));
      return o;
    }
  }

  // Conversion to string.
  template <typename T>
    inline
    std::string
    string_cast (const T& t)
  {
    std::ostringstream o;
    o << t;
    return o.str ();
  }
}

%token <::std::string> TEXT;
%token <int> NUMBER;
%printer { debug_stream () << $$; }
   <int> <::std::string> <::std::list<std::string>>;
%token END_OF_FILE 0;

%type <::std::string> item;
%type <::std::list<std::string>> list;

%%

result:
  list  { std::cout << $1 << std::endl; }
;

list:
  /* nothing */ { /* Generates an empty string list */ }
| list item     { std::swap ($$, $1); $$.push_back ($2); }
;

item:
  TEXT          { std::swap ($$, $1); }
| NUMBER        { $$ = string_cast ($1); }
;
%%

// The yylex function providing subsequent tokens:
// TEXT         "I have three numbers for you:"
// NUMBER       1
// NUMBER       2
// NUMBER       3
// TEXT         " and that's all!"
// END_OF_FILE

static
yy::parser::symbol_type
yylex ()
{
  static int stage = -1;
  ++stage;
  yy::parser::location_type loc(0, stage + 1, stage + 1);
  switch (stage)
  {
    case 0:
      return yy::parser::make_TEXT ("I have three numbers for you.", loc);
    case 1:
    case 2:
    case 3:
      return yy::parser::make_NUMBER (stage, loc);
    case 4:
      return yy::parser::make_TEXT ("And that's all!", loc);
    default:
      return yy::parser::make_END_OF_FILE (loc);
  }
}

// Mandatory error function
void
yy::parser::error (const yy::parser::location_type& loc, const std::string& msg)
{
  std::cerr << loc << ": " << msg << std::endl;
}

int
main ()
{
  yy::parser p;
  p.set_debug_level (!!getenv ("YYDEBUG"));
  return p.parse ();
}

// Local Variables:
// mode: C++
// End:
