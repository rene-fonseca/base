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
#include <base/SystemLogger.h>
#include <base/Application.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef ERROR // protect against the evil programmers
#else // unix
#  include <syslog.h>
#endif // flavor

#include <locale>
#include <codecvt>
#include <base/string/WideString.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// convert utf8* to wchar*
// convert wchar* to utf8*
// TAG: add support for \0 in strings!

/** Converts wstring to UTF-8 string. */
std::string toUTF8(const std::wstring& s) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.to_bytes(s);
}

/** Converts UTF-8 string to wstring. */
std::wstring toWide(const std::string& s) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s);
}

/** Converts String to wstring. */
std::wstring toWide(const String& s) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s.getElements(), s.getElements() + s.getLength());
}

/** Converts WideString to wstring. */
std::wstring toWide(const WideString& s) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
  std::string r = convert.to_bytes((const char32_t*)s.getElements(), (const char32_t*)s.getElements() + s.getLength());
  return toWide(r);
}

// std::u32string

// TAG: need to figure out String and WideString API - use string and wstring instead?

void SystemLogger::write(MessageType type, const String& message) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static WORD messageType[] = {
    EVENTLOG_INFORMATION_TYPE,
    EVENTLOG_WARNING_TYPE,
    EVENTLOG_ERROR_TYPE
  };
  HANDLE eventSource;
  Application* application = Application::getApplication();
  if (application) {
    eventSource = ::RegisterEventSource(
      nullptr,
      toWide(application->getFormalName()).c_str()
    );
  } else {
    eventSource = ::RegisterEventSource(nullptr, L"Unspecified");
  }
  if (eventSource != 0) {
    LPCTSTR strings[1];
    std::wstring w = toWide(message);
    strings[0] = w.c_str();
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
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
    syslog(LOG_USER | messageType[type], message.getElements(), "");
#else
    syslog(LOG_USER | messageType[type], message.getElements());
#endif
  } else {
    openlog("Unspecified", LOG_PID, 0);
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
    syslog(LOG_USER | messageType[type], message.getElements(), "");
#else
    syslog(LOG_USER | messageType[type], message.getElements());
#endif
  }
  closelog();
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
