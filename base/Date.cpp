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
#include <base/Date.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/string/Locale.h>
#include <base/NotImplemented.h>

#define _DK_SDU_MIP__BASE__WXP 6 // TAG: FIXME

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #include <time.h>
#else // unix
  #include <sys/time.h>
  #include <time.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

const int Date::DAYS_PER_MONTH_NONLEAP_YEAR[Date::MONTHS_PER_YEAR] = {
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
const int Date::DAYS_PER_MONTH_LEAP_YEAR[Date::MONTHS_PER_YEAR] = {
  31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
const int Date::DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[Date::MONTHS_PER_YEAR + 1] = {
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};
const int Date::DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[Date::MONTHS_PER_YEAR + 1] = {
  0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
};

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
inline Date FileTimeToDate(const FILETIME& time) throw() {
  return Date((*(unsigned long long*)(&time) - 116444736000000000ULL)/10000000);
}
#endif

int Date::normalize(DateTime& dateTime, bool redundancy) throw() {
  int carrier; // and borrow
  
  int month = dateTime.month;
  carrier = month/MONTHS_PER_YEAR;
  dateTime.month = month%MONTHS_PER_YEAR;
  if (month < 0) {
    --carrier; // borrow
    dateTime.month += MONTHS_PER_YEAR;
  }

  long long year = dateTime.year + carrier;

  carrier = dateTime.millisecond/1000;
  dateTime.millisecond %= 1000;
  if (dateTime.millisecond < 0) {
    --carrier; // borrow
    dateTime.millisecond += 1000;
  }

  long long second = static_cast<long long>(dateTime.second) + carrier;
  carrier = second/SECONDS_PER_MINUTE;
  dateTime.second = second%SECONDS_PER_MINUTE;
  if (second < 0) {
    --carrier; // borrow
    dateTime.second += SECONDS_PER_MINUTE;
  }
  
  long long minute = static_cast<long long>(dateTime.minute) + carrier;
  carrier = minute/MINUTES_PER_HOUR;
  dateTime.minute = minute%MINUTES_PER_HOUR;
  if (minute < 0) {
    --carrier; // borrow
    dateTime.minute += MINUTES_PER_HOUR;
  }
  
  long long hour = static_cast<long long>(dateTime.hour) + carrier;
  carrier = hour/HOURS_PER_DAY;
  dateTime.hour = hour%HOURS_PER_DAY;
  if (hour < 0) {
    --carrier; // borrow
    dateTime.hour += HOURS_PER_DAY;
  }

  long long day = static_cast<long long>(dateTime.day) + carrier;
  int daysInMonth = isLeapYear(year) ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month];
  if (day >= daysInMonth) { // traverse forward in time
    year += day/DAYS_PER_400_YEARS;
    day %= DAYS_PER_400_YEARS; // does not change leap year and month
    
    bool leapYear = isLeapYear(year);
    int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
    while (day >= daysInYear) { // maximum ~400 loops
      day -= daysInYear; // does not change month
      ++year;
      leapYear = isLeapYear(year);
      daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
    }
    
    int daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
    while (day >= daysInMonth) { // maximum 12 loops
      day -= daysInMonth;
      ++month;
      if (month == MONTHS_PER_YEAR) {
        ++year;
        leapYear = isLeapYear(year);
        month = 0;
      }
      daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
    }
  } else if (day < 0) { // traverse backward in time
    year += day/DAYS_PER_400_YEARS;
    day %= DAYS_PER_400_YEARS; // does not change leap year and month
    
    bool leapYear = isLeapYear(year);
    int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
    while (day <= daysInYear) { // maximum ~400 loops
      --year;
      leapYear = isLeapYear(year);
      daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
      day += daysInYear; // does not change month
    }
    
    int daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
    while (day < 0) { // maximum 12 loops
      --month;
      if (month < 0) {
        --year;
        leapYear = isLeapYear(year);
        month = MONTHS_PER_YEAR - 1;
      }
      daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
      day += daysInMonth;
    }
  }
  
  carrier = year/10000;
  
  dateTime.day = day;
  dateTime.month = month;
  dateTime.year = year%10000;
  
  if (redundancy) {
    dateTime.dayOfYear = day + (isLeapYear(dateTime.year) ?
                                DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[month] :
                                DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[month]);

    int a = (month <= 1) ? 1 : 0; // (14 - month)/MONTHS_PER_YEAR
    year -= a;
    // Gregorian calendar
    dateTime.weekday = (day + year + year/4 - year/100 + year/400 + 31 * (month + 1 + MONTHS_PER_YEAR * a - 2)/MONTHS_PER_YEAR)%7;

    int y = year + 4800; // 'a' is subtracted above
    int m = month + 1 + MONTHS_PER_YEAR * a - 3;
    int JD = day + (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045; // Julian day - Gregorian calendar

    int d4 = (JD + 31741 - (JD % 7)) % 146097 % 36524 % 1461;
    int L  = d4/1460;
    int d1 = ((d4 - L) % 365) + L;
    dateTime.week = d1/7 + 1;
  }
  
  return carrier; // unlikely to be non-zero
}

int Date::getDaysOfMonth(int month, int year) throw(OutOfDomain) {
  assert((month >= 0) && (month < MONTHS_PER_YEAR), OutOfDomain(Type::getType<Date>()));
  return isLeapYear(year) ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month];
}

int Date::getDayOfWeek(int day, int month, int year) throw() {
  DateTime dt;
  dt.year = year;
  dt.month = month;
  dt.day = day;
  dt.hour = 0;
  dt.minute = 0;
  dt.second = 0;
  dt.millisecond = 0;
  normalize(dt, false);
  int a = (dt.month <= 1) ? 1 : 0;
  year -= a;
  return (dt.day + dt.year + dt.year/4 - dt.year/100 + dt.year/400 + 31 * (dt.month + 1 + MONTHS_PER_YEAR * a - 2)/MONTHS_PER_YEAR)%7;
}

int Date::getWeek(int day, int month, int year) throw() {
  DateTime dt;
  dt.year = year;
  dt.month = month;
  dt.day = day;
  dt.hour = 0;
  dt.minute = 0;
  dt.second = 0;
  dt.millisecond = 0;
  normalize(dt, false);
  int a = (dt.month <= 1) ? 1 : 0; // (14 - dt.month)/MONTHS_PER_YEAR
  int y = dt.year + 4800 - a;
  int m = dt.month + 1 + MONTHS_PER_YEAR * a - 3;
  int JD = dt.day + (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045; // Julian day - Gregorian calendar
  
  int d4 = (JD + 31741 - (JD % 7)) % 146097 % 36524 % 1461;
  int L  = d4/1460;
  int d1 = ((d4 - L) % 365) + L;
  return d1/7 + 1;
}

int Date::getDayOfYear(int day, int month, int year) throw() {
  DateTime dt;
  dt.year = year;
  dt.month = month;
  dt.day = day;
  dt.hour = 0;
  dt.minute = 0;
  dt.second = 0;
  dt.millisecond = 0;
  normalize(dt, false);
  return dt.day +
    (isLeapYear(dt.year) ?
     DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[dt.month] :
     DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[dt.month]);
}

Date Date::getNow() throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  ::GetSystemTimeAsFileTime(&buffer);
  return FileTimeToDate(buffer);
#else // unix
  return Date(::time(0));
#endif
}

int Date::getBias() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  TIME_ZONE_INFORMATION information;
  DWORD status = ::GetTimeZoneInformation(&information);
  ASSERT(status != TIME_ZONE_ID_INVALID);
  return information.Bias * 60; // TAG: what about the other bias'
#else // unix
  tzset(); // update variables
  return timezone;
#endif // flavor
}

