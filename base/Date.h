/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DATE_H
#define _DK_SDU_MIP__BASE__DATE_H

#include <base/DateException.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class FormatOutputStream;

/**
  Representation of date/time in Coordinated Universal Time (UTC).

  @short Date and time.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.3
*/

class Date {
private:

  /** Seconds elapsed since epoch (i.e. 00:00:00 on January 1, 1970, Coordinated Universal Time (UTC)). */
  int date; // TAG: need 64bit accuracy - long long
public:

  enum Constants {
    SUNDAY = 0, /**< Index of Sunday. */
    MONDAY = 1, /**< Index of Monday. */
    TUESDAY = 2, /**< Index of Tuesday. */
    WEDNESDAY = 3, /**< Index of Wednesday. */
    THURSDAY = 4, /**< Index of Thursday. */
    FRIDAY = 5, /**< Index of Friday. */
    SATURDAY = 6, /**< Index of Saturday. */
    EPOCH_YEAR = 1970, /**< The year of the epoch. */
    EPOCH_WEEKDAY = THURSDAY, /**< The weekday of the first epoch day . */
    DAYS_PER_NONLEAP_YEAR = 365, /**< The number of days per non-leap year. */
    DAYS_PER_LEAP_YEAR = DAYS_PER_NONLEAP_YEAR + 1, /** The number of days per leap year. */
    DAYS_PER_400_YEARS = (300+4-1)*365 + (100-4+1)*366, /**< The number of days per 400 years. */
    DAYS_PER_WEEK = 7, /**< The number of days per week. */
    MONTHS_PER_YEAR = 12, /** The number of months per year. */
    HOURS_PER_DAY = 24, /**< The number of hours per day. */
    MINUTES_PER_HOUR = 60, /**< The number of minutes per hour. */
    MINUTES_PER_DAY = MINUTES_PER_HOUR * HOURS_PER_DAY, /**< The number of minutes per day. */
    SECONDS_PER_MINUTE = 60, /** The number of seconds per minute. */
    SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR, /**< The number of seconds per hour. */
    SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY /**< The number of seconds per day. */
  };
  
  /** The days per month for non-leap years. */
  static const int DAYS_PER_MONTH_NONLEAP_YEAR[MONTHS_PER_YEAR];
  /** The days per month for leap years. */
  static const int DAYS_PER_MONTH_LEAP_YEAR[MONTHS_PER_YEAR];
  /** The days before the first of the month for non-leap years. */
  static const int DAYS_BEFORE_FIRST_OF_MONTH_NONLEAP_YEAR[MONTHS_PER_YEAR + 1];
  /** The days before the first of the month for leap years. */
  static const int DAYS_BEFORE_FIRST_OF_MONTH_LEAP_YEAR[MONTHS_PER_YEAR + 1];
  
  struct DateTime {
    int year; /**< The year. */
    int month; /**< Operating range is [0;11]. */
    int day; /**< Operating range is [0;30]. */
    int weekday; /**< Operating range is [0;6]. */
    int dayOfYear; /** Operating range is [0;365]. */
    int week; /**< Operating range is [0;53]. */
    int hour; /**< Operating range is [0;23]. */
    int minute; /**< Operating range is [0;59]. */
    int second; /**< Operating range is [0;59]. */
    int millisecond; /**< Operating range is [0-999]. */
  };
  
  /**
    Normalizes the specified data/time structure such that the fields are
    brought into their normal operating range.

    @param dateTime The structure to normalize.
    @param redundancy Specifies whether or not to update all fields. The
    default is to update all fields.
    
    @return The year carrier. Only non-zero if the year falls outside the range [-9999;9999].
  */
  static int normalize(DateTime& dateTime, bool redundancy = true) throw();
  
  /**
    Returns true if the year is a leap year.
  */
  static inline bool isLeapYear(int year) throw() {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
  }
  
  /**
    Returns the number of days in the specified year.
  */
  static inline int getDaysOfYear(int year) throw() {
    return isLeapYear(year) ? 366 : 365;
  }
  
  /**
    Returns the number of days in the specified month.
  */
  static int getDaysOfMonth(int month, int year) throw(OutOfDomain);

  /**
    Returns the day of the week (Sunday = 0, Monday = 1, etc.). The result is
    unspecified for invalid dates.
  */
  static int getDayOfWeek(int day, int month, int year) throw();

  /**
    Returns the week of the year.
  */
  static int getWeek(int day, int month, int year) throw();

  /**
    Returns the day (in the range [0; 365]) of the year.

    @param day The day of the month [0;30].
    @param month The month of the year [0;11].
    @param year The year.
  */
  static int getDayOfYear(int day, int month, int year) throw();

  /**
    Returns the current time in UTC time.
  */
  static Date getNow() throw(DateException);
  
  /**
    Returns the bias (in seconds) for convertion from UTC to local time. Bias
    may change during the lifetime of the process. This method only works on
    some planet called Earth :-].
  */
  static int getBias() throw();
  
