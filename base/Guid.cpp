/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Guid.h>
#include <base/string/StringOutputStream.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Guid::Guid() throw() {
  clear(guid);
}

Guid::Guid(const uint8 (&value)[16]) throw()
{
  copy<uint8>(guid, value, sizeof(guid));
}

namespace {

  uint8 getGuidByte(const uint8* src)
  {
    if (!ASCIITraits::isHexDigit(src[0]) || !ASCIITraits::isHexDigit(src[1])) {
      throw InvalidFormat("Invalid guid.");
    }
    return (ASCIITraits::digitToValue(src[0]) << 4) | ASCIITraits::digitToValue(src[1]);
  }
}

Guid Guid::getGuid(const char* text)
{
  Guid guid;
  // {B429A864-CDFC-4044-BCC1-BDDE18EAF8DE}
  if (NativeString(text).getLength() != 38) { // handles nullptr
    throw InvalidFormat("Invalid guid.");
  }
  if ((text[0] != '{') || (text[37] != '}') ||
      (text[9] != '-') || (text[14] != '-') || (text[19] != '-') || (text[26] != '-')) {
    throw InvalidFormat("Invalid guid.");
  }

  static const uint8 OFFSETS[16] = {1, 3, 5, 7, 10, 12, 15, 17, 20, 22, 25, 27, 29, 31, 33, 35};
  for (unsigned int i = 0; i < SIZE; ++i) {
    guid.guid[i] = getGuidByte(reinterpret_cast<const uint8*>(text) + OFFSETS[i]);
  }

  return guid;
}

Guid::Guid(const Guid& _copy) noexcept
{
  copy<uint8>(guid, _copy.guid, sizeof(guid));
}

Guid& Guid::operator=(const Guid& eq) noexcept
{
  if (&eq != this) {
    copy<uint8>(guid, eq.guid, sizeof(guid));
  }
  return *this;
}

bool Guid::operator==(const Guid& eq) const noexcept
{
  return compare<uint8>(guid, eq.guid, sizeof(guid)) == 0;
}

bool Guid::operator!=(const Guid& eq) const noexcept
{
  return compare<uint8>(guid, eq.guid, sizeof(guid)) != 0;
}

bool Guid::isNull() const noexcept
{
  for (MemorySize i = 0; i < sizeof(guid); ++i) {
    if (guid[i] != 0) {
      return false;
    }
  }
  return true;
}

String Guid::createGuidAsString()
{
  return createGuid().toString();
}

Guid Guid::createGuid() noexcept
{
  Guid result;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  GUID _guid;
  HRESULT status = CoCreateGuid(&_guid);
  if (status != S_OK) {
    return result;
  }
  ASSERT(sizeof(_guid) == sizeof(result.guid)); // for C++17 static_assert();
  copy<uint8>(result.guid, reinterpret_cast<const uint8*>(&_guid), sizeof(result.guid));
#else
  ASSERT(!"Not implemented.");
#endif
  return result;
}

String Guid::toString() const
{
  // {B429A864-CDFC-4044-BCC1-BDDE18EAF8DE}

  char result[38];
  char* dest = result;

  *dest++ = '{';
  *dest++ = ASCIITraits::valueToUpperDigit(guid[0] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[0] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[1] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[1] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[2] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[2] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[3] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[3] >> 0);
  *dest++ = '-';
  *dest++ = ASCIITraits::valueToUpperDigit(guid[4] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[4] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[5] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[5] >> 0);
  *dest++ = '-';
  *dest++ = ASCIITraits::valueToUpperDigit(guid[6] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[6] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[7] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[7] >> 0);
  *dest++ = '-';
  *dest++ = ASCIITraits::valueToUpperDigit(guid[8] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[8] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[9] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[9] >> 0);
  *dest++ = '-';
  *dest++ = ASCIITraits::valueToUpperDigit(guid[10] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[10] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[11] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[11] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[12] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[12] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[13] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[13] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[14] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[14] >> 0);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[15] >> 4);
  *dest++ = ASCIITraits::valueToUpperDigit(guid[15] >> 0);
  *dest++ = '}';
  ASSERT((dest - result) == SIZE);
  return String(result, SIZE);

#if 0
  StringOutputStream s;
  s << HEX << setWidth(2) << ZEROPAD << NOPREFIX
    << '{'
    << guid[0]
    << guid[1]
    << guid[2]
    << guid[3] << '-'
    << guid[4]
    << guid[5] << '-'
    << guid[6]
    << guid[7] << '-'
    << guid[8]
    << guid[9] << '-'
    << guid[10]
    << guid[11]
    << guid[12]
    << guid[13]
    << guid[14]
    << guid[15]
    << '}' << FLUSH;
  return s.getString();
#endif
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Guid& guid)
{
  return stream << guid.toString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
