# Sanity test a C compiler.

# Copyright (C) 2006, 2009-2011 Free Software Foundation, Inc.
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

# Written by Paul Eggert.

AC_DEFUN([BISON_TEST_FOR_WORKING_C_COMPILER], [
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
       [[#include <limits.h>
	 int test_array[CHAR_BIT];]])],
    [],
    [AC_MSG_FAILURE([cannot compile a simple C program])])
])
