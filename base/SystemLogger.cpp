/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/SystemLogger.h>
#include <base/Application.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <syslog.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void SystemLogger::write(MessageType type, const String& message) throw() {
#if defined(__win32__)
  static WORD messageType[] = {EVENTLOG_INFORMATION_TYPE, EVENTLOG_WARNING_TYPE, EVENTLOG_ERROR_TYPE};
  LPCTSTR source = Application::getApplication()->getFormalName().getElements();
  HANDLE eventSource = RegisterEventSource(NULL, source);
  if (eventSource != NULL) {
    LPCTSTR strings[1];
    strings[0] = message.getElements();
    ReportEvent(eventSource, messageType[type], 0, 0, NULL, 1, 0, strings, NULL);
    DeregisterEventSource(eventSource);
  }
#else // __unix__
  static int messageType[] = {LOG_INFO, LOG_WARNING, LOG_ERR};
  openlog(Application::getApplication()->getFormalName().getElements(), LOG_PID, 0);
  syslog(LOG_USER | messageType[type], message.getElements());
  closelog();
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
