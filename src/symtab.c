/* Symbol table manager for Bison,
   Copyright (C) 1984, 1989, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include "hash.h"
#include "complain.h"
#include "symtab.h"
#include "gram.h"

/*------------------------.
| Distinguished symbols.  |
`------------------------*/

symbol_t *errtoken = NULL;
symbol_t *undeftoken = NULL;
symbol_t *eoftoken = NULL;
symbol_t *axiom = NULL;
symbol_t *startsymbol = NULL;
location_t startsymbol_location;

/*---------------------------------.
| Create a new symbol, named TAG.  |
`---------------------------------*/

static symbol_t *
symbol_new (const char *tag, location_t location)
{
  symbol_t *res = XMALLOC (symbol_t, 1);

  res->tag = xstrdup (tag);
  res->location = location;

  res->type_name = NULL;
  res->destructor = NULL;

  res->number = NUMBER_UNDEFINED;
  res->prec = 0;
  res->assoc = right_assoc;
  res->user_token_number = USER_NUMBER_UNDEFINED;
  res->alias = NULL;
  res->class = unknown_sym;

  nsyms++;
  return res;
}


/*-----------------------------------------------------------------.
| Return the tag of this SYMBOL in a printable form.  Warning: use |
| the first QUOTEARG slot: 0.                                      |
`-----------------------------------------------------------------*/

const char *
symbol_tag_get (symbol_t *symbol)
{
  return quotearg_style (escape_quoting_style, symbol->tag);
}


/*------------------------------------------------------------.
| Return the tag of this SYMBOL in a printable form.  Use the |
| QUOTEARG slot number N.                                     |
`------------------------------------------------------------*/

const char *
symbol_tag_get_n (symbol_t *symbol, int n)
{
  return quotearg_n_style (n, escape_quoting_style, symbol->tag);
}


/*-------------------------------.
| Print the tag of this SYMBOL.  |
`-------------------------------*/

void
symbol_tag_print (symbol_t *symbol, FILE *out)
{
  fputs (symbol_tag_get (symbol), out);
}


/*------------------------------------------------------------------.
| Set the TYPE_NAME associated to SYMBOL.  Does nothing if passed 0 |
| as TYPE_NAME.                                                     |
`------------------------------------------------------------------*/

void
symbol_type_set (symbol_t *symbol, location_t location, char *type_name)
{
  if (type_name)
    {
      if (symbol->type_name)
	complain_at (location,
		     _("type redeclaration for %s"), symbol_tag_get (symbol));
      symbol->type_name = type_name;
    }
}


/*-------------------------------------------------------------------.
| Set the DESTRUCTOR associated to SYMBOL.  Do nothing if passed 0.  |
`-------------------------------------------------------------------*/

void
symbol_destructor_set (symbol_t *symbol, location_t location, char *destructor)
{
  if (destructor)
    {
      if (symbol->destructor)
	complain_at (location,
		     _("%s redeclaration for %s"),
		     "%destructor", symbol_tag_get (symbol));
      symbol->destructor = destructor;
      symbol->destructor_location = location;
    }
}


/*----------------------------------------------------------------.
| Set the PRITNER associated to SYMBOL.  Do nothing if passed 0.  |
`----------------------------------------------------------------*/

void
symbol_printer_set (symbol_t *symbol, char *printer, location_t location)
{
  if (printer)
    {
      if (symbol->printer)
	complain_at (location,
		     _("%s redeclaration for %s"),
		     "%printer", symbol_tag_get (symbol));
      symbol->printer = printer;
      symbol->printer_location = location;
    }
}


/*------------------------------------------------------------------.
| Set the PRECEDENCE associated to SYMBOL.  Does nothing if invoked |
| with UNDEF_ASSOC as ASSOC.                                        |
`------------------------------------------------------------------*/

