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

%option debug nodefault noyywrap never-interactive
%option prefix="gram_" outfile="lex.yy.c"

%{
#include "system.h"
#include "mbswidth.h"
#include "complain.h"
#include "quote.h"
#include "struniq.h"
#include "getargs.h"
#include "gram.h"
#include "reader.h"

/* Each time we match a string, move the end cursor to its end. */
#define YY_USER_INIT				\
do {						\
  LOCATION_RESET (*yylloc);			\
  yylloc->file = current_file;			\
   /* This is only to avoid GCC warnings. */	\
  if (yycontrol) {;};				\
} while (0)

#define YY_USER_ACTION  extend_location (yylloc, yytext, yyleng);
#define YY_STEP         LOCATION_STEP (*yylloc)

#define YY_INPUT(buf, result, size) ((result) = no_cr_read (yyin, buf, size))


/* Read bytes from FP into buffer BUF of size SIZE.  Return the
   number of bytes read.  Remove '\r' from input, treating \r\n
   and isolated \r as \n.  */

static size_t
no_cr_read (FILE *fp, char *buf, size_t size)
{
  size_t s = fread (buf, 1, size, fp);
  if (s)
    {
      char *w = memchr (buf, '\r', s);
      if (w)
	{
	  char const *r = ++w;
	  char const *lim = buf + s;

	  for (;;)
	    {
	      /* Found an '\r'.  Treat it like '\n', but ignore any
		 '\n' that immediately follows.  */
	      w[-1] = '\n';
	      if (r == lim)
		{
		  int ch = getc (fp);
		  if (ch != '\n' && ungetc (ch, fp) != ch)
		    break;
		}
	      else if (*r == '\n')
		r++;

	      /* Copy until the next '\r'.  */
	      do
		{
		  if (r == lim)
		    return w - buf;
		}
	      while ((*w++ = *r++) != '\r');
	    }

	  return w - buf;
	}
    }

  return s;
}


/* Extend *LOC to account for token TOKEN of size SIZE.  */

static void
extend_location (location_t *loc, char const *token, int size)
{
  int line = loc->last_line;
  int column = loc->last_column;
  char const *p0 = token;
  char const *p = token;
  char const *lim = token + size;

  for (p = token; p < lim; p++)
    switch (*p)
      {
      case '\r':
	/* \r shouldn't survive no_cr_read.  */
	abort ();

      case '\n':
	line++;
	column = 1;
	p0 = p + 1;
	break;

      case '\t':
	column += mbsnwidth (p0, p - p0, 0);
	column += 8 - ((column - 1) & 7);
	p0 = p + 1;
	break;
      }

  loc->last_line = line;
  loc->last_column = column + mbsnwidth (p0, p - p0, 0);
}



/* STRING_OBSTACK -- Used to store all the characters that we need to
   keep (to construct ID, STRINGS etc.).  Use the following macros to
   use it.

   Use YY_OBS_GROW to append what has just been matched, and
   YY_OBS_FINISH to end the string (it puts the ending 0).
   YY_OBS_FINISH also stores this string in LAST_STRING, which can be
   used, and which is used by YY_OBS_FREE to free the last string.  */

static struct obstack string_obstack;

/* A string representing the most recently saved token.  */
static char *last_string;


#define YY_OBS_GROW   \
  obstack_grow (&string_obstack, yytext, yyleng)

#define YY_OBS_FINISH					\
  do {							\
    obstack_1grow (&string_obstack, '\0');		\
    last_string = obstack_finish (&string_obstack);	\
  } while (0)

#define YY_OBS_FREE \
  obstack_free (&string_obstack, last_string)

void
scanner_last_string_free (void)
{
  YY_OBS_FREE;
}

/* Within well-formed rules, RULE_LENGTH is the number of values in
   the current rule so far, which says where to find `$0' with respect
   to the top of the stack.  It is not the same as the rule->length in
   the case of mid rule actions.

   Outside of well-formed rules, RULE_LENGTH has an undefined value.  */
static int rule_length;

static void handle_dollar (braced_code_t code_kind,
			   char *cp, location_t location);
static void handle_at (braced_code_t code_kind,
		       char *cp, location_t location);
static void handle_syncline (char *args, location_t *location);
static int convert_ucn_to_byte (char const *hex_text);
static void unexpected_end_of_file (location_t *, char const *);

%}
%x SC_COMMENT SC_LINE_COMMENT SC_YACC_COMMENT
%x SC_STRING SC_CHARACTER
%x SC_ESCAPED_STRING SC_ESCAPED_CHARACTER
%x SC_BRACED_CODE SC_PROLOGUE SC_EPILOGUE

