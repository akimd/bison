/* Input parser for bison
   Copyright (C) 1984, 1986, 1989, 1992, 1998, 2000
   Free Software Foundation, Inc.

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
#include "xalloc.h"
#include "symtab.h"
#include "lex.h"
#include "gram.h"
#include "complain.h"
#include "output.h"
#include "reader.h"
#include "conflicts.h"

extern char *printable_version PARAMS ((int));

/* Number of slots allocated (but not necessarily used yet) in `rline'  */
static int rline_allocated;

typedef struct symbol_list
{
  struct symbol_list *next;
  bucket *sym;
  bucket *ruleprec;
}
symbol_list;

int lineno;
char **tags;
short *user_toknums;
static symbol_list *grammar;
static int start_flag;
static bucket *startval;

/* Nonzero if components of semantic values are used, implying
   they must be unions.  */
static int value_components_used;

/* Nonzero if %union has been seen.  */
static int typed;

/* Incremented for each %left, %right or %nonassoc seen */
static int lastprec;

/* Incremented for each generated symbol */
static int gensym_count;

static bucket *errtoken;
static bucket *undeftoken;


/*===================\
| Low level lexing.  |
\===================*/

static void
skip_to_char (int target)
{
  int c;
  if (target == '\n')
    complain (_("   Skipping to next \\n"));
  else
    complain (_("   Skipping to next %c"), target);

  do
    c = skip_white_space ();
  while (c != target && c != EOF);
  if (c != EOF)
    ungetc (c, finput);
}


/*---------------------------------------------------------.
| Read a signed integer from STREAM and return its value.  |
`---------------------------------------------------------*/

static inline int
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

/*-------------------------------------------------------------------.
| Dump the string from FINPUT to FOUTPUT.  MATCH is the delimiter of |
| the string (either ' or ").                                        |
`-------------------------------------------------------------------*/

static inline void
copy_string (FILE *fin, FILE *fout, int match)
{
  int c;

  putc (match, fout);
  c = getc (fin);

  while (c != match)
    {
      if (c == EOF)
	fatal (_("unterminated string at end of file"));
      if (c == '\n')
	{
	  complain (_("unterminated string"));
	  ungetc (c, fin);
	  c = match;		/* invent terminator */
	  continue;
	}

      putc (c, fout);

      if (c == '\\')
	{
	  c = getc (fin);
	  if (c == EOF)
	    fatal (_("unterminated string at end of file"));
	  putc (c, fout);
	  if (c == '\n')
	    lineno++;
	}

      c = getc (fin);
    }

  putc (c, fout);
}


/*---------------------------------------------------------------.
| Dump the comment from IN to OUT1 and OUT2.  C is either `*' or |
| `/', depending upon the type of comments used.  OUT2 might be  |
| NULL.                                                          |
`---------------------------------------------------------------*/

static inline void
copy_comment2 (FILE *in, FILE *out1, FILE *out2, int c)
{
  int cplus_comment;
  int ended;

  cplus_comment = (c == '/');
  putc (c, out1);
  if (out2)
    putc (c, out2);
  c = getc (in);

  ended = 0;
  while (!ended)
    {
      if (!cplus_comment && c == '*')
	{
	  while (c == '*')
	    {
	      putc (c, out1);
	      if (out2)
		putc (c, out2);
	      c = getc (in);
	    }

	  if (c == '/')
	    {
	      putc (c, out1);
	      if (out2)
		putc (c, out2);
	      ended = 1;
	    }
	}
      else if (c == '\n')
	{
	  lineno++;
	  putc (c, out1);
	  if (out2)
	    putc (c, out2);
	  if (cplus_comment)
	    ended = 1;
	  else
	    c = getc (in);
	}
      else if (c == EOF)
	fatal (_("unterminated comment"));
      else
	{
	  putc (c, out1);
	  if (out2)
	    putc (c, out2);
	  c = getc (in);
	}
    }
}


/*------------------------------------------------------------.
| Dump the comment from FIN to FOUT.  C is either `*' or `/', |
| depending upon the type of comments used.                   |
`------------------------------------------------------------*/

static inline void
copy_comment (FILE *fin, FILE *fout, int c)
{
  copy_comment2 (fin, fout, NULL, c);
}


/*-----------------------------------------------------------------.
| FIN is pointing to a location (i.e., a `@').  Output to FOUT a   |
| reference to this location. STACK_OFFSET is the number of values |
| in the current rule so far, which says where to find `$0' with   |
| respect to the top of the stack.                                 |
`-----------------------------------------------------------------*/

static inline void
copy_at (FILE *fin, FILE *fout, int stack_offset)
{
  int c;

  c = getc (fin);
  if (c == '$')
    {
      fprintf (fout, "yyloc");
      locations_flag = 1;
    }
  else if (isdigit (c) || c == '-')
    {
      int n;

      ungetc (c, fin);
      n = read_signed_integer (fin);

      fprintf (fout, "yylsp[%d]", n - stack_offset);
      locations_flag = 1;
    }
  else
    complain (_("@%s is invalid"), printable_version (c));
}

/*-------------------------------------------------------------------.
| Copy the contents of a `%{ ... %}' into the definitions file.  The |
| `%{' has already been read.  Return after reading the `%}'.        |
`-------------------------------------------------------------------*/

static void
copy_definition (void)
{
  int c;
  /* -1 while reading a character if prev char was %. */
  int after_percent;

  if (!no_lines_flag)
    fprintf (fattrs, "#line %d \"%s\"\n", lineno, infile);

  after_percent = 0;

  c = getc (finput);

  for (;;)
    {
      switch (c)
	{
	case '\n':
	  putc (c, fattrs);
	  lineno++;
	  break;

	case '%':
	  after_percent = -1;
	  break;

	case '\'':
	case '"':
	  copy_string (finput, fattrs, c);
	  break;

	case '/':
	  putc (c, fattrs);
	  c = getc (finput);
	  if (c != '*' && c != '/')
	    continue;
	  copy_comment (finput, fattrs, c);
	  break;

	case EOF:
	  fatal ("%s", _("unterminated `%{' definition"));

	default:
	  putc (c, fattrs);
	}

      c = getc (finput);

      if (after_percent)
	{
	  if (c == '}')
	    return;
	  putc ('%', fattrs);
	}
      after_percent = 0;

    }

}


