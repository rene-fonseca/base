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

# "features" must be built first
libplatform_la_SOURCES = features.cpp specific.cpp

sectiondir = $(includedir)/base/platforms
section_HEADERS = symbols.h
section_HEADERS += features.h
section_HEADERS += i386-pc-cygwin.h
section_HEADERS += i386-pc-gnu-linux.h
section_HEADERS += i386-pc-w2k.h
section_HEADERS += i386-pc-winnt4.h
section_HEADERS += mips-sgi-irix65.h
section_HEADERS += sparc-sun-solaris.h

# generated during configuration
section_HEADERS += platform.h

nobase_section_HEADERS = arch/architecture.h arch/sse/sse.h arch/sse2/sse2.h arch/sse3/sse3.h arch/x87/x87.h

nobase_section_HEADERS += compiler/compiler.h compiler/doxygen/doxygen.h compiler/gcc/gcc.h compiler/msc/msc.h compiler/v3mv/exception.h

nobase_section_HEADERS += os/os.h os/cygwin/cygwin.h os/solaris/solaris.h

# temporary fix
EXTRA_DIST = $(nobase_section_HEADERS)
