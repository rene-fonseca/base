/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__INET_ADDRESS_H
#define _DK_SDU_MIP__BASE_NET__INET_ADDRESS_H

#include <base/Object.h>
#include <base/string/InvalidFormat.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/net/HostNotFound.h>
#include <base/net/NetworkException.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class represents an Internet Protocol (IP) address (supports both IPv4
  and IPv6 addresses). IPv6 addresses can be represented on platforms without
  builtin IPv6 support. However, using an IPv6 address on platforms without
  direct support requires implicit conversion to an IPv4 address. If such a
  conversion isn't possible a NetworkException is raised.

  @short Internet Protocol address
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class InetAddress : public virtual Object {
public:

  /** Family of internet addresses. */
  enum Family {
    IP_VERSION_4, /**< Specifies Internet Protocol (IP) version 4. */
    IP_VERSION_6 /**< Specifies Internet Protocol (IP) version 6. */
  };

  /** Type of address. */
  enum AddressType {
    IPV4 = 1, /**< Address is an IPv4 address. */
    IPV6 = 2, /**< Address is an IPv6 address. */
    UNSPECIFIED = 4, /**< Address is the unspecified addresss (i.e. ::0). */
    LOOPBACK = 8, /**< Address is the loopback address. */
    MULTICAST = 16, /**< Address is a multicast address. */
    LINK_LOCAL = 32, /**< Address is a link local address. */
    SITE_LOCAL = 64, /**< Address is a site local address. */
    IPV4_MAPPED = 128, /**< Address is an IPv4 mapped address. */
    IPV4_COMPATIBLE = 256 /**< Address is compatible with IPv4. */
  };
private:

  /** The family of the address. */
  Family family;
  /** Internal structure holding the IP address in network byte order. */
  union Address {
    unsigned int words[4]; // enough for IPv6 and IPv4 addresses
    unsigned short halfWords[8];
    byte octets[16];
  } address;

  /** Parses the specified string as an Internet address (both IPv4 and IPv6). */
  bool parse(const String& addr) throw();
public:

  /**
    Returns the name of the local host.
  */
  static String getLocalHost() throw(NetworkException);

  /**
    Returns the IP addresses associated with the specified host name.

    @param name The name of the host (e.g. 'www.mip.sdu.dk').
  */
  static List<InetAddress> getAddressesByName(const String& name) throw(HostNotFound);

  /**
    Initializes the address as unspecified IPv4 address (matches any address).
  */
  InetAddress() throw();

  /**
    Initializes the address as from the specified binary address.

    @param addr The Internet address in network byte order.
    @param family Specifies the family of the binary address (IPv4 or IPv6).
  */
  InetAddress(const byte* addr, Family family) throw();

  /**
    Initializes the address from the specified string. Implicit initialization is allowed.

    @param addr The internet address (e.g. '172.30.33.14' or '::ffff:172.30.33.14').
  */
  InetAddress(const String& addr) throw(InvalidFormat);

  /**
    Initializes the address from the specified string.

    @param addr The internet address (e.g. '172.30.33.14' or '::ffff:172.30.33.14').
    @param family Specifies the family to accept.
  */
  InetAddress(const String& addr, Family family) throw(InvalidFormat);

  /**
    Copy constructor.
  */
  InetAddress(const InetAddress& copy) throw();

  /**
    Assignment operator.
  */
  InetAddress& operator=(const InetAddress& eq) throw();

  /**
    Returns the family of the address.
  */
  inline Family getFamily() const throw() {return family;}

  /**
    Returns the IP address in binary format in network byte order.
  */
  inline const byte* getAddress() const throw() {return address.octets;}

  /**
    Returns the IP address in binary format in network byte order (this is only
    valid if either isIPv4Mapped() or isIPv4Compatible() returns true).
  */
  inline const byte* getIPv4Address() const throw() {return &address.octets[12];}

  /**
    Returns the domain/host name associated with this IP address. Throws
    'HostNotFound' if the host cannot be resolved.

    @param fullyQualified Specifies that the fully-qualified domain name should be returned for local hosts. Default is false.
  */
  String getHostName(bool fullyQualified = false) const throw(HostNotFound);

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
    Returns true if the address is an IPv4-mapped IPv6 address (e.g.
    '::ffff:127.0.0.1'). Returns false if address is an IPv4 addresses or the
    address is the unspecified address (i.e. ::).
  */
  bool isIPv4Mapped() const throw();

  /**
    Returns true if this address is an IPv4 compatible IPv6 address (e.g.
    '::127.0.0.1') or if it is an IPv4 address. Returns false if the address is
    the unspecified address (i.e. ::).
  */
  bool isIPv4Compatible() const throw();

  /**
    Returns the type of the address.
  */
  unsigned int getType() const throw();

  /**
    Converts an IPv4 address to an IPv4-mapped IPv6 address (IPv6 addresses are not modified).

    @return Always returns true.
  */
  bool convertToIPv6() throw();

  /**
    Converts an IPv4-mapped IPv6 address to an IPv4 address (IPv4 addresses are not modified).

    @return Returns true on success (fails if address isn't an IPv4-mapped IPv6 address or an IPv4 address).
  */
  bool convertToIPv4() throw();

  /**
    Sets the address.

    @param addr The Internet address in network byte order.
    @param family Specifies the family of the binary address (IPv4 or IPv6).
  */
  void setAddress(const byte* addr, Family family) throw();

  /**
    Writes a string representation of the address to a stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value);
};

/**
  Writes a string representation of the InetAddress object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
