/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) // temporary solution until arch independant types have been defined
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  if (!defined(_COM_AZURE_DEV__BASE__INET_IPV6) && \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WXP))
#    define _COM_AZURE_DEV__BASE__INET_IPV6
#  endif
#else // unix
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/param.h> // may define MAXHOSTNAMELEN (linux, irix)
#  include <netinet/in.h> // define IP address
#  include <netdb.h> // gethostbyname, may define MAXHOSTNAMELEN (solaris)
#  include <arpa/inet.h> // defines inet_ntop...
#  include <unistd.h> // defines gethostname
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0
String InetAddress::getLocalDomainName() throw(NetworkException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#else // unix
#endif
}
#endif

String InetAddress::getLocalHost() throw(NetworkException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // I use thread local storage 'cause I don't know what the maximum length is
  // the microsoft example code that I have seen assumes that the name cannot exceed 200 chars
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  BASSERT(buffer.getSize()/sizeof(char) > 200);
  char* name = (char*)buffer.getElements();
  if (gethostname(name, buffer.getSize()/sizeof(char))) {
    throw NetworkException(
      "Unable to get local host name.",
      Type::getType<InetAddress>()
    );
  }
#else // unix
  char name[MAXHOSTNAMELEN + 1]; // does MAXHOSTNAMELEN include terminator
  if (gethostname(name, sizeof(name))) {
    throw NetworkException("Unable to get local host name.", Type::getType<InetAddress>());
  }
#endif // flavor
  return NativeString(name);
}

List<InetAddress> InetAddress::getAddressesByName(const String& name) throw(HostNotFound) {
  List<InetAddress> result;
  
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  struct addrinfo hint;
  fill<uint8>(Cast::getAddress(hint), sizeof(hint), 0);
  hint.ai_family = PF_UNSPEC;

  struct addrinfo* ai = nullptr;
  if (getaddrinfo(name.getElements(), 0, &hint, &ai) != 0) { // MT-level is safe
    throw HostNotFound(
      "Unable to lookup host by name",
      Type::getType<InetAddress>()
    );
  }

  struct addrinfo* i = ai;
  while (i) {
    uint8* address = nullptr;
    switch (i->ai_family) {
    case PF_INET:
      address = Cast::getAddress(
        Cast::pointer<struct sockaddr_in*>(i->ai_addr)->sin_addr
      );
      result.append(InetAddress(address, IP_VERSION_4));
      break;
    case PF_INET6:
      address = Cast::getAddress(
        Cast::pointer<struct sockaddr_in6*>(i->ai_addr)->sin6_addr
      );
      result.append(InetAddress(address, IP_VERSION_6));
      break;
    default:
      break; // just ignore the unsupported families
    }
    i = i->ai_next; // go to the next info entry
  }

  freeaddrinfo(ai); // release resources - MT-level is safe
#else // use ordinary BSD sockets - IPv4
  struct hostent* hp = nullptr;

#  if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (!(hp = gethostbyname(name.getElements()))) { // MT-safe
      throw HostNotFound(
        "Unable to lookup host by name",
        Type::getType<InetAddress>()
      );
    }
#  elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
         (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS))
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error = 0;
    if (!(hp = gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &error))) {
      throw HostNotFound(
        "Unable to lookup host by name",
        Type::getType<InetAddress>()
      );
    }
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error = 0;
    if (gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &hp, &error)) {
      throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
    }
#  else
//    #warning gethostbyname is not MT-safe
    if (!(hp = gethostbyname(name.getElements()))) {
      throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
    }
#  endif
    
  for (char** p = hp->h_addr_list; *p != 0; p++) {
    result.append(InetAddress(Cast::pointer<const uint8*>(*p), IP_VERSION_4));
  }
#endif // _COM_AZURE_DEV__BASE__INET_IPV6
  return result;
}

