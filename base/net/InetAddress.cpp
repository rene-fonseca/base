/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Base.h>
#include <base/net/InetAddress.h>
#include <base/Functor.h>
#include <base/concurrency/Thread.h>
#include <base/ByteOrder.h>
#include <base/Type.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32) // temporary solution until arch independant types have been defined
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  if (!defined(_DK_SDU_MIP__BASE__INET_IPV6) && (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__WXP))
#    define _DK_SDU_MIP__BASE__INET_IPV6
#  endif
typedef DWORD uint32_t;
#else // unix
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/param.h> // may define MAXHOSTNAMELEN (linux, irix)
#  include <netinet/in.h> // define IP address
#  include <netdb.h> // gethostbyname, may define MAXHOSTNAMELEN (solaris)
#  include <arpa/inet.h> // defines inet_ntop...
#  include <unistd.h> // defines gethostname
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if 0
String InetAddress::getLocalDomainName() throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
#endif
}
#endif

String InetAddress::getLocalHost() throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // I use thread local storage 'cause I don't know what the maximum length is
  // the microsoft example code that I have seen assumes that the name cannot exceed 200 chars
  Allocator<char>* buffer = Thread::getLocalStorage();
  ASSERT(buffer->getSize() > 200);
  char* name = buffer->getElements();
  if (gethostname(name, buffer->getSize())) {
    throw NetworkException("Unable to get local host name", Type::getType<InetAddress>());
  }
#else // unix
  char name[MAXHOSTNAMELEN + 1]; // does MAXHOSTNAMELEN include terminator
  if (gethostname(name, sizeof(name))) {
    throw NetworkException("Unable to get local host name", Type::getType<InetAddress>());
  }
#endif // flavor
  return String(name);
}

List<InetAddress> InetAddress::getAddressesByName(const String& name) throw(HostNotFound) {
  List<InetAddress> result;
  
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct addrinfo hint;
  fill<char>(Cast::getCharAddress(hint), sizeof(hint), 0);
  hint.ai_family = PF_UNSPEC;

  struct addrinfo* ai;
  if (getaddrinfo(name.getElements(), 0, &hint, &ai) != 0) { // MT-level is safe
    throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
  }

  struct addrinfo* i = ai;
  while (i) {
    uint8* addr;
    switch (i->ai_family) {
    case PF_INET:
      addr = getByteAddress(Cast::pointer<struct sockaddr_in*>(i->ai_addr)->sin_addr);
      result.append(InetAddress(addr, IP_VERSION_4));
      break;
    case PF_INET6:
      addr = getByteAddress(Cast::pointer<struct sockaddr_in6*>(i->ai_addr)->sin6_addr);
      result.append(InetAddress(addr, IP_VERSION_6));
      break;
//    default:
// just ignore the unsupported families
//      throw NetworkException("Unsupported family", Type::getType<InetAddress>());
    }
    i = i->ai_next; // go to the next info entry
  }

  freeaddrinfo(ai); // release resources - MT-level is safe
#else // use ordinary BSD sockets - IPv4
  struct hostent* hp;

#  if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (!(hp = gethostbyname(name.getElements()))) { // MT-safe
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  elif ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65) ||
         (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS))
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (!(hp = gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &error))) {
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &hp, &error)) {
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  else
    #warning gethostbyname is not MT-safe
    if (!(hp = gethostbyname(name.getElements()))) {
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  endif
    
  for (char** p = hp->h_addr_list; *p != 0; p++) {
    result.append(InetAddress(Cast::pointer<const uint8*>(*p), IP_VERSION_4));
  }
#endif // _DK_SDU_MIP__BASE__INET_IPV6
  return result;
}

InetAddress InetAddress::getAddressByName(const String& name) throw(HostNotFound) {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct addrinfo hint;
  fill<char>(Cast::getCharAddress(hint), sizeof(hint), 0);
  hint.ai_family = PF_UNSPEC;

  struct addrinfo* ai;
  if (getaddrinfo(name.getElements(), 0, &hint, &ai) != 0) { // MT-level is safe
    throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
  }
  
  struct addrinfo* i = ai;
  while (i) {
    uint8* addr;
    switch (i->ai_family) {
    case PF_INET:
      addr = getByteAddress(Cast::pointer<struct sockaddr_in*>(i->ai_addr)->sin_addr);
      return InetAddress(addr, IP_VERSION_4);
    case PF_INET6:
      addr = getByteAddress(Cast::pointer<struct sockaddr_in6*>(i->ai_addr)->sin6_addr);
      return InetAddress(addr, IP_VERSION_6);
    }
    i = i->ai_next; // go to the next info entry
  }
  
  freeaddrinfo(ai); // release resources - MT-level is safe
#else // use ordinary BSD sockets - IPv4
  struct hostent* hp;

#  if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (!(hp = gethostbyname(name.getElements()))) { // MT-safe
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  elif ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65) ||
         (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS))
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (!(hp = gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &error))) {
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &hp, &error)) {
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  else
    #warning gethostbyname is not MT-safe
    if (!(hp = gethostbyname(name.getElements()))) {
      throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
    }
#  endif
    
  for (char** p = hp->h_addr_list; *p != 0; p++) {
    return InetAddress(Cast::pointer<const uint8*>(*p), IP_VERSION_4);
  }
#endif // _DK_SDU_MIP__BASE__INET_IPV6
  throw HostNotFound("Unable to lookup host by name", Type::getType<InetAddress>());
}

