/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/net/InetEndPoint.h>
#include <base/Integer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

InetEndPoint::InetEndPoint() throw() : port(0) {
}

InetEndPoint::InetEndPoint(const InetAddress& address, unsigned short port) throw() :
  address(address), port(port) {
}

InetEndPoint::InetEndPoint(const InetAddress& address, const InetService& service) throw() :
  address(address), port(service.getPort()) {
}

InetEndPoint::InetEndPoint(const InetAddress& address, const String& service) throw(ServiceNotFound) :
  address(address), port(0) {
  try {
    Integer integer(service);
    if ((integer < 0) || (integer > 0xffff)) {
      throw ServiceNotFound("Port is out of range");
    }
    port = integer;
  } catch(InvalidFormat& e) {
    port = InetService(service).getPort();
  }
}

InetEndPoint::InetEndPoint(const String& host, const String& service) throw(ServiceNotFound) :
  address(host), port(0) {
  try {
    Integer integer(service);
    if ((integer < 0) || (integer > 0xffff)) {
      throw ServiceNotFound("Port is out of range");
    }
    port = integer;
  } catch(InvalidFormat& e) {
    port = InetService(service).getPort();
  }
}

InetEndPoint::InetEndPoint(const InetEndPoint& copy) throw() : address(copy.address), port(copy.port) {
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

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetEndPoint& value) {
  return stream << value.getAddress() << ":" << value.getPort();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
