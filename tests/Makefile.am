## Process this file with automake to create Makefile.in.

## Makefile for Bison testsuite.
## Copyright (C) 2000 Free Software Foundation, Inc.

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

# Distribute the testsuite since GNU M4 is needed to build it.
noinst_SCRIPTS = testsuite

SUITE = calc.m4

EXTRA_DIST = atgeneral.m4 suite.m4 $(SUITE)

check-local: atconfig testsuite
	$(SHELL) testsuite

testsuite: atgeneral.m4 suite.m4 $(SUITE)
	$(M4) -I $(srcdir) atgeneral.m4 suite.m4 | \
          sed -e 's/[ 	]*$$//' | \
	  sed -e '/^$$/N;/\n$$/D' > $@-tmp
	chmod +x $@-tmp
	mv $@-tmp $@

CLEANFILES = debug-*.sh
DISTCLEANFILES = atconfig
MAINTAINERCLEANFILES = testsuite
