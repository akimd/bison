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

%{

#include "system.h"
#include "skeleton.h"
#include "parse-skel.h"

%}

%option nounput
%option noyywrap
/* If we enable

   %option yylineno

   Then we have warning: `yy_flex_realloc' defined but not used.
   Seems like a Flex bug to me: Why the heck yylineno would trigger
   the REJECT exception???  */

%%

"%%{line}"          { return LINE; }
"%%{skeleton-line}" { return SLINE; }

"%%{yacc}"    { return YACC; }
"%%{section}" { return SECTION; }

"%%{guards}"   { return GUARDS; }
"%%{actions}"  { return ACTIONS; }
"%%{tokendef}" { return TOKENS; }

"%%{"[a-zA-Z][0-9a-zA-Z_-]+"}" { /* Muscle.  */
  size_t len = strlen (yytext);
  yylval.string = (char*) malloc (len - 3);
  strncpy (yylval.string, yytext + 3, len - 4);
  yylval.string[len - 4] = 0;
  return MUSCLE;
}

"%%\"".*"\"" { /* String.  */
  size_t len = strlen (yytext);
  yylval.string = (char*) malloc (len - 3);
  strncpy (yylval.string, yytext + 3, len - 4);
  yylval.string[len - 4] = 0;
  return STRING;
}

<<EOF>> { /* End of file.  */
  return 0;
}

"\n" { /* End of line.  */
  return '\n';
}

. { /* Character.  */
  yylval.character = *yytext;
  return CHARACTER;
}

%%