/*-------------------------------------------------------------------.
| Parse what comes after %token or %nterm.  For %token, WHAT_IS is   |
| token_sym and WHAT_IS_NOT is nterm_sym.  For %nterm, the arguments |
| are reversed.                                                      |
`-------------------------------------------------------------------*/

static void
parse_token_decl (symbol_class what_is, symbol_class what_is_not)
{
  int token = 0;
  char *typename = 0;
  struct bucket *symbol = NULL;	/* pts to symbol being defined */

  for (;;)
    {
      int tmp_char = ungetc (skip_white_space (), finput);

      if (tmp_char == '%')
	return;
      if (tmp_char == EOF)
	fatal (_("Premature EOF after %s"), token_buffer);

      token = lex ();
      if (token == COMMA)
	{
	  symbol = NULL;
	  continue;
	}
      if (token == TYPENAME)
	{
	  typename = xstrdup (token_buffer);
	  value_components_used = 1;
	  symbol = NULL;
	}
      else if (token == IDENTIFIER && *symval->tag == '\"' && symbol)
	{
	  if (symval->alias)
	    warn (_("symbol `%s' used more than once as a literal string"),
		  symval->tag);
	  else if (symbol->alias)
	    warn (_("symbol `%s' given more than one literal string"),
		  symbol->tag);
	  else
	    {
	      symval->class = token_sym;
	      symval->type_name = typename;
	      symval->user_token_number = symbol->user_token_number;
	      symbol->user_token_number = SALIAS;
	      symval->alias = symbol;
	      symbol->alias = symval;
	      /* symbol and symval combined are only one symbol */
	      nsyms--;
	    }
	  translations = 1;
 	  symbol = NULL;
	}
      else if (token == IDENTIFIER)
	{
	  int oldclass = symval->class;
	  symbol = symval;

	  if (symbol->class == what_is_not)
	    complain (_("symbol %s redefined"), symbol->tag);
	  symbol->class = what_is;
	  if (what_is == nterm_sym && oldclass != nterm_sym)
	    symbol->value = nvars++;

	  if (typename)
	    {
	      if (symbol->type_name == NULL)
		symbol->type_name = typename;
	      else if (strcmp (typename, symbol->type_name) != 0)
		complain (_("type redeclaration for %s"), symbol->tag);
	    }
	}
      else if (symbol && token == NUMBER)
	{
	  symbol->user_token_number = numval;
	  translations = 1;
	}
      else
	{
	  complain (_("`%s' is invalid in %s"),
		    token_buffer, (what_is == token_sym) ? "%token" : "%nterm");
	  skip_to_char ('%');
	}
    }

}


/*------------------------------.
| Parse what comes after %start |
`------------------------------*/

static void
parse_start_decl (void)
{
  if (start_flag)
    complain (_("multiple %s declarations"), "%start");
  if (lex () != IDENTIFIER)
    complain (_("invalid %s declaration"), "%start");
  else
    {
      start_flag = 1;
      startval = symval;
    }
}



/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol n in rule rule.                                        |
`--------------------------------------------------------------*/

static char *
get_type_name (int n, symbol_list * rule)
{
  int i;
  symbol_list *rp;

  if (n < 0)
    {
      complain (_("invalid $ value"));
      return NULL;
    }

  rp = rule;
  i = 0;

  while (i < n)
    {
      rp = rp->next;
      if (rp == NULL || rp->sym == NULL)
	{
	  complain (_("invalid $ value"));
	  return NULL;
	}
      i++;
    }

  return rp->sym->type_name;
}


/*-----------------------------------------------------------.
| read in a %type declaration and record its information for |
| get_type_name to access                                    |
`-----------------------------------------------------------*/

static void
parse_type_decl (void)
{
  char *name;

  if (lex () != TYPENAME)
    {
      complain ("%s", _("%type declaration has no <typename>"));
      skip_to_char ('%');
      return;
    }

  name = xstrdup (token_buffer);

  for (;;)
    {
      int t;
      int tmp_char = ungetc (skip_white_space (), finput);

      if (tmp_char == '%')
	return;
      if (tmp_char == EOF)
	fatal (_("Premature EOF after %s"), token_buffer);

      t = lex ();

      switch (t)
	{

	case COMMA:
	case SEMICOLON:
	  break;

	case IDENTIFIER:
	  if (symval->type_name == NULL)
	    symval->type_name = name;
	  else if (strcmp (name, symval->type_name) != 0)
	    complain (_("type redeclaration for %s"), symval->tag);

	  break;

	default:
	  complain (_("invalid %%type declaration due to item: %s"),
		    token_buffer);
	  skip_to_char ('%');
	}
    }
}



/*----------------------------------------------------------------.
| Read in a %left, %right or %nonassoc declaration and record its |
| information.                                                    |
`----------------------------------------------------------------*/

