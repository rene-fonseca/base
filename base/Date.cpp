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

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
#undef __STRICT_ANSI__ // need tzset
#endif

#include <base/Date.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/string/Locale.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  include <time.h>
#else // unix
#  include <wchar.h>
#  include <sys/time.h>
#  include <time.h>
#  include <unistd.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace internal {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  inline int64 nativeToDate(const FILETIME& time) noexcept
  {
    return (*(uint64*)(&time) - 116444736000000000LL)/10;
  }
  
  inline FILETIME dateToNative(int64 time) noexcept
  {
    const int64 temp = time * 10LL + 116444736000000000LL;
    return *reinterpret_cast<const FILETIME*>(&temp);
  }
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
  extern "C" long _timezone;

  inline int64 getTimezone() noexcept
  {
    return _timezone * 1000000LL;
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__OPENBSD)
  inline int64 getTimezone() noexcept
  {
    time_t now = time(NULL);
    struct tm result = {0};
    localtime_r(&now, &result);
    return result.tm_gmtoff * 1000000LL;
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  inline int64 getTimezone() noexcept
  {
    BASSERT(!"Not supported.");
    return 0 * 1000000LL;
  }
#else
  inline int64 getTimezone() noexcept
  {
    return timezone * 1000000LL;
  }
#endif
  
  inline int64 nativeToDate(time_t time) noexcept
  {
    return time * 1000000LL;
  }
  
  inline time_t dateToNative(int64 time) noexcept
  {
    return time/1000000;
  }
#endif // flavor
} // end of namespace internal

