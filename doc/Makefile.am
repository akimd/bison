## Process this file with automake to produce Makefile.in -*-Makefile-*-
## Copyright (C) 2001, 2002, 2003, 2005, 2006 Free Software Foundation, Inc.

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
man_MANS = bison.1
bison_TEXINFOS = gpl-3.0.texi fdl.texi

EXTRA_DIST = bison.1 refcard.tex

CLEANFILES = refcard.dvi refcard.log refcard.ps bison.fns
CLEANDIRS = *.t2d
clean-local:
	rm -rf $(CLEANDIRS)

refcard.dvi: refcard.tex
	tex refcard.tex

refcard.ps: refcard.dvi



## -------------- ##
## Doxygenation.  ##
## -------------- ##

DOXYGEN = doxygen

.PHONY: doc html

doc: html

html-local: Doxyfile
	$(DOXYGEN)

edit = sed -e "s|@ID@|$$Id|" \
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' \
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' \
	   -e 's,@top_builddir\@,$(top_builddir),g' \
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

EXTRA_DIST += Doxyfile.in
CLEANFILES += Doxyfile
# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
# Moreover, this file is updated whenever ChangeLog is touched: using
# sed instead of configure saves us a lot of time.
Doxyfile: $(top_srcdir)/ChangeLog $(srcdir)/Doxyfile.in
	Id=`grep '^\$$Id' $(top_srcdir)/ChangeLog`; \
	$(edit) $(srcdir)/Doxyfile.in >Doxyfile

CLEANDIRS += html latex
