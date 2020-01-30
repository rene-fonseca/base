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
#include <base/SystemLogger.h>
#include <base/Application.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef ERROR // protect against the evil programmers
#else // unix
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <syslog.h>
#endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void SystemLogger::write(MessageType type, const String& message) noexcept
{
  if (!message) {
    return;
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static WORD messageType[] = {
    EVENTLOG_INFORMATION_TYPE,
    EVENTLOG_WARNING_TYPE,
    EVENTLOG_ERROR_TYPE
  };
  HANDLE eventSource = 0;
  Application* application = Application::getApplication();
  if (application) {
    eventSource = ::RegisterEventSourceW(
      nullptr,
      ToWCharString(application->getFormalName())
    );
  } else {
    eventSource = ::RegisterEventSourceW(nullptr, L"Unspecified");
  }
  if (eventSource != 0) {
    LPCTSTR strings[1];
    ToWCharString w(message);
    strings[0] = w;
    ::ReportEventW(eventSource, messageType[type], 0, 0, 0, 1, 0, strings, 0);
    ::DeregisterEventSource(eventSource);
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else // unix
  // TAG: not MT-safe
  static int messageType[] = { LOG_INFO, LOG_WARNING, LOG_ERR };
  Application* application = Application::getApplication();
  if (application) {
    String formalName = application->getFormalName();
    openlog(formalName.getElements(), LOG_PID, 0);
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    syslog(LOG_USER | messageType[type], message.getElements(), "");
#else
    syslog(LOG_USER | messageType[type], message.getElements(), "");
#endif
  }
  else {
    openlog("Unspecified", LOG_PID, 0);
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    syslog(LOG_USER | messageType[type], message.getElements(), "");
#else
    syslog(LOG_USER | messageType[type], message.getElements(), "");
#endif
  }
  closelog();
#endif // flavor
}

void SystemLogger::write(MessageType type, const WideString& message) noexcept
{
  write(type, String(message));
}

void SystemLogger::write(MessageType type, const char* message) noexcept
{
  write(type, String(message));
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
