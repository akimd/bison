## Process this file with automake to produce Makefile.in -*-Makefile-*-

info_TEXINFOS = bison.texinfo
man_MANS = bison.1
bison_TEXINFOS = gpl.texi fdl.texi

EXTRA_DIST = FAQ bison.1 bison.rnh refcard.tex

CLEANFILES = refcard.dvi refcard.log refcard.ps

refcard.dvi: refcard.tex
	tex refcard.tex

refcard.ps: refcard.dvi
