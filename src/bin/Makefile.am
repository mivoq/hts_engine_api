
EXTRA_DIST = Makefile.mak

INCLUDES = -I$(top_srcdir)/include

bin_PROGRAMS = hts_engine hts_engine_me

hts_engine_SOURCES = hts_engine.c 

hts_engine_LDADD = ../lib/libHTSEngine.a

hts_engine_me_SOURCES = hts_engine_me.c 

hts_engine_me_LDADD = ../lib/libHTSEngine.a

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in
