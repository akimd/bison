/* Concentrate all options use in bison,
   Copyright 2001 Free Software Foundation, Inc.

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

#ifndef OPTIONS_H_
# define OPTIONS_H_

/* opt_access_t and struct option_table_struct need to be declare 
   here, for the parse_percent_token function in lex.c.  */

/* Option accessibility.  */
typedef enum opt_access_e
{
  opt_cmd_line = 1,
  opt_percent,
  opt_both
} opt_access_t;

/* This is the general struct, which contains user's options from
   command line or in grammar with percent flag.  */
struct option_table_struct
{
  /* Set the accessibility.  */
  opt_access_t access;
  /* This name is used for long option et percent directives.  */
  const char *name;
  /* Use for command line.  */
  int has_arg;
  /* A set_flag value causes the named flag to be set.  */
  int *set_flag;
  /* A retval action returns the code.  */
  int ret_val;
  /* The short option value, frequently a letter.  */
  int val;
};

extern const char *shortopts;
extern struct option *longopts;

/* Table which contain all options.  */
extern const struct option_table_struct option_table[];

/* Set the longopts variable from option_table.  */
void create_long_option_table PARAMS (());

#endif /* !OPTIONS_H_ */
