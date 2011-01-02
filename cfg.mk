# Customize maint.mk                           -*- makefile -*-

# Copyright (C) 2008-2011 Free Software Foundation, Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# It's useful to run maintainer-*check* targets during development, but we
# don't want to wait on a recompile because of an update to $(VERSION).  Thus,
# override the _is-dist-target from GNUmakefile so that maintainer-*check*
# targets are filtered out.
_is-dist-target = $(filter-out %clean maintainer-check% maintainer-%-check, \
  $(filter maintainer-% dist% alpha beta major,$(MAKECMDGOALS)))

# Use alpha.gnu.org for alpha and beta releases.
# Use ftp.gnu.org for major releases.
gnu_ftp_host-alpha = alpha.gnu.org
gnu_ftp_host-beta = alpha.gnu.org
gnu_ftp_host-major = ftp.gnu.org
gnu_rel_host = $(gnu_ftp_host-$(RELEASE_TYPE))

url_dir_list = \
  ftp://$(gnu_rel_host)/gnu/bison

# Tests not to run as part of "make distcheck".
# Exclude changelog-check here so that there's less churn in ChangeLog
# files -- otherwise, you'd need to have the upcoming version number
# at the top of the file for each `make distcheck' run.
local-checks-to-skip = changelog-check

# The local directory containing the checked-out copy of gnulib used in
# this release.  Used solely to get a date for the "announcement" target.
gnulib_dir = $(srcdir)/gnulib

bootstrap-tools = autoconf,automake,flex,gettext,gnulib

announcement_Cc_ = \
  bug-bison@gnu.org, help-bison@gnu.org, bison-patches@gnu.org, \
  coordinator@translationproject.org

update-copyright: update-b4-copyright update-package-copyright-year
update-copyright-env = \
  UPDATE_COPYRIGHT_FORCE=1 UPDATE_COPYRIGHT_USE_INTERVALS=1
