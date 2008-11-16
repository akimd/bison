## Copyright (C) 2001, 2002, 2003, 2005, 2006, 2007, 2008 Free Software
## Foundation, Inc.

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

AM_MAKEINFOFLAGS = --no-split
info_TEXINFOS = doc/bison.texinfo
doc_bison_TEXINFOS =				\
  $(CROSS_OPTIONS_TEXI)				\
  doc/fdl.texi					\
  doc/gpl-3.0.texi

CLEANFILES = doc/bison.fns
CLEANDIRS = doc/*.t2d
clean-local:
	rm -rf $(CLEANDIRS)

CROSS_OPTIONS_PL = $(top_srcdir)/build-aux/cross-options.pl
CROSS_OPTIONS_TEXI = $(top_srcdir)/doc/cross-options.texi
$(CROSS_OPTIONS_TEXI): $(top_srcdir)/src/getargs.c $(CROSS_OPTIONS_PL)
	-rm -f $@ $@.tmp
	$(MAKE) $(AM_MAKEFLAGS) src/bison$(EXEEXT)
	$(top_builddir)/src/bison --help | perl $(CROSS_OPTIONS_PL) >$@.tmp
	mv $@.tmp $@
MAINTAINERCLEANFILES = $(CROSS_OPTIONS_TEXI)

## ---------- ##
## Ref card.  ##
## ---------- ##

EXTRA_DIST += doc/refcard.tex
CLEANFILES += doc/refcard.dvi doc/refcard.log doc/refcard.ps

doc/refcard.dvi: doc/refcard.tex
	cd doc && tex refcard.tex

doc/refcard.ps: doc/refcard.dvi


## ----------- ##
## Man Pages.  ##
## ----------- ##

dist_man_MANS = $(top_srcdir)/doc/bison.1

EXTRA_DIST += $(dist_man_MANS:.1=.x) doc/common.x
MAINTAINERCLEANFILES += $(dist_man_MANS)

# Depend on configure to get version number changes.
common_dep = $(top_srcdir)/configure $(top_srcdir)/doc/common.x
$(top_srcdir)/doc/bison.1:      $(common_dep) $(top_srcdir)/src/getargs.c

# Differences to ignore when comparing the man page (the date).
remove_time_stamp = \
  sed 's/^\(\.TH[^"]*"[^"]*"[^"]*\)"[^"]*"/\1/'

MOSTLYCLEANFILES += $(top_srcdir)/doc/*.t

SUFFIXES = .x .1

PREPATH = src
.x.1:
	@program=`expr "/$*" : '.*/\(.*\)'` &&				 \
	save_IFS=$IFS;							 \
	IFS=$(PATH_SEPARATOR);						 \
	for dir in $(PREPATH); do					 \
	  IFS=$save_IFS;						 \
	  echo $(MAKE) $(AM_MAKEFLAGS) $$dir/$$program;			 \
	  $(MAKE) $(AM_MAKEFLAGS) $$dir/$$program || exit;		 \
	done
	@echo "Updating man page $@"
	PATH="$(top_builddir)/$(PREPATH)$(PATH_SEPARATOR)$$PATH";	 \
	export PATH;							 \
	$(HELP2MAN)							 \
	    --include=$*.x						 \
	    --include=$(top_srcdir)/doc/common.x			 \
	    --output=$@.t `echo '$*' | sed 's,.*/,,'`
	if $(remove_time_stamp) $@ >$@a.t 2>/dev/null &&		 \
	   $(remove_time_stamp) $@.t | cmp $@a.t - >/dev/null 2>&1; then \
		touch $@;						 \
	else								 \
		mv $@.t $@;						 \
	fi
	rm -f $@*.t

nodist_man_MANS = doc/yacc.1

## -------------- ##
## Doxygenation.  ##
## -------------- ##

DOXYGEN = doxygen

.PHONY: doc html

doc: html

html-local: doc/Doxyfile
	cd doc && $(DOXYGEN)

edit = sed -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' \
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' \
	   -e 's,@top_builddir\@,$(top_builddir),g' \
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

EXTRA_DIST += doc/Doxyfile.in
CLEANFILES += doc/Doxyfile
# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
doc/Doxyfile: $(top_srcdir)/doc/Doxyfile.in
	$(edit) $(top_srcdir)/doc/Doxyfile.in >doc/Doxyfile

CLEANDIRS += html latex
