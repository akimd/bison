/* Scan Bison Skeletons.                                       -*- C -*-
   Copyright (C) 2001, 2002  Free Software Foundation, Inc.

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

%option nodefault noyywrap nounput never-interactive
%option prefix="skel_" outfile="lex.yy.c"

%{
#include "system.h"
#include "error.h"
#include "getargs.h"
#include "files.h"
int skel_lex PARAMS ((void));
static int yylineno = 1;
static char *yyoutname = NULL;
%}
%%
  /* This is an approximation, but we don't need more. */
^"#output \""[^\"]+\"\n        {
     yytext[yyleng - 2] = '\0';
     XFREE (yyoutname);
     yyoutname = xstrdup (yytext + strlen ("#output \""));
     yyout = xfopen (yyoutname, "w");
     yylineno = 1;
   }

"@<:@"           fputc ('[', yyout);
"@:>@"           fputc (']', yyout);

"__oline__"      fprintf (yyout, "%d", yylineno+1);
"__ofile__"      fprintf (yyout, "%s", yyoutname);
[^@_\n]+         ECHO;
\n+              yylineno += yyleng; ECHO;
.                ECHO;

<<EOF>>          xfclose (yyout); free (yyoutname); return EOF;
%%

/* From lib/readpipe.c.  */
FILE *readpipe PARAMS ((const char *, ...));

/*----------------------.
| Run our backend, M4.  |
`----------------------*/

void m4_invoke PARAMS ((const char *definitions));

void
m4_invoke (const char *definitions)
{
  /* Invoke m4 on the definition of the muscles, and the skeleton. */
  const char *bison_pkgdatadir = getenv ("BISON_PKGDATADIR");
  const char *m4 = getenv ("M4");
  int pkg_data_len;
  char *full_skeleton;

  if (!m4)
    m4 = M4;
  if (!bison_pkgdatadir)
    bison_pkgdatadir = PKGDATADIR;
  pkg_data_len = strlen (bison_pkgdatadir);
  full_skeleton = XMALLOC (char, pkg_data_len + strlen (skeleton) + 2);
  if (bison_pkgdatadir[pkg_data_len-1] == '/')
    sprintf (full_skeleton, "%s%s", bison_pkgdatadir, skeleton);
  else
    sprintf (full_skeleton, "%s/%s", bison_pkgdatadir, skeleton);
  if (trace_flag & trace_tools)
    fprintf (stderr,
	     "running: %s -I %s m4sugar/m4sugar.m4 %s %s\n",
	     m4, bison_pkgdatadir, definitions, full_skeleton);
  skel_in = readpipe (m4,
		      "-I", bison_pkgdatadir,
		      "m4sugar/m4sugar.m4",
		      definitions,
		      full_skeleton,
		      NULL);
  XFREE (full_skeleton);
  if (!skel_in)
    error (EXIT_FAILURE, errno, "cannot run m4");
  skel_lex ();

  /* Reclaim Flex's buffers.  */
  yy_delete_buffer (YY_CURRENT_BUFFER);
}
