/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Integer.h>

Integer::Integer(String<> str) throw(InvalidFormat) {
  unsigned int length = str.length();
  bool sign = false;
  int index = 0;

  if (length == 0) {
    throw InvalidFormat("Not an Integer");
  }

  if (str[0] == '-') {
    sign = true;
    ++index;
  }

  long long temp = 0;

  while (index < length) {
    char ch = str[index];

    if ((ch < '0') || (ch > '9')) {
      throw InvalidFormat("Not an Integer");
    }

    temp = temp * 10 + (ch - '0') % 10;

    if ((sign && (temp > Integer::MAXIMUM)) || (!sign && (temp > Integer::MINIMUM))) {
      throw InvalidFormat("Not an Integer");
    }
  }

  value = sign ? -temp : temp;
}
