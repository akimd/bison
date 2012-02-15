# Make bison/src.

# Copyright (C) 2001-2012 Free Software Foundation, Inc.

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

AM_CFLAGS = $(WARN_CFLAGS) $(WERROR_CFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)/lib
BISON = ../tests/bison
# Use our own Bison to build the parser.  Of course, you ought to
# keep a sane version of Bison nearby...
YACC = $(BISON) -y
AM_YFLAGS = -d -v --warnings=all,error --report=all

LDADD = ../lib/libbison.a $(LIBINTL)

bin_PROGRAMS = bison
bin_SCRIPTS = $(YACC_SCRIPT)
EXTRA_SCRIPTS = yacc

bison_SOURCES =                                 \
  AnnotationList.c AnnotationList.h             \
  InadequacyList.c InadequacyList.h             \
  LR0.c LR0.h                                   \
  Sbitset.c Sbitset.h                           \
  assoc.c assoc.h                               \
  closure.c closure.h                           \
  complain.c complain.h                         \
  conflicts.c conflicts.h                       \
  derives.c derives.h                           \
  files.c files.h                               \
  flex-scanner.h                                \
  getargs.c getargs.h                           \
  gram.c gram.h                                 \
  lalr.h lalr.c                                 \
  ielr.h ielr.c                                 \
  location.c location.h                         \
  main.c                                        \
  muscle-tab.c muscle-tab.h                     \
  named-ref.c named-ref.h                       \
  nullable.c nullable.h                         \
  output.c output.h                             \
  parse-gram.y                                  \
  print.c print.h                               \
  print_graph.c print_graph.h                   \
  print-xml.c print-xml.h                       \
  reader.c reader.h                             \
  reduce.c reduce.h                             \
  relation.c relation.h                         \
  scan-code.h scan-code-c.c                     \
  scan-gram.h scan-gram-c.c                     \
  scan-skel.h scan-skel-c.c                     \
  state.c state.h                               \
  symlist.c symlist.h                           \
  symtab.c symtab.h                             \
  system.h                                      \
  tables.h tables.c                             \
  uniqstr.c uniqstr.h                           \
  graphviz.c graphviz.h

EXTRA_bison_SOURCES = scan-code.l scan-skel.l scan-gram.l

BUILT_SOURCES =                                 \
  parse-gram.c parse-gram.h                     \
  scan-code.c                                   \
  scan-skel.c                                   \
  scan-gram.c

MOSTLYCLEANFILES = yacc

yacc:
	$(AM_V_GEN)echo '#! /bin/sh' >$@
	$(AM_V_at)echo "exec '$(bindir)/bison' -y "'"$$@"' >>$@
	$(AM_V_at)chmod a+x $@


# The following rule is not designed to be portable,
# and relies on tools that not everyone has.

# Most functions in src/*.c should have static scope.
# Any that don't must be marked with `extern', but `main'
# and `usage' are exceptions.  They're always extern, but
# don't need to be marked.
#
# The second nm|grep checks for file-scope variables with `extern' scope.
sc_tight_scope: $(all_programs)
	@t=exceptions-$$$$;						\
	trap 's=$$?; rm -f $$t; exit $$s' 0 1 2 13 15;			\
	( printf '^main$$\n^usage$$\n';					\
	  grep -h -A1 '^extern .*[^;]$$' $(SOURCES)			\
	    | grep -vE '^(extern |--)' |sed 's/^/^/;s/ .*/$$/' ) > $$t;	\
	if nm -e *.$(OBJEXT)						\
	    | sed -n 's/.* T //p'					\
	    | grep -Ev -f $$t; then					\
	  echo 'the above functions should have static scope' 1>&2;	\
	  exit 1;							\
	fi;								\
	( printf '^program_name$$\n';					\
	  sed -n 's/^extern .*[* ]\([a-zA-Z_][a-zA-Z_0-9]*\);$$/^\1$$/p' \
	    $$(ls $(SOURCES) | grep '\.h$$') /dev/null) > $$t;		\
	if nm -e *.$(OBJEXT)						\
	    | sed -n 's/.* [BD] //p'					\
	    | grep -Ev -f $$t; then					\
	  echo 'the above variables should have static scope' 1>&2;	\
	  exit 1;							\
	fi
