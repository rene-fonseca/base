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
section_HEADERS = symbols.h features.h

nobase_section_HEADERS = \
  arch/architecture.h arch/sse/sse.h arch/sse2/sse2.h arch/sse3/sse3.h arch/x87/x87.h \
  backend/WindowImpl.h \
  compiler/compiler.h compiler/doxygen/doxygen.h compiler/gcc/gcc.h compiler/msc/msc.h compiler/v3mv/exception.h \
  os/os.h os/cygwin/cygwin.h os/solaris/solaris.h os/winnt4/winnt4.h

# temporary fix
EXTRA_DIST = $(nobase_section_HEADERS) \
  os/unix/GLX.h \
  os/unix/GLX.cpp \
  win32/GDI.h \
  win32/GDI.cpp \
  win32/AsyncReadFileContext.h \
  win32/AsyncReadFileContext.cpp \
  win32/AsyncReadStreamContext.h \
  win32/AsyncReadStreamContext.cpp \
  win32/AsyncWriteFileContext.h \
  win32/AsyncWriteFileContext.cpp \
  win32/AsyncWriteStreamContext.h \
  win32/AsyncWriteStreamContext.cpp
