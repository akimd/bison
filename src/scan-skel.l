/* Scan Bison Skeletons.                                       -*- C -*-

   Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 Free Software
   Foundation, Inc.

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
   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

%option nodefault noyywrap nounput never-interactive debug
%option prefix="skel_" outfile="lex.yy.c"

%{
/* Work around a bug in flex 2.5.31.  See Debian bug 333231
   <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.  */
#undef skel_wrap
#define skel_wrap() 1

#define FLEX_PREFIX(Id) skel_ ## Id
#include "flex-scanner.h"

#include <dirname.h>
#include <error.h>
#include <quotearg.h>

#include "complain.h"
#include "getargs.h"
#include "files.h"
#include "scan-skel.h"

#define YY_DECL static int skel_lex (void)
YY_DECL;

#define QPUTS(String) \
   fputs (quotearg_style (c_quoting_style, String), yyout)

static void at_directive_perform (char **outnamep, int *out_linenop);
static void fail_for_at_directive_too_many_args (void);
static void fail_for_at_directive_too_few_args (void);
static void fail_for_invalid_at (char const *at);

/* In SC_AT_DIRECTIVE_ARG context, the name of the directive.  */
static char *at_directive_name = NULL;

/* Currently, only the @warn, @complain, @fatal, @warn_at, @complain_at, and
   @fatal_at directives take multiple arguments, and the last three already
   can't take more than 7.  */
#define AT_DIRECTIVE_ARGC_MAX 7
static int at_directive_argc = 0;
static char *at_directive_argv[AT_DIRECTIVE_ARGC_MAX];
%}

%x SC_AT_GETTEXT_ARG
%x SC_AT_DIRECTIVE_ARGS
%x SC_AT_DIRECTIVE_SKIP_WS

%%

%{
  int out_lineno IF_LINT (= 0);
  char *outname = NULL;
%}

"@@"	   fputc ('@', yyout);
"@{"	   fputc ('[', yyout);
"@}"	   fputc (']', yyout);

"@oline@"  fprintf (yyout, "%d", out_lineno + 1);
"@ofile@"  QPUTS (outname);
"@dir_prefix@" QPUTS (dir_prefix);

"@gettext<" BEGIN SC_AT_GETTEXT_ARG;

@[a-z_]+"(" {
  yytext[yyleng-1] = '\0';
  at_directive_name = xstrdup (yytext);
  BEGIN SC_AT_DIRECTIVE_ARGS;
}

  /* This pattern must not match more than the previous @ patterns. */
@[^<@{}(\n]* fail_for_invalid_at (yytext);
\n	   out_lineno++; ECHO;
[^@\n]+	   ECHO;

<INITIAL><<EOF>> {
  if (outname)
    {
      free (outname);
      xfclose (yyout);
    }
  return EOF;
}

<SC_AT_GETTEXT_ARG>{
  "@>" {
    char *arg;
    obstack_1grow (&obstack_for_string, '\0');
    arg = obstack_finish (&obstack_for_string);
    if (!at_directive_name)
      {
        fprintf (yyout, "%s", _(arg));
        obstack_free (&obstack_for_string, arg);
        BEGIN INITIAL;
      }
    else
      {
        char const *translated = _(arg);
        size_t parent_size = strlen (at_directive_argv[at_directive_argc]);
        size_t translated_size = strlen (translated);
        char *copy = xmalloc (parent_size + translated_size + 1);
        strcpy (copy, at_directive_argv[at_directive_argc]);
        strcpy (copy + parent_size, translated);
        obstack_free (&obstack_for_string,
                      at_directive_argv[at_directive_argc]);
        obstack_grow (&obstack_for_string, copy,
                      parent_size + translated_size);
        free (copy);
        BEGIN SC_AT_DIRECTIVE_ARGS;
      }
  }
}

<SC_AT_DIRECTIVE_ARGS>{
  "@`" /* Emtpy.  Useful for starting an argument
          that begins with whitespace. */

  "@gettext<" {
    if (at_directive_argc >= AT_DIRECTIVE_ARGC_MAX)
      fail_for_at_directive_too_many_args ();
    obstack_1grow (&obstack_for_string, '\0');
    at_directive_argv[at_directive_argc] =
      obstack_finish (&obstack_for_string);
    BEGIN SC_AT_GETTEXT_ARG;
  }

  @[,)] {
    if (at_directive_argc >= AT_DIRECTIVE_ARGC_MAX)
      fail_for_at_directive_too_many_args ();

    obstack_1grow (&obstack_for_string, '\0');
    at_directive_argv[at_directive_argc++] =
      obstack_finish (&obstack_for_string);

    /* Like M4, skip whitespace after a comma.  */
    if (yytext[1] == ',')
      BEGIN SC_AT_DIRECTIVE_SKIP_WS;
    else
      {
        at_directive_perform (&outname, &out_lineno);
        obstack_free (&obstack_for_string, at_directive_argv[0]);
        at_directive_argc = 0;
        free (at_directive_name);
        at_directive_name = NULL;
        BEGIN INITIAL;
      }
  }
}

