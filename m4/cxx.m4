# -*- Autoconf -*-
# Sanity-test a C++ compiler.
#
# Copyright (C) 2004 Free Software Foundation, Inc.
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

# Written by Paul Eggert <eggert@cs.ucla.edu>.

AC_DEFUN([BISON_TEST_FOR_WORKING_CXX_COMPILER],
[
 AC_CACHE_CHECK([whether $CXX builds executables that work],
   bison_cv_cxx_works,
   [AC_LANG_PUSH([C++])
    AC_RUN_IFELSE(
      [AC_LANG_PROGRAM(
	 [#include <iostream>
	  using namespace std;],
         [std::cerr << "";
          cout << "";])],
      [bison_cv_cxx_works=yes],
      [bison_cv_cxx_works=no],
      [bison_cv_cxx_works=cross])
    AC_LANG_POP([C++])])

 case $bison_cv_cxx_works in
 yes)
    BISON_CXX_WORKS=':';;
 no | cross)
    BISON_CXX_WORKS='exit 77';;
 esac

 AC_SUBST([BISON_CXX_WORKS])
])
