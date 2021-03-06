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
#include <base/string/String.h>
#include <base/string/FormatOutputStream.h>
#include <base/net/ServiceNotFound.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class represents an internet service.

  @short Internet Protocol service.
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API InetService : public virtual Object {
private:

  /** Specifies the official name of the service. */
  String name;
  /** Specifies the port of the service (in host byte order). */
  unsigned short port = 0;
  /** Specifies the name of the protocol of the service. */
  String protocol;
public:

  /**
    Returns the port number of the service given by the service name and protocol.

    @param name Specifies the name of the service.
    @param protocol Specifies the name of the protocol. Default is "tcp".

    @return 0 if the service was not found.
  */
  static unsigned short getByName(
    const String& name, const String& protocol) noexcept;

  /**
    Returns the name of the service given by the specified port and protocol.

    @param port Specifies the port number.
    @param protocol Specifies the name of the protocol. Default is "tcp".

    @return Empty string if service was not found.
  */
  static String getByPort(unsigned short port, const String& protocol) noexcept;

  /**
    Initializes a service object by specified service name and protocol.

    @param name Specifies the name of the service.
    @param protocol Specifies the name of the protocol. Default is "tcp".
  */
  explicit InetService(
    const String& name,
    const String& protocol = Literal("tcp"));

  /**
    Initializes a service object by specified port number and protocol.

    @param port The port number.
    @param protocol Specifies the name of the protocol. Default is "tcp".
  */
  explicit InetService(
    unsigned short port,
    const String& protocol = Literal("tcp"));

  /**
    Copy constructor.
  */
  InetService(const InetService& copy) noexcept;

  /**
    Assignment operator.
  */
  InetService& operator=(const InetService& assign) noexcept;

  /**
    Returns the name of the service.
  */
  const String& getName() const noexcept;

  /**
    Returns the port associated with the service.
  */
  unsigned short getPort() const noexcept;

  /**
    Returns the protocol of the service.
  */
  const String& getProtocol() const noexcept;

  /**
    Writes a string representation of a InetService object to a format stream.
  */
  _COM_AZURE_DEV__BASE__API friend FormatOutputStream& operator<<(
    FormatOutputStream& stream, const InetService& value);
};

/**
  Writes a string representation of a InetService object to a format stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
  FormatOutputStream& stream, const InetService& value);

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(InetService)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
