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

libconcurrency_la_SOURCES = \
  AtomicCounter.cpp \
  Barrier.cpp \
  ConcurrencyException.cpp \
  Event.cpp \
  ExclusiveSynchronize.cpp \
  Lock.cpp \
  LockException.cpp \
  MutualExclusion.cpp \
  NISpinLock.cpp \
  Once.cpp \
  Process.cpp \
  ReadWriteLock.cpp \
  ReadWriteSpinLock.cpp \
  RecursiveSpinLock.cpp \
  Semaphore.cpp \
  SharedSynchronize.cpp \
  SingleExclusiveSynchronize.cpp \
  SpinLock.cpp \
  Synchronize.cpp \
  Synchronizeable.cpp \
  Thread.cpp \
  ThreadKey.cpp \
  ThreadPool.cpp

sectiondir = $(includedir)/base/concurrency
section_HEADERS = \
  AtomicCounter.h \
  Barrier.h \
  ConcurrencyException.h \
  Event.h \
  ExclusiveSynchronize.h \
  Lock.h \
  LockException.h \
  MutualExclusion.h \
  NISpinLock.h \
  Once.h \
  Process.h \
  ReadWriteLock.h \
  ReadWriteSpinLock.h \
  RecursiveSpinLock.h \
  Runnable.h \
  Semaphore.h \
  SharedSynchronize.h \
  SingleExclusiveSynchronize.h \
  SpinLock.h \
  Synchronize.h \
  Synchronizeable.h \
  Thread.h \
  ThreadKey.h \
  ThreadPool.h
