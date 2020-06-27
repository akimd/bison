This file attempts to describe the rules to use when hacking Bison.
Don't put this file into the distribution.

Everything related to the development of Bison is on Savannah:
http://savannah.gnu.org/projects/bison/.


Administrivia
=============

## If you incorporate a change from somebody on the net:
First, if it is a large change, you must make sure they have signed the
appropriate paperwork.  Second, be sure to add their name and email address
to THANKS.

## If a change fixes a test, mention the test in the commit message.

## Bug reports
If somebody reports a new bug, mention his name in the commit message and in
the test case you write.  Put him into THANKS.

The correct response to most actual bugs is to write a new test case which
demonstrates the bug.  Then fix the bug, re-run the test suite, and check
everything in.



Hacking
=======

## Visible Changes
Which include serious bug fixes, must be mentioned in NEWS.

## Translations
Only user visible strings are to be translated: error messages, bits of the
.output file etc.  This excludes impossible error messages (comparable to
assert/abort), and all the --trace output which is meant for the maintainers
only.

## Vocabulary
Use "nonterminal", not "variable" or "non-terminal" or "non terminal".
Abbreviated as "nterm".

## Syntax highlighting
It's quite nice to be in C++ mode when editing lalr1.cc for instance.
However tools such as Emacs will be fooled by the fact that braces and
parens do not nest, as in `[[}]]`.  As a consequence you might be misguided
by its visual pairing to parens.  The m4-mode is safer.  Unfortunately the
m4-mode is also fooled by `#` which is sees as a comment, stops pairing with
parens/brackets that are inside...

## Coding Style
Do not add horizontal tab characters to any file in Bison's repository
except where required.  For example, do not use tabs to format C code.
However, make files, ChangeLog, and some regular expressions require tabs.
Also, test cases might need to contain tabs to check that Bison properly
processes tabs in its input.

Prefer "res" as the name of the local variable that will be "return"ed by
the function.

### Bison
Follow the GNU Coding Standards.

Don't reinvent the wheel: we use gnulib, which features many components.
Actually, Bison has legacy code that we should replace with gnulib modules
(e.g., many ad hoc implementations of lists).

#### Includes
The `#include` directives follow an order:
- first section for *.c files is `<config.h>`.  Don't include it in header
  files
- then, for *.c files, the corresponding *.h file
- then possibly the `"system.h"` header
- then the system headers.
  Consider headers from `lib/` like system headers (i.e., `#include
  <verify.h>`, not `#include "verify.h"`).
- then headers from src/ with double quotes (`#include "getargs.h"`).

Keep headers sorted alphabetically in each section.

