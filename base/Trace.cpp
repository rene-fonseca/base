/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #include <string.h>
  #include <stdio.h>
#else // unix
  #include <string.h>
  #include <stdio.h>
  #include <syslog.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void Trace::message(const char* message) throw() {
  assert(message, NullPointer(Type::getType<Trace>()));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::OutputDebugString(message);
#else // unix
  const char* ident;
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, message);
  closelog();
#endif // flavor
}

void Trace::member(const void* pointer, const char* message) throw() {
  assert(message, NullPointer(Type::getType<Trace>()));
  unsigned int length = strlen(message);
  char buffer[sizeof("0x1234567812345678 >> ") + length];
#if (_DK_SDU_MIP__BASE__INT_SIZE == 4)
  sprintf(buffer, "%08x >> %s", pointer, message); // sprintf must be MT-safe
#elif (_DK_SDU_MIP__BASE__INT_SIZE == 8)
  sprintf(buffer, "%016x >> %s", pointer, message); // sprintf must be MT-safe
#else
  #error pointer type not supported
#endif
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::OutputDebugString(buffer);
#else // unix
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
  syslog(LOG_USER | LOG_INFO/* | LOG_DEBUG*/, buffer);
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
