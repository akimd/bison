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
#include "muscle_tab.h"
#include "files.h"
#include "getargs.h"
#include "output.h"
#include "gram.h"
#include "reader.h"
#include "conflicts.h"

/* Produce verbose parse errors.  */
#define YYERROR_VERBOSE 1

/* Pass the control structure to YYPARSE and YYLEX. */
#define YYPARSE_PARAM gram_control
#define YYLEX_PARAM gram_control
/* YYPARSE receives GRAM_CONTROL as a void *.  Provide a
   correctly typed access to it.  */
#define yycontrol ((gram_control_t *) gram_control)

/* Request detailed parse error messages, and pass them to
   GRAM_ERROR. */
#undef  yyerror
#define yyerror(Msg) \
        gram_error (yycontrol, &yylloc, Msg)

/* When debugging our pure parser, we want to see values and locations
   of the tokens.  */
#define YYPRINT(File, Type, Value) \
        yyprint (File, &yylloc, Type, &Value)
static void yyprint (FILE *file, const location_t *loc,
                     int type, const yystype *value);

symbol_class current_class = unknown_sym;
char *current_type = 0;
symbol_t *current_lhs;
associativity current_assoc;
int current_prec = 0;
%}


/* Only NUMBERS have a value.  */
%union
{
  symbol_t *symbol;
  int integer;
  char *string;
  associativity assoc;
};

/* Define the tokens together with there human representation. */
%token GRAM_EOF 0 "end of string"
%token STRING CHARACTER
%token INT

%token PERCENT_TOKEN "%token"
%token PERCENT_NTERM "%nterm"
%token PERCENT_TYPE "%type"
%token PERCENT_UNION "%union"
%token PERCENT_EXPECT "%expect"
%token PERCENT_START "%start"
%token PERCENT_LEFT "%left"
%token PERCENT_RIGHT "%right"
%token PERCENT_NONASSOC "%nonassoc"
%token PERCENT_PREC     "%prec"
%token PERCENT_VERBOSE  "%verbose"
%token PERCENT_ERROR_VERBOSE "%error-verbose"

%token PERCENT_OUTPUT "%output"
%token PERCENT_FILE_PREFIX "%file-prefix"
%token PERCENT_NAME_PREFIX "%name-prefix"

%token PERCENT_DEFINE "%define"
%token PERCENT_PURE_PARSER "%pure-parser"

%token PERCENT_DEFINES "%defines"

%token PERCENT_YACC "%yacc"

%token PERCENT_DEBUG "%debug"
%token PERCENT_LOCATIONS "%locations"
%token PERCENT_NO_LINES "%no-lines"
%token PERCENT_SKELETON "%skeleton"
%token PERCENT_TOKEN_TABLE "%token-table"

%token TYPE
%token EQUAL "="
%token SEMICOLON ";"
%token COLON ":"
%token PIPE "|"
%token ID "identifier"
%token PERCENT_PERCENT "%%"
%token PROLOGUE EPILOGUE
%token BRACED_CODE

%type <string> CHARACTER TYPE  STRING string_content
               BRACED_CODE PROLOGUE EPILOGUE epilogue.opt action
%type <integer> INT
%type <symbol> ID symbol string_as_id
%type <assoc> precedence_directive
%%
input: { LOCATION_RESET (yylloc); }
  directives "%%" gram epilogue.opt
    {
      yycontrol->errcode = 0;
      epilogue_set ($5, @5);
    }
;

directives:
  /* Nothing */
| directives directive
;

directive:
  grammar_directives
| PROLOGUE
   {
     prologue_augment ($1, @1);
   }
| "%debug"                                 { debug_flag = 1; }
| "%define" string_content string_content  { muscle_insert ($2, $3); }
| "%defines"                               { defines_flag = 1; }
| "%error-verbose"                         { error_verbose = 1; }
| "%expect" INT                            { expected_conflicts = $2; }
| "%file-prefix" "=" string_content        { spec_file_prefix = $3; }
| "%locations"                             { locations_flag = 1; }
| "%name-prefix" "=" string_content        { spec_name_prefix = $3; }
| "%no-lines"                              { no_lines_flag = 1; }
| "%output" "=" string_content             { spec_outfile = $3; }
| "%pure-parser"                           { pure_parser = 1; }
| "%skeleton" string_content               { skeleton = $2; }
| "%token-table"                           { token_table_flag = 1; }
| "%verbose"                               { report_flag = 1; }
| "%yacc"                                  { yacc_flag = 1; }
;

grammar_directives:
  precedence_directives
| "%nterm" { current_class = nterm_sym; } symbol_defs.1
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
| "%start" symbol
    {
      grammar_start_symbol_set ($2);
    }
