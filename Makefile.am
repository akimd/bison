## Process this file with automake to produce Makefile.in -*-Makefile-*-
AUTOMAKE_OPTIONS = 1.5 check-news readme-alpha dist-bzip2

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = config intl po lib src doc m4 tests

EXTRA_DIST = REFERENCES configure.bat OChangeLog Makefile.maint GNUmakefile

DISTCLEANFILES = intl/libintl.h


## ---------------------------- ##
## Customizing Makefile.maint.  ##
## ---------------------------- ##

move_if_change = $(srcdir)/config/move-if-change
prev_version_file = $(srcdir)/config/prev-version.txt
release_archive_dir = releases

# Uploading betas.
hosts = alpha
alpha_host = alpha.gnu.org
alpha_url_dir = gnu/bison

# Files to update automatically.
wget_files = $(srcdir)/config/config.guess $(srcdir)/config/config.sub \
	     $(srcdir)/config/texinfo.tex

cvs_files = $(srcdir)/config/missing
