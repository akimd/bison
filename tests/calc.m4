#                                                        -*- Autoconf -*-

cat <<EOF

Simple Calculator.

EOF


## ---------------------------------------------------- ##
## Compile the grammar described in the documentation.  ##
## ---------------------------------------------------- ##


# ------------------------- #
# Helping Autotest macros.  #
# ------------------------- #


# _AT_DATA_CALC_Y($1, $2, $3, [CPP-DIRECTIVES])
# ---------------------------------------------
# Produce `calc.y'.  Don't call this macro directly, because it contains
# some occurrences of `$1' etc. which will be interpreted by m4.  So
# you should call it with $1, $2, and $3 as arguments, which is what
# AT_DATA_CALC_Y does.
AT_DEFINE([_AT_DATA_CALC_Y],
[ifelse([$1$2$3],
        $[1]$[2]$[3], [],
        [errprint([$0: Invalid arguments: $@
])m4exit(1)])dnl
AT_DATA([calc.y],
[[/* Infix notation calculator--calc */

%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
]$4[

static int power (int base, int exponent);
static int read_signed_integer (FILE *stream);
static void yyerror (const char *s);
static int yylex (void);
extern void perror (const char *s);
%}

/* BISON Declarations */
%token NUM

%nonassoc '=' /* comparison	       */
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

/* Grammar follows */
%%
input:
  /* empty string */
| input line
;

line:
  '\n'
| exp '\n'
;

exp:
  NUM                { $$ = $1;             }
| exp '=' exp
  {
     if ($1 != $3)
       printf ("calc: error: %d != %d\n", $1, $3);
     $$ = $1 == $3;
  }
| exp '+' exp        { $$ = $1 + $3;        }
| exp '-' exp        { $$ = $1 - $3;        }
| exp '*' exp        { $$ = $1 * $3;        }
| exp '/' exp        { $$ = $1 / $3;        }
| '-' exp  %prec NEG { $$ = -$2;            }
| exp '^' exp        { $$ = power ($1, $3); }
| '(' exp ')'        { $$ = $2;             }
;
%%
/* The input. */
FILE *yyin;

static void
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

static int
yylex (void)
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

int
main (int argn, const char **argv)
{
  if (argn == 2)
    yyin = fopen (argv[1], "r");
  else
     yyin = stdin;

  if (!stdin)
    {
      perror (argv[1]);
      exit (1);
    }

#if YYDEBUG
  yydebug = 1;
#endif
  yyparse ();
  return 0;
}
]])
])# _AT_DATA_CALC_Y


# AT_DATA_CALC_Y([BISON-OPTIONS])
# -------------------------------
# Produce `calc.y'.
AT_DEFINE([AT_DATA_CALC_Y],
[_AT_DATA_CALC_Y($[1], $[2], $[3],
                 [ifelse(regexp([$1], [--yyerror-verbose]),
                         [-1], [],
                         [[#define YYERROR_VERBOSE]])])])



# _AT_CHECK_CALC(BISON-OPTIONS, INPUT)
# ------------------------------------
# Run `calc' on INPUT and expect no STDOUT nor STDERR.
# If `--debug' is passed to bison, discard all the debugging traces
# preserving only the `parse errors'.  Note that since there should be
# none, the `grep' will fail with exit status 1.
AT_DEFINE([_AT_CHECK_CALC],
[ifelse(regexp([$1], [--debug]),
  [-1],
     [AT_CHECK([echo "$2" | calc],
               [0], [], [])],
     [AT_CHECK([echo "$2" | calc 2>&1 >/dev/null | grep 'parse error' >&2],
               [1], [], [])])])


# _AT_CHECK_CALC_ERROR(BISON-OPTIONS, INPUT, [IF-YYERROR-VERBOSE])
# ----------------------------------------------------------------
# Run `calc' on INPUT, and expect STDERR.
AT_DEFINE([_AT_CHECK_CALC_ERROR],
[AT_CHECK([echo "$2" | calc 2>&1 >/dev/null | grep 'parse error' >&2], 0,
          [],
          [parse error[]ifelse(regexp([$1], [--yyerror-verbose]),
                               [-1], [], [$3])
])])


# AT_CHECK_CALC([BISON-OPTIONS], [PARSER-EXPECTED-STDERR])
# --------------------------------------------------------
# Start a testing chunk which compiles `calc' grammar with
# BISON-OPTIONS, and performs several tests over the parser.
AT_DEFINE([AT_CHECK_CALC],
[# We use integers to avoid dependencies upon the precision of doubles.
AT_SETUP([Calculator $1])

AT_DATA_CALC_Y([$1])

# Specify the output files to avoid problems on different file systems.
AT_CHECK([bison calc.y -o calc.c patsubst([$1], [--yyerror-verbose])],
         [0], [], [])
AT_CHECK([$CC $CFLAGS calc.c -o calc], 0, [], [])

# Test the priorities.
_AT_CHECK_CALC([$1],
[1 + 2 * 3 = 7
1 + 2 * -3 = -5

-1^2 = -1
(-1)^2 = 1

---1 = -1

1 - 2 - 3 = -4
1 - (2 - 3) = 2

2^2^3 = 256
(2^2)^3 = 64], [$2])

# Some parse errors.
_AT_CHECK_CALC_ERROR([$1], [+1],
                     [, unexpected `'+''])
_AT_CHECK_CALC_ERROR([$1], [1//2],
                     [, unexpected `'/'', expecting `NUM' or `'-'' or `'(''])
_AT_CHECK_CALC_ERROR([$1], [error],
                     [, unexpected `$undefined.'])
_AT_CHECK_CALC_ERROR([$1], [1 = 2 = 3],
                     [, unexpected `'=''])

AT_CLEANUP(calc calc.c calc.h calc.output)
])# AT_CHECK_CALC




# ------------------ #
# Test the parsers.  #
# ------------------ #

AT_CHECK_CALC()
# This one is very suspicious.  The test fails, but it might be normal.
AT_CHECK_CALC([--raw])

AT_CHECK_CALC([--defines])
AT_CHECK_CALC([--name-prefix=calc])
AT_CHECK_CALC([--verbose])
AT_CHECK_CALC([--yacc])
AT_CHECK_CALC([--yyerror-verbose])
AT_CHECK_CALC([--defines --name-prefix=calc --verbose --yacc --yyerror-verbose])

# When --debug, a lot of data is sent to STDERR, we can't test it.
AT_CHECK_CALC([--debug])
AT_CHECK_CALC([--debug --defines --name-prefix=calc --verbose --yacc --yyerror-verbose])
