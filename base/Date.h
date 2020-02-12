/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/DateException.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>
#include <base/collection/Hash.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class FormatOutputStream;

/**
  Representation of date/time in Coordinated Universal Time (UTC). It is recommended that you keep the
  date/time in UTC and only convert to local time when needed.
 
  @short Date and time.
  @version 1.3
*/

class _COM_AZURE_DEV__BASE__API Date {
private:

  /**
    Microseconds elapsed since epoch (i.e. 00:00:00 on January 1, 1970,
    Coordinated Universal Time (UTC)).
  */
  int64 date = 0;
public:

  enum Constants {
    /** Index of Sunday. */
    SUNDAY = 0,
    /** Index of Monday. */
    MONDAY = 1,
    /** Index of Tuesday. */
    TUESDAY = 2,
    /** Index of Wednesday. */
    WEDNESDAY = 3,
    /** Index of Thursday. */
    THURSDAY = 4,
    /** Index of Friday. */
    FRIDAY = 5,
    /** Index of Saturday. */
    SATURDAY = 6,
    
    /** Normalized index for January. */
    JANUARY = 0,
    /** Normalized index for February. */
    FEBRUARY = 1,
    /** Normalized index for March. */
    MARCH = 2,
    /** Normalized index for April. */
    APRIL = 3,
    /** Normalized index for May. */
    MAY = 4,
    /** Normalized index for June. */
    JUNE = 5,
    /** Normalized index for July. */
    JULY = 6,
    /** Normalized index for August. */
    AUGUST = 7,
    /** Normalized index for September. */
    SEPTEMBER = 8,
    /** Normalized index for October. */
    OCTOBER = 9,
    /** Normalized index for November. */
    NOVEMBER = 10,
    /** Normalized index for December. */
    DECEMBER = 11,
    
    /** The year of the epoch. */
    EPOCH_YEAR = 1970,
    /** The weekday of the first epoch day. */
    EPOCH_WEEKDAY = THURSDAY,
    /** The number of days per non-leap year. */
    DAYS_PER_NONLEAP_YEAR = 365,
    /** The number of days per leap year. */
    DAYS_PER_LEAP_YEAR = DAYS_PER_NONLEAP_YEAR + 1,
    /** The number of days per 400 years. */
    DAYS_PER_400_YEARS = (300+4-1)*365 + (100-4+1)*366,
    /** The number of days per week. */
    DAYS_PER_WEEK = 7,
    /** The number of months per year. */
    MONTHS_PER_YEAR = 12,
    /** The number of hours per day. */
    HOURS_PER_DAY = 24,
    /** The number of minutes per hour. */
    MINUTES_PER_HOUR = 60,
    /** The number of minutes per day. */
    MINUTES_PER_DAY = MINUTES_PER_HOUR * HOURS_PER_DAY,
    /** The number of seconds per minute. */
    SECONDS_PER_MINUTE = 60,
    /** The number of seconds per hour. */
    SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR,
    /** The number of seconds per day. */
    SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY
  };
  
  /** The days per month for non-leap years. */
  static const int DAYS_PER_MONTH_NONLEAP_YEAR[MONTHS_PER_YEAR];
  /** The days per month for leap years. */
  static const int DAYS_PER_MONTH_LEAP_YEAR[MONTHS_PER_YEAR];
  /** The days before the first of the month for non-leap years. */
  static const int DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[MONTHS_PER_YEAR + 1];
  /** The days before the first of the month for leap years. */
  static const int DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[MONTHS_PER_YEAR + 1];
  
  /** Structure used to split date into its common components. */
  class DateTime {
  public:
    
    int year = 0; /**< The year. */
    int month = 0; /**< Operating range is [0;11]. */
    int day = 0; /**< Operating range is [1;31]. */
    int weekday = 0; /**< Operating range is [0;6]. Sunday is 0. */
    int dayOfYear = 0; /** Operating range is [0;365]. */
    int hour = 0; /**< Operating range is [0;23]. */
    int minute = 0; /**< Operating range is [0;59]. */
    int second = 0; /**< Operating range is [0;61]. */
    int millisecond = 0; /**< Operating range is [0-999]. */
  };
  
  /** Returns the week of the year. */
  static int getWeek(const DateTime& dt) noexcept;

  /**
    Normalizes the specified data/time structure such that the fields are
    brought into their normal operating range.

    @param dateTime The structure to normalize.
    
    @return The year carrier. Only non-zero if the year falls outside the range
    [-9999;9999].
  */
  static int normalize(DateTime& dateTime) noexcept;
  
  /**
    Returns true if the year is a leap year.
  */
  static inline bool isLeapYear(int year) noexcept
  {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
  }
  
  /**
    Returns the number of days in the specified year.
  */
  static inline int getDaysOfYear(int year) noexcept
  {
    return isLeapYear(year) ? 366 : 365;
  }
  
  /**
    Returns the number of days in the specified month.
  */
  static int getDaysOfMonth(int month, int year);

  /**
    Returns the current time in UTC time.
  */
  static Date getNow();
  
  /**
    Returns the bias (in microseconds) for convertion from UTC to local time.
    Bias may change during the lifetime of the process. This method only works
    on some planet called Earth :-]. UTC = local time + bias.
  */
  static int64 getBias();
  