<SC_AT_GETTEXT_ARG,SC_AT_DIRECTIVE_ARGS>{
  [^@]+ { STRING_GROW; }
  "@@" { obstack_1grow (&obstack_for_string, '@'); }
  "@{" { obstack_1grow (&obstack_for_string, '['); }
  "@}" { obstack_1grow (&obstack_for_string, ']'); }
  @.? { fail_for_invalid_at (yytext); }
}

<SC_AT_DIRECTIVE_SKIP_WS>{
  [ \t\r\n]
  . { yyless (0); BEGIN SC_AT_DIRECTIVE_ARGS; }
}

<SC_AT_GETTEXT_ARG,SC_AT_DIRECTIVE_ARGS,SC_AT_DIRECTIVE_SKIP_WS>{
  <<EOF>> {
    fatal (_("unclosed %s directive in skeleton"),
           at_directive_name ? at_directive_name : "@gettext");
  }
}

%%

/*------------------------.
| Scan a Bison skeleton.  |
`------------------------*/

void
scan_skel (FILE *in)
{
  static bool initialized = false;
  if (!initialized)
    {
      initialized = true;
      obstack_init (&obstack_for_string);
    }
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

void
skel_scanner_free (void)
{
  obstack_free (&obstack_for_string, 0);
}

static
void at_directive_perform (char **outnamep, int *out_linenop)
{
  if (0 == strcmp (at_directive_name, "@basename"))
    {
      if (at_directive_argc > 1)
        fail_for_at_directive_too_many_args ();
      fputs (last_component (at_directive_argv[0]), yyout);
    }
  else if (0 == strcmp (at_directive_name, "@warn")
           || 0 == strcmp (at_directive_name, "@complain")
           || 0 == strcmp (at_directive_name, "@fatal"))
    {
      void (*func)(char const *, ...);
      switch (at_directive_name[1])
        {
          case 'w': func = warn; break;
          case 'c': func = complain; break;
          case 'f': func = fatal; break;
          default: aver (false); func = NULL; break;
        }
      switch (at_directive_argc)
        {
          case 1:
            func (at_directive_argv[0]);
            break;
          case 2:
            func (at_directive_argv[0], at_directive_argv[1]);
            break;
          case 3:
            func (at_directive_argv[0], at_directive_argv[1],
                  at_directive_argv[2]);
            break;
          case 4:
            func (at_directive_argv[0], at_directive_argv[1],
                  at_directive_argv[2], at_directive_argv[3]);
            break;
          case 5:
            func (at_directive_argv[0], at_directive_argv[1],
                  at_directive_argv[2], at_directive_argv[3],
                  at_directive_argv[4]);
            break;
          default:
            fail_for_at_directive_too_many_args ();
            break;
        }
    }
  else if (0 == strcmp (at_directive_name, "@warn_at")
           || 0 == strcmp (at_directive_name, "@complain_at")
           || 0 == strcmp (at_directive_name, "@fatal_at"))
    {
      void (*func)(location, char const *, ...);
      location loc;
      if (at_directive_argc < 3)
        fail_for_at_directive_too_few_args ();
      switch (at_directive_name[1])
        {
          case 'w': func = warn_at; break;
          case 'c': func = complain_at; break;
          case 'f': func = fatal_at; break;
          default: aver (false); func = NULL; break;
        }
      boundary_set_from_string (&loc.start, at_directive_argv[0]);
      boundary_set_from_string (&loc.end, at_directive_argv[1]);
      switch (at_directive_argc)
        {
          case 3:
            func (loc, at_directive_argv[2]);
            break;
          case 4:
            func (loc, at_directive_argv[2], at_directive_argv[3]);
            break;
          case 5:
            func (loc, at_directive_argv[2], at_directive_argv[3],
                  at_directive_argv[4]);
            break;
          case 6:
            func (loc, at_directive_argv[2], at_directive_argv[3],
                  at_directive_argv[4], at_directive_argv[5]);
            break;
          case 7:
            func (loc, at_directive_argv[2], at_directive_argv[3],
                  at_directive_argv[4], at_directive_argv[5],
                  at_directive_argv[6]);
            break;
          default:
            fail_for_at_directive_too_many_args ();
            break;
        }
    }
  else if (0 == strcmp (at_directive_name, "@output"))
    {
      if (at_directive_argc > 1)
        fail_for_at_directive_too_many_args ();
      if (*outnamep)
        {
          free (*outnamep);
          xfclose (yyout);
        }
      *outnamep = xstrdup (at_directive_argv[0]);
      output_file_name_check (*outnamep);
      yyout = xfopen (*outnamep, "w");
      *out_linenop = 1;
    }
  else
    fail_for_invalid_at (at_directive_name);
}

static void
fail_for_at_directive_too_few_args (void)
{
  fatal (_("too few arguments for %s directive in skeleton"),
         at_directive_name);
}

static void
fail_for_at_directive_too_many_args (void)
{
  fatal (_("too many arguments for %s directive in skeleton"),
         at_directive_name);
}

static void
fail_for_invalid_at (char const *at)
{
  fatal ("invalid @ in skeleton: %s", at);
}
