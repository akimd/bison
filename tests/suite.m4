#!/bin/sh
# Validation suite for Bison.
# Copyright (C) 2000 Free Software Foundation, Inc.

AT_INIT([bison])

# AT_INCLUDE([calc.m4])

AT_SETUP(both)
AT_CHECK([echo "stdout" && echo "stderr" >&2], 0, [], [])
AT_CLEANUP

AT_SETUP(err)
AT_CHECK([echo "stderr" >&2], 0, [], [])
AT_CLEANUP

AT_SETUP(out)
AT_CHECK([echo "stdout"], 0, [], [])
AT_CLEANUP
