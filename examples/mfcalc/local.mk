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

## -------------------- ##
## Building & testing.  ##
## -------------------- ##

BUILT_SOURCES += $(mfcalc_sources)
CLEANFILES +=  examples/mfcalc/mfcalc.[ch] examples/mfcalc/mfcalc.output

mfcalc_extracted =				\
  examples/mfcalc/calc.h			\
  examples/mfcalc/mfcalc.y
mfcalc_sources =				\
  $(mfcalc_extracted)
extracted += $(mfcalc_extracted)

check_PROGRAMS += examples/mfcalc/mfcalc
examples_mfcalc_mfcalc_LDADD = -lm
nodist_examples_mfcalc_mfcalc_SOURCES =		\
  $(mfcalc_sources)

examples_mfcalc_mfcalc_CPPFLAGS = -I$(top_builddir)/examples/mfcalc
dist_TESTS += examples/mfcalc/mfcalc.test