letter	  [.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_]
id	  {letter}({letter}|[0-9])*
directive %{letter}({letter}|[0-9]|-)*
int 	  [0-9]+

/* POSIX says that a tag must be both an id and a C union member, but
   historically almost any character is allowed in a tag.  We disallow
   NUL and newline, as this simplifies our implementation.  */
tag	 [^\0\n>]+

/* Zero or more instances of backslash-newline.  Following GCC, allow
   white space between the backslash and the newline.  */
splice	 (\\[ \f\t\v]*\n)*

%%
%{
  /* Nesting level of the current code in braces.  */
  int braces_level IF_LINT (= 0);

  /* Scanner context when scanning C code.  */
  int c_context IF_LINT (= 0);

  /* At each yylex invocation, mark the current position as the
     start of the next token.  */
  YY_STEP;
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
  "%glr-parser"           return PERCENT_GLR_PARSER;
  "%left"                 return PERCENT_LEFT;
  "%locations"            return PERCENT_LOCATIONS;
  "%merge"		  return PERCENT_MERGE;
  "%name"[-_]"prefix"     return PERCENT_NAME_PREFIX;
  "%no"[-_]"lines"        return PERCENT_NO_LINES;
  "%nonassoc"             return PERCENT_NONASSOC;
  "%nterm"                return PERCENT_NTERM;
  "%output"               return PERCENT_OUTPUT;
  "%parse-param"          return PERCENT_PARSE_PARAM;
  "%prec"                 rule_length--; return PERCENT_PREC;
  "%printer"              return PERCENT_PRINTER;
  "%pure"[-_]"parser"     return PERCENT_PURE_PARSER;
  "%right"                return PERCENT_RIGHT;
  "%lex-param"            return PERCENT_LEX_PARAM;
  "%skeleton"             return PERCENT_SKELETON;
  "%start"                return PERCENT_START;
  "%term"                 return PERCENT_TOKEN;
  "%token"                return PERCENT_TOKEN;
  "%token"[-_]"table"     return PERCENT_TOKEN_TABLE;
  "%type"                 return PERCENT_TYPE;
  "%union"                return PERCENT_UNION;
  "%verbose"              return PERCENT_VERBOSE;
  "%yacc"                 return PERCENT_YACC;

  {directive}             {
    complain_at (*yylloc, _("invalid directive: %s"), quote (yytext));
    YY_STEP;
  }

  ^"#line "{int}" \"".*"\"\n" {
    handle_syncline (yytext + sizeof "#line " - 1, yylloc);
    YY_STEP;
  }

  "="                     return EQUAL;
  ":"                     rule_length = 0; return COLON;
  "|"                     rule_length = 0; return PIPE;
  ","                     return COMMA;
  ";"                     return SEMICOLON;

  [ \f\n\t\v]  YY_STEP;

  {id}        {
    yylval->symbol = symbol_get (yytext, *yylloc);
    rule_length++;
    return ID;
  }

  {int} {
    unsigned long num;
    errno = 0;
    num = strtoul (yytext, 0, 10);
    if (INT_MAX < num || errno)
      {
	complain_at (*yylloc, _("integer out of range: %s"), quote (yytext));
	num = INT_MAX;
      }
    yylval->integer = num;
    return INT;
  }

  /* Characters.  We don't check there is only one.  */
  "'"         YY_OBS_GROW; BEGIN SC_ESCAPED_CHARACTER;

  /* Strings. */
  "\""        YY_OBS_GROW; BEGIN SC_ESCAPED_STRING;

  /* Comments. */
  "/*"        BEGIN SC_YACC_COMMENT;
  "//".*      YY_STEP;

  /* Prologue. */
  "%{"        BEGIN SC_PROLOGUE;

  /* Code in between braces.  */
  "{"         YY_OBS_GROW; braces_level = 0; BEGIN SC_BRACED_CODE;

  /* A type. */
  "<"{tag}">" {
    obstack_grow (&string_obstack, yytext + 1, yyleng - 2);
    YY_OBS_FINISH;
    yylval->struniq = struniq_new (last_string);
    YY_OBS_FREE;
    return TYPE;
  }

  "%%" {
    static int percent_percent_count;
    if (++percent_percent_count == 2)
      BEGIN SC_EPILOGUE;
    return PERCENT_PERCENT;
  }

  . {
    complain_at (*yylloc, _("invalid character: %s"), quote (yytext));
    YY_STEP;
  }
}


  /*---------------------------------------------------------------.
  | Scanning a Yacc comment.  The initial `/ *' is already eaten.  |
  `---------------------------------------------------------------*/

