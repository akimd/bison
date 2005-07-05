all: calc++

calc++:
	flex -ocalc++-scanner.cc calc++-scanner.ll
	bison -o calc++-parser.cc calc++-parser.yy
	$(CC) -o calc++ calc++.cc calc++-driver.cc calc++-parser.cc calc++-scanner.cc

check: all
	./test

clean:
	rm -f *~ *.o *.tab.* input position.hh location.hh stack.hh \
	      calc++-parser.cc calc++-parser.hh \
	      calc++-scanner.cc \
	      calc++

## ------------ ##
## Extracting.  ##
## ------------ ##

EXTRACTED = \
calc++-driver.hh calc++-driver.cc \
calc++-parser.yy \
calc++-scanner.ll \
calc++.cc

doc = ../../doc/bison.texinfo
extexi = gawk -f ../extexi

RECURSIVE_TARGETS += extract

$(EXTRACTED): $(doc) ../extexi
	$(extexi) $(doc) -- $(EXTRACTED)

extract extract-am: $(EXTRACTED)
