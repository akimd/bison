/* Token-reader for Bison's input parser,
   Copyright 1984, 1986, 1989, 1992, 2000 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include "system.h"
#include "getargs.h"
#include "files.h"
#include "getopt.h"		/* for optarg */
#include "symtab.h"
#include "lex.h"
#include "xalloc.h"
#include "complain.h"
#include "gram.h"
#include "quote.h"

/* Buffer for storing the current token.  */
char *token_buffer;

/* Allocated size of token_buffer, not including space for terminator.  */
static int maxtoken;

bucket *symval;
int numval;

static int unlexed;		/* these two describe a token to be reread */
static bucket *unlexed_symval;	/* by the next call to lex */


void
init_lex (void)
{
  maxtoken = 100;
  token_buffer = XCALLOC (char, maxtoken + 1);
  unlexed = -1;
}


static char *
grow_token_buffer (char *p)
{
  int offset = p - token_buffer;
  maxtoken *= 2;
  token_buffer = XREALLOC (token_buffer, char, maxtoken + 1);
  return token_buffer + offset;
}


int
skip_white_space (void)
{
  int c;
  int inside;

  c = getc (finput);

  for (;;)
    {
      int cplus_comment;

      switch (c)
	{
	case '/':
	  /* FIXME: Should probably be merged with copy_comment.  */
	  c = getc (finput);
	  if (c != '*' && c != '/')
	    {
	      complain (_("unexpected `/' found and ignored"));
	      break;
	    }
	  cplus_comment = (c == '/');

	  c = getc (finput);

	  inside = 1;
	  while (inside)
	    {
	      if (!cplus_comment && c == '*')
		{
		  while (c == '*')
		    c = getc (finput);

		  if (c == '/')
		    {
		      inside = 0;
		      c = getc (finput);
		    }
		}
	      else if (c == '\n')
		{
		  lineno++;
		  if (cplus_comment)
		    inside = 0;
		  c = getc (finput);
		}
	      else if (c == EOF)
		fatal (_("unterminated comment"));
	      else
		c = getc (finput);
	    }

	  break;

	case '\n':
	  lineno++;

	case ' ':
	case '\t':
	case '\f':
	  c = getc (finput);
	  break;

	default:
	  return c;
	}
    }
}


/*-----------------------------------------------------.
| Do a getc, but give error message if EOF encountered |
`-----------------------------------------------------*/

static int
xgetc (FILE *f)
{
  int c = getc (f);
  if (c == EOF)
    fatal (_("unexpected end of file"));
  return c;
}


/*------------------------------------------------------------------.
| Read one literal character from finput.  Process \ escapes.       |
| Append the normalized string version of the char to *PP.  Assign  |
| the character code to *PCODE. Return 1 unless the character is an |
| unescaped `term' or \n report error for \n                        |
`------------------------------------------------------------------*/

static int
literalchar (char **pp, int *pcode, char term)
{
  int c;
  char *p;
  int code;
  int wasquote = 0;

  c = xgetc (finput);
  if (c == '\n')
    {
      complain (_("unescaped newline in constant"));
      ungetc (c, finput);
      code = '?';
      wasquote = 1;
    }
  else if (c != '\\')
    {
      code = c;
      if (c == term)
	wasquote = 1;
    }
  else
    {
      c = xgetc (finput);
      if (c == 't')
	code = '\t';
      else if (c == 'n')
	code = '\n';
      else if (c == 'a')
	code = '\007';
      else if (c == 'r')
	code = '\r';
      else if (c == 'f')
	code = '\f';
      else if (c == 'b')
	code = '\b';
      else if (c == 'v')
	code = '\013';
      else if (c == '\\')
	code = '\\';
      else if (c == '\'')
	code = '\'';
      else if (c == '\"')
	code = '\"';
      else if (c <= '7' && c >= '0')
	{
	  code = 0;
	  while (c <= '7' && c >= '0')
	    {
	      code = (code * 8) + (c - '0');
	      if (code >= 256 || code < 0)
		{
		  complain (_("octal value outside range 0...255: `\\%o'"),
			    code);
		  code &= 0xFF;
		  break;
		}
	      c = xgetc (finput);
	    }
	  ungetc (c, finput);
	}
      else if (c == 'x')
	{
	  c = xgetc (finput);
	  code = 0;
	  while (1)
	    {
	      if (c >= '0' && c <= '9')
		code *= 16, code += c - '0';
	      else if (c >= 'a' && c <= 'f')
		code *= 16, code += c - 'a' + 10;
	      else if (c >= 'A' && c <= 'F')
		code *= 16, code += c - 'A' + 10;
	      else
		break;
	      if (code >= 256 || code < 0)
		{
		  complain (_("hexadecimal value above 255: `\\x%x'"), code);
		  code &= 0xFF;
		  break;
		}
	      c = xgetc (finput);
	    }
	  ungetc (c, finput);
	}
      else
	{
	  char buf [] = "c";
	  buf[0] = c;
	  complain (_("unknown escape sequence: `\\' followed by `%s'"),
		    quote (buf));
	  code = '?';
	}
    }				/* has \ */

  /* now fill token_buffer with the canonical name for this character
     as a literal token.  Do not use what the user typed,
     so that `\012' and `\n' can be interchangeable.  */

  p = *pp;
  if (code == term && wasquote)
    *p++ = code;
  else if (code == '\\')
    {
      *p++ = '\\';
      *p++ = '\\';
    }
  else if (code == '\'')
    {
      *p++ = '\\';
      *p++ = '\'';
    }
  else if (code == '\"')
    {
      *p++ = '\\';
      *p++ = '\"';
    }
  else if (code >= 040 && code < 0177)
    *p++ = code;
  else if (code == '\t')
    {
      *p++ = '\\';
      *p++ = 't';
    }
  else if (code == '\n')
    {
      *p++ = '\\';
      *p++ = 'n';
    }
  else if (code == '\r')
    {
      *p++ = '\\';
      *p++ = 'r';
    }
  else if (code == '\v')
    {
      *p++ = '\\';
      *p++ = 'v';
    }
  else if (code == '\b')
    {
      *p++ = '\\';
      *p++ = 'b';
    }
  else if (code == '\f')
    {
      *p++ = '\\';
      *p++ = 'f';
    }
  else
    {
      *p++ = '\\';
      *p++ = code / 0100 + '0';
      *p++ = ((code / 010) & 07) + '0';
      *p++ = (code & 07) + '0';
    }
  *pp = p;
  *pcode = code;
  return !wasquote;
}


