/* Input parser for bison
   Copyright (C) 1984, 1986, 1989, 1992, 1998, 2000, 2001, 2002
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
  symbol_t *sym;
  int line;

  /* The action is attached to the LHS of a rule. */
  const char *action;
  int action_line;

  symbol_t *ruleprec;
} symbol_list;

int lineno;
static symbol_list *grammar = NULL;
static int start_flag = 0;

/* Nonzero if %union has been seen.  */
static int typed = 0;

/* Incremented for each %left, %right or %nonassoc seen */
static int lastprec = 0;

static symbol_list *
symbol_list_new (symbol_t *sym)
{
  symbol_list *res = XMALLOC (symbol_list, 1);
  res->next = NULL;
  res->sym = sym;
  res->line = lineno;
  res->action = NULL;
  res->action_line = 0;
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
      ++i;
    }

  return rp->sym->type_name;
}

/*------------------------------------------------------------------.
| Copy the character C to OOUT, and insert quadigraphs when needed. |
`------------------------------------------------------------------*/

static inline void
copy_character (struct obstack *oout, int c)
{
  switch (c)
    {
    case '[':
      obstack_sgrow (oout, "@<:@");
      break;

    case ']':
      obstack_sgrow (oout, "@:>@");
      break;

    default:
      obstack_1grow (oout, c);
    }
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

      copy_character (oout, c);

      if (c == '\\')
	{
	  c = getc (fin);
	  if (c == EOF)
	    fatal (_("unterminated string at end of file"));
	  copy_character (oout, c);

	  if (c == '\n')
	    ++lineno;
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
	  ++lineno;
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
	  copy_character (oout, c);
	  c = getc (fin);
	}
    }
}


/*-------------------------------------------------------------------.
| FIN is pointing to a location (i.e., a `@').  Output to OOUT a     |
| reference to this location. RULE_LENGTH is the number of values in |
| the current rule so far, which says where to find `$0' with        |
| respect to the top of the stack.                                   |
`-------------------------------------------------------------------*/

static inline void
copy_at (FILE *fin, struct obstack *oout, int rule_length)
{
  int c = getc (fin);
  locations_flag = 1;

  if (c == '$')
    {
      obstack_sgrow (oout, "]b4_lhs_location[");
    }
  else if (isdigit (c) || c == '-')
    {
      int n;

      ungetc (c, fin);
      n = read_signed_integer (fin);
      if (n > rule_length)
	complain (_("invalid value: %s%d"), "@", n);
      else
	obstack_fgrow2 (oout, "]b4_rhs_location([%d], [%d])[",
			rule_length, n);
    }
  else
    {
      char buf[] = "@c";
      buf[1] = c;
      complain (_("%s is invalid"), quote (buf));
    }
}


/*------------------------------------------------------------------.
| FIN is pointing to a wannabee semantic value (i.e., a `$').       |
|                                                                   |
| Possible inputs: $[<TYPENAME>]($|integer)                         |
|                                                                   |
| Output to OOUT a reference to this semantic value. RULE_LENGTH is |
| the number of values in the current rule so far, which says where |
| to find `$0' with respect to the top of the stack.                |
`------------------------------------------------------------------*/

