/* Input parser for bison
   Copyright 1984, 1986, 1989, 1992, 1998, 2000, 2001
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
#include "quotearg.h"
#include "quote.h"
#include "getargs.h"
#include "files.h"
#include "symtab.h"
#include "options.h"
#include "lex.h"
#include "gram.h"
#include "complain.h"
#include "output.h"
#include "reader.h"
#include "conflicts.h"
#include "muscle_tab.h"

typedef struct symbol_list
{
  struct symbol_list *next;
  bucket *sym;
  int line;

  /* The action is attached to the LHS of a rule. */
  const char *action;
  int action_line;

  /* The guard is attached to the LHS of a rule. */
  const char *guard;
  int guard_line;
  bucket *ruleprec;
} symbol_list;

int lineno;
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

static bucket *errtoken = NULL;
static bucket *undeftoken = NULL;
static bucket *eoftoken = NULL;
static bucket *axiom = NULL;

static symbol_list *
symbol_list_new (bucket *sym)
{
  symbol_list *res = XMALLOC (symbol_list, 1);
  res->next = NULL;
  res->sym = sym;
  res->line = lineno;
  res->action = NULL;
  res->action_line = 0;
  res->guard = NULL;
  res->guard_line = 0;
  res->ruleprec = NULL;
  return res;
}



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

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in rule RULE.                                        |
`--------------------------------------------------------------*/

static char *
get_type_name (int n, symbol_list *rule)
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

/*------------------------------------------------------------.
| Dump the string from FIN to OOUT if non null.  MATCH is the |
| delimiter of the string (either ' or ").                    |
`------------------------------------------------------------*/

static inline void
copy_string2 (FILE *fin, struct obstack *oout, int match, int store)
{
  int c;

  if (store)
    obstack_1grow (oout, match);

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

      obstack_1grow (oout, c);

      if (c == '\\')
	{
	  c = getc (fin);
	  if (c == EOF)
	    fatal (_("unterminated string at end of file"));
	  obstack_1grow (oout, c);

	  if (c == '\n')
	    lineno++;
	}

      c = getc (fin);
    }

  if (store)
    obstack_1grow (oout, c);
}

/* FIXME. */

static inline void
copy_string (FILE *fin, struct obstack *oout, int match)
{
  copy_string2 (fin, oout, match, 1);
}

/* FIXME. */

static inline void
copy_identifier (FILE *fin, struct obstack *oout)
{
  int c;

  while (isalnum (c = getc (fin)) || c == '_')
    obstack_1grow (oout, c);

  ungetc (c, fin);
}


/*------------------------------------------------------------------.
| Dump the wannabee comment from IN to OOUT.  In fact we just saw a |
| `/', which might or might not be a comment.  In any case, copy    |
| what we saw.                                                      |
`------------------------------------------------------------------*/

static inline void
copy_comment (FILE *fin, struct obstack *oout)
{
  int cplus_comment;
  int ended;
  int c;

  /* We read a `/', output it. */
  obstack_1grow (oout, '/');

  switch ((c = getc (fin)))
    {
    case '/':
      cplus_comment = 1;
      break;
    case '*':
      cplus_comment = 0;
      break;
    default:
      ungetc (c, fin);
      return;
    }

  obstack_1grow (oout, c);
  c = getc (fin);

  ended = 0;
  while (!ended)
    {
      if (!cplus_comment && c == '*')
	{
	  while (c == '*')
	    {
	      obstack_1grow (oout, c);
	      c = getc (fin);
	    }

	  if (c == '/')
	    {
	      obstack_1grow (oout, c);
	      ended = 1;
	    }
	}
      else if (c == '\n')
	{
	  lineno++;
	  obstack_1grow (oout, c);
	  if (cplus_comment)
	    ended = 1;
	  else
	    c = getc (fin);
	}
      else if (c == EOF)
	fatal (_("unterminated comment"));
      else
	{
	  obstack_1grow (oout, c);
	  c = getc (fin);
	}
    }
}


/*-----------------------------------------------------------------.
| FIN is pointing to a location (i.e., a `@').  Output to OOUT a   |
| reference to this location. STACK_OFFSET is the number of values |
| in the current rule so far, which says where to find `$0' with   |
| respect to the top of the stack.                                 |
`-----------------------------------------------------------------*/

