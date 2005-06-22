%skeleton "lalr1.cc"                                  /*  -*- C++ -*- */
%define "parser_class_name" "calcxx_parser"
%defines
%{
# include <string>
# include "calc++-driver.hh"
%}

%error-verbose

// The parsing context.
%parse-param { calcxx_driver& driver }
%lex-param   { calcxx_driver& driver }

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

// Define yydebug.
%debug

// Symbols.

%union
{
  /// Value of a numeric literal.
  int          ival;
  /// Name of a variable.
  std::string *sval;
};

%token        YYEOF          0 "end of file"
%token        TOKEN_ASSIGN     ":="
%token <sval> TOKEN_IDENTIFIER "identifier"
%token <ival> TOKEN_NUMBER     "number"
%type  <ival> exp              "expression"

%printer    { debug_stream () << *$$; } "identifier"
%destructor { delete $$; } "identifier"

%printer    { debug_stream () << $$; } "number" "expression"

%%
%start unit;
unit: assignments exp  { driver.result = $2; };

assignments: assignments assignment {}
           | /* Nothing. */         {};

assignment: TOKEN_IDENTIFIER ":=" exp { driver.variables[*$1] = $3; };

%left '+' '-';
%left '*' '/';
exp: exp '+' exp   { $$ = $1 + $3; }
   | exp '-' exp   { $$ = $1 - $3; }
   | exp '*' exp   { $$ = $1 * $3; }
   | exp '/' exp   { $$ = $1 / $3; }
   | TOKEN_IDENTIFIER  { $$ = driver.variables[*$1]; }
   | TOKEN_NUMBER      { $$ = $1; };
%%
void
yy::calcxx_parser::error (const location& l, const std::string& m)
{
  driver.error (l, m);
}
