/* Bison Grammar Parser                             -*- C -*-
   Copyright (C) 2002 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307  USA
*/


%debug
%defines
%locations
%pure-parser
// %error-verbose
%defines
%name-prefix="gram_"

%{
#include "system.h"
#include "complain.h"
#include "muscle_tab.h"
#include "files.h"
#include "getargs.h"
#include "output.h"
#include "symlist.h"
#include "gram.h"
#include "reader.h"
#include "conflicts.h"

/* Produce verbose syntax errors.  */
#define YYERROR_VERBOSE 1
#define YYLLOC_DEFAULT(Current, Rhs, N)			\
do {							\
  if (N)						\
  {							\
    Current.first_column  = Rhs[1].first_column;	\
    Current.first_line    = Rhs[1].first_line;		\
    Current.last_column   = Rhs[N].last_column;		\
    Current.last_line     = Rhs[N].last_line;		\
  }							\
  else							\
  {							\
    Current = Rhs[0];					\
  }							\
} while (0)

/* Request detailed syntax error messages, and pass them to GRAM_ERROR.
   FIXME: depends on the undocumented availability of YYLLOC.  */
#undef  yyerror
#define yyerror(Msg) \
        gram_error (&yylloc, Msg)

#define YYPRINT(File, Type, Value) \
	print_token_value (File, Type, &Value)
static void print_token_value (FILE *file, int type, YYSTYPE const *value);

static void add_param (char const *, char const *, location_t);

symbol_class current_class = unknown_sym;
struniq_t current_type = 0;
symbol_t *current_lhs;
location_t current_lhs_location;
assoc_t current_assoc;
int current_prec = 0;
braced_code_t current_braced_code = action_braced_code;
%}


/* Only NUMBERS have a value.  */
%union
{
  symbol_t *symbol;
  symbol_list_t *list;
  int integer;
  char *string;
  assoc_t assoc;
  struniq_t struniq;
};

/* Define the tokens together with their human representation.  */
%token GRAM_EOF 0 "end of file"
%token STRING     "string"
%token INT        "integer"

%token PERCENT_TOKEN       "%token"
%token PERCENT_NTERM       "%nterm"

%token PERCENT_TYPE        "%type"
%token PERCENT_DESTRUCTOR  "%destructor"
%token PERCENT_PRINTER     "%printer"

%token PERCENT_UNION       "%union"

%token PERCENT_LEFT        "%left"
%token PERCENT_RIGHT       "%right"
%token PERCENT_NONASSOC    "%nonassoc"

%token PERCENT_PREC          "%prec"
%token PERCENT_DPREC         "%dprec"
%token PERCENT_MERGE         "%merge"


/*----------------------.
| Global Declarations.  |
`----------------------*/

%token
  PERCENT_DEBUG         "%debug"
  PERCENT_DEFINE        "%define"
  PERCENT_DEFINES       "%defines"
  PERCENT_ERROR_VERBOSE "%error-verbose"
  PERCENT_EXPECT        "%expect"
  PERCENT_FILE_PREFIX   "%file-prefix"
  PERCENT_GLR_PARSER    "%glr-parser"
  PERCENT_LEX_PARAM     "%lex-param"
  PERCENT_LOCATIONS     "%locations"
  PERCENT_NAME_PREFIX   "%name-prefix"
  PERCENT_NO_LINES      "%no-lines"
  PERCENT_OUTPUT        "%output"
  PERCENT_PARSE_PARAM   "%parse-param"
  PERCENT_PURE_PARSER   "%pure-parser"
  PERCENT_SKELETON      "%skeleton"
  PERCENT_START         "%start"
  PERCENT_TOKEN_TABLE   "%token-table"
  PERCENT_VERBOSE       "%verbose"
  PERCENT_YACC          "%yacc"
;

%token TYPE            "type"
%token EQUAL           "="
%token SEMICOLON       ";"
%token COLON           ":"
%token PIPE            "|"
%token ID              "identifier"
%token PERCENT_PERCENT "%%"
%token PROLOGUE        "%{...%}"
%token EPILOGUE        "epilogue"
%token BRACED_CODE     "{...}"


%type <string> STRING string_content
               BRACED_CODE code_content action
               PROLOGUE EPILOGUE
%type <struniq> TYPE
%type <integer> INT
%type <symbol> ID symbol string_as_id
%type <assoc> precedence_declarator
%type <list>  symbols.1
%%

input:
  declarations "%%" grammar epilogue.opt
