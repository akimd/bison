%{

#include <stdlib.h>
#include <string.h>

#include "parse-skel.h"

%}

%option nounput
%option noyywrap
%option yylineno

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
