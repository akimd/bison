// Work around an Automake 1.11.2 bug: it asks for the creation of
// y.tab.c and y.tab.h and then renames them as calc++-parser.cc and
// calc++-parser.h, but in the former it does not convert the
// #include "y.tab.h".  We don't want this to show in the
// documentation.
#include "calc++-parser.hh"