bool InetAddress::parse(const String& addr) throw() {
  String::ReadIterator i = addr.getBeginReadIterator();
  const String::ReadIterator end = addr.getEndReadIterator();

  int colon = addr.indexOf(':');
  bool readIPv4 = true; // read IPv4 address from end of addr

  if (colon < 0) { // IPv4
    family = IP_VERSION_4;
    address.words[0] = 0;
    address.words[1] = 0;
    address.words[2] = 0;
  } else { // IPv6
    family = IP_VERSION_6;
    int dot = addr.indexOf('.', colon + 1);
    readIPv4 = (dot >= 0);
    const String::ReadIterator endIPv6 = (dot >= 0) ? (i + addr.lastIndexOf(':', dot) + 1) : end;

    unsigned int maxValues = readIPv4 ? 6 : 8; // maximum number of half-words to read
    int zeroIndex = -1; // index of zero-compression (invalidated)
    unsigned int writeHead = 0;

    if ((i < endIPv6) && (*i == ':')) { // addr begins with zero-compression
      ++i;
      if (!((i < endIPv6) && (*i == ':'))) {
        return false;
      }
      ++i;
      zeroIndex = 0;
    }

    while ((writeHead < maxValues) && (i < endIPv6) && ASCIITraits::isHexDigit(*i)) { // read half-words
      unsigned int value = 0; // overflow not possible with 4 digits [0x0000;0xffff]
      value = ASCIITraits::digitToValue(*i++);
      if ((i < endIPv6) && ASCIITraits::isHexDigit(*i)) {
        value = 16 * value + ASCIITraits::digitToValue(*i++);
        if ((i < endIPv6) && ASCIITraits::isHexDigit(*i)) {
          value = 16 * value + ASCIITraits::digitToValue(*i++);
          if ((i < endIPv6) && ASCIITraits::isHexDigit(*i)) {
            value = 16 * value + ASCIITraits::digitToValue(*i++);
          }
        }
      }
      address.halfWords[writeHead++] = ByteOrder::toBigEndian<unsigned short>(value);

      if (i == endIPv6) {
        break;
      }

      if (!((i < endIPv6) && (*i++ == ':') && (i < end))) { // check for value separator - yes last check must be (i < end)
        return false;
      }

      if ((zeroIndex < 0) && (*i == ':')) { // check for zero-compression
        ++i;
        zeroIndex = writeHead;
      }
    }
    if (!((i == endIPv6) && ((zeroIndex >= 0) || (writeHead == maxValues)))) {
      return false;
    }
    if (zeroIndex >= 0) { // is zero-compression present
      move(&address.halfWords[zeroIndex + maxValues - writeHead], &address.halfWords[zeroIndex], writeHead - zeroIndex);
      fill<unsigned short>(&address.halfWords[zeroIndex], maxValues - writeHead, 0);
    }
  }

  if (readIPv4) { // should we read an IPv4 address
    unsigned int relativeIndex = 0;
    while ((relativeIndex < 4) && (i < end) && ASCIITraits::isDigit(*i)) { // get 4 integer values
      unsigned int value = ASCIITraits::digitToValue(*i++); // overflow not possible with 3 digits [0;999]
      if ((i < end) && ASCIITraits::isDigit(*i)) {
        value = 10 * value + ASCIITraits::digitToValue(*i++);
        if ((i < end) && ASCIITraits::isDigit(*i)) {
          value = 10 * value + ASCIITraits::digitToValue(*i++);
        }
      }
      if (value > 255) {
        return false;
      }
      address.octets[12 + relativeIndex++] = value;
      if (i == end) {
        break;
      }
      if (!((i < end) && (*i++ == '.'))) {
        return false;
      }
    }
    if (relativeIndex != 4) { // i == end checked below
      return false;
    }
  }

  return i == end;
}

