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
#include <base/string/Locale.h>
#include <locale.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String Locale::getLocale() throw() {
  return String(setlocale(LC_ALL, 0));
}

void Locale::setUserLocale() throw() {
  setlocale(LC_ALL, "");
}

void Locale::setASCIILocale() throw() {
  setlocale(LC_ALL, "C"); // TAG: or should we use POSIX
}

bool Locale::setLocale(const String& locale) throw() {
  return setlocale(LC_ALL, locale.getElements()) != 0;
}

void Locale::getLocaleSpecific() throw() {
  // locale name???
  // select locale - lock
  struct lconv* p = localeconv();
  // copy to objects
  // restore locale - unlock
}

/*
struct DateFormatSet {
  char separator;
};


struct CollectionFormatSet {
  char begin;
  char end;
  char separator;
};

class CollectionFormatSet2 {
private:
public:
  FormatSet(C)
  operator const CollectionFormatSet& throw() {
    return set;
  }
};

CollectionFormatSet defaultFormatSet {
  '{'
  '}'
  ';'
};
*/

DateFormatSet::DateFormatSet() throw() {  
  shortNameOfWeekday[0] = MESSAGE("Sun");
  shortNameOfWeekday[1] = MESSAGE("Mon");
  shortNameOfWeekday[2] = MESSAGE("Tue");
  shortNameOfWeekday[3] = MESSAGE("Wed");
  shortNameOfWeekday[4] = MESSAGE("Thu");
  shortNameOfWeekday[5] = MESSAGE("Fre");
  shortNameOfWeekday[6] = MESSAGE("Sat");
  
  longNameOfWeekday[0] = MESSAGE("Sunday");
  longNameOfWeekday[1] = MESSAGE("Monday");
  longNameOfWeekday[2] = MESSAGE("Tuesday");
  longNameOfWeekday[3] = MESSAGE("Wednesday");
  longNameOfWeekday[4] = MESSAGE("Thursday");
  longNameOfWeekday[5] = MESSAGE("Freday");
  longNameOfWeekday[6] = MESSAGE("Saturday");
  
  shortNameOfMonth[0] = MESSAGE("Jan");
  shortNameOfMonth[1] = MESSAGE("Feb");
  shortNameOfMonth[2] = MESSAGE("Mar");
  shortNameOfMonth[3] = MESSAGE("Apr");
  shortNameOfMonth[4] = MESSAGE("May");
  shortNameOfMonth[5] = MESSAGE("Jun");
  shortNameOfMonth[6] = MESSAGE("Jul");
  shortNameOfMonth[7] = MESSAGE("Aug");
  shortNameOfMonth[8] = MESSAGE("Sep");
  shortNameOfMonth[9] = MESSAGE("Oct");
  shortNameOfMonth[10] = MESSAGE("Nov");
  shortNameOfMonth[11] = MESSAGE("Dec");
  
  longNameOfMonth[0] = MESSAGE("January");
  longNameOfMonth[1] = MESSAGE("February");
  longNameOfMonth[2] = MESSAGE("March");
  longNameOfMonth[3] = MESSAGE("April");
  longNameOfMonth[4] = MESSAGE("May");
  longNameOfMonth[5] = MESSAGE("June");
  longNameOfMonth[6] = MESSAGE("July");
  longNameOfMonth[7] = MESSAGE("August");
  longNameOfMonth[8] = MESSAGE("September");
  longNameOfMonth[9] = MESSAGE("October");
  longNameOfMonth[10] = MESSAGE("November");
  longNameOfMonth[11] = MESSAGE("December");
  
  dateSeparator = '-';
  timeSeparator = ':';
  am = MESSAGE("AM");
  pm = MESSAGE("PM");
  
  shortTimeFormat = MESSAGE("%H:%M");
  mediumTimeFormat = MESSAGE("%H:%M:%S");
  longTimeFormat = MESSAGE("%H:%M:%S.%.");
  shortDateFormat = MESSAGE("%y-%m-%d");
  mediumDateFormat = MESSAGE("%d-%b-%Y");
  longDateFormat = MESSAGE("%A, %B %d, %Y");
}

Locale::Locale() throw() {  
  falseMessage = MESSAGE("false");
  trueMessage = MESSAGE("true");

  listBegin = '{';
  listEnd = '}';
  listSeparator = ';';
  vectorBegin = '(';
  vectorEnd = ')';
  vectorSeparator = ',';
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
