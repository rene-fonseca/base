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
  if (src == end) {
    return false;
  }
  float value = 0; // TAG: NAN!
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  /*
  if (NAN) {
    return false;
  }
  */
  _value = value;
  return true;
}

bool Posix::getSeries(const char* src, const char* end, double& _value)
{
  if (src == end) {
    return false;
  }
  double value = 0; // TAG: NAN!
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  /*
  if (NAN) {
    return false;
  }
  */
  _value = value;
  return true;
}

bool Posix::getSeries(const char* src, const char* end, long double& _value)
{
  if (src == end) {
    return false;
  }
  long double value = 0; // TAG: NAN!
  ms.reset(src, end);
  ms.clear();
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  /*
  if (NAN) {
    return false;
  }
  */
  _value = value;
  return true;
}

#if 1
bool Posix::toDouble(const char* src, const char* end, double& _value)
{
  if (src == end) {
    return false;
  }
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
#else
bool Posix::toDouble(const char* _src, const char* end, double& _value)
{
  const char* src = _src;
  if (src == end) {
    return false;
  }

  // optimize float to string also

  bool sign = false;
  if (*src == '-') {
    sign = true;
    ++src;
  }
  int exponent = 0;
  bool exponentSign = false;

  int64 temp = 0;
  unsigned int i = 0;
  unsigned int j = 0;
  while ((src != end) && (i < 19) && ASCIITraits::isDigit(*src)) {
    temp = (temp * 10) + ASCIITraits::digitToValue(*src++);
    ++i;
  }
  if (src != end) {
    if (*src == '.') {
      ++src;
      while ((src != end) && (i < 19) && ASCIITraits::isDigit(*src)) {
        temp = (temp * 10) + ASCIITraits::digitToValue(*src++);
        ++i;
        ++j;
      }
    }
    if (src != end) {
      if (*src == 'e') {
        ++src;
        if (src != end) {
          if (*src == '-') {
            exponentSign = true;
            ++src;
          }
          int k = 0;
          while ((src != end) && (k < 9) && ASCIITraits::isDigit(*src)) {
            exponent = (exponent * 10) + ASCIITraits::digitToValue(*src++);
            ++k;
          }
        }
      }
    }
  }

  j -= exponent;
  // TAG: use large integer

  if ((src == end) && (i > 0)) {
    static const int64 EXPONENTS[] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,
      100000000000LL,1000000000000LL,10000000000000LL,100000000000000LL,1000000000000000LL,
      10000000000000000LL,100000000000000000LL,1000000000000000000LL};
    if (sign) {
      temp = -temp;
    }
    if (j < getArraySize(EXPONENTS)) {
      _value = static_cast<double>(temp);
      _value /= EXPONENTS[j];
      return true;
    }
  }

  // TAG: handle nan, and -inf, inf

  double value = 0;
  MemoryStream ms(_src, end);
  ms.setLocale(std::locale::classic()); // c locale
  ms >> value;
  if (!ms.eof()) {
    return false;
  }
  _value = value;
  return true;
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
