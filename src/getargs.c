/* Parse command line arguments for bison.
   Copyright 1984, 1986, 1989, 1992, 2000, 2001 Free Software Foundation, Inc.

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
#include "complain.h"
#include "getargs.h"

int debug_flag = 0;
int defines_flag = 0;
int locations_flag = 0;
int no_lines_flag = 0;
int no_parser_flag = 0;
int token_table_flag = 0;
int verbose_flag = 0;
int yacc_flag = 0;	/* for -y */
int graph_flag = 0;
int trace_flag = 0;

const char *skeleton = NULL;

extern char *program_name;
const char *shortopts = "yvgdhrltknVo:b:p:S:";
static struct option longopts[] =
{
  /* Operation modes. */
  {"help",		no_argument,	0, 'h'},
  {"version",		no_argument, 	0, 'V'},
  {"yacc",		no_argument,	0, 'y'},
  {"fixed-output-files",no_argument,	0, 'y'},

  /* Parser. */
  {"skeleton",		required_argument,	0, 'S'},
  {"debug",		no_argument,		0, 't'},
  {"locations",		no_argument, 	&locations_flag, 1},
  /* was 'a';  apparently unused -wjh */
  {"name-prefix",	required_argument, 	0, 'p'},
  {"no-lines",		no_argument,		0, 'l'},
  {"no-parser",		no_argument,		0, 'n'},
  {"raw",		no_argument,		0, 'r'},
  {"token-table",	no_argument,		0, 'k'},

  /* Output. */
  {"defines",		optional_argument,     	0, 'd'},
  {"verbose",		no_argument,		0, 'v'},
  {"file-prefix",	required_argument, 	0, 'b'},
  {"output",		required_argument, 	0, 'o'},
  {"output-file",	required_argument, 	0, 'o'},
  {"graph",		optional_argument,     	0, 'g'},

  /* Hidden. */
  {"trace", 		no_argument, 	&trace_flag, 1},
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
  -v, --verbose              also produce an explanation of the automaton\n\
  -b, --file-prefix=PREFIX   specify a PREFIX for output files\n\
  -o, --output=FILE          leave output to FILE\n\
  -g, --graph                also produce a VCG description of the automaton\n\
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
  fprintf (stream, _("\
bison (GNU Bison) %s"), VERSION);
  putc ('\n', stream);
  putc ('\n', stream);

  fputs (_("\
Copyright 1984, 1986, 1989, 1992, 2000, 2001, 2002\n\
Free Software Foundation, Inc.\n"),
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

/* Under plain DOS, there is no difference on the case.  This can be
   troublesome when looking for `.tab' etc.  */
#ifdef MSDOS
# if defined (__DJGPP__)
/* Windows 9X and successors are case sensitive. */
#  define STRLWR(String)     ((String) ? (strlwr (String), (String)) : (String))
#  define AS_FILE_NAME(File) ((pathconf ((File), _PC_NAME_MAX) > 12) ? (File) : STRLWR (File))
# else
#  define AS_FILE_NAME(File) (strlwr (File), (File))
# endif
#else
# define AS_FILE_NAME(File) (File)
#endif

void
getargs (int argc, char *argv[])
{
  int c;

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
	verbose_flag = 1;
	break;

      case 'S':
	skeleton = AS_FILE_NAME (optarg);
	break;

      case 'd':
	/* Here, the -d and --defines options are differentiated.  */
	defines_flag = 1;
	spec_defines_file = AS_FILE_NAME (optarg);
	break;

      case 'l':
	no_lines_flag = 1;
	break;

      case 'k':
	token_table_flag = 1;
	break;

      case 'r':
	fatal (_("`%s' is no longer supported"), "--raw");
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

      default:
	fprintf (stderr, _("Try `%s --help' for more information.\n"),
		 program_name);
	exit (1);
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
