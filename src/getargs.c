/* Parse command line arguments for bison.
   Copyright 1984, 1986, 1989, 1992, 2000, 2001, 2002
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
#include "getopt.h"
#include "argmatch.h"
#include "complain.h"
#include "getargs.h"
#include "options.h"
#include "files.h"

int debug_flag = 0;
int defines_flag = 0;
int locations_flag = 0;
int no_lines_flag = 0;
int no_parser_flag = 0;
int report_flag = 0;
int token_table_flag = 0;
int yacc_flag = 0;	/* for -y */
int graph_flag = 0;
int trace_flag = 0;

const char *skeleton = NULL;
const char *include = NULL;

extern char *program_name;

/*----------------------.
| --report's handling.  |
`----------------------*/

static const char * const report_args[] =
{
  /* In a series of synonyms, present the most meaningful first, so
     that argmatch_valid be more readable.  */
  "none",
  "state", "states",
  "itemset", "itemsets",
  "lookahead", "lookaheads",
  "solved",
  "all",
  0
};

static const int report_types[] =
{
  report_none,
  report_states, report_states,
  report_states | report_itemsets, report_states | report_itemsets,
  report_states | report_lookaheads, report_states | report_lookaheads,
  report_states | report_solved_conflicts,
  report_all
};


static void
report_argmatch (char *args)
{
  ARGMATCH_ASSERT (report_args, report_types);
  do
    {
      int report = XARGMATCH ("--report", args,
			      report_args, report_types);
      if (report == report_none)
	report_flag = report_none;
      else
	report_flag |= report;
    }
  while ((args = strtok (NULL, ",")));
}

/*---------------------------.
| Display the help message.  |
`---------------------------*/

static void
usage (FILE *stream)
{
  /* Some efforts were made to ease the translators' task, please
     continue.  */
  fputs (_("\
GNU bison generates parsers for LALR(1) grammars.\n"), stream);
  putc ('\n', stream);

  fprintf (stream, _("\
Usage: %s [OPTION]... FILE\n"), program_name);
  putc ('\n', stream);

  fputs (_("\
If a long option shows an argument as mandatory, then it is mandatory\n\
for the equivalent short option also.  Similarly for optional arguments.\n"),
	 stream);
  putc ('\n', stream);

  fputs (_("\
Operation modes:\n\
  -h, --help      display this help and exit\n\
  -V, --version   output version information and exit\n\
  -y, --yacc      emulate POSIX yacc\n"), stream);
  putc ('\n', stream);

  fputs (_("\
Parser:\n\
  -S, --skeleton=FILE        specify the skeleton to use\n\
  -t, --debug                instrument the parser for debugging\n\
      --locations            enable locations computation\n\
  -p, --name-prefix=PREFIX   prepend PREFIX to the external symbols\n\
  -l, --no-lines             don't generate `#line' directives\n\
  -n, --no-parser            generate the tables only\n\
  -k, --token-table          include a table of token names\n\
"), stream);
  putc ('\n', stream);

  fputs (_("\
Output:\n\
  -d, --defines              also produce a header file\n\
  -r, --report=THINGS        also produce details on the automaton\n\
  -v, --verbose              same as `--report=state'\n\
  -b, --file-prefix=PREFIX   specify a PREFIX for output files\n\
  -o, --output=FILE          leave output to FILE\n\
  -g, --graph                also produce a VCG description of the automaton\n\
\n\
THINGS is a list of comma separated words that can include:\n\
  `state'        describe the states\n\
  `itemset'      complete the core item sets with their closure\n\
  `lookahead'    explicitly associate lookaheads to items\n\
  `solved'       describe shift/reduce conflicts solving\n\
  `all'          include all the above information\n\
  `none'         disable the report\n\
"), stream);
  putc ('\n', stream);

  fputs (_("\
Report bugs to <bug-bison@gnu.org>.\n"), stream);
}


/*------------------------------.
| Display the version message.  |
`------------------------------*/

static void
version (FILE *stream)
{
  /* Some efforts were made to ease the translators' task, please
     continue.  */
  fprintf (stream, _("bison (GNU Bison) %s"), VERSION);
  putc ('\n', stream);
  fputs (_("Written by Robert Corbett and Richard Stallman.\n"), stream);
  putc ('\n', stream);

  fprintf (stream,
	   _("Copyright (C) %d Free Software Foundation, Inc.\n"), 2002);

  fputs (_("\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
"),
	 stream);
}


/*----------------------.
| Process the options.  |
`----------------------*/

/* Under DOS, there is no difference on the case.  This can be
   troublesome when looking for `.tab' etc.  */
#ifdef MSDOS
# define AS_FILE_NAME(File) (strlwr (File), (File))
#else
# define AS_FILE_NAME(File) (File)
#endif

void
getargs (int argc, char *argv[])
{
  int c;

  struct option *longopts = long_option_table_new ();
  while ((c = getopt_long (argc, argv, shortopts, longopts, NULL)) != EOF)
    switch (c)
      {
      case 0:
	/* Certain long options cause getopt_long to return 0.  */
	break;

      case 'y':
	yacc_flag = 1;
	break;

      case 'h':
	usage (stdout);
	exit (0);

      case 'V':
	version (stdout);
	exit (0);

      case 'g':
	/* Here, the -g and --graph=FILE options are differentiated.  */
	graph_flag = 1;
	spec_graph_file = AS_FILE_NAME (optarg);
	break;

      case 'v':
	report_flag |= report_states;
	break;

      case 'S':
	skeleton = AS_FILE_NAME (optarg);
	break;

      case 'I':
	include = AS_FILE_NAME (optarg);
	break;

      case 'd':
	/* Here, the -d and --defines options are differentiated.  */
	defines_flag = 1;
	if (optarg)
	  spec_defines_file = AS_FILE_NAME (optarg);
	break;

      case 'l':
	no_lines_flag = 1;
	break;

      case 'k':
	token_table_flag = 1;
	break;

      case 'n':
	no_parser_flag = 1;
	break;

      case 't':
	debug_flag = 1;
	break;

      case 'o':
	spec_outfile = AS_FILE_NAME (optarg);
	break;

      case 'b':
	spec_file_prefix = AS_FILE_NAME (optarg);
	break;

      case 'p':
	spec_name_prefix = optarg;
	break;

      case 'r':
	report_argmatch (optarg);
	break;

      default:
	fprintf (stderr, _("Try `%s --help' for more information.\n"),
		 program_name);
	exit (1);
      }

  free (longopts);

  if (optind == argc)
    {
      fprintf (stderr, _("%s: no grammar file given\n"), program_name);
      exit (1);
    }
  if (optind < argc - 1)
    fprintf (stderr, _("%s: extra arguments ignored after `%s'\n"),
	     program_name, argv[optind]);

  infile = argv[optind];
}
