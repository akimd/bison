# Copyright (C) 2005-2006, 2008-2013 Free Software Foundation, Inc.
#
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

## ------------------- ##
## Parser generation.  ##
## ------------------- ##

# Don't depend on $(BISON) otherwise we would rebuild these files
# in srcdir, including during distcheck, which is forbidden.
examples/calc++/calc++-parser.stamp: $(BISON_IN)
SUFFIXES += .yy .stamp
.yy.stamp:
	$(AM_V_YACC)rm -f $@
	$(AM_V_at)touch $@.tmp
	$(AM_V_at)$(YACCCOMPILE) -o $*.cc $<
	$(AM_V_at)mv -f $@.tmp $@

$(calc_sources_generated): examples/calc++/calc++-parser.stamp
	@test -f $@ || rm -f examples/calc++/calc++-parser.stamp
	@test -f $@ || $(MAKE) $(AM_MAKEFLAGS) examples/calc++/calc++-parser.stamp
CLEANFILES +=                                   \
  $(calc_sources_generated)                     \
  examples/calc++/calc++-parser.output          \
  examples/calc++/calc++-parser.stamp           \
  examples/calc++/calc++-scanner.cc


## -------------------- ##
## Building & testing.  ##
## -------------------- ##

# Avoid using BUILT_SOURCES which is too global.
$(examples_calc___calc___OBJECTS): $(calc_sources_generated)

calc_sources_extracted =                        \
  examples/calc++/calc++-driver.cc              \
  examples/calc++/calc++-driver.hh              \
  examples/calc++/calc++-scanner.ll             \
  examples/calc++/calc++.cc
calc_extracted =                                \
  $(calc_sources_extracted)                     \
  examples/calc++/calc++-parser.yy
extracted += $(calc_extracted)
calc_sources_generated =                        \
  examples/calc++/calc++-parser.cc              \
  examples/calc++/calc++-parser.hh              \
  examples/calc++/location.hh                   \
  examples/calc++/position.hh                   \
  examples/calc++/stack.hh
calc_sources =                                  \
  $(calc_sources_extracted)                     \
  $(calc_sources_generated)

if BISON_CXX_WORKS
check_PROGRAMS += examples/calc++/calc++
nodist_examples_calc___calc___SOURCES =         \
  $(calc_sources)

examples_calc___calc___CPPFLAGS = -I$(top_builddir)/examples/calc++
examples_calc___calc___CXXFLAGS = $(AM_CXXFLAGS) $(FLEX_SCANNER_CXXFLAGS)
dist_TESTS += examples/calc++/calc++.test
else
EXTRA_DIST += examples/calc++/calc++.test
endif