InetAddress InetAddress::getAddressByName(const String& name) throw(HostNotFound) {
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  struct addrinfo hint;
  fill<uint8>(Cast::getAddress(hint), sizeof(hint), 0);
  hint.ai_family = PF_UNSPEC;

  struct addrinfo* ai = nullptr;
  if (getaddrinfo(name.getElements(), 0, &hint, &ai) != 0) { // MT-level is safe
    throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
  }
  
  struct addrinfo* i = ai;
  while (i) {
    uint8* address = nullptr;
    switch (i->ai_family) {
    case PF_INET:
      address = Cast::getAddress(Cast::pointer<struct sockaddr_in*>(i->ai_addr)->sin_addr);
      return InetAddress(address, IP_VERSION_4);
    case PF_INET6:
      address = Cast::getAddress(Cast::pointer<struct sockaddr_in6*>(i->ai_addr)->sin6_addr);
      return InetAddress(address, IP_VERSION_6);
    }
    i = i->ai_next; // go to the next info entry
  }
  
  freeaddrinfo(ai); // release resources - MT-level is safe
#else // use ordinary BSD sockets - IPv4
  struct hostent* hp;

#  if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (!(hp = gethostbyname(name.getElements()))) { // MT-safe
      throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
    }
#  elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
         (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS))
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error = 0;
    if (!(hp = gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &error))) {
      throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
    }
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error = 0;
    if (gethostbyname_r(name.getElements(), &h, buffer, sizeof(buffer), &hp, &error)) {
      throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
    }
#  else
//    #warning gethostbyname is not MT-safe
    if (!(hp = gethostbyname(name.getElements()))) {
      throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
    }
#  endif
    
  for (char** p = hp->h_addr_list; *p != 0; p++) {
    return InetAddress(Cast::pointer<const uint8*>(*p), IP_VERSION_4);
  }
#endif // _COM_AZURE_DEV__BASE__INET_IPV6
  throw HostNotFound("Unable to lookup host by name.", Type::getType<InetAddress>());
}

bool InetAddress::isIPv4(const String::ReadIterator begin, const String::ReadIterator end) throw() {
  String::ReadIterator i = begin;
  unsigned int words = 0; // get 4 decimal values
  while ((words < 4) && (i != end) && ASCIITraits::isDigit(*i)) {
    // overflow not possible with 3 digits [0;999]
    unsigned int value = ASCIITraits::digitToValue(*i++);
    if ((i < end) && ASCIITraits::isDigit(*i)) {
      value = 10 * value + ASCIITraits::digitToValue(*i++);
      if ((i < end) && ASCIITraits::isDigit(*i)) {
        value = 10 * value + ASCIITraits::digitToValue(*i++);
      }
    }
    if (value > 255) {
      return false;
    }
    if (i == end) {
      break;
    }
    if (!((i != end) && (*i++ == '.'))) {
      return false;
    }
  }
  if (words != 4) {
    return false;
  }
  return i == end;
}

