/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Integer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Integer::Integer(String str) throw(InvalidFormat) {
  unsigned int length = str.getLength();
  bool sign = false;
  unsigned int index = 0;

  if (length == 0) {
    throw InvalidFormat("Not an Integer");
  }

  if (str[0] == '-') {
    sign = true;
    ++index;
  }

  long long temp = 0;

  while (index < length) {
    char ch = str[index++];

    if ((ch < '0') || (ch > '9')) {
      throw InvalidFormat("Not an Integer");
    }

    temp = temp * 10 + (ch - '0') % 10;

    if ((sign && (-temp < Integer::MINIMUM)) || (!sign && (temp > Integer::MAXIMUM))) {
      throw InvalidFormat("Not an Integer");
    }
  }

  value = sign ? -temp : temp;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
