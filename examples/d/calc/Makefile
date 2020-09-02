# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BISON = bison
DC = dmd
XSLTPROC = xsltproc

all: calc

%.d %.xml %.gv: %.y
	$(BISON) $(BISONFLAGS) --xml --graph=$*.gv -o $*.d $<

%: %.d
	$(DC) $(DCFLAGS) $<

run: calc
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$<

html: calc.html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

clean:
	rm -f calc calc.d calc.xml calc.gv calc.html *.o
