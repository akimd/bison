## ----------------------##
## Prepare for testing.  ##
## ----------------------##

# serial 2

# Single argument says where are built sources to test, relative to the
# built test directory.  Maybe omitted if the same (flat distribution).

AC_DEFUN([AT_CONFIG],
[AT_TESTPATH=ifelse([$1], [], [.], [$1])
AC_SUBST([AT_TESTPATH])
# Try to be compatible with both Autoconf 2.13 and 2.50.
if test -z "$ECHO_C$ECHO_N$ECHO_T"; then
  ECHO_C="$ac_c"
  ECHO_N="$ac_n"
  ECHO_T="$ac_t"
  AC_SUBST([ECHO_C])
  AC_SUBST([ECHO_N])
  AC_SUBST([ECHO_T])
fi
])
