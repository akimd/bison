#ifndef CALCXX_DRIVER_HH
# define CALCXX_DRIVER_HH
# include <string>
# include <map>

/// Forward declarations.
union YYSTYPE;

namespace yy
{
  class calcxx_parser;
  class location;
}

class calcxx_driver;

// Announce to Flex the prototype we want for lexing function, ...
# define YY_DECL						\
  int yylex (YYSTYPE* yylval, yy::location* yylloc, calcxx_driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;

/// Conducting the whole scanning and parsing of Calc++.
class calcxx_driver
{
public:
  calcxx_driver ();
  virtual ~calcxx_driver ();

  /// The variables.
  std::map<std::string, int> variables;

  /// \name Handling the scanner.
  /// \{
  /// Open \a file for scanning.
  void scan_begin ();
  /// End scanning, clean up memory.
  void scan_end ();
  /// Whether to enable scanner traces.
  bool trace_scanning;
  /// \}

  /// \name Handling the parser.
  /// \{
  /// Parse the file \a f.
  void parse (const std::string& f);
  /// The file being parsed.
  std::string file;
  /// Whether to enable parsing traces.
  bool trace_parsing;
  /// \}

  /// The result.
  int result;

  /// \name Error handling.
  /// \{
  /// Register a located error.
  void error (const yy::location& l, const std::string& m);
  /// Register an error.
  void error (const std::string& m);
  /// \}
};
#endif
