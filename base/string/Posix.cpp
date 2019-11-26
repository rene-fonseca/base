/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/Posix.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  const int64 EXPONENTS[] = { // up to 10^18
    1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,
    10000000000LL, // requires 64-bit from here
    100000000000LL,
    1000000000000LL,
    10000000000000LL,
    100000000000000LL,
    1000000000000000LL,
    10000000000000000LL,
    100000000000000000LL,
    1000000000000000000LL
  };

  template<typename TYPE>
  inline int convertImpl(const char* src, const char* end, TYPE& _value) noexcept
  {
    if (src == end) {
      return -1;
    }

    bool negative = false;
    if (*src == '-') {
      negative = true;
      ++src;
    } else if (*src == '+') {
      ++src;
    }

    bool negativeExponent = false;
    int exponent = 0;

    constexpr unsigned int MAXIMUM_DIGITS = 19;
    int64 temp = 0;
    unsigned int i = 0;
    unsigned int j = 0;
    while ((src != end) && (i < MAXIMUM_DIGITS) && ASCIITraits::isDigit(*src)) {
      temp = (temp * 10) + ASCIITraits::digitToValue(*src++);
      ++i;
    }

    if (src != end) {
      if (*src == '.') {
        ++src;
        while ((src != end) && (i < MAXIMUM_DIGITS) && ASCIITraits::isDigit(*src)) {
          temp = (temp * 10) + ASCIITraits::digitToValue(*src++);
          ++i;
          ++j;
        }
      }
      
      if ((src != end) && (i > 0)) {
        if ((*src == 'e') || (*src == 'E')) { // read exponent
          ++src;
          if (src == end) {
            return -1; // missing exponent
          }
          if (src != end) {
            if (*src == '-') {
              negativeExponent = true;
              ++src;
            } else if (*src == '+') {
              ++src;
            }
            int k = 0;
            while ((src != end) && (k < 9) && ASCIITraits::isDigit(*src)) {
              exponent = (exponent * 10) + ASCIITraits::digitToValue(*src++);
              ++k;
            }
            if (k == 0) {
              return -1; // missing exponent
            }
          }
        }
      }
    }
    
    if ((src == end) && (i > 0)) { // read all and got at least 1 digit
      j -= exponent;
      if (j < getArraySize(EXPONENTS)) {
        if (negative) {
          temp = -temp;
        }
        TYPE value = static_cast<TYPE>(temp);
        if (j > 0) {
          if (negativeExponent) {
           value /= EXPONENTS[j];
          } else {
            value *= EXPONENTS[j];
          }
        }
        _value = value;
        return 1;
      }
    }
    
#if 0
    if ((src != end) && (i == 0)) { // only sign read
      if ((*src == 'i') || (*src == 'I')) {
        ++src;
        if ((*src == 'n') || (*src == 'N')) {
          ++src;
          if ((*src == 'f') || (*src == 'F')) {
            ++src;
          }
          if (src == end) {
          }
        }
      }
    }
#endif
    
    return 0;
  }
}

Posix::MemoryBuffer::MemoryBuffer(const char* src, const char* end)
{
  reset(src, end);
}

void Posix::MemoryBuffer::reset(const char* src, const char* end)
{
  char* _src(const_cast<char*>(src));
  setg(_src, _src, _src + (end - src));
}

Posix::MemoryStream::MemoryStream(const char* src, const char* end)
  : MemoryBuffer(src, end),
    std::istream(static_cast<std::streambuf*>(this))
{
}
  
void Posix::MemoryStream::setLocale(const std::locale& _Loc)
{
  std::locale _Oldlocale = ios_base::imbue(_Loc);
  const auto _Rdbuf = rdbuf();
  if (_Rdbuf) {
    _Rdbuf->pubimbue(_Loc);
  }
}

Posix::Posix()
{
  ms.setLocale(std::locale::classic()); // c locale
}

bool Posix::getSeries(const char* src, const char* end, float& _value)
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }

  float value = 0;
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::getSeries(const char* src, const char* end, double& _value)
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }

  double value = 0;
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::getSeries(const char* src, const char* end, long double& _value)
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }

  long double value = 0;
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::toFloat(const char* src, const char* end, float& _value)
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }
  
  float value = 0;
  MemoryStream ms(src, end);
  ms.setLocale(std::locale::classic()); // c locale
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::toDouble(const char* src, const char* end, double& _value)
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }
  
  // TAG: allow separate keywords for nan,snan,-inf,inf
  // TAG: handle +-nan, and +-inf, +-infinity, +-nan.* // ignore case
  // TAG: handle [+-]?0[xX]([0-9a-fA-F]+)?(.?[0-9a-fA-F]+)([pP][+-]?[0-9a-fA-F]+)?

  double value = 0;
  MemoryStream ms(src, end);
  ms.setLocale(std::locale::classic()); // c locale
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::toLongDouble(const char* src, const char* end, long double& _value)
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }
  
  long double value = 0;
  MemoryStream ms(src, end);
  ms.setLocale(std::locale::classic()); // c locale
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
