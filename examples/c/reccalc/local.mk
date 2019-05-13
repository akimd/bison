## Copyright (C) 2019 Free Software Foundation, Inc.
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

reccalcdir = $(docdir)/%D%

## ------ ##
## Calc.  ##
## ------ ##

check_PROGRAMS += %D%/reccalc
TESTS += %D%/reccalc.test
EXTRA_DIST += %D%/reccalc.test %D%/scan.l
nodist_%C%_reccalc_SOURCES = %D%/parse.y %D%/scan.h %D%/scan.c
BUILT_SOURCES += $(nodist_%C%_reccalc_SOURCES)
%D%/parse.c: $(dependencies)

# Tell Make that parse.o depends on scan.h, so that scan.h is built
# before parse.o.  Obfuscate the name of the target, otherwise
# Automake removes its recipe for parse.o and leaves only our
# additional dependency.
DASH = -
%D%/reccalc$(DASH)parse.o: %D%/scan.h

%D%/scan.c %D%/scan.h: %D%/scan.stamp
	@test -f $@ || rm -f %D%/scan.stamp
	@test -f $@ || $(MAKE) $(AM_MAKEFLAGS) %D%/scan.stamp

%D%/scan.stamp: %D%/scan.l
	$(AM_V_LEX)rm -f $@ $@.tmp
	$(AM_V_at)$(MKDIR_P) %D%
	$(AM_V_at)touch $@.tmp
	$(AM_V_at)$(LEX) -o%D%/scan.c --header-file=%D%/scan.h $(srcdir)/%D%/scan.l
	$(AM_V_at)mv $@.tmp $@

# Don't use gnulib's system headers.
%C%_reccalc_CPPFLAGS = -I$(top_srcdir)/%D% -I$(top_builddir)/%D%

dist_reccalc_DATA = %D%/parse.y %D%/scan.l %D%/Makefile %D%/README.md
CLEANFILES += %D%/parse.[ch] %D%/parse.output %D%/scan.[ch] %D%/*.stamp
CLEANDIRS += %D%/*.dSYM
