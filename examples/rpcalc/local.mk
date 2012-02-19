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
$(top_srcdir)/examples/rpcalc/rpcalc.stamp: $(doc) $(extexi)
	$(AM_V_GEN)rm -f $@ $@.tmp
	$(AM_V_at)touch $@.tmp
	$(AM_V_at)cd $(top_srcdir)/examples/rpcalc && \
	   $(AWK) -f ../extexi -v VERSION="$(VERSION)" \
	     ../../doc/bison.texinfo -- calc.h rpcalc.y
	$(AM_V_at)mv $@.tmp $@

$(rpcalc_extracted): $(top_srcdir)/examples/rpcalc/rpcalc.stamp
	$(AM_V_GEN)if test -f $@; then :; else \
	  rm -f $< && \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

## -------------------- ##
## Building & testing.  ##
## -------------------- ##

BUILT_SOURCES += $(rpcalc_sources)
MAINTAINERCLEANFILES += $(top_srcdir)/examples/rpcalc/rpcalc.stamp $(rpcalc_sources)
EXTRA_DIST += examples/rpcalc/rpcalc.stamp

rpcalc_extracted =				\
  examples/rpcalc/rpcalc.y
rpcalc_sources =				\
  $(rpcalc_extracted)

check_PROGRAMS += examples/rpcalc/rpcalc
examples_rpcalc_rpcalc_LDADD = -lm
examples_rpcalc_rpcalc_SOURCES =		\
  $(rpcalc_sources)

examples_rpcalc_rpcalc_CPPFLAGS = -I$(top_srcdir)/examples/rpcalc
TESTS += examples/rpcalc/rpcalc.test
EXTRA_DIST += examples/rpcalc/rpcalc.test
