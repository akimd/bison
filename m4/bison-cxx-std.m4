# bison-cxx-std.m4 serial 1

# Copyright (C) 2018 Free Software Foundation, # Inc.

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

m4_define([_BISON_CXX_COMPILE_STDCXX_11_snippet],
[#include <algorithm>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

  // C++11
  template <typename T>
  struct check
  {
    static_assert(sizeof(int) <= sizeof(T), "not big enough");
  };

  using right_angle_brackets = check<check<bool>>;

  auto f = std::make_shared<std::string>("shared_ptr");

  int a;
  decltype(a) b;

  typedef check<int> check_type;
  check_type c;
  check_type&& cr = static_cast<check_type&&>(c);

  auto d = a;

  // Some versions of libstdc++ do not support std::set::emplace.
  void foo()
  {
    std::set<int> is;
    is.emplace(42);
  }

  // Clang++ 3.5, for a while, was unable to process properly
  // the for-loop because its variable, r, is a typedef...
  // It failed as follows:
  //
  // error: unexpected ':' in nested name specifier; did you mean '::'?
  //    for (auto r: std::set<int>{1, 2})
  //               ^
  //               ::
  using r = std::set<int>;
  void bar()
  {
    for (int r: std::set<int>{1, 2})
      continue;
  }
])

m4_define([_BISON_CXX_COMPILE_STDCXX_14_snippet],
[  // C++14
  void mismatch()
  {
    using ints = std::vector<int>;
    auto v1 = ints{1, 2, 3};
    auto v2 = ints{1, 2};
    std::mismatch(std::begin(v1), std::end(v1),
                  std::begin(v2), std::end(v2));
  }
])

m4_define([_BISON_CXX_COMPILE_STDCXX_17_snippet],
[  // C++17
  namespace ns1::ns2::ns3 {}

#include <optional>
  auto opt_string = std::optional<std::string>{};
  auto out = std::ostringstream{};
])

m4_define([_BISON_CXX_COMPILE_STDCXX_2A_snippet],
[  // C++2A
])


m4_define([_BISON_CXX_COMPILE_STDCXX_11_testbody],
[AC_LANG_SOURCE([
_BISON_CXX_COMPILE_STDCXX_11_snippet
])])

m4_define([_BISON_CXX_COMPILE_STDCXX_14_testbody],
[AC_LANG_SOURCE([
_BISON_CXX_COMPILE_STDCXX_11_snippet
_BISON_CXX_COMPILE_STDCXX_14_snippet
])])

m4_define([_BISON_CXX_COMPILE_STDCXX_17_testbody],
[AC_LANG_SOURCE([
_BISON_CXX_COMPILE_STDCXX_11_snippet
_BISON_CXX_COMPILE_STDCXX_14_snippet
_BISON_CXX_COMPILE_STDCXX_17_snippet
])])

m4_define([_BISON_CXX_COMPILE_STDCXX_2A_testbody],
[AC_LANG_SOURCE([
_BISON_CXX_COMPILE_STDCXX_11_snippet
_BISON_CXX_COMPILE_STDCXX_14_snippet
_BISON_CXX_COMPILE_STDCXX_17_snippet
_BISON_CXX_COMPILE_STDCXX_2A_snippet
])])



AC_DEFUN([BISON_CXX_COMPILE_STDCXX_11],
[AC_REQUIRE([AC_PROG_CXX])
AC_LANG_PUSH([C++])
for f in '-std=c++11' '-std=c++11 -stdlib=libc++'
do
  AX_CHECK_COMPILE_FLAG([$f],
                        [AC_SUBST([CXX11_CXXFLAGS], [$f]) break],
                        [], [],
                        [_BISON_CXX_COMPILE_STDCXX_11_testbody])
done
AC_LANG_POP([C++])
])


AC_DEFUN([BISON_CXX_COMPILE_STDCXX_14],
[AC_REQUIRE([AC_PROG_CXX])
AC_LANG_PUSH([C++])
for f in '-std=c++14' '-std=c++14 -stdlib=libc++'
do
  AX_CHECK_COMPILE_FLAG([$f],
                        [AC_SUBST([CXX14_CXXFLAGS], [$f]) break],
                        [], [],
                        [_BISON_CXX_COMPILE_STDCXX_14_testbody])
done
AC_LANG_POP([C++])
])


AC_DEFUN([BISON_CXX_COMPILE_STDCXX_17],
[AC_REQUIRE([AC_PROG_CXX])
AC_LANG_PUSH([C++])
for f in '-std=c++17' '-std=c++17 -stdlib=libc++'
do
  AX_CHECK_COMPILE_FLAG([$f],
                        [AC_SUBST([CXX17_CXXFLAGS], [$f]) break],
                        [], [],
                        [_BISON_CXX_COMPILE_STDCXX_17_testbody])
done
AC_LANG_POP([C++])
])


AC_DEFUN([BISON_CXX_COMPILE_STDCXX_2A],
[AC_REQUIRE([AC_PROG_CXX])
AC_LANG_PUSH([C++])
for f in '-std=c++2a' '-std=c++2a -stdlib=libc++'
do
  AX_CHECK_COMPILE_FLAG([$f],
                        [AC_SUBST([CXX2A_CXXFLAGS], [$f]) break],
                        [], [],
                        [_BISON_CXX_COMPILE_STDCXX_2A_testbody])
done
AC_LANG_POP([C++])
])
