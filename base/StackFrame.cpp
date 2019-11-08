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
#include <base/TypeInfo.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

StackFrame::StackFrame()
{
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

MemorySize StackFrame::getStack(void** dest, MemorySize size)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  USHORT count = RtlCaptureStackBackTrace(1, size, dest, NULL);
#else
  unsigned int count = 0;
  void* _frame = getStackFrame();
  void* frame = _frame;
  while (frame && (count < size)) {
    ++count;
    void* ip = getInstructionPointer(frame);
    *dest++ = ip;
    void* symbol = DynamicLinker::getSymbolAddress(ip); // TAG: can we avoid this
    if (!symbol) {
      break;
    }
    if (reinterpret_cast<MemorySize>(ip) < 0x10000) { // TAG: what is the proper way to detect top of stack
      break;
    }
    auto previous = frame;
    frame = *reinterpret_cast<void**>(frame);
  }
  return count;
#endif
  // TAG: calc hash for stack trace
  return count;
}

StackFrame StackFrame::getStack(unsigned int levels)
{
  StackFrame frames;

  if (levels == 0) {
    return frames;
  }

  {
    void* trace[256]; // quick buffer
    const MemorySize count = getStack(trace, minimum<MemorySize>(levels, getArraySize(trace)));
    if (!INLINE_ASSERT(count > 0)) {
      return frames;
    }
    if ((count != getArraySize(trace)) || (count <= levels)) { // no overflow
      frames.frames.setSize(count);
      copy(frames.frames.getElements(), trace, count);
      return frames;
    }
  }
  
  PrimitiveArray<void*> buffer(1024);
  MemorySize count = 0;
  while (buffer.size() < (64 * 1024)) {
    count = getStack(buffer, minimum<MemorySize>(levels, buffer.size()));
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

void /*StackFrame::*/dump(FormatOutputStream& stream, void* const * trace, MemorySize size, bool verbose)
{
  // we could color code modules
  const unsigned int INDENT = 2;

  stream << "Stack trace:" << EOL;
  if (!trace || (size == 0)) {
    stream << indent(INDENT) << "NO STACK FRAMES" << EOL;
    return;
  }
  for (MemorySize i = 0; i < size; ++i) {
    void* ip = trace[i];
    void* symbol = DynamicLinker::getSymbolAddress(ip);
    if (symbol) {
      const String name = DynamicLinker::getSymbolName(ip);
      auto displacement = (reinterpret_cast<uint8*>(ip) - reinterpret_cast<uint8*>(symbol));
      if (name) {
        stream << indent(INDENT) << i << ": " << symbol << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << displacement << " " << TypeInfo::demangleName(name.native()) << EOL;
      } else {
        stream << indent(INDENT) << i << ": " << symbol << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << displacement << EOL;
      }
    } else {
      stream << indent(INDENT) << i << ": ?" << ip << EOL;
    }
    if (verbose) {
      stream << indent(INDENT) << "!" << DynamicLinker::getImagePath(ip) << EOL; // TAG: just show the filename
    }
  }
}

void StackFrame::dump(unsigned int levels)
{
  if (levels == 0) {
    return;
  }

  {
    void* trace[256]; // quick buffer
    const MemorySize count = getStack(trace, minimum<MemorySize>(levels, getArraySize(trace)));
    if (!INLINE_ASSERT(count > 0)) {
      return;
    }
    if ((count != getArraySize(trace)) || (count <= levels)) { // no overflow
      base::dump(ferr, trace, count, false);
      ferr << FLUSH;
      return;
    }
  }

  PrimitiveArray<void*> buffer(1024);
  MemorySize count = 0;
  while (buffer.size() < (64 * 1024)) {
    count = getStack(buffer, minimum<MemorySize>(levels, buffer.size()));
    if ((count == buffer.size()) && (count < levels)) { // overflow
      buffer.resize(buffer.size() * 2);
      continue;
    }
    break;
  }

  base::dump(ferr, buffer, count, false);
  ferr << FLUSH;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const StackFrame& value) throw(IOException)
{
  base::dump(stream, value.getTrace(), value.getSize(), false);
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
