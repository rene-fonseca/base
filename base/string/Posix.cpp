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
#include <sstream> // required until we have proper float to string implementation

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class MemoryBuffer : public std::streambuf {
public:

  MemoryBuffer(const char* src, const char* end);

  void reset(const char* src, const char* end);
};

class MemoryStreamImpl : virtual public MemoryBuffer, public std::istream {
public:

  MemoryStreamImpl(const char* src = nullptr, const char* end = nullptr);
  
  void setLocale(const std::locale& _Loc);
};

MemoryBuffer::MemoryBuffer(const char* src, const char* end)
{
  reset(src, end);
}

void MemoryBuffer::reset(const char* src, const char* end)
{
  char* _src(const_cast<char*>(src));
  setg(_src, _src, _src + (end - src));
}

MemoryStreamImpl::MemoryStreamImpl(const char* src, const char* end)
  : MemoryBuffer(src, end),
    std::istream(static_cast<std::streambuf*>(this))
{
}
  
void MemoryStreamImpl::setLocale(const std::locale& _Loc)
{
  std::locale _Oldlocale = ios_base::imbue(_Loc);
  const auto _Rdbuf = rdbuf();
  if (_Rdbuf) {
    _Rdbuf->pubimbue(_Loc);
  }
}

namespace {

  // TAG: for 32-bit arch we need to keep in int32
/*
  constexpr int64 power10(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (10LL * power10(exponent - 1)) : 1;
  }
*/

  constexpr int64 power5(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (5LL * power5(exponent - 1)) : 1;
  }

  constexpr int32 power2(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (static_cast<int32>(2) * power2(exponent - 1)) : 1;
  }

  const int64 EXPONENTS10[19] = { //  64 bit has room for 10^18 - excluding 1 bit for sign
    1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,
    10000000000LL, // requires 64-bit from here
    100000000000LL,
    1000000000000LL,
    10000000000000LL,
    100000000000000LL,
    1000000000000000LL,
    10000000000000000LL,
    100000000000000000LL,
    1000000000000000000LL // power10(18)
  };
  const int64 EXPONENTS5[28] = { // 64 bit has room for 5 ^ 27 - excluding 1 bit for sign
    power5(0),power5(1),power5(2),power5(3),power5(4),power5(5),power5(6),power5(7),power5(8),power5(9),
    power5(10),power5(11),power5(12),power5(13),power5(14),power5(15),power5(16),power5(17),power5(18),power5(19),
    power5(20),power5(21),power5(22),power5(23),power5(24),power5(25),power5(26),power5(27)
  };
  const int32 EXPONENTS2[28] = { // 32 bit has room for 2 ^ 27 - excluding 1 bit for sign
    power2(0),power2(1),power2(2),power2(3),power2(4),power2(5),power2(6),power2(7),power2(8),power2(9),
    power2(10),power2(11),power2(12),power2(13),power2(14),power2(15),power2(16),power2(17),power2(18),power2(19),
    power2(20),power2(21),power2(22),power2(23),power2(24),power2(25),power2(26),power2(27)
  };

  template<typename TYPE>
  constexpr TYPE getZero(bool negative) noexcept;

  template<>
  constexpr float getZero<float>(bool negative) noexcept
  {
    return negative ? -0.0f : 0.0f;
  }

  template<>
  constexpr double getZero<double>(bool negative) noexcept
  {
    return negative ? -0.0 : 0.0;
  }

  template<>
  constexpr long double getZero<long double>(bool negative) noexcept
  {
    return negative ? -0.0l : 0.0l;
  }

