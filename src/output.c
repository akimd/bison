/* Output the generated parsing program for bison,
   Copyright (C) 1984, 1986, 1989, 1992, 2000, 2001, 2002
   Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */


#include "system.h"
#include "quotearg.h"
#include "error.h"
#include "subpipe.h"
#include "getargs.h"
#include "files.h"
#include "gram.h"
#include "complain.h"
#include "output.h"
#include "reader.h"
#include "symtab.h"
#include "tables.h"
#include "muscle_tab.h"

/* From src/scan-skel.l. */
void scan_skel (FILE *);


static struct obstack format_obstack;

int error_verbose = 0;



/*-------------------------------------------------------------------.
| Create a function NAME which associates to the muscle NAME the     |
| result of formatting the FIRST and then TABLE_DATA[BEGIN..END[ (of |
| TYPE), and to the muscle NAME_max, the max value of the            |
| TABLE_DATA.                                                        |
`-------------------------------------------------------------------*/


#define GENERATE_MUSCLE_INSERT_TABLE(Name, Type)			\
									\
static void								\
Name (const char *name,							\
      Type *table_data,							\
      Type first,							\
      int begin,							\
      int end)								\
{									\
  Type min = first;							\
  Type max = first;							\
  int i;								\
  int j = 1;								\
									\
  obstack_fgrow1 (&format_obstack, "%6d", first);			\
  for (i = begin; i < end; ++i)						\
    {									\
      obstack_1grow (&format_obstack, ',');				\
      if (j >= 10)							\
	{								\
	  obstack_sgrow (&format_obstack, "\n  ");			\
	  j = 1;							\
	}								\
      else								\
	++j;								\
      obstack_fgrow1 (&format_obstack, "%6d", table_data[i]);		\
      if (table_data[i] < min)						\
	min = table_data[i];						\
      if (max < table_data[i])						\
	max = table_data[i];						\
    }									\
  obstack_1grow (&format_obstack, 0);					\
  muscle_insert (name, obstack_finish (&format_obstack));		\
									\
  /* Build `NAME_min' and `NAME_max' in the obstack. */			\
  obstack_fgrow1 (&format_obstack, "%s_min", name);			\
  obstack_1grow (&format_obstack, 0);					\
  MUSCLE_INSERT_LONG_INT (obstack_finish (&format_obstack),		\
			  (long int) min);				\
  obstack_fgrow1 (&format_obstack, "%s_max", name);			\
  obstack_1grow (&format_obstack, 0);					\
  MUSCLE_INSERT_LONG_INT (obstack_finish (&format_obstack),		\
			  (long int) max);				\
}

GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_unsigned_int_table, unsigned int)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_int_table, int)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_short_table, short)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_base_table, base_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_rule_number_table, rule_number_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_symbol_number_table, symbol_number_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_item_number_table, item_number_t)
GENERATE_MUSCLE_INSERT_TABLE(muscle_insert_state_number_table, state_number_t)


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

