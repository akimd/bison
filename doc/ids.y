%token ID
%%
s: a ID
a: expr
expr: %empty | expr ID ','
