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

// TAG: add type for int also
template<typename TYPE>
inline int parseInteger(const TYPE* src, const TYPE* end, unsigned int flags)
{
  if (flags & Integer::FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) {
      ++src; // eat space
    }
  }
  
  if (src == end) {
    throw InvalidFormat(Type::getType<Integer>());
  }
  
  BASSERT(sizeof(int) > sizeof(long long));
  long long value = 0;
  if ((flags & Integer::FLAG_ALLOW_SIGN) == 0) {
    unsigned int temp = 0;
    while (src != end) {
      const auto ch = *src++;
      if (!UnicodeTraits::isDigit(ch)) {
        break;
      }
      if (temp > PrimitiveTraits<unsigned int>::MAXIMUM/10) {
        throw InvalidFormat(Type::getType<Integer>());
      }
      temp *= 10;
      const unsigned int value = ASCIITraits::digitToValue(ch);
      if (temp > (PrimitiveTraits<unsigned int>::MAXIMUM - value)) {
        throw InvalidFormat(Type::getType<Integer>());
      }
      temp += value;
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

    value = 0; // overflow not possible
    if (negative) {
      while (src != end) {
        const auto ch = *src++;
        if (!UnicodeTraits::isDigit(ch)) {
          break;
        }
        value = value * 10 - ASCIITraits::digitToValue(ch);
        if (value < Integer::MINIMUM) {
          throw InvalidFormat(Type::getType<Integer>());
        }
      }
    } else {
      while (src != end) {
        const auto ch = *src++;
        if (!UnicodeTraits::isDigit(ch)) {
          break;
        }
        value = value * 10 + ASCIITraits::digitToValue(ch);
        if (value > Integer::MAXIMUM) {
          throw InvalidFormat(Type::getType<Integer>());
        }
      }
    }
  }
  
  if (flags & Integer::FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) { // rest must be spaces
      ++src;
    }
  }
  
  if (src != end) {
    throw InvalidFormat(Type::getType<Integer>());
  }
  return static_cast<int>(value);
}

int Integer::parse(const char* src, const char* end, unsigned int flags) throw(InvalidFormat)
{
  return parseInteger(src, end, flags);
}

int Integer::parse(const wchar* src, const wchar* end, unsigned int flags) throw(InvalidFormat)
{
  return parseInteger(src, end, flags);
}

int Integer::parse(const ucs4* src, const ucs4* end, unsigned int flags) throw(InvalidFormat)
{
  return parseInteger(src, end, flags);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
