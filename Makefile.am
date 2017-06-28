DIST_SUBDIRS = m4 t

# EXTRA_DIST = config misc

SUBDIRS = . t

ACLOCAL_AMFLAGS = -I m4

noinst_LTLIBRARIES = libchronogogo.la

bin_PROGRAMS = chronogogo

libchronogogo_la_SOURCES = \
	colour.h colour.c

libchronogogo_la_LDFLAGS = -avoid-version -static

chronogogo_SOURCES = chronogogo.c

chronogogo_LDADD = libchronogogo.la

test: all
	cd t && $(MAKE) test
