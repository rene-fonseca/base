/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/InetEndPoint.h>
#include <base/Integer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

InetEndPoint::InetEndPoint() throw() : port(0) {
}

InetEndPoint::InetEndPoint(const InetAddress& _address, unsigned short _port) throw()
  : address(_address), port(_port) {
}

InetEndPoint::InetEndPoint(const InetAddress& _address, const InetService& service) throw()
  : address(_address), port(service.getPort()) {
}

InetEndPoint::InetEndPoint(const InetAddress& _address, const String& service) throw(ServiceNotFound)
  : address(_address), port(0) {
  try {
    Integer integer(service);
    if ((integer < 0) || (integer > 0xffff)) {
      throw ServiceNotFound("Port is out of range", this);
    }
    port = integer;
  } catch (InvalidFormat& e) {
    port = InetService(service).getPort();
  }
}

InetEndPoint::InetEndPoint(const String& host, const String& service) throw(ServiceNotFound)
  : address(host), port(0) {
  try {
    Integer integer(service);
    if ((integer < 0) || (integer > 0xffff)) {
      throw ServiceNotFound("Port is out of range", this);
    }
    port = integer;
  } catch (InvalidFormat& e) {
    port = InetService(service).getPort();
  }
}

InetEndPoint::InetEndPoint(const InetEndPoint& copy) throw()
  : address(copy.address), port(copy.port) {
}

InetEndPoint& InetEndPoint::operator=(const InetEndPoint& eq) throw() {
  if (&eq != this) { // protect against self assignment
    address = eq.address;
    port = eq.port;
  }
  return *this;
}

InetService InetEndPoint::getService() const throw() {
  return InetService(port);
}

void InetEndPoint::setAddress(const InetAddress& value) throw() {
  address = value;
}

void InetEndPoint::setPort(unsigned short value) throw() {
  port = value;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetEndPoint& value) throw(IOException) {
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