static void
parse_assoc_decl (associativity assoc)
{
  char *name = NULL;
  int prev = 0;

  lastprec++;			/* Assign a new precedence level, never 0.  */

  for (;;)
    {
      int t;
      int tmp_char = ungetc (skip_white_space (), finput);

      if (tmp_char == '%')
	return;
      if (tmp_char == EOF)
	fatal (_("Premature EOF after %s"), token_buffer);

      t = lex ();

      switch (t)
	{
	case TYPENAME:
	  name = xstrdup (token_buffer);
	  break;

	case COMMA:
	  break;

	case IDENTIFIER:
	  if (symval->prec != 0)
	    complain (_("redefining precedence of %s"), symval->tag);
	  symval->prec = lastprec;
	  symval->assoc = assoc;
	  if (symval->class == nterm_sym)
	    complain (_("symbol %s redefined"), symval->tag);
	  symval->class = token_sym;
	  if (name)
	    {			/* record the type, if one is specified */
	      if (symval->type_name == NULL)
		symval->type_name = name;
	      else if (strcmp (name, symval->type_name) != 0)
		complain (_("type redeclaration for %s"), symval->tag);
	    }
	  break;

	case NUMBER:
	  if (prev == IDENTIFIER)
	    {
	      symval->user_token_number = numval;
	      translations = 1;
	    }
	  else
	    {
	      complain (_
			("invalid text (%s) - number should be after identifier"),
token_buffer);
	      skip_to_char ('%');
	    }
	  break;

	case SEMICOLON:
	  return;

	default:
	  complain (_("unexpected item: %s"), token_buffer);
	  skip_to_char ('%');
	}

      prev = t;

    }
}



/*-------------------------------------------------------------------.
| Copy the union declaration into fattrs (and fdefines), where it is |
| made into the definition of YYSTYPE, the type of elements of the   |
| parser value stack.                                                |
`-------------------------------------------------------------------*/

static void
parse_union_decl (void)
{
  int c;
  int count = 0;

  if (typed)
    complain (_("multiple %s declarations"), "%union");

  typed = 1;

  if (!no_lines_flag)
    fprintf (fattrs, "\n#line %d \"%s\"\n", lineno, infile);
  else
    fprintf (fattrs, "\n");

  fprintf (fattrs, "typedef union");
  if (fdefines)
    fprintf (fdefines, "typedef union");

  c = getc (finput);

  while (c != EOF)
    {
      putc (c, fattrs);
      if (fdefines)
	putc (c, fdefines);

      switch (c)
	{
	case '\n':
	  lineno++;
	  break;

	case '/':
	  c = getc (finput);
	  if (c != '*' && c != '/')
	    continue;
	  copy_comment2 (finput, fattrs, fdefines, c);
	  break;


	case '{':
	  count++;
	  break;

	case '}':
	  if (count == 0)
	    complain (_("unmatched %s"), "`}'");
	  count--;
	  if (count <= 0)
	    {
	      fprintf (fattrs, " YYSTYPE;\n");
	      if (fdefines)
		fprintf (fdefines, " YYSTYPE;\n");
	      /* JF don't choke on trailing semi */
	      c = skip_white_space ();
	      if (c != ';')
		ungetc (c, finput);
	      return;
	    }
	}

      c = getc (finput);
    }
}


/*-------------------------------------------------------.
| Parse the declaration %expect N which says to expect N |
| shift-reduce conflicts.                                |
`-------------------------------------------------------*/

static void
parse_expect_decl (void)
{
  int c;
  int count;
  char buffer[20];

  c = getc (finput);
  while (c == ' ' || c == '\t')
    c = getc (finput);

  count = 0;
  while (c >= '0' && c <= '9')
    {
      if (count < sizeof(buffer) - 1)
	buffer[count++] = c;
      c = getc (finput);
    }
  buffer[count] = 0;

  ungetc (c, finput);

  if (count <= 0 || count > 10)
    complain ("%s", _("argument of %expect is not an integer"));
  expected_conflicts = atoi (buffer);
}


/*-------------------------------------------------------------------.
| Parse what comes after %thong.  the full syntax is                 |
|                                                                    |
|                %thong <type> token number literal                  |
|                                                                    |
| the <type> or number may be omitted.  The number specifies the     |
| user_token_number.                                                 |
|                                                                    |
| Two symbols are entered in the table, one for the token symbol and |
| one for the literal.  Both are given the <type>, if any, from the  |
| declaration.  The ->user_token_number of the first is SALIAS and   |
| the ->user_token_number of the second is set to the number, if     |
| any, from the declaration.  The two symbols are linked via         |
| pointers in their ->alias fields.                                  |
|                                                                    |
| During OUTPUT_DEFINES_TABLE, the symbol is reported thereafter,    |
| only the literal string is retained it is the literal string that  |
| is output to yytname                                               |
`-------------------------------------------------------------------*/

static void
parse_thong_decl (void)
{
  int token;
  struct bucket *symbol;
  char *typename = 0;
  int usrtoknum;

  translations = 1;
  token = lex ();		/* fetch typename or first token */
  if (token == TYPENAME)
    {
      typename = xstrdup (token_buffer);
      value_components_used = 1;
      token = lex ();		/* fetch first token */
    }

  /* process first token */

  if (token != IDENTIFIER)
    {
      complain (_("unrecognized item %s, expected an identifier"),
		token_buffer);
      skip_to_char ('%');
      return;
    }
  symval->class = token_sym;
  symval->type_name = typename;
  symval->user_token_number = SALIAS;
  symbol = symval;

  token = lex ();		/* get number or literal string */

  if (token == NUMBER)
    {
      usrtoknum = numval;
      token = lex ();		/* okay, did number, now get literal */
    }
  else
    usrtoknum = 0;

  /* process literal string token */

  if (token != IDENTIFIER || *symval->tag != '\"')
    {
      complain (_("expected string constant instead of %s"), token_buffer);
      skip_to_char ('%');
      return;
    }
  symval->class = token_sym;
  symval->type_name = typename;
  symval->user_token_number = usrtoknum;

  symval->alias = symbol;
  symbol->alias = symval;

  nsyms--;			/* symbol and symval combined are only one symbol */
}


/*----------------------------------------------------------------.
| Read from finput until `%%' is seen.  Discard the `%%'.  Handle |
| any `%' declarations, and copy the contents of any `%{ ... %}'  |
| groups to fattrs.                                               |
`----------------------------------------------------------------*/

