/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Base.h>
#include <base/Date.h>
#include <base/concurrency/Thread.h>
#include <base/Exception.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #include <time.h>
#else // unix
  #include <sys/time.h>
  #include <time.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
inline Date FileTimeToDate(const FILETIME& time) {
  return Date((*reinterpret_cast<const long long*>(&time) - 116444736000000000LL)/10000000); // TAG: 0x0000001c1a021060LL
}
#endif

Date Date::getNow() throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  GetSystemTimeAsFileTime(&buffer);
  return FileTimeToDate(buffer);
#else // unix
  return Date(::time(0));
#endif
}

Date Date::getTime(int second, int minute, int hour) throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  SYSTEMTIME time = {0, 0, 0, 0, hour, minute, second, 0};
  SystemTimeToFileTime(&time, &buffer);
  return FileTimeToDate(buffer);
#else // unix
  struct tm temp = {second, minute, hour, 0, 0, 0, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
#endif
}

Date Date::getDate(int day, int month, int year) throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  SYSTEMTIME time = {year, month, 0, day, 0, 0, 0, 0};
  SystemTimeToFileTime(&time, &buffer);
  return FileTimeToDate(buffer);
#else // unix
  struct tm temp = {0, 0, 0, day, month, year, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
#endif
}

Date Date::getDate(int second, int minute, int hour, int day, int month, int year) throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  SYSTEMTIME time = {year, month, 0, day, hour, minute, second, 0};
  SystemTimeToFileTime(&time, &buffer);
  return FileTimeToDate(buffer);
#else // unix
  struct tm temp = {second, minute, hour, day, month, year, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
#endif
}

int Date::getSecond() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wSecond;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_sec;
#endif
}

int Date::getMinute() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wMinute;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_min;
#endif
}

int Date::getHour() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wHour;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_hour;
#endif
}

int Date::getDay() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wDay;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_mday;
#endif
}

int Date::getDayOfWeek() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wDayOfWeek;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_wday;
#endif
}

int Date::getDayOfYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: not implemented
  SYSTEMTIME time;
  GetLocalTime(&time);
  //  return time.wYear;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_yday;
#endif
}

int Date::getMonth() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wMonth;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_mon;
#endif
}

int Date::getYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wYear;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_year;
#endif
}

int Date::getUTCSecond() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wSecond;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_sec;
#endif
}

int Date::getUTCMinute() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wMinute;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_min;
#endif
}

int Date::getUTCHour() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wHour;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_hour;
#endif
}

int Date::getUTCDay() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wDay;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_mday;
#endif
}

int Date::getUTCDayOfWeek() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wDayOfWeek;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_wday;
#endif
}

int Date::getUTCDayOfYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: not implemented
  //  SYSTEMTIME time;
  //  GetSystemTime(&time);
  //  return time.wYear;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_yday;
#endif
}

int Date::getUTCMonth() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wMonth;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_mon;
#endif
}

int Date::getUTCYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wYear;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_year;
#endif
}

String Date::format(const String& format, bool local) const throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct tm time;
  if (local) {
    localtime_r(&static_cast<time_t>(date), &time);
  } else {
    gmtime_r(&static_cast<time_t>(date), &time); // MT-safe
  }
  size_t result = strftime(buffer->getElements(), buffer->getSize(), format.getElements(), &time);
  return String(buffer->getElements(), result);
#endif
}

WideString Date::format(const WideString& format, bool local) const throw(MemoryException) {
#if defined(_DK_SDU_MIP__BASE__WCSFTIME)
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct tm time;
  if (local) {
    localtime_r(&static_cast<time_t>(date), &time);
  } else {
    gmtime_r(&static_cast<time_t>(date), &time); // MT-safe
  }
  size_t result = wcsftime(pointer_cast<wchar_t*>(buffer->getElements()), buffer->getSize()/sizeof(wchar_t), format.getElements(), &time);
  return WideString(pointer_cast<const wchar_t*>(buffer->getElements()), result);
#endif
#else
  #warning WideString Date::format(const WideString& format, bool local) const throw(MemoryException) not available
  return WideString();
#endif
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Date& value) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  int date = value.getValue();
  return stream << ctime(&(time_t)date); // TAG: not MT-safe
#else // unix
  char buffer[256];
  int date = value.getValue();
  return stream << ctime_r(&static_cast<time_t>(date), buffer); // TAG: depends on specific API
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
