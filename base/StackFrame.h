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
    FLAG_USE_COLORS = 1 << 3
  };
  
  /**
    Format stack trace to stream.
  */
  static void toStream(FormatOutputStream& stream, const void* const* trace, MemorySize size, unsigned int flags = FLAG_SHOW_ADDRESS | FLAG_SHOW_MODULE | FLAG_INDENT);
};

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const StackFrame& value) throw(IOException);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
