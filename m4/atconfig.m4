## ----------------------##
## Prepare for testing.  ##
## ----------------------##

# serial 1

# Single argument says where are built sources to test, relative to the
# built test directory.  Maybe omitted if the same (flat distribution).

AC_DEFUN(AT_CONFIG,
[AT_TESTPATH=ifelse($1, , ., $1)
AC_SUBST(AT_TESTPATH)
])
