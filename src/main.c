/* Top level entry point of bison,
   Copyright (C) 1984, 1986, 1989, 1992, 1995, 2000, 2001, 2002
   Free Software Foundation, Inc.

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
#include "bitset_stats.h"
#include "bitset.h"
#include "getargs.h"
#include "symtab.h"
#include "gram.h"
#include "files.h"
#include "complain.h"
#include "derives.h"
#include "tables.h"
#include "output.h"
#include "reader.h"
#include "lalr.h"
#include "reduce.h"
#include "nullable.h"
#include "print.h"
#include "LR0.h"
#include "conflicts.h"
#include "print_graph.h"
#include "muscle_tab.h"

/* The name this program was run with, for messages.  */
char *program_name;



int
main (int argc, char *argv[])
{
  program_name = argv[0];
  setlocale (LC_ALL, "");
  (void) bindtextdomain (PACKAGE, LOCALEDIR);
  (void) textdomain (PACKAGE);

  getargs (argc, argv);

  time_report = trace_flag & trace_time;
  init_timevar ();
  timevar_start (TV_TOTAL);

  if (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  muscle_init ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_push (TV_READER);
  reader ();
  timevar_pop (TV_READER);

  if (complain_message_count)
    exit (1);

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_push (TV_REDUCE);
  reduce_grammar ();
  timevar_pop (TV_REDUCE);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_push (TV_SETS);
  derives_compute ();
  nullable_compute ();
  timevar_pop (TV_SETS);

  /* Convert to nondeterministic finite state machine.  In file LR0.
     See state.h for more info.  */
  timevar_push (TV_LR0);
  generate_states ();
  timevar_pop (TV_LR0);

  /* make it deterministic.  In file lalr.  */
  timevar_push (TV_LALR);
  lalr ();
  timevar_pop (TV_LALR);

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_push (TV_CONFLICTS);
  conflicts_solve ();
  conflicts_print ();
  timevar_pop (TV_CONFLICTS);

  /* Compute the parser tables.  */
  timevar_push (TV_ACTIONS);
  tables_generate ();
  timevar_pop (TV_ACTIONS);

  grammar_rules_never_reduced_report
    (_("rule never reduced because of conflicts"));

  /* Output file names. */
  compute_output_file_names ();

  /* Output the detailed report on the grammar.  */
  if (report_flag)
    {
      timevar_push (TV_REPORT);
      print_results ();
      timevar_pop (TV_REPORT);
    }

  /* Output the VCG graph.  */
  if (graph_flag)
    {
      timevar_push (TV_GRAPH);
      print_graph ();
      timevar_pop (TV_GRAPH);
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  if (complain_message_count)
    exit (1);

  /* Lookaheads are no longer needed. */
  timevar_push (TV_FREE);
  lalr_free ();
  timevar_pop (TV_FREE);

  /* Output the tables and the parser to ftable.  In file output.  */
  timevar_push (TV_PARSER);
  output ();
  timevar_pop (TV_PARSER);

  timevar_push (TV_FREE);
  nullable_free ();
  derives_free ();
  tables_free ();
  states_free ();
  reduce_free ();
  conflicts_free ();
  grammar_free ();

  /* The scanner memory cannot be released right after parsing, as it
     contains things such as user actions, prologue, epilogue etc.  */
  scanner_free ();
  muscle_free ();
  /* If using alloca.c, flush the alloca'ed memory for the benefit of
     people running Bison as a library in IDEs.  */
#if C_ALLOCA
  alloca (0);
#endif
  timevar_pop (TV_FREE);

  if (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (TV_TOTAL);
  timevar_print (stderr);

  return complain_message_count ? EXIT_FAILURE : EXIT_SUCCESS;
}
