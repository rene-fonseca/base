/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DATE_H
#define _DK_SDU_MIP__BASE__DATE_H

#include <base/Object.h>
#include <base/DateException.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Representation of date/time in Coordinated Universal Time (UTC).

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class Date : public Object {
private:

  /** Seconds elapsed since 00:00:00 on January 1, 1970, Coordinated Universal Time (UTC). */
  int date;
public:

  /**
    Returns the local time.
  */
  static Date getNow() throw(DateException);

  /**
    Returns date object for the specified time. Invalid values will be
    normalized. Throws 'DateException' if the date cannot be represented.

    @param second The second.
    @param minute The minute.
    @param hour The hour.
  */
  static Date getTime(int second, int minute, int hour) throw(DateException);

  /**
    Returns date object for the specified date. Invalid values will be
    normalized. Throws 'SystemException' if the date cannot be represented.

    @param day The day of the month.
    @param month The month of the year.
    @param year The year.
  */
  static Date getDate(int day, int month, int year) throw(DateException);

  /**
    Returns date object for the specified date and time. Invalid values will be
    normalized. Throws 'SystemException' if the date cannot be represented.

    @param second The second.
    @param minute The minute.
    @param hour The hour.
    @param day The day of the month.
    @param month The month of the year.
    @param year The year.
  */
  static Date getDate(int second, int minute, int hour, int day, int month, int year) throw(DateException);

  /**
    Initializes the date as 00:00:00 on January 1, 1970.
  */
  inline Date() throw() : date(0) {}

  /**
    Initializes the date with the specified date.

    @param date The number of seconds elapsed since 00:00:00 on January 1, 1970, Coordinated Universal Time (UTC).
  */
  inline Date(int date) throw() : date(date) {}

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
};

FormatOutputStream& operator<<(FormatOutputStream& stream, const Date& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
