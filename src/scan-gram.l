/* Bison Grammar Scanner                             -*- C -*-
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

%option debug nodefault noyywrap nounput never-interactive stack
%option prefix="gram_" outfile="lex.yy.c"

%{
#include "system.h"
#include "complain.h"
#include "quote.h"
#include "getargs.h"
#include "gram.h"
#include "reader.h"

/* Each time we match a string, move the end cursor to its end. */
#define YY_USER_INIT				\
do {						\
  LOCATION_RESET (*yylloc);			\
  yylloc->file = infile;			\
   /* This is only to avoid GCC warnings. */	\
  if (yycontrol) {;};				\
} while (0)

#define YY_USER_ACTION  LOCATION_COLUMNS (*yylloc, yyleng);
#define YY_LINES        LOCATION_LINES (*yylloc, yyleng);
#define YY_STEP         LOCATION_STEP (*yylloc);

/* STRING_OBSTACK -- Used to store all the characters that we need to
   keep (to construct ID, STRINGS etc.).  Use the following macros to
   use it.

   Use YY_OBS_GROW to append what has just been matched, and
   YY_OBS_FINISH to end the string (it puts the ending 0).
   YY_OBS_FINISH also stores this string in LAST_STRING, which can be
   used, and which is used by YY_OBS_FREE to free the last string.  */

static struct obstack string_obstack;
char *last_string;

#define YY_OBS_GROW   \
  obstack_grow (&string_obstack, yytext, yyleng)

#define YY_OBS_FINISH					\
  do {							\
    obstack_1grow (&string_obstack, '\0');		\
    last_string = obstack_finish (&string_obstack);	\
  } while (0)

#define YY_OBS_FREE						\
  do {								\
    obstack_free (&string_obstack, last_string);		\
  } while (0)

void
scanner_last_string_free (void)
{
  YY_OBS_FREE;
}


static int braces_level = 0;
static int percent_percent_count = 0;

/* Within well-formed rules, RULE_LENGTH is the number of values in
   the current rule so far, which says where to find `$0' with respect
   to the top of the stack.  It is not the same as the rule->length in
   the case of mid rule actions.

   Outside of well-formed rules, RULE_LENGTH has an undefined value.  */
static int rule_length;

static void handle_dollar PARAMS ((braced_code_t code_kind,
				   char *cp, location_t location));
static void handle_at PARAMS ((braced_code_t code_kind,
			       char *cp, location_t location));

%}
%x SC_COMMENT
%x SC_STRING SC_CHARACTER
%x SC_ESCAPED_STRING SC_ESCAPED_CHARACTER
%x SC_BRACED_CODE SC_PROLOGUE SC_EPILOGUE

id	 [.a-zA-Z_][.a-zA-Z_0-9]*
int 	 [0-9]+
eols     (\n|\r|\n\r|\r\n)+
blanks   [ \t\f]+

%%
%{
  /* At each yylex invocation, mark the current position as the
     start of the next token.  */
#define TR_POS 0
#if TR_POS
  fprintf (stderr, "FOO1: %p: ", yylloc);
  LOCATION_PRINT (stderr, *yylloc);
  fprintf (stderr, "\n");
#endif
  YY_STEP;
#if TR_POS
  fprintf (stderr, "BAR1: ");
  LOCATION_PRINT (stderr, *yylloc);
  fprintf (stderr, "\n");
#endif
%}


  /*----------------------------.
  | Scanning Bison directives.  |
  `----------------------------*/
