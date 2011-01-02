## Process this file with automake to produce Makefile.in -*-Makefile-*-

## Copyright (C) 2001-2003, 2005-2011 Free Software Foundation, Inc.

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

AM_MAKEINFOFLAGS = --no-split
info_TEXINFOS = bison.texinfo
bison_TEXINFOS = $(srcdir)/cross-options.texi gpl-3.0.texi fdl.texi

CLEANFILES = bison.fns
CLEANDIRS = *.t2d
clean-local:
	rm -rf $(CLEANDIRS)

CROSS_OPTIONS_PL = $(top_srcdir)/build-aux/cross-options.pl
$(srcdir)/cross-options.texi: $(top_srcdir)/src/getargs.c $(CROSS_OPTIONS_PL)
	mv -f $@ $@~ || : >$@~
	-rm -f $@.tmp
	cd $(top_builddir)/src && $(MAKE) $(AM_MAKEFLAGS) bison
	$(top_builddir)/src/bison --help \
	  | perl $(CROSS_OPTIONS_PL) $(top_srcdir)/src/scan-gram.l >$@.tmp
	diff -u $@~ $@.tmp || true
	mv $@.tmp $@
MAINTAINERCLEANFILES = $(srcdir)/cross-options.texi

## ---------- ##
## Ref card.  ##
## ---------- ##

EXTRA_DIST = refcard.tex
CLEANFILES += refcard.dvi refcard.log refcard.ps

refcard.dvi: refcard.tex
	tex refcard.tex

refcard.ps: refcard.dvi


## ----------- ##
## Man Pages.  ##
## ----------- ##

dist_man_MANS = $(srcdir)/bison.1

EXTRA_DIST += $(dist_man_MANS:.1=.x) common.x
MAINTAINERCLEANFILES += $(dist_man_MANS)

# Depend on configure to get version number changes.
common_dep = $(top_srcdir)/configure $(srcdir)/common.x
srcsrcdir = $(top_srcdir)/bin
$(srcdir)/bison.1:      $(common_dep) $(top_srcdir)/src/getargs.c

# Differences to ignore when comparing the man page (the date).
remove_time_stamp = \
  sed 's/^\(\.TH[^"]*"[^"]*"[^"]*\)"[^"]*"/\1/'

MOSTLYCLEANFILES = $(srcdir)/*.t

SUFFIXES = .x .1

PREPATH = $(top_builddir)/src
.x.1:
	@program=`expr "/$*" : '.*/\(.*\)'` &&				\
	save_IFS=$IFS;							\
	IFS=$(PATH_SEPARATOR);						\
	for dir in $(PREPATH); do					\
	  IFS=$save_IFS;						\
	  echo cd $$dir '&&' $(MAKE) $(AM_MAKEFLAGS) $$program &&	\
	  (cd $$dir && $(MAKE) $(AM_MAKEFLAGS) $$program) || exit;	\
	done
	@echo "Updating man page $@"
	PATH="$(PREPATH)$(PATH_SEPARATOR)$$PATH";			     \
	export PATH;							     \
	$(HELP2MAN)							     \
	    --include=$*.x						     \
	    --include=$(srcdir)/common.x				     \
	    --output=$@.t `echo '$*' | sed 's,.*/,,'`
	if $(remove_time_stamp) $@ >$@a.t 2>/dev/null &&		     \
	   $(remove_time_stamp) $@.t | cmp $@a.t - >/dev/null 2>&1; then     \
		touch $@;						     \
	else								     \
		mv $@.t $@;						     \
	fi
	rm -f $@*.t

nodist_man_MANS = yacc.1

## -------------- ##
## Doxygenation.  ##
## -------------- ##

DOXYGEN = doxygen

.PHONY: doc html

doc: html

html-local: Doxyfile
	$(DOXYGEN)

edit = sed -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' \
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' \
	   -e 's,@top_builddir\@,$(top_builddir),g' \
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

EXTRA_DIST += Doxyfile.in
CLEANFILES += Doxyfile
# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
Doxyfile: $(srcdir)/Doxyfile.in
	$(edit) $(srcdir)/Doxyfile.in >Doxyfile

CLEANDIRS += html latex
