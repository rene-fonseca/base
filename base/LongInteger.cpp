/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/LongInteger.h>
#include <base/string/ASCIITraits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

long long LongInteger::parse(const String& string, bool withoutSign) throw(InvalidFormat) {
  String::ReadIterator i = string.getBeginReadIterator();
  const String::ReadIterator end = string.getEndReadIterator();

  while ((i < end) && (*i == ' ')) {
    ++i; // eat space
  }
  
  assert(i < end, InvalidFormat("Not an integer", Type::getType<LongInteger>())); // do not accept empty strings
  
  long long value;
  if (withoutSign) {
    unsigned long long temp = 0;
    while (i < end) {
      char ch = *i++;
      if (!ASCIITraits::isDigit(ch)) {
        break;
      }
      assert(
        (temp < PrimitiveTraits<unsigned long long>::MAXIMUM/10) ||
        ((temp == PrimitiveTraits<unsigned long long>::MAXIMUM/10) &&
         (ASCIITraits::digitToValue(ch) <= PrimitiveTraits<unsigned long long>::MAXIMUM%10)),
        InvalidFormat(Type::getType<LongInteger>()));
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

    value = 0;
    if (negative) {
      while (i < end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        assert(
          (value > LongInteger::MINIMUM/10) ||
          ((value == LongInteger::MINIMUM/10) && (-ASCIITraits::digitToValue(ch) >= LongInteger::MINIMUM%10)),
          InvalidFormat(Type::getType<LongInteger>()));
        value = value * 10 - ASCIITraits::digitToValue(ch);
      }
    } else {
      while (i < end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        assert(
          (value < LongInteger::MAXIMUM/10) ||
          ((value == LongInteger::MAXIMUM/10) && (ASCIITraits::digitToValue(ch) <= LongInteger::MAXIMUM%10)),
          InvalidFormat(Type::getType<LongInteger>()));
        value = value * 10 + ASCIITraits::digitToValue(ch);
      }
    }
  }
  
  while ((i < end) && (*i == ' ')) { // rest must be spaces
    ++i;
  }

  assert(i == end, InvalidFormat(Type::getType<LongInteger>()));
  return value;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