static void
read_declarations (void)
{
  int c;
  int tok;

  for (;;)
    {
      c = skip_white_space ();

      if (c == '%')
	{
	  tok = parse_percent_token ();

	  switch (tok)
	    {
	    case TWO_PERCENTS:
	      return;

	    case PERCENT_LEFT_CURLY:
	      copy_definition ();
	      break;

	    case TOKEN:
	      parse_token_decl (token_sym, nterm_sym);
	      break;

	    case NTERM:
	      parse_token_decl (nterm_sym, token_sym);
	      break;

	    case TYPE:
	      parse_type_decl ();
	      break;

	    case START:
	      parse_start_decl ();
	      break;

	    case UNION:
	      parse_union_decl ();
	      break;

	    case EXPECT:
	      parse_expect_decl ();
	      break;
	    case THONG:
	      parse_thong_decl ();
	      break;

	    case LEFT:
	      parse_assoc_decl (left_assoc);
	      break;

	    case RIGHT:
	      parse_assoc_decl (right_assoc);
	      break;

	    case NONASSOC:
	      parse_assoc_decl (non_assoc);
	      break;

	    case SEMANTIC_PARSER:
	      if (semantic_parser == 0)
		{
		  semantic_parser = 1;
		  open_extra_files ();
		}
	      break;

	    case PURE_PARSER:
	      pure_parser = 1;
	      break;

	    case NOOP:
	      break;

	    default:
	      complain (_("unrecognized: %s"), token_buffer);
	      skip_to_char ('%');
	    }
	}
      else if (c == EOF)
	fatal (_("no input grammar"));
      else
	{
	  complain (_("unknown character: %s"), printable_version (c));
	  skip_to_char ('%');
	}
    }
}

/*-------------------------------------------------------------------.
| Assuming that a `{' has just been seen, copy everything up to the  |
| matching `}' into the actions file.  STACK_OFFSET is the number of |
| values in the current rule so far, which says where to find `$0'   |
| with respect to the top of the stack.                              |
`-------------------------------------------------------------------*/

static void
copy_action (symbol_list * rule, int stack_offset)
{
  int c;
  int n;
  int count;
  char *type_name;

  /* offset is always 0 if parser has already popped the stack pointer */
  if (semantic_parser)
    stack_offset = 0;

  fprintf (faction, "\ncase %d:\n", nrules);
  if (!no_lines_flag)
    fprintf (faction, "#line %d \"%s\"\n", lineno, infile);
  putc ('{', faction);

  count = 1;
  c = getc (finput);

  while (count > 0)
    {
      while (c != '}')
	{
	  switch (c)
	    {
	    case '\n':
	      putc (c, faction);
	      lineno++;
	      break;

	    case '{':
	      putc (c, faction);
	      count++;
	      break;

	    case '\'':
	    case '"':
	      copy_string (finput, faction, c);
	      break;

	    case '/':
	      putc (c, faction);
	      c = getc (finput);
	      if (c != '*' && c != '/')
		continue;
	      copy_comment (finput, faction, c);
	      break;

	    case '$':
	      c = getc (finput);
	      type_name = NULL;

	      if (c == '<')
		{
		  char *cp = token_buffer;

		  while ((c = getc (finput)) != '>' && c > 0)
		    {
		      if (cp == token_buffer + maxtoken)
			cp = grow_token_buffer (cp);

		      *cp++ = c;
		    }
		  *cp = 0;
		  type_name = token_buffer;
		  value_components_used = 1;

		  c = getc (finput);
		}
	      if (c == '$')
		{
		  fprintf (faction, "yyval");
		  if (!type_name)
		    type_name = get_type_name (0, rule);
		  if (type_name)
		    fprintf (faction, ".%s", type_name);
		  if (!type_name && typed)
		    complain (_("$$ of `%s' has no declared type"),
			      rule->sym->tag);
		}
	      else if (isdigit (c) || c == '-')
		{
		  ungetc (c, finput);
		  n = read_signed_integer (finput);
		  c = getc (finput);

		  if (!type_name && n > 0)
		    type_name = get_type_name (n, rule);

		  fprintf (faction, "yyvsp[%d]", n - stack_offset);
		  if (type_name)
		    fprintf (faction, ".%s", type_name);
		  if (!type_name && typed)
		    complain (_("$%d of `%s' has no declared type"),
			      n, rule->sym->tag);
		  continue;
		}
	      else
		complain (_("$%s is invalid"), printable_version (c));

	      break;

	    case '@':
	      copy_at (finput, faction, stack_offset);
	      break;

	    case EOF:
	      fatal (_("unmatched %s"), "`{'");

	    default:
	      putc (c, faction);
	    }

	  c = getc (finput);
	}

      /* above loop exits when c is '}' */

      if (--count)
	{
	  putc (c, faction);
	  c = getc (finput);
	}
    }

  fprintf (faction, ";\n    break;}");
}

/*-------------------------------------------------------------------.
| After `%guard' is seen in the input file, copy the actual guard    |
| into the guards file.  If the guard is followed by an action, copy |
| that into the actions file.  STACK_OFFSET is the number of values  |
| in the current rule so far, which says where to find `$0' with     |
| respect to the top of the stack, for the simple parser in which    |
| the stack is not popped until after the guard is run.              |
`-------------------------------------------------------------------*/

