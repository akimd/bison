#serial 2

dnl These are the prerequisite macros for files in the lib/
dnl directories of Bison.

AC_DEFUN([jm_PREREQ_QUOTEARG],
[
  AC_CHECK_FUNCS(isascii iswprint mbsinit)
  jm_FUNC_MBRTOWC
  AC_CHECK_HEADERS(limits.h stddef.h stdlib.h string.h wchar.h wctype.h)
  AC_HEADER_STDC
  AC_C_BACKSLASH_A
  AC_MBSTATE_T
  AM_C_PROTOTYPES
])
