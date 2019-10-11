/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/net/InetAddress.h>
#include <base/net/InetService.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Internet end point (i.e. both internet address and port number).

  @short Internet end point.
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API InetEndPoint : public virtual Object {
private:

  /** Host. */
  InetAddress address;
  /** Port. */
  unsigned short port = 0;
public:

  /**
    Initializes default end point.
  */
  InetEndPoint() throw();

  /**
    Initializes the end point from the specified address and port.

    @param address The address.
    @param port The port.
  */
  InetEndPoint(const InetAddress& address, unsigned short port) throw();

  /**
    Initializes the end point from the specified address and service.

    @param address The address.
    @param service The service.
  */
  InetEndPoint(const InetAddress& address, const InetService& service) throw();

  /**
    Initializes the end point from the specified address and service.

    @param address The address.
    @param service The service (e.g. 'http' or '80').
  */
  InetEndPoint(
    const InetAddress& address, const String& service) throw(ServiceNotFound);

  /**
    Initializes end point from string representations of host and service.

    @param host The host (e.g. '127.0.0.1' or 'www.google.com').
    @param service The service (e.g. 'http' or '80').
  */
  InetEndPoint(
    const String& host, const String& service) throw(ServiceNotFound);

  /**
    Initialize object from other object.
  */
  InetEndPoint(const InetEndPoint& copy) throw();

  /**
    Default assignment.
  */
  InetEndPoint& operator=(const InetEndPoint& eq) throw();

  /**
    Returns true if the end point are equal.
  */
  inline bool operator==(const InetEndPoint& eq) const throw() {
    return (port == eq.port) && (address == eq.address);
  }
  
  /**
    Returns true if the end point are non-equal.
  */
  inline bool operator!=(const InetEndPoint& eq) const throw() {
    return (port != eq.port) || (address != eq.address);
  }
  
  /**
    Returns the address.
  */
  inline const InetAddress& getAddress() const throw() {
    return address;
  }

  /**
    Returns the port.
  */
  inline unsigned short getPort() const throw() {
    return port;
  }

  /**
    Returns the port converted to service.
  */
  InetService getService() const throw();

  /**
    Sets the address.
  */
  void setAddress(const InetAddress& value) throw();

  /**
    Sets the port.
  */
  void setPort(unsigned short value) throw();
};

/**
  Writes a string representation of the InetEndPoint object to a format stream
  (e.g. "127.0.0.1:1234" or "[::1]:1234").
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
  FormatOutputStream& stream, const InetEndPoint& value) throw(IOException);

template<>
class Hash<InetEndPoint> {
public:
  
  inline unsigned long operator()(const InetEndPoint& value) throw() {
    return Hash<InetAddress>()(value.getAddress()) * 31 + value.getPort();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
