#
# Makefile.am for core/scr/src
#

AM_CXXFLAGS = -DY2LOG=\"scr\" -DSUSEVERSION=\"${SUSEVERSION}\"

serversdir = $(execcompdir)/servers

plugin_LTLIBRARIES = libpy2scr.la

libpy2scr_la_SOURCES =				\
	Y2CCSCR.cc				\
	ScriptingAgent.cc ScriptingAgent.h	\
	StdioSCRAgent.cc StdioSCRAgent.h	\
	SCRSubAgent.cc SCRSubAgent.h

libpy2scr_la_LDFLAGS = -version-info 2:0

libpy2scr_la_LIBADD = $(top_builddir)/libscr/src/libscr.la

INCLUDES = ${AGENT_INCLUDES}

pkgincludedir = $(includedir)/scr
# these go to $(pkgincludedir)
pkginclude_HEADERS =		\
	ScriptingAgent.h	\
	SCRSubAgent.h

