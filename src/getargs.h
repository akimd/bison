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

#ifndef GETARGS_H_
# define GETARGS_H_

/* flags set by % directives */
extern const char *skeleton;		/* for -S */
extern const char *include;		/* for -I */

extern int debug_flag; 		/* for -t */
extern int defines_flag;    	/* for -d */
extern int locations_flag;
extern int no_lines_flag;    	/* for -l */
extern int no_parser_flag;	/* for -n */
extern int token_table_flag;   	/* for -k */
extern int graph_flag;		/* for -g */
extern int yacc_flag;  		/* for -y */

/* --trace.  */
enum trace_e
  {
    trace_none      = 0,
    trace_resource  = 1 << 0,
    trace_sets      = 1 << 1,
    trace_bitsets   = 1 << 2,
    trace_tools     = 1 << 3,
    trace_automaton = 1 << 4,
    trace_grammar   = 1 << 5,
    trace_time      = 1 << 6,
    trace_all       = ~0
  };
extern int trace_flag;

/* --report.  */
enum report_e
  {
    report_none             = 0,
    report_states           = 1 << 0,
    report_itemsets         = 1 << 1,
    report_lookaheads       = 1 << 2,
    report_solved_conflicts = 1 << 3,
    report_all              = ~0
  };
extern int report_flag;

void getargs PARAMS ((int argc, char *argv[]));

#endif /* !GETARGS_H_ */
