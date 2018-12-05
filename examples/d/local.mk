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

ddir = $(docdir)/%D%

## ------ ##
## Calc.  ##
## ------ ##

if ENABLE_D
  check_SCRIPTS += %D%/calc
  TESTS += %D%/calc.test
endif

%D%/calc.d: %D%/calc.y $(BISON_IN) $(dist_pkgdata_DATA)
	$(AM_V_GEN)$(MKDIR_P) %D%
	$(AM_V_at)$(BISON) $< -o $@

%D%/calc: %D%/calc.d
	$(AM_V_GEN) $(DC) $(DCFLAGS) -of$@ $<

dist_d_DATA = %D%/calc.y %D%/Makefile
CLEANFILES += %D%/calc %D%/Calc.d