static inline void
copy_at (FILE *fin, struct obstack *oout, int stack_offset)
{
  int c;

  c = getc (fin);
  if (c == '$')
    {
      obstack_sgrow (oout, "yyloc");
      locations_flag = 1;
    }
  else if (isdigit (c) || c == '-')
    {
      int n;

      ungetc (c, fin);
      n = read_signed_integer (fin);
      if (n > stack_offset)
	complain (_("invalid value: %s%d"), "@", n);
      else
	{
	  /* Offset is always 0 if parser has already popped the stack
	     pointer.  */
	  obstack_fgrow1 (oout, "yylsp[%d]",
			  n - (semantic_parser ? 0 : stack_offset));
	  locations_flag = 1;
	}
    }
  else
    {
      char buf[] = "@c";
      buf[1] = c;
      complain (_("%s is invalid"), quote (buf));
    }
}


/*-------------------------------------------------------------------.
| FIN is pointing to a wannabee semantic value (i.e., a `$').        |
|                                                                    |
| Possible inputs: $[<TYPENAME>]($|integer)                          |
|                                                                    |
| Output to OOUT a reference to this semantic value. STACK_OFFSET is |
| the number of values in the current rule so far, which says where  |
| to find `$0' with respect to the top of the stack.                 |
`-------------------------------------------------------------------*/

