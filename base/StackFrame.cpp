/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/StackFrame.h>
#include <base/dl/DynamicLinker.h>
#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FileSystem.h>
#include <base/TypeInfo.h>
#include <base/string/ANSIEscapeSequence.h>
#include <base/io/FileDescriptor.h>
#include <base/Application.h>
#include <base/string/Format.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
#  include <execinfo.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

StackFrame::StackFrame()
{
}

MemoryDiff StackFrame::find(void* address) const noexcept
{
  const void* const* src = frames.getElements();
  for (MemorySize i = 0; i < frames.getSize(); ++i) {
    const void* ip = src[i];
    const void* symbol = DynamicLinker::getSymbolAddress(ip);
    if (symbol == address) {
      return i; // found
    }
  }
  return -1;
}

MemoryDiff StackFrame::findLast(void* address) const noexcept
{
  const void* const* src = frames.getElements();
  for (MemoryDiff i = frames.getSize() - 1; i >= 0; --i) {
    const void* ip = src[i];
    const void* symbol = DynamicLinker::getSymbolAddress(ip);
    if (symbol == address) {
      return i; // found
    }
  }
  return -1;
}

void StackFrame::stripUntil(MemorySize index) noexcept
{
  if (index > 0) {
    const MemorySize size = frames.getSize();
    if (index >= size) {
      frames.setSize(0);
      return;
    }
    void** dest = frames.getElements();
    move(dest, dest + index, size - index);
    frames.setSize(size - index);
  }
}

MemorySize StackFrame::stripUntil(void* address) noexcept
{
  MemoryDiff index = find(address);
  if (index > 0) { // found
    stripUntil(index);
  }
  return index;
}

void* StackFrame::getStackFrame() noexcept
{
  void** frame = nullptr;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return nullptr; // not available for x64
#elif ((_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM) || \
       (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC))
  asm("mov %%rbp, %0" : "=rm" ( frame ));
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
  asm (
    "movl %%ebp,%0;\n"
    : "=m" (frame) // output
  );
#else
  BASSERT(!"Not implemented");
#endif
  if (frame) {
    frame = reinterpret_cast<void**>(*frame); // get parent frame
  }
  return frame;
}

namespace {

  /** Returns the instruction pointer (return address) for the given stack frame. */
  inline void* getInstructionPointer(void* frame) noexcept
  {
    if (!frame) {
      return nullptr;
    }
    void* ip = reinterpret_cast<void**>(frame)[1]; // TAG: handle all ABIs
    return ip;
  }
}

void* StackFrame::getCaller() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return _ReturnAddress();
#else
  void* frame = getStackFrame();
  return getInstructionPointer(frame);
#endif
}

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

namespace {

  typedef USHORT (__stdcall *RtlCaptureStackBackTraceFunc)(ULONG FramesToSkip, ULONG FramesToCapture, PVOID* BackTrace, PULONG BackTraceHash);

  bool initialized = false;
  HMODULE handle = nullptr;
  RtlCaptureStackBackTraceFunc rtlCaptureStackBackTrace = nullptr;

  void loadNtDll()
  {
    if (!initialized) {
      initialized = true;
      handle = ::LoadLibraryExW(L"ntdll.dll", 0, 0);
      rtlCaptureStackBackTrace = (RtlCaptureStackBackTraceFunc)GetProcAddress(handle, "RtlCaptureStackBackTrace");
    }
  }
}

#endif

unsigned int StackFrame::getStack(void** dest, unsigned int size, unsigned int skip)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  USHORT count = RtlCaptureStackBackTrace(skip + 1, size, dest, NULL);
  while ((count > 0) && (reinterpret_cast<MemorySize>(dest[count - 1]) < 0x10000)) {
    --count; // remove bad text seg pointer
  }
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
  int count = backtrace(dest, size);
  if (count < 0) {
    return 0;
  }
  ++skip;
  if (skip >= count) {
    return 0;
  }
  auto write = dest;
  auto src = dest + skip;
  auto end = dest + count;
  while (src != end) { // remove bad text seg pointer
    if ((reinterpret_cast<MemorySize>(*src) >= 0x10000) /*&& !(reinterpret_cast<MemorySize>(*src) >> (12*4-1))*/) {
      *write++ = *src;
    }
    ++src;
  }
  count = static_cast<unsigned int>(write - dest);
  // we cannot detect overflow due to skip
