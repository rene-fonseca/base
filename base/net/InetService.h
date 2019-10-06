/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class represents an internet service.

  @short Internet Protocol service.
  @ingroup net
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class InetService : public virtual Object {
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
    const String& name, const String& protocol) throw();

  /**
    Returns the name of the service given by the specified port and protocol.

    @param port Specifies the port number.
    @param protocol Specifies the name of the protocol. Default is "tcp".

    @return Empty string if service was not found.
  */
  static String getByPort(unsigned short port, const String& protocol) throw();

  /**
    Initializes a service object by specified service name and protocol.

    @param name Specifies the name of the service.
    @param protocol Specifies the name of the protocol. Default is "tcp".
  */
  explicit InetService(
    const String& name,
    const String& protocol = Literal("tcp")) throw(ServiceNotFound);

  /**
    Initializes a service object by specified port number and protocol.

    @param port The port number.
    @param protocol Specifies the name of the protocol. Default is "tcp".
  */
  explicit InetService(
    unsigned short port,
    const String& protocol = Literal("tcp")) throw(ServiceNotFound);

  /**
    Copy constructor.
  */
  InetService(const InetService& copy) throw();

  /**
    Assignment operator.
  */
  InetService& operator=(const InetService& eq) throw();

  /**
    Returns the name of the service.
  */
  const String& getName() const throw();

  /**
    Returns the port associated with the service.
  */
  unsigned short getPort() const throw();

  /**
    Returns the protocol of the service.
  */
  const String& getProtocol() const throw();

  /**
    Writes a string representation of a InetService object to a format stream.
  */
  friend FormatOutputStream& operator<<(
    FormatOutputStream& stream, const InetService& value) throw(IOException);
};

/**
  Writes a string representation of a InetService object to a format stream.
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const InetService& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