static inline void
copy_dollar (FILE *fin, struct obstack *oout,
	     symbol_list *rule, int stack_offset)
{
  int c = getc (fin);
  const char *type_name = NULL;

  /* Get the type name if explicit. */
  if (c == '<')
    {
      read_type_name (fin);
      type_name = token_buffer;
      value_components_used = 1;
      c = getc (fin);
    }

  if (c == '$')
    {
      obstack_sgrow (oout, "yyval");

      if (!type_name)
	type_name = get_type_name (0, rule);
      if (type_name)
	obstack_fgrow1 (oout, ".%s", type_name);
      if (!type_name && typed)
	complain (_("$$ of `%s' has no declared type"),
		  rule->sym->tag);
    }
  else if (isdigit (c) || c == '-')
    {
      int n;
      ungetc (c, fin);
      n = read_signed_integer (fin);

      if (n > stack_offset)
	complain (_("invalid value: %s%d"), "$", n);
      else
	{
	  if (!type_name && n > 0)
	    type_name = get_type_name (n, rule);

	  /* Offset is always 0 if parser has already popped the stack
	     pointer.  */
	  obstack_fgrow1 (oout, "yyvsp[%d]",
			  n - (semantic_parser ? 0 : stack_offset));

	  if (type_name)
	    obstack_fgrow1 (oout, ".%s", type_name);
	  if (!type_name && typed)
	    complain (_("$%d of `%s' has no declared type"),
		      n, rule->sym->tag);
	}
    }
  else
    {
      char buf[] = "$c";
      buf[1] = c;
      complain (_("%s is invalid"), quote (buf));
    }
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
    {
      obstack_fgrow2 (&attrs_obstack, muscle_find ("linef"),
		      lineno, quotearg_style (c_quoting_style,
					      muscle_find ("filename")));
    }

  after_percent = 0;

  c = getc (finput);

  for (;;)
    {
      switch (c)
	{
	case '\n':
	  obstack_1grow (&attrs_obstack, c);
	  lineno++;
	  break;

	case '%':
	  after_percent = -1;
	  break;

	case '\'':
	case '"':
	  copy_string (finput, &attrs_obstack, c);
	  break;

	case '/':
	  copy_comment (finput, &attrs_obstack);
	  break;

	case EOF:
	  fatal ("%s", _("unterminated `%{' definition"));

	default:
	  obstack_1grow (&attrs_obstack, c);
	}

      c = getc (finput);

      if (after_percent)
	{
	  if (c == '}')
	    return;
	  obstack_1grow (&attrs_obstack, '%');
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
  token_t token = tok_undef;
  char *typename = NULL;

  /* The symbol being defined.  */
  struct bucket *symbol = NULL;

  /* After `%token' and `%nterm', any number of symbols maybe be
     defined.  */
  for (;;)
    {
      int tmp_char = ungetc (skip_white_space (), finput);

      /* `%' (for instance from `%token', or from `%%' etc.) is the
	 only valid means to end this declaration.  */
      if (tmp_char == '%')
	return;
      if (tmp_char == EOF)
	fatal (_("Premature EOF after %s"), token_buffer);

      token = lex ();
      if (token == tok_comma)
	{
	  symbol = NULL;
	  continue;
	}
      if (token == tok_typename)
	{
	  typename = xstrdup (token_buffer);
	  value_components_used = 1;
	  symbol = NULL;
	}
      else if (token == tok_identifier && *symval->tag == '\"' && symbol)
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
 	  symbol = NULL;
	}
      else if (token == tok_identifier)
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
      else if (symbol && token == tok_number)
	{
	  symbol->user_token_number = numval;
	  /* User defined EOF token? */
	  if (numval == 0)
	    eoftoken = symbol;
	}
      else
	{
	  complain (_("`%s' is invalid in %s"),
		    token_buffer,
		    (what_is == token_sym) ? "%token" : "%nterm");
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
  if (lex () != tok_identifier)
    complain (_("invalid %s declaration"), "%start");
  else
    {
      start_flag = 1;
      startval = symval;
    }
}

/*-----------------------------------------------------------.
| read in a %type declaration and record its information for |
| get_type_name to access                                    |
`-----------------------------------------------------------*/

static void
parse_type_decl (void)
{
  char *name;

  if (lex () != tok_typename)
    {
      complain ("%s", _("%type declaration has no <typename>"));
      skip_to_char ('%');
      return;
    }

  name = xstrdup (token_buffer);

  for (;;)
    {
      token_t t;
      int tmp_char = ungetc (skip_white_space (), finput);

      if (tmp_char == '%')
	return;
      if (tmp_char == EOF)
	fatal (_("Premature EOF after %s"), token_buffer);

      t = lex ();

      switch (t)
	{

	case tok_comma:
	case tok_semicolon:
	  break;

	case tok_identifier:
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
      token_t t;
      int tmp_char = ungetc (skip_white_space (), finput);

      if (tmp_char == '%')
	return;
      if (tmp_char == EOF)
	fatal (_("Premature EOF after %s"), token_buffer);

      t = lex ();

      switch (t)
	{
	case tok_typename:
	  name = xstrdup (token_buffer);
	  break;

	case tok_comma:
	  break;

	case tok_identifier:
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

	case tok_number:
	  if (prev == tok_identifier)
	    {
	      symval->user_token_number = numval;
	    }
	  else
	    {
	      complain (_
			("invalid text (%s) - number should be after identifier"),
token_buffer);
	      skip_to_char ('%');
	    }
	  break;

	case tok_semicolon:
	  return;

	default:
	  complain (_("unexpected item: %s"), token_buffer);
	  skip_to_char ('%');
	}

      prev = t;
    }
}



/*--------------------------------------------------------------.
| Copy the union declaration into the stype muscle		|
| (and fdefines),  where it is made into the definition of	|
| YYSTYPE, the type of elements of the parser value stack.	|
`--------------------------------------------------------------*/

static void
parse_union_decl (void)
{
  int c;
  int count = 0;
  bool done = FALSE;
  struct obstack union_obstack;
  if (typed)
    complain (_("multiple %s declarations"), "%union");

  typed = 1;

  obstack_init (&union_obstack);
  obstack_sgrow (&union_obstack, "union");

  while (!done)
    {
      c = xgetc (finput);

      /* If C contains '/', it is output by copy_comment ().  */
      if (c != '/')
	obstack_1grow (&union_obstack, c);

      switch (c)
	{
	case '\n':
	  lineno++;
	  break;

	case '/':
	  copy_comment (finput, &union_obstack);
	  break;

	case '{':
	  count++;
	  break;

	case '}':
	  /* FIXME: Errr.  How could this happen???. --akim */
	  if (count == 0)
	    complain (_("unmatched %s"), "`}'");
	  count--;
	  if (!count)
	    done = TRUE;
	  break;
	}
    }

  /* JF don't choke on trailing semi */
  c = skip_white_space ();
  if (c != ';')
    ungetc (c, finput);
  obstack_1grow (&union_obstack, 0);
  muscle_insert ("stype", obstack_finish (&union_obstack));
}


/*-------------------------------------------------------.
| Parse the declaration %expect N which says to expect N |
| shift-reduce conflicts.                                |
`-------------------------------------------------------*/

static void
parse_expect_decl (void)
{
  int c = skip_white_space ();
  ungetc (c, finput);

  if (!isdigit (c))
    complain (_("argument of %%expect is not an integer"));
  else
    expected_conflicts = read_signed_integer (finput);
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
  token_t token;
  struct bucket *symbol;
  char *typename = 0;
  int usrtoknum = SUNDEF;

  token = lex ();		/* fetch typename or first token */
  if (token == tok_typename)
    {
      typename = xstrdup (token_buffer);
      value_components_used = 1;
      token = lex ();		/* fetch first token */
    }

  /* process first token */

  if (token != tok_identifier)
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

  if (token == tok_number)
    {
      usrtoknum = numval;
      token = lex ();		/* okay, did number, now get literal */
    }

  /* process literal string token */

  if (token != tok_identifier || *symval->tag != '\"')
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

  /* symbol and symval combined are only one symbol.  */
  nsyms--;
}


static void
parse_muscle_decl (void)
{
  int ch = ungetc (skip_white_space (), finput);
  char *muscle_key;
  char *muscle_value;

  /* Read key. */
  if (!isalpha (ch) && ch != '_')
    {
      complain (_("invalid %s declaration"), "%define");
      skip_to_char ('%');
      return;
    }
  copy_identifier (finput, &muscle_obstack);
  obstack_1grow (&muscle_obstack, 0);
  muscle_key = obstack_finish (&muscle_obstack);

  /* Read value. */
  ch = skip_white_space ();
  if (ch != '"')
    {
      ungetc (ch, finput);
      if (ch != EOF)
	{
	  complain (_("invalid %s declaration"), "%define");
	  skip_to_char ('%');
	  return;
	}
      else
	fatal (_("Premature EOF after %s"), "\"");
    }
  copy_string2 (finput, &muscle_obstack, '"', 0);
  obstack_1grow (&muscle_obstack, 0);
  muscle_value = obstack_finish (&muscle_obstack);

  /* Store the (key, value) pair in the environment. */
  muscle_insert (muscle_key, muscle_value);
}



/*---------------------------------.
| Parse a double quoted parameter. |
`---------------------------------*/

static const char *
parse_dquoted_param (const char *from)
{
  struct obstack param_obstack;
  const char *param = NULL;
  int c;

  obstack_init (&param_obstack);
  c = skip_white_space ();

  if (c != '"')
    {
      complain (_("invalid %s declaration"), from);
      ungetc (c, finput);
      skip_to_char ('%');
      return NULL;
    }

  while ((c = literalchar ()) != '"')
    obstack_1grow (&param_obstack, c);

  obstack_1grow (&param_obstack, '\0');
  param = obstack_finish (&param_obstack);

  if (c != '"' || strlen (param) == 0)
    {
      complain (_("invalid %s declaration"), from);
      if (c != '"')
	ungetc (c, finput);
      skip_to_char ('%');
      return NULL;
    }

  return param;
}

/*----------------------------------.
| Parse what comes after %skeleton. |
`----------------------------------*/

static void
parse_skel_decl (void)
{
  skeleton = parse_dquoted_param ("%skeleton");
}

/*----------------------------------------------------------------.
| Read from finput until `%%' is seen.  Discard the `%%'.  Handle |
| any `%' declarations, and copy the contents of any `%{ ... %}'  |
| groups to ATTRS_OBSTACK.                                        |
`----------------------------------------------------------------*/

static void
read_declarations (void)
{
  for (;;)
    {
      int c = skip_white_space ();

      if (c == '%')
	{
	  token_t tok = parse_percent_token ();

	  switch (tok)
	    {
	    case tok_two_percents:
	      return;

	    case tok_percent_left_curly:
	      copy_definition ();
	      break;

	    case tok_token:
	      parse_token_decl (token_sym, nterm_sym);
	      break;

	    case tok_nterm:
	      parse_token_decl (nterm_sym, token_sym);
	      break;

	    case tok_type:
	      parse_type_decl ();
	      break;

	    case tok_start:
	      parse_start_decl ();
	      break;

	    case tok_union:
	      parse_union_decl ();
	      break;

	    case tok_expect:
	      parse_expect_decl ();
	      break;

	    case tok_thong:
	      parse_thong_decl ();
	      break;

	    case tok_left:
	      parse_assoc_decl (left_assoc);
	      break;

	    case tok_right:
	      parse_assoc_decl (right_assoc);
	      break;

	    case tok_nonassoc:
	      parse_assoc_decl (non_assoc);
	      break;

	    case tok_define:
	      parse_muscle_decl ();
	      break;

	    case tok_skel:
	      parse_skel_decl ();
	      break;

	    case tok_noop:
	      break;

	    case tok_stropt:
	    case tok_intopt:
	    case tok_obsolete:
	      abort ();
	      break;

	    case tok_illegal:
	    default:
	      complain (_("unrecognized: %s"), token_buffer);
	      skip_to_char ('%');
	    }
	}
      else if (c == EOF)
	fatal (_("no input grammar"));
      else
	{
	  char buf[] = "c";
	  buf[0] = c;
	  complain (_("unknown character: %s"), quote (buf));
	  skip_to_char ('%');
	}
    }
}

/*-------------------------------------------------------------------.
| Assuming that a `{' has just been seen, copy everything up to the  |
| matching `}' into the actions file.  STACK_OFFSET is the number of |
| values in the current rule so far, which says where to find `$0'   |
| with respect to the top of the stack.                              |
|                                                                    |
| This routine is used both for actions and guards.  Only            |
| ACTION_OBSTACK is used, but this is fine, since we use only        |
| pointers to relevant portions inside this obstack.                 |
`-------------------------------------------------------------------*/

static void
parse_braces (symbol_list *rule, int stack_offset)
{
  int c;
  int count;

  count = 1;
  while (count > 0)
    {
      while ((c = getc (finput)) != '}')
	switch (c)
	  {
	  case '\n':
	    obstack_1grow (&action_obstack, c);
	    lineno++;
	    break;

	  case '{':
	    obstack_1grow (&action_obstack, c);
	    count++;
	    break;

	  case '\'':
	  case '"':
	    copy_string (finput, &action_obstack, c);
	    break;

	  case '/':
	    copy_comment (finput, &action_obstack);
	    break;

	  case '$':
	    copy_dollar (finput, &action_obstack,
			 rule, stack_offset);
	    break;

	  case '@':
	    copy_at (finput, &action_obstack,
		     stack_offset);
	    break;

	  case EOF:
	    fatal (_("unmatched %s"), "`{'");

	  default:
	    obstack_1grow (&action_obstack, c);
	  }

      /* Above loop exits when C is '}'.  */
      if (--count)
	obstack_1grow (&action_obstack, c);
    }

  obstack_1grow (&action_obstack, '\0');
}


static void
parse_action (symbol_list *rule, int stack_offset)
{
  rule->action_line = lineno;
  parse_braces (rule, stack_offset);
  rule->action = obstack_finish (&action_obstack);
}


static void
parse_guard (symbol_list *rule, int stack_offset)
{
  token_t t = lex ();
  if (t != tok_left_curly)
    complain (_("invalid %s declaration"), "%guard");
  rule->guard_line = lineno;
  parse_braces (rule, stack_offset);
  rule->guard = obstack_finish (&action_obstack);
}



/*-------------------------------------------------------------------.
| Generate a dummy symbol, a nonterminal, whose name cannot conflict |
| with the user's names.                                             |
`-------------------------------------------------------------------*/

static bucket *
gensym (void)
{
  /* Incremented for each generated symbol */
  static int gensym_count = 0;
  static char buf[256];

  bucket *sym;

  sprintf (buf, "@%d", ++gensym_count);
  token_buffer = buf;
  sym = getsym (token_buffer);
  sym->class = nterm_sym;
  sym->value = nvars++;
  return sym;
}

/*-------------------------------------------------------------------.
| Parse the input grammar into a one symbol_list structure.  Each    |
| rule is represented by a sequence of symbols: the left hand side   |
| followed by the contents of the right hand side, followed by a     |
| null pointer instead of a symbol to terminate the rule.  The next  |
| symbol is the lhs of the following rule.                           |
|                                                                    |
| All guards and actions are copied out to the appropriate files,    |
| labelled by the rule number they apply to.                         |
|                                                                    |
| Bison used to allow some %directives in the rules sections, but    |
| this is no longer consider appropriate: (i) the documented grammar |
| doesn't claim it, (ii), it would promote bad style, (iii), error   |
| recovery for %directives consists in skipping the junk until a `%' |
| is seen and helrp synchronizing.  This scheme is definitely wrong  |
| in the rules section.                                              |
`-------------------------------------------------------------------*/

static void
readgram (void)
{
  token_t t;
  bucket *lhs = NULL;
  symbol_list *p = NULL;
  symbol_list *p1 = NULL;
  bucket *bp;

  /* Points to first symbol_list of current rule. its symbol is the
     lhs of the rule.  */
  symbol_list *crule = NULL;
  /* Points to the symbol_list preceding crule.  */
  symbol_list *crule1 = NULL;

  t = lex ();

  while (t != tok_two_percents && t != tok_eof)
    if (t == tok_identifier || t == tok_bar)
      {
	int action_flag = 0;
	/* Number of symbols in rhs of this rule so far */
	int rulelength = 0;
	int xactions = 0;	/* JF for error checking */
	bucket *first_rhs = 0;

	if (t == tok_identifier)
	  {
	    lhs = symval;

	    if (!start_flag)
	      {
		startval = lhs;
		start_flag = 1;
	      }

	    t = lex ();
	    if (t != tok_colon)
	      {
		complain (_("ill-formed rule: initial symbol not followed by colon"));
		unlex (t);
	      }
	  }

	if (nrules == 0 && t == tok_bar)
	  {
	    complain (_("grammar starts with vertical bar"));
	    lhs = symval;	/* BOGUS: use a random symval */
	  }
	/* start a new rule and record its lhs.  */

	nrules++;
	nitems++;

	p = symbol_list_new (lhs);

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
	    if (t == tok_prec)
	      {
		t = lex ();
		crule->ruleprec = symval;
		t = lex ();
	      }

	    if (!(t == tok_identifier || t == tok_left_curly))
	      break;

	    /* If next token is an identifier, see if a colon follows it.
	       If one does, exit this rule now.  */
	    if (t == tok_identifier)
	      {
		bucket *ssave;
		token_t t1;

		ssave = symval;
		t1 = lex ();
		unlex (t1);
		symval = ssave;
		if (t1 == tok_colon)
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
		/* Since the action was written out with this rule's
		   number, we must give the new rule this number by
		   inserting the new rule before it.  */

		/* Make a dummy nonterminal, a gensym.  */
		bucket *sdummy = gensym ();

		/* Make a new rule, whose body is empty, before the
		   current one, so that the action just read can
		   belong to it.  */
		nrules++;
		nitems++;
		p = symbol_list_new (sdummy);
		/* Attach its lineno to that of the host rule. */
		p->line = crule->line;
		/* Move the action from the host rule to this one. */
		p->action = crule->action;
		p->action_line = crule->action_line;
		crule->action = NULL;

		if (crule1)
		  crule1->next = p;
		else
		  grammar = p;
		/* End of the rule. */
		crule1 = symbol_list_new (NULL);
		crule1->next = crule;

		p->next = crule1;

		/* Insert the dummy generated by that rule into this
		   rule.  */
		nitems++;
		p = symbol_list_new (sdummy);
		p1->next = p;
		p1 = p;

		action_flag = 0;
	      }

	    if (t == tok_identifier)
	      {
		nitems++;
		p = symbol_list_new (symval);
		p1->next = p;
		p1 = p;
	      }
	    else		/* handle an action.  */
	      {
		parse_action (crule, rulelength);
		action_flag = 1;
		xactions++;	/* JF */
	      }
	    rulelength++;
	  }			/* end of  read rhs of rule */

	/* Put an empty link in the list to mark the end of this rule  */
	p = symbol_list_new (NULL);
	p1->next = p;
	p1 = p;

	if (t == tok_prec)
	  {
	    complain (_("two @prec's in a row"));
	    t = lex ();
	    crule->ruleprec = symval;
	    t = lex ();
	  }

	if (t == tok_guard)
	  {
	    if (!semantic_parser)
	      complain (_("%%guard present but %%semantic_parser not specified"));

	    parse_guard (crule, rulelength);
	    t = lex ();
	  }

	if (t == tok_left_curly)
	  {
	    /* This case never occurs -wjh */
	    if (action_flag)
	      complain (_("two actions at end of one rule"));
	    parse_action (crule, rulelength);
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
	if (t == tok_semicolon)
	  t = lex ();
      }
    else
      {
	complain (_("invalid input: %s"), quote (token_buffer));
	t = lex ();
      }

  /* grammar has been read.  Do some checking */

  if (nrules == 0)
    fatal (_("no rules in the input grammar"));

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

  /* Insert the initial rule, which line is that of the first rule
     (not that of the start symbol):

     axiom: %start EOF.  */
  p = symbol_list_new (axiom);
  p->line = grammar->line;
  p->next = symbol_list_new (startval);
  p->next->next = symbol_list_new (eoftoken);
  p->next->next->next = symbol_list_new (NULL);
  p->next->next->next->next = grammar;
  nrules += 1;
  nitems += 3;
  grammar = p;
  startval = axiom;

  if (nsyms > MAXSHORT)
    fatal (_("too many symbols (tokens plus nonterminals); maximum %d"),
	   MAXSHORT);

  ntokens = nsyms - nvars;
}

/* At the end of the grammar file, some C source code must
   be stored. It is going to be associated to the epilogue
   directive.  */
static void
read_additionnal_code (void)
{
  char c;
  struct obstack el_obstack;

  obstack_init (&el_obstack);

  if (!no_lines_flag)
    {
      obstack_fgrow2 (&el_obstack, muscle_find ("linef"),
		      lineno, quotearg_style (c_quoting_style,
					      muscle_find ("filename")));
    }

  while ((c = getc (finput)) != EOF)
    obstack_1grow (&el_obstack, c);

  obstack_1grow (&el_obstack, 0);
  muscle_insert ("epilogue", obstack_finish (&el_obstack));
}


/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

static void
token_translations_init (void)
{
  bucket *bp = NULL;
  int i;

  token_translations = XCALLOC (short, max_user_token_number + 1);

  /* Initialize all entries for literal tokens to 2, the internal
     token number for $undefined., which represents all invalid
     inputs.  */
  for (i = 0; i <= max_user_token_number; i++)
    token_translations[i] = 2;

  for (bp = firstsymbol; bp; bp = bp->next)
    {
      /* Non-terminal? */
      if (bp->value >= ntokens)
	continue;
      /* A token string alias? */
      if (bp->user_token_number == SALIAS)
	continue;

      assert (bp->user_token_number != SUNDEF);

      /* A token which translation has already been set? */
      if (token_translations[bp->user_token_number] != 2)
	complain (_("tokens %s and %s both assigned number %d"),
		  symbols[token_translations[bp->user_token_number]]->tag,
		  bp->tag, bp->user_token_number);
      token_translations[bp->user_token_number] = bp->value;
    }
}


/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

static void
packsymbols (void)
{
  bucket *bp = NULL;
  int tokno = 1;
  int last_user_token_number;

  symbols = XCALLOC (bucket *, nsyms);

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
	  /* This symbol and its alias are a single token defn.
	     Allocate a tokno, and assign to both check agreement of
	     prec and assoc fields and make both the same */
	  if (bp->value == -1)
	    {
	      if (bp == eoftoken || bp->alias == eoftoken)
		bp->value = bp->alias->value = 0;
	      else
		{
		  bp->value = bp->alias->value = tokno++;
		}
	    }

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

	  /* Do not do processing below for SALIASs.  */
	  if (bp->user_token_number == SALIAS)
	    continue;

	}
      else /* bp->class == token_sym */
	{
	  if (bp == eoftoken)
	    bp->value = 0;
	  else
	    bp->value = tokno++;
	}

      if (bp->class == token_sym)
	{
	  if (bp->user_token_number == SUNDEF)
	    bp->user_token_number = ++last_user_token_number;
	  if (bp->user_token_number > max_user_token_number)
	    max_user_token_number = bp->user_token_number;
	}

      symbols[bp->value] = bp;
    }

  token_translations_init ();

  error_token_number = errtoken->value;

  if (startval->class == unknown_sym)
    fatal (_("the start symbol %s is undefined"), startval->tag);
  else if (startval->class == token_sym)
    fatal (_("the start symbol %s is a token"), startval->tag);

  start_symbol = startval->value;
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

  /* We use short to index items.  */
  if (nitems >= MAXSHORT)
    fatal (_("too many items (max %d)"), MAXSHORT);

  ritem = XCALLOC (short, nitems + 1);
  rules = XCALLOC (rule_t, nrules) - 1;

  itemno = 0;
  ruleno = 1;

  p = grammar;
  while (p)
    {
      bucket *ruleprec = p->ruleprec;
      rules[ruleno].lhs = p->sym->value;
      rules[ruleno].rhs = itemno;
      rules[ruleno].line = p->line;
      rules[ruleno].useful = TRUE;
      rules[ruleno].action = p->action;
      rules[ruleno].action_line = p->action_line;
      rules[ruleno].guard = p->guard;
      rules[ruleno].guard_line = p->guard_line;

      p = p->next;
      while (p && p->sym)
	{
	  ritem[itemno++] = p->sym->value;
	  /* A rule gets by default the precedence and associativity
	     of the last token in it.  */
	  if (p->sym->class == token_sym)
	    {
	      rules[ruleno].prec = p->sym->prec;
	      rules[ruleno].assoc = p->sym->assoc;
	    }
	  if (p)
	    p = p->next;
	}

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      if (ruleprec)
	{
	  rules[ruleno].prec = ruleprec->prec;
	  rules[ruleno].assoc = ruleprec->assoc;
	  rules[ruleno].precsym = ruleprec->value;
	}

      ritem[itemno++] = -ruleno;
      ruleno++;

      if (p)
	p = p->next;
    }

  ritem[itemno] = 0;
  nritems = itemno;
  assert (nritems == nitems);

  if (trace_flag)
    ritem_print (stderr);
}

/*-------------------------------------------------------------------.
| Read in the grammar specification and record it in the format      |
| described in gram.h.  All guards are copied into the GUARD_OBSTACK |
| and all actions into ACTION_OBSTACK, in each case forming the body |
| of a C function (YYGUARD or YYACTION) which contains a switch      |
| statement to decide which guard or action to execute.              |
`-------------------------------------------------------------------*/

void
reader (void)
{
  start_flag = 0;
  startval = NULL;		/* start symbol not specified yet. */

  nsyms = 0;
  nvars = 0;
  nrules = 0;
  nitems = 0;

  typed = 0;
  lastprec = 0;

  semantic_parser = 0;
  pure_parser = 0;

  grammar = NULL;

  lex_init ();
  lineno = 1;

  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  /* Initialize the symbol table.  */
  tabinit ();

  /* Construct the axiom symbol. */
  axiom = getsym ("$axiom");
  axiom->class = nterm_sym;
  axiom->value = nvars++;

  /* Construct the error token */
  errtoken = getsym ("error");
  errtoken->class = token_sym;
  errtoken->user_token_number = 256;	/* Value specified by POSIX.  */

  /* Construct a token that represents all undefined literal tokens.
     It is always token number 2.  */
  undeftoken = getsym ("$undefined.");
  undeftoken->class = token_sym;
  undeftoken->user_token_number = 2;

  /* Initialize the obstacks. */
  obstack_init (&action_obstack);
  obstack_init (&attrs_obstack);
  obstack_init (&output_obstack);

  finput = xfopen (infile, "r");

  /* Read the declaration section.  Copy %{ ... %} groups to
     TABLE_OBSTACK and FDEFINES file.  Also notice any %token, %left,
     etc. found there.  */
  read_declarations ();

  /* If the user did not define her EOFTOKEN, do it now. */
  if (!eoftoken)
    {
      eoftoken = getsym ("$");
      eoftoken->class = token_sym;
      /* Value specified by POSIX.  */
      eoftoken->user_token_number = 0;
    }

  /* Read in the grammar, build grammar in list form.  Write out
     guards and actions.  */
  readgram ();
  /* Some C code is given at the end of the grammar file. */
  read_additionnal_code ();

  lex_free ();
  xfclose (finput);

  /* Assign the symbols their symbol numbers.  Write #defines for the
     token symbols into FDEFINES if requested.  */
  packsymbols ();

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  LIST_FREE (symbol_list, grammar);
}
