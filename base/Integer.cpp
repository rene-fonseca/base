/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Integer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

int Integer::parse(const String& str) throw(InvalidFormat) {
  unsigned int length = str.getLength();
  bool sign = false;
  unsigned int index = 0;

  assert(length > 0, InvalidFormat("Not an Integer"));

  if (str[0] == '-') {
    sign = true;
    ++index;
  }

  long long temp = 0;

  while (index < length) {
    char ch = str[index++];

    assert(ASCIITraits::isDigit(ch), InvalidFormat("Not an Integer"));
    temp = temp * 10 + ASCIITraits::digitToValue(ch);

    if ((sign && (-temp < Integer::MINIMUM)) || (!sign && (temp > Integer::MAXIMUM))) {
      throw InvalidFormat("Not an Integer");
    }
  }

  return sign ? -temp : temp;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
