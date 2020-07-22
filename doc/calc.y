%union
{
  int ival;
  const char *sval;
}

%token <ival> NUM
%nterm <ival> exp
%token <sval> STR
%nterm <sval> useless

%left '+' '-'
%left '*'

%%
exp:
  exp '+' exp
| exp '-' exp
| exp '*' exp
| exp '/' exp
| NUM
;
useless: STR;
