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

noinst_LTLIBRARIES = libsecurity.la

libsecurity_la_SOURCES = \
  AccessControlEntry.cpp \
  AccessControlList.cpp \
  Adler32.cpp \
  Base64.cpp \
  CRC32.cpp \
  Group.cpp \
  MD5Sum.cpp \
  SecurityException.cpp \
  SHA1.cpp \
  SHA256.cpp \
  SHA384.cpp \
  SHA512.cpp \
  User.cpp \
  Trustee.cpp

sectiondir = $(includedir)/base/security
section_HEADERS = \
  AccessControlEntry.h \
  AccessControlList.h \
  Adler32.h \
  Base64.h \
  CRC32.h \
  Group.h \
  MD5Sum.h \
  SecurityException.h \
  SHA1.h \
  SHA256.h \
  SHA384.h \
  SHA512.h \
  User.h \
  Trustee.h