Date Date::getTime(int second, int minute, int hour, bool local) throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  SYSTEMTIME time = {0, 0, 0, 0, hour, minute, second, 0};
#if (_DK_SDU_MIP__BASE__OS >= _DK_SDU_MIP__BASE__WXP)
  if (local) {
    assert(::TzSpecificLocalTimeToSystemTime(0, &time, &time) != 0, DateException(Type::getType<Date>()));
  }
#endif
  assert(::SystemTimeToFileTime(&time, &buffer), DateException(Type::getType<Date>()));
#if (_DK_SDU_MIP__BASE__OS < _DK_SDU_MIP__BASE__WXP)
  if (local) {
    assert(::LocalFileTimeToFileTime(&buffer, &buffer) != 0, DateException(Type::getType<Date>()));
  }
#endif
  return FileTimeToDate(buffer);
#else // unix
  struct tm temp = {second, minute, hour, 0, 0, 0, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) { // TAG: fixme
    throw DateException("Unable to represent date", Type::getType<Date>());
  }
  if (!local) {
    result -= timezone; // convert to UTC
  }
  return Date(result);
#endif
}

Date Date::getDate(int day, int month, int year, bool local) throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  SYSTEMTIME time = {year, month, 0, day, 0, 0, 0, 0};
#if (_DK_SDU_MIP__BASE__OS >= _DK_SDU_MIP__BASE__WXP)
  if (local) {
    assert(::TzSpecificLocalTimeToSystemTime(0, &time, &time) != 0, DateException(Type::getType<Date>()));
  }
