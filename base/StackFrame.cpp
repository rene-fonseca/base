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

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM)
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

StackFrame StackFrame::getStack(unsigned int levels)
{
  StackFrame frames;

  unsigned int count = 0;
  void* _frame = getStackFrame();
  void* frame = _frame;
  while (frame && (count < levels)) {
    ++count;
    void* ip = reinterpret_cast<void**>(frame)[1]; // TAG: handle all ABIs
    if (reinterpret_cast<MemorySize>(ip) < 0x10000) { // TAG: what is the proper way to detect top of stack
      break;
    }
    frame = *reinterpret_cast<void**>(frame);
  }
  frames.frames.setSize(count);
  void** dest = frames.frames.getElements();
  frame = _frame;
  count = 0;
  while (frame && (count < levels)) {
    ++count;
    void* ip = reinterpret_cast<void**>(frame)[1]; // TAG: handle all ABIs
    *dest++ = ip;
    frame = *reinterpret_cast<void**>(frame);
  }
  return frames;
}

void StackFrame::dump(unsigned int levels)
{
  void* frame = getStackFrame();
  MemorySize count = 0;
  fout << "Stack trace:" << EOL;
  if (!frame) {
    fout << indent(2) << "NO STACK FRAMES" << EOL;
    return;
  }
  while (frame && (count < levels)) {
    // TAG: need to get symbol name from debugger info
    void* ip = reinterpret_cast<void**>(frame)[1]; // TAG: handle all ABIs
    if (reinterpret_cast<MemorySize>(ip) < 0x10000) { // TAG: what is the proper way to detect top of stack
      break;
    }
    const String name = DynamicLinker::getSymbolName(ip);
    if (name) {
      fout << indent(2) << count << ": " << TypeInfo::demangleName(name.native()) << EOL;
    } else {
      fout << indent(2) << count << ": " << ip << EOL;
    }
    ++count;
    frame = *reinterpret_cast<void**>(frame);
  }
  fout << FLUSH;
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
    const String name = DynamicLinker::getSymbolName(ip);
    if (name) {
      stream << indent(2) << i << ": " << TypeInfo::demangleName(name.native()) << EOL;
    } else {
      stream << indent(2) << i << ": " << ip << EOL;
    }
  }
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
