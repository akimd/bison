## Process this file with automake to produce Makefile.in -*-Makefile-*-

AUTOMAKE_OPTIONS = ansi2knr

noinst_LIBRARIES = libbison.a

INCLUDES = -I.. -I$(srcdir) -I../intl

# Heck, we are still using an old version of Automake which does not
# understand LIBOBJ additions using $objext...

EXTRA_DIST = malloc.c realloc.c

libbison_a_SOURCES = \
  error.c error.h \
  getopt.h getopt.c getopt1.c \
  obstack.h obstack.c \
  quote.h quote.c quotearg.h quotearg.c \
  xalloc.h xmalloc.c xstrdup.c

libbison_a_LIBADD = @LIBOBJS@
libbison_a_DEPENDENCIES = $(libbison_a_LIBADD)