<SC_YACC_COMMENT>
{
  "*/" {
    YY_STEP;
    BEGIN INITIAL;
  }

  .|\n	   ;
  <<EOF>>  unexpected_end_of_file (yylloc, "*/");
}


  /*------------------------------------------------------------.
  | Scanning a C comment.  The initial `/ *' is already eaten.  |
  `------------------------------------------------------------*/

<SC_COMMENT>
{
  "*"{splice}"/"  YY_OBS_GROW; BEGIN c_context;
  <<EOF>>	  unexpected_end_of_file (yylloc, "*/");
}


  /*--------------------------------------------------------------.
  | Scanning a line comment.  The initial `//' is already eaten.  |
  `--------------------------------------------------------------*/

<SC_LINE_COMMENT>
{
  "\n"		 YY_OBS_GROW; BEGIN c_context;
  {splice}	 YY_OBS_GROW;
  <<EOF>>	 BEGIN c_context;
}


  /*----------------------------------------------------------------.
  | Scanning a C string, including its escapes.  The initial `"' is |
  | already eaten.                                                  |
  `----------------------------------------------------------------*/

<SC_ESCAPED_STRING>
{
  "\"" {
    YY_OBS_GROW;
    YY_OBS_FINISH;
    yylval->string = last_string;
    rule_length++;
    BEGIN INITIAL;
    return STRING;
  }

  .|\n	    YY_OBS_GROW;
  <<EOF>>   unexpected_end_of_file (yylloc, "\"");
}

  /*---------------------------------------------------------------.
  | Scanning a C character, decoding its escapes.  The initial "'" |
  | is already eaten.                                              |
  `---------------------------------------------------------------*/

<SC_ESCAPED_CHARACTER>
{
  "'" {
    YY_OBS_GROW;
    YY_OBS_FINISH;
    yylval->symbol = symbol_get (last_string, *yylloc);
    symbol_class_set (yylval->symbol, token_sym, *yylloc);
    symbol_user_token_number_set (yylval->symbol,
				  (unsigned char) last_string[1], *yylloc);
    YY_OBS_FREE;
    rule_length++;
    BEGIN INITIAL;
    return ID;
  }

  .|\n	    YY_OBS_GROW;
  <<EOF>>   unexpected_end_of_file (yylloc, "'");
}


  /*----------------------------.
  | Decode escaped characters.  |
  `----------------------------*/

<SC_ESCAPED_STRING,SC_ESCAPED_CHARACTER>
{
  \\[0-7]{1,3} {
    unsigned long c = strtoul (yytext + 1, 0, 8);
    if (UCHAR_MAX < c)
      {
	complain_at (*yylloc, _("invalid escape sequence: %s"),
		     quote (yytext));
	YY_STEP;
      }
    else
      obstack_1grow (&string_obstack, c);
  }

  \\x[0-9a-fA-F]+ {
    unsigned long c;
    errno = 0;
    c = strtoul (yytext + 2, 0, 16);
    if (UCHAR_MAX < c || errno)
      {
	complain_at (*yylloc, _("invalid escape sequence: %s"),
		     quote (yytext));
	YY_STEP;
      }
    else
      obstack_1grow (&string_obstack, c);
  }

  \\a	obstack_1grow (&string_obstack, '\a');
  \\b	obstack_1grow (&string_obstack, '\b');
  \\f	obstack_1grow (&string_obstack, '\f');
  \\n	obstack_1grow (&string_obstack, '\n');
  \\r	obstack_1grow (&string_obstack, '\r');
  \\t	obstack_1grow (&string_obstack, '\t');
  \\v	obstack_1grow (&string_obstack, '\v');

  /* \\[\"\'?\\] would be shorter, but it confuses xgettext.  */
  \\("\""|"'"|"?"|"\\")  obstack_1grow (&string_obstack, yytext[1]);

  \\(u|U[0-9a-fA-F]{4})[0-9a-fA-F]{4} {
    int c = convert_ucn_to_byte (yytext);
    if (c < 0)
      {
	complain_at (*yylloc, _("invalid escape sequence: %s"),
		     quote (yytext));
	YY_STEP;
      }
    else
      obstack_1grow (&string_obstack, c);
  }
  \\(.|\n)	{
    complain_at (*yylloc, _("unrecognized escape sequence: %s"),
		 quote (yytext));
    YY_OBS_GROW;
  }
}


  /*----------------------------------------------------------.
  | Scanning a C character without decoding its escapes.  The |
  | initial "'" is already eaten.                             |
  `----------------------------------------------------------*/

