/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Base.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <csignal>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void breakpoint() noexcept
{
  static bool useBreakpoint = true;
  if (useBreakpoint) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DebugBreak();
#else
    std::raise(SIGINT);
#endif
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