#endif
  assert(::SystemTimeToFileTime(&time, &buffer), DateException(Type::getType<Date>()));
#if (_DK_SDU_MIP__BASE__OS < _DK_SDU_MIP__BASE__WXP)
  if (local) {
    ::LocalFileTimeToFileTime(&buffer, &buffer);
  }
#endif
  return FileTimeToDate(buffer);
#else // unix
  struct tm temp = {0, 0, 0, day, month, year, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) { // TAG: fixme
    throw DateException("Unable to represent date", Type::getType<Date>());
  }
  if (!local) {
    result -= timezone; // convert to UTC
  }
  return Date(result);
#endif
}

Date Date::getDate(int second, int minute, int hour, int day, int month, int year, bool local) throw(DateException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME buffer;
  SYSTEMTIME time = {year, month, 0, day, hour, minute, second, 0};
#if (_DK_SDU_MIP__BASE__OS >= _DK_SDU_MIP__BASE__WXP)
  if (local) {
    assert(::TzSpecificLocalTimeToSystemTime(0, &time, &time) != 0, DateException(Type::getType<Date>()));
  }
#endif
  assert(::SystemTimeToFileTime(&time, &buffer), DateException(Type::getType<Date>()));
#if (_DK_SDU_MIP__BASE__OS < _DK_SDU_MIP__BASE__WXP)
  if (local) {
    assert(::LocalFileTimeToFileTime(&buffer, &buffer) != 0, DateException(Type::getType<Date>()));
  }
#endif
  return FileTimeToDate(buffer);
#else // unix
  struct tm temp = {second, minute, hour, day, month, year, 0, 0, 0};
  int result;
  if ((result = mktime(&temp)) == -1) {
    throw DateException("Unable to represent date", Type::getType<Date>());
  }
  if (!local) {
    result -= timezone; // convert to UTC
  }
  return Date(result);
#endif
}

Date::Date(const DateTime& dateTime) throw() {
  DateTime dt = dateTime;
  normalize(dt, false);
  int seconds = dt.second + SECONDS_PER_MINUTE * dt.minute + SECONDS_PER_HOUR * dt.hour;
  long long days = dt.day +
    (isLeapYear(dt.year) ?
     DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[dt.month] :
     DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[dt.month]);
  
  if (dt.year >= EPOCH_YEAR) { // positive seconds contribution
    int leapYears = 0;
    int lastYear = dt.year - 1; // exclude current year
    if (lastYear >= 2000) {
      leapYears = ((lastYear - 2000)/4 + 1) + -((lastYear - 2000)/100 + 1) + ((lastYear - 2000)/400 + 1);
    } else if (lastYear >= 1972) {
      leapYears += (lastYear - 1972)/4 + 1;
    }
    days += DAYS_PER_NONLEAP_YEAR * static_cast<long long>(dt.year - EPOCH_YEAR) +
      (DAYS_PER_LEAP_YEAR-DAYS_PER_NONLEAP_YEAR) * static_cast<long long>(leapYears);
  } else { // negative seconds contribution
    int leapYears = 0;
    int lastYear = dt.year; // include current year!
    if (lastYear <= 1900) {
      leapYears = ((1900 - lastYear)/4 + 1) + -((1900 - lastYear)/100 + 1) + ((1900 - lastYear)/400 + 1);
    } else if (lastYear <= 1968) {
      leapYears += (1968 - lastYear)/4 + 1;
    }
    days -= DAYS_PER_NONLEAP_YEAR * static_cast<long long>(EPOCH_YEAR - dt.year) +
      (DAYS_PER_LEAP_YEAR-DAYS_PER_NONLEAP_YEAR) * static_cast<long long>(leapYears);
  }
  
  date = seconds + days * SECONDS_PER_DAY;
}

