/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__LOCALE_H
#define _DK_SDU_MIP__BASE_STRING__LOCALE_H

#include <base/Object.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class NumericFormatSet : public Object {
private:
  
  unsigned int groupingDigits; /**< The number of digits in a group (0 ~ disables grouping). */
  char groupingSymbol; /**< The grouping symbol. */
  char decimalSymbol; /**< Decimal point symbol. */
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
    PRECEDES_QUANTITY, /**< Currency symbol immediately precedes the quantity (i.e. symbol and quantity are not separated). */
    SUCCEEDS_QUANTITY, /**< Currency symbol immediately succeeds the quantity (i.e. symbol and quantity are not separated). */
    PRECEDES_QUANTITY_WITH_SPACE, /**< Currency symbol precedes the quantity but is separated with a space. */
    SUCCEEDS_QUANTITY_WITH_SPACE, /**< Currency symbol succeeds the quantity but is separated with a space. */
  };

  /** Relative sign positions. */
  enum SignPosition {
    PARENTHESES, /**< Quantity is contained within parentheses. */
    SIGN_PRECEDES_QUANTITY, /**< Sign precedes quantity and currency symbol. */
    SIGN_SUCCEEDS_QUANTITY, /**< Sign succeeds quantity and currency symbol. */
    SIGN_PRECEDES_SYMBOL, /**< Sign immediately precedes the currency symbol. */
    SIGN_SUCCEEDS_SYMBOL /**< Sign immediately succeeds the currency symbol. */
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

  /** The short names of days. */
  String daysShort[7];
  /** The long name of days. */
  String daysLong[7];
  /** The short names of months. */
  String monthsShort[12];
  /** The long name of months. */
  String monthsLong[12];
  
  /** The order of day, month, and year. */
  enum DateOrder {
    DMY, /**< day-month-year. */
    DYM, /**< day-year-month. */
    MDY, /**< month-day-year. */
    MYD, /**< month-year-day. */
    YDM, /**< year-day-month. */
    YMD /**< year-month-day. */
  };
  
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
  char separator;
public:

  DateFormatSet() throw() {
  }
};

/**
  This class provides multi-language support.

  @short Multi-language support
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Locale : public Object {
public:

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
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