See also the [Header
files](https://www.gnu.org/software/gnulib/manual/html_node/Header-files.html)
and the [Implementation
files](https://www.gnu.org/software/gnulib/manual/html_node/Implementation-files.html#Implementation-files)
nodes of the gnulib documentation.

Some source files are in the build tree (e.g., `src/scan-gram.c` made from
`src/scan-gram.l`).  For them to find the headers from `src/`, we actually
use `#include "src/getargs.h"` instead of `#include "getargs.h"`---that
saves us from additional `-I` flags.

### Skeletons
We try to use the "typical" coding style for each language.

#### CPP
We indent the CPP directives this way:

```
#if FOO
# if BAR
#  define BAZ
# endif
#endif
```

Don't indent with leading spaces in the skeletons (it's OK in the grammar
files though, e.g., in `%code {...}` blocks).

On occasions, use `cppi -c` to see where we stand.  We don't aim at full
correctness: depending `-d`, some bits can be in the *.c file, or the *.h
file within the double-inclusion cpp-guards.  In that case, favor the case
of the *.h file, but don't waste time on this.

Don't hesitate to leave a comment on the `#endif` (e.g., `#endif /* FOO
*/`), especially for long blocks.

There is no consistency on `! defined` vs. `!defined`.  The day gnulib
decides, we'll follow them.

#### C/C++
Follow the GNU Coding Standards.

The `glr.c` skeleton was implemented with `camlCase`.  We are migrating it
to `snake_case`.  Because we are standardizing the code, it is currently
inconsistent.

Use `YYFOO` and `yyfoo` for entities that are exposed to the user.  They are
part of our contract with the users wrt backward compatibility.

Use `YY_FOO` and `yy_foo` for private matters.  Users should not use them,
we are free to change them without fear of backward compatibility issues.

Use `*_t` for types, especially for `yy*_t` in which case we shouldn't worry
about the C standard introducing such a name.

#### C++
Follow the C++ Core Guidelines
(http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).  The Google
ones may be interesting too
(https://google.github.io/styleguide/cppguide.html).

Our enumerators, such as the kinds (symbol and token kinds), should be lower
case, but it was too late to follow that track for token kinds, and symbol
kind enumerators are made to be consistent with them.

Use `*_type` for type aliases.  Use `foo_get()` and `foo_set(v)` for
accessors, or simply `foo()` and `foo(v)`.

Use the `yy` prefix for private stuff, but there's no need for it in the
public API.  The `yy` prefix is already taken care of via the namespace.

#### Java
We follow https://www.oracle.com/technetwork/java/codeconventions-150003.pdf
and https://google.github.io/styleguide/javaguide.html.  Unfortunately at
some point some GNU Coding Style was installed in Java, but it's an error.
So we should for instance stop putting spaces in function calls.  Because we
are standardizing the code, it is currently inconsistent.

Use a 2-space indentation (Google) rather than 4 (Oracle).

Don't use the "yy" prefix for public members: "getExpectedTokens", not
"yyexpectedTokens" or "yygetExpectedTokens".

## Commit Messages
Imitate the style we use.  Use `git log` to get sources of inspiration.

If the changes have a small impact on Bison's generated parser, embed these
changes in the commit itself.  If the impact is large, first push all the
changes except those about src/parse-gram.[ch], and then another commit
named "regen" which is only about them.

## Debugging
Bison supports tracing of its various steps, via the `--trace` option.
Since it is not meant for the end user, it is not displayed by `bison
--help`, nor is it documented in the manual.  Instead, run `bison
--trace=help`.

## Documentation
Use `@option` for options and options with their argument if they have no
space (e.g., `@option{-Dfoo=bar}`).  However, use `@samp` elsewhere (e.g.,
`@samp{-I foo}`).


Working from the Repository
===========================

These notes intend to help people working on the checked-out sources.  These
requirements do not apply when building from a distribution tarball.

## Requirements

We've opted to keep only the highest-level sources in the repository.  This
eases our maintenance burden, (fewer merges etc.), but imposes more
requirements on anyone wishing to build from the just-checked-out sources.
For example, you have to use the latest stable versions of the maintainer
tools we depend upon, including:

- Autoconf <http://www.gnu.org/software/autoconf/>
- Automake <http://www.gnu.org/software/automake/>
- Flex <http://www.gnu.org/software/flex/>
- Gettext <http://www.gnu.org/software/gettext/>
- Gperf <http://www.gnu.org/software/gperf/>
- Graphviz <http://www.graphviz.org>
- Gzip <http://www.gnu.org/software/gzip/>
- Help2man <http://www.gnu.org/software/help2man/>
- Perl <http://www.cpan.org/>
- Rsync <http://samba.anu.edu.au/rsync/>
- Tar <http://www.gnu.org/software/tar/>
- Texinfo <http://www.gnu.org/software/texinfo/>

Valgrind <http://valgrind.org/> is also highly recommended, if it supports
your architecture.

If you're using a GNU/Linux distribution, the easiest way to install the
above packages depends on your system.  The following shell command should
work for Debian-based systems such as Ubuntu:

    sudo apt-get install \
      autoconf automake autopoint flex gperf graphviz help2man texinfo valgrind

Bison is written using Bison grammars, so there are bootstrapping issues.
The bootstrap script attempts to discover when the C code generated from the
grammars is out of date, and to bootstrap with an out-of-date version of the
C code, but the process is not foolproof.  Also, you may run into similar
problems yourself if you modify Bison.

Only building the initial full source tree will be a bit painful.  Later,
after synchronizing from the repository a plain 'make' should be sufficient.
Note, however, that when gnulib is updated, running './bootstrap' again
might be needed.

## First checkout

Obviously, if you are reading these notes, you did manage to check out this
package from the repository.  For the record, you will find all the relevant
information on http://savannah.gnu.org/git/?group=bison.

Bison uses Git submodules: subscriptions to other Git repositories.  In
particular it uses gnulib, the GNU portability library.  To ask Git to
perform the first checkout of the submodules, run

    $ git submodule update --init

The next step is to get other files needed to build, which are extracted
from other source packages:

    $ ./bootstrap

Bootstrapping updates the submodules to the versions registered in the
top-level directory.  To change gnulib, first check out the version you want
in `gnulib`, then commit this change in Bison's repository, and finally run
bootstrap.

If it fails with missing symbols (e.g., `error: possibly undefined macro:
AC_PROG_GNU_M4`), you are likely to have forgotten the submodule
initialization part.  To recover from it, run `git reset --hard HEAD`, and
restart with the submodule initialization.  Otherwise, there you are!  Just

    $ ./configure
    $ make
    $ make check

At this point, there should be no difference between your local copy, and
the master copy:

    $ git diff

should output no difference.

Enjoy!

## Updating

If you have git at version 1.8.2 or later, the command

    $ git submodule update --recursive --remote

will be useful for updating to the latest version of all submodules.

Under earlier versions, use of submodules make things somewhat different
because git does not yet support recursive operations: submodules must be
taken care of explicitly.

### Updating Bison

If you pull a newer version of a branch, say via `git pull`, you might
import requests for updated submodules.  A simple `git diff` will reveal if
the current version of the submodule (i.e., the actual contents of the
gnulib directory) and the current request from the subscriber (i.e., the
reference of the version of gnulib that the Bison repository requests)
differ.  To upgrade the submodules (i.e., to check out the version that is
actually requested by the subscriber, run `git submodule update`.

    $ git pull
    $ git submodule update

### Updating a submodule
To update a submodule, say gnulib, do as follows:

Get the most recent version of the master branch from git.

    $ cd gnulib
    $ git fetch
    $ git checkout -b master --track origin/master

Make sure Bison can live with that version of gnulib.

    $ cd ..
    $ ./bootstrap
    $ make distcheck

Register your changes.

    $ git commit ...

For a suggestion of what gnulib commit might be stable enough for a formal
release, see the ChangeLog in the latest gnulib snapshot at
http://erislabs.net/ianb/projects/gnulib/.

The Autoconf files we use are currently:
- m4/m4.m4
- lib/m4sugar/m4sugar.m4
- lib/m4sugar/foreach.m4

These files don't change very often in Autoconf, so it should be relatively
straight-forward to examine the differences in order to decide whether to
update.

## Troubleshooting

Bison is self-hosted: its parser is generated by Bison.  We don't force
ourselves to use the previous release of Bison, we use the current git
master for several reasons:
- dogfooding: let Bison be its first user
- monitoring: seeing the diff on the generated parsers with git is very
  helpful, as it allows to see easily the impact of the changes on a real
  case parser.

If you are unlucky the generated files, src/parse-gram.[ch], may be older
than their source, src/parse-gram.y.  And your current version of Bison
might not be able to grok it.  In that case, first refresh the generated
files:

    $ touch src/parse-gram.[ch]

Then proceed.

In case you wrecked your current copy of the parser, get back the previous
version, compile bison, then force it to recreate the files:

    $ git checkout HEAD^ src/parse-gram.[ch]
    $ make -C _build
    $ touch src/parse-gram.y
    $ make -C _build


Test Suite
==========

## make check
Consume without moderation.  It is composed of two kinds of tests: the
examples, and the main test suite.

### The Examples
In examples/, there is a number of ready-to-use examples (see
examples/README.md).  These examples have small test suites run by `make
check`.  The test results are in local `*.log` files (e.g.,
`$build/examples/c/calc/calc.log`).

### The Main Test Suite
The main test suite, in tests/, is written on top of GNU Autotest, which is
part of Autoconf.  Run `info autoconf 'Using Autotest'` to read the
documentation, not only about how to write tests, but also where are the
logs, how to read them etc.

The main test suite generates a log for each test (e.g.,
`$build/tests/testsuite.dir/004/testsuite.log` for test #4), and a main log
file in `$build/tests/testsuite.log`.  The latter is meant for end users: it
contains lots of details that should help diagnosing issues, including build
issues.  The per-test logs are more convenient when working locally.

#### TESTSUITEFLAGS
To run just the main test suite, run `make check-local`.

The default is for make check-local to run all tests sequentially.  This can
be very time consuming when checking repeatedly or on slower setups.  This
can be sped up in two ways:

Using -j, in a make-like fashion, for example:

    $ make check-local TESTSUITEFLAGS='-j8'

Actually, when using GNU Make, TESTSUITEFLAGS defaults to the -jN passed to
it, so you may simply run

    $ make check-local -j8

Running only the tests of a certain category, as specified in the AT files
with AT_KEYWORDS([[category]]). Categories include:
- c++, for c++ parsers
- deprec, for tests concerning deprecated constructs.
- glr, for glr parsers
- java, for java parsers
- report, for automaton dumps

To get a list of all the tests (and their keywords for -k), run

    $ ./tests/testsuite -l

To run a specific set of tests, use -k (for "keyword"). For example:

    $ make check-local TESTSUITEFLAGS='-k c++'

Both can be combined.

    $ make check-local TESTSUITEFLAGS='-j8 -k c++'

To rerun the tests that failed:

    $ make recheck -j5

#### Updating the Expectations
Sometimes some changes have a large impact on the test suite (e.g., when we
added the `[-Wother]` part to all the warnings).  Part of the update can be
done with a crude tool: `build-aux/update-test`.

Once you ran the test suite, and therefore have many `testsuite.log` files,
run, from the source tree:

    $ ./build-aux/update-test $build/tests/testsuite.dir/*/testsuite.log

where `$build` would be your build tree.  This will hopefully update most
tests.  Re-run the test suite.  It might be interesting to run `update-test`
again, since some early failures may stop latter tests from being run.  Yet
at some point, you'll have to fix remaining issues by hand...


## Running Java parsers
Use the `javaexec.sh` script.  For instance to run the parser of test case
504:

    $ sh ./_build/javaexec.sh -cp ./_build/tests/testsuite.dir/504 Calc

## Using Sanitizers
Address sanitizer (ASAN) and undefined-behavior sanitizer (UBSAN) are very
useful.  Here's one way to set ASAN up with GCC 10 on Mac Ports

1. Configure with

    $ ./configure -C --enable-gcc-warnings \
        CPPFLAGS='-isystem /opt/local/include' \
        CC='gcc-mp-10 -fsanitize=address' \
        CFLAGS='-ggdb' \
        CXX='g++-mp-10.0 -fsanitize=address' \
        CXXFLAGS='-ggdb' \
        LDFLAGS='-L/opt/local/lib'

2. Compile

3. Generate debug symbols:

    $ dsymutil src/bison

4. Run the tests with leak detection enabled
   (`ASAN_OPTIONS=detect_leaks=1`).  E.g. for counterexamples:

    $ make check-local TESTSUITEFLAGS='-j5 -k cex' ASAN_OPTIONS=detect_leaks=1

## make maintainer-check-valgrind
This target uses valgrind both to check bison, and the generated parsers.

This is not mature on Mac OS X.  First, Valgrind does support the way bison
calls m4, so Valgrind cannot be used to check bison on Mac OS X.

Second, there are many errors that come from the platform itself, not from
bison.  build-aux/darwin11.4.0.valgrind addresses some of them.

Third, valgrind issues warnings such as:

    --99312:0:syswrap- WARNING: Ignoring sigreturn( ..., UC_RESET_ALT_STACK );

which cause the test to fail uselessly.  It is hard to ignore these errors
with a major overhaul of the way instrumentation is performed in the test
suite.  So currently, do not try to run valgrind on Mac OS X.

## Release checks
Try to run the test suite with more severe conditions before a
release:

- Configure the package with --enable-gcc-warnings, so that one checks that
  1. Bison compiles cleanly, 2. the parsers it produces compile cleanly too.

- Maybe build with -DGNULIB_POSIXCHECK, which suggests gnulib modules that
  can fix portability issues.  See if you really want to pay attention to
  its warnings; there's no need to obey blindly to it
  (<http://lists.gnu.org/archive/html/bison-patches/2012-05/msg00057.html>).

- Check with `make syntax-check` if there are issues diagnosed by gnulib.

- run `make maintainer-check` which:
  - runs `valgrind -q bison` to run Bison under Valgrind.
  - runs the parsers under Valgrind.
  - runs the test suite with G++ as C compiler...

- run `make maintainer-check-push`, which runs `make maintainer-check` while
  activating the push implementation and its pull interface wrappers in many
  test cases that were originally written to exercise only the pull
  implementation.  This makes certain the push implementation can perform
  every task the pull implementation can.

- run `make maintainer-check-xml`, which runs `make maintainer-check` while
  checking Bison's XML automaton report for every working grammar passed to
  Bison in the test suite.  The check just diffs the output of Bison's
  included XSLT style sheets with the output of --report=all and --graph.

- running `make maintainer-check-release` takes care of running
  maintainer-check, maintainer-check-push and maintainer-check-xml.

- Change tests/atlocal/CFLAGS to add your preferred options.

- Test with a very recent version of GCC for both C and C++.  Testing with
  older versions that are still in use is nice too.

## gnulib
To run tests on gnulib components (e.g., on bitset):

    cd gnulib
    ./gnulib-tool --test bitset-tests

possibly within a specified environment:

    CC='gcc-mp-8 -fsanitize=undefined' ./gnulib-tool --test bitset-tests

To be able to run the tests several times, and to use symlinks instead of
copies so that one can update the origin gnulib directory and immediately
re-run the tests, run:

    ./gnulib-tool --symlink --create-test --dir=/tmp/gnutest bitset-tests
    cd /tmp/gnutest
    ./configure -C CC='gcc-mp-8 -fsanitize=undefined' CFLAGS='-ggdb'
    make check



Release Procedure
=================

See README-release.

<!--

Copyright (C) 2002-2005, 2007-2015, 2018-2020 Free Software Foundation,
Inc.

This file is part of GNU Bison.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Local Variables:
mode: markdown
fill-column: 76
ispell-dictionary: "american"
End:

LocalWords:  Automake Autoconf Gettext Gzip Rsync Valgrind gnulib submodules
LocalWords:  submodule init cd distcheck ChangeLog valgrind sigreturn sudo
LocalWords:  UC gcc DGNULIB POSIXCHECK xml XSLT glr lalr README po runtime rc
LocalWords:  gnupload gnupg gpg keyserver BDF ncftp filename clearsign cvs dir
LocalWords:  symlinks vti html lt POSIX Cc'ed Graphviz Texinfo autoconf jN
LocalWords:  automake autopoint graphviz texinfo PROG Wother parsers YYFOO
LocalWords:  TESTSUITEFLAGS deprec struct gnulib's getopt config ggdb yyfoo
LocalWords:  bitset fsanitize symlink CFLAGS MERCHANTABILITY ispell wrt YY
LocalWords:  american Administrivia camlCase yy accessors namespace src hoc
LocalWords:  getExpectedTokens yyexpectedTokens yygetExpectedTokens parens
LocalWords:  regen dogfooding Autotest testsuite getargs CPP BAZ endif cppi
LocalWords:  cpp javaexec cp Calc ASAN UBSAN CPPFLAGS isystem CXX cex Gperf
LocalWords:  CXXFLAGS LDFLAGS dsymutil gperf

-->