static inline void
copy_dollar (FILE *fin, struct obstack *oout,
	     symbol_list *rule, int rule_length)
{
  int c = getc (fin);
  const char *type_name = NULL;

  /* Get the type name if explicit. */
  if (c == '<')
    {
      read_type_name (fin);
      type_name = token_buffer;
      c = getc (fin);
    }

  if (c == '$')
    {
      if (!type_name)
	type_name = get_type_name (0, rule);
      if (!type_name && typed)
	complain (_("$$ of `%s' has no declared type"),
		  rule->sym->tag);
      if (!type_name)
	type_name = "";
      obstack_fgrow1 (oout,
		      "]b4_lhs_value([%s])[", type_name);
    }
  else if (isdigit (c) || c == '-')
    {
      int n;
      ungetc (c, fin);
      n = read_signed_integer (fin);

      if (n > rule_length)
	complain (_("invalid value: %s%d"), "$", n);
      else
	{
	  if (!type_name && n > 0)
	    type_name = get_type_name (n, rule);
	  if (!type_name && typed)
	    complain (_("$%d of `%s' has no declared type"),
		      n, rule->sym->tag);
	  if (!type_name)
	    type_name = "";
	  obstack_fgrow3 (oout, "]b4_rhs_value([%d], [%d], [%s])[",
			  rule_length, n, type_name);
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
copy_definition (struct obstack *oout)
{
  int c;
  /* -1 while reading a character if prev char was %. */
  int after_percent;

  if (!no_lines_flag)
    {
      obstack_fgrow2 (oout, muscle_find ("linef"),
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
	  obstack_1grow (oout, c);
	  ++lineno;
	  break;

	case '%':
	  after_percent = -1;
	  break;

	case '\'':
	case '"':
	  copy_string (finput, oout, c);
	  break;

	case '/':
	  copy_comment (finput, oout);
	  break;

	case EOF:
	  fatal ("%s", _("unterminated `%{' definition"));

	default:
	  copy_character (oout, c);
	}

      c = getc (finput);

      if (after_percent)
	{
	  if (c == '}')
	    return;
	  obstack_1grow (oout, '%');
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
  symbol_t *symbol = NULL;

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
	  symbol = NULL;
	}
      else if (token == tok_identifier && *symval->tag == '\"' && symbol)
	{
	  symbol_make_alias (symbol, symval, typename);
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
	    symbol->number = nvars++;
	  if (what_is == token_sym && symbol->number == NUMBER_UNDEFINED)
	    symbol->number = ntokens++;

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
	    {
	      eoftoken = symbol;
	      eoftoken->number = 0;
	      /* It is always mapped to 0, so it was already counted in
		 NTOKENS.  */
	      --ntokens;
	    }
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
      startsymbol = symval;
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
	  symbol_type_set (symval, name);
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

  /* Assign a new precedence level, never 0.  */
  ++lastprec;

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
	  if (symval->class == nterm_sym)
	    complain (_("symbol %s redefined"), symval->tag);
	  if (symval->number == NUMBER_UNDEFINED)
	    {
	      symval->number = ntokens++;
	      symval->class = token_sym;
	    }
	  symbol_precedence_set (symval, lastprec, assoc);
	  if (name)
	    symbol_type_set (symval, name);
	  break;

	case tok_number:
	  if (prev == tok_identifier)
	    {
	      symval->user_token_number = numval;
	    }
	  else
	    {
	      complain
		(_("invalid text (%s) - number should be after identifier"),
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

  MUSCLE_INSERT_INT ("stype_line", lineno);
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
	  ++lineno;
	  break;

	case '/':
	  copy_comment (finput, &union_obstack);
	  break;

	case '{':
	  ++count;
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
| groups to PRE_PROLOGUE_OBSTACK or POST_PROLOGUE_OBSTACK.        |
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
              if (!typed)
		copy_definition (&pre_prologue_obstack);
	      else
		copy_definition (&post_prologue_obstack);
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
	      assert (0);
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

/*------------------------------------------------------------------.
| Assuming that a `{' has just been seen, copy everything up to the |
| matching `}' into ACTION_OBSTACK.                                 |
|                                                                   |
| RULE_LENGTH is the number of values in the current rule so far,   |
| which says where to find `$0' with respect to the top of the      |
| stack.  It is not the same as the rule->length in the case of mid |
| rule actions.                                                     |
|                                                                   |
| This routine is used for actions.                                 |
`------------------------------------------------------------------*/

static void
parse_action (symbol_list *rule, int rule_length)
{
  int count = 1;
  rule->action_line = lineno;
  while (count > 0)
    {
      int c;
      while ((c = getc (finput)) != '}')
	switch (c)
	  {
	  case '\n':
	    copy_character (&action_obstack, c);
	    ++lineno;
	    break;

	  case '{':
	    copy_character (&action_obstack, c);
	    ++count;
	    break;

	  case '\'':
	  case '"':
	    copy_string (finput, &action_obstack, c);
	    break;

	  case '/':
	    copy_comment (finput, &action_obstack);
	    break;

	  case '$':
	    copy_dollar (finput, &action_obstack, rule, rule_length);
	    break;

	  case '@':
	    copy_at (finput, &action_obstack, rule_length);
	    break;

	  case EOF:
	    fatal (_("unmatched %s"), "`{'");

	  default:
	    copy_character (&action_obstack, c);
	  }

      /* Above loop exits when C is '}'.  */
      if (--count)
	copy_character (&action_obstack, c);
    }

  obstack_1grow (&action_obstack, '\0');
  rule->action = obstack_finish (&action_obstack);
}



/*-------------------------------------------------------------------.
| Generate a dummy symbol, a nonterminal, whose name cannot conflict |
| with the user's names.                                             |
`-------------------------------------------------------------------*/

static symbol_t *
gensym (void)
{
  /* Incremented for each generated symbol */
  static int gensym_count = 0;
  static char buf[256];

  symbol_t *sym;

  sprintf (buf, "@%d", ++gensym_count);
  token_buffer = buf;
  sym = getsym (token_buffer);
  sym->class = nterm_sym;
  sym->number = nvars++;
  return sym;
}

/*-------------------------------------------------------------------.
| Parse the input grammar into a one symbol_list structure.  Each    |
| rule is represented by a sequence of symbols: the left hand side   |
| followed by the contents of the right hand side, followed by a     |
| null pointer instead of a symbol to terminate the rule.  The next  |
| symbol is the lhs of the following rule.                           |
|                                                                    |
| All actions are copied out, labelled by the rule number they apply |
| to.                                                                |
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
  symbol_t *lhs = NULL;
  symbol_list *p = NULL;
  symbol_list *p1 = NULL;

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
	symbol_t *first_rhs = 0;

	if (t == tok_identifier)
	  {
	    lhs = symval;

	    if (!start_flag)
	      {
		startsymbol = lhs;
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

	++nrules;
	++nritems;

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
	    lhs->number = nvars;
	    ++nvars;
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
		symbol_t *ssave;
		token_t t1;

		ssave = symval;
		t1 = lex ();
		unlex (t1);
		symval = ssave;
		if (t1 == tok_colon)
		  {
		    warn (_("previous rule lacks an ending `;'"));
		    break;
		  }

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
		symbol_t *sdummy = gensym ();

		/* Make a new rule, whose body is empty, before the
		   current one, so that the action just read can
		   belong to it.  */
		++nrules;
		++nritems;
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
		++nritems;
		p = symbol_list_new (sdummy);
		p1->next = p;
		p1 = p;

		action_flag = 0;
	      }

	    if (t == tok_identifier)
	      {
		++nritems;
		p = symbol_list_new (symval);
		p1->next = p;
		p1 = p;
	      }
	    else		/* handle an action.  */
	      {
		parse_action (crule, rulelength);
		action_flag = 1;
		++xactions;	/* JF */
	      }
	    ++rulelength;
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

	if (t == tok_left_curly)
	  {
	    /* This case never occurs -wjh */
	    if (action_flag)
	      complain (_("two actions at end of one rule"));
	    parse_action (crule, rulelength);
	    action_flag = 1;
	    ++xactions;	/* -wjh */
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
	if (t == tok_two_percents || t == tok_eof)
	  warn (_("previous rule lacks an ending `;'"));
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
  symbols_check_defined ();

  /* Insert the initial rule, which line is that of the first rule
     (not that of the start symbol):

     axiom: %start EOF.  */
  p = symbol_list_new (axiom);
  p->line = grammar->line;
  p->next = symbol_list_new (startsymbol);
  p->next->next = symbol_list_new (eoftoken);
  p->next->next->next = symbol_list_new (NULL);
  p->next->next->next->next = grammar;
  nrules += 1;
  nritems += 3;
  grammar = p;

  if (nsyms > SHRT_MAX)
    fatal (_("too many symbols (tokens plus nonterminals); maximum %d"),
	   SHRT_MAX);

  assert (nsyms == ntokens + nvars);
}

/* At the end of the grammar file, some C source code must
   be stored. It is going to be associated to the epilogue
   directive.  */
static void
read_additionnal_code (void)
{
  int c;
  struct obstack el_obstack;

  obstack_init (&el_obstack);

  if (!no_lines_flag)
    {
      obstack_fgrow2 (&el_obstack, muscle_find ("linef"),
		      lineno, quotearg_style (c_quoting_style,
					      muscle_find ("filename")));
    }

  while ((c = getc (finput)) != EOF)
    copy_character (&el_obstack, c);

  obstack_1grow (&el_obstack, 0);
  muscle_insert ("epilogue", obstack_finish (&el_obstack));
}


/*---------------------------------------------------------------.
| Convert the rules into the representation using RRHS, RLHS and |
| RITEM.                                                         |
`---------------------------------------------------------------*/

static void
packgram (void)
{
  unsigned int itemno;
  int ruleno;
  symbol_list *p;

  ritem = XCALLOC (item_number_t, nritems);
  rules = XCALLOC (rule_t, nrules) - 1;

  itemno = 0;
  ruleno = 1;

  p = grammar;
  while (p)
    {
      symbol_t *ruleprec = p->ruleprec;
      rules[ruleno].user_number = ruleno;
      rules[ruleno].number = ruleno;
      rules[ruleno].lhs = p->sym;
      rules[ruleno].rhs = ritem + itemno;
      rules[ruleno].line = p->line;
      rules[ruleno].useful = TRUE;
      rules[ruleno].action = p->action;
      rules[ruleno].action_line = p->action_line;

      p = p->next;
      while (p && p->sym)
	{
	  /* item_number_t = symbol_number_t.
	     But the former needs to contain more: negative rule numbers. */
	  ritem[itemno++] = symbol_number_as_item_number (p->sym->number);
	  /* A rule gets by default the precedence and associativity
	     of the last token in it.  */
	  if (p->sym->class == token_sym)
	    rules[ruleno].prec = p->sym;
	  if (p)
	    p = p->next;
	}

      /* If this rule has a %prec,
         the specified symbol's precedence replaces the default.  */
      if (ruleprec)
	{
	  rules[ruleno].precsym = ruleprec;
	  rules[ruleno].prec = ruleprec;
	}
      ritem[itemno++] = -ruleno;
      ++ruleno;

      if (p)
	p = p->next;
    }

  assert (itemno == nritems);

  if (trace_flag)
    ritem_print (stderr);
}

/*------------------------------------------------------------------.
| Read in the grammar specification and record it in the format     |
| described in gram.h.  All actions are copied into ACTION_OBSTACK, |
| in each case forming the body of a C function (YYACTION) which    |
| contains a switch statement to decide which action to execute.    |
`------------------------------------------------------------------*/

void
reader (void)
{
  lex_init ();
  lineno = 1;

  /* Initialize the muscle obstack.  */
  obstack_init (&muscle_obstack);

  /* Initialize the symbol table.  */
  symbols_new ();

  /* Construct the axiom symbol. */
  axiom = getsym ("$axiom");
  axiom->class = nterm_sym;
  axiom->number = nvars++;

  /* Construct the error token */
  errtoken = getsym ("error");
  errtoken->class = token_sym;
  errtoken->number = ntokens++;

  /* Construct a token that represents all undefined literal tokens.
     It is always token number 2.  */
  undeftoken = getsym ("$undefined.");
  undeftoken->class = token_sym;
  undeftoken->number = ntokens++;

  /* Initialize the obstacks. */
  obstack_init (&action_obstack);
  obstack_init (&output_obstack);
  obstack_init (&pre_prologue_obstack);
  obstack_init (&post_prologue_obstack);

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
      eoftoken->number = 0;
      /* Value specified by POSIX.  */
      eoftoken->user_token_number = 0;
    }

  /* Read in the grammar, build grammar in list form.  Write out
     actions.  */
  readgram ();
  /* Some C code is given at the end of the grammar file. */
  read_additionnal_code ();

  lex_free ();
  xfclose (finput);

  /* Assign the symbols their symbol numbers.  Write #defines for the
     token symbols into FDEFINES if requested.  */
  symbols_pack ();

  /* Convert the grammar into the format described in gram.h.  */
  packgram ();

  /* The grammar as a symbol_list is no longer needed. */
  LIST_FREE (symbol_list, grammar);
}
