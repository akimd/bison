## Process this file with automake to create Makefile.in.

## Makefile for Bison testsuite.
## Copyright 2000, 2001, 2002  Free Software Foundation, Inc.

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

EXTRA_DIST = $(TESTSUITE_AT) testsuite

check_SCRIPTS        = bison
DISTCLEANFILES       = atconfig $(check_SCRIPTS)
MAINTAINERCLEANFILES = Makefile.in $(TESTSUITE)

## ------------ ##
## package.m4.  ##
## ------------ ##

$(srcdir)/package.m4: $(top_srcdir)/configure.in
	{					\
	  echo '# Signature of the current package.'; \
	  echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])'; \
	  echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])'; \
	  echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])'; \
	  echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])'; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
	} >$(srcdir)/package.m4

EXTRA_DIST += package.m4

## ------------ ##
## Test suite.  ##
## ------------ ##

TESTSUITE_AT = \
	testsuite.at \
	input.at output.at sets.at reduce.at calc.at torture.at regression.at
TESTSUITE = $(srcdir)/testsuite

AUTOTEST = $(AUTOM4TE) --language=autotest
$(srcdir)/$(TESTSUITE): package.m4 $(TESTSUITE_AT)
	$(AUTOTEST) -I $(srcdir) testsuite.at -o $@.tmp
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && ./config.status tests/$@

clean-local:
	$(SHELL) $(TESTSUITE) --clean

check-local: atconfig atlocal $(TESTSUITE)
	$(SHELL) $(TESTSUITE)

# Run the test suite on the *installed* tree.
installcheck-local:
	$(SHELL) $(TESTSUITE) AUTOTEST_PATH=$(exec_prefix)/bin
