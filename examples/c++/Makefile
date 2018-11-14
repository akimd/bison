# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BISON = bison
CXX = g++
CXXFLAGS =
PROGS = simple variant variant-11

simple: CXXFLAGS = -std=c++14
variant-11: CXXFLAGS = -std=c++11

all: $(PROGS)

%.cc %.hh: %.yy
	$(BISON) $(BISONFLAGS) -o $*.cc $<

%: %.cc
	$(CXX) $(CXXFLAGS) -o$@ $<

clean:
	rm -f $(PROGS:=.cc) $(PROGS)
