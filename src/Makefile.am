## Process this file with automake to produce Makefile.in -*-Makefile-*-
AUTOMAKE_OPTIONS = 1.4 ../lib/ansi2knr

DEFS = @DEFS@ \
  -DXPFILE=\"${datadir}/bison.simple\" \
  -DXPFILE1=\"${datadir}/bison.hairy\" \
  -DLOCALEDIR=\"${datadir}/locale\"
INCLUDES = -I../intl -I$(top_srcdir)/lib -I..
LDADD = @INTLLIBS@ ../lib/libbison.a

bin_PROGRAMS = bison

bison_SOURCES = LR0.c allocate.c closure.c complain.c conflicts.c	\
    derives.c	\
    files.c getargs.c gram.c lalr.c lex.c main.c nullable.c \
    output.c	\
    print.c reader.c reduce.c symtab.c warshall.c

EXTRA_bison_SOURCES = vmsgetargs.c

noinst_HEADERS = alloc.h closure.h complain.h \
 derives.h \
 files.h getargs.h gram.h lalr.h lex.h \
 output.h state.h	\
 reader.h reduce.h symtab.h system.h types.h

data_DATA = bison.simple bison.hairy

EXTRA_DIST = bison.s1 bison.hairy build.com bison.cld vmshlp.mar

bison.simple: bison.s1 Makefile
	sed -e "s/@bison_version@/$(VERSION)/" $(srcdir)/bison.s1 | \
	awk '\
	/^#line/ { printf "#line %d \"$(datadir)/bison.simple\"\n", NR+1; next }\
	{ print }' >$@t
	mv $@t $@

DISTCLEANFILES = bison.simple