const int Date::DAYS_PER_MONTH_NONLEAP_YEAR[Date::MONTHS_PER_YEAR] = {
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

const int Date::DAYS_PER_MONTH_LEAP_YEAR[Date::MONTHS_PER_YEAR] = {
  31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

const int
  Date::DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[Date::MONTHS_PER_YEAR + 1] = {
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

const int
  Date::DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[Date::MONTHS_PER_YEAR + 1] = {
  0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
};

int Date::normalize(DateTime& dateTime, bool redundancy) noexcept
{
  int carrier = 0; // and borrow
  
  int month = dateTime.month;
  if (month >= MONTHS_PER_YEAR) {
    carrier += month/MONTHS_PER_YEAR;
    month %= MONTHS_PER_YEAR;
  } else if (month < 0) {
    if ((-month)%MONTHS_PER_YEAR == 0) {
      carrier += month/MONTHS_PER_YEAR;
      month = 0;
    } else {
      carrier += month/MONTHS_PER_YEAR - 1;
      month = MONTHS_PER_YEAR - ((-month)%MONTHS_PER_YEAR);
    }
  }
  BASSERT((month >= 0) && (month < MONTHS_PER_YEAR));
  
  int64 year = dateTime.year + carrier;

  carrier = dateTime.millisecond/1000;
  dateTime.millisecond %= 1000;
  if (dateTime.millisecond < 0) {
    --carrier; // borrow
    dateTime.millisecond += 1000;
  }

  int64 second = static_cast<int64>(dateTime.second) + carrier;
  carrier = static_cast<int>(second/SECONDS_PER_MINUTE);
  dateTime.second = second%SECONDS_PER_MINUTE;
  if (second < 0) {
    --carrier; // borrow
    dateTime.second += SECONDS_PER_MINUTE;
  }
  
  int64 minute = static_cast<int64>(dateTime.minute) + carrier;
  carrier = static_cast<int>(minute/MINUTES_PER_HOUR);
  dateTime.minute = minute%MINUTES_PER_HOUR;
  if (minute < 0) {
    --carrier; // borrow
    dateTime.minute += MINUTES_PER_HOUR;
  }
  
  int64 hour = static_cast<int64>(dateTime.hour) + carrier;
  carrier = static_cast<int>(hour/HOURS_PER_DAY);
  dateTime.hour = hour%HOURS_PER_DAY;
  if (hour < 0) {
    --carrier; // borrow
    dateTime.hour += HOURS_PER_DAY;
  }

  int64 day = static_cast<int64>(dateTime.day) + carrier;
  
  int daysInMonth = isLeapYear(static_cast<int>(year)) ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month];
  if (day >= daysInMonth) { // traverse forward in time
    year += day/DAYS_PER_400_YEARS;
    day %= DAYS_PER_400_YEARS; // does not change leap year and month
    
    bool leapYear = isLeapYear(static_cast<int>(year));
    int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
    while (day >= daysInYear) { // maximum ~400 loops
      day -= daysInYear; // does not change month
      ++year;
      leapYear = isLeapYear(static_cast<int>(year));
      daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
    }
    
    int daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
    while (day >= daysInMonth) { // maximum 12 loops
      day -= daysInMonth;
      ++month;
      if (month == MONTHS_PER_YEAR) {
        ++year;
        leapYear = isLeapYear(static_cast<int>(year));
        month = 0;
      }
      daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
    }
  } else if (day < 0) { // traverse backward in time
    year += day/DAYS_PER_400_YEARS;
    day %= DAYS_PER_400_YEARS; // does not change leap year and month
    
    bool leapYear = isLeapYear(static_cast<int>(year));
    int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
    while (day <= daysInYear) { // maximum ~400 loops
      --year;
      leapYear = isLeapYear(static_cast<int>(year));
      daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
      day += daysInYear; // does not change month
    }
    
    int daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
    while (day < 0) { // maximum 12 loops
      --month;
      if (month < 0) {
        --year;
        leapYear = isLeapYear(static_cast<int>(year));
        month = MONTHS_PER_YEAR - 1;
      }
      daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
      day += daysInMonth;
    }
  }
  
  carrier = static_cast<int>(year/10000);
  
  dateTime.day = static_cast<int>(day);
  dateTime.month = month;
  dateTime.year = year%10000;
  
  if (redundancy) {
    if (isLeapYear(dateTime.year)) {
      dateTime.dayOfYear = static_cast<int>(day) + DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[month];
    } else {
      dateTime.dayOfYear = static_cast<int>(day) + DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[month];
    }
    
    int a = (month <= 1) ? 1 : 0; // (14 - month)/MONTHS_PER_YEAR
    year -= a;
    // Gregorian calendar
    dateTime.weekday = (day + year + year/4 - year/100 + year/400 + 31 * (month + 1 + MONTHS_PER_YEAR * a - 2)/MONTHS_PER_YEAR)%7;

    int y = static_cast<int>(year) + 4800; // 'a' is subtracted above
    int m = month + 1 + MONTHS_PER_YEAR * a - 3;
    // Gregorian calendar
    int julianDay =
      static_cast<int>(day + (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045);

    int d4 = (julianDay + 31741 - (julianDay % 7)) % 146097 % 36524 % 1461;
    int L  = d4/1460;
    int d1 = ((d4 - L) % 365) + L;
    dateTime.week = d1/7 + 1;
  }
  
  return carrier; // unlikely to be non-zero
}

int Date::getDaysOfMonth(int month, int year)
{
  bassert(
    (month >= 0) && (month < MONTHS_PER_YEAR),
    OutOfDomain(Type::getType<Date>())
  );
  return isLeapYear(year) ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month];
}

int Date::getDayOfWeek(int day, int month, int year) noexcept
{
  DateTime dt;
  dt.year = year;
  dt.month = month;
  dt.day = day;
  dt.hour = 0;
  dt.minute = 0;
  dt.second = 0;
  dt.millisecond = 0;
  normalize(dt, false);
  
  int a = (14 - (dt.month + 1))/MONTHS_PER_YEAR;
  int y = dt.year - a;
  int m = (dt.month + 1) + MONTHS_PER_YEAR * a - 2;
  // Julian calendar: d = (5 + day + y + y/4 + (31*m)/12) mod 7
  // Gregorian calendar: d = (day + y + y/4 - y/100 + y/400 + (31*m)/12) mod 7
  return ((dt.day + 1) + y + y/4 - y/100 + y/400 +
    (31 * m)/MONTHS_PER_YEAR)%DAYS_PER_WEEK;
  // TAG: 0 for Sunday but should be 6 according to ISO...
}

int Date::getWeek(int day, int month, int year) noexcept
{
  // week number as defined in ISO-8601
  DateTime dt;
  dt.year = year;
  dt.month = month;
  dt.day = day;
  dt.hour = 0;
  dt.minute = 0;
  dt.second = 0;
  dt.millisecond = 0;
  normalize(dt, false);
  
  int a = (14 - (dt.month + 1))/MONTHS_PER_YEAR;
  int y = dt.year + 4800 - a;
  int m = (dt.month + 1) + MONTHS_PER_YEAR * a - 3;
  // Gregorian calendar
  int julianDay =
    (dt.day + 1) + (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045;
  
  int d4 =
    (julianDay + 31741 - (julianDay % DAYS_PER_WEEK)) % 146097 % 36524 % 1461;
  int L  = d4/1460;
  int d1 = ((d4 - L) % 365) + L;
  return d1/DAYS_PER_WEEK + 1;
}

int Date::getDayOfYear(int day, int month, int year) noexcept
{
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

Date Date::getNow()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  FILETIME nativeTime;
  ::GetSystemTimeAsFileTime(&nativeTime);
  return internal::nativeToDate(nativeTime);
#else // unix
  return Date(::time(0) * 1000000LL);
#endif
}

int64 Date::getBias() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  TIME_ZONE_INFORMATION information;
  DWORD status = ::GetTimeZoneInformation(&information);
  BASSERT(status != TIME_ZONE_ID_INVALID);
  return information.Bias * 60000000LL; // TAG: what about the other bias'
#else // unix
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  throw NotSupported();
  return 0;
#else
  tzset(); // update variables
  return internal::getTimezone();
#endif
#endif // flavor
}

Date Date::getTime(int second, int minute, int hour, bool local)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: normalize input
  FILETIME nativeTime;
  SYSTEMTIME time = {0, 0, 0, 0, static_cast<WORD>(hour), static_cast<WORD>(minute), static_cast<WORD>(second), 0};
#if (_COM_AZURE_DEV__BASE__OS >= _COM_AZURE_DEV__BASE__WXP)
  if (local) {
    bassert(
      ::TzSpecificLocalTimeToSystemTime(0, &time, &time) != 0,
      DateException(Type::getType<Date>())
    );
  }
#endif
  bassert(
    ::SystemTimeToFileTime(&time, &nativeTime),
    DateException(Type::getType<Date>())
  );
#if (_COM_AZURE_DEV__BASE__OS < _COM_AZURE_DEV__BASE__WXP)
  if (local) {
    bassert(
      ::LocalFileTimeToFileTime(&nativeTime, &nativeTime) != 0,
      DateException(Type::getType<Date>())
    );
  }
#endif
  return internal::nativeToDate(nativeTime);
#else // unix
  struct tm temp = {second, minute, hour, 0, 0, 0, 0, 0, 0};
  time_t time = mktime(&temp);
  if (time == ((time_t)-1)) {
    throw DateException("Unable to represent date.", Type::getType<Date>());
  }
  int64 result = internal::nativeToDate(time);
  if (!local) {
    result -= internal::getTimezone(); // convert to UTC
  }
  return Date(result);
#endif
}

Date Date::getDate(
  int day, int month, int year, bool local)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: normalize
  FILETIME nativeTime;
  SYSTEMTIME time = {static_cast<WORD>(year), static_cast<WORD>(month), 0, static_cast<WORD>(day), 0, 0, 0, 0};
#if (_COM_AZURE_DEV__BASE__OS >= _COM_AZURE_DEV__BASE__WXP)
  if (local) {
    bassert(
      ::TzSpecificLocalTimeToSystemTime(0, &time, &time) != 0,
      DateException(Type::getType<Date>())
    );
  }
#endif
  bassert(
    ::SystemTimeToFileTime(&time, &nativeTime),
    DateException(Type::getType<Date>())
  );
#if (_COM_AZURE_DEV__BASE__OS < _COM_AZURE_DEV__BASE__WXP)
  if (local) {
    ::LocalFileTimeToFileTime(&nativeTime, &nativeTime);
  }
#endif
  return internal::nativeToDate(nativeTime);
#else // unix
  struct tm temp = {0, 0, 0, day, month, year - 1900, 0, 0, 0};
  time_t time = mktime(&temp);
  if (time == ((time_t)-1)) {
    throw DateException("Unable to represent date.", Type::getType<Date>());
  }
  int64 result = internal::nativeToDate(time);
  if (!local) {
    result -= internal::getTimezone(); // convert to UTC
  }
  return Date(result);
#endif
}

