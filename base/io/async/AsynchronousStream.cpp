/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/async/AsynchronousStream.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

bool AsynchronousStream::asyncTest() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return ::SleepEx(0, TRUE) == WAIT_IO_COMPLETION;
#else // unix
  return false;
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
