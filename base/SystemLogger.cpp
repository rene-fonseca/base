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
#include <base/SystemLogger.h>
#include <base/Application.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef ERROR // protect against the evil programmers
#else // unix
#  include <syslog.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void SystemLogger::write(MessageType type, const String& message) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static WORD messageType[] = {EVENTLOG_INFORMATION_TYPE, EVENTLOG_WARNING_TYPE, EVENTLOG_ERROR_TYPE};
  LPCTSTR source;
  HANDLE eventSource;
  Application* application = Application::getApplication();
  if (application) {
    eventSource = ::RegisterEventSource(0, application->getFormalName().getElements());
  } else {
    eventSource = ::RegisterEventSource(0, "Unspecified");
  }
  if (eventSource != 0) {
    LPCTSTR strings[1];
    strings[0] = message.getElements();
    ::ReportEvent(eventSource, messageType[type], 0, 0, 0, 1, 0, strings, 0);
    ::DeregisterEventSource(eventSource);
  }
#else // unix
  // TAG: not MT-safe
  static int messageType[] = {LOG_INFO, LOG_WARNING, LOG_ERR};
  Application* application = Application::getApplication();
  if (application) {
    String formalName = application->getFormalName();
    openlog(formalName.getElements(), LOG_PID, 0);
    syslog(LOG_USER | messageType[type], message.getElements());
  } else {
    openlog("Unspecified", LOG_PID, 0);
    syslog(LOG_USER | messageType[type], message.getElements());
  }
  closelog();
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
