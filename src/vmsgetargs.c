/* VMS version of getargs; Uses DCL command parsing.
   Copyright 1989, 1992 Free Software Foundation, Inc.

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


#include <ctype.h>
#include <stdio.h>
#include "files.h"

/*
 *	VMS version of getargs: Uses DCL command parsing
 *		(argc and argv are ignored)
 */
int verbose_flag;
int defines_flag;
int debug_flag;
int no_lines_flag;
extern int no_parser_flag;
extern int token_table_flag;
extern int raw_flag;
extern int yacc_flag;
extern char * version_string;

getargs(argc,argv)
     int argc;
     char *argv[];
{
  register char *cp;
  static char Input_File[256];
  static char output_spec[256], name_prefix_spec[256], file_prefix_spec[256];
  extern char *infile;

  verbose_flag = 0;
  defines_flag = 0;
  debug_flag = 0;
  yacc_flag = 0;
  no_lines_flag = 0;
  no_parser_flag = 0;
  token_table_flag = 0;
  raw_flag = 0;
  /*
   *	Check for /VERBOSE qualifier
   */
  if (cli_present("BISON$VERBOSE")) verbose_flag = 1;
  /*
   *	Check for /DEFINES qualifier
   */
  if (cli_present("BISON$DEFINES")) defines_flag = 1;
  /*
   *	Check for /FIXED_OUTFILES qualifier
   */
  if (cli_present("BISON$FIXED_OUTFILES")) yacc_flag = 1;
  if (cli_present("BISON$YACC")) yacc_flag = 1;
  /*
   *	Check for /VERSION qualifier
   */
  if (cli_present("BISON$VERSION")) printf("%s",version_string);
  /*
   *	Check for /NOLINES qualifier
   */
  if (cli_present("BISON$NOLINES")) no_lines_flag = 1;
  /*
   *	Check for /NOPARSER qualifier
   */
  if (cli_present("BISON$NOPARSER")) no_parser_flag = 1;
  /*
   *	Check for /RAW qualifier
   */
  if (cli_present("BISON$RAW")) raw_flag = 1;
  /*
   *	Check for /TOKEN_TABLE qualifier
   */
  if (cli_present("BISON$TOKEN_TABLE")) token_table_flag = 1;
  /*
   *	Check for /DEBUG qualifier
   */
  if (cli_present("BISON$DEBUG")) debug_flag = 1;
  /*
   *	Get the filename
   */
  cli_get_value("BISON$INFILE", Input_File, sizeof(Input_File));
  /*
   *	Lowercaseify the input filename
   */
  cp = Input_File;
  while(*cp)
    {
      if (isupper(*cp)) *cp = tolower(*cp);
      cp++;
    }
  infile = Input_File;
  /*
   *	Get the output file
   */
  if (cli_present("BISON$OUTPUT"))
    {
      cli_get_value("BISON$OUTPUT", output_spec, sizeof(output_spec));
      for (cp = spec_outfile = output_spec; *cp; cp++)
	if (isupper(*cp))
	  *cp = tolower(*cp);
    }
  /*
   *	Get the output file
   */
  if (cli_present("BISON$FILE_PREFIX"))
    {
      cli_get_value("BISON$FILE_PREFIX", file_prefix_spec,
		     sizeof(file_prefix_spec));
      for (cp = spec_file_prefix = file_prefix_spec; *cp; cp++)
	if (isupper(*cp))
	  *cp = tolower(*cp);
    }
  /*
   *	Get the output file
   */
  if (cli_present("BISON$NAME_PREFIX"))
    {
      cli_get_value("BISON$NAME_PREFIX", name_prefix_spec,
		     sizeof(name_prefix_spec));
      for (cp = spec_name_prefix = name_prefix_spec; *cp; cp++)
	if (isupper(*cp))
	  *cp = tolower(*cp);
    }
}

/************		DCL PARSING ROUTINES		**********/

/*
 *	See if "NAME" is present
 */
int
cli_present(Name)
     char *Name;
{
  struct {int Size; char *Ptr;} Descr;

  Descr.Ptr = Name;
  Descr.Size = strlen(Name);
  return (cli$present(&Descr) & 1) ? 1 : 0;
}

/*
 *	Get value of "NAME"
 */
int
cli_get_value(Name,Buffer,Size)
     char *Name;
     char *Buffer;
{
  struct {int Size; char *Ptr;} Descr1,Descr2;

  Descr1.Ptr = Name;
  Descr1.Size = strlen(Name);
  Descr2.Ptr = Buffer;
  Descr2.Size = Size-1;
  if (cli$get_value(&Descr1,&Descr2,&Descr2.Size) & 1) {
    Buffer[Descr2.Size] = 0;
    return 1;
  }
  return 0;
}
