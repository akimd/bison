## Copyright (C) 2018-2019 Free Software Foundation, Inc.
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

ddir = $(docdir)/%D%

## ------ ##
## Calc.  ##
## ------ ##

if ENABLE_D
  check_SCRIPTS += %D%/calc
  TESTS += %D%/calc.test
endif
EXTRA_DIST += %D%/calc.test

%D%/calc.d: %D%/calc.y $(dependencies)
	$(AM_V_GEN)$(MKDIR_P) %D%
	$(AM_V_at)$(BISON) $(srcdir)/%D%/calc.y -o $@

%D%/calc: %D%/calc.d
	$(AM_V_GEN) $(DC) $(DCFLAGS) -of$@ %D%/calc.d

dist_d_DATA = %D%/calc.y %D%/Makefile %D%/README.md
CLEANFILES += %D%/calc %D%/calc.[do]