static void
copy_guard (symbol_list * rule, int stack_offset)
{
  int c;
  int n;
  int count;
  char *type_name;
  int brace_flag = 0;

  /* offset is always 0 if parser has already popped the stack pointer */
  if (semantic_parser)
    stack_offset = 0;

  fprintf (fguard, "\ncase %d:\n", nrules);
  if (!no_lines_flag)
    fprintf (fguard, "#line %d \"%s\"\n", lineno, infile);
  putc ('{', fguard);

  count = 0;
  c = getc (finput);

  while (brace_flag ? (count > 0) : (c != ';'))
    {
      switch (c)
	{
	case '\n':
	  putc (c, fguard);
	  lineno++;
	  break;

	case '{':
	  putc (c, fguard);
	  brace_flag = 1;
	  count++;
	  break;

	case '}':
	  putc (c, fguard);
	  if (count > 0)
	    count--;
	  else
	    {
	      complain (_("unmatched %s"), "`}'");
	      c = getc (finput);	/* skip it */
	    }
	  break;

	case '\'':
	case '"':
	  copy_string (finput, fguard, c);
	  break;

	case '/':
	  putc (c, fguard);
	  c = getc (finput);
	  if (c != '*' && c != '/')
	    continue;
	  copy_comment (finput, fguard, c);
	  break;

	case '$':
	  c = getc (finput);
	  type_name = NULL;

	  if (c == '<')
	    {
	      char *cp = token_buffer;

	      while ((c = getc (finput)) != '>' && c > 0)
		{
		  if (cp == token_buffer + maxtoken)
		    cp = grow_token_buffer (cp);

		  *cp++ = c;
		}
	      *cp = 0;
	      type_name = token_buffer;

	      c = getc (finput);
	    }

	  if (c == '$')
	    {
	      fprintf (fguard, "yyval");
	      if (!type_name)
		type_name = rule->sym->type_name;
	      if (type_name)
		fprintf (fguard, ".%s", type_name);
	      if (!type_name && typed)
		complain (_("$$ of `%s' has no declared type"),
			  rule->sym->tag);
	    }
	  else if (isdigit (c) || c == '-')
	    {
	      ungetc (c, finput);
	      n = read_signed_integer (finput);
	      c = getc (finput);

	      if (!type_name && n > 0)
		type_name = get_type_name (n, rule);

	      fprintf (fguard, "yyvsp[%d]", n - stack_offset);
	      if (type_name)
		fprintf (fguard, ".%s", type_name);
	      if (!type_name && typed)
		complain (_("$%d of `%s' has no declared type"),
			  n, rule->sym->tag);
	      continue;
	    }
	  else
	    complain (_("$%s is invalid"), printable_version (c));
	  break;

	case '@':
	  copy_at (finput, fguard, stack_offset);
	  break;

	case EOF:
	  fatal ("%s", _("unterminated %guard clause"));

	default:
	  putc (c, fguard);
	}

      if (c != '}' || count != 0)
	c = getc (finput);
    }

  c = skip_white_space ();

  fprintf (fguard, ";\n    break;}");
  if (c == '{')
    copy_action (rule, stack_offset);
  else if (c == '=')
    {
      c = getc (finput);	/* why not skip_white_space -wjh */
      if (c == '{')
	copy_action (rule, stack_offset);
    }
  else
    ungetc (c, finput);
}


static void
record_rule_line (void)
{
  /* Record each rule's source line number in rline table.  */

  if (nrules >= rline_allocated)
    {
      rline_allocated = nrules * 2;
      rline = XREALLOC (rline, short, rline_allocated);
    }
  rline[nrules] = lineno;
}


/*-------------------------------------------------------------------.
| Generate a dummy symbol, a nonterminal, whose name cannot conflict |
| with the user's names.                                             |
`-------------------------------------------------------------------*/

static bucket *
gensym (void)
{
  bucket *sym;

  sprintf (token_buffer, "@%d", ++gensym_count);
  sym = getsym (token_buffer);
  sym->class = nterm_sym;
  sym->value = nvars++;
  return sym;
}

#if 0
/*------------------------------------------------------------------.
| read in a %type declaration and record its information for        |
| get_type_name to access.  This is unused.  It is only called from |
| the #if 0 part of readgram                                        |
`------------------------------------------------------------------*/

static int
get_type (void)
{
  int k;
  int t;
  char *name;

  t = lex ();

  if (t != TYPENAME)
    {
      complain (_("invalid %s declaration"), "%type");
      return t;
    }

  name = xstrdup (token_buffer);

  for (;;)
    {
      t = lex ();

      switch (t)
	{
	case SEMICOLON:
	  return lex ();

	case COMMA:
	  break;

	case IDENTIFIER:
	  if (symval->type_name == NULL)
	    symval->type_name = name;
	  else if (strcmp (name, symval->type_name) != 0)
	    complain (_("type redeclaration for %s"), symval->tag);

	  break;

	default:
	  return t;
	}
    }
}

#endif

/*------------------------------------------------------------------.
| Parse the input grammar into a one symbol_list structure.  Each   |
| rule is represented by a sequence of symbols: the left hand side  |
| followed by the contents of the right hand side, followed by a    |
| null pointer instead of a symbol to terminate the rule.  The next |
| symbol is the lhs of the following rule.                          |
|                                                                   |
| All guards and actions are copied out to the appropriate files,   |
| labelled by the rule number they apply to.                        |
`------------------------------------------------------------------*/

