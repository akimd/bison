/* Parse command line arguments for bison.
   Copyright (C) 1984, 1986, 1989, 1992, 2000 Free Software Foundation, Inc.

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

#include <stdio.h>
#include "getopt.h"
#include "system.h"
#include "files.h"
#include "getargs.h"

char *spec_file_prefix; /* for -b. */
char *spec_name_prefix; /* for -p.  */

int debug_flag = 0;
int defines_flag = 0;
int locations_flag = 0;
int no_lines_flag = 0;
int no_parser_flag = 0;
int raw_flag = 0;
int token_table_flag = 0;
int verbose_flag = 0;
int statistics_flag = 0;
int yacc_flag = 0;	/* for -y */

extern char *program_name;

static struct option longopts[] =
{
  {"debug", 0, &debug_flag, 1},
  {"defines", 0, &defines_flag, 1},
  {"file-prefix", 1, 0, 'b'},
  {"fixed-output-files", 0, &yacc_flag, 1},
  {"help", 0, 0, 'h'},
  {"name-prefix", 1, 0, 'p'}, /* was 'a';  apparently unused -wjh */
  {"locations", 0, &locations_flag, 1},
  {"no-lines", 0, &no_lines_flag, 1},
  {"no-parser", 0, &no_parser_flag, 1},
  {"output", 1, 0, 'o'},
  {"output-file", 1, 0, 'o'},
  {"raw", 0, &raw_flag, 1},
  {"statistics", 0, &statistics_flag, 1},
  {"token-table", 0, &token_table_flag, 1},
  {"verbose", 0, &verbose_flag, 1},
  {"version", 0, 0, 'V'},
  {"yacc", 0, &yacc_flag, 1},
  {0, 0, 0, 0}
};

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
  -t, --debug                instrument the parser for debugging\n\
      --locations            enable locations computation\n\
  -p, --name-prefix=PREFIX   prepend PREFIX to the external symbols\n\
  -l, --no-lines             don't generate `#line' directives\n\
  -n, --no-parser            generate the tables only\n\
  -r, --raw                  number the tokens from 3\n\
  -k, --token-table          include a table of token names\n\
"), stream);
  putc ('\n', stream);

  fputs (_("\
Output:\n\
  -d, --defines              also produce a header file\n\
  -v, --verbose              also produce an explanation of the automaton\n\
  -b, --file-prefix=PREFIX   specify a PREFIX for output files\n\
  -o, --output-file=FILE     leave output to FILE\n"), stream);
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
  fprintf (stream, _("\
bison (GNU Bison) %s"), VERSION);
  putc ('\n', stream);
  putc ('\n', stream);

  fputs (_("\
Copyright (C) 1984, 1986, 1989, 1992, 2000 Free Software Foundation, Inc.\n"),
	 stream);

  fputs (_("\
This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
"),
	 stream);
}


/*----------------------.
| Process the options.  |
`----------------------*/

void
getargs (int argc, char *argv[])
{
  int c;

  while ((c = getopt_long (argc, argv, "yvdhrltknVo:b:p:", longopts, (int *)0))
	 != EOF)
    {
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

	case 'v':
	  verbose_flag = 1;
	  break;

	case 'd':
	  defines_flag = 1;
	  break;

	case 'l':
	  no_lines_flag = 1;
	  break;

	case 'k':
	  token_table_flag = 1;
	  break;

	case 'r':
	  raw_flag = 1;
	  break;

	case 'n':
	  no_parser_flag = 1;
	  break;

	case 't':
	  debug_flag = 1;
	  break;

	case 'o':
	  spec_outfile = optarg;
	  break;

	case 'b':
	  spec_file_prefix = optarg;
	  break;

	case 'p':
	  spec_name_prefix = optarg;
	  break;

	default:
	  usage (stderr);
	  exit (1);
	}
    }

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
