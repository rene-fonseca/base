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

#include <base/mem/Allocator.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Stack frame.
*/

class _COM_AZURE_DEV__BASE__API StackFrame { // TAG: rename
private:
  
  Allocator<void*> frames;
public:
  
  StackFrame();

  /** Returns the number of stack frames. */
  inline void* const * getTrace() const noexcept
  {
    return frames.getElements();
  }

  /** Returns the number of stack frames. */
  inline MemorySize getSize() const noexcept
  {
    return frames.getSize();
  }
  
  /** Returns the number of stack frames. */
  inline bool isEmpty() const noexcept
  {
    return frames.isEmpty();
  }
  
  /** Returns the stack frame. */
  inline void* getFrame(MemorySize index) const noexcept
  {
    if (index >= frames.getSize()) {
      return nullptr;
    }
    return frames.getElements()[index];
  }

  /**
    Returns the index of the symbol address if found.

    @return Returns -1 if not found.
  */  
  MemoryDiff find(void* address) const noexcept;

  /**
    Returns the index of the last occurrence symbol address if found.

    @return Returns -1 if not found.
  */  
  MemoryDiff findLast(void* address) const noexcept;

  /**
    Strips any frames before the given index.
  */
  void stripUntil(MemorySize index) noexcept;

  /**
    Strips any frames before the first occurrence of the given address if present on the stack.

    @return The number of frames skipped.
  */  
  MemorySize stripUntil(void* ip) noexcept;

  /**
    Returns the stack frame. Not available for all platforms.
  */
  static void* getStackFrame() noexcept;

  /**
    Returns the caller. Not available for all platforms.
  */
  static void* getCaller() noexcept;

  /**
    Returns the stack trace.
  */
  static unsigned int getStack(void** dest, unsigned int size, unsigned int skip = 1);

  /**
    Returns the stack.
   
    @param levels The maximum number of levels to dump.
  */
  static StackFrame getStack(unsigned int skip = 1, unsigned int levels = 32);

  /**
   Dump stack.
   
    @param levels The maximum number of levels to dump.
  */
  static void dump(unsigned int skip = 1, unsigned int levels = 32);

  enum {
    FLAG_SHOW_ADDRESS = 1 << 0,
    FLAG_SHOW_MODULE = 1 << 1,
    FLAG_INDENT = 1 << 2,
    FLAG_COMPACT = 1 << 3,
    FLAG_FULL_PATH = 1 << 4,
    FLAG_USE_COLORS = 1 << 5,
    FLAG_STRIP_NAMESPACE = 1 << 6,
    FLAG_TRIM_SYSTEM = 1 << 7,
    FLAG_DEFAULT = FLAG_COMPACT | FLAG_SHOW_ADDRESS | FLAG_SHOW_MODULE | FLAG_INDENT | FLAG_TRIM_SYSTEM
  };
  
  /**
    Format stack trace to stream.
  */
  static void toStream(FormatOutputStream& stream, const void* const* trace, MemorySize size, unsigned int flags = FLAG_DEFAULT);
};

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const StackFrame& value) throw(IOException);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
