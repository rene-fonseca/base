/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Date.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/**
  Requirements:
    int must be compatible with time_t.
*/

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Date Date::getNow() throw(DateException) {
  struct timeval tv;
  if (gettimeofday(&tv, 0)) {
    throw DateException("Unable to get local time");
  }
  return Date(tv.tv_sec);
}

Date Date::getTime(int seconds, int minutes, int hours) throw(DateException) {
  struct tm temp;
  fill<char>((char*)&temp, 0, sizeof(temp));
  temp.tm_sec = seconds;
  temp.tm_min = minutes;
  temp.tm_hour = hours;
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
}

Date Date::getDate(int day, int month, int year) throw(DateException) {
  struct tm temp;
  fill<char>((char*)&temp, 0, sizeof(temp));
  temp.tm_mday = day;
  temp.tm_mon = month;
  temp.tm_year = year;
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
}

Date Date::getDate(int seconds, int minutes, int hours, int day, int month, int year) throw(DateException) {
  struct tm temp;
  fill<char>((char*)&temp, 0, sizeof(temp));
  temp.tm_sec = seconds;
  temp.tm_min = minutes;
  temp.tm_hour = hours;
  temp.tm_mday = day;
  temp.tm_mon = month;
  temp.tm_year = year;
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date");
  }
  return Date(result);
}

int Date::getSeconds() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_sec;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_sec;
#endif
}

int Date::getMinutes() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_min;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_min;
#endif
}

int Date::getHours() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_hour;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_hour;
#endif
}

int Date::getDay() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_mday;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_mday;
#endif
}

int Date::getDayOfWeek() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_wday;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_wday;
#endif
}

int Date::getDayOfYear() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_yday;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_yday;
#endif
}

int Date::getMonth() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_mon;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_mon;
#endif
}

int Date::getYear() const throw() {
#if defined(__win32__)
  struct tm* result = localtime(&(time_t)date);
  return result->tm_year;
#else // __unix__
  struct tm result;
  localtime_r(&(time_t)date, &result);
  return result.tm_year;
#endif
}

int Date::getUTCSeconds() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result->tm_sec;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_sec;
#endif
}

int Date::getUTCMinutes() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result.tm_min;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_min;
#endif
}

int Date::getUTCHours() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result->tm_hour;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_hour;
#endif
}

int Date::getUTCDay() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result->tm_mday;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_mday;
#endif
}

int Date::getUTCDayOfWeek() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result->tm_wday;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_wday;
#endif
}

int Date::getUTCDayOfYear() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result->tm_yday;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_yday;
#endif
}

int Date::getUTCMonth() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result->tm_mon;
#else // __unix__
  struct tm result;
  gmtime_r(&(time_t)date, &result); // MT-safe
  return result.tm_mon;
#endif
}

int Date::getUTCYear() const throw() {
#if defined(__win32__)
  struct tm* result = gmtime(&(time_t)date);
  return result->tm_year;
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
