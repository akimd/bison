/* Top level entry point of bison,
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
#include "system.h"
#include "machine.h"	/* JF for MAXSHORT */

extern	int lineno;
extern	int verboseflag;

/* Nonzero means failure has been detected; don't write a parser file.  */
int failure;

/* The name this program was run with, for messages. */
char *program_name;

extern void getargs(), openfiles(), reader(), reduce_grammar();
extern void set_derives(), set_nullable(), generate_states();
extern void lalr(), initialize_conflicts(), verbose(), terse();
extern void output(), done();


/* VMS complained about using `int'.  */
int
main(argc, argv)
int argc;
char *argv[];
{
  program_name = argv[0];
  failure = 0;
  lineno = 0;
  getargs(argc, argv);
  openfiles();

  /* read the input.  Copy some parts of it to fguard, faction, ftable and fattrs.
     In file reader.c.
     The other parts are recorded in the grammar; see gram.h.  */
  reader();

  /* find useless nonterminals and productions and reduce the grammar.  In
     file reduce.c */
  reduce_grammar();

  /* record other info about the grammar.  In files derives and nullable.  */
  set_derives();
  set_nullable();

  /* convert to nondeterministic finite state machine.  In file LR0.
     See state.h for more info.  */
  generate_states();

  /* make it deterministic.  In file lalr.  */
  lalr();

  /* Find and record any conflicts: places where one token of lookahead is not
     enough to disambiguate the parsing.  In file conflicts.
     Currently this does not do anything to resolve them;
     the trivial form of conflict resolution that exists is done in output.  */
  initialize_conflicts();

  /* print information about results, if requested.  In file print. */
  if (verboseflag)
    verbose();
  else
    terse();

  /* output the tables and the parser to ftable.  In file output. */
  output();
  done(failure);
}

/* functions to report errors which prevent a parser from being generated */

void
fatal(s)
char *s;
{
  extern char *infile;

  if (infile == 0)
    fprintf(stderr, "fatal error: %s\n", s);
  else
    fprintf(stderr, "\"%s\", line %d: %s\n", infile, lineno, s);
  done(1);
}


/* JF changed to accept/deal with variable args.
   DO NOT change this to use varargs.  It will appear to work
   but will break on systems that don't have the necessary library
   functions.  This is the ONLY safe way to write such a function.  */
/*VARARGS1*/

void
fatals(fmt,x1,x2,x3,x4,x5,x6,x7,x8)
char *fmt;
{
  char buffer[200];

  sprintf(buffer, fmt, x1,x2,x3,x4,x5,x6,x7,x8);
  fatal(buffer);
}


void
toomany(s)
char *s;
{
  char buffer[200];

	/* JF new msg */
  sprintf(buffer, "limit of %d exceeded, too many %s", MAXSHORT, s);
  fatal(buffer);
}


void
berror(s)
char *s;
{
  fprintf(stderr, "internal error, %s\n", s);
  abort();
}
