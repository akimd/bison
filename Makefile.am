PFILE = bison.simple
PFILE1 = bison.hairy
INCLUDES = -DXPFILE=\"$(datadir)/$(PFILE)\" \
	   -DXPFILE1=\"$(datadir)/$(PFILE1)\"
bin_PROGRAMS = bison
bison_SOURCES = LR0.c allocate.c closure.c conflicts.c derives.c	\
    files.c getargs.c gram.c lalr.c lex.c main.c nullable.c output.c	\
    print.c reader.c reduce.c symtab.c warshall.c version.c getopt.c	\
    getopt1.c
EXTRA_bison_SOURCES = vmsgetargs.c
bison_LDADD = @ALLOCA@
noinst_HEADERS = files.h gram.h lex.h machine.h new.h state.h symtab.h	\
    system.h types.h getopt.h
data_DATA = $(PFILE) $(PFILE1)
info_TEXINFOS = bison.texinfo
man_MANS = bison.1
EXTRA_DIST = $(PFILE) $(PFILE1) $(man_MANS)				\
    REFERENCES configure.bat build.com bison.cld bison.rnh vmshlp.mar
