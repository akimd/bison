# Configure template for GNU Bison.                   -*-Autoconf-*-
#
# Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
# Foundation, Inc.
#
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

# We have strange test case titles, so we need Autoconf 2.61 or better.
AC_PREREQ(2.61)

AC_INIT([GNU Bison],
        m4_esyscmd([build-aux/git-version-gen .tarball-version]),
        [bug-bison@gnu.org])
AC_DEFINE([PACKAGE_COPYRIGHT_YEAR], [2008],
          [The copyright year for this package])
AC_SUBST([PACKAGE_COPYRIGHT_YEAR], [2008])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# We don't have a file m4/Makefile.am, so we need Automake 1.8 or later.
# We want gnits strictness only when rolling a formal release so that we can,
# for example, run make dist at other times without being required to add a
# bogus NEWS entry.
AM_INIT_AUTOMAKE([1.8 dist-bzip2]
                 m4_bmatch(m4_defn([AC_PACKAGE_VERSION]), [-], [gnu], [gnits]))
AC_CONFIG_HEADERS([lib/config.h:lib/config.hin])

# Checks for the compiler.
AC_PROG_CC_STDC

# Gnulib (early checks).
gl_EARLY

AC_ARG_ENABLE(gcc-warnings,
[  --enable-gcc-warnings   turn on lots of GCC warnings (not recommended)],
[case "${enableval}" in
   yes|no) ;;
   *)      AC_MSG_ERROR([bad value ${enableval} for gcc-warnings option]) ;;
 esac],
	      [enableval=no])
if test "${enableval}" = yes; then
  gl_WARN_ADD([-Werror], [WERROR_CFLAGS])
  AC_SUBST([WERROR_CFLAGS])
  gl_WARN_ADD([-Wall])
  gl_WARN_ADD([-Wextra -Wno-sign-compare])
  gl_WARN_ADD([-Wcast-align])
  gl_WARN_ADD([-Wcast-qual])
  gl_WARN_ADD([-Wformat])
  gl_WARN_ADD([-Wpointer-arith])
  gl_WARN_ADD([-Wwrite-strings])
  AC_SUBST([WARN_CXXFLAGS], [$WARN_CFLAGS])
  # The following warnings are not suitable for C++.
  gl_WARN_ADD([-Wbad-function-cast])
  gl_WARN_ADD([-Wmissing-declarations])
  gl_WARN_ADD([-Wmissing-prototypes])
  gl_WARN_ADD([-Wshadow])
  gl_WARN_ADD([-Wstrict-prototypes])
  AC_SUBST([WARN_CFLAGS])
  AC_DEFINE([lint], 1, [Define to 1 if the compiler is checking for lint.])
fi

BISON_TEST_FOR_WORKING_C_COMPILER

AC_ARG_ENABLE([yacc],
  [AC_HELP_STRING([--disable-yacc],
     [do not build a yacc command or an -ly library])],
  , [enable_yacc=yes])
case $enable_yacc in
yes)
  YACC_SCRIPT=yacc
  YACC_LIBRARY=liby.a;;
*)
  YACC_SCRIPT=
  YACC_LIBRARY=;;
esac
AC_SUBST([YACC_SCRIPT])
AC_SUBST([YACC_LIBRARY])

# Checks for programs.
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB
BISON_PROG_GNU_M4
if test x"$ac_cv_prog_gnu_m4" != xyes; then
  AC_MSG_ERROR([GNU M4 1.4 is required])
fi
AC_DEFINE_UNQUOTED([M4], ["$M4"], [Define to the GNU M4 executable name.])
AM_MISSING_PROG([HELP2MAN], [help2man])
AC_PATH_PROG([XSLTPROC], [xsltproc])
AC_SUBST([XSLTPROC])

# Checks for header files.
AC_CHECK_HEADERS_ONCE([locale.h])

# Checks for compiler characteristics.
AC_C_INLINE

# Gnulib (later checks).  Putting them here rather than right after
# gl_EARLY avoids some redundant checks.
gl_INIT

# Checks for library functions.
AC_CHECK_FUNCS_ONCE([setlocale])
AM_WITH_DMALLOC
BISON_PREREQ_SUBPIPE
BISON_PREREQ_TIMEVAR

# Gettext.
# We've never tested with gettext versions before 0.15, so play it safe.
AM_GNU_GETTEXT([external], [need-formatstring-macros])
AM_GNU_GETTEXT_VERSION([0.15])
BISON_I18N

# Internationalized parsers.
AC_CONFIG_FILES([runtime-po/Makefile.in])
# Autoconf macros for packages using internationalized parsers.
aclocaldir='${datadir}/aclocal'
AC_SUBST([aclocaldir])

# Create the benchmark script.
AC_CONFIG_FILES([etc/bench.pl], [chmod +x etc/bench.pl])

# Initialize the test suite.
AC_CONFIG_TESTDIR(tests)
AC_CONFIG_FILES([tests/Makefile tests/atlocal])
AC_CONFIG_FILES([tests/bison], [chmod +x tests/bison])
AC_CHECK_PROGS([VALGRIND], [valgrind])
AC_PROG_CXX
BISON_TEST_FOR_WORKING_CXX_COMPILER
AM_MISSING_PROG([AUTOM4TE], [autom4te])
# Needed by tests/atlocal.in.
AC_SUBST([GCC])
AC_SUBST([O0CFLAGS],   [`echo $CFLAGS   | sed 's/-O[[0-9]] *//'`])
AC_SUBST([O0CXXFLAGS], [`echo $CXXFLAGS | sed 's/-O[[0-9]] *//'`])

gt_JAVACOMP([1.3])
gt_JAVAEXEC

AC_CONFIG_FILES([Makefile
		 build-aux/Makefile
		 po/Makefile.in
		 data/Makefile
		 etc/Makefile
		 examples/Makefile
		    examples/calc++/Makefile
		 lib/Makefile src/Makefile
		 doc/Makefile
		   doc/yacc.1])
AC_OUTPUT