void
symbol_precedence_set (symbol_t *symbol, location_t location,
		       int prec, associativity assoc)
{
  if (assoc != undef_assoc)
    {
      if (symbol->prec != 0)
	complain_at (location,
		     _("redefining precedence of %s"),
		     symbol_tag_get (symbol));
      symbol->prec = prec;
      symbol->assoc = assoc;
    }

  /* Only terminals have a precedence. */
  symbol_class_set (symbol, token_sym, location);
}


/*-------------------------------------.
| Set the CLASS associated to SYMBOL.  |
`-------------------------------------*/

void
symbol_class_set (symbol_t *symbol, symbol_class class, location_t location)
{
  if (symbol->class != unknown_sym && symbol->class != class)
    complain_at (location, _("symbol %s redefined"), symbol_tag_get (symbol));

  if (class == nterm_sym && symbol->class != nterm_sym)
    symbol->number = nvars++;
  else if (class == token_sym && symbol->number == NUMBER_UNDEFINED)
    symbol->number = ntokens++;

  symbol->class = class;
}


/*-------------------------------------------------.
| Set the USER_TOKEN_NUMBER associated to SYMBOL.  |
`-------------------------------------------------*/

void
symbol_user_token_number_set (symbol_t *symbol,
			      int user_token_number, location_t location)
{
  assert (symbol->class == token_sym);

  if (symbol->user_token_number != USER_NUMBER_UNDEFINED
      && symbol->user_token_number != user_token_number)
    complain_at (location, _("redefining user token number of %s"),
		 symbol_tag_get (symbol));

  symbol->user_token_number = user_token_number;
  /* User defined EOF token? */
  if (user_token_number == 0)
    {
      eoftoken = symbol;
      eoftoken->number = 0;
      /* It is always mapped to 0, so it was already counted in
	 NTOKENS.  */
      --ntokens;
    }
}


/*------------.
| Free THIS.  |
`------------*/

static void
symbol_free (symbol_t *this)
{
#if 0
  /* This causes crashes because one string can appear more
     than once.  */
  XFREE (this->type_name);
#endif
  XFREE (this->tag);
  XFREE (this);
}


/*-----------------------------------------------------------.
| If THIS is not defined, report an error, and consider it a |
| nonterminal.                                               |
`-----------------------------------------------------------*/

static bool
symbol_check_defined (symbol_t *this)
{
  if (this->class == unknown_sym)
    {
      complain_at
	(this->location,
	 _("symbol %s is used, but is not defined as a token and has no rules"),
	 symbol_tag_get (this));
      this->class = nterm_sym;
      this->number = nvars++;
    }

  return TRUE;
}


/*-------------------------------------------------------------------.
| Declare the new SYMBOL.  Make it an alias of SYMVAL, and type them |
| with TYPENAME.                                                     |
`-------------------------------------------------------------------*/

void
symbol_make_alias (symbol_t *symbol, symbol_t *symval)
{
  if (symval->alias)
    warn (_("symbol `%s' used more than once as a literal string"),
	  symbol_tag_get (symval));
  else if (symbol->alias)
    warn (_("symbol `%s' given more than one literal string"),
	  symbol_tag_get (symbol));
  else
    {
      symval->class = token_sym;
      symval->user_token_number = symbol->user_token_number;
      symbol->user_token_number = USER_NUMBER_ALIAS;
      symval->alias = symbol;
      symbol->alias = symval;
      /* symbol and symval combined are only one symbol */
      nsyms--;
      ntokens--;
      assert (ntokens == symbol->number || ntokens == symval->number);
      symbol->number = symval->number =
	(symval->number < symbol->number) ? symval->number : symbol->number;
    }
}


/*---------------------------------------------------------.
| Check that THIS, and its alias, have same precedence and |
| associativity.                                           |
`---------------------------------------------------------*/

