#ifndef FILES_H_
# define FILES_H_
/* File names and variables for bison,
   Copyright 1984, 1989, 2000 Free Software Foundation, Inc.

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

/* These two should be pathnames for opening the sample parser files.
   When bison is installed, they should be absolute pathnames.
   XPFILE1 and XPFILE2 normally come from config.h.  */

/* File name specified with -o for the output file, or 0 if no -o.  */
extern char *spec_outfile;

/* For -a. */
extern char *spec_name_prefix;

/* File name pfx specified with -b, or 0 if no -b.  */
extern char *spec_file_prefix;

/* --verbose. */
extern char *spec_verbose_file;

/* File name specified for the output VCG graph.  */
extern char *spec_graph_file;

/* File name specified with --defines.  */
extern char *spec_defines_file;

/* Read grammar specifications. */
extern FILE *finput;


/* Output all the action code; precise form depends on which parser. */
extern struct obstack action_obstack;

/* Output the tables and the parser and also contains all the %{
   ... %} definitions.  */
extern struct obstack table_obstack;

/* optionally output #define's for token numbers. */
extern struct obstack defines_obstack;

/* If semantic parser, output a .h file that defines YYSTYPE... */
extern struct obstack attrs_obstack;

/* ... and output yyguard, containing all the guard code. */
extern struct obstack guard_obstack;

/* The verbose output. */
extern struct obstack output_obstack;

extern char *infile;
extern char *attrsfile;

void open_files PARAMS((void));
void close_files PARAMS((void));

void compute_output_file_names PARAMS((void));
void output_files PARAMS((void));

FILE *xfopen PARAMS ((const char *name, const char *mode));
int xfclose PARAMS ((FILE *ptr));

const char *skeleton_find PARAMS ((const char *envvar,
				   const char *skeleton_name));
#endif /* !FILES_H_ */
