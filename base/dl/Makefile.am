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

noinst_LTLIBRARIES = libdl.la libmodule.la

libdl_la_SOURCES = LinkerException.cpp DynamicLinker.cpp LinkerModule.cpp LinkerManager.cpp

sectiondir = $(includedir)/base/dl
section_HEADERS = LinkerException.h DynamicLinker.h LinkerModule.h LinkerManager.h

libmodule_la_SOURCES = module.cpp
libmodule_la_LDFLAGS = -nostartfiles
