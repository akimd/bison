/* Top level entry point of bison,
   Copyright 1984, 1986, 1989, 1992, 1995, 2000, 2001, 2002
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
#include <malloc.h>
#include <sys/times.h>

/* The name this program was run with, for messages.  */
char *program_name;

static void
stage (const char *title)
{
  struct mallinfo minfo = mallinfo ();
  struct tms tinfo;
  times (&tinfo);
  fprintf (stderr, "STAGE: %30s: %9d (%9d): %ldu %lds\n",
	   title,
	   minfo.uordblks, minfo.arena,
	   tinfo.tms_utime, tinfo.tms_stime);
}

int
main (int argc, char *argv[])
{
  program_name = argv[0];
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  getargs (argc, argv);

  if (trace_flag)
    bitset_stats_enable ();

  muscle_init ();

  stage ("initialized muscles");

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */
  reader ();

  stage ("reader");

  if (complain_message_count)
    exit (1);

  /* Find useless nonterminals and productions and reduce the grammar. */
  reduce_grammar ();

  stage ("reduced grammar");

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  set_derives ();
  set_nullable ();

  /* Convert to nondeterministic finite state machine.  In file LR0.
     See state.h for more info.  */
  generate_states ();

  stage ("generated states");
  /* make it deterministic.  In file lalr.  */
  lalr ();

  stage ("lalred");
  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  conflicts_solve ();
  conflicts_print ();

  stage ("solved conflicts");
  /* Output file names. */
  compute_output_file_names ();

  /* Output the detailed report on the grammar.  */
  if (report_flag)
    print_results ();

  stage ("printed results");
  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  if (complain_message_count)
    exit (1);

  /* Output the VCG graph.  */
  if (graph_flag)
    print_graph ();

  /* Output the tables and the parser to ftable.  In file output.  */
  output ();
  stage ("made output");

  states_free ();
  stage ("freed states");
  reduce_free ();
  stage ("freed reduce");
  conflicts_free ();
  stage ("freed conflicts");
  free_nullable ();
  stage ("freed nullable");
  free_derives ();
  stage ("freed derives");
  grammar_free ();
  stage ("freed grammar");

  /* The scanner memory cannot be released right after parsing, as it
     contains things such as user actions, prologue, epilogue etc.  */
  scanner_free ();
  stage ("freed scanner");
  muscle_free ();
  stage ("freed muscles");
  /* If using alloca.c, flush the alloca'ed memory for the benefit of
     people running Bison as a library in IDEs.  */
#if C_ALLOCA
  alloca (0);
#endif

  if (trace_flag)
    bitset_stats_dump (stderr);

  return complain_message_count ? EXIT_FAILURE : EXIT_SUCCESS;
}
