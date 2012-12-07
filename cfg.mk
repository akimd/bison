# Customize maint.mk                           -*- makefile -*-
# Copyright (C) 2008-2012 Free Software Foundation, Inc.

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

# Update version, then recompile so that tests/bison --version be
# up-to-date, then compile our parser again with our up-to-date bison.
.PHONY: regen
regen: _version
	$(MAKE) $(AM_MAKEFLAGS)
	touch $(srcdir)/src/parse-gram.y
	$(MAKE) $(AM_MAKEFLAGS)

# Used in maint.mk's web-manual rule
manual_title = The Yacc-compatible Parser Generator
gendocs_options_ = -I $(abs_top_srcdir)/doc -I $(abs_top_builddir)/doc

# It's useful to run maintainer-*check* targets during development, but we
# don't want to wait on a recompile because of an update to $(VERSION).  Thus,
# override the _is-dist-target from GNUmakefile so that maintainer-*check*
# targets are filtered out.
_is-dist-target = $(filter-out %clean maintainer-check% maintainer-%-check, \
  $(filter maintainer-% dist% alpha beta major,$(MAKECMDGOALS)))

url_dir_list = \
  ftp://$(gnu_rel_host)/gnu/bison

# Tests not to run as part of "make distcheck".
# Exclude changelog-check here so that there's less churn in ChangeLog
# files -- otherwise, you'd need to have the upcoming version number
# at the top of the file for each `make distcheck' run.
local-checks-to-skip = \
  changelog-check \
  sc_immutable_NEWS			\
  sc_prohibit_always_true_header_tests	\
  sc_prohibit_atoi_atof			\
  sc_prohibit_strcmp

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

# At least for Mac OS X's grep, the order between . and [ in "[^.[]"
# matters:
# $ LC_ALL=fr_FR grep -nE '[^[.]' /dev/null
# $ LC_ALL=C grep -nE '[^[.]' /dev/null
# grep: invalid collating element or class
# $ LC_ALL=fr_FR grep -nE '[^.[]' /dev/null
# $ LC_ALL=C grep -nE '[^.[]' /dev/null
sc_at_parser_check:
	@prohibit='AT_PARSER_CHECK\(\[+[^.[]|AT_CHECK\(\[+\./'		\
	halt='use AT_PARSER_CHECK for and only for generated parsers'	\
	  $(_sc_search_regexp)

exclude = \
  $(foreach a,$(1),$(eval $(subst $$,$$$$,exclude_file_name_regexp--sc_$(a))))
$(call exclude,								\
  bindtextdomain=^lib/main.c$$						\
  program_name=^lib/main.c$$						\
  prohibit_always-defined_macros=^data/yacc.c$$|^djgpp/			\
  prohibit_always-defined_macros+=?|^lib/timevar.c$$			\
  prohibit_always-defined_macros+=?|^src/(parse-gram.c|system.h)$$	\
  prohibit_always-defined_macros+=?|^tests/regression.at$$		\
  prohibit_defined_have_decl_tests=?|^lib/timevar.c$$			\
  prohibit_doubled_word=^tests/named-refs.at$$                          \
  prohibit_magic_number_exit=^doc/bison.texi$$				\
  prohibit_magic_number_exit+=?|^tests/(conflicts|regression).at$$	\
  require_config_h_first=^(lib/yyerror|data/(glr|yacc))\.c$$		\
  space_tab=^tests/(input|c\+\+)\.at$$					\
  trailing_blank=^src/parse-gram.[ch]$$					\
  unmarked_diagnostics=^(djgpp/|doc/bison.texi$$|tests/c\+\+\.at$$)	\
)
