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
%error-verbose

%{
#include "system.h"
#include "obstack.h"
#include "files.h"
#include "output.h"
#include "skeleton.h"
#include "muscle_tab.h"

extern FILE* yyin;
extern int   yylineno;

char* prefix = NULL;
FILE* parser = NULL;

size_t output_line;
size_t skeleton_line;

static int merror PARAMS ((const char* error));
static int yyerror PARAMS ((const char* error));

%}

%union
{
  char* muscle;
  char* string;
  char character;
  int yacc;
}

%token <muscle> MUSCLE
%token <string> STRING
%token <character> CHARACTER

%token LINE
%token SLINE

%token YACC
%token SECTION

%token GUARDS
%token TOKENS
%token ACTIONS

%type <yacc> section.yacc

%start skeleton

%%

skeleton : /* Empty.  */    { }
         | section skeleton { }
;

section : section.header section.body { }
;

section.header : SECTION gb MUSCLE gb STRING gb section.yacc gb '\n'
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

gb : /* Empty.  */ { }
   | gb CHARACTER  { /* Do not echo garbage characters.  */ }
;

%%

static int
merror (const char* error)
{
  printf ("line %d: %%{%s} undeclared.\n", skeleton_line, error);
  return 0;
}

static int
yyerror (const char* error)
{
  fprintf (stderr, "%s\n", error);
  return 0;
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
  yyin = fopen (skel, "r");
  yydebug = 0;
  yyparse ();

  /* Close the last parser.  */
  if (parser)
    parser = (xfclose (parser), NULL);
}