static void
readgram (void)
{
  int t;
  bucket *lhs = NULL;
  symbol_list *p;
  symbol_list *p1;
  bucket *bp;

  symbol_list *crule;		/* points to first symbol_list of current rule.  */
  /* its symbol is the lhs of the rule.   */
  symbol_list *crule1;		/* points to the symbol_list preceding crule.  */

  p1 = NULL;

  t = lex ();

  while (t != TWO_PERCENTS && t != ENDFILE)
    {
      if (t == IDENTIFIER || t == BAR)
	{
	  int action_flag = 0;
	  int rulelength = 0;	/* number of symbols in rhs of this rule so far  */
	  int xactions = 0;	/* JF for error checking */
	  bucket *first_rhs = 0;

	  if (t == IDENTIFIER)
	    {
	      lhs = symval;

	      if (!start_flag)
		{
		  startval = lhs;
		  start_flag = 1;
		}

	      t = lex ();
	      if (t != COLON)
		{
		  complain (_("ill-formed rule: initial symbol not followed by colon"));
		  unlex (t);
		}
	    }

	  if (nrules == 0 && t == BAR)
	    {
	      complain (_("grammar starts with vertical bar"));
	      lhs = symval;	/* BOGUS: use a random symval */
	    }
	  /* start a new rule and record its lhs.  */

	  nrules++;
	  nitems++;

	  record_rule_line ();

	  p = XCALLOC (symbol_list, 1);
	  p->sym = lhs;

	  crule1 = p1;
	  if (p1)
	    p1->next = p;
	  else
	    grammar = p;

	  p1 = p;
	  crule = p;

	  /* mark the rule's lhs as a nonterminal if not already so.  */

	  if (lhs->class == unknown_sym)
	    {
	      lhs->class = nterm_sym;
	      lhs->value = nvars;
	      nvars++;
	    }
	  else if (lhs->class == token_sym)
	    complain (_("rule given for %s, which is a token"), lhs->tag);

	  /* read the rhs of the rule.  */

	  for (;;)
	    {
	      t = lex ();
	      if (t == PREC)
		{
		  t = lex ();
		  crule->ruleprec = symval;
		  t = lex ();
		}

	      if (!(t == IDENTIFIER || t == LEFT_CURLY))
		break;

	      /* If next token is an identifier, see if a colon follows it.
	         If one does, exit this rule now.  */
	      if (t == IDENTIFIER)
		{
		  bucket *ssave;
		  int t1;

		  ssave = symval;
		  t1 = lex ();
		  unlex (t1);
		  symval = ssave;
		  if (t1 == COLON)
		    break;

		  if (!first_rhs)	/* JF */
		    first_rhs = symval;
		  /* Not followed by colon =>
		     process as part of this rule's rhs.  */
		}

	      /* If we just passed an action, that action was in the middle
	         of a rule, so make a dummy rule to reduce it to a
	         non-terminal.  */
	      if (action_flag)
		{
		  bucket *sdummy;

		  /* Since the action was written out with this rule's */
		  /* number, we must give the new rule this number */
		  /* by inserting the new rule before it.  */

		  /* Make a dummy nonterminal, a gensym.  */
		  sdummy = gensym ();

		  /* Make a new rule, whose body is empty,
		     before the current one, so that the action
		     just read can belong to it.  */
		  nrules++;
		  nitems++;
		  record_rule_line ();
		  p = XCALLOC (symbol_list, 1);
		  if (crule1)
		    crule1->next = p;
		  else
		    grammar = p;
		  p->sym = sdummy;
		  crule1 = XCALLOC (symbol_list, 1);
		  p->next = crule1;
		  crule1->next = crule;

		  /* insert the dummy generated by that rule into this rule.  */
		  nitems++;
		  p = XCALLOC (symbol_list, 1);
		  p->sym = sdummy;
		  p1->next = p;
		  p1 = p;

		  action_flag = 0;
		}

	      if (t == IDENTIFIER)
		{
		  nitems++;
		  p = XCALLOC (symbol_list, 1);
		  p->sym = symval;
		  p1->next = p;
		  p1 = p;
		}
	      else		/* handle an action.  */
		{
		  copy_action (crule, rulelength);
		  action_flag = 1;
		  xactions++;	/* JF */
		}
	      rulelength++;
	    }			/* end of  read rhs of rule */

	  /* Put an empty link in the list to mark the end of this rule  */
	  p = XCALLOC (symbol_list, 1);
	  p1->next = p;
	  p1 = p;

	  if (t == PREC)
	    {
	      complain (_("two @prec's in a row"));
	      t = lex ();
	      crule->ruleprec = symval;
	      t = lex ();
	    }
	  if (t == GUARD)
	    {
	      if (!semantic_parser)
		complain ("%s",
			  _
			  ("%guard present but %semantic_parser not specified"));

	      copy_guard (crule, rulelength);
	      t = lex ();
	    }
	  else if (t == LEFT_CURLY)
	    {
	      /* This case never occurs -wjh */
	      if (action_flag)
		complain (_("two actions at end of one rule"));
	      copy_action (crule, rulelength);
	      action_flag = 1;
	      xactions++;	/* -wjh */
	      t = lex ();
	    }
	  /* If $$ is being set in default way, report if any type
	     mismatch.  */
	  else if (!xactions
		   && first_rhs && lhs->type_name != first_rhs->type_name)
	    {
	      if (lhs->type_name == 0
		  || first_rhs->type_name == 0
		  || strcmp (lhs->type_name, first_rhs->type_name))
		complain (_("type clash (`%s' `%s') on default action"),
			  lhs->type_name ? lhs->type_name : "",
			  first_rhs->type_name ? first_rhs->type_name : "");
	    }
	  /* Warn if there is no default for $$ but we need one.  */
	  else if (!xactions && !first_rhs && lhs->type_name != 0)
	    complain (_("empty rule for typed nonterminal, and no action"));
	  if (t == SEMICOLON)
	    t = lex ();
	}
#if 0
      /* these things can appear as alternatives to rules.  */
/* NO, they cannot.
	a) none of the documentation allows them
	b) most of them scan forward until finding a next %
		thus they may swallow lots of intervening rules
*/
      else if (t == TOKEN)
	{
	  parse_token_decl (token_sym, nterm_sym);
	  t = lex ();
	}
      else if (t == NTERM)
	{
	  parse_token_decl (nterm_sym, token_sym);
	  t = lex ();
	}
      else if (t == TYPE)
	{
	  t = get_type ();
	}
      else if (t == UNION)
	{
	  parse_union_decl ();
	  t = lex ();
	}
      else if (t == EXPECT)
	{
	  parse_expect_decl ();
	  t = lex ();
	}
      else if (t == START)
	{
	  parse_start_decl ();
	  t = lex ();
	}
#endif

      else
	{
	  complain (_("invalid input: %s"), token_buffer);
	  t = lex ();
	}
    }

  /* grammar has been read.  Do some checking */

  if (nsyms > MAXSHORT)
    fatal (_("too many symbols (tokens plus nonterminals); maximum %d"),
	   MAXSHORT);
  if (nrules == 0)
    fatal (_("no rules in the input grammar"));

  if (typed == 0		/* JF put out same default YYSTYPE as YACC does */
      && !value_components_used)
    {
      /* We used to use `unsigned long' as YYSTYPE on MSDOS,
         but it seems better to be consistent.
         Most programs should declare their own type anyway.  */
      fprintf (fattrs, "#ifndef YYSTYPE\n#define YYSTYPE int\n#endif\n");
      if (fdefines)
	fprintf (fdefines, "#ifndef YYSTYPE\n#define YYSTYPE int\n#endif\n");
    }

  /* Report any undefined symbols and consider them nonterminals.  */

  for (bp = firstsymbol; bp; bp = bp->next)
    if (bp->class == unknown_sym)
      {
	complain (_
		  ("symbol %s is used, but is not defined as a token and has no rules"),
bp->tag);
	bp->class = nterm_sym;
	bp->value = nvars++;
      }

  ntokens = nsyms - nvars;
}

