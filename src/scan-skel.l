                                                             /* -*- C -*- */
/* Scan Bison Skeletons.
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

%option debug nodefault noyywrap nounput
%option prefix="skel_" outfile="lex.yy.c"

/* If we enable

   %option yylineno

   Then we have warning: `yy_flex_realloc' defined but not used.
   Seems like a Flex bug to me: Why the heck yylineno would trigger
   the REJECT exception???  */

%{
#include "system.h"
#include "skeleton.h"
#include "parse-skel.h"
%}

%{
/* Each time we match a string, move the end cursor to its end. */
#define YY_USER_ACTION  yylloc->last_column += yyleng;
%}
%%
%{
  /* At each yylex invocation, mark the current position as the
     start of the next token.  */
  LOCATION_STEP (*yylloc);
%}

"%%{line}"          { return LINE; }
"%%{skeleton-line}" { return SLINE; }

"%%{yacc}"    { return YACC; }
"%%{section}" { return SECTION; }

"%%{guards}"   { return GUARDS; }
"%%{actions}"  { return ACTIONS; }
"%%{tokendef}" { return TOKENS; }

  /* Muscle.  */
"%%{"[a-zA-Z][0-9a-zA-Z_-]+"}" {
  yylval->string = xstrndup (yytext + 3, yyleng - 4);
  return MUSCLE;
}

  /* String.  */
"%%\"".*"\"" {
  yylval->string = xstrndup (yytext + 3, yyleng - 4);
  return STRING;
}

  /* End of line.  */
"\n" {
  LOCATION_LINES (*yylloc, yyleng);
  return '\n';
}

  /* White spaces.  */
[\t ]+ {
  yylval->string = yytext;
  return BLANKS;
}

  /* Plain Characters.  */
[^%\n]+ {
  yylval->string = yytext;
  return RAW;
}

  /* Plain Character.  */
. {
  yylval->character = *yytext;
  return CHARACTER;
}

%%
