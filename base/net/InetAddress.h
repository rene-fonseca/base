/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__INET_ADDRESS_H
#define _DK_SDU_MIP__BASE_NET__INET_ADDRESS_H

#include <base/Object.h>
#include <base/string/InvalidFormat.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/net/HostNotFound.h>
#include <base/net/NetworkException.h>

/**
  This class represents an Internet Protocol (IP) address (support for both IPv4 and IPv6).

  @author René Møller Fonseca
  @version 1.0
*/

class InetAddress : public virtual Object {
public:

  /** Family of internet addresses. */
  typedef enum {IPv4, IPv6} Family;
private:

  /** Internal structure holding the IP address in network byte order. */
#if defined(HAVE_INET_IPV6)
  struct {char buffer[16];} address; // enough for IPv6 and IPv4 addresses
#else
  struct {char buffer[4];} address; // enough for IPv4 addresses
#endif
public:

  /**
    Returns the name of the local host.
  */
  static String<> getLocalHost();

  /**
    Returns the IP addresses associated with the specified host name.

    @param name The name of the host (e.g. 'www.mip.sdu.dk').
  */
  static List<InetAddress> getAddressesByName(const String<>& name) throw(HostNotFound);

  /**
    Initializes the address as unspecified address.
  */
  InetAddress() throw();

  /**
    Initializes the address as from the specified binary address.

    @param addr The internet address in network byte order.
    @param family Specifies the family of the binary address (IPv4 or IPv6).
  */
  InetAddress(const char* addr, Family family) throw(NetworkException);

  /**
    Initializes the address from the specified string. Implicit initialization is allowed.

    @param addr The internet address (e.g. '172.30.33.14' or '::ffff:172.30.33.14').
  */
  InetAddress(const String<>& addr) throw(InvalidFormat);

  /**
    Copy constructor.
  */
  InetAddress(const InetAddress& copy) throw();

  /**
    Assignment operator.
  */
  InetAddress& operator=(const InetAddress& eq) throw();

  /**
    Returns the IP address in binary format in network byte order.
  */
  const char* getAddress() const throw();

  /**
    Returns the domain/host name associated with this IP address. Throws
    'HostNotFound' if the host cannot be resolved.

    @param fullyQualified Specifies that the fully-qualified domain name should be returned for local hosts. Default is false.
  */
  String<> getHostName(bool fullyQualified = false) const throw(HostNotFound);

  /**
    Equality operator.
  */
  bool operator==(const InetAddress& eq) throw();

  /**
    Returns true if this address is the unspecified address (i.e. '::' and
    '0.0.0.0' in the case of IPv6 and IPv4, respectively).
  */
  bool isUnspecified() const throw();

  /**
    Returns true if this address is the loopback address (i.e. '::1' and
    '127.0.0.1' in the case of IPv6 and IPv4, respectively). The loopback
    address is a unicast address used to send packets to the local host.
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
    Returns true if the address is an IPv4-mapped IPv6 address
    (e.g. '::ffff:127.0.0.1'). Returns false for IPv4 addresses.
  */
  bool isV4Mapped() const throw();

  /**
    Returns true if this address is IPv4 compatible (e.g. '::127.0.0.1').
    Returns true for IPv4 addresses.
  */
  bool isV4Compatible() const throw();

  /**
    Sets the address. Throws 'NetworkException' if the specified family is not
    supported by the operating system.

    @param addr The internet address in network byte order.
    @param family Specifies the family of the binary address (IPv4 or IPv6).
  */
  void setAddress(const char* addr, Family family) throw(NetworkException);

  /**
    Writes a string representation of the address to a stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value);
};

/**
  Writes a string representation of the InetAddress object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value);

#endif
