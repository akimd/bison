/* Concentrate all options use in bison,
   Copyright 2001, 2002 Free Software Foundation, Inc.

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
#include "getopt.h"
#include "files.h"
#include "getargs.h"
#include "symtab.h"
#include "gram.h"
#include "lex.h"
#include "output.h"
#include "options.h"

/* Shorts options.  */
const char *shortopts = "yvegdhr:ltknVo:b:p:S:";

/* A CLI option only.
   Arguments is the policy: `no', `optional', `required'.
   OptionChar is the value given to the Var if the latter is specified.  */
#undef OPTN
#define OPTN(OptionString, Arguments, Var, Token, OptionChar) \
  { opt_cmd_line, \
    (OptionString), (Arguments##_argument), (Var), (Token), (OptionChar) },

/* A directive only. */
#undef DRTV
#define DRTV(DirectiveString, Arguments, Var, Token) \
  { opt_percent, \
    (DirectiveString), (Arguments##_argument), (Var), (Token), (0) },

/* An option activated both by a directive and an CLI option. */
#undef BOTH
#define BOTH(String, Arguments, Var, Token, OptionChar) \
  { opt_both, \
    (String), (Arguments##_argument), (Var), (Token), (OptionChar) },


const struct option_table_s option_table[] =
{
  /*
   * Command line.
   */

  /* Operation modes. */
  OPTN ("help",          no,		0,	0,   'h')
  OPTN ("version",       no,		0,	0,   'V')

  /* Parser. */
  OPTN ("name-prefix",	required,	0,	0,   'p')
  OPTN ("include",      required,       0,      0,   'I')

  /* Output. */
  OPTN ("file-prefix",	required,	0,	0,   'b')
  OPTN ("output",	required,	0,	0,   'o')
  OPTN ("output-file",	required,	0,	0,   'o')
  OPTN ("graph",	optional,	0,	0,   'g')
  OPTN ("report",	required,   	0,	0,   'r')
  OPTN ("verbose",	     no,	0,    	0,   'v')

  /* Hidden. */
  OPTN ("trace",        no,   &trace_flag,      0,     1)

  /*
   * Percent declarations.
   */

  DRTV ("token",	no,		NULL, tok_token)
  DRTV ("term",		no,		NULL, tok_token)
  DRTV ("nterm",	no,		NULL, tok_nterm)
  DRTV ("type",		no,		NULL, tok_type)
  DRTV ("union",	no,		NULL, tok_union)
  DRTV ("expect",	no,		NULL, tok_expect)
  DRTV ("thong",	no,		NULL, tok_thong)
  DRTV ("start",	no,		NULL, tok_start)
  DRTV ("left",		no,		NULL, tok_left)
  DRTV ("right",	no,		NULL, tok_right)
  DRTV ("nonassoc",	no,		NULL, tok_nonassoc)
  DRTV ("binary",	no,		NULL, tok_nonassoc)
  DRTV ("prec",		no,		NULL, tok_prec)
  DRTV ("verbose",	no,     &report_flag, tok_intopt)
  DRTV ("error-verbose",no,   &error_verbose, tok_intopt)

  /* FIXME: semantic parsers will output an `include' of an
     output file: be sure that the naem included is indeed the name of
     the output file.  */ /* FIXME Should we activate this options ?
     */
  BOTH ("output",      required,     &spec_outfile, tok_stropt, 'o')
  BOTH ("file-prefix", required, &spec_file_prefix, tok_stropt, 'b')
  BOTH ("name-prefix", required, &spec_name_prefix, tok_stropt, 'p')

  DRTV ("define",	no,	 	  NULL, tok_define)
  DRTV ("pure-parser",	no,	  &pure_parser, tok_intopt)

  /*
   * Percent and command line declarations.
   */

  /* Output.  */
  BOTH ("defines",     optional,   &defines_flag,    tok_intopt,   'd')

  /* Operation modes.  */
  BOTH ("fixed-output-files", no,  &yacc_flag,	     tok_intopt,   'y')
  BOTH ("yacc",	              no,  &yacc_flag,	     tok_intopt,   'y')

  /* Parser.  */
  BOTH ("debug",	  no,     &debug_flag,	     tok_intopt,   't')
  BOTH ("locations",      no, &locations_flag,       tok_intopt,     1)
  BOTH ("no-lines",       no,  &no_lines_flag,       tok_intopt,   'l')
  BOTH ("no-parser",      no, &no_parser_flag,       tok_intopt,   'n')
  BOTH ("raw",	          no,               0,       tok_obsolete,   0)
  BOTH ("skeleton",       required,         0,       tok_skel,	   'S')
  BOTH ("token-table",    no, &token_table_flag,     tok_intopt,   'k')

  {0, 0, 0, 0, 0, 0}
};


/*--------------------------------------------------------.
| Create the longoptions structure from the option_table, |
| for the getopt file.					  |
`--------------------------------------------------------*/

struct option *
long_option_table_new ()
{
  struct option *res = NULL;
  int i = 0;
  int j = 0;
  int number_options;

  for (number_options = 0; option_table[i].name; i++)
    if (option_table[i].access == opt_cmd_line
	|| option_table[i].access == opt_both)
      ++number_options;

  res = XMALLOC (struct option, number_options + 1);
  for (i = 0; option_table[i].name; i++)
    if (option_table[i].access == opt_cmd_line
	|| option_table[i].access == opt_both)
      {
	res[j].name = option_table[i].name;
	res[j].has_arg = option_table[i].has_arg;
	/* When a getopt_long option has an associated variable
	   (member FLAG), then it is set of the VAL member value.  In
	   other words, we cannot expect getopt_long to store the
	   argument if we also want a short option.  */
	if (res[j].has_arg == optional_argument)
	  res[j].flag = NULL;
	else
	  res[j].flag = option_table[i].flag;
	res[j++].val = option_table[i].val;
      }
  res[number_options].name = NULL;
  res[number_options].has_arg = 0;
  res[number_options].flag = NULL;
  res[number_options].val = 0;

  return res;
}
