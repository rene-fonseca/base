/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/net/InetService.h>
#include <base/ByteOrder.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <winsock2.h>
#else // unix
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <netdb.h>
#endif
#  include <netinet/in.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned short InetService::getByName(const String& name, const String& protocol) noexcept
{
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
  struct servent* sp = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  sp = getservbyname(name.getElements(), protocol.getElements()); // MT-safe
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS))
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyname_r(name.getElements(), protocol.getElements(), &result, buffer, sizeof(buffer));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyname_r(name.getElements(), protocol.getElements(), &result, buffer, sizeof(buffer), &sp);
#else
//  #warning Using MT-unsafe getservbyname
  sp = getservbyname(name.getElements(), protocol.getElements());
#endif
  return sp ? ByteOrder::fromBigEndian<unsigned short>(sp->s_port) : 0;
#else
  BASSERT(!"Not supported.");
  return 0;
#endif
}

String InetService::getByPort(unsigned short port, const String& protocol) noexcept
{
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
  struct servent* sp = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  sp = getservbyport(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements()); // MT-safe
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS))
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyport_r(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements(), &result, buffer, sizeof(buffer));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyport_r(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements(), &result, buffer, sizeof(buffer), &sp);
#else
//  #warning Using MT-unsafe getservbyport
  sp = getservbyport(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements());
#endif
  return sp ? String(sp->s_name) : String();
#else
  BASSERT(!"Not supported.");
  return String();
#endif
}

InetService::InetService(
  const String& name, const String& protocol)
{
  port = getByName(name, protocol);
  bassert(
    port != 0,
    ServiceNotFound("Unable to resolve service by name.", this)
  );
  this->name = name;
  this->protocol = protocol;
}

InetService::InetService(
  unsigned short port, const String& protocol) {
  name = getByPort(port, protocol);
  bassert(
    name.isProper(),
    ServiceNotFound("Unable to resolve service by port.", this)
  );
  this->port = port;
  this->protocol = protocol;
}

InetService::InetService(const InetService& copy) noexcept
  : name(copy.name), port(copy.port), protocol(copy.protocol)
{
}

InetService& InetService::operator=(const InetService& assign) noexcept
{
  if (&assign != this) { // protect against self assignment
    name = assign.name;
    port = assign.port;
    protocol = assign.protocol;
  }
  return *this;
}

const String& InetService::getName() const noexcept
{
  return name;
}

unsigned short InetService::getPort() const noexcept
{
  return port;
}

const String& InetService::getProtocol() const noexcept
{
  return protocol;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetService& value)
{
  FormatOutputStream::PushContext push(stream);
  return stream << value.name << ' ' << value.port << '/' << value.protocol;
}

#if 0
#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(InetService) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/net");
  TEST_IMPACT(PRIVACY);

  void run() override
  {
    InetService service1();
  }
};

TEST_REGISTER(InetService);

#endif
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