#else
  void* frame = getStackFrame();
  // ++skip;
  while (frame && skip--) {
    frame = *reinterpret_cast<void**>(frame);
  }
  unsigned int count = 0;
  while (frame && (count < size)) {
    ++count;
    void* ip = getInstructionPointer(frame);
    void* symbol = DynamicLinker::getSymbolAddress(ip); // TAG: can we avoid this
    if (!symbol) {
      break;
    }
    if (reinterpret_cast<MemorySize>(ip) < 0x10000) { // TAG: what is the proper way to detect top of stack
      break;
    }
    *dest++ = ip;
    frame = *reinterpret_cast<void**>(frame);
  }
#endif
  // TAG: calc hash for stack trace
  return count;
}

StackFrame StackFrame::getStack(unsigned int skip, unsigned int levels)
{
  StackFrame frames;

  if (levels == 0) {
    return frames;
  }

  ++skip;

  {
    void* trace[256]; // quick buffer
    const unsigned int count = getStack(trace, minimum<MemorySize>(levels, getArraySize(trace)), skip);
    if (!INLINE_ASSERT(count > 0)) {
      return frames;
    }
    if ((count != getArraySize(trace)) || (count <= levels)) { // no overflow
      frames.frames.setSize(count);
      copy(frames.frames.getElements(), trace, count);
      return frames;
    }
  }
  
  PrimitiveStackArray<void*> buffer(1024);
  unsigned int count = 0;
  while (buffer.size() < (64 * 1024)) {
    count = getStack(buffer, minimum<MemorySize>(levels, buffer.size()), skip);
    if ((count == buffer.size()) && (count < levels)) { // overflow
      buffer.resize(buffer.size() * 2);
      continue;
    }
    break;
  }

  frames.frames.setSize(count);
  copy(frames.frames.getElements(), static_cast<void**>(buffer), count);

  return frames;
}

// TAG: add support for registering stack trace in global lookup and get hash key for it

