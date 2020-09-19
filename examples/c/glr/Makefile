# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = calc
BISON = bison
XSLTPROC = xsltproc

all: $(BASE)

%.c %.h %.xml %.gv: %.y
	$(BISON) $(BISONFLAGS) --defines --xml --graph=$*.gv -o $*.c $<

$(BASE): $(BASE).o
	$(CC) $(CFLAGS) -o $@ $^

run: $(BASE)
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$<

html: $(BASE).html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =									\
  $(BASE) *.o $(BASE).[ch] $(BASE).output $(BASE).xml $(BASE).html $(BASE).gv

clean:
	rm -f $(CLEANFILES)
