/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

/*
  POSIX:
    beginNegative = "-"
    beginPositive = "+"
    endNegative = ""
    endPositive = ""
    radix = "."
    grouping = ""
*/

class NumericFormatSet {
private:
  char* decimalPoint;
  char* thousandsseparator;
  char* grouping;
public:
};

class MonetaryFormatSet {
public:

  typedef enum {PARENTHESES, PRECEDES_QUANTITY, SUCCEEDS_QUANTITY, PRECEDES_SYMBOL, SUCCEEDS_SYMBOL} SignPosition;
private:
  char* int_currencySymbol;
  char* currencySymbol; /* Local currency symbol.  */
  char* decimalPoint;    /* Decimal point character.  */
  char* thousandsSeparator;    /* Thousands separator.  */
  char* grouping;         /* Like `grouping' element (above).  */
  char* positiveSign;        /* Sign for positive values.  */
  char* negativeSign;        /* Sign for negative values.  */
  int int_fractionalDigits;       /* Int'l fractional digits.  */
  int fractionalDigits;      /* Local fractional digits.  */
  /* 1 if currency_symbol precedes a positive value, 0 if succeeds.  */
  bool positiveSymbolPrecedes;
  /* 1 if a space separates currency_symbol from a positive value.  */
  bool positiveSymbolSeparated;
  /* 1 if currency_symbol precedes a negative value, 0 if succeeds.  */
  bool negativeSymbolPrecedes;
  /* 1 if a space separates currency_symbol from a negative value.  */
  bool negativeSymbolSeparated;
  /* Positive and negative sign positions:
     0 Parentheses surround the quantity and currency_symbol.
     1 The sign string precedes the quantity and currency_symbol.
     2 The sign string succeeds the quantity and currency_symbol.
     3 The sign string immediately precedes the currency_symbol.
     4 The sign string immediately succeeds the currency_symbol.  */
  //SignPosition positiveSign;
  //SignPosition negativeSign;
public:
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