bool InetAddress::isIPv6(const String::ReadIterator begin, const String::ReadIterator end) throw() {
  String::ReadIterator i = begin;
  String::ReadIterator j = i;
  String::ReadIterator endIPv6 = i; // end of address excluding IPv4 suffix
  
  while ((j != end) && (*j != '.')) {
    if (*j == ':') {
      endIPv6 = j;
    }
    ++j;
  }
  if (j == end) {
    endIPv6 = end;
  }
  
  const bool readIPv4 = j != end; // IPv4 suffix is present
  unsigned int maxWords = readIPv4 ? 6 : 8; // maximum number of half-words to read
  bool zeroCompression = false;
  unsigned int words = 0;
  
  if ((i != endIPv6) && (*i == ':')) { // address begins with zero-compression
    ++i;
    if (!((i != endIPv6) && (*i == ':'))) {
      return false;
    }
    ++i;
    zeroCompression = true;
    --maxWords;
  }
  // TAG: reading RFC 2373: zero compression fields of 0 words are prohibited
  
  while ((words < maxWords) && (i != endIPv6) && ASCIITraits::isHexDigit(*i)) {
    ++i;
    if ((i != endIPv6) && ASCIITraits::isHexDigit(*i)) {
      ++i;
      if ((i != endIPv6) && ASCIITraits::isHexDigit(*i)) {
        ++i;
        if ((i != endIPv6) && ASCIITraits::isHexDigit(*i)) {
          ++i;
        }
      }
    }
    if (i == endIPv6) {
      if (endIPv6 != end) {
        ++i; // skip last colon
      }
      break;
    }
    // check for separator - yes last check must be (i != end)
    if (!((*i++ == ':') && (i != end))) { // skip separator
      return false;
    }
    if (!zeroCompression && (*i == ':')) { // check for zero-compression
      ++i; // skip compression separator
      zeroCompression = true;
      --maxWords;
    }
  }
  if (!((i == endIPv6) && (zeroCompression || (words == maxWords)))) {
    return false;
  }
  
  if (readIPv4) { // should we read an IPv4 address
    unsigned int words = 0;
    // get 4 decimal values
    while ((words < 4) && (i < end) && ASCIITraits::isDigit(*i)) {
      // overflow not possible with 3 digits [0;999]
      unsigned int value = ASCIITraits::digitToValue(*i++);
      if ((i < end) && ASCIITraits::isDigit(*i)) {
        value = 10 * value + ASCIITraits::digitToValue(*i++);
        if ((i < end) && ASCIITraits::isDigit(*i)) {
          value = 10 * value + ASCIITraits::digitToValue(*i++);
        }
      }
      if (value > 255) {
        return false;
      }
      if (i == end) {
        break;
      }
      if (!((i < end) && (*i++ == '.'))) {
        return false;
      }
    }
    if (words != 4) {
      return false;
    }
  }
  return i == end;
}

bool InetAddress::parse(const String& address) throw() {
  String::ReadIterator i = address.getBeginReadIterator();
  const String::ReadIterator end = address.getEndReadIterator();

  int colon = address.indexOf(':');
  bool readIPv4 = true; // read IPv4 address from end of addr

  if (colon < 0) { // IPv4
    family = IP_VERSION_4;
    this->address.words[0] = 0;
    this->address.words[1] = 0;
    this->address.words[2] = 0;
  } else { // IPv6
    family = IP_VERSION_6;
    int dot = address.indexOf('.', colon + 1);
    readIPv4 = (dot >= 0);
    const String::ReadIterator endIPv6 = (dot >= 0) ? (i + address.lastIndexOf(':', dot) + 1) : end;

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
      this->address.halfWords[writeHead++] = ByteOrder::toBigEndian<uint16>(value);

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
      uint16* dest = &reinterpret_cast<uint16*>(&this->address)[zeroIndex + maxValues - writeHead];
      uint16* src = &reinterpret_cast<uint16*>(&this->address)[zeroIndex];
      move(dest, src, writeHead - zeroIndex);
      fill<uint16>(src, maxValues - writeHead, 0);
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
      this->address.octets[12 + relativeIndex++] = value;
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

InetAddress::InetAddress() throw() {
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  family = IP_VERSION_6;
#else
  family = IP_VERSION_4;
#endif
  address.words[0] = 0; // set to unspecified address
  address.words[1] = 0;
  address.words[2] = 0;
  address.words[3] = 0;
}

InetAddress::InetAddress(const uint8* address, Family family) throw() {
  setAddress(address, family);
}

InetAddress::InetAddress(const String& address) throw(InvalidFormat) {
  bassert(parse(address), InvalidFormat("Not an Internet address", this));
}

InetAddress::InetAddress(const String& address, Family family) throw(InvalidFormat) {
  bassert(
    parse(address) && (this->family == family),
    InvalidFormat("Not an Internet address", this)
  );
}

InetAddress::InetAddress(const InetAddress& copy) throw()
  : family(copy.family),
    address(copy.address)
{
}

InetAddress& InetAddress::operator=(const InetAddress& assign) throw() {
  if (&assign != this) {
    family = assign.family;
    address = assign.address;
  }
  return *this;
}

String InetAddress::getHostName(bool fullyQualified) const throw(HostNotFound) {
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  struct sockaddr_in6 addr;
  clear(addr);
#if (defined(SIN6_LEN))
  addr.sin6_len = sizeof(addr);
#endif
  addr.sin6_family = AF_INET6;
  copy<uint8>(
    Cast::getAddress(addr.sin6_addr),
    (const uint8*)&address, // TAG: Cast::getAddress(address),
    sizeof(address)
  );
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
    throw HostNotFound("Unable to resolve IP address.", this);
  }

  return NativeString(hostname);
#else // use ordinary BSD sockets - IPv4
  struct hostent* hp = nullptr;

  if ((family == IP_VERSION_6) &&
      ((address.words[0] != 0) ||
       (address.words[1] != 0) ||
       ((address.words[2] != ByteOrder::toBigEndian<uint32>(0xffffU)) &&
        (address.words[3] != 0)))) {
    throw HostNotFound("Unable to resolve IP address.", this);
  }
  
#  if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    hp = gethostbyaddr(
      Cast::getCharAddress(address.words[3]),
      sizeof(address.words[3]),
      AF_INET
    ); // MT-safe
    bassert(hp, HostNotFound("Unable to resolve IP address", this));
#  elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
         (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS))
    struct hostent result;
    char buffer[1024]; // how big should this buffer be
    int error = 0;
    hp = gethostbyaddr_r(
      Cast::getCharAddress(address.words[3]),
      sizeof(address.words[3]),
      AF_INET,
      &result,
      buffer,
      sizeof(buffer),
      &error
    );
    bassert(hp, HostNotFound("Unable to resolve IP address", this));
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    struct hostent result;
    char buffer[1024]; // how big should this buffer be
    int error = 0;
    if (gethostbyaddr_r(
          Cast::getCharAddress(address.words[3]),
          sizeof(address.words[3]),
          AF_INET,
          &result,
          buffer,
          sizeof(buffer),
          &hp,
          &error)) {
      throw HostNotFound("Unable to resolve IP address.", this);
    }
