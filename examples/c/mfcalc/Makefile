# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = mfcalc
BISON = bison
XSLTPROC = xsltproc

all: $(BASE)

%.c %.xml %.gv: %.y
	$(BISON) $(BISONFLAGS) --xml --graph=$*.gv -o $*.c $<

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(BASE)
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$<

html: $(BASE).html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

clean:
	rm -f $(BASE) $(BASE).c $(BASE).html $(BASE).xml $(BASE).gv $(BASE).output