<INITIAL>
{
  "%binary"               return PERCENT_NONASSOC;
  "%debug"                return PERCENT_DEBUG;
  "%define"               return PERCENT_DEFINE;
  "%defines"              return PERCENT_DEFINES;
  "%destructor"           return PERCENT_DESTRUCTOR;
  "%dprec"		  return PERCENT_DPREC;
  "%error"[-_]"verbose"   return PERCENT_ERROR_VERBOSE;
  "%expect"               return PERCENT_EXPECT;
  "%file-prefix"          return PERCENT_FILE_PREFIX;
  "%fixed"[-_]"output"[-_]"files"   return PERCENT_YACC;
  "%glr"[-_]"parser"	  return PERCENT_GLR_PARSER;
  "%left"                 return PERCENT_LEFT;
  "%locations"            return PERCENT_LOCATIONS;
  "%merge"		  return PERCENT_MERGE;
  "%name"[-_]"prefix"     return PERCENT_NAME_PREFIX;
  "%no"[-_]"lines"        return PERCENT_NO_LINES;
  "%nonassoc"             return PERCENT_NONASSOC;
  "%nterm"                return PERCENT_NTERM;
  "%output"               return PERCENT_OUTPUT;
  "%prec"                 { rule_length--; return PERCENT_PREC; }
  "%printer"              return PERCENT_PRINTER;
  "%pure"[-_]"parser"     return PERCENT_PURE_PARSER;
  "%right"                return PERCENT_RIGHT;
  "%skeleton"             return PERCENT_SKELETON;
  "%start"                return PERCENT_START;
  "%term"                 return PERCENT_TOKEN;
  "%token"                return PERCENT_TOKEN;
  "%token"[-_]"table"     return PERCENT_TOKEN_TABLE;
  "%type"                 return PERCENT_TYPE;
  "%union"                return PERCENT_UNION;
  "%verbose"              return PERCENT_VERBOSE;
  "%yacc"                 return PERCENT_YACC;

  "="                     return EQUAL;
  ":"                     { rule_length = 0; return COLON; }
  "|"                     { rule_length = 0; return PIPE; }
  ";"                     return SEMICOLON;

  {eols}      YY_LINES; YY_STEP;
  {blanks}    YY_STEP;
  {id}        {
    yylval->symbol = symbol_get (yytext, *yylloc);
    rule_length++;
    return ID;
  }

  {int}       yylval->integer = strtol (yytext, 0, 10); return INT;

  /* Characters.  We don't check there is only one.  */
  "'"         YY_OBS_GROW; yy_push_state (SC_ESCAPED_CHARACTER);

  /* Strings. */
  "\""        YY_OBS_GROW; yy_push_state (SC_ESCAPED_STRING);

  /* Comments. */
  "/*"        yy_push_state (SC_COMMENT);
  "//".*      YY_STEP;

  /* Prologue. */
  "%{"        yy_push_state (SC_PROLOGUE);

  /* Code in between braces.  */
  "{"         YY_OBS_GROW; ++braces_level; yy_push_state (SC_BRACED_CODE);

  /* A type. */
  "<"[^>]+">" {
    obstack_grow (&string_obstack, yytext + 1, yyleng - 2);
    YY_OBS_FINISH;
    yylval->string = last_string;
    return TYPE;
  }


  "%%"   {
    if (++percent_percent_count == 2)
      yy_push_state (SC_EPILOGUE);
    return PERCENT_PERCENT;
  }

  .           {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": invalid character: `%c'\n"), *yytext);
    YY_STEP;
  }
}


  /*------------------------------------------------------------.
  | Whatever the start condition (but those which correspond to |
  | entity `swallowed' by Bison: SC_ESCAPED_STRING and          |
  | SC_ESCAPED_CHARACTER), no M4 character must escape as is.   |
  `------------------------------------------------------------*/

<SC_COMMENT,SC_STRING,SC_CHARACTER,SC_BRACED_CODE,SC_PROLOGUE,SC_EPILOGUE>
{
  \[          if (YY_START != SC_COMMENT) obstack_sgrow (&string_obstack, "@<:@");
  \]          if (YY_START != SC_COMMENT) obstack_sgrow (&string_obstack, "@:>@");
}



  /*-----------------------------------------------------------.
  | Scanning a C comment. The initial `/ *' is already eaten.  |
  `-----------------------------------------------------------*/

<SC_COMMENT>
{
  "*/" { /* End of the comment. */
    if (yy_top_state () == INITIAL)
      {
	YY_STEP;
      }
    else
      {
	YY_OBS_GROW;
      }
    yy_pop_state ();
  }

  [^\[\]*\n\r]+  	if (yy_top_state () != INITIAL) YY_OBS_GROW;
  {eols}	if (yy_top_state () != INITIAL) YY_OBS_GROW; YY_LINES;
  .             /* Stray `*'. */if (yy_top_state () != INITIAL) YY_OBS_GROW;

  <<EOF>> {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unexpected end of file in a comment\n"));
    yy_pop_state ();
  }
}


  /*----------------------------------------------------------------.
  | Scanning a C string, including its escapes.  The initial `"' is |
  | already eaten.                                                  |
  `----------------------------------------------------------------*/

