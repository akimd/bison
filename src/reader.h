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
  const char* name;
  const char* type;
}
merger_list;

typedef struct gram_control_s
{
  int errcode;
} gram_control_t;

/* From the scanner.  */
extern FILE *gram_in;
extern int gram__flex_debug;
void scanner_last_string_free PARAMS ((void));
void scanner_initialize PARAMS ((void));
void scanner_free PARAMS ((void));

# define YY_DECL \
  int gram_lex (yystype *yylval, location_t *yylloc, \
		gram_control_t *yycontrol)
YY_DECL;


/* From the parser.  */
extern int gram_debug;
void gram_error (location_t *loc, const char *msg);
int gram_parse (void *control);

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
void grammar_start_symbol_set PARAMS ((symbol_t *s, location_t l));
void prologue_augment PARAMS ((const char *prologue, location_t location));
void epilogue_set PARAMS ((const char *epilogue, location_t location));
void grammar_symbol_append PARAMS ((symbol_t *s, location_t l));
void grammar_rule_begin PARAMS ((symbol_t *lhs, location_t l));
void grammar_rule_end PARAMS ((location_t l));
void grammar_midrule_action PARAMS ((void));
void grammar_current_rule_prec_set PARAMS ((symbol_t *precsym,
					    location_t l));
void grammar_current_rule_dprec_set PARAMS ((int dprec,
					    location_t l));
void grammar_current_rule_merge_set PARAMS ((const char* name,
					    location_t l));

void grammar_current_rule_symbol_append PARAMS ((symbol_t *symbol,
						 location_t l));
void grammar_current_rule_action_append PARAMS ((const char *action,
						 location_t l));
extern symbol_list_t *current_rule;
void reader PARAMS ((void));
void free_merger_functions PARAMS ((void));

extern merger_list *merge_functions;

extern int typed;

#endif /* !READER_H_ */