Date Date::getDate(
  int second,
  int minute,
  int hour,
  int day,
  int month,
  int year,
  bool local)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: normalize input
  FILETIME nativeTime;
  SYSTEMTIME time = {static_cast<WORD>(year), static_cast<WORD>(month), 0, static_cast<WORD>(day), static_cast<WORD>(hour), static_cast<WORD>(minute), static_cast<WORD>(second), 0};
#if (_COM_AZURE_DEV__BASE__OS >= _COM_AZURE_DEV__BASE__WXP)
  if (local) {
    bassert(
      ::TzSpecificLocalTimeToSystemTime(0, &time, &time) != 0,
      DateException(Type::getType<Date>())
    );
  }
#endif
  bassert(
    ::SystemTimeToFileTime(&time, &nativeTime),
    DateException(Type::getType<Date>())
  );
#if (_COM_AZURE_DEV__BASE__OS < _COM_AZURE_DEV__BASE__WXP)
  if (local) {
    bassert(
      ::LocalFileTimeToFileTime(&nativeTime, &nativeTime) != 0,
      DateException(Type::getType<Date>())
    );
  }
#endif
  return internal::nativeToDate(nativeTime);
#else // unix
  struct tm temp = {second, minute, hour, day, month, year - 1900, 0, 0, 0};
  time_t time = mktime(&temp);
  if (time == ((time_t)-1)) {
    throw DateException("Unable to represent date.", Type::getType<Date>());
  }
  int64 result = internal::nativeToDate(time);
  if (!local) {
    result -= internal::getTimezone(); // convert to UTC
  }
  return Date(result);
