## Copyright (C) 2001, 2002 Free Software Foundation, Inc.

## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
## 02111-1307  USA

AUTOMAKE_OPTIONS = ../lib/ansi2knr

DEFS = @DEFS@ \
  -DPKGDATADIR=\"$(pkgdatadir)\" \
  -DLOCALEDIR=\"$(datadir)/locale\"

AM_CFLAGS = $(WARNING_CFLAGS) $(WERROR_CFLAGS)
AM_YFLAGS = "-dv"

# config.h is in build/.
INCLUDES = -I$(top_builddir)      \
           -I$(top_srcdir)/lib

LDADD = $(INTLLIBS) ../lib/libbison.a

bin_PROGRAMS = bison

bison_SOURCES = 				  \
	LR0.c LR0.h				  \
	assoc.c assoc.h				  \
	closure.c closure.h			  \
	complain.c complain.h			  \
	conflicts.c conflicts.h			  \
	derives.c derives.h			  \
	files.c files.h				  \
	getargs.c getargs.h			  \
	gram.c gram.h				  \
	lalr.h lalr.c				  \
	location.c location.h			  \
	main.c					  \
	muscle_tab.c muscle_tab.h		  \
	nullable.c nullable.h			  \
	output.c output.h			  \
	parse-gram.h parse-gram.y		  \
	print.c print.h				  \
	print_graph.c print_graph.h		  \
	reader.c reader.h			  \
	reduce.c reduce.h			  \
	relation.c relation.h			  \
	scan-gram.l				  \
	scan-skel.l				  \
	state.c state.h				  \
	symlist.c symlist.h			  \
	symtab.c symtab.h			  \
	system.h				  \
	tables.h tables.c			  \
	vcg.c vcg.h				  \
	vcg_defaults.h

BUILT_SOURCES = scan-skel.c scan-gram.c parse-gram.c parse-gram.h

EXTRA_bison_SOURCES = vmsgetargs.c

EXTRA_DIST = build.com bison.cld vmshlp.mar

echo:
	echo $(bison_SOURCES) $(noinst_HEADERS)
