## Process this file with automake to produce Makefile.in -*-Makefile-*-

## Copyright (C) 2005-2006, 2008-2012 Free Software Foundation, Inc.

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

## ------------ ##
## Extracting.  ##
## ------------ ##

# Extract in src.
$(top_srcdir)/examples/calc++/calc.stamp: $(doc) $(extexi)
	$(AM_V_GEN)rm -f $@ $@.tmp
	$(AM_V_at)touch $@.tmp
	$(AM_V_at)cd $(top_srcdir)/examples/calc++ && \
	   $(AWK) -f ../extexi -v VERSION="$(VERSION)" \
	     ../../doc/bison.texinfo -- calc++-parser.yy \
	     calc++-scanner.ll calc++.cc calc++-driver.hh calc++-driver.cc
	$(AM_V_at)mv $@.tmp $@

$(calc_extracted): $(top_srcdir)/examples/calc++/calc.stamp
	$(AM_V_GEN)if test -f $@; then :; else \
	  rm -f $< && \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

## -------------------- ##
## Building & testing.  ##
## -------------------- ##

BUILT_SOURCES += $(calc_sources) examples/calc++/calc++-parser.h
CLEANFILES += *.tmp
MAINTAINERCLEANFILES += $(top_srcdir)/examples/calc++/calc.stamp $(calc_sources)
EXTRA_DIST += examples/calc++/calc.stamp

calc_extracted =				\
  examples/calc++/calc++-scanner.ll		\
  examples/calc++/calc++.cc			\
  examples/calc++/calc++-driver.hh		\
  examples/calc++/calc++-driver.cc		\
  examples/calc++/calc++-parser.yy
calc_generated =				\
  examples/calc++/stack.hh			\
  examples/calc++/position.hh			\
  examples/calc++/location.hh
calc_sources =					\
  $(calc_extracted) $(calc_generated)
if BISON_CXX_WORKS
check_PROGRAMS += examples/calc++/calc++
examples_calc___calc___SOURCES =		\
  $(calc_sources)				\
  examples/calc++/y.tab.h			\
  examples/calc++/calc++-parser.hh

examples_calc___calc___CPPFLAGS = -I$(top_srcdir)/examples/calc++
TESTS += examples/calc++/calc++.test
endif
EXTRA_DIST += examples/calc++/calc++.test
