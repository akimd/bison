// Work around an Automake 1.11.2 bug: it asks for the creation of
// y.tab.h and then renames it as calc++-parser.h instead of
// calc++-parser.hh.  We don't want this to show in the documentation.
#include "calc++-parser.h"