  /**
    Returns date object for the specified time. Invalid values will be
    normalized. Raises 'DateException' if the date cannot be represented.
    
    @param second The second.
    @param minute The minute.
    @param hour The hour.
    @param local Specifies that the time is given in local time. The default is UTC time.
  */
  static Date getTime(int second, int minute, int hour, bool local = false) throw(DateException);
  
  /**
    Returns date object for the specified date. Invalid values will be
    normalized. Raises 'SystemException' if the date cannot be represented.

    @param day The day of the month.
    @param month The month of the year.
    @param year The year.
    @param local Specifies that the time is given in local time. The default is UTC time.
  */
  static Date getDate(int day, int month, int year, bool local = false) throw(DateException);

  /**
    Returns date object for the specified date and time. Invalid values will be
    normalized. Raises 'SystemException' if the date cannot be represented.

    @param second The second.
    @param minute The minute.
    @param hour The hour.
    @param day The day of the month.
    @param month The month of the year.
    @param year The year.
    @param local Specifies that the time is given in local time. The default is UTC time.
  */
  static Date getDate(int second, int minute, int hour, int day, int month, int year, bool local = false) throw(DateException);
  
  /**
    Returns the date corresponding to the specified Julian day.
  */
  static Date getDateByJulianDay(int day) throw();
  
  /**
    Initializes the date as 00:00:00 on January 1, 1970.
  */
  inline Date() throw() : date(0) {}
  
  /**
    Initializes the date with the specified date.

    @param date The number of seconds elapsed since 00:00:00 on January 1, 1970,
    Coordinated Universal Time (UTC).
  */
  Date(int date) throw();

  /**
    Initializes object from date/time structure.
  */
  Date(const DateTime& dateTime) throw();
  
  /**
    Initialize date from other date.
  */
  inline Date(const Date& copy) throw() : date(copy.date) {}

  /**
    Assignment of date with date.
  */
  inline Date& operator=(const Date& eq) throw() {date = eq.date; return *this;}

  /**
    Returns the seconds elapsed since 00:00:00 on January 1, 1970, Coordinated Universal Time (UTC).
  */
  inline int getValue() const throw() {return date;}

  /**
    Adds a bias (in seconds) to the date.
  */
  inline void addBias(int bias) throw() {
    date += bias;
  }
  
  /**
    Returns the second in local time.
  */
  int getSecond() const throw();

  /**
    Returns the minute in local time.
  */
  int getMinute() const throw();

  /**
    Returns the hour in local time.
  */
  int getHour() const throw();

  /**
    Returns the day of the month in local time.
  */
  int getDay() const throw();

  /**
    Returns the day of the week in local time.
  */
  int getDayOfWeek() const throw();

  /**
    Returns the day of the year in local time.
  */
  int getDayOfYear() const throw();
  
  /**
    Returns the month in local time.
  */
  int getMonth() const throw();
  
  /**
    Returns the year in local time.
  */
  int getYear() const throw();
  
  /**
    Returns the second in UTC.
  */
  int getUTCSecond() const throw();
  
  /**
    Returns the minute in UTC.
  */
  int getUTCMinute() const throw();

  /**
    Returns the hour in UTC.
  */
  int getUTCHour() const throw();

  /**
    Returns the day of the month in UTC.
  */
  int getUTCDay() const throw();

  /**
    Returns the day of the week in UTC.
  */
  int getUTCDayOfWeek() const throw();

  /**
    Returns the day of the year in UTC.
  */
  int getUTCDayOfYear() const throw();

  /**
    Returns the month in UTC.
  */
  int getUTCMonth() const throw();

  /**
    Returns the year in UTC.
  */
  int getUTCYear() const throw();  

  /**
    Returns the Julian day.
  */
  int getJulianDay() const throw();
  
  /**
    Fills the given time structure with the year, month, day of month, day of
    week, hour, minute, second, and millisecond (if supported).

    @param time The time structure.
    @param local Specifies whether the time is in local time.
  */
  void split(DateTime& time, bool local = false) const throw();

  /**
    Returns the date/time as a string.

    @param format The desired format of the resulting string.
    @param local When true, specifies that the time should be presented in local
    time otherwise UTC is assumed. The Default is true (local).
  */
  String format(const String& format, bool local = true) const throw(InvalidFormat, MemoryException);
  
  /**
    Returns the date/time as a string.

    @param format The desired format of the resulting string.
    @param local When true, specifies that the time should be presented in local
    time otherwise UTC is assumed. The Default is true (local).
  */
  WideString format(const WideString& format, bool local = true) const throw(InvalidFormat, MemoryException);
};

inline Date::Date(int _date) throw() : date(_date) {}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Date& date) throw(InvalidFormat, IOException);

template<>
class Relocateable<Date> {
public:

  static const bool IS_RELOCATEABLE = Relocateable<int>::IS_RELOCATEABLE; // TAG: fixme - long long
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