void
unlex (int token)
{
  unlexed = token;
  unlexed_symval = symval;
}

/*-----------------------------------------------------------------.
| We just read `<' from FIN.  Store in TOKEN_BUFFER, the type name |
| specified between the `<...>'.                                   |
`-----------------------------------------------------------------*/

void
read_type_name (FILE *fin)
{
  char *p = token_buffer;
  int c = getc (fin);

  while (c != '>')
    {
      if (c == EOF)
	fatal (_("unterminated type name at end of file"));
      if (c == '\n')
	{
	  complain (_("unterminated type name"));
	  ungetc (c, fin);
	  break;
	}

      if (p == token_buffer + maxtoken)
	p = grow_token_buffer (p);

      *p++ = c;
      c = getc (fin);
    }
  *p = 0;
}


token_t
lex (void)
{
  int c;
  char *p;

  if (unlexed >= 0)
    {
      symval = unlexed_symval;
      c = unlexed;
      unlexed = -1;
      return c;
    }

  c = skip_white_space ();
  /* for error messages (token buffer always valid) */
  *token_buffer = c;
  token_buffer[1] = 0;

  switch (c)
    {
    case EOF:
      strcpy (token_buffer, "EOF");
      return tok_eof;

    case 'A':    case 'B':    case 'C':    case 'D':    case 'E':
    case 'F':    case 'G':    case 'H':    case 'I':    case 'J':
    case 'K':    case 'L':    case 'M':    case 'N':    case 'O':
    case 'P':    case 'Q':    case 'R':    case 'S':    case 'T':
    case 'U':    case 'V':    case 'W':    case 'X':    case 'Y':
    case 'Z':
    case 'a':    case 'b':    case 'c':    case 'd':    case 'e':
    case 'f':    case 'g':    case 'h':    case 'i':    case 'j':
    case 'k':    case 'l':    case 'm':    case 'n':    case 'o':
    case 'p':    case 'q':    case 'r':    case 's':    case 't':
    case 'u':    case 'v':    case 'w':    case 'x':    case 'y':
    case 'z':
    case '.':    case '_':

      p = token_buffer;
      while (isalnum (c) || c == '_' || c == '.')
	{
	  if (p == token_buffer + maxtoken)
	    p = grow_token_buffer (p);

	  *p++ = c;
	  c = getc (finput);
	}

      *p = 0;
      ungetc (c, finput);
      symval = getsym (token_buffer);
      return tok_identifier;

    case '0':    case '1':    case '2':    case '3':    case '4':
    case '5':    case '6':    case '7':    case '8':    case '9':
      {
	numval = 0;

	p = token_buffer;
	while (isdigit (c))
	  {
	    if (p == token_buffer + maxtoken)
	      p = grow_token_buffer (p);

	    *p++ = c;
	    numval = numval * 10 + c - '0';
	    c = getc (finput);
	  }
	*p = 0;
	ungetc (c, finput);
	return tok_number;
      }

    case '\'':
      /* parse the literal token and compute character code in  code  */

      translations = -1;
      {
	int code, discode;
	char discard[10], *dp;

	p = token_buffer;
	*p++ = '\'';
	literalchar (&p, &code, '\'');

	c = getc (finput);
	if (c != '\'')
	  {
	    complain (_("use \"...\" for multi-character literal tokens"));
	    while (1)
	      {
		dp = discard;
		if (!literalchar (&dp, &discode, '\''))
		  break;
	      }
	  }
	*p++ = '\'';
	*p = 0;
	symval = getsym (token_buffer);
	symval->class = token_sym;
	if (!symval->user_token_number)
	  symval->user_token_number = code;
	return tok_identifier;
      }

    case '\"':
      /* parse the literal string token and treat as an identifier */

      translations = -1;
      {
	int code;		/* ignored here */
	p = token_buffer;
	*p++ = '\"';
	/* Read up to and including ".  */
	while (literalchar (&p, &code, '\"'))
	  {
	    if (p >= token_buffer + maxtoken - 4)
	      p = grow_token_buffer (p);
	  }
	*p = 0;

	symval = getsym (token_buffer);
	symval->class = token_sym;

	return tok_identifier;
      }

    case ',':
      return tok_comma;

    case ':':
      return tok_colon;

    case ';':
      return tok_semicolon;

    case '|':
      return tok_bar;

    case '{':
      return tok_left_curly;

    case '=':
      do
	{
	  c = getc (finput);
	  if (c == '\n')
	    lineno++;
	}
      while (c == ' ' || c == '\n' || c == '\t');

      if (c == '{')
	{
	  strcpy (token_buffer, "={");
	  return tok_left_curly;
	}
      else
	{
	  ungetc (c, finput);
	  return tok_illegal;
	}

    case '<':
      read_type_name (finput);
      return tok_typename;

    case '%':
      return parse_percent_token ();

    default:
      return tok_illegal;
    }
}

