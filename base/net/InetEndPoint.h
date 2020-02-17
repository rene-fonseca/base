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

  /** Returns the port for the given service. */
  static unsigned short getPort(const String& service);

  /** Returns the end point for the given string. */
  static InetEndPoint getEndPoint(const String& endpoint);
  
  /**
    Initializes default end point.
  */
  InetEndPoint() noexcept;

  /**
    Initializes the end point from the specified address and port.

    @param address The address.
    @param port The port.
  */
  InetEndPoint(const InetAddress& address, unsigned short port) noexcept;

  /**
    Initializes the end point from the specified address and service.

    @param address The address.
    @param service The service.
  */
  InetEndPoint(const InetAddress& address, const InetService& service) noexcept;

  /**
    Initializes the end point from the specified address and service.

    @param address The address.
    @param service The service (e.g. 'http' or '80').
  */
  InetEndPoint(const InetAddress& address, const String& service);

  /**
    Initializes end point from string representations of host and service.

    @param host The host (e.g. '127.0.0.1' or 'www.google.com').
    @param port The port.
  */
  InetEndPoint(const String& host, unsigned short port);

  /**
    Initializes end point from string representations of host and service.

    @param host The host (e.g. '127.0.0.1' or 'www.google.com').
    @param service The service (e.g. 'http' or '80').
  */
  InetEndPoint(const String& host, const String& service);

  /**
    Initialize object from other object.
  */
  InetEndPoint(const InetEndPoint& copy) noexcept;

  /**
    Default assignment.
  */
  InetEndPoint& operator=(const InetEndPoint& assign) noexcept;

  /**
    Returns true if the end point are equal.
  */
  inline bool operator==(const InetEndPoint& compare) const noexcept
  {
    return (port == compare.port) && (address == compare.address);
  }
  
  /**
    Returns true if the end point are non-equal.
  */
  inline bool operator!=(const InetEndPoint& compare) const noexcept
  {
    return (port != compare.port) || (address != compare.address);
  }
  
  /**
    Returns the address.
  */
  inline const InetAddress& getAddress() const noexcept
  {
    return address;
  }

  /**
    Returns the port.
  */
  inline unsigned short getPort() const noexcept
  {
    return port;
  }

  /**
    Returns the port converted to service.
  */
  InetService getService() const noexcept;

  /**
    Sets the address.
  */
  void setAddress(const InetAddress& value) noexcept;

  /**
    Sets the port.
  */
  void setPort(unsigned short value) noexcept;
};

/**
  Writes a string representation of the InetEndPoint object to a format stream
  (e.g. "127.0.0.1:1234" or "[::1]:1234").
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
  FormatOutputStream& stream, const InetEndPoint& value);

template<>
class Hash<InetEndPoint> {
public:
  
  inline unsigned long operator()(const InetEndPoint& value) noexcept
  {
    return Hash<InetAddress>()(value.getAddress()) * 31 + value.getPort();
  }
};

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(InetEndPoint)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
