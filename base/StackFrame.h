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
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Stack frame.
*/

class _COM_AZURE_DEV__BASE__API StackFrame {
private:
  
  Allocator<const void*> frames; // we use void* over MemorySize since debugger can show symbols
public:

  /** Returns true if stack trace is supported. */
  static inline constexpr bool doesSupportStackTrace() noexcept
  {
#if (defined(_COM_AZURE_DEV__BASE__NO_STACK_TRACE))
    return false;
#else
    return true;
#endif
  }

  /** Print stack trace to standard out instead of standard error. */
  static bool getUseStandardOut() noexcept;

  /** Print stack trace to standard out instead of standard error. */
  static void setUseStandardOut(bool useStandardOut) noexcept;

  /** Returns the preferred output stream. */
  static FormatOutputStream& getErrorStream() noexcept;

  StackFrame();

  StackFrame(const ConstSpan<const void*>& stackTrace);

  StackFrame& operator=(const ConstSpan<const void*>& assign);

  /** Returns hash for stack trace. */
  static unsigned long getHash(const ConstSpan<const void*>& stackTrace) noexcept;

  /** Returns hash for stack trace. */
  inline unsigned long getHash() const noexcept
  {
    return getHash(frames.getSpan());
  }
  
  /** Returns the stack frames. */
  inline ConstSpan<const void*> getTrace() const noexcept
  {
    return frames.getSpan();
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
  inline const void* getFrame(MemorySize index) const noexcept
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
  MemoryDiff find(const void* address) const noexcept;

  /**
    Returns the index of the last occurrence symbol address if found.

    @return Returns -1 if not found.
  */  
  MemoryDiff findLast(const void* address) const noexcept;

  /**
    Strips any frames before the given index.
  */
  void stripUntil(MemorySize index) noexcept;

  /**
    Strips any frames before the first occurrence of the given address if present on the stack.

    @return The number of frames skipped.
  */  
  MemorySize stripUntil(const void* ip) noexcept;

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
  static unsigned int getStack(const void** dest, unsigned int size, unsigned int skip = 1, bool trim = true);

  /**
    Returns the stack.
   
    @param skip Skip the first frames.
    @param levels The maximum number of levels to dump.
    @param trim Trim to base address.
  */
  static StackFrame getStack(unsigned int skip = 1, unsigned int levels = 32, bool trim = true);

  /** Returns true if stack traces are equal. */
  bool operator==(const StackFrame& compare) const noexcept
  {
    if (frames.getSize() != compare.getSize()) {
      return false;
    }
    for (MemorySize i = 0; i < frames.getSize(); ++i) {
      if (frames.getElements()[i] != compare.frames.getElements()[i]) {
        return false;
      }
    }
    return true;
  }

  /** Returns true if stack traces are non-equal. */
  inline bool operator!=(const StackFrame& compare) const noexcept
  {
    return !(operator==(compare));
  }

  /** Returns true if stack traces are equal. */
  bool operator==(const ConstSpan<const void*>& compare) const noexcept
  {
    if (frames.getSize() != compare.getSize()) {
      return false;
    }
    for (MemorySize i = 0; i < frames.getSize(); ++i) {
      if (frames.getElements()[i] != compare[i]) {
        return false;
      }
    }
    return true;
  }

  /** Returns true if stack traces are non-equal. */
  inline bool operator!=(const ConstSpan<const void*>& compare) const noexcept
  {
    return !(operator==(compare));
  }

  /** Returns true if stack trace is empty. */
  inline operator bool() const noexcept
  {
    return !frames.isEmpty();
  }

  /**
   Dump stack.
   
    @param levels The maximum number of levels to dump.
  */
  static void dump(unsigned int skip = 1, unsigned int levels = 32);
  
  enum {
    FLAG_SHOW_ADDRESS = 1 << 0, // Show address.
    FLAG_SHOW_MODULE = 1 << 1, // Show module.
    FLAG_INDENT = 1 << 2, //< Indent.
    FLAG_COMPACT = 1 << 3, //< Only show.
    FLAG_FULL_PATH = 1 << 4, //< Show full path for module.
    FLAG_USE_COLORS = 1 << 5, //< Use ANSI colors.
    FLAG_USE_COLORS_IF_ANSI_TERMINAL = 1 << 6, //< Use ANSI colors if supported by stream.
    FLAG_STRIP_NAMESPACE = 1 << 7, //< base namespace.
    FLAG_TRIM_SYSTEM = 1 << 8, // Skip system functions.
    FLAG_DEFAULT = FLAG_COMPACT | FLAG_SHOW_ADDRESS | FLAG_SHOW_MODULE | FLAG_INDENT | FLAG_TRIM_SYSTEM | FLAG_USE_COLORS_IF_ANSI_TERMINAL
  };
  
  /**
    Format stack trace to stream.
  */
  static FormatOutputStream& toStream(FormatOutputStream& stream,
                                      const ConstSpan<const void*>& stackTrace,
                                      unsigned int flags = FLAG_DEFAULT);
};

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const StackFrame& value);

template<>
class _COM_AZURE_DEV__BASE__API Hash<StackFrame> {
public:

  unsigned long operator()(const StackFrame& value) noexcept
  {
    return value.getHash();
  }
};

// TAG: refactor to use StackTrace 
typedef StackFrame StackTrace;

/** Combines formatting with stack trace. */
class _COM_AZURE_DEV__BASE__API FormattedStackTrace {
private:

  const StackTrace& stackTrace;
  unsigned int flags = 0;
public:

  /** Initializes formatting. */
  inline FormattedStackTrace(const StackTrace& _stackTrace, unsigned int _flags = StackTrace::FLAG_DEFAULT) noexcept
    : stackTrace(_stackTrace), flags(_flags)
  {
  }

  /** Returns the flags. */
  inline unsigned int getFlags() const noexcept
  {
    return flags;
  }

  /** Returns the stack trace. */
  inline operator const StackTrace&() const noexcept
  {
    return stackTrace;
  }
};

/** Writes stack trace to stream with formatting flags. */
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const FormattedStackTrace& stackTrace)
{
  return StackTrace::toStream(stream, static_cast<const StackTrace&>(stackTrace).getTrace(), stackTrace.getFlags());
}

/** Returns container as HTML. */
_COM_AZURE_DEV__BASE__API String getContainerAsHTML(const StackTrace& stackTrace);

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)

inline ClingMimeBundle mime_bundle_repr(const StackTrace& stackTrace)
{
  return ClingMimeBundleHandle().setHTMLText(getContainerAsHTML(stackTrace));
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
