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

noinst_LTLIBRARIES = libnet.la

libnet_la_SOURCES = \
  ClientSocket.cpp \
  HostNotFound.cpp \
  InetAddress.cpp \
  InetInterface.cpp \
  InetService.cpp \
  MimeType.cpp \
  NetworkException.cpp \
  ServerSocket.cpp \
  ServiceNotFound.cpp \
  Socket.cpp \
  Url.cpp \
  Urn.cpp \
  InetEndPoint.cpp

sectiondir = $(includedir)/base/net
section_HEADERS = \
  Socket.h \
  InetAddress.h \
  HostNotFound.h \
  MimeType.h \
  NetworkException.h \
  InetInterface.h \
  InetService.h \
  ClientSocket.h \
  ServerSocket.h \
  ServiceNotFound.h \
  Url.h \
  Urn.h \
  InetEndPoint.h
