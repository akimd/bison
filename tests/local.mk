## Makefile for Bison testsuite.

## Copyright (C) 2000-2012 Free Software Foundation, Inc.

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

EXTRA_DIST += $(TESTSUITE_AT) tests/testsuite tests/package.m4

DISTCLEANFILES       += tests/atconfig $(check_SCRIPTS)
MAINTAINERCLEANFILES += $(TESTSUITE)

## ------------ ##
## package.m4.  ##
## ------------ ##

$(top_srcdir)/tests/package.m4: $(top_srcdir)/configure
	rm -f $@ $@.tmp
	{ \
	  echo '# Signature of the current package.'; \
	  echo 'm4_define([AT_PACKAGE_NAME],      [$(PACKAGE_NAME)])'; \
	  echo 'm4_define([AT_PACKAGE_TARNAME],   [$(PACKAGE_TARNAME)])'; \
	  echo 'm4_define([AT_PACKAGE_VERSION],   [$(PACKAGE_VERSION)])'; \
	  echo 'm4_define([AT_PACKAGE_STRING],    [$(PACKAGE_STRING)])'; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])'; \
	} >$@.tmp
	mv $@.tmp $@

## ------------ ##
## Test suite.  ##
## ------------ ##

TESTSUITE_AT =                                  \
  tests/actions.at                              \
  tests/c++.at                                  \
  tests/calc.at                                 \
  tests/conflicts.at                            \
  tests/cxx-type.at                             \
  tests/existing.at                             \
  tests/glr-regression.at                       \
  tests/headers.at                              \
  tests/input.at                                \
  tests/java.at                                 \
  tests/local.at                                \
  tests/named-refs.at                           \
  tests/output.at                               \
  tests/push.at                                 \
  tests/reduce.at                               \
  tests/regression.at                           \
  tests/sets.at                                 \
  tests/skeletons.at                            \
  tests/synclines.at                            \
  tests/testsuite.at                            \
  tests/torture.at

TESTSUITE = $(top_srcdir)/tests/testsuite
RUN_TESTSUITE = $(TESTSUITE) -C tests $(TESTSUITEFLAGS)

AUTOTEST = $(AUTOM4TE) --language=autotest
AUTOTESTFLAGS = -I $(top_srcdir)/tests
$(TESTSUITE): $(top_srcdir)/tests/package.m4 $(TESTSUITE_AT)
	$(AUTOTEST) $(AUTOTESTFLAGS) $(top_srcdir)/tests/testsuite.at -o $@.tmp
	mv $@.tmp $@

clean-local: clean-local-tests
clean-local-tests:
	test ! -f $(TESTSUITE) || $(TESTSUITE) -C tests --clean

check-local: tests/atconfig tests/atlocal $(TESTSUITE)
# Move into tests/ so that testsuite.dir etc. be created there.
	$(RUN_TESTSUITE)

check_SCRIPTS = tests/bison

# Run the test suite on the *installed* tree.
installcheck-local:
	$(RUN_TESTSUITE) AUTOTEST_PATH="$(bindir)"

# Be real mean with it.
.PHONY: maintainer-check-g++
maintainer-check-g++: $(TESTSUITE)
	$(RUN_TESTSUITE) CC='$(CXX)'

.PHONY: maintainer-check-posix
maintainer-check-posix: $(TESTSUITE)
	$(RUN_TESTSUITE) POSIXLY_CORRECT=1 _POSIX2_VERSION=200112

.PHONY: maintainer-check-valgrind
maintainer-check-valgrind: $(TESTSUITE)
	test -z '$(VALGRIND)' || \
	   VALGRIND_OPTS='--leak-check=full --show-reachable=yes' \
	   $(RUN_TESTSUITE) PREBISON='$(VALGRIND) -q' PREPARSER='$(VALGRIND) -q'

.PHONY: maintainer-check
maintainer-check: maintainer-check-posix maintainer-check-valgrind maintainer-check-g++

.PHONY: maintainer-push-check
maintainer-push-check:
	BISON_USE_PUSH_FOR_PULL=1 $(MAKE) $(AM_MAKEFLAGS) maintainer-check

.PHONY: maintainer-xml-check
maintainer-xml-check:
	BISON_TEST_XML=1 $(MAKE) $(AM_MAKEFLAGS) maintainer-check
