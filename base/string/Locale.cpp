/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

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
  operator const CollectionFormatSet& throw() {return set;}
};

CollectionFormatSet defaultFormatSet {
  '{'
  '}'
  ';'
};
*/

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