static void
prepare_symbols (void)
{
  MUSCLE_INSERT_INT ("tokens_number", ntokens);
  MUSCLE_INSERT_INT ("nterms_number", nvars);
  MUSCLE_INSERT_INT ("undef_token_number", undeftoken->number);
  MUSCLE_INSERT_INT ("user_token_number_max", max_user_token_number);

  muscle_insert_symbol_number_table ("translate",
				     token_translations,
				     token_translations[0],
				     1, max_user_token_number + 1);

  /* tname -- token names.  */
  {
    int i;
    int j = 0;
    for (i = 0; i < nsyms; i++)
      {
	/* Be sure not to use twice the same QUOTEARG slot:
	   SYMBOL_TAG_GET uses slot 0.  */
	const char *cp =
	  quotearg_n_style (1, c_quoting_style,
			    symbols[i]->tag);
	/* Width of the next token, including the two quotes, the comma
	   and the space.  */
	int strsize = strlen (cp) + 2;

	if (j + strsize > 75)
	  {
	    obstack_sgrow (&format_obstack, "\n  ");
	    j = 2;
	  }

	for (; *cp; cp++)
	  switch (*cp)
	    {
	    case '[': obstack_sgrow (&format_obstack, "@<:@"); break;
	    case ']': obstack_sgrow (&format_obstack, "@:>@"); break;
	    default: obstack_1grow (&format_obstack, *cp); break;
	    }

	obstack_sgrow (&format_obstack, ", ");
	j += strsize;
      }
    /* Add a NULL entry to list of tokens (well, 0, as NULL might not be
       defined).  */
    obstack_sgrow (&format_obstack, "0");

    /* Finish table and store. */
    obstack_1grow (&format_obstack, 0);
    muscle_insert ("tname", obstack_finish (&format_obstack));
  }

  /* Output YYTOKNUM. */
  {
    int i;
    int *values = XCALLOC (int, ntokens);
    for (i = 0; i < ntokens; ++i)
      values[i] = symbols[i]->user_token_number;
    muscle_insert_int_table ("toknum", values,
			     values[0], 1, ntokens);
    free (values);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger                                         |
`-------------------------------------------------------------*/

static void
prepare_rules (void)
{
  rule_number_t r;
  unsigned int i = 0;
  item_number_t *rhs = XMALLOC (item_number_t, nritems);
  unsigned int *prhs = XMALLOC (unsigned int, nrules);
  unsigned int *rline = XMALLOC (unsigned int, nrules);
  symbol_number_t *r1 = XMALLOC (symbol_number_t, nrules);
  unsigned int *r2 = XMALLOC (unsigned int, nrules);
  short *dprec = XMALLOC (short, nrules);
  short *merger = XMALLOC (short, nrules);

  for (r = 0; r < nrules; ++r)
    {
      item_number_t *rhsp = NULL;
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      for (rhsp = rules[r].rhs; *rhsp >= 0; ++rhsp)
	rhs[i++] = *rhsp;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->number;
      /* Length of rule R's RHS. */
      r2[r] = i - prhs[r];
      /* Separator in RHS. */
      rhs[i++] = -1;
      /* Line where rule was defined. */
      rline[r] = rules[r].location.first_line;
      /* Dynamic precedence (GLR) */
      dprec[r] = rules[r].dprec;
      /* Merger-function index (GLR) */
      merger[r] = rules[r].merger;
    }
  assert (i == nritems);

  muscle_insert_item_number_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_insert_unsigned_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_insert_unsigned_int_table ("rline", rline, 0, 0, nrules);
  muscle_insert_symbol_number_table ("r1", r1, 0, 0, nrules);
  muscle_insert_unsigned_int_table ("r2", r2, 0, 0, nrules);
  muscle_insert_short_table ("dprec", dprec, 0, 0, nrules);
  muscle_insert_short_table ("merger", merger, 0, 0, nrules);

  MUSCLE_INSERT_INT ("rules_number", nrules);

  free (rhs);
  free (prhs);
  free (rline);
  free (r1);
  free (r2);
  free (dprec);
  free (merger);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

static void
prepare_states (void)
{
  state_number_t i;
  symbol_number_t *values =
    (symbol_number_t *) alloca (sizeof (symbol_number_t) * nstates);
  for (i = 0; i < nstates; ++i)
    values[i] = states[i]->accessing_symbol;
  muscle_insert_symbol_number_table ("stos", values,
				     0, 1, nstates);

  MUSCLE_INSERT_INT ("last", high);
  MUSCLE_INSERT_INT ("final_state_number", final_state->number);
  MUSCLE_INSERT_INT ("states_number", nstates);
}



/*---------------------------------.
| Output the user actions to OUT.  |
`---------------------------------*/

static void
user_actions_output (FILE *out)
{
  rule_number_t r;

  fputs ("m4_define([b4_actions], \n[[", out);
  for (r = 0; r < nrules; ++r)
    if (rules[r].action)
      {
	fprintf (out, "  case %d:\n", r + 1);

	if (!no_lines_flag)
	  fprintf (out, muscle_find ("linef"),
		   rules[r].action_location.first_line,
		   quotearg_style (c_quoting_style,
				   muscle_find ("filename")));
	fprintf (out, "    %s\n    break;\n\n",
		 rules[r].action);
      }
  fputs ("]])\n\n", out);
}

/*--------------------------------------.
| Output the merge functions to OUT.   |
`--------------------------------------*/

static void
merger_output (FILE *out)
{
  int n;
  merger_list* p;

  fputs ("m4_define([b4_mergers], \n[[", out);
  for (n = 1, p = merge_functions; p != NULL; n += 1, p = p->next)
    {
      if (p->type[0] == '\0')
	fprintf (out, "  case %d: yyval = %s (*yy0, *yy1); break;\n",
		 n, p->name);
      else
	fprintf (out, "  case %d: yyval.%s = %s (*yy0, *yy1); break;\n",
		 n, p->type, p->name);
    }
  fputs ("]])\n\n", out);
}

/*--------------------------------------.
| Output the tokens definition to OUT.  |
`--------------------------------------*/

static void
token_definitions_output (FILE *out)
{
  int i;
  int first = 1;

  fputs ("m4_define([b4_tokens], \n[", out);
  for (i = 0; i < ntokens; ++i)
    {
      symbol_t *symbol = symbols[i];
      int number = symbol->user_token_number;

      /* At this stage, if there are literal aliases, they are part of
	 SYMBOLS, so we should not find symbols which are the aliases
	 here.  */
      assert (number != USER_NUMBER_ALIAS);

      /* Skip error token.  */
      if (symbol == errtoken)
	continue;

      /* If this string has an alias, then it is necessarily the alias
	 which is to be output.  */
      if (symbol->alias)
	symbol = symbol->alias;

      /* Don't output literal chars or strings (when defined only as a
	 string).  Note that must be done after the alias resolution:
	 think about `%token 'f' "f"'.  */
      if (symbol->tag[0] == '\'' || symbol->tag[0] == '\"')
	continue;

      /* Don't #define nonliteral tokens whose names contain periods
	 or '$' (as does the default value of the EOF token).  */
      if (strchr (symbol->tag, '.') || strchr (symbol->tag, '$'))
	continue;

      fprintf (out, "%s[[[%s]], [%d]]",
	       first ? "" : ",\n", symbol->tag, number);

      first = 0;
    }
  fputs ("])\n\n", out);
}


/*---------------------------------------.
| Output the symbol destructors to OUT.  |
`---------------------------------------*/

static void
symbol_destructors_output (FILE *out)
{
  int i;
  int first = 1;

  fputs ("m4_define([b4_symbol_destructors], \n[", out);
  for (i = 0; i < nsyms; ++i)
    if (symbols[i]->destructor)
      {
	symbol_t *symbol = symbols[i];

	/* Filename, lineno,
	   Symbol-name, Symbol-number,
	   destructor, typename. */
	fprintf (out, "%s[[[%s]], [[%d]], [[%s]], [[%d]], [[%s]], [[%s]]]",
		 first ? "" : ",\n",
		 infile, symbol->destructor_location.first_line,
		 symbol->tag,
		 symbol->number,
		 symbol->destructor,
		 symbol->type_name);

	first = 0;
      }
  fputs ("])\n\n", out);
}


/*------------------------------------.
| Output the symbol printers to OUT.  |
`------------------------------------*/

static void
symbol_printers_output (FILE *out)
{
  int i;
  int first = 1;

  fputs ("m4_define([b4_symbol_printers], \n[", out);
  for (i = 0; i < nsyms; ++i)
    if (symbols[i]->destructor)
      {
	symbol_t *symbol = symbols[i];

	/* Filename, lineno,
	   Symbol-name, Symbol-number,
	   printer, typename. */
	fprintf (out, "%s[[[%s]], [[%d]], [[%s]], [[%d]], [[%s]], [[%s]]]",
		 first ? "" : ",\n",
		 infile, symbol->printer_location.first_line,
		 symbol->tag,
		 symbol->number,
		 symbol->printer,
		 symbol->type_name);

	first = 0;
      }
  fputs ("])\n\n", out);
}


static void
prepare_actions (void)
{
  /* Figure out the actions for the specified state, indexed by
     lookahead token type.  */

  muscle_insert_rule_number_table ("defact", yydefact,
				   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_insert_state_number_table ("defgoto", yydefgoto,
				    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_insert_base_table ("pact", base,
			     base[0], 1, nstates);
  MUSCLE_INSERT_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_insert_base_table ("pgoto", base,
			     base[nstates], nstates + 1, nvectors);

  muscle_insert_base_table ("table", table,
			    table[0], 1, high + 1);
  MUSCLE_INSERT_INT ("table_ninf", table_ninf);

  muscle_insert_base_table ("check", check,
			    check[0], 1, high + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_insert_unsigned_int_table ("conflict_list_heads", conflict_table,
				    conflict_table[0], 1, high + 1);
  muscle_insert_unsigned_int_table ("conflicting_rules", conflict_list,
				    conflict_list[0], 1, conflict_list_cnt);
}


/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

static void
output_skeleton (void)
{
  FILE *in;
  FILE *out;
  int filter_fd[2];
  char const *argv[7];
  pid_t pid;

  /* Compute the names of the package data dir and skeleton file.
     Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  char const m4sugar[] = "m4sugar/m4sugar.m4";
  char *full_path;
  char const *p;
  char const *m4 = (p = getenv ("M4")) ? p : M4;
  char const *pkgdatadir = (p = getenv ("BISON_PKGDATADIR")) ? p : PKGDATADIR;
  size_t skeleton_size = strlen (skeleton) + 1;
  size_t pkgdatadirlen = strlen (pkgdatadir);
  while (pkgdatadirlen && pkgdatadir[pkgdatadirlen - 1] == '/')
    pkgdatadirlen--;
  full_path = xmalloc (pkgdatadirlen + 1
		       + (skeleton_size < sizeof m4sugar
			  ? sizeof m4sugar : skeleton_size));
  strcpy (full_path, pkgdatadir);
  full_path[pkgdatadirlen] = '/';
  strcpy (full_path + pkgdatadirlen + 1, m4sugar);
  in = fopen (full_path, "r");
  if (! in)
    error (EXIT_FAILURE, errno, "%s", full_path);
  xfclose (in);
  strcpy (full_path + pkgdatadirlen + 1, skeleton);

  /* Create an m4 subprocess connected to us via two pipes.  */

  if (trace_flag & trace_tools)
    fprintf (stderr, "running: %s -I %s %s - %s\n",
	     m4, pkgdatadir, m4sugar, full_path);

  argv[0] = m4;
  argv[1] = "-I";
  argv[2] = pkgdatadir;
  argv[3] = m4sugar;
  argv[4] = "-";
  argv[5] = full_path;
  argv[6] = NULL;

  init_subpipe ();
  pid = create_subpipe (argv, filter_fd);
  free (full_path);

  out = fdopen (filter_fd[0], "w");
  if (! out)
    error (EXIT_FAILURE, errno, "fdopen");

  /* Output the definitions of all the muscles.  */
  fputs ("m4_init()\n", out);

  user_actions_output (out);
  merger_output (out);
  token_definitions_output (out);
  symbol_destructors_output (out);
  symbol_printers_output (out);

  muscles_m4_output (out);

  fputs ("m4_wrap([m4_divert_pop(0)])\n", out);
  fputs ("m4_divert_push(0)dnl\n", out);
  xfclose (out);

  /* Read and process m4's output.  */
  timevar_push (TV_M4);
  in = fdopen (filter_fd[1], "r");
  if (! in)
    error (EXIT_FAILURE, errno, "fdopen");
  scan_skel (in);
  xfclose (in);
  reap_subpipe (pid, m4);
  timevar_pop (TV_M4);
}

static void
prepare (void)
{
  /* Flags. */
  MUSCLE_INSERT_INT ("locations_flag", locations_flag);
  MUSCLE_INSERT_INT ("defines_flag", defines_flag);
  MUSCLE_INSERT_INT ("error_verbose", error_verbose);
  MUSCLE_INSERT_INT ("pure", pure_parser);
  MUSCLE_INSERT_INT ("debug", debug_flag);

  /* FIXME: This is wrong: the muscles should decide whether they hold
     a copy or not, but the situation is too obscure currently.  */
  MUSCLE_INSERT_STRING ("prefix", spec_name_prefix ? spec_name_prefix : "yy");
  MUSCLE_INSERT_STRING ("output_infix", output_infix ? output_infix : "");
  MUSCLE_INSERT_STRING ("output_prefix", short_base_name);
  MUSCLE_INSERT_STRING ("output_parser_name", parser_file_name);
  MUSCLE_INSERT_STRING ("output_header_name", spec_defines_file);

  /* User Code.  */
  obstack_1grow (&pre_prologue_obstack, 0);
  obstack_1grow (&post_prologue_obstack, 0);
  muscle_insert ("pre_prologue", obstack_finish (&pre_prologue_obstack));
  muscle_insert ("post_prologue", obstack_finish (&post_prologue_obstack));

  /* Find the right skeleton file.  */
  if (!skeleton)
    {
      if (glr_parser)
	skeleton = "glr.c";
      else
	skeleton = "yacc.c";
    }

  /* Parse the skeleton file and output the needed parsers.  */
  MUSCLE_INSERT_STRING ("skeleton", skeleton);
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

void
output (void)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  obstack_free (&format_obstack, NULL);
  obstack_free (&pre_prologue_obstack, NULL);
  obstack_free (&post_prologue_obstack, NULL);
}
