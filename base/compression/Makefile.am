# ***************************************************************************
#    The Base Framework
#    A framework for developing platform independent applications
#
#    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>
#
#    This framework is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
#    For the licensing terms refer to the file 'LICENSE'.
# ***************************************************************************

include $(top_srcdir)/common.am

noinst_LTLIBRARIES = libcompression.la

libcompression_la_SOURCES = \
  BZip2Deflater.cpp \
  BZip2Inflater.cpp \
  Deflater.cpp \
  Inflater.cpp \
  Huffman.cpp \
  ZLibDeflater.cpp \
  ZLibInflater.cpp

sectiondir = $(includedir)/base/compression
section_HEADERS = \
  BZip2Deflater.h \
  BZip2Inflater.h \
  Deflater.h \
  Inflater.h \
  Huffman.h \
  ZLibDeflater.h \
  ZLibInflater.h
