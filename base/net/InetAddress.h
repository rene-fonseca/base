/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__INET_ADDRESS_H
#define _DK_SDU_MIP__BASE_NET__INET_ADDRESS_H

#include "base/Object.h"
#include "base/string/InvalidFormat.h"
#include "base/string/String.h"
#include "HostNotFound.h"

/**
  This class represents an Internet Protocol (IP) address (support for both IPv4 and IPv6).

  @author René Møller Fonseca
  @version 1.0
*/

class InetAddress : public Object {
public:

  /** Family of internet addresses. */
  typedef enum {IPv4, IPv6} Family;
private:

  /** Internal structure holding the IP address in network byte order. */
  char address[16]; // enough for IPv6 addresses
public:

  /**
    Returns the name of the local host.
  */
  static String getLocalHost();

  /**
    Adds the IP addresses associated with the specified host name into the specified collection.

    @param name The name of the host (e.g. 'www.mip.sdu.dk').
    @param collection The collection to receive the addesses.
  */
//  static void getAddressesByName(const String& name, Collection<InetAddress>& collection) throw(HostNotFound);

  /**
    Initializes the address as unspecified address.
  */
  InetAddress() throw();

  /**
    Initializes the address as from the specified binary address.

    @param addr The internet address in network byte order.
    @param family Specifies the family of the binary address (IPv4 or IPv6).
  */
  InetAddress(const char* addr, Family family) throw();

  /**
    Initializes the address from the specified string. Implicit initialization is allowed.

    @param addr The internet address (e.g. '172.30.33.14' or '::ffff:172.30.33.14').
  */
  InetAddress(const String& addr) throw(InvalidFormat);

  /**
    Copy constructor.
  */
  InetAddress(const InetAddress& copy) throw();

  /**
    Assignment operator.
  */
  InetAddress& operator=(const InetAddress& eq) throw();

  /**
    Returns the host name associated with this IP address. If the host's name cannot be located in the name service, the numeric form of the host's address is returned instead of its name.
  */
  String getHostName() const throw(HostNotFound);

  /**
    Equality operator.
  */
  bool operator==(const InetAddress& eq) throw();

  /**
    Returns true if this address is the unspecified address (i.e. '::').
  */
  bool isUnspecified() const throw();

  /**
    Returns true if this address is the loopback address (i.e. '::1'). The loopback address is a unicast address used to send packets to the local host.
  */
  bool isLoopback() const throw();

  /**
    Returns true if this address is a multicast address.
  */
  bool isMulticast() const throw();

  /**
    Returns true if this address is link local.
  */
  bool isLinkLocal() const throw();

  /**
    Returns true if this address is site local.
  */
  bool isSiteLocal() const throw();

  /**
    Returns true if the address is an IPv4-mapped IPv6 address (e.g. '::ffff:127.0.0.1').
  */
  bool isV4Mapped() const throw();

  /**
    Returns true if this address is IPv4 compatible (e.g. '::127.0.0.1').
  */
  bool isV4Compatible() const throw();

  /**
    Returns the IP address in binary format in network byte order.
  */
  const char* getBytes() const throw();

  /**
    Writes a string representation of the address to a stream.
  */
  FormatOutputStream& operator<<(FormatOutputStream& stream) const;

  /**
    Destroys the address.
  */
  ~InetAddress() throw();
};

#endif
