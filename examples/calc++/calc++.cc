#line 7416 "../../doc/bison.texinfo"
#include <iostream>
#include "calc++-driver.hh"

int
main (int argc, const char* argv[])
{
  calcxx_driver driver;
  for (++argv; argv[0]; ++argv)
    if (*argv == std::string ("-p"))
      driver.trace_parsing = true;
    else if (*argv == std::string ("-s"))
      driver.trace_scanning = true;
    else
      {
	driver.parse (*argv);
	std::cout << driver.result << std::endl;
      }
}
