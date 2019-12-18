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
#include <base/UnitTest.h>
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

  template<typename TYPE>
  inline int convertImpl(const char* src, const char* end, TYPE& _value) noexcept
  {
    // ok that we only advance 1 word for wchar at a time - surrogates not accepted anyway
    // -1 invalid, 1 valid, 0 not handled
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

    if (src != end) {
      if ((*src == 'i') || (*src == 'I')) {
        ++src;
        if ((src != end) && ((*src == 'n') || (*src == 'N'))) {
          ++src;
          if ((src != end) && ((*src == 'f') || (*src == 'F'))) {
            ++src;
            if (src == end) {
              _value = negative ? -Math::getInfinity<TYPE>() : Math::getInfinity<TYPE>();
              return 1;
            }
            if ((*src == 'i') || (*src == 'I')) {
              ++src;
              if ((src != end) && ((*src == 'n') || (*src == 'N'))) {
                ++src;
                if ((src != end) && ((*src == 'i') || (*src == 'I'))) {
                  ++src;
                  if ((src != end) && ((*src == 't') || (*src == 'T'))) {
                    ++src;
                    if ((src != end) && ((*src == 'y') || (*src == 'Y'))) {
                      ++src;
                      if (src != end) {
                        return -1;
                      }
                      _value = negative ? -Math::getInfinity<TYPE>() : Math::getInfinity<TYPE>();
                      return 1;
                    }
                  }
                }
              }
            }
          }
        }
        return -1; // invalid
      }

      if ((*src == 'n') || (*src == 'N')) {
        ++src;
        if ((src != end) && ((*src == 'a') || (*src == 'A'))) {
          ++src;
          if ((src != end) && ((*src == 'n') || (*src == 'N'))) {
            ++src;
            if (src == end) {
              _value = negative ? -Math::getNaN<TYPE>() : Math::getNaN<TYPE>();
              return 1;
            }
          }
        }
        return -1; // invalid
      }
    }

    bool skipped = false;
    bool negativeExponent = false;
    int exponent = 0;
    bool gotExponent = false;

    constexpr unsigned int MAXIMUM_DIGITS = 19;
    constexpr unsigned int MAXIMUM_EXPONENT_DIGITS = 9;
    int64 temp = 0;
    unsigned int i = 0;
    unsigned int j = 0;
    while ((src != end) && (i < MAXIMUM_DIGITS) && ASCIITraits::isDigit(*src)) {
      temp = (temp * 10) + ASCIITraits::digitToValue(*src++);
      ++i;
    }

    // TAG: switch to LargeInteger - we can safely discard if most than worst case digits for given type
    // we still validate format - even if we do not convert to value now
    while ((src != end) && ASCIITraits::isDigit(*src)) {
      skipped = true;
      ++src;
      ++i;
    }

    if (src != end) {
      // read decimals
      if (*src == '.') {
        ++src;
        while ((src != end) && (i < MAXIMUM_DIGITS) && ASCIITraits::isDigit(*src)) {
          temp = (temp * 10) + ASCIITraits::digitToValue(*src++);
          ++i;
          ++j;
        }

        // we still validate format - even if we do not convert to value now
        while ((src != end) && ASCIITraits::isDigit(*src)) {
          skipped = true;
          ++src;
          ++i;
          ++j;
        }
      }
    }
    
    if ((src != end) && (i > 0)) {
      // read exponent
      if ((*src == 'e') || (*src == 'E')) { // read exponent
        ++src;
        gotExponent = true;
        if (src == end) {
          return -1; // missing exponent
        }
        if (*src == '-') {
          negativeExponent = true;
          ++src;
        } else if (*src == '+') {
          ++src;
        }
        
        int k = 0;
        if (temp != 0) {
          bool gotExponentDigit = false;
          while ((src != end) && (*src == '0')) { // read 0 prefixes
            gotExponentDigit = true;
            ++src;
          }
          if (gotExponentDigit && (src == end)) {
            k = 1;
          }
          while ((src != end) && (k < MAXIMUM_EXPONENT_DIGITS) && ASCIITraits::isDigit(*src)) {
            exponent = (exponent * 10) + ASCIITraits::digitToValue(*src++);
            ++k;
          }

          // we still validate format - even if we do not convert to value now
          while ((src != end) && ASCIITraits::isDigit(*src)) {
            skipped = true;
            ++src;
            ++k;
          }
        } else { // just skip any digits since value is always going to be 0
          while ((src != end) && ASCIITraits::isDigit(*src)) {
            ++src;
            ++k;
          }
        }
        
        if (k == 0) {
          return -1; // missing exponent
        }
      }
    }
    
    if (src != end) {
      return -1; // not everything read
    }
    
    if (i == 0) {
      return -1; // no digit read
    }
    
    if (!skipped) { // read all and got at least 1 digit
      if (temp == 0) { // exponent is irrelevant
        return Math::getZero<TYPE>(negative); // preserve -0
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
      BASSERT(exponent >= 0);

      // TAG: make sure arch support 64-bit arithmetic - otherwise we need to fallback to 32-bit
      if (static_cast<MemorySize>(exponent) < getArraySize(Math::EXPONENTS10_64)) {
        if (negative) {
          temp = -temp;
        }
        TYPE value = static_cast<TYPE>(temp);
        if (exponent > 0) {
          if (negativeExponent) {
            value /= Math::EXPONENTS10_64[exponent];
          } else {
            value *= Math::EXPONENTS10_64[exponent];
          }
        }
        _value = value;
        return 1;
      } else if (static_cast<MemorySize>(exponent) < getArraySize(Math::EXPONENTS5_64)) { // exponent 10 => 2*5
        if (negative) {
          temp = -temp;
        }
        TYPE value = static_cast<TYPE>(temp);
        if (exponent > 0) {
          if (negativeExponent) {
            value /= Math::EXPONENTS5_64[exponent]; // no exponent overflow for 64 bit int
            value /= Math::EXPONENTS2_64[exponent]; // power 2 math is exact
            // f.setBase2Exponent(f.getBase2Exponent() - Math::EXPONENTS2_64[exponent]);
            // TAG: need to update exponent directly to guarantee exact
          } else {
            value *= Math::EXPONENTS5_64[exponent]; // no exponent overflow for 64 bit int
            value *= Math::EXPONENTS2_64[exponent]; // power 2 math is exact
            // TAG: need to update exponent directly to guarantee exact
          }
        }
        _value = value;
        return 1;
      }
    }
    
    // TAG: handle exponent overflow - use max/min/inf?
    
    BASSERT(skipped);
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

  MemoryStreamImpl& ms = *(this->ms);
  float value = 0;
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms || !ms.eof()) {
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

  MemoryStreamImpl& ms = *(this->ms);
  double value = 0;
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms || !ms.eof()) {
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

  MemoryStreamImpl& ms = *(this->ms);
  long double value = 0;
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms || !ms.eof()) {
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
  if (!ms || !ms.eof()) {
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
  if (!ms || !ms.eof()) {
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
  if (!ms || !ms.eof()) {
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

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Posix) : public UnitTest {
public:

  TEST_PRIORITY(20);
  TEST_PROJECT("base/string");
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Posix posix;
    double d = 0;
    TEST_ASSERT(posix.toDouble(String("0"), d));
    TEST_ASSERT(posix.toDouble(String(".0"), d));
    TEST_ASSERT(posix.toDouble(String("0."), d));

    TEST_ASSERT(posix.toDouble(String("123"), d));
    TEST_ASSERT(posix.toDouble(String("123."), d));
    TEST_ASSERT(posix.toDouble(String("123.e1"), d));
    TEST_ASSERT(posix.toDouble(String("123.e+1"), d));
    TEST_ASSERT(posix.toDouble(String("123.e-1"), d));

    TEST_ASSERT(posix.toDouble(String("-0"), d));
    TEST_ASSERT(posix.toDouble(String("-123"), d));
    TEST_ASSERT(posix.toDouble(String("-123."), d));
    TEST_ASSERT(posix.toDouble(String("-123.e1"), d));
    TEST_ASSERT(posix.toDouble(String("-123.e+1"), d));
    TEST_ASSERT(posix.toDouble(String("-123.e-1"), d));

    TEST_ASSERT(posix.toDouble(String("+0"), d));
    TEST_ASSERT(posix.toDouble(String("+123"), d));
    TEST_ASSERT(posix.toDouble(String("+123."), d));
    TEST_ASSERT(posix.toDouble(String("+123.e1"), d));
    TEST_ASSERT(posix.toDouble(String("+123.e+1"), d));
    TEST_ASSERT(posix.toDouble(String("+123.e-1"), d));

    TEST_ASSERT(posix.toDouble(String("123.E1"), d));
    TEST_ASSERT(posix.toDouble(String("123.E+1"), d));
    TEST_ASSERT(posix.toDouble(String("123.E-1"), d));

    TEST_ASSERT(!posix.toDouble(String("12 3"), d));
    TEST_ASSERT(!posix.toDouble(String("123e-1 "), d));
    TEST_ASSERT(!posix.toDouble(String(" 123e-1"), d));
    TEST_ASSERT(!posix.toDouble(String("123e-1e1"), d));
    TEST_ASSERT(!posix.toDouble(String("123e-1 1"), d));
    TEST_ASSERT(!posix.toDouble(String(""), d));
    TEST_ASSERT(!posix.toDouble(String("."), d));
    TEST_ASSERT(!posix.toDouble(String(".0."), d));

    TEST_ASSERT(posix.toDouble(String("inf"), d));
    TEST_ASSERT(posix.toDouble(String("infinity"), d));
    TEST_ASSERT(posix.toDouble(String("+inf"), d));
    TEST_ASSERT(posix.toDouble(String("+infinity"), d));
    TEST_ASSERT(posix.toDouble(String("-inf"), d));
    TEST_ASSERT(posix.toDouble(String("-infinity"), d));
    TEST_ASSERT(!posix.toDouble(String("-infi"), d));
    TEST_ASSERT(!posix.toDouble(String("infinity "), d));
    TEST_ASSERT(posix.toDouble(String("nan"), d));
    TEST_ASSERT(posix.toDouble(String("NAN"), d));
    TEST_ASSERT(posix.toDouble(String("+nan"), d));
    TEST_ASSERT(posix.toDouble(String("+NAN"), d));
    TEST_ASSERT(posix.toDouble(String("-nan"), d));
    TEST_ASSERT(posix.toDouble(String("-NAN"), d));
  }
};

TEST_REGISTER(Posix);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
