/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__LOCALE_H
#define _DK_SDU_MIP__BASE_STRING__LOCALE_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/OutOfDomain.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class NumericFormatSet : public Object {
private:

  /** The number of digits in a group (0 ~ disables grouping). */
  unsigned int groupingDigits;
  /** The grouping symbol. */
  char groupingSymbol;
  /** Decimal point symbol. */
  char decimalSymbol;
public:

  NumericFormatSet() throw() {
    groupingDigits = 0;
    groupingSymbol = ',';
    decimalSymbol = '.';
  }
  
};

class MonetaryFormatSet : public Object {
public:

  /** Relative currency positions. */
  enum CurrencyPosition {
    /**
      Currency symbol immediately precedes the quantity (i.e. symbol and
      quantity are not separated).
    */
    PRECEDES_QUANTITY,
    /**
      Currency symbol immediately succeeds the quantity (i.e. symbol and
      quantity are not separated).
    */
    SUCCEEDS_QUANTITY,
    /**
      Currency symbol precedes the quantity but is separated with a space.
    */
    PRECEDES_QUANTITY_WITH_SPACE,
    /**
      Currency symbol succeeds the quantity but is separated with a space.
    */
    SUCCEEDS_QUANTITY_WITH_SPACE,
  };

  /** Relative sign positions. */
  enum SignPosition {
    /** Quantity is contained within parentheses. */
    PARENTHESES,
    /** Sign precedes quantity and currency symbol. */
    SIGN_PRECEDES_QUANTITY,
    /** Sign succeeds quantity and currency symbol. */
    SIGN_SUCCEEDS_QUANTITY,
    /** Sign immediately precedes the currency symbol. */
    SIGN_PRECEDES_SYMBOL,
    /** Sign immediately succeeds the currency symbol. */
    SIGN_SUCCEEDS_SYMBOL
  };
private:
  
  String currencySymbol; /**< Currency symbol. */
  unsigned int fractionalDigits; /**< Number of fractional digits. */
  unsigned int groupingDigits; /**< The number of digits in a group (0 ~ disables grouping). */
  char groupingSymbol; /**< The grouping symbol. */
  char decimalSymbol; /**< Decimal point symbol. */
  struct {
    /** The position of the currency symbol for negative quantities.*/
    CurrencyPosition currencyPosition;
    /** The position of the sign designator for negative quantities.*/
    SignPosition signPosition;
  } negative;
  struct {
    /** The position of the currency symbol for positive quantities.*/
    CurrencyPosition currencyPosition;
    /** The position of the sign designator for positive quantities.*/
    SignPosition signPosition;
  } positive;
public:

  MonetaryFormatSet() throw() {
    currencySymbol = "";
    fractionalDigits = 2;
    groupingDigits = 0;
    groupingSymbol = ',';
    decimalSymbol = '.';
    negative.currencyPosition = PRECEDES_QUANTITY_WITH_SPACE;
    negative.signPosition = SIGN_SUCCEEDS_SYMBOL;
    positive.currencyPosition = PRECEDES_QUANTITY_WITH_SPACE;    
    positive.signPosition = SIGN_SUCCEEDS_SYMBOL;
  }
};

class DateFormatSet : public Object {
private:

  /** The abbreviated name of weekdays. */
  String shortNameOfWeekday[7];
  /** The full name of weekdays. */
  String longNameOfWeekday[7];
  /** The abbreviated name of months. */
  String shortNameOfMonth[12];
  /** The full name of months. */
  String longNameOfMonth[12];
  
  enum DayFormat {
    D, /**< One digit day (1-7). */
    DD, /**< Two digit day (01-07). */
    DDD, /**< Day is given by its short name. */
    DDDD /**< Day is given by its long name. */
  };

  enum MonthFormat {
    M, /**< Month is given by its number (1-12). */
    MM, /**< Month given by two digits (01-12). */
    MMM, /**< Month is given by short name. */
    MMMM /**< Month is given by its long name. */
  };
  
  enum YearFormat {
    YY, /**< Two digit year. */
    YYYY /**< Four digit year. */
  };
  
