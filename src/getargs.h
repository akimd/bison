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

#ifndef GETARGS_H_
# define GETARGS_H_

/* flags set by % directives */
extern char *spec_file_prefix;	/* for -b */
extern char *spec_name_prefix; 	/* for -p */
extern const char *skeleton;

extern int debug_flag; 		/* for -t */
extern int defines_flag;    	/* for -d */
extern int locations_flag;
extern int no_lines_flag;    	/* for -l */
extern int no_parser_flag;	/* for -n */
extern int token_table_flag;   	/* for -k */
extern int verbose_flag;	/* for -v */
extern int graph_flag;		/* for -g */
extern int yacc_flag;  		/* for -y */
extern int trace_flag;

void getargs PARAMS ((int argc, char *argv[]));

#endif /* !GETARGS_H_ */
