# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = bistromathic
BISON = bison
XSLTPROC = xsltproc

# We need to find the headers and libs for readline (and possibly intl).
# You probably need to customize this for your own environment.
CPPFLAGS = -I/opt/local/include
LDFLAGS = -L/opt/local/lib

# Find the translation catalogue for Bison's generated messagess.
BISON_LOCALEDIR = $(shell $(BISON) $(BISON_FLAGS) --print-localedir)
CPPFLAGS += -DENABLE_NLS -DBISON_LOCALEDIR='"$(BISON_LOCALEDIR)"'

LIBS = -lreadline -lm # In some environments, -lintl is needed.

all: $(BASE)

%.c %.h %.xml %.gv: %.y
	$(BISON) $(BISONFLAGS) --defines --xml --graph=$*.gv -o $*.c $<

$(BASE): parse.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

run: $(BASE)
	@echo "Type bistromathic expressions.  Quit with ctrl-d."
	./$<

html: $(BASE).html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =						\
  $(BASE) *.o						\
  parse.[ch] parse.output parse.xml parse.html parse.gv

clean:
	rm -f $(CLEANFILES)
