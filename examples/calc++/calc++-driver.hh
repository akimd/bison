#line 7036 "../../doc/bison.texinfo"
#ifndef CALCXX_DRIVER_HH
# define CALCXX_DRIVER_HH
# include <string>
# include <map>
#line 7051 "../../doc/bison.texinfo"
// Forward declarations.
union YYSTYPE;
namespace yy
{
  class location;
  class calcxx_parser;
}
class calcxx_driver;
#line 7069 "../../doc/bison.texinfo"
// Announce to Flex the prototype we want for lexing function, ...
# define YY_DECL						 \
  int yylex (YYSTYPE* yylval, yy::location* yylloc, calcxx_driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;
#line 7082 "../../doc/bison.texinfo"
// Conducting the whole scanning and parsing of Calc++.
class calcxx_driver
{
public:
  calcxx_driver ();
  virtual ~calcxx_driver ();

  std::map<std::string, int> variables;

  int result;
#line 7101 "../../doc/bison.texinfo"
  // Handling the scanner.
  void scan_begin ();
  void scan_end ();
  bool trace_scanning;
#line 7112 "../../doc/bison.texinfo"
  // Handling the parser.
  void parse (const std::string& f);
  std::string file;
  bool trace_parsing;
#line 7126 "../../doc/bison.texinfo"
  // Error handling.
  void error (const yy::location& l, const std::string& m);
  void error (const std::string& m);
};
#endif // ! CALCXX_DRIVER_HH
