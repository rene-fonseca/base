/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ShortInteger.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

short ShortInteger::parse(const char* src, const char* end, unsigned int flags)
{
  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) {
      ++src; // eat space
    }
  }

  if (src == end) {
    _throw InvalidFormat(Type::getType<ShortInteger>());
  }
  
  int value = 0;
  if ((flags & FLAG_ALLOW_SIGN) == 0) {
    unsigned int temp = 0;
    while (src != end) {
      char ch = *src++;
      if (!ASCIITraits::isDigit(ch)) {
        break;
      }
      bassert(temp < PrimitiveTraits<unsigned short>::MAXIMUM, InvalidFormat(Type::getType<ShortInteger>()));
      temp = temp * 10 + ASCIITraits::digitToValue(ch);
    }
    value = temp;
  } else {
    bool negative = false;
    if (*src == '-') {
      negative = true;
      ++src;
    } else if (*src == '+') {
      ++src;
    }

    value = 0; // overflow not possible (2 * sizeof(short) == sizeof(int))
    if (negative) {
      while (src != end) {
        char ch = *src++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        value = value * 10 - ASCIITraits::digitToValue(ch);
        bassert(value >= ShortInteger::MINIMUM, InvalidFormat(Type::getType<ShortInteger>()));
      }
    } else {
      while (src != end) {
        char ch = *src++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }      
        value = value * 10 + ASCIITraits::digitToValue(ch);
        bassert(value <= ShortInteger::MAXIMUM, InvalidFormat(Type::getType<ShortInteger>()));
      }
    }
  }
  
  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) { // rest must be spaces
      ++src;
    }
  }

  bassert(src == end, InvalidFormat(Type::getType<ShortInteger>()));
  return value;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
