/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Trace.h>
#include <base/Base.h>
#include <base/Type.h>
#include <base/mem/NullPointer.h>
#include <base/string/WideString.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  include <string.h>
#  include <stdio.h>
#else // unix
#  include <string.h>
#  include <stdio.h>
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <syslog.h>
#endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
namespace win32 {
  extern "C" void OutputDebugStringA(const char*);
#define OutputDebugString OutputDebugStringA
};
#endif // cygwin

void Trace::message(const char* message) noexcept
{
  if (!message) {
    throw NullPointer(Type::getType<Trace>());
  }
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::OutputDebugString(ToWCharString(message));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN) // special case
  win32::OutputDebugString(message);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else // unix
  // const char* ident = nullptr;
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, message, "");
#else
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, message, "");
#endif
  closelog();
#endif // flavor
}

void Trace::member(const void* pointer, const char* message) noexcept
{
  if (!message) {
    throw NullPointer(Type::getType<Trace>());
  }
  
  const MemorySize length = getNullTerminatedLength(message);
  PrimitiveStackArray<char> buffer(22 + length + 1);
  // TAG: remove sprintf dependency
  sprintf(buffer, "%p >> %s", pointer, message); // sprintf must be MT-safe
#if (_COM_AZURE_DEV__BASE__INT_SIZE > 8)
#  error pointer type not supported
#endif
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::OutputDebugString(ToWCharString(buffer));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN) // special case
  win32::OutputDebugString(buffer);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else // unix
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, buffer, "");
#else
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, buffer, ""); // TAG: use vsyslog() instead
#endif
  closelog();
#endif // flavor
}

Trace::Trace(const char* _msg) noexcept
  : msg(_msg)
{
  Trace::member(this, msg);
}

Trace::~Trace() noexcept
{
  Trace::member(this, msg);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