<SC_CHARACTER>
{
  "'"			YY_OBS_GROW; BEGIN c_context;
  \\{splice}[^$@\[\]]	YY_OBS_GROW;
  <<EOF>>		unexpected_end_of_file (yylloc, "'");
}


  /*----------------------------------------------------------------.
  | Scanning a C string, without decoding its escapes.  The initial |
  | `"' is already eaten.                                           |
  `----------------------------------------------------------------*/

<SC_STRING>
{
  "\""			YY_OBS_GROW; BEGIN c_context;
  \\{splice}[^$@\[\]]	YY_OBS_GROW;
  <<EOF>>		unexpected_end_of_file (yylloc, "\"");
}


  /*---------------------------------------------------.
  | Strings, comments etc. can be found in user code.  |
  `---------------------------------------------------*/

<SC_BRACED_CODE,SC_PROLOGUE,SC_EPILOGUE>
{
  "'"		  YY_OBS_GROW; c_context = YY_START; BEGIN SC_CHARACTER;
  "\""		  YY_OBS_GROW; c_context = YY_START; BEGIN SC_STRING;
  "/"{splice}"*"  YY_OBS_GROW; c_context = YY_START; BEGIN SC_COMMENT;
  "/"{splice}"/"  YY_OBS_GROW; c_context = YY_START; BEGIN SC_LINE_COMMENT;
}


  /*---------------------------------------------------------------.
  | Scanning some code in braces (%union and actions). The initial |
  | "{" is already eaten.                                          |
  `---------------------------------------------------------------*/

<SC_BRACED_CODE>
{
  "{"|"<"{splice}"%"  YY_OBS_GROW; braces_level++;
  "%"{splice}">"      YY_OBS_GROW; braces_level--;
  "}" {
    YY_OBS_GROW;
    braces_level--;
    if (braces_level < 0)
      {
	YY_OBS_FINISH;
	yylval->string = last_string;
	rule_length++;
	BEGIN INITIAL;
	return BRACED_CODE;
      }
  }

  /* Tokenize `<<%' correctly (as `<<' `%') rather than incorrrectly
     (as `<' `<%').  */
  "<"{splice}"<"  YY_OBS_GROW;

  "$"("<"{tag}">")?(-?[0-9]+|"$") { handle_dollar (current_braced_code,
						   yytext, *yylloc); }
  "@"(-?[0-9]+|"$")               { handle_at (current_braced_code,
					       yytext, *yylloc); }

  <<EOF>>  unexpected_end_of_file (yylloc, "}");
}


  /*--------------------------------------------------------------.
  | Scanning some prologue: from "%{" (already scanned) to "%}".  |
  `--------------------------------------------------------------*/

<SC_PROLOGUE>
{
  "%}" {
    YY_OBS_FINISH;
    yylval->string = last_string;
    BEGIN INITIAL;
    return PROLOGUE;
  }

  <<EOF>>  unexpected_end_of_file (yylloc, "%}");
}


  /*---------------------------------------------------------------.
  | Scanning the epilogue (everything after the second "%%", which |
  | has already been eaten).                                       |
  `---------------------------------------------------------------*/

<SC_EPILOGUE>
{
  <<EOF>> {
    YY_OBS_FINISH;
    yylval->string = last_string;
    BEGIN INITIAL;
    return EPILOGUE;
  }
}


  /*----------------------------------------------------------------.
  | By default, grow the string obstack with the input, escaping M4 |
  | quoting characters.						    |
  `----------------------------------------------------------------*/