#endif
}

Date::Date(const DateTime& dateTime) noexcept
{
  DateTime dt = dateTime;
  normalize(dt, false);
  int64 days = dt.day +
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
    days += DAYS_PER_NONLEAP_YEAR * static_cast<int64>(dt.year - EPOCH_YEAR) +
      (DAYS_PER_LEAP_YEAR-DAYS_PER_NONLEAP_YEAR) * static_cast<int64>(leapYears);
  } else { // negative seconds contribution
    int leapYears = 0;
    int lastYear = dt.year; // include current year!
    if (lastYear <= 1900) {
      leapYears = ((1900 - lastYear)/4 + 1) + -((1900 - lastYear)/100 + 1) + ((1900 - lastYear)/400 + 1);
    } else if (lastYear <= 1968) {
      leapYears += (1968 - lastYear)/4 + 1;
    }
    days -= DAYS_PER_NONLEAP_YEAR * static_cast<int64>(EPOCH_YEAR - dt.year) +
      (DAYS_PER_LEAP_YEAR-DAYS_PER_NONLEAP_YEAR) * static_cast<int64>(leapYears);
  }
  
  const int seconds = dt.second + SECONDS_PER_MINUTE * dt.minute +
    SECONDS_PER_HOUR * dt.hour;
  date = dt.millisecond * 1000LL +
    (seconds + days * SECONDS_PER_DAY) * 1000000LL;
}

// TAG: add getUTCMillisecond

int Date::getMillisecond() const noexcept
{
  return (date/1000)%1000; // TAG: fixme UTC to local
}

