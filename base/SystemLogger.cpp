/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/SystemLogger.h>
#include <base/Application.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #undef ERROR // protect against the evil programmers
#else // unix
  #include <syslog.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void SystemLogger::write(MessageType type, const String& message) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  static WORD messageType[] = {EVENTLOG_INFORMATION_TYPE, EVENTLOG_WARNING_TYPE, EVENTLOG_ERROR_TYPE};
  LPCTSTR source = Application::getApplication()->getFormalName().getElements();
  HANDLE eventSource = ::RegisterEventSource(0, source);
  if (eventSource != 0) {
    LPCTSTR strings[1];
    strings[0] = message.getElements();
    ::ReportEvent(eventSource, messageType[type], 0, 0, 0, 1, 0, strings, 0);
    ::DeregisterEventSource(eventSource);
  }
#else // unix
  static int messageType[] = {LOG_INFO, LOG_WARNING, LOG_ERR};
  openlog(Application::getApplication()->getFormalName().getElements(), LOG_PID, 0);
  syslog(LOG_USER | messageType[type], message.getElements());
  closelog();
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
