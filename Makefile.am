# -*- Mode: Makefile -*-
#
# Makefile for libmtwist convienience library
#

ANALYZE = clang
ANALYZE_FLAGS = "--analyze"
# Based on COMPILE target
ANALYZE_COMMAND = $(ANALYZE) \
	$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) \
	$(ANALYZE_FLAGS)

noinst_LTLIBRARIES = libmtwist.la
AM_CPPFLAGS = -DMTWIST_CONFIG -I$(top_srcdir)/src

libmtwist_la_SOURCES = \
mt.c \
mtwist.h \
mtwist_internal.h \
seed.c

TESTS=mttest

mttest_SOURCES = test.c
mttest_LDADD = libmtwist.la

EXTRA_PROGRAMS=$(TESTS)

CLEANFILES=$(TESTS) \
*.plist


if MAINTAINER_MODE
# Run Clang static analyzer over sources.
analyze: $(SOURCES)
	@list='$(SOURCES)'; \
	result=0; \
	for file in $$list; do \
	  if echo $$file | grep '\.c$$' >/dev/null 2>&1; then \
	    $(RECHO) "Analyzing $$file"; \
	    $(ANALYZE_COMMAND) $(srcdir)/$$file; \
	    status=$$?; \
	    if test $$status != 0; then \
	      result=1; \
	    fi; \
	  fi; \
	done; \
	set -e; exit $$result
endif
