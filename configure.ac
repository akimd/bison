# Configure template for GNU Bison.                   -*-Autoconf-*-
#
# Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA

# We need a recent Autoconf to run a recent Autotest.
AC_PREREQ(2.54)

AC_INIT([GNU Bison], [1.875], [bug-bison@gnu.org])
AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([1.7 check-news readme-alpha dist-bzip2])
AC_CONFIG_HEADERS([config.h:config.hin])

# Checks for the compiler.
AC_PROG_CC
# Remove with Automake 1.8.
AC_ISC_POSIX
AC_GNU_SOURCE
AC_MINIX
AM_PROG_CC_STDC
AC_ARG_ENABLE(gcc-warnings,
[  --enable-gcc-warnings   turn on lots of GCC warnings (not recommended)],
[case "${enableval}" in
   yes|no) ;;
   *)      AC_MSG_ERROR([bad value ${enableval} for gcc-warnings option]) ;;
 esac],
              [enableval=no])
if test "${enableval}" = yes; then
  BISON_WARNING(-Werror)
  AC_SUBST([WERROR_CFLAGS], [$WARNING_CFLAGS])
  WARNING_CFLAGS=
  BISON_WARNING(-Wall)
  BISON_WARNING(-W)
  BISON_WARNING(-Wbad-function-cast)
  BISON_WARNING(-Wcast-align)
  BISON_WARNING(-Wcast-qual)
  BISON_WARNING(-Wformat)
  BISON_WARNING(-Wmissing-declarations)
  BISON_WARNING(-Wmissing-prototypes)
  BISON_WARNING(-Wnested-externs)
  BISON_WARNING(-Wshadow)
  BISON_WARNING(-Wstrict-prototypes)
  BISON_WARNING(-Wwrite-strings)
  AC_DEFINE([lint], 1, [Define to 1 if the compiler is checking for lint.])
fi

# Checks for programs.
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB
BISON_PROG_GNU_M4
if test x"$ac_cv_prog_gnu_m4" != xyes; then
  AC_MSG_ERROR([GNU M4 1.4 is required])
fi
AC_DEFINE_UNQUOTED([M4], ["$M4"], [Define to the GNU M4 executable name.])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([ctype.h locale.h memory.h stdlib.h string.h unistd.h])
AC_HEADER_STDBOOL

# Checks for compiler characteristics.
AC_C_INLINE

# Checks for types.
AC_CHECK_TYPES([uintptr_t])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_OBSTACK
AC_FUNC_ERROR_AT_LINE
AC_FUNC_STRNLEN
AC_CHECK_FUNCS(setlocale)
AC_CHECK_DECLS([free, getenv, getopt,
                stpcpy, strchr, strspn, strnlen,
                malloc, memchr, memrchr])
AC_REPLACE_FUNCS(memchr memrchr \
                 strchr stpcpy strrchr strspn strtol strtoul)
AC_FUNC_MALLOC
AC_FUNC_REALLOC
jm_PREREQ_MBSWIDTH
jm_PREREQ_QUOTEARG
jm_FUNC_ARGMATCH
jm_PREREQ_ERROR
AM_WITH_DMALLOC
BISON_PREREQ_SUBPIPE
BISON_PREREQ_TIMEVAR

# Gettext.
AM_GNU_GETTEXT(external, need-ngettext)
AM_GNU_GETTEXT_VERSION(0.11.5)

# Initialize the test suite.
AC_CONFIG_TESTDIR(tests)
AC_CONFIG_FILES([tests/Makefile tests/atlocal])
AC_CONFIG_FILES([tests/bison], [chmod +x tests/bison])
AC_CHECK_PROGS([VALGRIND], [valgrind])
AC_CHECK_PROGS([GXX], [g++])
AM_MISSING_PROG([AUTOM4TE], [autom4te])
# Needed by tests/atlocal.in.
AC_SUBST([GCC])
O0CFLAGS=`echo $CFLAGS | sed 's/-O[0-9] *//'`
AC_SUBST([O0CFLAGS])

AC_CONFIG_FILES([Makefile
                 config/Makefile
                 po/Makefile.in
                 data/Makefile
                 lib/Makefile src/Makefile doc/Makefile
                 m4/Makefile])
AC_OUTPUT
