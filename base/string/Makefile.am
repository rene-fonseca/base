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

noinst_LTLIBRARIES = libstring.la

libstring_la_SOURCES = ANSIEscapeSequence.cpp \
                       ASCIITraits.cpp \
                       CharacterSet.cpp \
                       FormatInputStream.cpp \
                       FormatOutputStream.cpp \
                       InvalidFormat.cpp \
                       Locale.cpp \
                       RegExp.cpp \
                       String.cpp \
                       StringInputStream.cpp \
                       StringOutputStream.cpp \
                       WideString.cpp

sectiondir = $(includedir)/base/string
section_HEADERS = ANSIEscapeSequence.h \
                  ASCIITraits.h \
                  FormatInputStream.h \
                  FormatOutputStream.h \
                  InvalidFormat.h \
                  Locale.h \
                  MultibyteException.h \
                  RegExp.h \
                  String.h \
                  StringException.h \
                  StringInputStream.h \
                  StringOutputStream.h \
                  WideStringException.h \
                  WideString.h

EXTRA_DIST = unicode/UnicodeFolding.cpp \
             unicode/UnicodeLookup.cpp
