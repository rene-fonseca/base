/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__INET_END_POINT_H
#define _DK_SDU_MIP__BASE_NET__INET_END_POINT_H

#include <base/Object.h>
#include <base/net/InetAddress.h>
#include <base/net/InetService.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Internet end point (i.e. both internet address and port number).

  @short Internet end point.
  @ingroup net
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class InetEndPoint : public virtual Object {
private:

  /** Host. */
  InetAddress address;
  /** Port. */
  unsigned short port;
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
  InetEndPoint(const InetAddress& address, const String& service) throw(ServiceNotFound);

  /**
    Initializes end point from string representations of host and service.

    @param host The host (e.g. '127.0.0.1' or 'www.mip.sdu.dk').
    @param service The service (e.g. 'http' or '80').
  */
  InetEndPoint(const String& host, const String& service) throw(ServiceNotFound);

  /**
    Initialize object from other object.
  */
  InetEndPoint(const InetEndPoint& copy) throw();

  /**
    Default assignment.
  */
  InetEndPoint& operator=(const InetEndPoint& eq) throw();

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
  Writes a string representation of the InetEndPoint object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const InetEndPoint& value) throw(IOException);

class Hash<InetEndPoint> {
public:
  
  inline unsigned long operator()(const InetEndPoint& value) throw() {
    return Hash<InetAddress>()(value.getAddress()) * 31 + value.getPort();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
