/* Concentrate all options use in bison,
   Copyright 2001 Free Software Foundation, Inc.

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

#include <stdio.h>
#include "xalloc.h"
#include "system.h"
#include "getopt.h"
#include "getargs.h"
#include "gram.h"
#include "symtab.h"
#include "lex.h"
#include "options.h"

/* Shorts options.  */
const char *shortopts = "yvgdhrltknVo:b:p:S:";

/* Long options.  */
struct option *longopts = NULL;

struct percent_table_struct *percent_table = NULL;

const struct option_table_struct option_table[] =
{
  /*
   * Command line.
   */

  /* Operation modes. */
  {opt_cmd_line, "help",      no_argument,		   0,	      0,   'h'},
  {opt_cmd_line, "version",   no_argument,		   0,	      0,   'V'},

  /* Parser. */
  /* was 'a';  apparently unused -wjh */
  {opt_cmd_line, "name-prefix",	required_argument,	   0,	      0,   'p'},

  /* Output. */
  {opt_cmd_line, "file-prefix",	required_argument,	   0,	      0,   'b'},
  {opt_cmd_line, "output-file",	required_argument,	   0,	      0,   'o'},
  {opt_cmd_line, "graph",	optional_argument,	   0,	      0,   'g'},

  /* Hidden. */
  {opt_cmd_line, "statistics", no_argument, &statistics_flag,	      0,     1},

  /*
   * Percent declarations.
   */

  {opt_percent, "token",		0,		NULL, tok_token,     0},
  {opt_percent, "term",			0,		NULL, tok_token,     0},
  {opt_percent, "nterm",		0,		NULL, tok_nterm,     0},
  {opt_percent, "type",			0,		NULL, tok_type,      0},
  {opt_percent, "guard",		0,		NULL, tok_guard,     0},
  {opt_percent, "union",		0,		NULL, tok_union,     0},
  {opt_percent, "expect",		0,		NULL, tok_expect,    0},
  {opt_percent, "thong",		0,		NULL, tok_thong,     0},
  {opt_percent, "start",		0,		NULL, tok_start,     0},
  {opt_percent, "left",			0,		NULL, tok_left,      0},
  {opt_percent, "right",		0,		NULL, tok_right,     0},
  {opt_percent, "nonassoc",		0,		NULL, tok_nonassoc,  0},
  {opt_percent, "binary",		0,		NULL, tok_nonassoc,  0},
  {opt_percent, "prec",			0,		NULL, tok_prec,      0},
#if 0
  /* For the time being, this is not enabled yet, while it's possible
     though, since we use obstacks.  The only risk is with semantic
     parsers which will output an `include' of an output file: be sure
     that the naem included is indeed the name of the output file.  */
  /* FIXME Should we activate this options ?  */
  {opt_both, "output-file", required_argument, &spec_outfile, tok_setopt,  'o'},
  {opt_both, "file-prefix", required_argument,&spec_file_prefix,tok_setopt,'b'},
  {opt_both, "name-prefix", required_argument,&spec_name_prefix,tok_setopt,'p'},
#endif
  {opt_percent, "define",		0,		NULL, tok_define,    0},
  {opt_percent, "semantic-parser",	0,  &semantic_parser, tok_noop,	     0},
  {opt_percent, "pure-parser",		0,	&pure_parser, tok_noop,      0},

  /*
   * Percent and command line declarations.
   */

  /* FIXME Usually option use `-' to separe words on command line
     and `_' for percent option.  But here the separator are always
     the same, the char `-'.  */

  /* Output.  */
  {opt_both, "defines",	       optional_argument,   &defines_flag, tok_noop,	   'd'},
  {opt_both, "verbose",	       no_argument,   &verbose_flag, tok_noop,	   'v'},

  /* Operation modes.  */
  {opt_both, "fixed-output-files", no_argument,  &yacc_flag, tok_noop,	   'y'},
  {opt_both, "yacc",	       no_argument,      &yacc_flag, tok_noop,	   'y'},

  /* Parser.  */
  {opt_both, "debug",	       no_argument,     &debug_flag, tok_noop,	   't'},
  {opt_both, "locations",      no_argument, &locations_flag, tok_noop,       1},
  {opt_both, "no-lines",       no_argument,  &no_lines_flag, tok_noop,	   'l'},
  {opt_both, "no-parser",      no_argument, &no_parser_flag, tok_noop,	   'n'},
  {opt_both, "raw",	       no_argument,               0, tok_obsolete, 'r'},
  {opt_both, "skeleton",       required_argument,         0, tok_skel,	   'S'},
  {opt_both, "token-table",    no_argument, &token_table_flag, tok_noop,   'k'},

  {0, 0, 0, 0, 0, 0}
};

/*--------------------------------------------------------.
| Create the longoptions structure from the option_table, |
| for the getopt file.					  |
`--------------------------------------------------------*/
void
create_long_option_table ()
{
  int i = 0;
  int j = 0;
  int number_options;

  for (number_options = 0; option_table[i].name; i++)
    if (option_table[i].access == opt_cmd_line
	|| option_table[i].access == opt_both)
      ++number_options;

  longopts = XMALLOC (struct option, number_options + 1);
  for (i = 0; option_table[i].name; i++)
    if (option_table[i].access == opt_cmd_line
	|| option_table[i].access == opt_both)
      {
	/* Copy the struct information in the longoptions.  */
	longopts[j].name = option_table[i].name;
	longopts[j].has_arg = option_table[i].has_arg;
	longopts[j].flag = option_table[i].set_flag;
	longopts[j++].val = option_table[i].val;
      }
  longopts[number_options].name = NULL;
  longopts[number_options].has_arg = 0;
  longopts[number_options].flag = NULL;
  longopts[number_options].val = 0;
}