int Date::getSecond() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return time.wSecond;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return result.tm_sec;
#endif
}

int Date::getMinute() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return time.wMinute;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return result.tm_min;
#endif
}

int Date::getHour() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return time.wHour;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return result.tm_hour;
#endif
}

int Date::getDay() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return time.wDay - 1;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return result.tm_mday - 1;
#endif
}

int Date::getDayOfWeek() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return time.wDayOfWeek;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return result.tm_wday;
#endif
}

int Date::getDayOfYear() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return 0; // TAG: fixme
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return result.tm_yday;
#endif
}

int Date::getMonth() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return time.wMonth - 1;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return result.tm_mon;
#endif
}

int Date::getYear() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return time.wYear;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  localtime_r(&nativeTime, &result);
  return 1900 + result.tm_year;
#endif
}

int Date::getUTCSecond() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
  return time.wSecond;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return result.tm_sec;
#endif
}

int Date::getUTCMinute() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
  return time.wMinute;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return result.tm_min;
#endif
}

int Date::getUTCHour() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
  return time.wHour;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return result.tm_hour;
#endif
}

int Date::getUTCDay() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
  return time.wDay - 1;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return result.tm_mday - 1;
#endif
}

int Date::getUTCDayOfWeek() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
  return time.wDayOfWeek;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return result.tm_wday;
#endif
}

int Date::getUTCDayOfYear() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    (::FileTimeToSystemTime(&nativeTime, &time) != 0) &&
    (::SystemTimeToTzSpecificLocalTime(0, &time, &time) != 0),
    DateException(this)
  );
  return 0; // TAG: fixme
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return result.tm_yday;
#endif
}

int Date::getUTCMonth() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
  return time.wMonth - 1;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return result.tm_mon;
#endif
}

int Date::getUTCYear() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
  return time.wYear;
#else // unix
  struct tm result;
  time_t nativeTime = internal::dateToNative(date);
  gmtime_r(&nativeTime, &result); // MT-safe
  return 1900 + result.tm_year;
#endif
}

// http://www.faqs.org/faqs/calendars/faq/part2

Date Date::getDateByJulianDay(int julianDay) noexcept
{
  // Gregorian calendar
  int a = julianDay + 32044;
  int b = (4 * a + 3)/146097;
  int c = a - (b * 146097)/4;

  // For the Julian calendar: b = 0 and c = julianDay + 32082
  
  int d = (4 * c + 3)/1461;
  int e = c - (1461 * d)/4;
  int m = (5 * e + 2)/153;
  int day = e - (153 * m + 2)/5 + 1 - 1;
  int month = m + 3 - MONTHS_PER_YEAR * m/10 - 1;
  int year = 100 * b + d - 4800 + m/10;
  return getDate(day, month, year);
}

// ISO-8601: monday first day of week

