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

noinst_LTLIBRARIES = libconcurrency.la

libconcurrency_la_SOURCES = SpinLock.cpp \
                            ReadWriteLock.cpp \
                            Synchronizeable.cpp \
                            Synchronize.cpp \
                            ExclusiveSynchronize.cpp \
                            SharedSynchronize.cpp \
                            ThreadKey.cpp \
                            Thread.cpp \
                            Semaphore.cpp \
                            MutualExclusion.cpp \
                            Event.cpp \
                            Lock.cpp \
                            AtomicCounter.cpp \
                            ThreadPool.cpp \
                            Process.cpp

sectiondir = $(includedir)/base/concurrency
section_HEADERS = \
  AtomicCounter.h \
  Event.h \
  MutualExclusion.h \
  Runnable.h \
  Semaphore.h \
  Thread.h \
  ThreadKey.h \
  Synchronize.h \
  ExclusiveSynchronize.h \
  SharedSynchronize.h \
  SpinLock.h \
  Synchronizeable.h \
  Lock.h \
  ReadWriteLock.h \
  ThreadPool.h \
  Process.h
