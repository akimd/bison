# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = bistromathic
BISON = bison
FLEX = flex
XSLTPROC = xsltproc

all: $(BASE)

%.c %.h %.xml %.gv: %.y
	$(BISON) $(BISONFLAGS) --defines --xml --graph=$*.gv -o $*.c $<

%.c: %.l
	$(FLEX) $(FLEXFLAGS) -o$*.c $<

scan.o: parse.h
$(BASE): parse.o scan.o
	$(CC) $(CFLAGS) -o $@ $^

run: $(BASE)
	@echo "Type bistromathic expressions.  Quit with ctrl-d."
	./$<

html: $(BASE).html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =						\
  $(BASE) *.o						\
  parse.[ch] parse.output parse.xml parse.html parse.gv	\
  scan.c

clean:
	rm -f $(CLEANFILES)
