#                                                        -*- Autoconf -*-

cat <<EOF

Regression tests.

EOF


## ------------------ ##
## Duplicate string.  ##
## ------------------ ##


AT_SETUP([Duplicate string])

AT_DATA([duplicate.y],
[[/* `Bison -v' used to dump core when two tokens are defined with the same
   string, as LE and GE below. */

%token NUM
%token LE "<="
%token GE "<="

%%
exp: '(' exp ')' | NUM ;
%%
]])

AT_CHECK([bison -v duplicate.y -o duplicate.c], 0, ignore, ignore)

AT_CLEANUP([duplicate.*])



## ---------------------- ##
## %union and --defines.  ##
## ---------------------- ##


AT_SETUP([%union and --defines])

AT_DATA([union.y],
[%union
{
  int   integer;
  char *string ;
}
%%
exp: {};
])

AT_CHECK([bison --defines union.y])

AT_CLEANUP([union.*])
