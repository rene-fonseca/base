/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__INET_SERVICE_H
#define _DK_SDU_MIP__BASE_NET__INET_SERVICE_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/string/FormatOutputStream.h>
#include <base/net/ServiceNotFound.h>

/**
  This class represents an internet service.

  @author René Møller Fonseca
  @version 1.0
*/

class InetService : public virtual Object {
private:

  /** Specifies the official name of the service. */
  String<> name;
  /** Specifies the port of the service (in host byte order). */
  unsigned short port;
  /** Specifies the name of the protocol of the service. */
  String<> protocol;
public:

  /**
    Returns the port number of the service given by the service name and protocol.

    @param name Specifies the name of the service.
    @param protocol Specifies the name of the protocol. Default is "tcp".

    @return 0 if the service was not found.
  */
  static unsigned short getByName(const String<>& name, const String<>& protocol) throw();

  /**
    Returns the name of the service given by the specified port and protocol.

    @param port Specifies the port number.
    @param protocol Specifies the name of the protocol. Default is "tcp".

    @return Empty string if service was not found.
  */
  static String<> getByPort(unsigned short port, const String<>& protocol) throw();

  /**
    Initializes a service object by specified service name and protocol.

    @param name Specifies the name of the service.
    @param protocol Specifies the name of the protocol. Default is "tcp".
  */
  InetService(const String<>& name, const String<>& protocol = "tcp") throw(ServiceNotFound);

  /**
    Initializes a service object by specified port number and protocol.

    @param port The port number.
    @param protocol Specifies the name of the protocol. Default is "tcp".
  */
  InetService(unsigned short port, const String<>& protocol = "tcp") throw(ServiceNotFound);

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
  const String<>& getName() const throw();

  /**
    Returns the port associated with the service.
  */
  unsigned short getPort() const throw();

  /**
    Returns the protocol of the service.
  */
  const String<>& getProtocol() const throw();

  /**
    Writes a string representation of a InetService object to a format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const InetService& value);
};

/**
  Writes a string representation of a InetService object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const InetService& value);

#endif