// int Date::getMillisecond() const throw() {
//   ...
// }

int Date::getSecond() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  return time.wSecond;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_sec;
#endif
}

int Date::getMinute() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  return time.wMinute;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_min;
#endif
}

int Date::getHour() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  return time.wHour;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_hour;
#endif
}

int Date::getDay() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  return time.wDay;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_mday;
#endif
}

int Date::getDayOfWeek() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  return time.wDayOfWeek;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_wday;
#endif
}

int Date::getDayOfYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(this);
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  //  return time.wYear;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_yday;
#endif
}

int Date::getMonth() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  return time.wMonth;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_mon;
#endif
}

int Date::getYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert((::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time) != 0) &&
         (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
         DateException(this));
  return time.wYear;
#else // unix
  struct tm result;
  localtime_r(&static_cast<time_t>(date), &result);
  return result.tm_year;
#endif
}

int Date::getUTCSecond() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  return time.wSecond;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_sec;
#endif
}

int Date::getUTCMinute() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  return time.wMinute;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_min;
#endif
}

int Date::getUTCHour() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  return time.wHour;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_hour;
#endif
}

int Date::getUTCDay() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  return time.wDay;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_mday;
#endif
}

int Date::getUTCDayOfWeek() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  return time.wDayOfWeek;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_wday;
#endif
}

int Date::getUTCDayOfYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(this);
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
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  return time.wMonth;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_mon;
#endif
}

int Date::getUTCYear() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEMTIME time;
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  return time.wYear;
#else // unix
  struct tm result;
  gmtime_r(&static_cast<time_t>(date), &result); // MT-safe
  return result.tm_year;
#endif
}

Date Date::getDateByJulianDay(int julianDay) throw() {
  int a = julianDay + 32044;
  int b = (4 * a + 3)/146097;
  int c = a - (b * 146097)/4;
  int d = (4 * c + 3)/1461;
  int e = c - (1461 * d)/4;
  int m = (5 * e + 2)/153;
  int day = e - (153 * m + 2)/5 + 1 - 1;
  int month = m + 3 - MONTHS_PER_YEAR * m/10 - 1;
  int year = 100 * b + d - 4800 + m/10;
  return getDate(day, month, year);
}

