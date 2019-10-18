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

#if defined(_COM_AZURE_DEV__BASE__DEBUG)
void _COM_AZURE_DEV__BASE__BUILD_DEBUG() noexcept
{
  breakpoint(); // do not call
}
#else
void _COM_AZURE_DEV__BASE__BUILD_RELEASE() noexcept
{
  breakpoint(); // do not call
}
#endif

void _COM_AZURE_DEV__BASE__CONCATENATE(_COM_AZURE_DEV__BASE__VERSION_, _COM_AZURE_DEV__BASE__MAJOR_VERSION)() noexcept
{
  breakpoint(); // do not call
}

// Generate symbols to cause linker failure on mismatching shared/static builds
#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)
void _COM_AZURE_DEV__BASE__BUILD_SHARED() noexcept
{
  breakpoint(); // do not call
}
#else
void _COM_AZURE_DEV__BASE__BUILD_STATIC() noexcept
{
  breakpoint(); // do not call
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
