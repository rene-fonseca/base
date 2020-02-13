/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

// TAG: add ISO format method

#include <base/platforms/features.h>

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
#undef __STRICT_ANSI__ // need tzset
#endif

#include <base/Date.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/string/Locale.h>
#include <base/string/Parser.h>
#include <base/UnitTest.h>
#include <base/build.h>

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

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

namespace internal {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  inline int64 nativeToDate(const FILETIME& time) noexcept
  {
    int64 temp = (*(int64*)(&time) - 116444736000000000LL)/10;
    return temp;
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
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
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

const int Date::DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[Date::MONTHS_PER_YEAR + 1] = {
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

const int Date::DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[Date::MONTHS_PER_YEAR + 1] = {
  0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
};

int Date::normalize(DateTime& dateTime) noexcept
{
  int carrier = 0; // and borrow
  carrier = dateTime.millisecond/1000;
  dateTime.millisecond %= 1000;
  if (dateTime.millisecond < 0) {
    --carrier; // borrow
    dateTime.millisecond += 1000;
  }

  int64 second = static_cast<int64>(dateTime.second) + carrier;
  carrier = static_cast<int>(second/SECONDS_PER_MINUTE);
  dateTime.second = second % SECONDS_PER_MINUTE;
  if (second < 0) {
    --carrier; // borrow
    dateTime.second += SECONDS_PER_MINUTE;
  }
  
  int64 minute = static_cast<int64>(dateTime.minute) + carrier;
  carrier = static_cast<int>(minute/MINUTES_PER_HOUR);
  dateTime.minute = minute % MINUTES_PER_HOUR;
  if (minute < 0) {
    --carrier; // borrow
    dateTime.minute += MINUTES_PER_HOUR;
  }
  
  int64 hour = static_cast<int64>(dateTime.hour) + carrier;
  carrier = static_cast<int>(hour/HOURS_PER_DAY);
  dateTime.hour = hour % HOURS_PER_DAY;
  if (hour < 0) {
    --carrier; // borrow
    dateTime.hour += HOURS_PER_DAY;
  }

#if 0
  // count days from first valid date and then add time delta
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
    
    int64 year = static_cast<int64>(dateTime.year) + carrier;
  
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
    
    // int daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
    while (day < 0) { // maximum 12 loops
      --month;
      if (month < 0) {
        --year;
        // leapYear = isLeapYear(static_cast<int>(year));
        month = MONTHS_PER_YEAR - 1;
      }
      // daysInMonth = leapYear ? DAYS_PER_MONTH_LEAP_YEAR[month] : DAYS_PER_MONTH_NONLEAP_YEAR[month]; // month is valid
      day += daysInMonth;
    }
  }
  
  carrier = static_cast<int>(year/10000);
#endif
  
  // dateTime.day = static_cast<int>(day + 1);
  // dateTime.month = month;
  // dateTime.year = dateTime.year % 10000;
  
  if (true) {
    if (isLeapYear(dateTime.year)) {
      dateTime.dayOfYear = static_cast<int>(dateTime.day - 1) + DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[dateTime.month];
    } else {
      dateTime.dayOfYear = static_cast<int>(dateTime.day - 1) + DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[dateTime.month];
    }
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

#if 0
int Date::getDayOfWeek(int day, int month, int year) noexcept
{
  int a = (14 - (dt.month + 1))/MONTHS_PER_YEAR;
  int y = dt.year - a;
  int m = (dt.month + 1) + MONTHS_PER_YEAR * a - 2;
  // Julian calendar: d = (5 + day + y + y/4 + (31*m)/12) mod 7
  // Gregorian calendar: d = (day + y + y/4 - y/100 + y/400 + (31*m)/12) mod 7
  return (dt.day + y + y/4 - y/100 + y/400 +
    (31 * m)/MONTHS_PER_YEAR)%DAYS_PER_WEEK;
  // TAG: 0 for Sunday but should be 6 according to ISO...
}
#endif

int Date::getWeek(const DateTime& dt) noexcept
{
  int a = (14 - (dt.month + 1))/MONTHS_PER_YEAR;
  int y = dt.year + 4800 - a;
  int m = (dt.month + 1) + MONTHS_PER_YEAR * a - 3;
  // Gregorian calendar
  int julianDay =
    dt.day + (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045;
  
  int d4 =
    (julianDay + 31741 - (julianDay % DAYS_PER_WEEK)) % 146097 % 36524 % 1461;
  int L  = d4/1460;
  int d1 = ((d4 - L) % 365) + L;
  return d1/DAYS_PER_WEEK + 1;
}

Date Date::getNow()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  FILETIME nativeTime;
  ::GetSystemTimeAsFileTime(&nativeTime);
  return internal::nativeToDate(nativeTime);
#else // unix

#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR)
    // _POSIX_C_SOURCE >= 199309L
    struct timespec time;
    int status = clock_gettime(CLOCK_REALTIME, &time);
    if (!INLINE_ASSERT(!status)) {
      return Date(::time(nullptr) * 1000000LL);
    }
    return static_cast<int64>(1000000) * time.tv_sec + (time.tv_nsec + 500)/1000;
#else
    struct timeval temp;
    if (!INLINE_ASSERT(gettimeofday(&temp, 0) == 0)) {
      return Date(::time(nullptr) * 1000000LL);
    }
    return static_cast<uint64>(1000000) * temp.tv_sec + temp.tv_usec;
#endif
  
#endif
}

int64 Date::getBias()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

//  #if (_COM_AZURE_DEV__BASE__OS < _COM_AZURE_DEV__BASE__WXP)
  // UTC = local time + bias
    FILETIME localTime;
    FILETIME utcTime;
    clear(localTime);
    if (::LocalFileTimeToFileTime(&localTime, &utcTime) == 0) {
      _throw DateException(Type::getType<Date>());
    }
    return *reinterpret_cast<const int64*>(&utcTime)/10; // 100 ns to ms
//  #endif

#if 0
  TIME_ZONE_INFORMATION information;
  DWORD status = ::GetTimeZoneInformation(&information);
  BASSERT(status != TIME_ZONE_ID_INVALID);
  return information.Bias * 60000000LL;
#endif

#else // unix
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  // _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
  return 0;
#else
  tzset(); // update variables
  return internal::getTimezone();
#endif
#endif // flavor
}

Date Date::getTime(int second, int minute, int hour)
{
  DateTime dt;
  dt.second = second;
  dt.minute = minute;
  dt.hour = hour;
  dt.day = 1;
  dt.month = 0;
  dt.year = 1970;
  return makeDate(dt);
}

Date Date::getDate(int day, int month, int year)
{
  DateTime dt;
  dt.day = day;
  dt.month = month;
  dt.year = year;
  return makeDate(dt);
}

Date Date::getDate(
  int second, int minute, int hour,
  int day, int month, int year)
{
  DateTime dt;
  dt.second = second;
  dt.minute = minute;
  dt.hour = hour;
  dt.day = day;
  dt.month = month;
  dt.year = year;
  return makeDate(dt);
}

bool Date::isValidDateTime(const DateTime& dt)
{
  if (!((dt.year >= -9999) && (dt.year <= 9999))) {
    return false;
  }
  if (!((dt.month >= 0) && (dt.month <= 11))) {
    return false;
  }
  if (!((dt.day >= 1) &&
       (dt.day <= (isLeapYear(dt.year) ? DAYS_PER_MONTH_LEAP_YEAR[dt.month] : DAYS_PER_MONTH_NONLEAP_YEAR[dt.month])))) {
    return false;
  }
  if (!((dt.hour >= 0) && (dt.hour <= 23))) {
    return false;
  }
  if (!((dt.minute >= 0) && (dt.minute <= 59))) {
    return false;
  }
  if (!((dt.second >= 0) && (dt.second <= 60))) { // leap second
    return false;
  }
  if (!((dt.millisecond >= 0) && (dt.millisecond <= 999))) {
    return false;
  }
  return true;
}

Date Date::makeDate(const DateTime& dt, bool local)
{
  BASSERT(dt.millisecond >= 0);
  BASSERT(dt.second >= 0);
  BASSERT(dt.minute >= 0);
  BASSERT(dt.hour >= 0);

  BASSERT((dt.day >= 1) && (dt.day <= 31));
  BASSERT((dt.month >= 0) && (dt.month <= 11));

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  FILETIME nativeTime;
  SYSTEMTIME time = {static_cast<WORD>(dt.year), static_cast<WORD>(dt.month + 1), 0, static_cast<WORD>(dt.day),
    static_cast<WORD>(dt.hour), static_cast<WORD>(dt.minute), static_cast<WORD>(dt.second),
    static_cast<WORD>(dt.millisecond)};
  bassert(
    ::SystemTimeToFileTime(&time, &nativeTime),
    DateException(Type::getType<Date>())
  );
  return internal::nativeToDate(nativeTime);
#else // unix
  struct tm temp = {dt.second, dt.minute, dt.hour, dt.day, dt.month, dt.year - 1900, 0, 0, 0};
  time_t time = (time_t)-1;
  if (local) { // use timezone
    time = mktime(&temp);
  } else { // no timezone adjustment
    time = timegm(&temp);
  }
  if (time == ((time_t)-1)) {
    _throw DateException("Unable to represent date.", Type::getType<Date>());
  }
  int64 result = internal::nativeToDate(time);
  return Date(result);
#endif



#if 0
  // TAG: normalize(dt, false);
  int64 days = static_cast<int64>(dt.day) +
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
#endif
}

int Date::getMillisecond() const noexcept
{
  return date/1000 % 1000; // can be negative
}

int Date::getSecond() const noexcept
{
  return split().second;
}

int Date::getMinute() const noexcept
{
  return split().minute;
}

int Date::getHour() const noexcept
{
  return split().hour;
}

int Date::getDay() const noexcept
{
  return split().day;
}

int Date::getDayOfWeek() const noexcept
{
  return split().weekday;
}

int Date::getDayOfYear() const noexcept
{
  return split().dayOfYear;
}

int Date::getMonth() const noexcept
{
  return split().month;
}

int Date::getYear() const noexcept
{
  return split().year;
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
  DateTime dt = split();
  // 10 BC => -9
  int a = (14 - (dt.month + 1))/MONTHS_PER_YEAR;
  int y = dt.year + 4800 - a;
  int m = (dt.month + 1) + MONTHS_PER_YEAR * a - 3;
  // Gregorian calendar
  return dt.day +
    (153 * m + 2)/5 + 365 * y + y/4 - y/100 + y/400 - 32045;
  // Julian calendar
  // return dt.day + (153 * m + 2)/5 + 365 * y + y/4 - 32083;
}

Date::DateTime Date::split(bool local) const noexcept
{
#if 0
  result.millisecond = date/1000 % 1000;
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
  
  int weekday = (EPOCH_WEEKDAY + days) % DAYS_PER_WEEK;
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
    while (true) { // TAG: need optimization
      bool leapYear = isLeapYear(year);
      int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
      if (days < daysInYear) {
        break;
      }
      ++year;
      days -= daysInYear;
    }
  } else {
    do { // TAG: need optimization
      --year;
      bool leapYear = isLeapYear(year);
      int daysInYear = leapYear ? DAYS_PER_LEAP_YEAR : DAYS_PER_NONLEAP_YEAR;
      days += daysInYear;
    } while (days < 0);
  }
  result.year = year;
  result.dayOfYear = days;
#endif

  DateTime result;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEMTIME time;
  FILETIME nativeTime = internal::dateToNative(date);
  if (local) {
    FILETIME utcTime;
    if (::LocalFileTimeToFileTime(&nativeTime, &utcTime) != 0) {
      nativeTime = utcTime;
    } else {
      // _throw DateException(Type::getType<Date>());
    }
  }
  binternalerror(
    ::FileTimeToSystemTime(&nativeTime, &time)
  );
  result.year = time.wYear;
  result.month = time.wMonth - 1; // 0-indexed
  result.day = time.wDay; // 1-indexed
  result.weekday = time.wDayOfWeek;
  result.hour = time.wHour;
  result.minute = time.wMinute;
  result.second = time.wSecond;
  result.millisecond = time.wMilliseconds;
  
  if (true) {
    const bool leapYear = isLeapYear(result.year);
    int dayOfYear = result.day - 1 + // 1-indexed
      (leapYear ?
       DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[result.month] :
       DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[result.month]);
    result.dayOfYear = dayOfYear;
  }
#else // unix
  struct tm time;
  time_t nativeTime = internal::dateToNative(date);
  if (local) { // timezone adjustment
    localtime_r(&nativeTime, &time); // MT-safe
  } else { // no timezone adjustment
    gmtime_r(&nativeTime, &time); // MT-safe
  }
  result.year = 1900 + time.tm_year;
  result.month = time.tm_mon;
  result.day = time.tm_mday;
  result.weekday = time.tm_wday;
  result.dayOfYear = time.tm_yday;
  result.hour = time.tm_hour;
  result.minute = time.tm_min;
  result.second = time.tm_sec;
  result.millisecond = date/1000 % 1000;
#endif

  BASSERT(result.millisecond >= 0);
  BASSERT(result.second >= 0);
  BASSERT(result.minute >= 0);
  BASSERT(result.hour >= 0);

  BASSERT((result.day >= 1) && (result.day <= 31));
  BASSERT((result.month >= 0) && (result.month <= 11));
  BASSERT((result.dayOfYear >= 0) && (result.dayOfYear <= 365));
  return result;
}

String Date::getISO8601(int offset) const
{
  // https://tools.ietf.org/html/rfc3339

  if (offset == 0) {
  } else {
    int HH = ((offset > 0) ? offset : -offset)/60;
    if (HH > 24) {
      _throw DateException("Invalid timezone offset.");
    }
  }

  Date temp(date + offset * 60 * 1000000ULL);
  auto dt = temp.split();

  StringOutputStream sos;
  sos << setWidth(4) << ZEROPAD << dt.year << '-'
      << setWidth(2) << ZEROPAD << (dt.month + 1) << '-'
      << setWidth(2) << ZEROPAD << dt.day
      << 'T' << setWidth(2) << ZEROPAD << dt.hour << ':'
      << setWidth(2) << ZEROPAD << dt.minute << ':'
      << setWidth(2) << ZEROPAD << dt.second;

  if (offset == 0) {
    sos << 'Z'; // UTC
  } else {
    int sign = 1;
    if (offset < 0) {
      sign = -1;
      offset = -offset;
    }
    sos << ((sign > 0) ? '+' : '-')
        << setWidth(2) << ZEROPAD << (offset / 60) << ':'
        << setWidth(2) << ZEROPAD << (offset % 60);
  }
  return sos;
}

String Date::getISO8601Compact(int offset) const
{
  if (offset == 0) {
  } else {
    int HH = ((offset > 0) ? offset : -offset)/60;
    if (HH > 24) {
      _throw DateException("Invalid timezone offset.");
    }
  }

  Date temp(date + offset * 60 * 1000000ULL);
  auto dt = temp.split();

  StringOutputStream sos;
  sos << setWidth(4) << ZEROPAD << dt.year
      << setWidth(2) << ZEROPAD << (dt.month + 1)
      << setWidth(2) << ZEROPAD << dt.day
      << 'T' << setWidth(2) << ZEROPAD << dt.hour
      << setWidth(2) << ZEROPAD << dt.minute
      << setWidth(2) << ZEROPAD << dt.second;

  if (offset == 0) {
    sos << 'Z'; // UTC
  } else {
    int sign = 1;
    if (offset < 0) {
      sign = -1;
      offset = -offset;
    }
    sos << ((sign > 0) ? '+' : '-')
        << setWidth(2) << ZEROPAD << (offset / 60)
        << setWidth(2) << ZEROPAD << (offset % 60);
  }
  return sos;
}

String Date::getISO8601_MS() const
{
  // 2020-02-12T18:15:23.678Z
  auto dt = split();
  StringOutputStream sos;
  sos << setWidth(4) << ZEROPAD << dt.year << '-'
      << setWidth(2) << ZEROPAD << (dt.month + 1) << '-'
      << setWidth(2) << ZEROPAD << dt.day
      << 'T' << setWidth(2) << ZEROPAD << dt.hour << ':'
      << setWidth(2) << ZEROPAD << dt.minute << ':'
      << setWidth(2) << ZEROPAD << dt.second
      << '.' << setWidth(3) << ZEROPAD << (date/1000 % 1000) // ms
      << 'Z'; // UTC
  return sos;
}

String Date::getISO8601_US() const
{
  // 2020-02-12T18:15:23.678001Z
  auto dt = split();
  StringOutputStream sos;
  sos << setWidth(4) << ZEROPAD << dt.year << '-'
      << setWidth(2) << ZEROPAD << (dt.month + 1) << '-'
      << setWidth(2) << ZEROPAD << dt.day
      << 'T' << setWidth(2) << ZEROPAD << dt.hour << ':'
      << setWidth(2) << ZEROPAD << dt.minute << ':'
      << setWidth(2) << ZEROPAD << dt.second
      << '.' << setWidth(6) << ZEROPAD << (date % 1000000) // us
      << 'Z'; // UTC
  return sos;
}

Date Date::parseISO8601(const String& text, bool subsecond)
{
  DateTime dt;
  
  Parser parser(text);
  dt.year = parser.readDigits(4); // year
  bool compact = (parser.peek() != '-');
  if (!compact) {
    parser.read('-');
  }
  dt.month = parser.readDigits(2) - 1; // month
  if (!compact) {
    parser.read('-');
  }
  dt.day = parser.readDigits(2); // day
  parser.read('T');
  dt.hour = parser.readDigits(2); // hours
  if (!compact) {
    parser.read(':');
  }
  dt.minute = parser.readDigits(2); // minutes
  if (!compact) {
    parser.read(':');
  }
  dt.second = parser.readDigits(2); // seconds
  
  unsigned int us = 0;
  if (!compact && subsecond) {
    if (parser.hasMore() && (parser.peek() == '.')) {
      parser.read('.');
      unsigned int count = parser.getNumberOfDigits();
      if (count > 9) {
        _throw InvalidFormat("Invalid date/time format.");
      }
      unsigned int value = parser.readDigits(count);
      while (count++ < 9) {
        value *= 10;
      }
      value /= 1000;
      us = value;
    }
  }
  
  int64 offset = 0;
  if (parser.peek() == 'Z') {
    parser.read('Z');
  } else {
    int sign = 1;
    if (parser.peek() == '+') {
      parser.read('+');
    } else {
      parser.read('-');
      sign = -1;
    }
    int HH = parser.readDigits(2); // hours
    int MM = 0;
    if (parser.hasMore()) {
      if (!compact) {
        parser.read(':');
      }
      MM = parser.readDigits(2); // minutes
    }
    offset = sign * ((HH * 60 + MM) * 60) * 1000000ULL;
  }
  
  if (parser.hasMore()) {
    _throw InvalidFormat("Invalid date/time format.");
  }
  if (!isValidDateTime(dt)) {
    _throw InvalidFormat("Invalid date/time.");
  }
  
  int64 result = makeDate(dt).getValue();
  result -= offset;
  if (result > 0) {
    result += us;
  } else {
    result -= us;
  }
  return result;
}

String Date::format(const String& format) const
{
  const DateTime dateTime = split();
  
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
        stream << dateTime.day;
        break;
      case 'D':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.month << '/';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.day << '/';
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.year % 100;
        break;
      case 'e':
        stream << dateTime.day;
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
        stream << dateTime.day;
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
        stream << dateTime.weekday;
        break;
      case 'U': // First Sunday is the first day of week 1
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << Date::getWeek(dateTime);
        // week of year with Sunday as first day of week 00-53
        break;
      case 'V': // First Monday is the first day of week 1 - read standard!
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << Date::getWeek(dateTime); // 01-53
        break;
      case 'w': // 0 is Sunday
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << dateTime.weekday;
        break;
      case 'W':
        stream << setWidth(2);
        if (flags & DEFAULT) {stream << ZEROPAD;}
        stream << Date::getWeek(dateTime);
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
        _throw InvalidFormat(this);
      }
    } else {
      stream << *i;
    }
    ++i; // skip code or normal character
  }
  stream << FLUSH;
  return stream.getString();
}

