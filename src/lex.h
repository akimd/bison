/* Token type definitions for bison's input reader,
   Copyright 1984, 1989, 1992, 2000, 2001 Free Software Foundation, Inc.

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

#ifndef LEX_H_
# define LEX_H_

/* Token-type codes. */
typedef enum token_e
  {
    tok_undef,	/* Not defined.  Used to initial token_t vars. */
    tok_eof,
    tok_identifier,
    tok_comma,
    tok_colon,
    tok_semicolon,
    tok_bar,
    tok_left_curly,
    tok_two_percents,
    tok_percent_left_curly,
    tok_token,
    tok_nterm,
    tok_guard,
    tok_type,
    tok_union,
    tok_start,
    tok_left,
    tok_right,
    tok_nonassoc,
    tok_prec,
    tok_typename,
    tok_number,
    tok_expect,
    tok_thong,
    tok_noop,
    tok_intopt,
    tok_stropt,
    tok_illegal,
    tok_obsolete
  } token_t;

extern const char *token_buffer;
extern bucket *symval;
extern int numval;

void lex_init PARAMS ((void));
void lex_free PARAMS ((void));
int skip_white_space PARAMS ((void));
void unlex PARAMS ((token_t));
void read_type_name PARAMS ((FILE *fin));

/* Return one of the token-type codes.  When an identifier is seen,
   the code IDENTIFIER is returned and the name is looked up in the
   symbol table using symtab.c; symval is set to a pointer to the
   entry found.  */

token_t lex PARAMS ((void));

token_t parse_percent_token PARAMS ((void));

#endif /* !LEX_H_ */
