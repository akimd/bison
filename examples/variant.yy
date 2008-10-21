/* Test file for C++ parsers using variants.
   Based on an example by Michiel De Wilde <mdewilde.agilent@gmail.com>. */
%language "C++"
%debug
%defines
%define variant

%code requires // *.hh
{
#include <string>
}

%code // *.cc
{
#include <algorithm>
#include <iostream>
#include <sstream>

static yy::parser::token_type yylex(yy::parser::semantic_type* yylval);
}

%token <std::string> TEXT
%token <int> NUMBER
%printer { debug_stream() << $$; } <int> <std::string>
%token END_OF_FILE 0

%type <std::string> text result

%%

result:
 text  { std::cout << $1 << std::endl; }
;

text:
  /* nothing */         { /* This will generate an empty string */ }
| text TEXT             { std::swap($$,$1); $$.append($2); }
| text NUMBER           {
                          std::swap($$,$1);
                          std::ostringstream o;
                          o << ' ' << $2;
                          $$.append(o.str());
                        }
;
%%

// The yylex function providing subsequent tokens:
// TEXT		"I have three numbers for you:"
// NUMBER	1
// NUMBER	2
// NUMBER	3
// TEXT		" and that's all!"
// END_OF_FILE

static
yy::parser::token_type
yylex(yy::parser::semantic_type* yylval)
{
  static int stage = 0;
  yy::parser::token_type result;

  switch (stage)
  {
    case 0:
      yylval->build<std::string>();
      yylval->as<std::string>() = std::string("I have three numbers for you:");
      result = yy::parser::token::TEXT;
      break;
    case 1:
    case 2:
    case 3:
      yylval->build<int>();
      yylval->as<int>() = stage;
      result = yy::parser::token::NUMBER;
      break;
    case 4:
      yylval->build<std::string>();
      yylval->as<std::string>() = std::string(" and that's all!");
      result = yy::parser::token::TEXT;
      break;
    default:
      result = yy::parser::token::END_OF_FILE;
      break;
  }

  stage++;
  return result;
}

// Mandatory error function
void
yy::parser::error(const yy::parser::location_type& yylloc,
                  const std::string& message)
{
  std::cerr << yylloc << ": " << message << std::endl;
}

int
main(int argc, char *argv[])
{
  yy::parser p;
  p.set_debug_level(!!getenv("YYDEBUG"));
  p.parse();
}

// Local Variables:
// mode: C++
// End:
