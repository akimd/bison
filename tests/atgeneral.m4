divert(-1)						-*- shell-script -*-
# `m4' macros used in building test suites.
# Copyright (C) 2000 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# This script is part of Autotest.  Unlimited permission to copy,
# distribute and modify the testing scripts that are the output of
# that Autotest script is given.  You need not follow the terms of the
# GNU General Public License when using or distributing such scripts,
# even though portions of the text of Autotest appear in them.  The
# GNU General Public License (GPL) does govern all other use of the
# material that constitutes the Autotest.
#
# Certain portions of the Autotest source text are designed to be
# copied (in certain cases, depending on the input) into the output of
# Autotest.  We call these the "data" portions.  The rest of the
# Autotest source text consists of comments plus executable code that
# decides which of the data portions to output in any given case.  We
# call these comments and executable code the "non-data" portions.
# Autotest never copies any of the non-data portions into its output.
#
# This special exception to the GPL applies to versions of Autotest
# released by the Free Software Foundation.  When you make and
# distribute a modified version of Autotest, you may extend this
# special exception to the GPL to apply to your modified version as
# well, *unless* your modified version has the potential to copy into
# its output some of the text that was the non-data portion of the
# version that you started with.  (In other words, unless your change
# moves or copies text from the non-data portions to the data
# portions.)  If your modification has such potential, you must delete
# any notice of this special exception to the GPL from your modified
# version.

changequote([, ])

define(AT_DEFINE, defn([define]))
define(AT_EVAL, defn([eval]))
define(AT_FORMAT, defn([format]))
define(AT_INCLUDE, defn([include]))
define(AT_SHIFT, defn([shift]))
define(AT_UNDEFINE, defn([undefine]))

undefine([define])
undefine([eval])
undefine([format])
undefine([include])
undefine([shift])
undefine([undefine])

# AT_CASE(SWITCH, VAL1, IF-VAL1, VAL2, IF-VAL2, ..., DEFAULT)

# m4 equivalent of
# switch (SWITCH)
# {
#   case VAL1:
#     IF-VAL1;
#     break;
#   case VAL2:
#     IF-VAL2;
#     break;
#   ...
#   default:
#     DEFAULT;
#     break;
# }.
# All the values are optional, and the macro is robust to active
# symbols properly quoted.
AT_DEFINE(AT_CASE,
[ifelse([$#], 0, [],
	[$#], 1, [],
	[$#], 2, [$2],
        [$1], [$2], [$3],
        [AT_CASE([$1], AT_SHIFT(AT_SHIFT(AT_SHIFT($@))))])])


# Use of diversions:
#  0 - overall initialization; for each test group: skipping and cleanups;
#  1 - for each test group: proper code, to reinsert between cleanups;
#  2 - overall wrap up: generation of debugging scripts and statistics.


# AT_LINE

# Return the current file sans directory, a colon, and the current line.

AT_DEFINE(AT_LINE,
[patsubst(__file__, ^.*/\(.*\), \1):__line__])

# AT_INIT(PROGRAM)

# Begin testing suite, using PROGRAM to check version.  The search path
# should be already preset so the proper executable will be selected.

AT_DEFINE(AT_INIT,
[AT_DEFINE(AT_ordinal, 0)
. ./atconfig
# Snippet (3

at_usage="Usage: [$]0 [OPTION]...

  -e  Abort the full suite and inhibit normal clean up if a test fails
  -n  Do not redirect stdout and stderr and do not test their contents
  -s  Inhibit verbosity while generating or executing debugging scripts
  -v  Force more detailed output, default for debugging scripts unless -s
  -x  Have the shell to trace command execution; also implies option -n"

while test [$][#] -gt 0; do
  case "[$]1" in
    --help) echo "$at_usage"; exit 0 ;;
    --version) echo "[$]0 ($at_package) $at_version"; exit 0 ;;
    -e) at_stop_on_error=1; shift ;;
    -n) at_no_redirs=1; shift ;;
    -s) at_verbose=; at_silent=1; shift ;;
    -v) at_verbose=1; at_silent=; shift ;;
    -x) at_traceon='set -vx'; at_traceoff='set +vx'; at_no_redirs=1; shift ;;
    *) echo 1>&2 "Try \`[$]0 --help' for more information."; exit 1 ;;
  esac
done


# To check whether a test succeeded or not, we compare an expected
# output with a reference.  In the testing suite, we just need `cmp'
# but in debugging scripts, we want more information, so we prefer
# `diff -u'.  Nonetheless we will use `diff' only, because in DOS
# environments, `diff' considers that two files are equal included
# when there are only differences on the coding of new lines. `cmp'
# does not.
#
# Finally, not all the `diff' support `-u', and some, like Tru64, even
# refuse to `diff' /dev/null.
: >empty

if diff -u empty empty >/dev/null 2>&1; then
  at_diff='diff -u'
else
  at_diff='diff'
fi



# Each generated debugging script, containing a single test group, cleans
# up files at the beginning only, not at the end.  This is so we can repeat
# the script many times and browse left over files.  To cope with such left
# over files, the full test suite cleans up both before and after test groups.
# Snippet )3

if test -n "`$1 --version | sed -n s/$at_package.*$at_version/OK/p`"; then
  at_banner="Testing suite for $at_package, version $at_version"
  at_dashes=`echo $at_banner | sed s/./=/g`
  echo "$at_dashes"
  echo "$at_banner"
  echo "$at_dashes"
else
  echo '======================================================='
  echo 'ERROR: Not using the proper version, no tests performed'
  echo '======================================================='
  exit 1
fi

# Remove any debugging script resulting from a previous run.
rm -f debug-*.sh

at_failed_list=
at_ignore_count=0
divert(2)[]dnl

# Wrap up the testing suite with summary statistics.

rm -f at-check-line
at_fail_count=0
if test -z "$at_failed_list"; then
  if test "$at_ignore_count" = 0; then
    at_banner="All $at_test_count tests were successful"
  else
    at_banner="All $at_test_count tests were successful ($at_ignore_count ignored)"
  fi
else
  echo
  echo $at_n "Writing \`debug-NN.sh' scripts, NN =$at_c"
  for at_group in $at_failed_list; do
    echo $at_n " $at_group$at_c"
    ( echo '#!/bin/sh'
      sed -n '/^[#] Snippet (1/,/^[#] Snippet )1/p' atconfig
      test -z "$at_silent" && echo 'at_verbose=1'
      sed -n '/^[#] Snippet (2/,/^[#] Snippet )2/p' atconfig
      sed -n "/^[#] Snippet (3/,/^[#] Snippet )3/p" [$]0
      sed -n "/^[#] Snippet (c$at_group(/,/^[#] Snippet )c$at_group)/p" [$]0
      at_desc="`sed -n \
        '/^[#] Snippet (d'$at_group'(/,/^[#] Snippet )d'$at_group')/p' [$]0 \
        | sed -n '2s/^[#] //p'`"
      echo 'if test -n "$at_verbose"; then'
      echo '  at_banner="[$]0: '$at_desc'"'
      echo '  at_dashes=`echo $at_banner | sed s/./=/g`'
      echo '  echo'
      echo '  echo "$at_dashes"'
      echo '  echo "$at_banner"'
      echo '  echo "$at_dashes"'
      echo 'fi'
      echo
      sed -n "/^[#] Snippet (d$at_group(/,/^[#] Snippet )d$at_group)/p" [$]0
      sed -n "/^[#] Snippet (s$at_group(/,/^[#] Snippet )s$at_group)/p" [$]0
      echo 'exit 0'
    ) | grep -v '^[#] Snippet' > debug-$at_group.sh
    chmod +x debug-$at_group.sh
    at_fail_count=`expr $at_fail_count + 1`
  done
  echo ', done'
  if test -n "$at_stop_on_error"; then
    at_banner='ERROR: One of the tests failed, inhibiting subsequent tests'
  else
    at_banner="ERROR: Suite unsuccessful, $at_fail_count of $at_test_count tests failed"
  fi
fi
at_dashes=`echo $at_banner | sed s/./=/g`
echo
echo "$at_dashes"
echo "$at_banner"
echo "$at_dashes"

if test -n "$at_failed_list"; then
  if test -z "$at_silent"; then
    echo
    echo 'When reporting failed tests to maintainers, do not merely list test'
    echo 'numbers, as the numbering changes between releases and pretests.'
    echo 'Be careful to give at least all the information you got about them.'
    echo 'You may investigate any problem if you feel able to do so, in which'
    echo 'case the generated debugging scripts provide good starting points.'
    echo "Go on and modify them at will.  \`./debug-NN --help' gives usage"
    echo 'information.  Now, failed tests will be executed again, verbosely.'
    for at_group in $at_failed_list; do
      ./debug-$at_group.sh
    done
  fi
  exit 1
fi

exit 0
divert[]dnl
])

# AT_SETUP(DESCRIPTION)

# Start a group of related tests, all to be executed in the same subshell.
# The group is testing what DESCRIPTION says.

AT_DEFINE(AT_SETUP,
[AT_DEFINE([AT_ordinal], AT_EVAL(AT_ordinal + 1))
pushdef([AT_group_description], [$1])
pushdef([AT_data_files], )
pushdef([AT_data_expout], )
pushdef([AT_data_experr], )
if test -z "$at_stop_on_error" || test -z "$at_failed_list"; then
divert(1)[]dnl
  echo AT_LINE > at-check-line
  if test -n "$at_verbose"; then
    echo 'testing AT_group_description'
    echo $at_n "     $at_c"
  fi
  echo $at_n "substr(AT_ordinal. $srcdir/AT_LINE                            , 0, 30)[]$at_c"
  if test -z "$at_skip_mode"; then
    (
[#] Snippet (d[]AT_ordinal[](
[#] Testing AT_group_description
[#] Snippet )d[]AT_ordinal[])
[#] Snippet (s[]AT_ordinal[](
[#] starting from `AT_LINE'.
$at_traceon
])


# AT_CLEANUP(FILES)

# Complete a group of related tests, recursively remove those FILES
# created within the test.  There is no need to list stdout, stderr,
# nor files created with AT_DATA.

AT_DEFINE(AT_CLEANUP,
$at_traceoff
[[#] Snippet )s[]AT_ordinal[])
    )
    case [$]? in
      0) echo ok
	 ;;
      77) echo "ignored near \``cat at-check-line`'"
	  at_ignore_count=`expr $at_ignore_count + 1`
	  ;;
      *) echo "FAILED near \``cat at-check-line`'"
	 at_failed_list="$at_failed_list AT_ordinal"
	 ;;
    esac
  else
     echo 'ignored (skipped)'
     at_ignore_count=`expr $at_ignore_count + 1`
  fi
  at_test_count=AT_ordinal
  if test -z "$at_stop_on_error" || test -z "$at_failed_list"; then
divert(0)[]dnl
[#] Snippet (c[]AT_ordinal[](

rm ifelse([AT_data_files$1], , [-f], [-rf[]AT_data_files[]ifelse($1, , , [ $1])]) stdout stderr[]AT_data_expout[]AT_data_experr
[#] Snippet )c[]AT_ordinal[])
undivert(1)[]dnl
    rm ifelse([AT_data_files$1], , [-f], [-rf[]AT_data_files[]ifelse($1, , , [ $1])]) stdout stderr[]AT_data_expout[]AT_data_experr
  fi
fi
popdef([AT_data_experr])
popdef([AT_data_expout])
popdef([AT_data_files])
popdef([AT_group_description])])


# AT_DATA(FILE, CONTENTS)

# Initialize an input data FILE with given CONTENTS, which should end with
# an end of line.
# This macro is not robust to active symbols in CONTENTS *on purpose*.
# If you don't want CONTENT to be evaluated, quote it twice.

AT_DEFINE(AT_DATA,
[AT_DEFINE([AT_data_files], AT_data_files[ ]$1)
cat > $1 <<'EOF'
$2[]EOF
])


# AT_CHECK(COMMANDS, [STATUS], STDOUT, STDERR)

# Execute a test by performing given shell COMMANDS.  These commands
# should normally exit with STATUS, while producing expected STDOUT and
# STDERR contents.  The special word `expout' for STDOUT means that file
# `expout' contents has been set to the expected stdout.  The special word
# `experr' for STDERR means that file `experr' contents has been set to
# the expected stderr.
# STATUS is not checked if it is empty.
# STDOUT and STDERR can be the special value `ignore', in which case
# their content is not checked.

AT_DEFINE(AT_CHECK,
[$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/AT_LINE: testing..."
echo AT_LINE > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
$1
ifelse([$2],,,
[at_status=$?
if test $at_status != $2; then
dnl Maybe there was an important message to read before it died.
  test -n "$at_verbose" && test -z "$at_no_redirs" && cat stderr >&6
dnl Exit with the same code, at least to preserve 77.
  exit $at_status
fi
])dnl
$at_traceoff
if test -z "$at_no_redirs"; then
  exec 1>&5 2>&6
  AT_CASE([$3],
          ignore, [test -n "$at_verbose" && cat stdout;:],
          expout, [AT_DEFINE([AT_data_expout], [ expout])dnl
$at_diff expout stdout || exit 1],
          [], [$at_diff empty stdout || exit 1],
          [echo $at_n "patsubst([$3], [\([\"`$]\)], \\\1)$at_c" | $at_diff - stdout || exit 1])
  AT_CASE([$4],
          ignore, [test -n "$at_verbose" && cat stderr;:],
          experr, [AT_DEFINE([AT_data_experr], [ experr])dnl
$at_diff experr stderr || exit 1],
          [], [$at_diff empty stderr || exit 1],
          [echo $at_n "patsubst([$4], [\([\"`$]\)], \\\1)$at_c" | $at_diff - stderr || exit 1])
fi
$at_traceon
])

divert(0)dnl
