                                                             /* -*- C -*- */
/* Parse Bison Skeletons.
   Copyright (C) 2001  Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

%debug
%defines
%verbose
%locations
%name-prefix="skel_"
%pure-parser

%{
#include "system.h"
#include "obstack.h"
#include "quotearg.h"
#include "files.h"
#include "getargs.h"
#include "output.h"
#include "skeleton.h"
#include "muscle_tab.h"

#define YYERROR_VERBOSE 1

/* Pass the control structure to YYPARSE but not YYLEX (yet?). */
#define YYPARSE_PARAM skel_control
/* YYPARSE receives SKEL_CONTROL as a void *.  Provide a correctly
   typed access to it.  */
#define yycontrol ((skel_control_t *) skel_control)

char* prefix = NULL;
FILE* parser = NULL;

size_t output_line;
size_t skeleton_line;

static void merror PARAMS ((const char* error));

/* Request detailed parse error messages, and pass them to
   YLEVAL_ERROR. */
#undef  yyerror
#define yyerror(Msg) \
        skel_error (yycontrol, &yylloc, Msg)

/* When debugging our pure parser, we want to see values and locations
   of the tokens.  */
#define YYPRINT(File, Type, Value) \
        yyprint (File, &yylloc, Type, &Value)
static void yyprint (FILE *file, const yyltype *loc,
                     int type, const yystype *value);
%}

%union
{
  char *string;
  char character;
  int  boolean;
}

/* Name of a muscle. */
%token <string> MUSCLE
/* A string dedicated to Bison (%%"foo").  */
%token <string> STRING
/* Raw data, to output directly. */
%token <string> RAW
/* Spaces. */
%token <string> BLANKS
/* Raw data, but char by char. */
%token <character> CHARACTER

%token LINE
%token SLINE

%token YACC
%token SECTION

%token GUARDS
%token TOKENS
%token ACTIONS

%type <boolean> section.yacc

%start input

%%
input:
     { LOCATION_RESET (yylloc) } skeleton
  ;

skeleton : /* Empty.  */    { }
         | section skeleton { }
;

section : section.header section.body { }
;

section.header : SECTION BLANKS MUSCLE BLANKS STRING BLANKS section.yacc '\n'
{
  char *name = 0;
  char *limit = 0;
  char *suffix = $5;

  /* Close the previous parser.  */
  if (parser)
    parser = (xfclose (parser), NULL);

  /* If the following section should be named with the yacc-style, and it's
     suffix is of the form 'something.h' or 'something.c', then add '.tab' in
     the middle of the suffix.  */
  if (tab_extension && $7 && (strsuffix (suffix, ".h") ||
			      strsuffix (suffix, ".c")))
    {
      size_t prefix_len = strlen (prefix);
      size_t suffix_len = strlen (suffix);

      /* Allocate enough space to insert '.tab'.  */
      name = XMALLOC (char, prefix_len + suffix_len + 5);
      limit = strrchr (suffix, '.');
      if (!limit)
	limit = suffix;

      /* Prefix is 'X', suffix is 'Y.Z'.  Name will be 'XY.tab.Z'.  */
      {
	char* cp = 0;
	cp = stpcpy (name, prefix);
	cp = stpncpy (cp, suffix, limit - suffix);
	cp = stpcpy (cp, ".tab");
	cp = stpcpy (cp, limit);
      }
    }
  else
    name = stringappend (prefix, suffix);

  /* Prepare the next parser to be output.  */
  parser = xfopen (name, "w");
  MUSCLE_INSERT_STRING ("parser-file-name", name);
  XFREE (name);

  ++skeleton_line;
}
;

section.yacc : /* Empty.  */ { $$ = 0; }
             | YACC          { $$ = 1; }
;

section.body
: /* Empty.  */ { }
| section.body '\n' { fputc ('\n', parser); ++output_line; ++skeleton_line; }
| section.body LINE  { fprintf (parser, "%d", output_line); }
| section.body SLINE { fprintf (parser, "%d", skeleton_line); }
| section.body GUARDS { guards_output (parser, &output_line); }
| section.body TOKENS { token_definitions_output (parser, &output_line); }
| section.body ACTIONS { actions_output (parser, &output_line); }
| section.body CHARACTER { fputc ($2, parser); }
| section.body RAW       { fputs ($2, parser); }
| section.body BLANKS    { fputs ($2, parser); }
| section.body MUSCLE {
  const char* value = muscle_find ($2);
  if (value)
    {
      fputs (value, parser);
      output_line += get_lines_number (value);
    }
  else
    {
      fprintf (parser, "%%{%s}", $2);
      merror ($2);
    }
}
;
%%
/*------------------------------------------------------------------.
| When debugging the parser, display tokens' locations and values.  |
`------------------------------------------------------------------*/

static void
yyprint (FILE *file,
         const yyltype *loc, int type, const yystype *value)
{
  fputs (" (", file);
  LOCATION_PRINT (file, *loc);
  fputs (")", file);
  switch (type)
    {
    case MUSCLE:
    case STRING:
    case RAW:
    case BLANKS:
      fprintf (file, " = %s", quotearg_style (c_quoting_style,
					      value->string));
      break;

    case CHARACTER:
      fprintf (file, " = '%c'", value->character);
      break;

    case YACC:
      fprintf (file, " = %s", value->boolean ? "true" : "false");
      break;
    }
}


static void
merror (const char* error)
{
  printf ("line %d: %%{%s} undeclared.\n", skeleton_line, error);
}

void
skel_error (skel_control_t *control,
	    const yyltype *loc, const char *msg)
{
  /* Neutralize GCC warnings for unused parameters. */
  skel_control_t *c = control;
  c++;
  LOCATION_PRINT (stderr, *loc);
  fprintf (stderr, "%s\n", msg);
}

void
process_skeleton (const char* skel)
{
  /* Compute prefix.  Actually, it seems that the processing I need here is
     done in compute_base_names, and the result stored in short_base_name.  */
  prefix = short_base_name;

  /* Prepare a few things.  */
  output_line = 1;
  skeleton_line = 1;

  /* Output.  */
  skel_in = fopen (skel, "r");
  /* FIXME: This is not acceptable for a release. */
  skel__flex_debug = getenv ("BISON_TRACE_SCAN") ? 1 : 0;
  skel_debug = getenv ("BISON_TRACE_PARSE") ? 1 : 0;
  skel_parse (NULL);

  /* Close the last parser.  */
  if (parser)
    parser = (xfclose (parser), NULL);
}
