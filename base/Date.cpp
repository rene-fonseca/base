/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Date.h>

#if defined(__win32__)
  #include <windows.h>
  #include <time.h>
#else // __unix__
  #include <sys/time.h>
  #include <time.h>
  #include <unistd.h>
#endif

/**
  Requirements:
    int must be compatible with time_t.
*/

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(__win32__)
inline Date FileTimeToDate(const FILETIME& time) {
  ASSERT(sizeof(FILETIME) == sizeof(long long));
  return Date((*(long long*)(&time) - 116444736000000000LL)/10000000); // TAG: 0x0000001c1a021060LL
}
#endif

Date Date::getNow() throw(DateException) {
#if defined(__win32__)
  FILETIME buffer;
  GetSystemTimeAsFileTime(&buffer);
  return FileTimeToDate(buffer);
#else // __unix__
  return Date(::time(NULL));
#endif
}

Date Date::getTime(int second, int minute, int hour) throw(DateException) {
#if defined(__win32__)
  FILETIME buffer;
  SYSTEMTIME time = {0, 0, 0, 0, hour, minute, second, 0};
  SystemTimeToFileTime(&time, &buffer);
  return FileTimeToDate(buffer);
#else // __unix__
  struct tm temp = {seconds, minute, hour, 0, 0, 0, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
#endif
}

Date Date::getDate(int day, int month, int year) throw(DateException) {
#if defined(__win32__)
  FILETIME buffer;
  SYSTEMTIME time = {year, month, 0, day, 0, 0, 0, 0};
  SystemTimeToFileTime(&time, &buffer);
  return FileTimeToDate(buffer);
#else // __unix__
  struct tm temp = {0, 0, 0, day, month, year, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
#endif
}

Date Date::getDate(int second, int minute, int hour, int day, int month, int year) throw(DateException) {
#if defined(__win32__)
  FILETIME buffer;
  SYSTEMTIME time = {year, month, 0, day, hour, minute, second, 0};
  SystemTimeToFileTime(&time, &buffer);
  return FileTimeToDate(buffer);
#else // __unix__
  struct tm temp = {second, minute, hour, day, month, year, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
#endif
}

int Date::getSecond() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wSecond;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_sec;
#endif
}

int Date::getMinute() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wMinute;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_min;
#endif
}

int Date::getHour() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wHour;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_hour;
#endif
}

int Date::getDay() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wDay;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_mday;
#endif
}

int Date::getDayOfWeek() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wDayOfWeek;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_wday;
#endif
}

int Date::getDayOfYear() const throw() {
#if defined(__win32__)
  // TAG: not implemented
  SYSTEMTIME time;
  GetLocalTime(&time);
  //  return time.wYear;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_yday;
#endif
}

int Date::getMonth() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wMonth;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_mon;
#endif
}

int Date::getYear() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetLocalTime(&time);
  return time.wYear;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_year;
#endif
}

int Date::getUTCSecond() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wSecond;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_sec;
#endif
}

int Date::getUTCMinute() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wMinute;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_min;
#endif
}

int Date::getUTCHour() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wHour;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_hour;
#endif
}

int Date::getUTCDay() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wDay;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_mday;
#endif
}

int Date::getUTCDayOfWeek() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wDayOfWeek;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_wday;
#endif
}

int Date::getUTCDayOfYear() const throw() {
#if defined(__win32__)
  // TAG: not implemented
  //  SYSTEMTIME time;
  //  GetSystemTime(&time);
  //  return time.wYear;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_yday;
#endif
}

int Date::getUTCMonth() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wMonth;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_mon;
#endif
}

int Date::getUTCYear() const throw() {
#if defined(__win32__)
  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wYear;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_year;
#endif
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Date& value) {
#if defined(__win32__)
  int date = value.getValue();
  return stream << ctime(&(time_t)date); // not MT-safe
#else // __unix__
  char buffer[256];
  int date = value.getValue();
  return stream << ctime_r(&(time_t)date, buffer);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