  /**
    Returns date object for the specified time. Invalid values will be
    normalized. Raises 'DateException' if the date cannot be represented.
    The used date is EPOCH.
    
    @param second The second.
    @param minute The minute.
    @param hour The hour.
  */
  static Date getTime(int second, int minute, int hour);
  
  /**
    Returns date object for the specified date. Invalid values will be
    normalized. Raises 'DateException' if the date cannot be represented.

    @param day The day of the month.
    @param month The month of the year.
    @param year The year.
  */
  static Date getDate(int day, int month, int year);

  /**
    Returns date object for the specified date and time. Invalid values will be
    normalized. Raises 'DateException' if the date cannot be represented.

    @param second The second.
    @param minute The minute.
    @param hour The hour.
    @param day The day of the month.
    @param month The month of the year.
    @param year The year.
  */
  static Date getDate(
    int second,
    int minute,
    int hour,
    int day,
    int month,
    int year);
  
  /**
    Returns the date corresponding to the specified Julian day.
  */
  static Date getDateByJulianDay(int day) noexcept;
  
  /**
    Initializes the date as 00:00:00 on January 1, 1970.
  */
  inline Date() noexcept
  {
  }
  
  /**
    Initializes the date with the specified date.

    @param date The number of microseconds elapsed since 00:00:00 on January 1,
    1970, Coordinated Universal Time (UTC).
  */
  inline Date(int64 _date) noexcept
    : date(_date)
  {
  }
  
  /** Returns true if the date/time is within the valid range. */
  static bool isValidDateTime(const DateTime& dt);

  /**
    Returns the date/time for the given date/time components.

    @param dateTime The broken out date and time.
    @param local Specifies that datetime is in local time. UTC by default.
  */
  static Date makeDate(const DateTime& dateTime, bool local = false);
  
  /**
    Initialize date from other date.
  */
  inline Date(const Date& copy) noexcept
    : date(copy.date)
  {
  }

  /**
    Assignment of date with date.
  */
  inline Date& operator=(const Date& assign) noexcept
  {
    date = assign.date;
    return *this;
  }

  /**
    Returns the number of microseconds elapsed since 00:00:00 on January 1,
    1970, Coordinated Universal Time (UTC).
  */
  inline int64 getValue() const noexcept
  {
    return date;
  }
  
  /** Returns true if date is not epoch. */
  inline operator bool() const noexcept
  {
    return date != 0;
  }

  /** Converts from UTC to local time. */
  Date getLocalTime() const noexcept
  {
    // UTC = local time + bias
    return date - Date::getBias();
  }

  /** Converts from local to UTC time. */
  Date getUTCTime() const noexcept
  {
    // UTC = local time + bias
    return date + Date::getBias();
  }
  
  /**
    Adds a bias (in microseconds) to the date.
  */
  inline void addBias(int64 bias) noexcept
  {
    date += bias;
  }
  
  /**
    Returns the millisecond in local time.
  */
  int getMillisecond() const noexcept;
  
  /**
    Returns the second in local time.
  */
  int getSecond() const noexcept;

  /**
    Returns the minute in local time.
  */
  int getMinute() const noexcept;

  /**
    Returns the hour in local time.
  */
  int getHour() const noexcept;

  /**
    Returns the day of the month in local time. [1-31].
  */
  int getDay() const noexcept;

  /**
    Returns the day of the week in local time. Week starts at Sunday (0).
  */
  int getDayOfWeek() const noexcept;

  /**
    Returns the day of the year in local time.
  */
  int getDayOfYear() const noexcept;
  
  /**
    Returns the month in local time. 0-11.
  */
  int getMonth() const noexcept;
  
  /**
    Returns the year in local time.
  */
  int getYear() const noexcept;
  
  /**
    Returns the Julian day.
  */
  int getJulianDay() const noexcept;
  
  /**
    Fills the given time structure with the year, month, day of month, day of
    week, hour, minute, second, and millisecond (if supported).

    @param time The time structure.
    @param local Specifies that time should be converted to local time from UTC.
  */
  DateTime split(bool local = false) const noexcept;

  /** Returns date/time in ISO 8601 format. E.g. 2020-01-31T18:00:00Z. */
  String getISO8601(int offset = 0) const;

  /** Returns date/time in ISO 8601 format with milliseconds. E.g. 2020-01-31T18:00:00.123Z. */
  String getISO8601_MS() const;

  /** Returns date/time in ISO 8601 format with microseconds. E.g. 2020-01-31T18:00:00.123456Z. */
  String getISO8601_US() const;

  // TAG: add support for timezone
  /** Returns date/time in ISO 8601 format. E.g. 2020-01-31T18:00:00Z. */
  static Date parseISO8601(const String& text, bool subsecond = false);

  /**
    Returns the date/time as a string.

    @param format The desired format of the resulting string.
  */
  String format(const String& format) const;
  
  /**
    Returns the date/time as a string.

    @param format The desired format of the resulting string.
  */
  WideString format(const WideString& format) const;
};

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Date& date);

template<>
class IsUninitializeable<Date> : public IsUninitializeable<int64> {
};

template<>
class _COM_AZURE_DEV__BASE__API Hash<Date> {
public:

  inline unsigned long operator()(const Date& value) noexcept
  {
    Hash<int64> hash;
    return hash(value.getValue());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
