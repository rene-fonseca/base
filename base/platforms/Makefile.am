# ***************************************************************************
#    The Base Framework
#    A framework for developing platform independent applications
#
#    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>
#
#    This framework is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
#    For the licensing terms refer to the file 'LICENSE'.
# ***************************************************************************

include $(top_srcdir)/common.am

noinst_LTLIBRARIES = libplatform.la

libplatform_la_SOURCES = specific.cpp

sectiondir = $(includedir)/base/platforms
section_HEADERS = i386-pc-cygwin.h i386-pc-gnu-linux.h i386-pc-w2k.h i386-pc-winnt4.h mips-sgi-irix65.h sparc-sun-solaris.h symbols.h

# generated during configuration
section_HEADERS += platform.h
