/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Trace.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #include <string.h>
  #include <stdio.h>
#else // unix
  #include <string.h>
  #include <stdio.h>
  #include <syslog.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void Trace::message(const char* message) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::OutputDebugString(message);
#else // unix
  const char* ident;
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
  syslog(LOG_USER | LOG_INFO, message);
  closelog();
//  fprintf(stderr, "TRACE %s\n", message); // fprintf must be MT-safe
#endif // flavour
}

void Trace::member(const void* ptr, const char* message) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int length = strlen(message);
  char buffer[sizeof("0x1234567812345678 >> ") + length];
  sprintf(buffer, "%08x >> %s", ptr, message); // sprintf must be MT-safe
  ::OutputDebugString(buffer);
#else // unix
  unsigned int length = strlen(message);
  char buffer[sizeof("0x1234567812345678 >> ") + length];
  sprintf(buffer, "%08x >> %s", ptr, message); // sprintf must be MT-safe
  openlog("TRACE", LOG_PID, 0); // TAG: fixme - do not reopen
  syslog(LOG_USER | LOG_INFO, message);
  closelog();
//  fprintf(stderr, "TRACE %08x >> %s\n", ptr, message); // fprintf must be MT-safe
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
