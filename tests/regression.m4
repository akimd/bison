#                                                        -*- Autoconf -*-

cat <<EOF

Regression tests.

EOF

AT_SETUP(Duplicate string)

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

AT_CLEANUP(duplicate.*)
