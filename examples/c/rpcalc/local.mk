## Copyright (C) 2005-2006, 2008-2015, 2018-2020 Free Software
## Foundation, Inc.
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

BUILT_SOURCES += $(rpcalc_sources)
CLEANFILES +=  %D%/rpcalc.[ch] %D%/rpcalc.output
CLEANDIRS += %D%/*.dSYM

rpcalc_extracted = %D%/rpcalc.y
rpcalc_sources = $(rpcalc_extracted)
extracted += $(rpcalc_extracted)

check_PROGRAMS += %D%/rpcalc
nodist_%C%_rpcalc_SOURCES = $(rpcalc_sources)
%D%/rpcalc.c: $(dependencies)
# Don't use gnulib's system headers.
%C%_rpcalc_CPPFLAGS = -I$(top_builddir)/%D%
%C%_rpcalc_LDADD = -lm

dist_TESTS += %D%/rpcalc.test

## ------------ ##
## Installing.  ##
## ------------ ##

rpcalcdir = $(docdir)/%D%
rpcalc_DATA = $(rpcalc_extracted)
dist_rpcalc_DATA = %D%/Makefile
