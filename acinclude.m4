dnl BISON_DEFINE_FILE(VARNAME, FILE)
dnl Defines (with AC_DEFINE) VARNAME to the expansion of the FILE
dnl variable, expanding ${prefix} and such.
dnl Example: BISON_DEFINE_FILE(DATADIR, datadir)
dnl By Alexandre Oliva <oliva@dcc.unicamp.br>
AC_DEFUN(BISON_DEFINE_FILE, [
        ac_expanded=`(
            test "x$prefix" = xNONE && prefix="$ac_default_prefix"
            test "x$exec_prefix" = xNONE && exec_prefix="${prefix}"
            eval echo \""[$]$2"\"
        )`
        AC_DEFINE_UNQUOTED($1, "$ac_expanded")
])
