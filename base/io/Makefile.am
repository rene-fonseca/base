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

SUBDIRS = async

noinst_LTLIBRARIES = libio.la

libio_la_SOURCES = \
  BrokenStream.cpp \
  Handle.cpp \
  FileDescriptor.cpp \
  TimedOut.cpp \
  AccessDenied.cpp \
  BindException.cpp \
  Stream.cpp \
  InputStream.cpp \
  OutputStream.cpp \
  FileDescriptorInputStream.cpp \
  FileDescriptorOutputStream.cpp \
  FileNotFound.cpp \
  IOException.cpp \
  EndOfFile.cpp \
  EOFException.cpp \
  FilterInputStream.cpp \
  PrimitiveOutputStream.cpp \
  PrimitiveInputStream.cpp \
  File.cpp \
  FileInputStream.cpp \
  FileOutputStream.cpp \
  FilterOutputStream.cpp \
  BufferedOutputStream.cpp \
  BufferedInputStream.cpp \
  Pipe.cpp \
  MappedFile.cpp \
  FileReader.cpp \
  FileRegion.cpp \
  LockableRegion.cpp \
  PushInterface.cpp \
  PullInterface.cpp

sectiondir = $(includedir)/base/io
section_HEADERS = \
  BrokenStream.h \
  Handle.h \
  InputStream.h \
  PrimitiveOutputStream.h \
  FileInputStream.h \
  FilterInputStream.h \
  BufferedInputStream.h \
  FileDescriptor.h \
  Externalizable.h \
  OutputStream.h \
  FilterOutputStream.h \
  BufferedOutputStream.h \
  FileOutputStream.h \
  PrimitiveInputStream.h \
  EOFException.h \
  EndOfFile.h \
  FileNotFound.h \
  IOException.h \
  BindException.h \
  Stream.h \
  FileDescriptorInputStream.h \
  FileDescriptorOutputStream.h \
  AccessDenied.h \
  TimedOut.h \
  Pipe.h \
  File.h \
  MappedFile.h \
  FileReader.h \
  FileException.h \
  PipeException.h \
  FileRegion.h \
  LockableRegion.h \
  PushInterface.h \
  PullInterface.h
