## Process this file with automake to produce Makefile.in -*-Makefile-*-
AUTOMAKE_OPTIONS = 1.4 ../lib/ansi2knr
DEFS = @DEFS@ \
  -DBISON_SIMPLE=\"$(pkgdatadir)/bison.simple\" \
  -DBISON_HAIRY=\"$(pkgdatadir)/bison.hairy\" \
  -DLOCALEDIR=\"$(datadir)/locale\"

INCLUDES = -I../intl -I$(top_srcdir)/lib -I..
LDADD = @INTLLIBS@ ../lib/libbison.a

bin_PROGRAMS = bison

bison_SOURCES = LR0.c closure.c complain.c conflicts.c	\
    derives.c	\
    files.c getargs.c gram.c lalr.c lex.c main.c nullable.c \
    output.c	\
    print.c reader.c reduce.c symtab.c warshall.c vcg.c print_graph.c

EXTRA_bison_SOURCES = vmsgetargs.c

noinst_HEADERS = LR0.h closure.h complain.h conflicts.h \
 derives.h \
 files.h getargs.h gram.h lalr.h lex.h nullable.h \
 output.h state.h	\
 print.h reader.h reduce.h symtab.h warshall.h system.h types.h \
 vcg.h vcg_defaults.h print_graph.h

pkgdata_DATA = bison.simple bison.hairy

EXTRA_DIST = $(pkgdata_DATA) build.com bison.cld vmshlp.mar