static bool
symbol_check_alias_consistence (symbol_t *this)
{
  /* Check only those who _are_ the aliases. */
  if (this->alias && this->user_token_number == USER_NUMBER_ALIAS)
    {
      if (this->prec != this->alias->prec)
	{
	  if (this->prec != 0 && this->alias->prec != 0)
	    complain (_("conflicting precedences for %s and %s"),
		      symbol_tag_get (this), symbol_tag_get (this->alias));
	  if (this->prec != 0)
	    this->alias->prec = this->prec;
	  else
	    this->prec = this->alias->prec;
	}

      if (this->assoc != this->alias->assoc)
	{
	  /* FIXME: For some reason (probably the S/R => keep the S),
	     the right assoc is chosen has the ``not set''.  This is
	     not nice, fix this!  */
	  if (this->assoc != right_assoc
	      && this->alias->assoc != right_assoc)
	    complain (_("conflicting associativities for %s and %s"),
		      symbol_tag_get (this), symbol_tag_get (this->alias));
	  if (this->assoc != 0)
	    this->alias->assoc = this->assoc;
	  else
	    this->assoc = this->alias->assoc;
	}
    }
  return TRUE;
}


/*-------------------------------------------------------------------.
| Assign a symbol number, and write the definition of the token name |
| into FDEFINES.  Put in SYMBOLS.                                    |
`-------------------------------------------------------------------*/

static bool
symbol_pack (symbol_t *this)
{
  if (this->class == nterm_sym)
    {
      this->number += ntokens;
    }
  else if (this->alias)
    {
      /* This symbol and its alias are a single token defn.
	 Allocate a tokno, and assign to both check agreement of
	 prec and assoc fields and make both the same */
      if (this->number == NUMBER_UNDEFINED)
	{
	  if (this == eoftoken || this->alias == eoftoken)
	    this->number = this->alias->number = 0;
	  else
	    {
	      assert (this->alias->number != NUMBER_UNDEFINED);
	      this->number = this->alias->number;
	    }
	}
      /* Do not do processing below for USER_NUMBER_ALIASs.  */
      if (this->user_token_number == USER_NUMBER_ALIAS)
	return TRUE;
    }
  else /* this->class == token_sym */
    {
      assert (this->number != NUMBER_UNDEFINED);
    }

  symbols[this->number] = this;
  return TRUE;
}




/*--------------------------------------------------.
| Put THIS in TOKEN_TRANSLATIONS if it is a token.  |
`--------------------------------------------------*/

static bool
symbol_translation (symbol_t *this)
{
  /* Non-terminal? */
  if (this->class == token_sym
      && this->user_token_number != USER_NUMBER_ALIAS)
    {
      /* A token which translation has already been set? */
      if (token_translations[this->user_token_number] != undeftoken->number)
	complain (_("tokens %s and %s both assigned number %d"),
		  symbol_tag_get (symbols[token_translations[this->user_token_number]]),
		  symbol_tag_get (this), this->user_token_number);

      token_translations[this->user_token_number] = this->number;
    }

  return TRUE;
}


/*----------------------.
| A symbol hash table.  |
`----------------------*/

/* Initial capacity of symbols hash table.  */
#define HT_INITIAL_CAPACITY 257

static struct hash_table *symbol_table = NULL;

static bool
hash_compare_symbol_t (const symbol_t *m1, const symbol_t *m2)
{
  return strcmp (m1->tag, m2->tag) ? FALSE : TRUE;
}

static unsigned int
hash_symbol_t (const symbol_t *m, unsigned int tablesize)
{
  return hash_string (m->tag, tablesize);
}


/*-------------------------------.
| Create the symbol hash table.  |
`-------------------------------*/

void
symbols_new (void)
{
  symbol_table = hash_initialize (HT_INITIAL_CAPACITY,
				  NULL,
				  (Hash_hasher) hash_symbol_t,
				  (Hash_comparator) hash_compare_symbol_t,
				  (Hash_data_freer) symbol_free);
}


