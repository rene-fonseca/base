/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/concurrency/Thread.h>
#include <base/Profiler.h>
#include <base/StackFrame.h>
#include <base/mem/Heap.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** State for all threads. Do NOT access directly from external modules. Context may change at any time. */
class _COM_AZURE_DEV__BASE__API ThreadLocalContext : public DynamicObject {
public:

  static constexpr unsigned int STORAGE_BUFFERS = 4; // allows limited recursion
  static constexpr unsigned int PROFILER_TASKS = 64; // allows limited recursion

  /** The thread object associated with context. */
  Thread* thread = nullptr;
  /** Simple unique id - does NOT relate to system IDs. */
  unsigned int simpleId = 0;
  String name;
  
  /** Profiling context. */
  struct {
    unsigned int suspended = 0;
    unsigned int captureIO = 32;
    unsigned int tasks = 0; // number of tasks
    Allocator<Profiler::Event> events; // stack for task events
  } profiling;
  
  // ATTENTION: watch out for recursions which might use the same thread local resource!
  
  /** Counter for storage usage. */
  FixedResourceManager<STORAGE_BUFFERS> storageManager;
  /** The thread local storage. */
  Allocator<uint8> storage[STORAGE_BUFFERS];
  /** String output stream. */
  StringOutputStream stringOutputStream;
  /** String output stream. */
  unsigned int stringOutputStreamUsage = 0;
  /** Random generator. */
  RandomInputStream randomInputStream;
  /** Last known stack trace for exception. */
  StackFrame stackTrace;
  
  /** Number of exceptions thrown by thread. */
  unsigned int numberOfExceptions = 0;
  /** Total microseconds spend waiting. */
  uint64 waiting = 0;
  /** Bytes read. */
  uint64 bytesRead = 0;
  /** Bytes written. */
  uint64 bytesWritten = 0;

  ThreadLocalContext();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
