/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/LongInteger.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

long long LongInteger::parse(
  const String& string,
  bool withoutSign) throw(InvalidFormat) {
  String::ReadIterator i = string.getBeginReadIterator();
  const String::ReadIterator end = string.getEndReadIterator();

  while ((i < end) && (*i == ' ')) {
    ++i; // eat space
  }
  
  bassert(
    i < end,
    InvalidFormat("Not an integer", Type::getType<LongInteger>())
  ); // do not accept empty strings
  
  long long value;
  if (withoutSign) {
    unsigned long long temp = 0;
    while (i < end) {
      char ch = *i++;
      if (!ASCIITraits::isDigit(ch)) {
        break;
      }
      bassert(
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
        bassert(
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
        bassert(
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

  bassert(i == end, InvalidFormat(Type::getType<LongInteger>()));
  return value;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
