/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ShortInteger.h>
#include <base/string/ASCIITraits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

short ShortInteger::parse(const String& str, bool withoutSign) throw(InvalidFormat) {
  String::ReadIterator i = str.getBeginReadIterator();
  const String::ReadIterator end = str.getEndReadIterator();
  
  while ((i < end) && (*i == ' ')) {
    ++i; // eat space
  }
  
  assert(i < end, InvalidFormat(Type::getType<ShortInteger>()));
  
  int value;
  if (withoutSign) {
    unsigned int temp = 0;
    while (i < end) {
      char ch = *i++;
      if (!ASCIITraits::isDigit(ch)) {
        break;
      }
      assert(temp < PrimitiveTraits<unsigned short>::MAXIMUM,
             InvalidFormat(Type::getType<ShortInteger>()));
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

    value = 0; // overflow not possible (2 * sizeof(short) == sizeof(int))
    if (negative) {
      while (i < end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        value = value * 10 - ASCIITraits::digitToValue(ch);
        assert(value >= ShortInteger::MINIMUM, InvalidFormat(Type::getType<ShortInteger>()));
      }
    } else {
      while (i < end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }      
        value = value * 10 + ASCIITraits::digitToValue(ch);
        assert(value <= ShortInteger::MAXIMUM, InvalidFormat(Type::getType<ShortInteger>()));
      }
    }
  }
  
  while ((i < end) && (*i == ' ')) { // rest must be spaces
    ++i;
  }
  
  assert(i == end, InvalidFormat(Type::getType<ShortInteger>()));
  return value;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