InetAddress::InetAddress() throw() : family(IP_VERSION_6) {
  address.words[0] = 0; // set to unspecified address
  address.words[1] = 0;
  address.words[2] = 0;
  address.words[3] = 0;
}

InetAddress::InetAddress(const uint8* addr, Family family) throw() {
  setAddress(addr, family);
}

InetAddress::InetAddress(const String& addr) throw(InvalidFormat) {
  assert(parse(addr), InvalidFormat("Not an Internet address", this));
}

InetAddress::InetAddress(const String& addr, Family family) throw(InvalidFormat) {
  assert(parse(addr) && (this->family == family), InvalidFormat("Not an Internet address", this));
}

InetAddress::InetAddress(const InetAddress& copy) throw() :
  family(copy.family),
  address(copy.address) {
}

InetAddress& InetAddress::operator=(const InetAddress& eq) throw() {
  family = eq.family; // no need to protect against self-assignment
  address = eq.address;
  return *this;
}

String InetAddress::getHostName(bool fullyQualified) const throw(HostNotFound) {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct sockaddr_in6 addr;
  fill<char>(Cast::getCharAddress(addr), sizeof(addr), 0);
#if defined(SIN6_LEN)
  addr.sin6_len = sizeof(addr);
#endif
  addr.sin6_family = AF_INET6;
  copy<char>(Cast::getCharAddress(addr.sin6_addr), getCharAddress(address), sizeof(address));
  char hostname[NI_MAXHOST]; // includes space for terminator

  if (getnameinfo(
        Cast::pointer<sockaddr*>(&addr),
        sizeof(addr),
        hostname,
        sizeof(hostname),
        0,
        0,
        NI_NAMEREQD | (fullyQualified ? 0 : NI_NOFQDN)
      ) != 0) {
    throw HostNotFound("Unable to resolve IP address", this);
  }

  return String(hostname);
#else // use ordinary BSD sockets - IPv4
  struct hostent* hp;

  if ((family == IP_VERSION_6) &&
      ((address.words[0] != 0) ||
       (address.words[1] != 0) ||
       (address.words[2] != ByteOrder::toBigEndian<unsigned int>(0xffffU)) &&
       (address.words[3] != 0))) {
    throw HostNotFound("Unable to resolve IP address", this);
  }
  
#  if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    hp = gethostbyaddr(
      Cast::getCharAddress(address.words[3]),
      sizeof(address.words[3]),
      AF_INET
    ); // MT-safe
    assert(hp, HostNotFound("Unable to resolve IP address", this));
