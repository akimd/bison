## Process this file with automake to produce Makefile.in -*-Makefile-*-

AUTOMAKE_OPTIONS = ansi2knr

noinst_LIBRARIES = libbison.a

INCLUDES = -I.. -I$(srcdir) -I../intl

libbison_a_SOURCES = getopt.c getopt1.c xmalloc.c xstrdup.c
noinst_HEADERS = getopt.h xalloc.h

libbison_a_LIBADD = @LIBOBJS@ @ALLOCA@
libbison_a_DEPENDENCIES = $(libbison_a_LIBADD)