  enum DateStandardFormat {
    SHORT_DATE_FORMAT,
    MIDDLE_DATE_FORMAT,
    LONG_DATE_FORMAT
  };

  /** Date separator used with short formats. */
  char dateSeparator;
  /** Time separator. */
  char timeSeparator;
  /** AM symbol. */
  String am;
  /** PM symbol. */
  String pm;
  
  String shortTimeFormat;
  String mediumTimeFormat;
  String longTimeFormat;
  String shortDateFormat;
  String mediumDateFormat;
  String longDateFormat;
public:

// TAG: need time zone support - specifies local time offset from UTC time
//  int getOffset() const throw();
//  void setOffset(int offset) throw();
//  String getTimeZone() throw(); returns (+|-)hh:mm if unknown name
  
  /**
    Initializes object with POSIX settings.
  */
  DateFormatSet() throw();

  inline char getDateSeparator() const throw() {
    return dateSeparator;
  }

  inline char getTimeSeparator() const throw() {
    return timeSeparator;
  }

  String getShortNameOfWeekday(unsigned int weekday) const throw(OutOfDomain) {
    assert(weekday < 7, OutOfDomain(Type::getType<DateFormatSet>()));
    return shortNameOfWeekday[weekday];
  }
  
  String getLongNameOfWeekday(unsigned int weekday) const throw(OutOfDomain) {
    assert(weekday < 7, OutOfDomain(Type::getType<DateFormatSet>()));
    return longNameOfWeekday[weekday];
  }
  
  String getShortNameOfMonth(unsigned int month) const throw(OutOfDomain) {
    assert(month < 12, OutOfDomain(Type::getType<DateFormatSet>()));
    return shortNameOfMonth[month];
  }
  
  String getLongNameOfMonth(unsigned int month) const throw(OutOfDomain) {
    assert(month < 12, OutOfDomain(Type::getType<DateFormatSet>()));
    return longNameOfMonth[month];
  }

  inline String getPM() const throw() {
    return pm;
  }

  inline String getAM() const throw() {
    return am;
  }
  


  inline String getShortDateFormat() const throw() {
    return shortDateFormat;
  }
  
  void setShortDateFormat(const String& format) throw();
  
  inline String getMediumDateFormat() const throw() {
    return mediumDateFormat;
  }
  
  void setMediumDateFormat(const String& format) throw();
  
  inline String getLongDateFormat() const throw() {
    return longDateFormat;
  }
  
  void setLongDateFormat(const String& format) throw();

  inline String getShortTimeFormat() const throw() {
    return shortTimeFormat;
  }
  
  void setShortTimeFormat(const String& format) throw();
  
  inline String getMediumTimeFormat() const throw() {
    return mediumTimeFormat;
  }
  
  void setMediumTimeFormat(const String& format) throw();
  
  inline String getLongTimeFormat() const throw() {
    return longTimeFormat;
  }
  
  void setLongTimeFormat(const String& format) throw();
};

/**
  This class provides multi-language support.

  @short Multi-language support
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class Locale : public DateFormatSet {
public:

  static const Locale POSIX;

  //static void enable() throw(IOException);
  
  Locale() throw();
  
  /**
    Returns the current locale as a string.
  */
  static String getLocale() throw();
  
  /**
    Sets the application locale according to the environment settings.
  */
  static void setUserLocale() throw();
  
  /**
    Sets the locale of the application.
  */
  static void setASCIILocale() throw();
  
  /**
    Sets the locale of the application.

    @return True on success.
  */
  static bool setLocale(const String& locale) throw();

  /**
    Queries the locale specific numeric and monetary information.
  */
  static void getLocaleSpecific() throw();


  char listBegin; // {
  char listEnd; // }
  char listSeparator; // ;
  char vectorBegin; // (
  char vectorEnd; // )
  char vectorSeparator; // ,
  
  inline char getListSeparator() const throw() {
    return listSeparator;
  }

  String falseMessage;
  String trueMessage;
  
  inline const String& getBoolean(bool value) const throw() {
    if (value) {
      return trueMessage;
    } else {
      return falseMessage;
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
