/* Parse command line arguments for bison.
   Copyright (C) 1984, 1986, 1989, 1992, 2000 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#ifndef GETARGS_H_
# define GETARGS_H_

/* flags set by % directives */
extern char *spec_file_prefix;	/* for -b */
extern char *spec_name_prefix; 	/* for -p */

extern int debugflag;  		/* for -t */
extern int definesflag;    	/* for -d */
extern int nolinesflag;    	/* for -l */
extern int noparserflag;	/* for -n */
extern int rawtoknumflag;	/* for -r */
extern int statisticsflag;
extern int toknumflag;   	/* for -k */
extern int verboseflag;		/* for -v */
extern int yaccflag;  		/* for -y */

void getargs PARAMS ((int argc, char *argv[]));

#endif /* !GETARGS_H_ */
