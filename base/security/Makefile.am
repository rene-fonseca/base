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

libsecurity_la_SOURCES = AccessControlEntry.cpp AccessControlList.cpp CRC32.cpp MD5Sum.cpp SHA1.cpp Base64.cpp Group.cpp User.cpp

sectiondir = $(includedir)/base/security
section_HEADERS = AccessControlEntry.h AccessControlList.h CRC32.h MD5Sum.h SHA1.h Base64.h Group.h User.h
