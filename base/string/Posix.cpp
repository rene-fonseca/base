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
  
void Posix::MemoryStream::setLocale(const std::locale& _Loc) {
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

#if 0
bool Posix::toDouble(const std::string& text, double& _value)
{
  if (text.empty()) {
    return false;
  }
  // TAG: return parseDoubleSingleton(text.c_str(), text.c_str() + text.size());
  
  double value = 0;
  std::stringstream ss(text);
  ss.imbue(std::locale::classic()); // posix
  ss >> d;
  if (!ss.eof()) {
    return false;
  }
  _value = value;
  return true;
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