WideString Date::format(const WideString& text) const
{
  return format(String(text));
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Date& date)
{
  stream.addDateField(date);
  return stream;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Date) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base");

  void run() override
  {
    Date now = Date::getNow();

    // fout << "NOW=" << now << ENDL;
    TEST_ASSERT(now);

    Date d1 = Date::getTime(0, 0, 0);
    TEST_ASSERT(!d1);
    // fout << "EPOCH=" << d1 << ENDL;
    Date d2 = Date::getDate(1, 0, 2020);
    TEST_ASSERT(d2);
    Date d3 = Date::getDate(1, 5, 2, 1, 0, 2020); // TAG: normalize values
    TEST_ASSERT(d3);
    
    Date d4(d3.getValue());
    // fout << d4 << ENDL;
    
    Date::DateTime time = d3.split();
    TEST_ASSERT(time.second == 1);
    TEST_ASSERT(time.minute == 5);
    TEST_ASSERT(time.hour == 2);
    TEST_ASSERT(time.day == 1);
    TEST_ASSERT(time.month == 0);
    TEST_ASSERT(time.year == 2020);
    TEST_ASSERT(time.weekday == 3);
    // fout << time.weekday << ENDL;
    TEST_ASSERT(time.dayOfYear == 0);

    TEST_ASSERT(d3.getSecond() == 1);
    TEST_ASSERT(d3.getMinute() == 5);
    TEST_ASSERT(d3.getHour() == 2);
    TEST_ASSERT(d3.getDay() == 1);
    TEST_ASSERT(d3.getMonth() == 0);
    TEST_ASSERT(d3.getYear() == 2020);
    
    Date::DateTime dateTime;
    dateTime.year = 4000;
    dateTime.month = 11;
    dateTime.day = 31;
    dateTime.hour = 23;
    dateTime.minute = 1;
    dateTime.second = 1;
    dateTime.millisecond = 999;
    Date::normalize(dateTime);
    Date d5 = Date::makeDate(dateTime);
    TEST_ASSERT(d5);
    // fout << Date::makeDate(dateTime) << ENDL;

    static const Literal DAY_NAMES[] = {
      Literal("Sunday"),
      Literal("Monday"),
      Literal("Tuesday"),
      Literal("Wednesday"),
      Literal("Thursday"),
      Literal("Friday"),
      Literal("Saturday")
    };

    TEST_ASSERT(Date::getDate(2, Date::AUGUST, 1953).split().weekday == Date::SUNDAY);
    TEST_ASSERT(Date::getDate(13, Date::FEBRUARY, 2053).split().weekday == Date::THURSDAY);
    TEST_ASSERT(Date::getDate(20, Date::NOVEMBER, 2055).split().weekday == Date::SATURDAY);
    
    String text = now.getISO8601_US();
    // fout << text << ENDL;
    Date now2 = Date::parseISO8601(text, true);
    String text2 = now2.getISO8601_US();
    TEST_ASSERT(text2 == text); // do not compare date value due to potential subsecond rounding

    text = now.getISO8601Compact();
    // fout << text << ENDL;
    now2 = Date::parseISO8601(text, true);
    text2 = now2.getISO8601Compact();
    TEST_ASSERT(text2 == text); // do not compare date value due to potential subsecond rounding

    text = now.getISO8601(60);
    // fout << text << ENDL;
    now2 = Date::parseISO8601(text, true);
    text2 = now2.getISO8601();
    // fout << text2 << ENDL;
  }
};

TEST_REGISTER(Date);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