<SC_COMMENT,SC_LINE_COMMENT,SC_STRING,SC_CHARACTER,SC_BRACED_CODE,SC_PROLOGUE,SC_EPILOGUE>
{
  \$	obstack_sgrow (&string_obstack, "$][");
  \@	obstack_sgrow (&string_obstack, "@@");
  \[	obstack_sgrow (&string_obstack, "@{");
  \]	obstack_sgrow (&string_obstack, "@}");
  .|\n  YY_OBS_GROW;
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
  else
    {
      long num;
      errno = 0;
      num = strtol (cp, 0, 10);

      if (INT_MIN <= num && num <= rule_length && ! errno)
	{
	  int n = num;
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
      else
	complain_at (location, _("integer out of range: %s"), quote (text));
    }
}


/*---------------------------------------------------------------.
| TEXT is expected to be $$ in some code associated to a symbol: |
| destructor or printer.                                         |
`---------------------------------------------------------------*/

static inline void
handle_symbol_code_dollar (char *text, location_t location)
{
  char *cp = text + 1;
  if (*cp == '$')
    obstack_sgrow (&string_obstack, "]b4_dollar_dollar[");
  else
    complain_at (location, _("invalid value: %s"), quote (text));
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
  else
    {
      long num;
      errno = 0;
      num = strtol (cp, 0, 10);

      if (INT_MIN <= num && num <= rule_length && ! errno)
	{
	  int n = num;
	  obstack_fgrow2 (&string_obstack, "]b4_rhs_location([%d], [%d])[",
			  rule_length, n);
	}
      else
	complain_at (location, _("integer out of range: %s"), quote (text));
    }
}


/*---------------------------------------------------------------.
| TEXT is expected to be @$ in some code associated to a symbol: |
| destructor or printer.                                         |
`---------------------------------------------------------------*/

static inline void
handle_symbol_code_at (char *text, location_t location)
{
  char *cp = text + 1;
  if (*cp == '$')
    obstack_sgrow (&string_obstack, "]b4_at_dollar[");
  else
    complain_at (location, _("invalid value: %s"), quote (text));
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


/*------------------------------------------------------------------.
| Convert universal character name UCN to a single-byte character,  |
| and return that character.  Return -1 if UCN does not correspond  |
| to a single-byte character.					    |
`------------------------------------------------------------------*/

static int
convert_ucn_to_byte (char const *ucn)
{
  unsigned long code = strtoul (ucn + 2, 0, 16);

  /* FIXME: Currently we assume Unicode-compatible unibyte characters
     on ASCII hosts (i.e., Latin-1 on hosts with 8-bit bytes).  On
     non-ASCII hosts we support only the portable C character set.
     These limitations should be removed once we add support for
     multibyte characters.  */

  if (UCHAR_MAX < code)
    return -1;

#if ! ('$' == 0x24 && '@' == 0x40 && '`' == 0x60 && '~' == 0x7e)
  {
    /* A non-ASCII host.  Use CODE to index into a table of the C
       basic execution character set, which is guaranteed to exist on
       all Standard C platforms.  This table also includes '$', '@',
       and '`', which are not in the basic execution character set but
       which are unibyte characters on all the platforms that we know
       about.  */
    static signed char const table[] =
      {
	'\0',   -1,   -1,   -1,   -1,   -1,   -1, '\a',
	'\b', '\t', '\n', '\v', '\f', '\r',   -1,   -1,
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
	 ' ',  '!',  '"',  '#',  '$',  '%',  '&', '\'',
	 '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	 '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
	 '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	 '@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
	 'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	 'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
	 'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_',
	 '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
	 'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	 'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
	 'x',  'y',  'z',  '{',  '|',  '}',  '~'
      };

    code = code < sizeof table ? table[code] : -1;
  }
#endif

  return code;
}


/*----------------------------------------------------------------.
| Handle `#line INT "FILE"'.  ARGS has already skipped `#line '.  |
`----------------------------------------------------------------*/

static void
handle_syncline (char *args, location_t *location)
{
  int lineno = strtol (args, &args, 10);
  const char *file = NULL;
  file = strchr (args, '"') + 1;
  *strchr (file, '"') = 0;
  current_file = xstrdup (file);
  location->file = current_file;
  location->last_line = lineno;
}


/*-------------------------------------------------------------.
| Report an unexpected end of file at LOC.  An end of file was |
| encountered and the expected TOKEN_END was missing.  After   |
| reporting the problem, pretend that TOKEN_END was found.     |
`-------------------------------------------------------------*/

static void
unexpected_end_of_file (location_t *loc, char const *token_end)
{
  size_t i = strlen (token_end);

  complain_at (*loc, _("missing `%s' at end of file"), token_end);

  /* Adjust location's last column so that any later message does not
     mention the characters just inserted.  */
  loc->last_column -= i;

  while (i != 0)
    unput (token_end[--i]);
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
