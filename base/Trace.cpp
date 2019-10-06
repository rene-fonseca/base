/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  include <string.h>
#  include <stdio.h>
#else // unix
#  include <string.h>
#  include <stdio.h>
#  include <syslog.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
namespace win32 {
  extern "C" void OutputDebugStringA(const char*);
#define OutputDebugString OutputDebugStringA
};
#endif // cygwin

void Trace::message(const char* message) throw() {
  bassert(message, NullPointer(Type::getType<Trace>()));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::OutputDebugString(toWide(message).c_str());
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN) // special case
  win32::OutputDebugString(message);
#else // unix
  const char* ident;
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, message, "");
#else
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, message);
#endif
  closelog();
#endif // flavor
}

void Trace::member(const void* pointer, const char* message) throw() {
  bassert(message, NullPointer(Type::getType<Trace>()));
  const char* src = message;
  while (*src) {
    ++src;
  }
  unsigned int length = src - message;
  SimpleBuffer<char> buffer(22 + length + 1);
  // TAG: remove sprintf dependency
  sprintf(buffer, "%p >> %s", pointer, message); // sprintf must be MT-safe
#if (_DK_SDU_MIP__BASE__INT_SIZE > 8)
#  error pointer type not supported
#endif
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::OutputDebugString(toWide(buffer).c_str());
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN) // special case
  win32::OutputDebugString(buffer);
#else // unix
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, buffer, "");
#else
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, buffer);
#endif
  closelog();
#endif // flavor
}

Trace::Trace(const char* _msg) throw() : msg(_msg) {
  Trace::member(this, msg);
}

Trace::~Trace() throw() {
  Trace::member(this, msg);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