#  else
//    #warning gethostbyaddr is not MT-safe
    if (!(hp = gethostbyaddr(Cast::getCharAddress(address.words[3]), sizeof(address.words[3]), AF_INET))) {
      throw HostNotFound("Unable to resolve IP address.", this);
    }
#  endif

  return NativeString(hp->h_name);
#endif // _COM_AZURE_DEV__BASE__INET_IPV6
}

bool InetAddress::operator==(const InetAddress& compare) const throw()
{
  return (address.words[0] == compare.address.words[0]) &&
    (address.words[1] == compare.address.words[1]) &&
    (address.words[2] == compare.address.words[2]) &&
    (address.words[3] == compare.address.words[3]);
}

bool InetAddress::isSynonymous(const InetAddress& eq) const throw()
{
  if (family == eq.family) {
    return (address.words[3] == eq.address.words[3]) &&
      (address.words[2] == eq.address.words[2]) &&
      (address.words[1] == eq.address.words[1]) &&
      (address.words[0] == eq.address.words[0]);
  } else if (family == IP_VERSION_6) {
    if ((address.words[3] == 0) && // is unspecified
        (address.words[2] == 0) &&
        (address.words[1] == 0) &&
        (address.words[0] == 0)) {
      return eq.address.words[3] == 0; // is unspecified IPv4 address
    } else if ((address.words[0] == 0) && // is IPv4 mapped
               (address.words[1] == 0) &&
               (address.words[2] == ByteOrder::toBigEndian<uint32>(0xffffU)) &&
               (address.words[3] != 0)) {
      return address.words[3] == eq.address.words[3];
    } else {
      return false;
    }
  } else { // IP_VERSION_4
    if ((eq.address.words[3] == 0) && // is unspecified
        (eq.address.words[2] == 0) &&
        (eq.address.words[1] == 0) &&
        (eq.address.words[0] == 0)) {
      return address.words[3] == 0; // is unspecified IPv4 address
    } else if ((eq.address.words[0] == 0) && // is IPv4 mapped
               (eq.address.words[1] == 0) &&
               (eq.address.words[2] == ByteOrder::toBigEndian<uint32>(0xffffU)) &&
               (eq.address.words[3] != 0)) {
      return address.words[3] == eq.address.words[3];
    } else {
      return false;
    }
  }
}

