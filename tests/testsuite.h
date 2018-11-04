#include <config.h>
/* We don't need perfect functions for these tests. */
#undef malloc
#undef memcmp
#undef realloc

/* In C++ pre C++11 it is standard practice to use 0 for the null
   pointer.  But GCC -std=c++98 with -Wzero-as-null-pointer-constant
   warns about this.  Warning introduced in GCC 4.7. */
#if defined __cplusplus && __cplusplus < 201103L
# if defined __GNUC__ && ! defined __clang__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
#  pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
# endif
#endif
