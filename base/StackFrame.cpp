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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void* StackFrame::getStackFrame() noexcept
{
  void** frame = nullptr;

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM)
  asm("mov %%rsp, %0" : "=rm" ( frame ));
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
