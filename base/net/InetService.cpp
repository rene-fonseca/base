/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/net/InetService.h>
#include <base/ByteOrder.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <winsock2.h>
#else // unix
#  include <netdb.h>
#  include <netinet/in.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

unsigned short InetService::getByName(
  const String& name, const String& protocol) throw() {
  struct servent* sp = nullptr;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  sp = getservbyname(name.getElements(), protocol.getElements()); // MT-safe
#elif ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65) || \
       (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS))
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyname_r(name.getElements(), protocol.getElements(), &result, buffer, sizeof(buffer));
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyname_r(name.getElements(), protocol.getElements(), &result, buffer, sizeof(buffer), &sp);
#else
//  #warning Using MT-unsafe getservbyname
  sp = getservbyname(name.getElements(), protocol.getElements());
#endif
  return sp ? ByteOrder::fromBigEndian<unsigned short>(sp->s_port) : 0;
}

String InetService::getByPort(
  unsigned short port, const String& protocol) throw() {
  struct servent* sp = nullptr;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  sp = getservbyport(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements()); // MT-safe
#elif ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65) || \
       (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS))
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyport_r(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements(), &result, buffer, sizeof(buffer));
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyport_r(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements(), &result, buffer, sizeof(buffer), &sp);
#else
//  #warning Using MT-unsafe getservbyport
  sp = getservbyport(ByteOrder::toBigEndian<unsigned short>(port), protocol.getElements());
#endif
  return sp ? String(sp->s_name) : String();
}

InetService::InetService(
  const String& name, const String& protocol) throw(ServiceNotFound) {
  port = getByName(name, protocol);
  bassert(
    port != 0,
    ServiceNotFound("Unable to resolve service by name", this)
  );
  this->name = name;
  this->protocol = protocol;
}

InetService::InetService(
  unsigned short port, const String& protocol) throw(ServiceNotFound) {
  name = getByPort(port, protocol);
  bassert(
    name.isProper(),
    ServiceNotFound("Unable to resolve service by port", this)
  );
  this->port = port;
  this->protocol = protocol;
}

InetService::InetService(const InetService& copy) throw() :
  name(copy.name), port(copy.port), protocol(copy.protocol) {
}

InetService& InetService::operator=(const InetService& eq) throw() {
  if (&eq != this) { // protect against self assignment
    name = eq.name;
    port = eq.port;
    protocol = eq.protocol;
  }
  return *this;
}

const String& InetService::getName() const throw() {
  return name;
}

unsigned short InetService::getPort() const throw() {
  return port;
}

const String& InetService::getProtocol() const throw() {
  return protocol;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const InetService& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  return stream << value.name << ' ' << value.port << '/' << value.protocol;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
