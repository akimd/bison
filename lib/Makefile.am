# Make bison/lib.

# Copyright (C) 2001-2004, 2006, 2008-2011 Free Software Foundation,
# Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

include gnulib.mk

AM_CFLAGS += $(WARN_CFLAGS)

# Implementation of bitsets.
bitsets_sources = \
  abitset.c abitset.h bbitset.h bitset.c bitset.h bitset_stats.c	\
  bitset_stats.h bitsetv.c bitsetv.h ebitset.c ebitset.h lbitset.c	\
  lbitset.h libiberty.h vbitset.c vbitset.h

# Additional bitset operations.
additional_bitsets_sources = \
  bitsetv-print.h bitsetv-print.c

# timevars, stolen from GCC.
timevars_sources = \
  timevar.h timevar.c timevar.def

# Non-gnulib sources in Bison's internal library.
libbison_a_SOURCES += \
  get-errno.h get-errno.c \
  $(bitsets_sources) $(additional_bitsets_sources) $(timevars_sources)

# The Yacc compatibility library.
lib_LIBRARIES = $(YACC_LIBRARY)
EXTRA_LIBRARIES = liby.a
liby_a_SOURCES = main.c yyerror.c
