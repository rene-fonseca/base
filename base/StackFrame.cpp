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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

StackFrame::StackFrame()
{
}

void* StackFrame::getStackFrame() noexcept
{
  void** frame = nullptr;

#if ((_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM) || \
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
  // register void* sp asm ("sp");
  if (frame) {
    frame = (void**)*frame; // get parent frame
  }
  return frame;
}

namespace {

  /** Returns the instruction pointer (return address) for the given stack frame. */
  inline void* getInstructionPointer(void* frame) noexcept
  {
    void* ip = reinterpret_cast<void**>(frame)[1]; // TAG: handle all ABIs
    return ip;
  }
}

void* StackFrame::getCaller() noexcept
{
  void* frame = getStackFrame();
  return getInstructionPointer(frame);
}

#if 0 // low level
MemorySize StackFrame::getStack(void** dest, MemorySize size)
{
  return 0;
}
#endif

StackFrame StackFrame::getStack(unsigned int levels)
{
  StackFrame frames;
  unsigned int count = 0;
  void* _frame = getStackFrame();
  void* frame = _frame;
  while (frame && (count < levels)) {
    ++count;
    void* ip = getInstructionPointer(frame);
    void* symbol = DynamicLinker::getSymbolAddress(ip); // TAG: can we avoid this
    if (!symbol) {
      break;
    }
    if (reinterpret_cast<MemorySize>(ip) < 0x10000) { // TAG: what is the proper way to detect top of stack
      break;
    }
    auto previous = frame;
    frame = *reinterpret_cast<void**>(frame);
    if (!frame || (frame == previous)) {
      break;
    }
  }
  const MemorySize size = count;
  frames.frames.setSize(size);
  void** dest = frames.frames.getElements();
  frame = _frame;
  count = 0;
  while (frame && (count < size)) {
    ++count;
    void* ip = getInstructionPointer(frame);
    *dest++ = ip;
    frame = *reinterpret_cast<void**>(frame);
  }
  return frames;
}

void StackFrame::dump(unsigned int levels)
{
  static bool here = false;
  if (here) {
    return;
  }
  here = true;

  FormatOutputStream& stream = ferr;

  void* frame = getStackFrame();
  MemorySize count = 0;
  stream << "Stack trace:" << EOL;
  if (!frame) {
    stream << indent(2) << "NO STACK FRAMES" << EOL;
    return;
  }
  while (frame && (count < levels)) {
    void* ip = getInstructionPointer(frame);
    void* symbol = DynamicLinker::getSymbolAddress(ip);
    if (!symbol) {
      break;
    }
    if (reinterpret_cast<MemorySize>(ip) < 0x10000) {
      break;
    }
    const String name = DynamicLinker::getSymbolName(ip);
    if (name) {
      stream << indent(2) << count << ": " << symbol << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << (reinterpret_cast<uint8*>(ip) - reinterpret_cast<uint8*>(symbol)) << " " << TypeInfo::demangleName(name.native()) << EOL;
    } else {
      stream << indent(2) << count << ": " << symbol << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << (reinterpret_cast<uint8*>(ip) - reinterpret_cast<uint8*>(symbol)) << EOL;
    }
    ++count;
    frame = *reinterpret_cast<void**>(frame);
  }
  stream << FLUSH;
  here = false;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const StackFrame& value) throw(IOException)
{
  auto size = value.getSize();
  stream << "Stack trace:" << EOL;
  if (size == 0) {
    stream << indent(2) << "NO STACK FRAMES" << EOL;
    return stream;
  }
  for (MemorySize i = 0; i < size; ++i) {
    void* ip = value.getFrame(i);
    void* symbol = DynamicLinker::getSymbolAddress(ip);
    if (!symbol) {
      break;
    }
    const String name = DynamicLinker::getSymbolName(ip);
    if (name) {
      stream << indent(2) << i << ": " << symbol << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << (reinterpret_cast<uint8*>(ip) - reinterpret_cast<uint8*>(symbol)) << " " << TypeInfo::demangleName(name.native()) << EOL;
    } else {
      stream << indent(2) << i << ": " << symbol << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << (reinterpret_cast<uint8*>(ip) - reinterpret_cast<uint8*>(symbol)) << EOL;
    }
  }
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
