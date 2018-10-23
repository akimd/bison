## Copyright (C) 2018 Free Software Foundation, Inc.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

## -------------------- ##
## Building & testing.  ##
## -------------------- ##

BUILT_SOURCES += $(simple_sources)
CLEANFILES +=  %D%/simple.[ch] %D%/simple.output
CLEANDIRS += %D%/*.dSYM

simple_extracted = %D%/simple.yy
simple_sources = $(simple_extracted)
extracted += $(simple_extracted)

if ENABLE_CXX14
  check_PROGRAMS += %D%/simple
  nodist_%C%_simple_SOURCES = $(simple_sources)

  %C%_simple_CXXFLAGS = $(CXX11_CXXFLAGS)
  # Don't use gnulib's system headers.
  %C%_simple_CPPFLAGS = -I$(top_builddir)
  dist_TESTS += %D%/simple.test
  %D%/simple.cc: $(BISON_IN) $(dist_pkgdata_DATA)
endif


## ------------ ##
## Installing.  ##
## ------------ ##

cxxdir = $(docdir)/examples/c++
cxx_DATA = $(simple_extracted)