#  elif ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65) ||
         (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS))
    struct hostent result;
    char buffer[1024]; // how big should this buffer be
    int error;
    hp = gethostbyaddr_r(
      Cast::getCharAddress(address.words[3]),
      sizeof(address.words[3]),
      AF_INET,
      &result,
      buffer,
      sizeof(buffer),
      &error
    );
    assert(hp, HostNotFound("Unable to resolve IP address", this));
#  elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct hostent result;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (gethostbyaddr_r(
          Cast::getCharAddress(address.words[3]),
          sizeof(address.words[3]),
          AF_INET,
          &result,
          buffer,
          sizeof(buffer),
          &hp,
          &error)) {
      throw HostNotFound("Unable to resolve IP address", this);
    }
#  else
    #warning gethostbyaddr is not MT-safe
    if (!(hp = gethostbyaddr(Cast::getCharAddress(address.words[3]), sizeof(address.words[3]), AF_INET))) {
      throw HostNotFound("Unable to resolve IP address", this);
    }
#  endif

  return String(hp->h_name);
#endif // _DK_SDU_MIP__BASE__INET_IPV6
}

bool InetAddress::operator==(const InetAddress& eq) throw() {
  return (address.words[0] == eq.address.words[0]) &&
    (address.words[1] == eq.address.words[1]) &&
    (address.words[2] == eq.address.words[2]) &&
    (address.words[3] == eq.address.words[3]);
}

bool InetAddress::isUnspecified() const throw() {
  // ok for both IPv4 and IPv6
  return (address.words[0] == 0) &&
    (address.words[1] == 0) &&
    (address.words[2] == 0) &&
    (address.words[3] == 0);
}

bool InetAddress::isLoopback() const throw() {
  return (address.words[0] == 0) && (address.words[1] == 0) && (address.words[2] == 0) &&
    (
      (family == IP_VERSION_6) && (address.words[3] == ByteOrder::toBigEndian<unsigned int>(0x00000001U)) || // IPv6 only
      (family == IP_VERSION_4) && (address.words[3] == ByteOrder::toBigEndian<unsigned int>(0x7f000001U)) // IPv4 only
    );
}

bool InetAddress::isMulticast() const throw() {
  return (family == IP_VERSION_6) && (address.octets[0] == 0xff); // TAG: IPv4 version?
}

bool InetAddress::isLinkLocal() const throw() {
  return (family == IP_VERSION_6) && (address.words[0] & ByteOrder::toBigEndian<unsigned int>(0xffc00000U) == ByteOrder::toBigEndian<unsigned int>(0xfe800000U)); // TAG: IPv4 version?
}

bool InetAddress::isSiteLocal() const throw() {
  return (family == IP_VERSION_6) && (address.words[0] & ByteOrder::toBigEndian<unsigned int>(0xffc00000U) == ByteOrder::toBigEndian<unsigned int>(0xfec00000U)); // TAG: IPv4 version?
}

bool InetAddress::isIPv4Mapped() const throw() {
  return (family == IP_VERSION_6) && (address.words[0] == 0) && (address.words[1] == 0) && (address.words[2] == ByteOrder::toBigEndian<unsigned int>(0xffffU)) && (address.words[3] != 0);
}

bool InetAddress::isIPv4Compatible() const throw() {
  // ok for both IPv4 and IPv6
  return (address.words[0] == 0) && (address.words[1] == 0) && (address.words[2] == 0) && (address.words[3] != 0);
}

