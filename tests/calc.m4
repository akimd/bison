#                                                        -*- Autoconf -*-

cat <<EOF

Simple Calculator.

EOF

## ---------------------------------------------------- ##
## Compile the grammar described in the documentation.  ##
## ---------------------------------------------------- ##

# We use integers to avoid dependencies upon the precision of doubles.
AT_SETUP(Compiling a grammar)

AT_DATA([calc.y],
[[/* Infix notation calculator--calc */

%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define YYSTYPE int

static int power (int base, int exponent);
static int read_signed_integer (FILE *stream);
extern void perror (const char *s);
%}

/* BISON Declarations */
%token NUM
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

/* Grammar follows */
%%
input:    /* empty string */
        | input line
;

line:     '\n'
        | exp '\n'  { printf ("%d", $1);     }
;

exp:      NUM                { $$ = $1;             }
        | exp '+' exp        { $$ = $1 + $3;        }
        | exp '-' exp        { $$ = $1 - $3;        }
        | exp '*' exp        { $$ = $1 * $3;        }
        | exp '/' exp        { $$ = $1 / $3;        }
        | '-' exp  %prec NEG { $$ = -$2;            }
        | exp '^' exp        { $$ = power ($1, $3); }
        | '(' exp ')'        { $$ = $2;             }
;
%%
FILE *yyin = stdin;

int
main (int argn, const char **argv)
{
  if (argn == 2)
    yyin = fopen (argv[1], "r");
  if (!stdin)
    {
      perror (argv[1]);
      exit (1);
    }
  yyparse ();
  return 0;
}

int
yyerror (const char *s)
{
  fprintf (stderr, "%s\n", s);
}

static int
read_signed_integer (FILE *stream)
{
  int c = getc (stream);
  int sign = 1;
  int n = 0;

  if (c == '-')
    {
      c = getc (stream);
      sign = -1;
    }

  while (isdigit (c))
    {
      n = 10 * n + (c - '0');
      c = getc (stream);
    }

  ungetc (c, stream);

  return sign * n;
}

/*---------------------------------------------------------------.
| Lexical analyzer returns an integer on the stack and the token |
| NUM, or the ASCII character read if not a number.  Skips all   |
| blanks and tabs, returns 0 for EOF.                            |
`---------------------------------------------------------------*/

int
yylex ()
{
  int c;

  /* Skip white space.  */
  while ((c = getc (yyin)) == ' ' || c == '\t')
    ;
  /* process numbers   */
  if (c == '.' || isdigit (c))
    {
      ungetc (c, yyin);
      yylval = read_signed_integer (yyin);
      return NUM;
    }
  /* Return end-of-file.  */
  if (c == EOF)
    return 0;
  /* Return single chars. */
  return c;
}

static int
power (int base, int exponent)
{
  int res = 1;
  if (exponent < 0)
    exit (1);
  for (/* Niente */; exponent; --exponent)
    res *= base;
  return res;
}
]])

# Specify the output files to avoid problems on different file systems.
AT_CHECK([bison calc.y -o calc.c], 0, [], [])
AT_CHECK([$CC $CFLAGS calc.c -o calc], 0, [], [])

# AT_CHECK_CALC(INPUT, OUTPUT)
# ----------------------------
# Run `calc' on INPUT, and expect OUTPUT.
AT_DEFINE([AT_CHECK_CALC],
[AT_CHECK([echo "$1" | calc], 0, [$2], [])])

# Test the priorities.
AT_CHECK_CALC([1 + 2 * 3],   [7])
AT_CHECK_CALC([1 + 2 * -3], [-5])

AT_CHECK_CALC([-1^2],  [-1])
AT_CHECK_CALC([(-1)^2], [1])

AT_CHECK_CALC([---1], [-1])

AT_CHECK_CALC([1 - 2 - 3],  [-4])
AT_CHECK_CALC([1 - (2 - 3)], [2])

AT_CHECK_CALC([2^2^3],  [256])
AT_CHECK_CALC([(2^2)^3], [64])

AT_CLEANUP(calc calc.c)
