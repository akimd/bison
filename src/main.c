/* Top level entry point of bison,
   Copyright 1984, 1986, 1989, 1992, 1995, 2000, 2001
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
#include "getargs.h"
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
#include "symtab.h"
#include "lex.h"

/* The name this program was run with, for messages.  */
char *program_name;


int
main (int argc, char *argv[])
{
  program_name = argv[0];
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  lineno = 0;
  getargs (argc, argv);

  open_files ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */
  reader ();
  if (complain_message_count)
    exit (1);

  /* Find useless nonterminals and productions and reduce the grammar. */
  reduce_grammar ();

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  set_derives ();
  set_nullable ();

  /* Convert to nondeterministic finite state machine.  In file LR0.
     See state.h for more info.  */
  generate_states ();

  /* make it deterministic.  In file lalr.  */
  lalr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  solve_conflicts ();
  conflicts_print ();

  /* Output file names. */
  compute_output_file_names ();

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  if (complain_message_count)
    exit (1);

  /* Output the detailed report on the grammar.  */
  print_results ();

  /* Output the VCG graph. */
  print_graph ();

  /* Output the tables and the parser to ftable.  In file output.  */
  output ();

  /* Close the input files. */
  close_files ();

  /* Free the symbol table data structure.  */
  free_symtab ();

  lex_free ();

  reduce_free ();
  free_conflicts ();
  free_nullable ();
  free_derives ();

  output_files ();

  /* If using alloca.c, flush the alloca'ed memory for the benefit of
     people running Bison as a library in IDEs.  */
#if C_ALLOCA
    alloca (0);
#endif

  return complain_message_count ? EXIT_FAILURE : EXIT_SUCCESS;
}
