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

int Integer::parse(const char* src, const char* end, unsigned int flags) throw(InvalidFormat)
{
  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) {
      ++src; // eat space
    }
  }
  
  if (src == end) {
    throw InvalidFormat(Type::getType<Integer>());
  }
  
  long long value = 0;
  if ((flags & FLAG_ALLOW_SIGN) == 0) {
    unsigned long long temp = 0;
    while (src != end) {
      char ch = *src++;
      if (!ASCIITraits::isDigit(ch)) {
        break;
      }
      bassert(temp < PrimitiveTraits<unsigned int>::MAXIMUM, InvalidFormat(Type::getType<Integer>()));
      temp = temp * 10 + ASCIITraits::digitToValue(ch);
    }
    value = temp;
  } else {
    bool negative = false;
    if (*src == '-') {
      negative = true;
      ++src; // yummy that tasted great
    } else if (*src == '+') {
      ++src; // I'm not full yet
    }

    value = 0; // overflow not possible (2 * sizeof(int) == sizeof(long long))
    if (negative) {
      while (src != end) {
        char ch = *src++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        value = value * 10 - ASCIITraits::digitToValue(ch);
        bassert(value >= Integer::MINIMUM, InvalidFormat(Type::getType<Integer>()));
      }
    } else {
      while (src != end) {
        char ch = *src++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }      
        value = value * 10 + ASCIITraits::digitToValue(ch);
        bassert(value <= Integer::MAXIMUM, InvalidFormat(Type::getType<Integer>()));
      }
    }
  }
  
  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) { // rest must be spaces
      ++src;
    }
  }
  
  if (src != end) {
    throw InvalidFormat(Type::getType<Integer>());
  }
  return value;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