;


	/*------------------------------------.
	| Declarations: before the first %%.  |
	`------------------------------------*/

declarations:
  /* Nothing */
| declarations declaration semi_colon.opt
;

declaration:
  grammar_declaration
| PROLOGUE                                 { prologue_augment ($1, @1); }
| "%debug"                                 { debug_flag = 1; }
| "%define" string_content string_content  { muscle_insert ($2, $3); }
| "%defines"                               { defines_flag = 1; }
| "%error-verbose"                         { error_verbose = 1; }
| "%expect" INT                            { expected_conflicts = $2; }
| "%file-prefix" "=" string_content        { spec_file_prefix = $3; }
| "%glr-parser" 			   { glr_parser = 1; }
| "%lex-param" code_content		   { add_param ("lex_param", $2, @2); }
| "%locations"                             { locations_flag = 1; }
| "%name-prefix" "=" string_content        { spec_name_prefix = $3; }
| "%no-lines"                              { no_lines_flag = 1; }
| "%output" "=" string_content             { spec_outfile = $3; }
| "%parse-param" code_content		{ add_param ("parse_param", $2, @2); }
| "%pure-parser"                           { pure_parser = 1; }
| "%skeleton" string_content               { skeleton = $2; }
| "%token-table"                           { token_table_flag = 1; }
| "%verbose"                               { report_flag = 1; }
| "%yacc"                                  { yacc_flag = 1; }
;

grammar_declaration:
  precedence_declaration
| symbol_declaration
| "%start" symbol
    {
      grammar_start_symbol_set ($2, @2);
    }
| "%union" BRACED_CODE
    {
      typed = 1;
      MUSCLE_INSERT_INT ("stype_line", @2.first_line);
      muscle_insert ("stype", $2);
    }
| "%destructor"
    { current_braced_code = destructor_braced_code; }
  BRACED_CODE symbols.1
    {
      symbol_list_t *list;
      for (list = $4; list; list = list->next)
	symbol_destructor_set (list->sym, $3, @3);
      symbol_list_free ($4);
      current_braced_code = action_braced_code;
    }
| "%printer"
    { current_braced_code = printer_braced_code; }
  BRACED_CODE symbols.1
    {
      symbol_list_t *list;
      for (list = $4; list; list = list->next)
	symbol_printer_set (list->sym, $3, list->location);
      symbol_list_free ($4);
      current_braced_code = action_braced_code;
    }
;

symbol_declaration:
  "%nterm" { current_class = nterm_sym; } symbol_defs.1
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
| "%token" { current_class = token_sym; } symbol_defs.1
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
| "%type" TYPE symbols.1
    {
      symbol_list_t *list;
      for (list = $3; list; list = list->next)
	symbol_type_set (list->sym, $2, @2);
      symbol_list_free ($3);
    }
;

precedence_declaration:
  precedence_declarator type.opt symbols.1
    {
      symbol_list_t *list;
      ++current_prec;
      for (list = $3; list; list = list->next)
	{
	  symbol_type_set (list->sym, current_type, @2);
	  symbol_precedence_set (list->sym, current_prec, $1, @1);
	}
      symbol_list_free ($3);
      current_type = NULL;
    }
;

precedence_declarator:
  "%left"     { $$ = left_assoc; }
| "%right"    { $$ = right_assoc; }
| "%nonassoc" { $$ = non_assoc; }
;

type.opt:
  /* Nothing. */ { current_type = NULL; }
| TYPE           { current_type = $1; }
;

/* One or more nonterminals to be %typed. */

symbols.1:
  symbol            { $$ = symbol_list_new ($1, @1); }
| symbols.1 symbol  { $$ = symbol_list_prepend ($1, $2, @2); }
;

/* One token definition.  */
symbol_def:
  TYPE
     {
       current_type = $1;
     }
| ID
     {
       symbol_class_set ($1, current_class, @1);
       symbol_type_set ($1, current_type, @1);
     }
| ID INT
    {
      symbol_class_set ($1, current_class, @1);
      symbol_type_set ($1, current_type, @1);
      symbol_user_token_number_set ($1, $2, @2);
    }
| ID string_as_id
    {
      symbol_class_set ($1, current_class, @1);
      symbol_type_set ($1, current_type, @1);
      symbol_make_alias ($1, $2, @$);
    }
| ID INT string_as_id
    {
      symbol_class_set ($1, current_class, @1);
      symbol_type_set ($1, current_type, @1);
      symbol_user_token_number_set ($1, $2, @2);
      symbol_make_alias ($1, $3, @$);
    }