bool InetAddress::isUnspecified() const throw() {
  // ok for both IPv4 and IPv6
  return (address.words[0] == 0) &&
    (address.words[1] == 0) &&
    (address.words[2] == 0) &&
    (address.words[3] == 0);
}

bool InetAddress::isLoopback() const throw() {
  if (family == IP_VERSION_6) {
    return (address.words[0] == 0) && (address.words[1] == 0) && (address.words[2] == 0) &&
      (address.words[3] == ByteOrder::toBigEndian<uint32>(0x00000001U));
  } else {
    return address.words[3] == ByteOrder::toBigEndian<uint32>(0x7f000001U); // IPv4 only
  }
}

bool InetAddress::isMulticast() const throw() {
  return ((family == IP_VERSION_6) && (address.octets[0] == 0xff)) ||
    ((family == IP_VERSION_4) && ((address.octets[12] & 0xf0) == 0xe0)); // class d
}

bool InetAddress::isLinkLocal() const throw() {
  if (family == IP_VERSION_6) {
    return (address.words[0] & ByteOrder::toBigEndian<uint32>(0xffc00000U)) ==
      ByteOrder::toBigEndian<uint32>(0xfe800000U);
  } else {
    return false;
  }
}

bool InetAddress::isSiteLocal() const throw() {
  return (family == IP_VERSION_6) &&
    ((address.words[0] & ByteOrder::toBigEndian<uint32>(0xffc00000U)) ==
     ByteOrder::toBigEndian<uint32>(0xfec00000U));
}

bool InetAddress::isIPv4Mapped() const throw() {
  return (family == IP_VERSION_6) &&
    (address.words[0] == 0) &&
    (address.words[1] == 0) &&
    (address.words[2] == ByteOrder::toBigEndian<uint32>(0xffffU)) &&
    (address.words[3] != 0);
}

bool InetAddress::isIPv4Compatible() const throw() {
  // ok for both IPv4 and IPv6
  return (address.words[0] == 0) &&
    (address.words[1] == 0) &&
    (address.words[2] == 0) &&
    (address.words[3] != 0);
}

unsigned int InetAddress::getType() const throw() {
  unsigned int result = (family == IP_VERSION_4) ? IPV4 : IPV6;
  if (family == IP_VERSION_4) {
    if (address.words[3] == 0) {
      result |= UNSPECIFIED;
    } else if (address.words[3] == ByteOrder::toBigEndian<uint32>(0x7f000001U)) {
      result |= LOOPBACK;
    }
    if (address.octets[12] < 0x80) {
      result |= CLASS_A;
    } else if (address.octets[12] < 0xc0) {
      result |= CLASS_B;
    } else if (address.octets[12] < 0xe0) {
      result |= CLASS_C;
    } else if (address.octets[12] < 0xf0) {
      result |= CLASS_D | MULTICAST;
    } else {
      result |= CLASS_E;
    }
    result |= IPV4_COMPATIBLE;
  } else { // IPv6
    if ((address.words[0] == 0) && (address.words[1] == 0)) {
      if (address.words[2] == 0) {
        if (address.words[3] == 0) {
          result |= UNSPECIFIED;
        } else if (address.words[3] == ByteOrder::toBigEndian<uint32>(0x00000001U)) {
          result |= LOOPBACK;
          result |= IPV4_COMPATIBLE;
        } else {
          result |= IPV4_COMPATIBLE;
        }
      } else if ((address.words[2] == ByteOrder::toBigEndian<uint32>(0x0000ffffU)) && (address.words[3] != 0)) {
        result |= IPV4_MAPPED;
      }
    } else if (address.octets[0] == 0xff) {
      result |= MULTICAST;
    } else if ((address.words[0] & ByteOrder::toBigEndian<uint32>(0xffc00000U)) == ByteOrder::toBigEndian<uint32>(0xfe800000U)) {
      result |= LINK_LOCAL;
    } else if ((address.words[0] & ByteOrder::toBigEndian<uint32>(0xffc00000U)) == ByteOrder::toBigEndian<uint32>(0xfec00000U)) {
      result |= SITE_LOCAL;
    }
  }
  return result;
}

