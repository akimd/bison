## Process this file with automake to create Makefile.in.

## Makefile for Bison testsuite.
## Copyright 2000, 2001 Free Software Foundation, Inc.

## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

EXTRA_DIST = atlocal.in bison $(TESTSUITE_AT) testsuite

DISTCLEANFILES       = atconfig atlocal
MAINTAINERCLEANFILES = Makefile.in $(TESTSUITE)

TESTSUITE_AT = \
	testsuite.at \
	output.at calc.at regression.at
TESTSUITE = $(srcdir)/testsuite

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest
$(srcdir)/$(TESTSUITE): $(TESTSUITE_AT)
	$(AUTOTEST) -I $(srcdir) testsuite.at -o $@.tmp
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && ./config.status tests/$@

atlocal: $(srcdir)/atlocal.in $(top_builddir)/config.status
	cd $(top_builddir) && ./config.status tests/$@

clean-local:
	$(SHELL) $(TESTSUITE) --clean

check-local: atconfig atlocal $(TESTSUITE)
	$(SHELL) $(TESTSUITE)

# Run the test suite on the *installed* tree.
installcheck-local:
	$(SHELL) $(TESTSUITE) AUTOTEST_PATH=$(exec_prefix)/bin