;

/* One or more symbol definitions. */
symbol_defs.1:
  symbol_def
    {;}
| symbol_defs.1 symbol_def
    {;}
;


	/*------------------------------------------.
	| The grammar section: between the two %%.  |
	`------------------------------------------*/

grammar:
  rules_or_grammar_declaration
| grammar rules_or_grammar_declaration
;

/* As a Bison extension, one can use the grammar declarations in the
   body of the grammar.  But to remain LALR(1), they must be ended
   with a semi-colon.  */
rules_or_grammar_declaration:
  rules
| grammar_declaration ";"
    {
      if (yacc_flag)
	complain_at (@$, _("POSIX forbids declarations in the grammar"));
    }
| error ";"
    {
      yyerrok;
    }
;

rules:
  ID ":" { current_lhs = $1; current_lhs_location = @1; } rhses.1 ";"
    {;}
;

rhses.1:
  rhs                { grammar_rule_end (@1); }
| rhses.1 "|" rhs    { grammar_rule_end (@3); }
;

rhs:
  /* Nothing.  */
    { grammar_rule_begin (current_lhs, current_lhs_location); }
| rhs symbol
    { grammar_current_rule_symbol_append ($2, @2); }
| rhs action
    { grammar_current_rule_action_append ($2, @2); }
| rhs "%prec" symbol
    { grammar_current_rule_prec_set ($3, @3); }
| rhs "%dprec" INT
    { grammar_current_rule_dprec_set ($3, @3); }
| rhs "%merge" TYPE
    { grammar_current_rule_merge_set ($3, @3); }
;

symbol:
  ID              { $$ = $1; }
| string_as_id    { $$ = $1; }
;

action:
  BRACED_CODE
   { $$ = $1; }
;

/* A string used as an ID: we have to keep the quotes. */
string_as_id:
  STRING
    {
      $$ = symbol_get ($1, @1);
      symbol_class_set ($$, token_sym, @1);
    }
;

/* A string used for its contents.  Strip the quotes. */
string_content:
  STRING
    {
      $$ = $1 + 1;
      $$[strlen ($$) - 1] = '\0';
    };


/* A BRACED_CODE used for its contents.  Strip the braces. */
code_content:
  BRACED_CODE
    {
      $$ = $1 + 1;
      $$[strlen ($$) - 1] = '\0';
    };


epilogue.opt:
  /* Nothing.  */
| "%%" EPILOGUE
    {
      epilogue_augment ($2, @2);
      scanner_last_string_free ();
    }
;

semi_colon.opt:
  /* Nothing.  */
| ";"
;
%%
static void
add_param (char const *type, char const *decl, location_t loc)
{
  static char const alphanum[] =
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "_";
  char const *alpha = alphanum + 10;
  char const *name_start = NULL;
  char const *p;

  for (p = decl; *p; p++)
    if ((p == decl || ! strchr (alphanum, p[-1])) && strchr (alpha, p[0]))
      name_start = p;

  if (! name_start)
    complain_at (loc, _("missing identifier in parameter declaration"));
  else
    {
      char *name;
      size_t name_len;

      for (name_len = 1;
	   name_start[name_len] && strchr (alphanum, name_start[name_len]);
	   name_len++)
	continue;

      name = xmalloc (name_len + 1);
      memcpy (name, name_start, name_len);
      name[name_len] = '\0';
      muscle_pair_list_grow (type, decl, name);
      free (name);
    }

  scanner_last_string_free ();
}

/*------------------------------------------------------------------.
| When debugging the parser, display tokens' locations and values.  |
`------------------------------------------------------------------*/

static void
print_token_value (FILE *file, int type, YYSTYPE const *value)
{
  fputc (' ', file);
  switch (type)
    {
    case ID:
      fprintf (file, " = %s", value->symbol->tag);
      break;

    case INT:
      fprintf (file, " = %d", value->integer);
      break;

    case STRING:
      fprintf (file, " = \"%s\"", value->string);
      break;

    case TYPE:
      fprintf (file, " = <%s>", value->struniq);
      break;

    case BRACED_CODE:
    case PROLOGUE:
    case EPILOGUE:
      fprintf (file, " = {{ %s }}", value->string);
      break;

    default:
      fprintf (file, "unknown token type");
      break;
    }
}

void
gram_error (location_t const *loc, char const *msg)
{
  complain_at (*loc, "%s", msg);
}