| "%token" { current_class = token_sym; } symbol_defs.1
    {
      current_class = unknown_sym;
      current_type = NULL;
    }
| "%type" TYPE {current_type = $2; } nterms_to_type.1
    {
      current_type = NULL;
    }
| "%union" BRACED_CODE semi_colon_opt
    {
      typed = 1;
      MUSCLE_INSERT_INT ("stype_line", @2.first_line);
      muscle_insert ("stype", $2);
    }
;

precedence_directives:
  precedence_directive type.opt
    { current_assoc = $1; ++current_prec; }
  terms_to_prec.1
    { current_assoc = non_assoc; current_type = NULL; }
;

precedence_directive:
  "%left"     { $$ = left_assoc; }
| "%right"    { $$ = right_assoc; }
| "%nonassoc" { $$ = non_assoc; }
;

type.opt:
  /* Nothing. */ { current_type = NULL;}
| TYPE           { current_type = $1; }
;

/* One or more nonterminals to be %typed. */
nterms_to_type.1:
  ID                   { symbol_type_set ($1, current_type); }
| nterms_to_type.1 ID  { symbol_type_set ($2, current_type); }
;

/* One or more symbols to be given a precedence/associativity.  */
terms_to_prec.1:
  symbol
    {
      symbol_type_set ($1, current_type);
      symbol_precedence_set ($1, current_prec, current_assoc);
    }
| terms_to_prec.1 symbol
    {
      symbol_type_set ($2, current_type);
      symbol_precedence_set ($2, current_prec, current_assoc);
    }
;


/* One token definition.  */
symbol_def:
  TYPE
     {
       current_type = $1;
     }
| ID
     {
       symbol_class_set ($1, current_class);
       symbol_type_set ($1, current_type);
     }
| ID INT
    {
      symbol_class_set ($1, current_class);
      symbol_type_set ($1, current_type);
      symbol_user_token_number_set ($1, $2);
    }
| ID string_as_id
    {
      symbol_class_set ($1, current_class);
      symbol_type_set ($1, current_type);
      symbol_make_alias ($1, $2);
    }
| ID INT string_as_id
    {
      symbol_class_set ($1, current_class);
      symbol_type_set ($1, current_type);
      symbol_user_token_number_set ($1, $2);
      symbol_make_alias ($1, $3);
    }
;

/* One or more symbol definitions. */
symbol_defs.1:
  symbol_def
    {;}
| symbol_defs.1 symbol_def
    {;}
;

gram:
  rules
| gram rules
;

rules:
  ID ":" { current_lhs = $1; } rhses.1 ";"
    {;}
;

rhses.1:
  rhs                { grammar_rule_end (); }
| rhses.1 "|" rhs    { grammar_rule_end (); }
;

rhs:
  /* Nothing.  */
    { grammar_rule_begin (current_lhs); }
| rhs symbol
    { grammar_current_rule_symbol_append ($2); }
| rhs action
    { grammar_current_rule_action_append ($2, @2.first_line); }
| rhs "%prec" symbol
    { grammar_current_rule_prec_set ($3); }
;

symbol:
  ID              { $$ = $1; }
| string_as_id    { $$ = $1; }
| CHARACTER       { $$ = getsym ($1); }
;

action:
  BRACED_CODE
   { $$ = $1; }
;

/* A string used as an ID: we have to keep the quotes. */
string_as_id:
  STRING
    {
      $$ = getsym ($1);
      symbol_class_set ($$, token_sym);
    }
;

/* A string used for its contents.  Strip the quotes. */
string_content:
  STRING
    {
      $$ = $1 + 1;
      $$[strlen ($$) - 1] = '\0';
    };


epilogue.opt:
  /* Nothing.  */
    {
      $$ = xstrdup ("");
    }
| "%%" EPILOGUE
    {
      $$ = $2;
    }
;

semi_colon_opt:
  /* Nothing.  */
| ";"
;
%%
/*------------------------------------------------------------------.
| When debugging the parser, display tokens' locations and values.  |
`------------------------------------------------------------------*/

static void
yyprint (FILE *file,
         const location_t *loc, int type, const yystype *value)
{
  fputs (" (", file);
  LOCATION_PRINT (file, *loc);
  fputs (")", file);
  switch (type)
    {
    case CHARACTER:
      fprintf (file, " = '%s'", value->string);
      break;

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
      fprintf (file, " = <%s>", value->string);
      break;

    case BRACED_CODE:
    case PROLOGUE:
    case EPILOGUE:
      fprintf (file, " = {{ %s }}", value->string);
      break;
    }
}

void
gram_error (gram_control_t *control ATTRIBUTE_UNUSED,
	    location_t *yylloc, const char *msg)
{
  LOCATION_PRINT (stderr, *yylloc);
  fprintf (stderr, ": %s\n", msg);
}