  template<typename TYPE>
  inline int convertImpl(const char* src, const char* end, TYPE& _value) noexcept
  {
    // ok that we only advance 1 word for wchar at a time - surrogates not accepted anyway
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
      if (temp == 0) { // exponent is irrelevant
        return getZero<TYPE>(negative); // preserve -0
      }
      if (negativeExponent) {
        exponent += j;
      } else {
        exponent -= j;
        if (exponent < 0) {
          exponent = -exponent;
          negativeExponent = true;
        }
      }

      if (exponent < getArraySize(EXPONENTS10)) {
        if (negative) {
          temp = -temp;
        }
        TYPE value = static_cast<TYPE>(temp);
        if (exponent > 0) {
          if (negativeExponent) {
            value /= EXPONENTS10[exponent];
          } else {
            value *= EXPONENTS10[exponent];
          }
        }
        _value = value;
        return 1;
      } else if (exponent < getArraySize(EXPONENTS5)) { // exponent 10 => 2*5
        if (negative) {
          temp = -temp;
        }
        TYPE value = static_cast<TYPE>(temp);
        if (exponent > 0) {
          if (negativeExponent) {
            value /= EXPONENTS5[exponent]; // no exponent overflow for 64 bit int
            value /= EXPONENTS2[exponent]; // power 2 math is exact
            // f.setBase2Exponent(f.getBase2Exponent() - EXPONENTS2[exponent]);
            // TAG: need to update exponent directly to guarantee exact
          } else {
            value *= EXPONENTS5[exponent]; // no exponent overflow for 64 bit int
            value *= EXPONENTS2[exponent]; // power 2 math is exact
            // TAG: need to update exponent directly to guarantee exact
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

Posix::Posix()
{
  ms = new MemoryStreamImpl();
  ms->setLocale(std::locale::classic()); // c locale
}

Posix::~Posix()
{
  delete ms;
}

bool Posix::getSeries(const char* src, const char* end, float& _value) noexcept
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }

  float value = 0;
  ms->reset(src, end);
  ms->clear();
  *ms >> value;
  if (!ms->eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::getSeries(const char* src, const char* end, double& _value) noexcept
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }

  double value = 0;
  ms->reset(src, end);
  ms->clear();
  *ms >> value;
  if (!ms->eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::getSeries(const char* src, const char* end, long double& _value) noexcept
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }

  long double value = 0;
  ms->reset(src, end);
  ms->clear();
  *ms >> value;
  if (!ms->eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::toFloat(const char* src, const char* end, float& _value) noexcept
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }
  
  float value = 0;
  MemoryStreamImpl ms(src, end);
  ms.setLocale(std::locale::classic()); // c locale
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::toDouble(const char* src, const char* end, double& _value) noexcept
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
  MemoryStreamImpl ms(src, end);
  ms.setLocale(std::locale::classic()); // c locale
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::toLongDouble(const char* src, const char* end, long double& _value) noexcept
{
  const int status = convertImpl(src, end, _value);
  if (status > 0) {
    return true;
  } else if (status < 0) {
    return false;
  }
  
  long double value = 0;
  MemoryStreamImpl ms(src, end);
  ms.setLocale(std::locale::classic()); // c locale
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}

bool Posix::toFloat(const wchar* src, const wchar* end, float& _value) noexcept
{
  PrimitiveStackArray<char> buffer(end - src);
  char* dest = buffer;
  while (src != end) {
    if (*src >= 0x7f) {
      return false; // not a float
    }
    *dest++ = *src++;
  }
  return toFloat(static_cast<const char*>(buffer), dest, _value);
}

bool Posix::toDouble(const wchar* src, const wchar* end, double& _value) noexcept
{
  PrimitiveStackArray<char> buffer(end - src);
  char* dest = buffer;
  while (src != end) {
    if (*src >= 0x7f) {
      return false; // not a float
    }
    *dest++ = *src++;
  }
  return toDouble(static_cast<const char*>(buffer), dest, _value);
}

bool Posix::toLongDouble(const wchar* src, const wchar* end, long double& _value) noexcept
{
  PrimitiveStackArray<char> buffer(end - src);
  char* dest = buffer;
  while (src != end) {
    if (*src >= 0x7f) {
      return false; // not a float
    }
    *dest++ = *src++;
  }
  return toLongDouble(static_cast<const char*>(buffer), dest, _value);
}

bool Posix::toFloat(const ucs4* src, const ucs4* end, float& _value) noexcept
{
  PrimitiveStackArray<char> buffer(end - src);
  char* dest = buffer;
  while (src != end) {
    if (*src >= 0x7f) {
      return false; // not a float
    }
    *dest++ = *src++;
  }
  return toFloat(static_cast<const char*>(buffer), dest, _value);
}

bool Posix::toDouble(const ucs4* src, const ucs4* end, double& _value) noexcept
{
  PrimitiveStackArray<char> buffer(end - src);
  char* dest = buffer;
  while (src != end) {
    if (*src >= 0x7f) {
      return false; // not a float
    }
    *dest++ = *src++;
  }
  return toDouble(static_cast<const char*>(buffer), dest, _value);
}

bool Posix::toLongDouble(const ucs4* src, const ucs4* end, long double& _value) noexcept
{
  PrimitiveStackArray<char> buffer(end - src);
  char* dest = buffer;
  while (src != end) {
    if (*src >= 0x7f) {
      return false; // not a float
    }
    *dest++ = *src++;
  }
  return toLongDouble(static_cast<const char*>(buffer), dest, _value);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
