/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/EUI64.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

EUI64::EUI64() noexcept
{
  id[0] = 0xff;
  id[1] = 0xff;
  id[2] = 0xff;
  id[3] = 0x00;
  id[4] = 0x00;
  id[5] = 0x00;
  id[6] = 0x00;
  id[7] = 0x00;
}

EUI64::EUI64(const uint8 value[8]) noexcept
{
  copy<uint8>(id, value, sizeof(id));
}

EUI64::EUI64(const String& value) {
  static const char SEPARATORS[8] = {'x', ':', ':', '-', ':', ':', ':', ':'}; // 'x' value is not used
  String::ReadIterator i = value.getBeginReadIterator();
  const String::ReadIterator end = value.getEndReadIterator();
  unsigned int index = 0;
  while (i < end) {
    char first = *i++;
    bassert(i < end, InvalidFormat(this));
    char second = *i++;
    bassert(ASCIITraits::isHexDigit(first) && ASCIITraits::isHexDigit(second), InvalidFormat(this));
    id[index++] = (ASCIITraits::digitToValue(first) << 4) | ASCIITraits::digitToValue(second);
    if (index < getArraySize(id)) {
      bassert(*i++ == SEPARATORS[index], InvalidFormat(this));
    }
  }
  bassert(i == end, InvalidFormat(this));
}

EUI64::EUI64(const EUI64& _copy) noexcept
{
  copy<uint8>(id, _copy.id, sizeof(id));
}

EUI64& EUI64::operator=(const EUI64& assign) noexcept
{
  if (&assign != this) {
    copy<uint8>(id, assign.id, sizeof(id));
  }
  return *this;
}

bool EUI64::operator==(const EUI64& _compare) const noexcept
{
  return compare<uint8>(id, _compare.id, sizeof(id)) == 0;
}

bool EUI64::operator!=(const EUI64& _compare) const noexcept
{
  return compare<uint8>(id, _compare.id, sizeof(id)) != 0;
}

bool EUI64::isInvalid() const noexcept
{
  return (id[0] == 0xff) && (id[1] == 0xff) && (id[2] == 0xff) &&
    (id[3] == 0) && (id[4] == 0) && (id[5] == 0) && (id[6] == 0) && (id[7] == 0);
}

unsigned int EUI64::getCompanyId() const noexcept
{
  return (((static_cast<unsigned int>(id[0]) << 8) | id[1]) << 8) | id[2];
}

void EUI64::setCompanyId(unsigned int companyId)
{
  if (!(companyId <= 0xffffff)) {
    throw OutOfDomain(this);
  }
  id[0] = companyId >> 16;
  id[1] = companyId >> 8;
  id[2] = companyId;
}

uint64 EUI64::getExtensionId() const noexcept {
  return (((((((static_cast<uint64>(id[3]) << 8) | id[4]) << 8) | id[5]) << 8) | id[6]) << 8) | id[7];
}

void EUI64::setExtensionId(uint64 extensionId)
{
  if (!(extensionId <= 0xffffffff)) {
    throw OutOfDomain(this);
  }
  id[3] = extensionId >> 32;
  id[4] = extensionId >> 24;
  id[5] = extensionId >> 16;
  id[6] = extensionId >> 8;
  id[7] = extensionId;
}

void EUI64::getEUI48(uint8* eui48) const noexcept
{
  eui48[0] = id[0];
  eui48[1] = id[1];
  eui48[2] = id[2];
  eui48[3] = id[5];
  eui48[4] = id[6];
  eui48[5] = id[7];
}

void EUI64::setEUI48(const uint8* eui48) noexcept
{
  id[0] = eui48[0];
  id[1] = eui48[1];
  id[2] = eui48[2];
  id[3] = 0xff;
  id[4] = 0xfe;
  id[5] = eui48[3];
  id[6] = eui48[4];
  id[7] = eui48[5];
}

void EUI64::getMAC48(uint8* mac) const noexcept
{
  mac[0] = swapNibbles(id[0]);
  mac[1] = swapNibbles(id[1]);
  mac[2] = swapNibbles(id[2]);
  mac[3] = swapNibbles(id[5]);
  mac[4] = swapNibbles(id[6]);
  mac[5] = swapNibbles(id[7]);
}

void EUI64::setMAC48(const uint8* mac) noexcept
{
  id[0] = swapNibbles(mac[0]);
  id[1] = swapNibbles(mac[1]);
  id[2] = swapNibbles(mac[2]);
  id[3] = 0xff;
  id[4] = 0xff;
  id[5] = swapNibbles(mac[3]);
  id[6] = swapNibbles(mac[4]);
  id[7] = swapNibbles(mac[5]);
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const EUI64& value)
{
  const uint8* id = value.getBytes();
  StringOutputStream s;
  s << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[0] << ':'
    << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[1] << ':'
    << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[2] << '-'
    << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[3] << ':'
    << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[4] << ':'
    << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[5] << ':'
    << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[6] << ':'
    << HEX << setWidth(2) << ZEROPAD << NOPREFIX << id[7] << FLUSH;
  return stream << s.getString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