unsigned int InetAddress::getType() const throw() {
  unsigned int result = (family == IP_VERSION_4) ? IPV4 : IPV6;
  if (family == IP_VERSION_4) {
    if (address.words[3] == 0) {
      result |= UNSPECIFIED;
    } else if (address.words[3] == ByteOrder::toBigEndian<unsigned int>(0x7f000001U)) {
      result |= LOOPBACK;
    }
    result |= IPV4_COMPATIBLE;
  } else { // IPv6
    if ((address.words[0] == 0) && (address.words[1] == 0)) {
      if (address.words[2] == 0) {
        if (address.words[3] == 0) {
          result |= UNSPECIFIED;
        } else if (address.words[3] == ByteOrder::toBigEndian<unsigned int>(0x00000001U)) {
          result |= LOOPBACK;
          result |= IPV4_COMPATIBLE;
        } else {
          result |= IPV4_COMPATIBLE;
        }
      } else if ((address.words[2] == ByteOrder::toBigEndian<unsigned int>(0x0000ffffU)) && (address.words[3] != 0)) {
        result |= IPV4_MAPPED;
      }
    } else if (address.octets[0] == 0xff) {
      result |= MULTICAST;
    } else if (address.words[0] & ByteOrder::toBigEndian<unsigned int>(0xffc00000U) == ByteOrder::toBigEndian<unsigned int>(0xfe800000U)) {
      result |= LINK_LOCAL;
    } else if (address.words[0] & ByteOrder::toBigEndian<unsigned int>(0xffc00000U) == ByteOrder::toBigEndian<unsigned int>(0xfec00000U)) {
      result |= SITE_LOCAL;
    }
  }
  return result;
}

bool InetAddress::convertToIPv6() throw() {
  if (family == IP_VERSION_4) {
    family = IP_VERSION_6;
    address.words[2] = ByteOrder::toBigEndian<unsigned int>(0x0000ffffU);
  }
  return true; // cannot fail
}

bool InetAddress::convertToIPv4() throw() {
  if (family == IP_VERSION_6) {
    if (isIPv4Mapped()) {
      family = IP_VERSION_4;
      address.words[2] = 0;
      return true;
    } else {
      return false; // cannot convert
    }
  } else {
    return true; // already IPv4
  }
}

void InetAddress::setAddress(const uint8* addr, Family family) throw() {
  this->family = family;
  switch (family) {
  case IP_VERSION_4:
    address.words[0] = 0;
    address.words[1] = 0;
    address.words[2] = 0;
    address.octets[12] = *addr++;
    address.octets[13] = *addr++;
    address.octets[14] = *addr++;
    address.octets[15] = *addr++;
    break;
  case IP_VERSION_6:
    copy(address.octets, addr, 16);
    break;
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value) throw(IOException) {
  // TAG: do not write directly to stream: use internal stream first
  if (value.family == InetAddress::IP_VERSION_6) {
    unsigned int type = value.getType();
    if (type & InetAddress::IPV4_MAPPED) { // ::ffff:255.255.255.255
      stream << MESSAGE("::ffff:"); // write IPv4 address after this
    } else if (type & (InetAddress::IPV4_COMPATIBLE|InetAddress::LOOPBACK) == InetAddress::IPV4_COMPATIBLE) { // ::255.255.255.255
      stream << MESSAGE("::"); // write IPv4 address after this
    } else { // 1080::8:800:200c:417a
      const unsigned short* addr = value.address.halfWords;

      // find longest zero sequence
      int firstZero = 8; // index 8 used later
      int endZero = 8;
      for (int i = 0; i < 8;) {
        if (addr[i] == 0) {
          int first = i;
          for (int j = first; (j < 8) && (addr[j] == 0); ++j, ++i) { // find end of zeros
          }
          if ((i - first) > (endZero - firstZero)) {
            firstZero = first;
            endZero = i;
          }
        }
        ++i; // also skip non-zero element after zero element
      }

      for (int i = 0; i < firstZero;) { // write values before zeros
        stream << NOPREFIX << HEX << ByteOrder::fromBigEndian<unsigned short>(addr[i++]);
        if (i < firstZero) {
          stream << ':';
        }
      }
      if (firstZero < endZero) { // are zeros present
        stream << MESSAGE("::");
        for (int i = endZero; i < 8; ++i) { // write values after zeros
          if (i > endZero) {
            stream << ':';
          }
          stream << NOPREFIX << HEX << ByteOrder::fromBigEndian<unsigned short>(addr[i]);
        }
      }
      return stream; // do not write IPv4 address
    }
  }

  const uint8* addr = value.getIPv4Address();
  return stream << DEC << addr[0] << '.' << DEC << addr[1] << '.' << DEC << addr[2] << '.' << DEC << addr[3];
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
