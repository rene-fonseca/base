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

noinst_LTLIBRARIES = libmem.la

libmem_la_SOURCES = \
  Allocator.cpp \
  CheckedPointer.cpp \
  DebugDynamicMemory.cpp \
  DynamicMemory.cpp \
  Heap.cpp \
  Indirect.cpp \
  MemoryCorruption.cpp \
  MemoryDump.cpp \
  NullPointer.cpp \
  Pointer.cpp \
  ProtectedPointer.cpp \
  Reference.cpp \
  SharedMemory.cpp \
  VirtualMemory.cpp

sectiondir = $(includedir)/base/mem
section_HEADERS = \
  Allocator.h \
  AllocatorEnumeration.h \
  Buffer.h \
  CapacityAllocator.h \
  CheckedPointer.h \
  DebugDynamicMemory.h \
  DynamicMemory.h \
  Heap.h \
  Indirect.h \
  Lifetime.h \
  MemoryCorruption.h \
  MemoryDump.h \
  NullPointer.h \
  OwnershipPointer.h \
  Pointer.h \
  ProtectedPointer.h \
  Reference.h \
  ReferenceCountedAllocator.h \
  ReferenceCountedCapacityAllocator.h \
  ReferenceCountedObject.h \
  ReferenceCountedObjectWrapper.h \
  ReferenceCounter.h \
  SharedMemory.h \
  StrictPointer.h \
  VirtualMemory.h