void StackFrame::toStream(FormatOutputStream& stream, const void* const * trace, MemorySize size, unsigned int flags)
{
  const bool showAddress = (flags & FLAG_SHOW_ADDRESS) != 0;
  const bool useColors = (flags & FLAG_USE_COLORS) != 0;
  const unsigned int INDENT = ((flags & FLAG_INDENT) != 0) ? 2 : 0;

  stream << "Stack trace:" << EOL;
  if (!trace || (size == 0)) {
    stream << indent(INDENT) << "NO STACK FRAMES" << EOL;
    return;
  }

  unsigned int field1 = 1;
  if (size >= 100) {
    field1 = 3;
  } else if (size >= 10) {
    field1 = 2;
  }

  unsigned int field2 = 8; // never less than 8
  if (sizeof(MemorySize) > 4) {
    MemorySize bits = 0;
    for (MemorySize i = 0; i < size; ++i) {
      MemorySize ip = reinterpret_cast<MemorySize>(trace[i]); // technically we should resolve symbol - but not hard done
      bits |= ip;
    }
    unsigned int digits = 0;
    while (bits) {
      bits >>= 4;
      ++digits;
    }
    field2 = maximum(digits, field2);
  }
  field2 += 2; // prefix

  String lastAddress;
  MemorySize count = 0;
  for (MemorySize i = 0; i < size; ++i) {
    const void* ip = trace[i];
    const void* symbol = DynamicLinker::getSymbolAddress(ip);

    String path;
    if (flags & FLAG_SHOW_MODULE) {
      path = DynamicLinker::getImagePath(ip);
      if ((flags & FLAG_FULL_PATH) == 0) {
        path = FileSystem::getComponent(path, FileSystem::FILENAME);
      }
    }

    if (symbol) {
      stream << indent(INDENT) << setWidth(field1) << count << ": ";
      const String name = DynamicLinker::getSymbolName(ip);
      auto displacement = (reinterpret_cast<const uint8*>(ip) - reinterpret_cast<const uint8*>(symbol));
      if (showAddress) {
        StringOutputStream sos;
        sos << setWidth(field2) << symbol << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << displacement;
        const String address = sos.toString();
        if (useColors) {
          MemorySize j = 0; // find first difference for new address
          for (; j < minimum(address.getLength(), lastAddress.getLength()); ++j) {
            if (address[j] != lastAddress[j]) {
              break;
            }
          }
          lastAddress = address;
          const bool useDim = false && !WINDOWS;
          if (useDim) {
            stream << dim();
          }
          stream << setForeground(ANSIEscapeSequence::RED) << address.substring(0, j) /*<< '.'*/ << normal();
          if (!useDim) {
            stream << underscore();
          }
          stream << setForeground(ANSIEscapeSequence::RED) << address.substring(j)
                 << normal();
        } else {
          stream << address;
        }
      }
      if (name) {
        if (showAddress) {
          stream << " ";
        }
        String demangled = TypeInfo::demangleName(name.native());
        const bool stripNamespace = flags & FLAG_STRIP_NAMESPACE; // this will compact output quite a bit
        if (stripNamespace) {
          demangled.replaceAll("base::", String()); // or std:: // TAG: we should check char just before to make sure this is a namespace
          // TAG: we can add a register of namespace macro like register of test to support stripping of any namespace
        }
        if (useColors) {
          if (path) {
            stream << setForeground(ANSIEscapeSequence::GREEN) << path << normal() << "!";
          }
          auto index = demangled.indexOf('(');
          if (index >= 0) {
            stream << setForeground(ANSIEscapeSequence::BLUE) << bold() << demangled.substring(0, index) << normal()
                   << setForeground(ANSIEscapeSequence::BLUE) << italic() << demangled.substring(index);
          } else {
            stream << setForeground(ANSIEscapeSequence::BLUE) << bold() << demangled;
          }
          stream << normal();
        } else {
          if (path) {
            stream << path << "!" << demangled;
          } else {
            stream << demangled;
          }
        }

        if (flags & FLAG_TRIM_SYSTEM) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
          if (demangled == "CtrlRoutine") {
            stream << EOL;
            break;
          }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
          if (demangled == "_pthread_start") {
            stream << EOL;
            break;
          }
#endif
          if (demangled == "main") { // TAG: add support for all platforms - better to backtrim when we get stack trace - also handle threads
            stream << EOL;
            break;
          }
        }
      }
      stream << EOL;
    } else { // no descrption
      if ((flags & FLAG_COMPACT) == 0) {
        stream << indent(INDENT) << setWidth(field1) << count << ": ";
        if (useColors) {
          stream << setForeground(ANSIEscapeSequence::RED) << setWidth(field2) << ip << normal();
        } else {
          stream << setWidth(field2) << ip;
        }
        stream << "?"; // TAG: we could still show image name
        stream << EOL;
      }
    }
    ++count;
  }
}

void StackFrame::dump(unsigned int skip, unsigned int levels)
{
  if (levels == 0) {
    return;
  }
  const bool colors = FileDescriptor::getStandardError().isANSITerminal(); // use ferr
  
  ++skip;
  const unsigned int flags = StackFrame::FLAG_SHOW_ADDRESS | StackFrame::FLAG_SHOW_MODULE | StackFrame::FLAG_INDENT |
    (colors ? StackFrame::FLAG_USE_COLORS : 0);

  {
    void* trace[256]; // quick buffer
    const MemorySize count = getStack(trace, minimum<MemorySize>(levels, getArraySize(trace)), skip);
    if (!INLINE_ASSERT(count > 0)) {
      return;
    }
    if ((count != getArraySize(trace)) || (count <= levels)) { // no overflow
      toStream(ferr, trace, count, flags);
      ferr << FLUSH;
      return;
    }
  }

  PrimitiveStackArray<void*> buffer(1024);
  MemorySize count = 0;
  while (buffer.size() < (64 * 1024)) {
    count = getStack(buffer, minimum<MemorySize>(levels, buffer.size()), skip);
    if ((count == buffer.size()) && (count < levels)) { // overflow
      buffer.resize(buffer.size() * 2);
      continue;
    }
    break;
  }

  toStream(ferr, buffer, count, flags);
  ferr << FLUSH;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const StackFrame& value) throw(IOException)
{
  const bool colors = false; // FileDescriptor::getStandardError().isANSITerminal(); // stream may not be stderr
  StackFrame::toStream(
    stream, value.getTrace(), value.getSize(),
    StackFrame::FLAG_DEFAULT | (colors ? StackFrame::FLAG_USE_COLORS : 0)
  );
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
