## Process this file with automake to produce Makefile.in -*-Makefile-*-

## Copyright (C) 2001-2012 Free Software Foundation, Inc.

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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = build-aux po runtime-po lib data src doc examples tests etc

# Files installed for use by Automake.
aclocaldir = @aclocaldir@
aclocal_DATA = m4/bison-i18n.m4

EXTRA_DIST = .prev-version .version \
  cfg.mk ChangeLog-1998 ChangeLog-2012 PACKAGING \
  djgpp/Makefile.maint djgpp/README.in djgpp/config.bat \
  djgpp/config.sed djgpp/config.site djgpp/config_h.sed \
  djgpp/subpipe.c djgpp/subpipe.h djgpp/djunpack.bat \
  djgpp/fnchange.lst djgpp/testsuite.sed

MAINTAINER_CHECKS =				\
  maintainer-check				\
  maintainer-push-check				\
  maintainer-xml-check				\
  maintainer-release-check
.PHONY: $(MAINTAINER_CHECKS)
$(MAINTAINER_CHECKS):
	$(AM_V_GEN)cd tests && $(MAKE) $(AM_MAKEFLAGS) $@

# See comments in build-aux/git-version-gen.  However, we make .version depend
# on configure so that .version and VERSION/PACKAGE_VERSION stay in sync in the
# working copy (for example, when you run autoreconf && make).  Allowing these
# to fall out of sync appears to have little potential to improve Bison build
# efficiency (even if we were to replace VERSION/PACKAGE_VERSION with .version
# everywhere possible).  On the other hand, it could be harmful.  For example,
# a developer might naively reference .version in a test case while the bison
# executable still compiles with VERSION, and so the test case might fail or
# pass incorrectly.
BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version: configure
	$(AM_V_GEN)echo $(VERSION) > $@-t && mv $@-t $@
dist-hook: gen-ChangeLog
	$(AM_V_GEN)echo $(VERSION) > $(distdir)/.tarball-version

.PHONY: update-b4-copyright update-package-copyright-year
update-b4-copyright:
	$(AM_V_GEN) find data -type f		\
	  | grep -v -E '^data/bison.m4$$'	\
	  | xargs $(build_aux)/$@
	@echo 'warning: src/parse-gram.[hc] may need to be regenerated.'
update-package-copyright-year:
	$(AM_V_GEN)$(build_aux)/$@ configure.ac

gen_start_date = 2012-01-16
.PHONY: gen-ChangeLog
gen-ChangeLog:
	$(AM_V_GEN)if test -d $(srcdir)/.git; then	\
	  cl=$(distdir)/ChangeLog &&			\
	  rm -f $$cl.tmp &&				\
	  $(top_srcdir)/build-aux/gitlog-to-changelog	\
	    --strip-tab					\
	    --strip-cherry-pick				\
	    --no-cluster				\
	    --amend=$(srcdir)/build-aux/git-log-fix	\
	    --srcdir=$(srcdir)				\
	    --since=$(gen_start_date) > $$cl.tmp &&	\
	  mv -f $$cl.tmp $$cl;				\
	fi