/*--------------------------------------------------------------.
| For named tokens, but not literal ones, define the name.  The |
| value is the user token number.                               |
`--------------------------------------------------------------*/

static void
output_token_defines (FILE *file)
{
  bucket *bp;
  char *cp, *symbol;
  char c;

  for (bp = firstsymbol; bp; bp = bp->next)
    {
      symbol = bp->tag;		/* get symbol */

      if (bp->value >= ntokens)
	continue;
      if (bp->user_token_number == SALIAS)
	continue;
      if ('\'' == *symbol)
	continue;		/* skip literal character */
      if (bp == errtoken)
	continue;		/* skip error token */
      if ('\"' == *symbol)
	{
	  /* use literal string only if given a symbol with an alias */
	  if (bp->alias)
	    symbol = bp->alias->tag;
	  else
	    continue;
	}

      /* Don't #define nonliteral tokens whose names contain periods.  */
      cp = symbol;
      while ((c = *cp++) && c != '.');
      if (c != '\0')
	continue;

      fprintf (file, "#define\t%s\t%d\n", symbol,
	       ((translations && !raw_flag)
		? bp->user_token_number : bp->value));
      if (semantic_parser)
	fprintf (file, "#define\tT%s\t%d\n", symbol, bp->value);
    }

  putc ('\n', file);
}


/*------------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into   |
| FDEFINES.  Set up vectors TAGS and SPREC of names and precedences |
| of symbols.                                                       |
`------------------------------------------------------------------*/

static void
packsymbols (void)
{
  bucket *bp;
  int tokno = 1;
  int i;
  int last_user_token_number;
  static char DOLLAR[] = "$";

  /* int lossage = 0; JF set but not used */

  tags = XCALLOC (char *, nsyms + 1);
  tags[0] = DOLLAR;
  user_toknums = XCALLOC (short, nsyms + 1);
  user_toknums[0] = 0;

  sprec = XCALLOC (short, nsyms);
  sassoc = XCALLOC (short, nsyms);

  max_user_token_number = 256;
  last_user_token_number = 256;

  for (bp = firstsymbol; bp; bp = bp->next)
    {
      if (bp->class == nterm_sym)
	{
	  bp->value += ntokens;
	}
      else if (bp->alias)
	{
	  /* this symbol and its alias are a single token defn.
	     allocate a tokno, and assign to both check agreement of
	     ->prec and ->assoc fields and make both the same */
	  if (bp->value == 0)
	    bp->value = bp->alias->value = tokno++;

	  if (bp->prec != bp->alias->prec)
	    {
	      if (bp->prec != 0 && bp->alias->prec != 0
		  && bp->user_token_number == SALIAS)
		complain (_("conflicting precedences for %s and %s"),
			  bp->tag, bp->alias->tag);
	      if (bp->prec != 0)
		bp->alias->prec = bp->prec;
	      else
		bp->prec = bp->alias->prec;
	    }

	  if (bp->assoc != bp->alias->assoc)
	    {
	      if (bp->assoc != 0 && bp->alias->assoc != 0
		  && bp->user_token_number == SALIAS)
		complain (_("conflicting assoc values for %s and %s"),
			  bp->tag, bp->alias->tag);
	      if (bp->assoc != 0)
		bp->alias->assoc = bp->assoc;
	      else
		bp->assoc = bp->alias->assoc;
	    }

	  if (bp->user_token_number == SALIAS)
	    continue;		/* do not do processing below for SALIASs */

	}
      else			/* bp->class == token_sym */
	{
	  bp->value = tokno++;
	}

      if (bp->class == token_sym)
	{
	  if (translations && !(bp->user_token_number))
	    bp->user_token_number = ++last_user_token_number;
	  if (bp->user_token_number > max_user_token_number)
	    max_user_token_number = bp->user_token_number;
	}

      tags[bp->value] = bp->tag;
      user_toknums[bp->value] = bp->user_token_number;
      sprec[bp->value] = bp->prec;
      sassoc[bp->value] = bp->assoc;

    }

  if (translations)
    {
      int j;

      token_translations = XCALLOC (short, max_user_token_number + 1);

      /* initialize all entries for literal tokens to 2, the internal
         token number for $undefined., which represents all invalid
         inputs.  */
      for (j = 0; j <= max_user_token_number; j++)
	token_translations[j] = 2;

      for (bp = firstsymbol; bp; bp = bp->next)
	{
	  if (bp->value >= ntokens)
	    continue;		/* non-terminal */
	  if (bp->user_token_number == SALIAS)
	    continue;
	  if (token_translations[bp->user_token_number] != 2)
	    complain (_("tokens %s and %s both assigned number %d"),
		      tags[token_translations[bp->user_token_number]],
		      bp->tag, bp->user_token_number);
	  token_translations[bp->user_token_number] = bp->value;
	}
    }

  error_token_number = errtoken->value;

  if (!no_parser_flag)
    output_token_defines (ftable);

  if (startval->class == unknown_sym)
    fatal (_("the start symbol %s is undefined"), startval->tag);
  else if (startval->class == token_sym)
    fatal (_("the start symbol %s is a token"), startval->tag);

  start_symbol = startval->value;

  if (defines_flag)
    {
      output_token_defines (fdefines);

      if (!pure_parser)
	{
	  if (spec_name_prefix)
	    fprintf (fdefines, "\nextern YYSTYPE %slval;\n",
		     spec_name_prefix);
	  else
	    fprintf (fdefines, "\nextern YYSTYPE yylval;\n");
	}

      if (semantic_parser)
	for (i = ntokens; i < nsyms; i++)
	  {
	    /* don't make these for dummy nonterminals made by gensym.  */
	    if (*tags[i] != '@')
	      fprintf (fdefines, "#define\tNT%s\t%d\n", tags[i], i);
	  }
#if 0
      /* `fdefines' is now a temporary file, so we need to copy its
         contents in `done', so we can't close it here.  */
      fclose (fdefines);
      fdefines = NULL;
#endif
    }
}