bool InetAddress::convertToIPv6() throw() {
  if (family == IP_VERSION_4) {
    family = IP_VERSION_6;
    if (!isUnspecified()) { // leave unspecified
      address.words[2] = ByteOrder::toBigEndian<uint32>(0x0000ffffU);
    }
  }
  return true; // cannot fail
}

bool InetAddress::convertToIPv4() throw() {
  if (family == IP_VERSION_6) {
    if (isIPv4Mapped()) {
      family = IP_VERSION_4;
      address.words[2] = 0;
    } else if (isUnspecified()) {
      family = IP_VERSION_4;
    } else {
      return false; // cannot convert
    }
  }
  return true; // now IPv4 address
}

void InetAddress::setAddress(const uint8* address, Family family) throw() {
  this->family = family;
  switch (family) {
  case IP_VERSION_4:
    this->address.words[0] = 0;
    this->address.words[1] = 0;
    this->address.words[2] = 0;
    this->address.octets[12] = *address++;
    this->address.octets[13] = *address++;
    this->address.octets[14] = *address++;
    this->address.octets[15] = *address++;
    break;
  case IP_VERSION_6:
    copy(this->address.octets, address, 16);
    break;
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value)
{
  // char buffer[sizeof("ffff:ffff:ffff:ffff:ffff:ffff:123.123.123.123")];
  // TAG: do not write directly to stream: use internal stream first
  if (value.family == InetAddress::IP_VERSION_6) {
    unsigned int type = value.getType();
    if (type & InetAddress::IPV4_MAPPED) { // ::ffff:255.255.255.255
      stream << "::ffff:"; // write IPv4 address after this
    } else if ((type & (InetAddress::IPV4_COMPATIBLE|InetAddress::LOOPBACK)) == InetAddress::IPV4_COMPATIBLE) { // ::255.255.255.255
      stream << "::"; // write IPv4 address after this
    } else { // 1080::8:800:200c:417a
      const uint16* addr = reinterpret_cast<const uint16*>(&value.address);

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
        stream << NOPREFIX << HEX << ByteOrder::fromBigEndian<uint16>(addr[i++]);
        if (i < firstZero) {
          stream << ':';
        }
      }
      if (firstZero < endZero) { // are zeros present
        stream << "::";
        for (int i = endZero; i < 8; ++i) { // write values after zeros
          if (i > endZero) {
            stream << ':';
          }
          stream << NOPREFIX
                 << HEX << ByteOrder::fromBigEndian<uint16>(addr[i]);
        }
      }
      return stream; // do not write IPv4 address
    }
  }

  const uint8* addr = value.getIPv4Address();
  return stream << DEC << addr[0] << '.'
                << DEC << addr[1] << '.'
                << DEC << addr[2] << '.'
                << DEC << addr[3];
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(InetAddress) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/net");
  TEST_IMPACT(PRIVACY);

  void run() override
  {
    InetAddress a1("127.0.0.1");
    InetAddress a2("172.30.33.14");
    InetAddress a3("::ffff:172.30.33.14");
    InetAddress a4("0.0.0.0");
    InetAddress a5("::1");
    TEST_ASSERT(a4.isUnspecified());
    TEST_ASSERT(a3.isIPv4Mapped());
    TEST_ASSERT(a1.isLoopback());
    TEST_ASSERT(a5.isLoopback());
    TEST_ASSERT(a2.convertToIPv6());
    TEST_ASSERT(a3.convertToIPv4());
  }
};

TEST_REGISTER(InetAddress);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
