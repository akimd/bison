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

AUTOMAKE_OPTIONS = ansi2knr

AM_CFLAGS = $(WARNING_CFLAGS)

noinst_LIBRARIES = libbison.a

# config.h is in build/., and system.h is in src/src/.
INCLUDES = -I$(top_builddir)      \
           -I$(top_srcdir)/src \
           -I$(top_srcdir)/lib

# Heck, we are still using an old version of Automake which does not
# understand inner AC_LIBOBJ (i.e., from Autoconf itself, not from
# configure.in).
EXTRA_DIST = malloc.c realloc.c strnlen.c

libbison_a_SOURCES = \
  gettext.h \
  basename.c dirname.h dirname.c \
  getopt.h getopt.c getopt1.c \
  hash.h hash.c \
  quote.h quote.c quotearg.h quotearg.c \
  readpipe.c unlocked-io.h \
  xalloc.h xmalloc.c xstrdup.c xstrndup.c \
  $(bitsets_sources) $(additional_bitsets_sources) $(timevars_sources)

# Implementation of bitsets
bitsets_sources = \
libiberty.h \
abitset.c  bitset.c        bitset_stats.h  ebitset.c  lbitset.h \
abitset.h  bitset.h        bitsetv.c       ebitset.h \
bbitset.h  bitset_stats.c  bitsetv.h       lbitset.c

# Additional bitset operations.
additional_bitsets_sources = \
	bitsetv-print.h bitsetv-print.c

# timevars, stolen from GCC.
timevars_sources = \
	timevar.h timevar.c timevar.def

libbison_a_LIBADD = @LIBOBJS@ @ALLOCA@
libbison_a_DEPENDENCIES = $(libbison_a_LIBADD)
