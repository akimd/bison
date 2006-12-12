/* Scan Bison Skeletons.                                       -*- C -*-

   Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 Free Software
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

#define FLEX_NO_OBSTACK
#define FLEX_PREFIX(Id) skel_ ## Id
#include "flex-scanner.h"

#include <dirname.h>
#include <error.h>
#include <quotearg.h>

#include "complain.h"
#include "getargs.h"
#include "files.h"
#include "scan-skel.h"

int skel_lex (void);

#define QPUTS(String) \
   fputs (quotearg_style (c_quoting_style, String), yyout)

static char *decode_at_digraphs (char *in);
%}
%%

%{
  int lineno IF_LINT (= 0);
  char *outname = NULL;
%}

"@output ".*\n {
  char *file_name = yytext + sizeof "@output " - 1;
  yytext[yyleng - 1] = '\0';
  if (outname)
    {
      free (outname);
      xfclose (yyout);
    }
  outname = decode_at_digraphs (file_name);
  output_file_name_check (outname);
  yyout = xfopen (outname, "w");
  lineno = 1;
}

"@@"	   fputc ('@', yyout);
"@{"	   fputc ('[', yyout);
"@}"	   fputc (']', yyout);

"@oline@"  fprintf (yyout, "%d", lineno + 1);
"@ofile@"  QPUTS (outname);
"@dir_prefix@" QPUTS (dir_prefix);

"@basename(".*"@)" {
  char *file_name = yytext + sizeof "@basename " - 1;
  yytext[yyleng - 2] = '\0';
  file_name = decode_at_digraphs (file_name);
  fputs (last_component (file_name), yyout);
  free (file_name);
}

  /* This pattern must not match more than the previous @ patterns. */
@[^{}@\n]* fatal ("invalid @ in skeleton: %s", yytext);
\n	   lineno++; ECHO;
[^@\n]+	   ECHO;

<<EOF>> {
  if (outname)
    {
      free (outname);
      xfclose (yyout);
    }
  return EOF;
}
%%

/*------------------------.
| Scan a Bison skeleton.  |
`------------------------*/

void
scan_skel (FILE *in)
{
  skel_in = in;
  skel__flex_debug = trace_flag & trace_skeleton;
  skel_lex ();
  /* Reclaim Flex's buffers.  */
  yylex_destroy ();
}

static char *
decode_at_digraphs (char *in)
{
  char *out_start = xnmalloc (strlen (in)+1, sizeof *out_start);
  char *out;
  for (out = out_start; *in != '\0'; ++in, ++out)
    {
      if (*in == '@')
        {
          switch (*++in)
            {
              case '@': *out = '@'; break;
              case '{': *out = '['; break;
              case '}': *out = ']'; break;
              default:
                if (*in != '\0')
                  in[1] = '\0';
                fatal ("invalid @ in skeleton: %s", in-1);
                break;
            }
        }
      else
        *out = *in;
    }
  *out = '\0';
  return out_start;
}
