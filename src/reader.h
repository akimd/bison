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

# include "symlist.h"
# include "parse-gram.h"

typedef struct merger_list
{
  struct merger_list* next;
  struniq_t name;
  struniq_t type;
}
merger_list;

/* From the scanner.  */
extern FILE *gram_in;
extern int gram__flex_debug;
void scanner_initialize (void);
void scanner_free (void);
void scanner_last_string_free (void);

# define YY_DECL int gram_lex (yystype *val, location_t *loc)
YY_DECL;


/* From the parser.  */
extern int gram_debug;
int gram_parse (void);

/* The sort of braced code we are in.  */
typedef enum braced_code_e
  {
    action_braced_code,
    destructor_braced_code,
    printer_braced_code
  } braced_code_t;
/* FIXME: This is really a dirty hack which demonstrates that we
   should probably not try to parse the actions now.  */
extern braced_code_t current_braced_code;


/* From reader.c. */
void grammar_start_symbol_set (symbol_t *s, location_t l);
void prologue_augment (const char *prologue, location_t location);
void epilogue_augment (const char *epilogue, location_t location);
void grammar_symbol_append (symbol_t *s, location_t l);
void grammar_rule_begin (symbol_t *lhs, location_t l);
void grammar_rule_end (location_t l);
void grammar_midrule_action (void);
void grammar_current_rule_prec_set (symbol_t *precsym, location_t l);
void grammar_current_rule_dprec_set (int dprec, location_t l);
void grammar_current_rule_merge_set (struniq_t name, location_t l);

void grammar_current_rule_symbol_append (symbol_t *symbol, location_t l);
void grammar_current_rule_action_append (const char *action, location_t l);
extern symbol_list_t *current_rule;
void reader (void);
void free_merger_functions (void);

extern merger_list *merge_functions;

extern int typed;

#endif /* !READER_H_ */