<SC_ESCAPED_STRING>
{
  "\"" {
    assert (yy_top_state () == INITIAL);
    YY_OBS_GROW;
    YY_OBS_FINISH;
    yylval->string = last_string;
    yy_pop_state ();
    rule_length++;
    return STRING;
  }

  [^\"\n\r\\]+      YY_OBS_GROW;

  {eols}    obstack_1grow (&string_obstack, '\n'); YY_LINES;

  <<EOF>> {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unexpected end of file in a string\n"));
    assert (yy_top_state () == INITIAL);
    YY_OBS_FINISH;
    yylval->string = last_string;
    yy_pop_state ();
    return STRING;
  }
}

  /*---------------------------------------------------------------.
  | Scanning a C character, decoding its escapes.  The initial "'" |
  | is already eaten.                                              |
  `---------------------------------------------------------------*/

<SC_ESCAPED_CHARACTER>
{
  "'" {
    YY_OBS_GROW;
    assert (yy_top_state () == INITIAL);
    {
      YY_OBS_FINISH;
      yylval->symbol = symbol_get (last_string, *yylloc);
      symbol_class_set (yylval->symbol, token_sym, *yylloc);
      symbol_user_token_number_set (yylval->symbol,
				    (unsigned char) last_string[1], *yylloc);
      YY_OBS_FREE;
      yy_pop_state ();
      rule_length++;
      return ID;
    }
  }

  [^\n\r\\] YY_OBS_GROW;

  {eols}    obstack_1grow (&string_obstack, '\n'); YY_LINES;

  <<EOF>> {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unexpected end of file in a character\n"));
    assert (yy_top_state () == INITIAL);
    YY_OBS_FINISH;
    yylval->string = last_string;
    yy_pop_state ();
    return CHARACTER;
  }
}


  /*----------------------------.
  | Decode escaped characters.  |
  `----------------------------*/

<SC_ESCAPED_STRING,SC_ESCAPED_CHARACTER>
{
  \\[0-7]{3}		{
    long c = strtol (yytext + 1, 0, 8);
    if (c > 255)
      {
	LOCATION_PRINT (stderr, *yylloc);
	fprintf (stderr, _(": invalid escape: %s\n"), quote (yytext));
	YY_STEP;
      }
    else
      obstack_1grow (&string_obstack, c);
  }

  \\x[0-9a-fA-F]{2}	{
    obstack_1grow (&string_obstack, strtol (yytext + 2, 0, 16));
  }

  \\a	obstack_1grow (&string_obstack, '\a');
  \\b	obstack_1grow (&string_obstack, '\b');
  \\f	obstack_1grow (&string_obstack, '\f');
  \\n	obstack_1grow (&string_obstack, '\n');
  \\r	obstack_1grow (&string_obstack, '\r');
  \\t	obstack_1grow (&string_obstack, '\t');
  \\v	obstack_1grow (&string_obstack, '\v');
  \\[\\""'']   obstack_1grow (&string_obstack, yytext[1]);
  \\(.|\n)	{
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unrecognized escape: %s\n"), quote (yytext));
    YY_OBS_GROW;
  }
  /* FLex wants this rule, in case of a `\<<EOF>>'. */
  \\                   YY_OBS_GROW;
}


  /*----------------------------------------------------------.
  | Scanning a C character without decoding its escapes.  The |
  | initial "'" is already eaten.                             |
  `----------------------------------------------------------*/

<SC_CHARACTER>
{
  "'" {
    YY_OBS_GROW;
    assert (yy_top_state () != INITIAL);
    yy_pop_state ();
  }

  [^\[\]\'\n\r\\]+     YY_OBS_GROW;
  \\(.|\n)             YY_OBS_GROW;
  /* FLex wants this rule, in case of a `\<<EOF>>'. */
  \\                   YY_OBS_GROW;

  {eols}               YY_OBS_GROW; YY_LINES;

  <<EOF>> {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unexpected end of file in a character\n"));
    assert (yy_top_state () != INITIAL);
    yy_pop_state ();
  }
}


  /*----------------------------------------------------------------.
  | Scanning a C string, without decoding its escapes.  The initial |
  | `"' is already eaten.                                           |
  `----------------------------------------------------------------*/

<SC_STRING>
{
  "\"" {
    assert (yy_top_state () != INITIAL);
    YY_OBS_GROW;
    yy_pop_state ();
  }

  [^\[\]\"\n\r\\]+      YY_OBS_GROW;
  \\(.|\n)              YY_OBS_GROW;
  /* FLex wants this rule, in case of a `\<<EOF>>'. */
  \\                   YY_OBS_GROW;

  {eols}                YY_OBS_GROW; YY_LINES;

  <<EOF>> {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unexpected end of file in a string\n"));
    assert (yy_top_state () != INITIAL);
    yy_pop_state ();
  }
}


  /*---------------------------------------------------.
  | Strings, comments etc. can be found in user code.  |
  `---------------------------------------------------*/

<SC_BRACED_CODE,SC_PROLOGUE,SC_EPILOGUE>
{
  /* Characters.  We don't check there is only one.  */
  "'"         YY_OBS_GROW; yy_push_state (SC_CHARACTER);

  /* Strings. */
  "\""        YY_OBS_GROW; yy_push_state (SC_STRING);

  /* Comments. */
  "/*"        YY_OBS_GROW; yy_push_state (SC_COMMENT);
  "//".*      YY_OBS_GROW;

  /* Not comments. */
  "/"         YY_OBS_GROW;
}


  /*---------------------------------------------------------------.
  | Scanning some code in braces (%union and actions). The initial |
  | "{" is already eaten.                                          |
  `---------------------------------------------------------------*/

<SC_BRACED_CODE>
{
  "}" {
    YY_OBS_GROW;
    if (--braces_level == 0)
      {
	yy_pop_state ();
	YY_OBS_FINISH;
	yylval->string = last_string;
	rule_length++;
	return BRACED_CODE;
      }
  }

  "{"  	        YY_OBS_GROW; braces_level++;

  "$"("<"[^>]+">")?(-?[0-9]+|"$") { handle_dollar (current_braced_code,
						   yytext, *yylloc); }
  "@"(-?[0-9]+|"$")               { handle_at (current_braced_code,
					       yytext, *yylloc); }

  [^$@\[\]/\'\"\{\}\n\r]+ YY_OBS_GROW;
  {eols}	YY_OBS_GROW; YY_LINES;

  /* A lose $, or /, or etc. */
  .             YY_OBS_GROW;

  <<EOF>> {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unexpected end of file in a braced code\n"));
    yy_pop_state ();
    YY_OBS_FINISH;
    yylval->string = last_string;
    return BRACED_CODE;
  }

}


  /*--------------------------------------------------------------.
  | Scanning some prologue: from "%{" (already scanned) to "%}".  |
  `--------------------------------------------------------------*/

<SC_PROLOGUE>
{
  "%}" {
    yy_pop_state ();
    YY_OBS_FINISH;
    yylval->string = last_string;
    return PROLOGUE;
  }

  [^%\[\]/\'\"\n\r]+ YY_OBS_GROW;
  "%"                YY_OBS_GROW;
  {eols}	     YY_OBS_GROW; YY_LINES;

  <<EOF>> {
    LOCATION_PRINT (stderr, *yylloc);
    fprintf (stderr, _(": unexpected end of file in a prologue\n"));
    yy_pop_state ();
    YY_OBS_FINISH;
    yylval->string = last_string;
    return PROLOGUE;
  }
}


  /*---------------------------------------------------------------.
  | Scanning the epilogue (everything after the second "%%", which |
  | has already been eaten.                                        |
  `---------------------------------------------------------------*/

<SC_EPILOGUE>
{
  ([^\[\]]|{eols})+  YY_OBS_GROW;

  <<EOF>> {
    yy_pop_state ();
    YY_OBS_FINISH;
    yylval->string = last_string;
    return EPILOGUE;
  }
}


%%

/*------------------------------------------------------------------.
| TEXT is pointing to a wannabee semantic value (i.e., a `$').      |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|integer)                         |
|                                                                   |
| Output to the STRING_OBSTACK a reference to this semantic value.  |
`------------------------------------------------------------------*/

static inline void
handle_action_dollar (char *text, location_t location)
{
  const char *type_name = NULL;
  char *cp = text + 1;

  /* Get the type name if explicit. */
  if (*cp == '<')
    {
      type_name = ++cp;
      while (*cp != '>')
	++cp;
      *cp = '\0';
      ++cp;
    }

  if (*cp == '$')
    {
      if (!type_name)
	type_name = symbol_list_n_type_name_get (current_rule, location, 0);
      if (!type_name && typed)
	complain_at (location, _("$$ of `%s' has no declared type"),
		     current_rule->sym->tag);
      if (!type_name)
	type_name = "";
      obstack_fgrow1 (&string_obstack,
		      "]b4_lhs_value([%s])[", type_name);
    }
  else if (('0' <= *cp && *cp <= '9') || *cp == '-')
    {
      int n = strtol (cp, &cp, 10);

      if (n > rule_length)
	complain_at (location, _("invalid value: %s%d"), "$", n);
      else
	{
	  if (!type_name && n > 0)
	    type_name = symbol_list_n_type_name_get (current_rule, location,
						     n);
	  if (!type_name && typed)
	    complain_at (location, _("$%d of `%s' has no declared type"),
		      n, current_rule->sym->tag);
	  if (!type_name)
	    type_name = "";
	  obstack_fgrow3 (&string_obstack,
			  "]b4_rhs_value([%d], [%d], [%s])[",
			  rule_length, n, type_name);
	}
    }
  else
    {
      complain_at (location, _("%s is invalid"), quote (text));
    }
}


/*---------------------------------------------------------------.
| TEXT is expexted tp be $$ in some code associated to a symbol: |
| destructor or printer.                                         |
`---------------------------------------------------------------*/

static inline void
handle_symbol_code_dollar (char *text, location_t location)
{
  char *cp = text + 1;
  if (*cp == '$')
    obstack_sgrow (&string_obstack, "]b4_dollar_dollar[");
  else
    complain_at (location, _("%s is invalid"), quote (text));
}


/*-----------------------------------------------------------------.
| Dispatch onto handle_action_dollar, or handle_destructor_dollar, |
| depending upon CODE_KIND.                                        |
`-----------------------------------------------------------------*/

static void
handle_dollar (braced_code_t braced_code_kind,
	       char *text, location_t location)
{
  switch (braced_code_kind)
    {
    case action_braced_code:
      handle_action_dollar (text, location);
      break;

    case destructor_braced_code:
    case printer_braced_code:
      handle_symbol_code_dollar (text, location);
      break;
    }
}


/*------------------------------------------------------.
| TEXT is a location token (i.e., a `@...').  Output to |
| STRING_OBSTACK a reference to this location.          |
`------------------------------------------------------*/

static inline void
handle_action_at (char *text, location_t location)
{
  char *cp = text + 1;
  locations_flag = 1;

  if (*cp == '$')
    {
      obstack_sgrow (&string_obstack, "]b4_lhs_location[");
    }
  else if (('0' <= *cp && *cp <= '9') || *cp == '-')
    {
      int n = strtol (cp, &cp, 10);

      if (n > rule_length)
	complain_at (location, _("invalid value: %s%d"), "@", n);
      else
	obstack_fgrow2 (&string_obstack, "]b4_rhs_location([%d], [%d])[",
			rule_length, n);
    }
  else
    {
      complain_at (location, _("%s is invalid"), quote (text));
    }
}


/*---------------------------------------------------------------.
| TEXT is expexted tp be @$ in some code associated to a symbol: |
| destructor or printer.                                         |
`---------------------------------------------------------------*/

static inline void
handle_symbol_code_at (char *text, location_t location)
{
  char *cp = text + 1;
  if (*cp == '$')
    obstack_sgrow (&string_obstack, "]b4_at_dollar[");
  else
    complain_at (location, _("%s is invalid"), quote (text));
}


/*-------------------------------------------------------------------.
| Dispatch onto handle_action_at, or handle_destructor_at, depending |
| upon CODE_KIND.                                                    |
`-------------------------------------------------------------------*/

static void
handle_at (braced_code_t braced_code_kind,
	   char *text, location_t location)
{
  switch (braced_code_kind)
    {
    case action_braced_code:
      handle_action_at (text, location);
      break;

    case destructor_braced_code:
    case printer_braced_code:
      handle_symbol_code_at (text, location);
      break;
    }
}


/*-------------------------.
| Initialize the scanner.  |
`-------------------------*/

void
scanner_initialize (void)
{
  obstack_init (&string_obstack);
}


/*-----------------------------------------------.
| Free all the memory allocated to the scanner.  |
`-----------------------------------------------*/

void
scanner_free (void)
{
  obstack_free (&string_obstack, 0);
  /* Reclaim Flex's buffers.  */
  yy_delete_buffer (YY_CURRENT_BUFFER);
}
