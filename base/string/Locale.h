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

#include <base/Object.h>
#include <base/string/String.h>
#include <base/OutOfDomain.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API NumericFormatSet : public Object {
private:

  /** The number of digits in a group (0 ~ disables grouping). */
  unsigned int groupingDigits = 0;
  /** The grouping symbol. */
  char groupingSymbol = 0;
  /** Decimal point symbol. */
  char decimalSymbol = 0;
public:

  NumericFormatSet() noexcept
  {
    groupingDigits = 0;
    groupingSymbol = ',';
    decimalSymbol = '.';
  }
  
};

class _COM_AZURE_DEV__BASE__API MonetaryFormatSet : public Object {
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
  unsigned int fractionalDigits = 0; /**< Number of fractional digits. */
  unsigned int groupingDigits = 0; /**< The number of digits in a group (0 ~ disables grouping). */
  char groupingSymbol = 0; /**< The grouping symbol. */
  char decimalSymbol = 0; /**< Decimal point symbol. */
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

  MonetaryFormatSet() noexcept
  {
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

/** Date and time formatting. */
class _COM_AZURE_DEV__BASE__API DateFormatSet : public Object {
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
  char dateSeparator = ':';
  /** Time separator. */
  char timeSeparator = '-';
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
  // String getTimeZone() noexcept; returns (+|-)hh:mm if unknown name
  
  /**
    Initializes object with POSIX settings.
  */
  DateFormatSet() noexcept;

  /** Returns date separator. */
  inline char getDateSeparator() const noexcept
  {
    return dateSeparator;
  }

  /** Returns time separator. */
  inline char getTimeSeparator() const noexcept
  {
    return timeSeparator;
  }

  /** Returns short name of weekday. */
  String getShortNameOfWeekday(unsigned int weekday) const
  {
    bassert(weekday < 7, OutOfDomain(Type::getType<DateFormatSet>()));
    return shortNameOfWeekday[weekday];
  }
  
  /** Returns long name of weekday. */
  String getLongNameOfWeekday(unsigned int weekday) const
  {
    bassert(weekday < 7, OutOfDomain(Type::getType<DateFormatSet>()));
    return longNameOfWeekday[weekday];
  }
  
  /** Returns short name of month. */
  String getShortNameOfMonth(unsigned int month) const
  {
    bassert(month < 12, OutOfDomain(Type::getType<DateFormatSet>()));
    return shortNameOfMonth[month];
  }
  
  /** Returns long name of month. */
  String getLongNameOfMonth(unsigned int month) const
  {
    bassert(month < 12, OutOfDomain(Type::getType<DateFormatSet>()));
    return longNameOfMonth[month];
  }

  /** Returns PM. */
  inline const String& getPM() const noexcept
  {
    return pm;
  }

  /** Returns AM. */
  inline const String& getAM() const noexcept
  {
    return am;
  }
  
  /** Returns short date format. */
  inline const String& getShortDateFormat() const noexcept
  {
    return shortDateFormat;
  }
  
  /** Sets short date format. */
  void setShortDateFormat(const String& format) noexcept;
  
  inline const String& getMediumDateFormat() const noexcept
  {
    return mediumDateFormat;
  }
  
  /** Returns medium date format. */
  void setMediumDateFormat(const String& format) noexcept;
  
  /** Returns short date format. */
  inline const String& getLongDateFormat() const noexcept
  {
    return longDateFormat;
  }
  
  /** Returns long date format. */
  void setLongDateFormat(const String& format) noexcept;

  /** Returns short time format. */
  inline const String& getShortTimeFormat() const noexcept
  {
    return shortTimeFormat;
  }
  
  /** Sets short time format. */
  void setShortTimeFormat(const String& format) noexcept;
  
  /** Returns medium time format. */
  inline const String& getMediumTimeFormat() const noexcept
  {
    return mediumTimeFormat;
  }
  
  /** Sets medium time format. */
  void setMediumTimeFormat(const String& format) noexcept;
  
  /** Returns long time format. */
  inline const String& getLongTimeFormat() const noexcept
  {
    return longTimeFormat;
  }
  
  /** Sets long time format. */
  void setLongTimeFormat(const String& format) noexcept;
};

/**
  This class provides multi-language support.

  @short Multi-language support
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API Locale : public DateFormatSet {
public:

  /** POSIX locale. */
  static const Locale POSIX;
  
  String falseMessage;
  String trueMessage;
  
  /** Initializes locale. */
  Locale() noexcept;
  
  /**
    Returns the current locale as a string.
  */
  static String getLocale() noexcept;
  
  /**
    Sets the application locale according to the environment settings.
  */
  static void setUserLocale() noexcept;
  
  /**
    Sets the locale of the application.
  */
  static void setASCIILocale() noexcept;
  
  /**
    Sets the locale of the application.

    @return True on success.
  */
  static bool setLocale(const String& locale) noexcept;

  /**
    Queries the locale specific numeric and monetary information.
  */
  static void getLocaleSpecific() noexcept;
  
  /** Returns boolean as string. */
  inline const String& getBoolean(bool value) const noexcept
  {
    if (value) {
      return trueMessage;
    } else {
      return falseMessage;
    }
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