int Date::getJulianDay() const noexcept
{
  DateTime dt;
  split(dt);
  // 10 BC => -9
  int a = (14 - (dt.month + 1))/MONTHS_PER_YEAR;
  int y = dt.year + 4800 - a;
  int m = (dt.month + 1) + MONTHS_PER_YEAR * a - 3;
  // Gregorian calendar
  return (dt.day + 1) +
    (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045;
  // Julian calendar
  // return (dt.day + 1) + (153 * m + 2)/5 + 365 * y + y/4 - 32083;
}

void Date::split(DateTime& result, bool local) const noexcept
{
#if 0
  result.microsecond = date%1000000;
  int days = date/SECONDS_PER_DAY;
  int seconds = date%SECONDS_PER_DAY;
  if (seconds < 0) {
    seconds += SECONDS_PER_DAY;
    --days;
  }
  result.hour = seconds/SECONDS_PER_HOUR;
  seconds %= SECONDS_PER_HOUR;
  result.minute = seconds/SECONDS_PER_MINUTE;
  result.second = seconds%SECONDS_PER_MINUTE;
  
  int weekday = (EPOCH_WEEKDAY + days)%DAYS_PER_WEEK;
  if (weekday < 0) {
    weekday += DAYS_PER_WEEK;
  }
  result.weekday = weekday;

  {
    int a = (month <= 1) ? 1 : 0; // (14 - month)/MONTHS_PER_YEAR
    int y = year + 4800 - a;
    int m = month + 1 + MONTHS_PER_YEAR * a - 3;
    // Gregorian calendar
    int julianDay =
      day + (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045;
    
    int d4 = (julianDay + 31741 - (julianDay % 7)) % 146097 % 36524 % 1461;
    int L  = d4/1460;
    int d1 = ((d4 - L) % 365) + L;
    result.week = d1/7 + 1;
  }
  
  if (days >= 0) {
    int year = EPOCH_YEAR;
    while (true) { // TAG: need otimization
      bool leapYear = isLeapYear(year);
      int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
      if (days < daysInYear) {
        break;
      }
      ++year;
      days -= daysInYear;
    }
  } else {
    do { // TAG: need otimization
      --year;
      bool leapYear = isLeapYear(year);
      int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
      days += daysInYear;
    } while (days < 0);
  }
  result.year = year;
  result.dayOfYear = days;
#endif
    // if local then add timezone bias
  
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  // TAG: convert to local time when required
  bassert(
    ::FileTimeToSystemTime(&nativeTime, &time),
    DateException(this)
  );
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
  time_t nativeTime = internal::dateToNative(date);
  if (local) {
    localtime_r(&nativeTime, &time);
  } else {
    gmtime_r(&nativeTime, &time); // MT-safe
  }
  result.year = 1900 + time.tm_year;
  result.month = time.tm_mon;
  result.day = time.tm_mday - 1;
  result.weekday = time.tm_wday;
  result.hour = time.tm_hour;
  result.minute = time.tm_min;
  result.second = time.tm_sec;
  result.millisecond = 0;
#endif
}

String Date::format(
  const String& format,
  bool local) const {
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
      bassert(i != end, InvalidFormat(this));
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
          bassert(i != end, InvalidFormat(this));
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
          BASSERT((bias >= -(12*60 + 59)) && (bias <= (12*60 + 59))); // 
          if (bias > 0) {
            stream << '+' << setWidth(2) << ZEROPAD << bias/60
                   << setWidth(2) << ZEROPAD << bias%60;
          } else {
            stream << '-' << setWidth(2) << ZEROPAD << -bias/60
                   << setWidth(2) << ZEROPAD << -bias%60;
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

WideString Date::format(
  const WideString& format,
  bool local) const
{
#if defined(_COM_AZURE_DEV__BASE__HAVE_WCSFTIME)
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  time_t nativeTime = (date + 500)/1000; // internal::dateToNative(date);
  struct tm time;
  if (local) {
    localtime_s(&time, &nativeTime);
  } else {
    gmtime_s(&time, &nativeTime); // MT-safe
  }
  size_t result = wcsftime(
    Cast::pointer<wchar*>(buffer.getElements()),
    buffer.getSize()/sizeof(wchar),
    nullptr, // TAG: FIXME? format.getElements(),
    &time
  );
  return WideString(
    Cast::pointer<const wchar*>(buffer.getElements()),
    result
  );
#else // unix
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  time_t nativeTime = internal::dateToNative(date);
  struct tm time;
  if (local) {
    localtime_r(&nativeTime, &time);
  } else {
    gmtime_r(&nativeTime, &time); // MT-safe
  }
  size_t result = wcsftime(
    Cast::pointer<wchar*>(buffer.getElements()),
    buffer.getSize()/sizeof(wchar),
    nullptr, // TAG: FIXME format.getElements(),
    &time
  );
  return WideString(
    Cast::pointer<const wchar*>(buffer.getElements()),
    result
  );
#endif
#else
#  warning WideString Date::format(const WideString& format, bool local) const not available
  return WideString();
#endif
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const Date& date)
{
  stream.addDateField(date);
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