/*---------------------------------------------------------------.
| Convert the rules into the representation using RRHS, RLHS and |
| RITEMS.                                                        |
`---------------------------------------------------------------*/

static void
packgram (void)
{
  int itemno;
  int ruleno;
  symbol_list *p;

  bucket *ruleprec;

  ritem = XCALLOC (short, nitems + 1);
  rlhs = XCALLOC (short, nrules) - 1;
  rrhs = XCALLOC (short, nrules) - 1;
  rprec = XCALLOC (short, nrules) - 1;
  rprecsym = XCALLOC (short, nrules) - 1;
  rassoc = XCALLOC (short, nrules) - 1;

  itemno = 0;
  ruleno = 1;

  p = grammar;
  while (p)
    {
      rlhs[ruleno] = p->sym->value;
      rrhs[ruleno] = itemno;
      ruleprec = p->ruleprec;

      p = p->next;
      while (p && p->sym)
	{
	  ritem[itemno++] = p->sym->value;
	  /* A rule gets by default the precedence and associativity
	     of the last token in it.  */
	  if (p->sym->class == token_sym)
	    {
	      rprec[ruleno] = p->sym->prec;
	      rassoc[ruleno] = p->sym->assoc;
	    }
	  if (p)
	    p = p->next;
	}

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      if (ruleprec)
	{
	  rprec[ruleno] = ruleprec->prec;
	  rassoc[ruleno] = ruleprec->assoc;
	  rprecsym[ruleno] = ruleprec->value;
	}

      ritem[itemno++] = -ruleno;
      ruleno++;

      if (p)
	p = p->next;
    }

  ritem[itemno] = 0;
}

/*-------------------------------------------------------------------.
| Read in the grammar specification and record it in the format      |
| described in gram.h.  All guards are copied into the FGUARD file   |
| and all actions into FACTION, in each case forming the body of a C |
| function (YYGUARD or YYACTION) which contains a switch statement   |
| to decide which guard or action to execute.                        |
`-------------------------------------------------------------------*/

void
reader (void)
{
  start_flag = 0;
  startval = NULL;		/* start symbol not specified yet. */

#if 0
  /* initially assume token number translation not needed.  */
  translations = 0;
#endif
  /* Nowadays translations is always set to 1, since we give `error' a
     user-token-number to satisfy the Posix demand for YYERRCODE==256.
   */
  translations = 1;

  nsyms = 1;
  nvars = 0;
  nrules = 0;
  nitems = 0;
  rline_allocated = 10;
  rline = XCALLOC (short, rline_allocated);

  typed = 0;
  lastprec = 0;

  gensym_count = 0;

  semantic_parser = 0;
  pure_parser = 0;

  grammar = NULL;

  init_lex ();
  lineno = 1;

  /* Initialize the symbol table.  */
  tabinit ();
  /* Construct the error token */
  errtoken = getsym ("error");
  errtoken->class = token_sym;
  errtoken->user_token_number = 256;	/* Value specified by POSIX.  */
  /* Construct a token that represents all undefined literal tokens.
     It is always token number 2.  */
  undeftoken = getsym ("$undefined.");
  undeftoken->class = token_sym;
  undeftoken->user_token_number = 2;

  /* Read the declaration section.  Copy %{ ... %} groups to FTABLE
     and FDEFINES file.  Also notice any %token, %left, etc. found
     there.  */
  putc ('\n', ftable);
  fprintf (ftable, "\
/* %s, made from %s\n\
   by GNU bison %s.  */\n\
\n", no_parser_flag ? "Bison-generated parse tables" : "A Bison parser", infile, VERSION);

  fputs ("#define YYBISON 1  /* Identify Bison output.  */\n\n", ftable);
  read_declarations ();
  /* Start writing the guard and action files, if they are needed.  */
  output_headers ();
  /* Read in the grammar, build grammar in list form.  Write out
     guards and actions.  */
  readgram ();
  /* Now we know whether we need the line-number stack.  If we do,
     write its type into the .tab.h file.  */
  if (fdefines)
    reader_output_yylsp (fdefines);
  /* Write closing delimiters for actions and guards.  */
  output_trailers ();
  if (locations_flag)
    fputs ("#define YYLSP_NEEDED\n\n", ftable);
  /* Assign the symbols their symbol numbers.  Write #defines for the
     token symbols into FDEFINES if requested.  */
  packsymbols ();
  /* Convert the grammar into the format described in gram.h.  */
  packgram ();
  /* Free the symbol table data structure since symbols are now all
     referred to by symbol number.  */
  free_symtab ();
}


void
reader_output_yylsp (FILE *f)
{
  if (locations_flag)
    fputs ("\
\n\
#ifndef YYLTYPE\n\
typedef struct yyltype\n\
{\n\
  int timestamp;\n\
  int first_line;\n\
  int first_column;\
\n\
  int last_line;\n\
  int last_column;\n\
  char *text;\n\
} yyltype;\n\
\n\
# define YYLTYPE yyltype\n\
#endif\n\
\n",
	   f);
}
