/* Parse command line arguments for bison,
   Copyright (C) 1984, 1986, 1989 Free Software Foundation, Inc.

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
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */


#include <stdio.h>
#include "getopt.h"
#include "system.h"
#include "files.h"

int verboseflag;
int definesflag;
int debugflag;
int nolinesflag;
char *spec_name_prefix; /* for -p.  */
char *spec_file_prefix; /* for -b. */
extern int fixed_outfiles;/* for -y */

extern char *program_name;
extern char *version_string;

extern void fatal();

struct option longopts[] =
{
  {"debug", 0, &debugflag, 1},
  {"defines", 0, &definesflag, 1},
  {"file-prefix", 1, 0, 'b'},
  {"fixed-output-files", 0, &fixed_outfiles, 1},
  {"help", 0, 0, 'h'},
  {"name-prefix", 1, 0, 'a'},
  {"no-lines", 0, &nolinesflag, 1},
  {"output", 1, 0, 'o'},
  {"output-file", 1, 0, 'o'},
  {"verbose", 0, &verboseflag, 1},
  {"version", 0, 0, 'V'},
  {"yacc", 0, &fixed_outfiles, 1},
  {0, 0, 0, 0}
};

void
usage (stream)
     FILE *stream;
{
  fprintf (stream, "\
Usage: %s [-dhltvyV] [-b file-prefix] [-o outfile] [-p name-prefix]\n\
       [--debug] [--defines] [--fixed-output-files] [--no-lines]\n\
       [--verbose] [--version] [--help] [--yacc]\n\
       [--file-prefix=prefix] [--name-prefix=prefix]\n\
       [--output=outfile] grammar-file\n",
	   program_name);
}

void
getargs(argc, argv)
     int argc;
     char *argv[];
{
  register int c;

  verboseflag = 0;
  definesflag = 0;
  debugflag = 0;
  fixed_outfiles = 0;

  while ((c = getopt_long (argc, argv, "yvdhltVo:b:p:", longopts, (int *)0))
	 != EOF)
    {
      switch (c)
	{
	case 0:
	  /* Certain long options cause getopt_long to return 0.  */
	  break;

	case 'y':
	  fixed_outfiles = 1;
	  break;
	  
	case 'h':
	  usage (stdout);
	  exit (0);

	case 'V':
	  printf ("%s", version_string);
	  exit (0);
	  
	case 'v':
	  verboseflag = 1;
	  break;
	  
	case 'd':
	  definesflag = 1;
	  break;
	  
	case 'l':
	  nolinesflag = 1;
	  break;
	  
	case 't':
	  debugflag = 1;
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
      fprintf(stderr, "%s: no grammar file given\n", program_name);
      exit(1);
    }
  if (optind < argc - 1)
    fprintf(stderr, "%s: warning: extra arguments ignored\n", program_name);

  infile = argv[optind];
}
