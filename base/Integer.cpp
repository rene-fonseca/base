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
#include <base/string/ASCIITraits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

int Integer::parse(const String& str, bool withoutSign) throw(InvalidFormat) {
  String::ReadIterator i = str.getBeginReadIterator();
  const String::ReadIterator end = str.getEndReadIterator();
  
  while ((i < end) && (*i == ' ')) {
    ++i; // eat space
  }
  
  assert(i < end, InvalidFormat(Type::getType<Integer>()));
  
  long long value;
  if (withoutSign) {
    unsigned long long temp = 0;
    while (i < end) {
      char ch = *i++;
      if (!ASCIITraits::isDigit(ch)) {
        break;
      }
      assert(temp < PrimitiveTraits<unsigned int>::MAXIMUM,
             InvalidFormat(Type::getType<Integer>()));
      temp = temp * 10 + ASCIITraits::digitToValue(ch);
    }
    value = temp;
  } else {
    bool negative = false;
    if (*i == '-') {
      negative = true;
      ++i; // yummy that tasted great
    } else if (*i == '+') {
      ++i; // I'm not full yet
    }

    value = 0; // overflow not possible (2 * sizeof(int) == sizeof(long long))
    if (negative) {
      while (i < end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        value = value * 10 - ASCIITraits::digitToValue(ch);
        assert(value >= Integer::MINIMUM, InvalidFormat(Type::getType<Integer>()));
      }
    } else {
      while (i < end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }      
        value = value * 10 + ASCIITraits::digitToValue(ch);
        assert(value <= Integer::MAXIMUM, InvalidFormat(Type::getType<Integer>()));
      }
    }
  }
  
  while ((i < end) && (*i == ' ')) { // rest must be spaces
    ++i;
  }
  
  assert(i == end, InvalidFormat(Type::getType<Integer>()));
  return value;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
