/* Input parser for bison
   Copyright (C) 2000, 2001, 2002  Free Software Foundation, Inc.

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

#ifndef READER_H_
# define READER_H_

typedef struct symbol_list
{
  struct symbol_list *next;
  symbol_t *sym;
  int line;

  /* The action is attached to the LHS of a rule. */
  const char *action;
  int action_line;

  symbol_t *ruleprec;
} symbol_list;

# include "parse-gram.h"

typedef int location_t;

/* Initialize LOC. */
# define LOCATION_RESET(Loc)                  \
  (Loc).first_column = (Loc).first_line = 1;  \
  (Loc).last_column =  (Loc).last_line = 1;

/* Advance of NUM columns. */
# define LOCATION_COLUMNS(Loc, Num)           \
  (Loc).last_column += Num;

/* Advance of NUM lines. */
# define LOCATION_LINES(Loc, Num)             \
  (Loc).last_column = 1;                      \
  (Loc).last_line += Num;

/* Restart: move the first cursor to the last position. */
# define LOCATION_STEP(Loc)                   \
  (Loc).first_column = (Loc).last_column;     \
  (Loc).first_line = (Loc).last_line;

/* Output LOC on the stream OUT. */
# define LOCATION_PRINT(Out, Loc)                               \
  fprintf (stderr, "%s:", infile);				\
  if ((Loc).first_line != (Loc).last_line)                      \
    fprintf (Out, "%d.%d-%d.%d",                                \
             (Loc).first_line, (Loc).first_column,              \
             (Loc).last_line, (Loc).last_column - 1);           \
  else if ((Loc).first_column < (Loc).last_column - 1)          \
    fprintf (Out, "%d.%d-%d", (Loc).first_line,                 \
             (Loc).first_column, (Loc).last_column - 1);        \
  else                                                          \
    fprintf (Out, "%d.%d", (Loc).first_line, (Loc).first_column)

typedef struct gram_control_s
{
  int errcode;
} gram_control_t;

/* From the scanner.  */
extern FILE *gram_in;
extern int gram__flex_debug;

# define YY_DECL \
  int gram_lex (yystype *yylval, yyltype *yylloc, \
		gram_control_t *yycontrol)
YY_DECL;


/* From the parser.  */
extern int gram_debug;
void gram_error (gram_control_t *control,
		 yyltype *loc, const char *msg);
int gram_parse (void *control);

char *get_type_name PARAMS ((int n, symbol_list *rule));
extern int typed;

/* From reader.c. */
void grammar_start_symbol_set PARAMS ((symbol_t *s));
void prologue_augment PARAMS ((const char *prologue, location_t location));
void epilogue_set PARAMS ((const char *epilogue, location_t location));
void grammar_symbol_append PARAMS ((symbol_t *s));
void grammar_rule_begin PARAMS ((symbol_t *lhs));
void grammar_rule_end PARAMS ((void));
void grammar_midrule_action PARAMS ((void));
void grammar_current_rule_prec_set PARAMS ((symbol_t *precsym));
void grammar_current_rule_symbol_append PARAMS ((symbol_t *symbol));
void grammar_current_rule_action_append PARAMS ((const char *action,
						 int line));
extern symbol_list *current_rule;
void reader PARAMS ((void));

#endif /* !READER_H_ */