/* the following table dictates the action taken for the various %
   directives.  A set_flag value causes the named flag to be set.  A
   retval action returns the code.  */
struct percent_table_struct
{
  const char *name;
  void *set_flag;
  int retval;
};

struct percent_table_struct percent_table[] =
{
  { "token",		NULL,			tok_token },
  { "term",		NULL,			tok_token },
  { "nterm",		NULL,			tok_nterm },
  { "type",		NULL,			tok_type },
  { "guard",		NULL,			tok_guard },
  { "union",		NULL,			tok_union },
  { "expect",		NULL,			tok_expect },
  { "thong",		NULL,			tok_thong },
  { "start",		NULL,			tok_start },
  { "left",		NULL,			tok_left },
  { "right",		NULL,			tok_right },
  { "nonassoc",		NULL,			tok_nonassoc },
  { "binary",		NULL,			tok_nonassoc },
  { "prec",		NULL,			tok_prec },
  { "locations",	&locations_flag,	tok_noop },		/* -l */
  { "no_lines",		&no_lines_flag,		tok_noop },		/* -l */
  { "raw",		&raw_flag,		tok_noop },		/* -r */
  { "token_table",	&token_table_flag,	tok_noop },		/* -k */
  { "yacc",		&yacc_flag,		tok_noop },		/* -y */
  { "fixed_output_files",&yacc_flag,		tok_noop },		/* -y */
  { "defines",		&defines_flag,		tok_noop },		/* -d */
  { "no_parser",	&no_parser_flag,	tok_noop },		/* -n */
#if 0
  /* For the time being, this is not enabled yet, while it's possible
     though, since we use obstacks.  The only risk is with semantic
     parsers which will output an `include' of an output file: be sure
     that the naem included is indeed the name of the output file.  */
  { "output_file",	&spec_outfile,		tok_setopt },	/* -o */
  { "file_prefix",	&spec_file_prefix,	tok_setopt },	/* -b */
  { "name_prefix",	&spec_name_prefix,	tok_setopt },	/* -p */
#endif
  { "verbose",		&verbose_flag,		tok_noop },		/* -v */
  { "debug",		&debug_flag,		tok_noop },		/* -t */
  { "semantic_parser",	&semantic_parser,	tok_noop },
  { "pure_parser",	&pure_parser,		tok_noop },
/*    {"help", <print usage stmt>, tok_noop}, *//* -h */
/*    {"version", <print version number> ,  tok_noop}, *//* -V */
  { NULL, NULL, tok_illegal}
};

/* Parse a token which starts with %.
   Assumes the % has already been read and discarded.  */

int
parse_percent_token (void)
{
  int c;
  char *p;
  struct percent_table_struct *tx;

  p = token_buffer;
  c = getc (finput);
  *p++ = '%';
  *p++ = c;			/* for error msg */
  *p = 0;

  switch (c)
    {
    case '%':
      return tok_two_percents;

    case '{':
      return tok_percent_left_curly;

    case '<':
      return tok_left;

    case '>':
      return tok_right;

    case '2':
      return tok_nonassoc;

    case '0':
      return tok_token;

    case '=':
      return tok_prec;
    }
  if (!isalpha (c))
    return tok_illegal;

  p = token_buffer;
  *p++ = '%';
  while (isalpha (c) || c == '_' || c == '-')
    {
      if (p == token_buffer + maxtoken)
	p = grow_token_buffer (p);

      if (c == '-')
	c = '_';
      *p++ = c;
      c = getc (finput);
    }

  ungetc (c, finput);

  *p = 0;

  /* table lookup % directive */
  for (tx = percent_table; tx->name; tx++)
    if (strcmp (token_buffer + 1, tx->name) == 0)
      break;

  if (tx->retval == tok_setopt)
    {
      *((char **) (tx->set_flag)) = optarg;
      return tok_noop;
    }
  if (tx->set_flag)
    {
      *((int *) (tx->set_flag)) = 1;
      return tok_noop;
    }
  return tx->retval;
}
