/*                                                       -*- C -*-
  Copyright (C) 2020-2022, 2025-2026 Free Software Foundation, Inc.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* Simplified C++ Type and Expression Grammar.
   Written by Paul Hilfinger for Bison's test suite.  */

%define api.pure
%header
%define api.header.include {"c++-types.h"}
%locations
%debug

/* Nice error messages with details. */
%define parse.error detailed

%code requires
{
  union node {
    struct {
      int is_nterm;
      int parents;
    } node_info;
    struct {
      int is_nterm; /* 1 */
      int parents;
      char const *form;
      union node *children[3];
    } nterm;
    struct {
      int is_nterm; /* 0 */
      int parents;
      char *text;
    } term;
  };
  typedef union node node_t;
}

%define api.value.type union

%code
{
  /* Portability issues for strdup. */
#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#endif

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  static node_t *new_nterm (char const *, node_t *, node_t *, node_t *);
  static node_t *new_term (char *);
  static void free_node (node_t *);
  static char *node_to_string (const node_t *);
  static void node_print (FILE *, const node_t *);
  static node_t *stmt_merge (YYSTYPE x0, YYSTYPE x1);

  static void yyerror (YYLTYPE const * const loc, const char *msg);
  static yytoken_kind_t yylex (YYSTYPE *lval, YYLTYPE *lloc);
}

%expect-rr 1

%token
  TYPENAME "typename"
  ID "identifier"

%right '='
%left '+'

%glr-parser

%type <node_t *> stmt expr decl declarator TYPENAME ID
%destructor { free_node ($$); } <node_t *>
%printer { node_print (yyo, $$); } <node_t *>

%%

prog : %empty
     | prog stmt   {
                     YYLOCATION_PRINT (stdout, &@2);
                     fputs (": ", stdout);
                     node_print (stdout, $2);
                     putc ('\n', stdout);
                     fflush (stdout);
                     free_node ($2);
                   }
     ;

stmt : expr ';'  %merge <stmt_merge>     { $$ = $1; }
     | decl      %merge <stmt_merge>
     | error ';'        { $$ = new_nterm ("<error>", NULL, NULL, NULL); }
     ;

expr : ID
     | TYPENAME '(' expr ')'
                        { $$ = new_nterm ("<cast>(%s, %s)", $3, $1, NULL); }
     | expr '+' expr    { $$ = new_nterm ("+(%s, %s)", $1, $3, NULL); }
     | expr '=' expr    { $$ = new_nterm ("=(%s, %s)", $1, $3, NULL); }
     ;

decl : TYPENAME declarator ';'
                        { $$ = new_nterm ("<declare>(%s, %s)", $1, $2, NULL); }
     | TYPENAME declarator '=' expr ';'
                        { $$ = new_nterm ("<init-declare>(%s, %s, %s)", $1,
                                          $2, $4); }
     ;

declarator
     : ID
     | '(' declarator ')' { $$ = $2; }
     ;

%%

/* A C error reporting function.  */
static void
yyerror (YYLTYPE const * const loc, const char *msg)
{
  YYLOCATION_PRINT (stderr, loc);
  fprintf (stderr, ": %s\n", msg);
}

/* The input file. */
FILE * input = NULL;

yytoken_kind_t
yylex (YYSTYPE *lval, YYLTYPE *lloc)
{
  static int line_num = 1;
  static int col_num = 0;

  while (1)
    {
      int c;
      assert (!feof (input));
      c = getc (input);
      switch (c)
        {
        case EOF:
          return 0;
        case '\t':
          col_num = (col_num + 7) & ~7;
          break;
        case ' ': case '\f':
          col_num += 1;
          break;
        case '\n':
          line_num += 1;
          col_num = 0;
          break;
        default:
          {
            yytoken_kind_t tok;
            lloc->first_line = lloc->last_line = line_num;
            lloc->first_column = col_num;
            if (isalpha (c))
              {
                char buffer[256];
                unsigned i = 0;

                do
                  {
                    buffer[i++] = (char) c;
                    col_num += 1;
                    assert (i != sizeof buffer - 1);
                    c = getc (input);
                  }
                while (isalnum (c) || c == '_');

                ungetc (c, input);
                buffer[i++] = 0;
                if (isupper ((unsigned char) buffer[0]))
                  {
                    tok = TYPENAME;
                    lval->TYPENAME = new_term (strdup (buffer));
                  }
                else
                  {
                    tok = ID;
                    lval->ID = new_term (strdup (buffer));
                  }
              }
            else
              {
                col_num += 1;
                tok = c;
              }
            lloc->last_column = col_num;
            return tok;
          }
        }
    }
}

static node_t *
new_nterm (char const *form, node_t *child0, node_t *child1, node_t *child2)
{
  node_t *res = malloc (sizeof *res);
  res->nterm.is_nterm = 1;
  res->nterm.parents = 0;
  res->nterm.form = form;
  res->nterm.children[0] = child0;
  if (child0)
    child0->node_info.parents += 1;
  res->nterm.children[1] = child1;
  if (child1)
    child1->node_info.parents += 1;
  res->nterm.children[2] = child2;
  if (child2)
    child2->node_info.parents += 1;
  return res;
}

static node_t *
new_term (char *text)
{
  node_t *res = malloc (sizeof *res);
  res->term.is_nterm = 0;
  res->term.parents = 0;
  res->term.text = text;
  return res;
}

static void
free_node (node_t *node)
{
  if (!node)
    return;
  node->node_info.parents -= 1;
  /* Free only if 0 (last parent) or -1 (no parents).  */
  if (node->node_info.parents > 0)
    return;
  if (node->node_info.is_nterm == 1)
    {
      free_node (node->nterm.children[0]);
      free_node (node->nterm.children[1]);
      free_node (node->nterm.children[2]);
    }
  else
    free (node->term.text);
  free (node);
}

static char *
node_to_string (const node_t *node)
{
  char *res;
  if (!node)
    res = strdup ("");
  else if (node->node_info.is_nterm)
    {
      char *child0 = node_to_string (node->nterm.children[0]);
      char *child1 = node_to_string (node->nterm.children[1]);
      char *child2 = node_to_string (node->nterm.children[2]);
      res = malloc (strlen (node->nterm.form) + strlen (child0)
                    + strlen (child1) + strlen (child2) + 1);
      sprintf (res, node->nterm.form, child0, child1, child2);
      free (child2);
      free (child1);
      free (child0);
    }
  else
    res = strdup (node->term.text);
  return res;
}

static void
node_print (FILE *out, const node_t *n)
{
  char *str = node_to_string (n);
  fputs (str, out);
  free (str);
}


static node_t *
stmt_merge (YYSTYPE x0, YYSTYPE x1)
{
  return new_nterm ("<OR>(%s, %s)", x0.stmt, x1.stmt, NULL);
}

static int
process (const char *file)
{
  int is_stdin = !file || strcmp (file, "-") == 0;
  if (is_stdin)
    input = stdin;
  else
    input = fopen (file, "r");
  assert (input);
  int status = yyparse ();
  if (!is_stdin)
    fclose (input);
  return status;
}

int
main (int argc, char **argv)
{
  if (getenv ("YYDEBUG"))
    yydebug = 1;

  int ran = 0;
  for (int i = 1; i < argc; ++i)
    // Enable parse traces on option -p.
    if (strcmp (argv[i], "-p") == 0)
      yydebug = 1;
    else
      {
        int status = process (argv[i]);
        ran = 1;
        if (!status)
          return status;
      }

  if (!ran)
    {
      int status = process (NULL);
      if (!status)
        return status;
    }
  return 0;
}