/*----------------------------------------------------------------.
| Find the symbol named KEY, and return it.  If it does not exist |
| yet, create it.                                                 |
`----------------------------------------------------------------*/

symbol_t *
getsym (const char *key, location_t location)
{
  symbol_t probe;
  symbol_t *entry;

  (const char *) probe.tag = key;
  entry = hash_lookup (symbol_table, &probe);

  if (!entry)
    {
      /* First insertion in the hash. */
      entry = symbol_new (key, location);
      hash_insert (symbol_table, entry);
    }
  return entry;
}


/*-------------------.
| Free the symbols.  |
`-------------------*/

void
symbols_free (void)
{
  hash_free (symbol_table);
}


/*---------------------------------------------------------------.
| Look for undefined symbols, report an error, and consider them |
| terminals.                                                     |
`---------------------------------------------------------------*/

void
symbols_do (symbol_processor processor, void *processor_data)
{
  hash_do_for_each (symbol_table,
		    (Hash_processor) processor,
		    processor_data);
}


/*--------------------------------------------------------------.
| Check that all the symbols are defined.  Report any undefined |
| symbols and consider them nonterminals.                       |
`--------------------------------------------------------------*/

void
symbols_check_defined (void)
{
  symbols_do (symbol_check_defined, NULL);
}

/*------------------------------------------------------------------.
| Set TOKEN_TRANSLATIONS.  Check that no two symbols share the same |
| number.                                                           |
`------------------------------------------------------------------*/

static void
symbols_token_translations_init (void)
{
  int num_256_available_p = TRUE;
  int i;

  /* Find the highest user token number, and whether 256, the POSIX
     preferred user token number for the error token, is used.  */
  max_user_token_number = 0;
  for (i = 0; i < ntokens; ++i)
    {
      symbol_t *this = symbols[i];
      if (this->user_token_number != USER_NUMBER_UNDEFINED)
	{
	  if (this->user_token_number > max_user_token_number)
	    max_user_token_number = this->user_token_number;
	  if (this->user_token_number == 256)
	    num_256_available_p = FALSE;
	}
    }

  /* If 256 is not used, assign it to error, to follow POSIX.  */
  if (num_256_available_p
      && errtoken->user_token_number == USER_NUMBER_UNDEFINED)
    errtoken->user_token_number = 256;

  /* Set the missing user numbers. */
  if (max_user_token_number < 256)
    max_user_token_number = 256;

  for (i = 0; i < ntokens; ++i)
    {
      symbol_t *this = symbols[i];
      if (this->user_token_number == USER_NUMBER_UNDEFINED)
	this->user_token_number = ++max_user_token_number;
      if (this->user_token_number > max_user_token_number)
	max_user_token_number = this->user_token_number;
    }

  token_translations = XCALLOC (symbol_number_t, max_user_token_number + 1);

  /* Initialize all entries for literal tokens to 2, the internal
     token number for $undefined., which represents all invalid
     inputs.  */
  for (i = 0; i < max_user_token_number + 1; i++)
    token_translations[i] = undeftoken->number;
  symbols_do (symbol_translation, NULL);
}


/*----------------------------------------------------------------.
| Assign symbol numbers, and write definition of token names into |
| FDEFINES.  Set up vectors SYMBOL_TABLE, TAGS of symbols.        |
`----------------------------------------------------------------*/

void
symbols_pack (void)
{
  symbols = XCALLOC (symbol_t *, nsyms);

  symbols_do (symbol_check_alias_consistence, NULL);
  symbols_do (symbol_pack, NULL);

  symbols_token_translations_init ();

  if (startsymbol->class == unknown_sym)
    fatal_at (startsymbol_location,
	      _("the start symbol %s is undefined"),
	      symbol_tag_get (startsymbol));
  else if (startsymbol->class == token_sym)
    fatal_at (startsymbol_location,
	      _("the start symbol %s is a token"),
	      symbol_tag_get (startsymbol));
}
