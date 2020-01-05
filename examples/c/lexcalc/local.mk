## Copyright (C) 2018-2020 Free Software Foundation, Inc.
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

lexcalcdir = $(docdir)/%D%

## --------- ##
## LexCalc.  ##
## --------- ##

if FLEX_WORKS
  check_PROGRAMS += %D%/lexcalc
  TESTS += %D%/lexcalc.test
  nodist_%C%_lexcalc_SOURCES = %D%/parse.y %D%/parse.h %D%/scan.l
  # Don't use gnulib's system headers.
  %C%_lexcalc_CPPFLAGS = -I$(top_srcdir)/%D% -I$(top_builddir)/%D%
endif FLEX_WORKS

%D%/parse.c: $(dependencies)

EXTRA_DIST += %D%/lexcalc.test
dist_lexcalc_DATA = %D%/parse.y %D%/scan.l %D%/Makefile %D%/README.md
CLEANFILES += %D%/parse.[ch] %D%/scan.c %D%/parse.output
CLEANDIRS += %D%/*.dSYM