int Date::getJulianDay() const throw() {
  DateTime dt;
  split(dt);
  int a = (dt.month <= 1) ? 1 : 0; // (14 - dt.month)/MONTHS_PER_YEAR
  int y = dt.year + 4800 - a;
  int m = dt.month + MONTHS_PER_YEAR * a - 3;
  return dt.day + (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045; // Gregorian calendar
}

void Date::split(DateTime& result, bool local) const throw() {
  // if local then add timezone bias
//   // milliseconds?
//   int days = date/SECONDS_PER_DAY;
//   int seconds = date%SECONDS_PER_DAY;
//   if (seconds < 0) {
//     seconds += SECONDS_PER_DAY;
//     --days;
//   }
//   result.hour = seconds/SECONDS_PER_HOUR;
//   seconds %= SECONDS_PER_HOUR;
//   result.minute = seconds/SECONDS_PER_MINUTE;
//   result.second = seconds%SECONDS_PER_MINUTE;

//   int weekday = (EPOCH_WEEKDAY + days)%DAYS_PER_WEEK;
//   if (weekday < 0) {
//     weekday += DAYS_PER_WEEK;
//   }
//   result.weekday = weekday;

// TAG: fixme - calc. week
  
//   if (days >= 0) {
//     int year = EPOCH_YEAR;
//     while (true) { // TAG: need otimization
//       bool leapYear = isLeapYear(year);
//       int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
//       if (days < daysInYear) {
//         break;
//       }
//       ++year;
//       days -= daysInYear;
//     }
//   } else {
//     do { // TAG: need otimization
//       --year;
//       bool leapYear = isLeapYear(year);
//       int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
//       days += daysInYear;
//     } while (days < 0);
//   }
//   result.year = year;
//   result.dayOfYear = days;
  
//   bool leapYear = isLeapYear(year);
//   const int daysBeforeMonth[MONTHS_PER_YEAR] = leapYear ? DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR : DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR;
//   int month = 1;
//   while (days < daysBeforeMonth[month]) { // remember that index 12 is ok
//     ++month;
//   }
//   --month;
//   days -= daysBeforeMonth[month];
//   result.day = days;

  // TAG: need Daylight Saving Time (DST) support + local time support
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned long long fileTime = date * 10000000ULL + 116444736000000000ULL;
  // TAG: convert to local time when required
  SYSTEMTIME time;
  assert(::FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&fileTime), &time), DateException(this));
  result.year = time.wYear;
  result.month = time.wMonth - 1;
  result.day = time.wDay - 1;
  result.weekday = time.wDayOfWeek;
  result.hour = time.wHour;
  result.minute = time.wMinute;
  result.second = time.wSecond;
  result.millisecond = time.wMilliseconds;
#else // unix
  struct tm time;
  if (local) {
    localtime_r(&static_cast<time_t>(date), &time);
  } else {
    gmtime_r(&static_cast<time_t>(date), &time); // MT-safe
  }
  result.year = time.tm_year + 1900;
  result.month = time.tm_mon;
  result.day = time.tm_mday - 1;
  result.weekday = time.tm_wday;
  result.hour = time.tm_hour;
  result.minute = time.tm_min;
  result.second = time.tm_sec;
  result.millisecond = 0;
#endif
}

