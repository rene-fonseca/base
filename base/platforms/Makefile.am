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

archdir = $(includedir)/base/platforms/arch
arch_HEADERS = arch/architecture.h

compilerdir = $(includedir)/base/platforms/compiler
compiler_HEADERS = compiler/compiler.h

gccdir = $(includedir)/base/platforms/compiler/gcc
gcc_HEADERS = compiler/gcc/gcc.h

mscdir = $(includedir)/base/platforms/compiler/msc
msc_HEADERS = compiler/msc/msc.h

v3mvdir = $(includedir)/base/platforms/compiler/v3mv
v3mv_HEADERS = compiler/v3mv/exception.h

osdir = $(includedir)/base/platforms/os
os_HEADERS = os/os.h

cygwindir = $(includedir)/base/platforms/os/cygwin
cygwin_HEADERS = os/cygwin/cygwin.h

solarisdir = $(includedir)/base/platforms/os/solaris
solaris_HEADERS = os/solaris/solaris.h
