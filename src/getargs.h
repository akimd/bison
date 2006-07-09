/* Parse command line arguments for bison.
   Copyright (C) 1984, 1986, 1989, 1992, 2000, 2001, 2002, 2003, 2004, 2005, 2006
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
   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifndef GETARGS_H_
# define GETARGS_H_

/* flags set by % directives */

/* for -S */
extern char const *skeleton;

/* for -I */
extern char const *include;

extern bool debug_flag;			/* for -t */
extern bool defines_flag;		/* for -d */
extern bool graph_flag;			/* for -g */
extern bool locations_flag;
extern bool no_lines_flag;		/* for -l */
extern bool no_parser_flag;		/* for -n */
extern bool token_table_flag;		/* for -k */
extern bool yacc_flag;			/* for -y */

extern bool error_verbose;


/* GLR_PARSER is true if the input file says to use the GLR
   (Generalized LR) parser, and to output some additional information
   used by the GLR algorithm.  */

extern bool glr_parser;

/* PURE_PARSER is true if should generate a parser that is all pure
   and reentrant.  */

extern bool pure_parser;

/* NONDETERMINISTIC_PARSER is true iff conflicts are accepted.  This
   is used by the GLR parser, and might be used in BackTracking
   parsers too.  */

extern bool nondeterministic_parser;


/*-----------.
| --report.  |
`-----------*/

enum report
  {
    report_none             = 0,
    report_states           = 1 << 0,
    report_itemsets         = 1 << 1,
    report_lookahead_tokens = 1 << 2,
    report_solved_conflicts = 1 << 3,
    report_all              = ~0
  };
/** What appears in the *.output file.  */
extern int report_flag;

/*----------.
| --trace.  |
`----------*/
enum trace
  {
    trace_none      = 0,       /**< No traces. */
    trace_scan      = 1 << 0,  /**< Grammar scanner traces. */
    trace_parse     = 1 << 1,  /**< Grammar parser traces. */
    trace_resource  = 1 << 2,  /**< Memory allocation. */
    trace_sets      = 1 << 3,  /**< Grammar sets: firsts, nullable etc. */
    trace_bitsets   = 1 << 4,  /**< Use of bitsets. */
    trace_tools     = 1 << 5,  /**< m4 invocation. */
    trace_automaton = 1 << 6,  /**< Construction of the automaton. */
    trace_grammar   = 1 << 7,  /**< Reading, reducing the grammar. */
    trace_time      = 1 << 8,  /**< Time consumption. */
    trace_skeleton  = 1 << 9,  /**< Skeleton postprocessing. */
    trace_m4        = 1 << 10, /**< M4 traces. */
    trace_all       = ~0       /**< All of the above.  */
  };
/** What debug items bison displays during its run.  */
extern int trace_flag;

/*-------------.
| --warnings.  |
`-------------*/

enum warnings
  {
    warnings_none             = 0,      /**< Issue no warnings.  */
    warnings_error            = 1 << 0, /**< Warnings are treated as errors.  */
    warnings_yacc             = 1 << 1, /**< POSIXME.  */
    warnings_all              = ~0      /**< All of the above.  */
  };
/** What warnings are issued.  */
extern int warnings_flag;


/** Process the command line arguments.
 *
 *  \param argc   size of \a argv
 *  \param argv   list of arguments.
 */
void getargs (int argc, char *argv[]);

#endif /* !GETARGS_H_ */