String Date::format(const String& format, bool local) const throw(InvalidFormat, MemoryException) {
  // time zones?
  DateTime dateTime;
  split(dateTime, local);
  
  // name of current locale
  // name of time zone abbrev. DK??? se W3C.org - 2 chars
  
  String::ReadIterator i = format.getBeginReadIterator();
  String::ReadIterator end = format.getEndReadIterator();
  StringOutputStream stream;

  enum Flags {
    DEFAULT = 1
  };
  
  while (i != end) {
    if (*i == '%') {
      ++i; // skip %
      assert(i != end, InvalidFormat(this));
      char code = *i;
      unsigned int flags = DEFAULT;
      bool readFlags = true;
      while (readFlags) {
        switch (code) {
        case '#': // select alternate format
          flags &= ~DEFAULT;
          break;
        default:
          readFlags = false;
        }
        if (readFlags) {
          ++i; // skip flag
          assert(i != end, InvalidFormat(this));
          code = *i; // get code
        }
      }
      switch (code) {
      case 'a':
        stream << Locale::POSIX.getShortNameOfWeekday(dateTime.weekday); // use locale
        break;
      case 'A':
        stream << Locale::POSIX.getLongNameOfWeekday(dateTime.weekday); // use locale
        break;
      case 'b':
        stream << Locale::POSIX.getShortNameOfMonth(dateTime.month); // use locale
        break;
      case 'B':
        stream << Locale::POSIX.getLongNameOfMonth(dateTime.month); // use locale
        break;
      case 'c':
        // locale's appropriate date and time representation
        if (flags & DEFAULT) {
          // long date time format for current locale
        } else {
          // date time format for current locale
        }
        break;
      case 'C':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.year/100;
        break;
      case 'd':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.day + 1;
        break;
      case 'D':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.month << '/';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.day + 1 << '/';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.year % 100;
        break;
      case 'e':
        stream << dateTime.day + 1;
        break;
      case 'F': // ISO 8601:2000 standard date format 2002-00-00
        stream << setWidth(4);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.year << '-';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.month << '-';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.day + 1;
        break;
      case 'g':
        break;
      case 'h': // same as %b
        stream << Locale::POSIX.getShortNameOfMonth(dateTime.month); // use locale
        break;        
      case 'H':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.hour;
        break;
      case 'I':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << ((dateTime.hour % 12 == 0) ? 12 : (dateTime.hour % 12));
        break;
      case 'j':
        stream << setWidth(3);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.dayOfYear + 1;
        break;
      case 'm':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.month;
        break;
      case 'M':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.minute;
        break;
      case 'n': // newline
        stream << EOL;
        break;
      case 'p':
        if (dateTime.hour >= 12) { // TAG: what is hour could be 24
          stream << Locale::POSIX.getPM(); // use locale
        } else {
          stream << Locale::POSIX.getAM(); // use locale
        }
        break;
      case 'r': // time in a.m. and p.m. notation
        // TAG: fixme - use locale
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << ((dateTime.hour % 12 == 0) ? 12 : (dateTime.hour % 12)) << ':'; // use locale
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.minute << ':'; // use locale
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.second << ' '; // use locale
        if (dateTime.hour >= 12) { // TAG: what is hour could be 24
          stream << Locale::POSIX.getPM(); // use locale
        } else {
          stream << Locale::POSIX.getAM(); // use locale
        }
        break;
      case 'R': // %H:%M
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.hour << ':';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.minute;
        break;
      case 'S':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.second;
        break;
      case 't':
        stream << ASCIITraits::HT;
        break;
      case 'T': // %H:%M:%S
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.hour << ':';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.minute << ':';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.second;
        break;
      case 'u': // weekday with 1 ~ Monday
        stream << dateTime.weekday; // fixme
        break;
      case 'U': // First Sunday is the first day of week 1
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.week;
        // week of year with Sunday as first day of week 00-53
        break;
      case 'V': // First Monday is the first day of week 1 - read standard!
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.week; // 01-53 // TAG: fixme
        break;
      case 'w': // 0 is Sunday
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.weekday;
        break;
      case 'W':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.week; // TAG: fixme
        // week of year with Monday as first day of week 00-53
        break;
      case 'x': // locales date representation
        stream << "<locales date repre>"; // fixme
        break;
      case 'X': // locales time representation
        stream << "<locales time repre>"; // fixme
        break;
      case 'y':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.year % 100;
        break;
      case 'Y':
        stream << setWidth(4);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.year;
        break;
      case 'z': // offset from UTC +hhmm/-hhmm
        {
          int bias = +1234; // TAG: FIXME
          ASSERT((bias >= -(12*60 + 59)) && (bias <= (12*60 + 59))); // 
          if (bias > 0) {
            stream << '+' << setWidth(2) << ZEROPAD << bias/60 << setWidth(2) << ZEROPAD << bias%60;
          } else {
            stream << '-' << setWidth(2) << ZEROPAD << -bias/60 << setWidth(2) << ZEROPAD << -bias%60;
          }
        }
        break;
      case 'Z': // time zone abbreviation
        stream << 'T' << 'Z'; // fixme
        break;
      case '%':
        stream << '%';
        break;
      case '.': // not part of Single UNIX specification
        stream << setWidth(3);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.millisecond;
        break;
      default:
        throw InvalidFormat(this);
      }
    } else {
      stream << *i;
    }
    ++i; // skip code or normal character
  }
  stream << FLUSH;
  return stream.getString();
}

WideString Date::format(const WideString& format, bool local) const throw(InvalidFormat, MemoryException) {
#if defined(_DK_SDU_MIP__BASE__HAVE_WCSFTIME)
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct tm time;
  if (local) {
    localtime_r(&static_cast<time_t>(date), &time);
  } else {
    gmtime_r(&static_cast<time_t>(date), &time); // MT-safe
  }
  size_t result = wcsftime(pointer_cast<wchar_t*>(buffer->getElements()), buffer->getSize()/sizeof(wchar_t), format.getElements(), &time);
  return WideString(pointer_cast<const wchar_t*>(buffer->getElements()), result);
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

FormatOutputStream& operator<<(FormatOutputStream& stream, const Date& date) throw(InvalidFormat, IOException) {
  stream.addDateField(date);
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
