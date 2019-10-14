/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Integer.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: add support for wchar for parsing
// TAG: use iterator instead

#if 0
int Integer::parse(const char* src, const char* end, bool withoutSign) throw(InvalidFormat)
{
  return -1;
}
#endif

int Integer::parse(const String& str, bool withoutSign) throw(InvalidFormat)
{
  String::ReadIterator i = str.getBeginReadIterator();
  const String::ReadIterator end = str.getEndReadIterator();
  
  while ((i < end) && (*i == ' ')) {
    ++i; // eat space
  }
  
  bassert(i < end, InvalidFormat(Type::getType<Integer>()));
  
  long long value = 0;
  if (withoutSign) {
    unsigned long long temp = 0;
    while (i < end) {
      char ch = *i++;
      if (!ASCIITraits::isDigit(ch)) {
        break;
      }
      bassert(temp < PrimitiveTraits<unsigned int>::MAXIMUM,
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
        bassert(value >= Integer::MINIMUM, InvalidFormat(Type::getType<Integer>()));
      }
    } else {
      while (i < end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }      
        value = value * 10 + ASCIITraits::digitToValue(ch);
        bassert(value <= Integer::MAXIMUM, InvalidFormat(Type::getType<Integer>()));
      }
    }
  }
  
  while ((i < end) && (*i == ' ')) { // rest must be spaces
    ++i;
  }
  
  bassert(i == end, InvalidFormat(Type::getType<Integer>()));
  return value;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
