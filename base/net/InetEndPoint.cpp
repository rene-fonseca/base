/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/InetEndPoint.h>
#include <base/Integer.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

InetEndPoint::InetEndPoint() noexcept
{
}

InetEndPoint::InetEndPoint(
  const InetAddress& _address, unsigned short _port) noexcept
  : address(_address), port(_port)
{
}

InetEndPoint::InetEndPoint(
  const InetAddress& _address, const InetService& service) noexcept
  : address(_address), port(service.getPort())
{
}

InetEndPoint::InetEndPoint(
  const InetAddress& _address, const String& service) throw(ServiceNotFound)
  : address(_address)
{
  try {
    Integer integer(service);
    if ((integer < 0) || (integer > 0xffff)) {
      throw ServiceNotFound("Port is out of range", this);
    }
    port = integer;
  } catch (InvalidFormat&) {
    port = InetService(service).getPort();
  }
}

InetEndPoint::InetEndPoint(
  const String& host, const String& service) throw(ServiceNotFound)
  : address(host)
{
  try {
    Integer integer(service);
    if ((integer < 0) || (integer > 0xffff)) {
      throw ServiceNotFound("Port is out of range", this);
    }
    port = integer;
  } catch (InvalidFormat&) {
    port = InetService(service).getPort();
  }
}

InetEndPoint::InetEndPoint(const InetEndPoint& copy) noexcept
  : address(copy.address), port(copy.port)
{
}

InetEndPoint& InetEndPoint::operator=(const InetEndPoint& assign) noexcept
{
  if (&assign != this) { // protect against self assignment
    address = assign.address;
    port = assign.port;
  }
  return *this;
}

InetService InetEndPoint::getService() const noexcept
{
  return InetService(port);
}

void InetEndPoint::setAddress(const InetAddress& value) noexcept
{
  address = value;
}

void InetEndPoint::setPort(unsigned short value) noexcept
{
  port = value;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const InetEndPoint& value)
{
  FormatOutputStream::PushContext push(stream);
  const InetAddress::Family family = value.getAddress().getFamily();
  if (family == InetAddress::IP_VERSION_6) {
    stream << '[';
  }
  stream << value.getAddress();
  if (family == InetAddress::IP_VERSION_6) {
    stream << ']';
  }
  stream << ':' << value.getPort();
  return stream;
}

#if 0 && defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(InetAddress) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/net");
  TEST_IMPACT(PRIVACY);

  void run() override
  {
    InetEndPoint e1("127.0.0.1:80");
    InetEndPoint e2("127.0.0.1:9999");
  }
};

TEST_REGISTER(InetAddress);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
