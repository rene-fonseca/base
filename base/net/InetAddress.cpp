/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <config.h>
#include <base/net/InetAddress.h>
#include <base/Functor.h>
#include <base/concurrency/Thread.h>

#if defined(__win32__) // temporary solution until arch independant types have been defined
  typedef DWORD uint32_t;
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h> // may define MAXHOSTNAMELEN (linux, irix)
#include <netinet/in.h> // define IP address
#include <netdb.h> // may define MAXHOSTNAMELEN (solaris)
#include <arpa/inet.h> // defines inet_ntop...
#include <unistd.h> // defines gethostname

String<> InetAddress::getLocalHost() {
  char name[MAXHOSTNAMELEN + 1]; // does MAXHOSTNAMELEN include terminator
  gethostname(name, sizeof(name));
  return String<>(name);
}

List<InetAddress> InetAddress::getAddressesByName(const String<>& name) throw(HostNotFound) {
  List<InetAddress> result;

#if defined(HAVE_INET_IPV6)
  struct addrinfo hint;
  fill<char>((char*)&hint, sizeof(hint), 0);
  hint.ai_family = PF_UNSPEC;

  struct addrinfo* ai;
  if (getaddrinfo((const char*)name, NULL, &hint, &ai) != 0) { // MT-level is safe
    throw HostNotFound("Unable to lookup host by name");
  }

  struct addrinfo* i = ai;
  while (i) {
    char* addr;
    switch (i->ai_family) {
    case PF_INET:
      addr = (char*)&((struct sockaddr_in*)(i->ai_addr))->sin_addr;
      result.append(InetAddress(addr, IPv4));
      break;
    case PF_INET6:
      addr = (char*)&((struct sockaddr_in6*)(i->ai_addr))->sin6_addr;
      result.append(InetAddress(addr, IPv6));
      break;
    default:
      throw NetworkException("Unsupported family");
    }
    i = i->ai_next; // go to the next info entry
  }

  freeaddrinfo(ai); // release resources - MT-level is safe
#else
  struct hostent* hp;

  #if defined(__sgi__) || defined(__solaris__)
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (!(hp = gethostbyname_r((const char*)name, &h, buffer, sizeof(buffer), &error))) {
      throw HostNotFound("Unable to lookup host by name");
    }
  #elif defined(__linux__)
    struct hostent h;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (gethostbyname_r((const char*)name, &h, buffer, sizeof(buffer), &hp, &error)) {
      throw HostNotFound("Unable to lookup host by name");
    }
  #else
    #warning gethostbyname is not MT-safe
    if (!(hp = gethostbyname((const char*)name))) {
      throw HostNotFound("Unable to lookup host by name");
    }
  #endif

  for (char** p = hp->h_addr_list; *p != 0; p++) {
    result.append(InetAddress(*p, IPv4));
  }
#endif // HAVE_INET_IPV6
  return result;
}

InetAddress::InetAddress() throw() {
  fill<char>((char*)&address, sizeof(address), 0); // set to unspecified addr
}

InetAddress::InetAddress(const char* addr, Family family) throw(NetworkException) {
  setAddress(addr, family);
}

InetAddress::InetAddress(const String<>& addr) throw(InvalidFormat) {
#if defined(HAVE_INET_IPV6)
  struct in_addr temp;
  if (inet_pton(AF_INET, addr.getBytes(), &temp) > 0) { // try IPv4 format - MT-level is safe
    // make IPv4-mapped IPv6 address (network byte order)
    ((uint32_t*)(&address))[0] = 0x00000000;
    ((uint32_t*)(&address))[1] = 0x00000000;
    ((uint32_t*)(&address))[2] = htonl(0x0000ffff);
    ((uint32_t*)(&address))[3] = ((uint32_t*)(&temp))[0]; // temp is in network byte order
  } else if (inet_pton(AF_INET6, addr.getBytes(), &address) > 0) { // try IPv6 format - MT-level is safe
    // IPv6 address already copied to address field
  } else {
    throw InvalidFormat("Not a valid IP address");
  }
#else
  struct in_addr temp;
  if ((temp.s_addr = inet_addr(addr.getBytes())) == -1) { // MT-level is safe???
    throw InvalidFormat("Not a valid IPv4 address");
  }
  ((uint32_t*)(&address))[0] = ((uint32_t*)(&temp))[0];
#endif // HAVE_INET_IPV6
}

InetAddress::InetAddress(const InetAddress& copy) throw() : address(copy.address) {
}

InetAddress& InetAddress::operator=(const InetAddress& eq) throw() {
  if (&eq != this) { // protect against self assignment
    address = eq.address;
  }
  return *this;
}

const char* InetAddress::getAddress() const throw() {
  return (char*)&address;
}

String<> InetAddress::getHostName(bool fullyQualified) const throw(HostNotFound) {
#if defined(HAVE_INET_IPV6)
  struct sockaddr_in6 addr;
  fill<char>((char*)&addr, sizeof(addr), 0);
#if defined(SIN6_LEN)
  addr.sin6_len = sizeof(addr);
#endif
  addr.sin6_family = AF_INET6;
  copy<char>((char*)&addr.sin6_addr, (const char*)&address, sizeof(address));
  char hostname[NI_MAXHOST]; // includes space for terminator

  if (getnameinfo((sockaddr*)&addr, sizeof(addr), hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD | (fullyQualified ? 0 : NI_NOFQDN)) != 0) {
    throw HostNotFound("Unable to resolve IP address");
  }

  return String<>(hostname);
#else
  struct hostent* hp;

  #if defined(__sgi__) || defined(__solaris__)
    struct hostent result;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (!(hp = gethostbyaddr_r(&address, sizeof(address), AF_INET, &result, buffer, sizeof(buffer), &error))) {
      throw HostNotFound("Unable to resolve IP address");
    }
  #elif defined(__linux__)
    struct hostent result;
    char buffer[1024]; // how big should this buffer be
    int error;
    if (gethostbyaddr_r(&address, sizeof(address), AF_INET, &result, buffer, sizeof(buffer), &hp, &error)) {
      throw HostNotFound("Unable to resolve IP address");
    }
  #else
    #warning gethostbyaddr is not MT-safe
    if (!(hp = gethostbyaddr((const char*)&address, sizeof(address), AF_INET))) {
      throw HostNotFound("Unable to resolve IP address");
    }
  #endif

  return String<>(hp->h_name);
#endif // HAVE_INET_IPV6
}

bool InetAddress::operator==(const InetAddress& eq) throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_ARE_ADDR_EQUAL((struct in6_addr*)&address, (struct in6_addr*)&eq.address);
#else
  return address.buffer == eq.address.buffer;
#endif // HAVE_INET_IPV6
}

bool InetAddress::isUnspecified() const throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_IS_ADDR_UNSPECIFIED((struct in6_addr*)&address);
#else
  return (uint32_t)address.buffer == htonl(0x00000000);
#endif // HAVE_INET_IPV6
}

bool InetAddress::isLoopback() const throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_IS_ADDR_LOOPBACK((struct in6_addr*)&address);
#else
  return (uint32_t)address.buffer == htonl(0x7f000001);
#endif // HAVE_INET_IPV6
}

bool InetAddress::isMulticast() const throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_IS_ADDR_MULTICAST((struct in6_addr*)&address);
#else
  return false;
#endif // HAVE_INET_IPV6
}

bool InetAddress::isLinkLocal() const throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_IS_ADDR_LINKLOCAL((struct in6_addr*)&address);
#else
  return false;
#endif // HAVE_INET_IPV6
}

bool InetAddress::isSiteLocal() const throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_IS_ADDR_SITELOCAL((struct in6_addr*)&address);
#else
  return false;
#endif // HAVE_INET_IPV6
}

bool InetAddress::isV4Mapped() const throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_IS_ADDR_V4MAPPED((struct in6_addr*)&address);
#else
  return false;
#endif // HAVE_INET_IPV6
}

bool InetAddress::isV4Compatible() const throw() {
#if defined(HAVE_INET_IPV6)
  return IN6_IS_ADDR_V4COMPAT((struct in6_addr*)&address);
#else
  return true;
#endif // HAVE_INET_IPV6
}

void InetAddress::setAddress(const char* addr, Family family) throw(NetworkException) {
#if defined(HAVE_INET_IPV6)
  switch (family) {
  case IPv4:
    // make IPv4-mapped IPv6 address (network byte order)
    ((uint32_t*)(&address))[0] = 0x00000000;
    ((uint32_t*)(&address))[1] = 0x00000000;
    ((uint32_t*)(&address))[2] = htonl(0x0000ffff);
    ((uint32_t*)(&address))[3] = ((uint32_t*)addr)[0];
    break;
  case IPv6:
    ((uint32_t*)(&address))[0] = ((uint32_t*)addr)[0];
    ((uint32_t*)(&address))[1] = ((uint32_t*)addr)[1];
    ((uint32_t*)(&address))[2] = ((uint32_t*)addr)[2];
    ((uint32_t*)(&address))[3] = ((uint32_t*)addr)[3];
    break;
  }
#else
  switch (family) {
  case IPv4:
    *(uint32_t*)&address = *(uint32_t*)addr;
    break;
  case IPv6:
    throw NetworkException("Operating system does not support IPv6");
    break;
  }
#endif // HAVE_INET_IPV6
}

FormatOutputStream& InetAddress::toFormatStream(FormatOutputStream& stream) const {
#if defined(HAVE_INET_IPV6)
  #if (INET6_ADDRSTRLEN > THREAD_LOCAL_STORAGE)
    #error The requested amount of local storage is not available.
  #endif
  char qwerty[1234];
  char* buffer = (char*)&qwerty;
//  char* buffer = Thread::getLocalStorage();
  if (isV4Mapped()) {
    inet_ntop(AF_INET, &((uint32_t*)(&address))[3], buffer, THREAD_LOCAL_STORAGE); // MT-level is safe
  } else {
    inet_ntop(AF_INET6, &address, buffer, THREAD_LOCAL_STORAGE); // MT-level is safe
  }
  return stream << buffer;
/*  char buffer[INET6_ADDRSTRLEN]; // longest possible string is "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"
  if (isV4Mapped()) {
    inet_ntop(AF_INET, &((uint32_t*)(&address))[3], buffer, sizeof(buffer)); // MT-level is safe
  } else {
    inet_ntop(AF_INET6, &address, buffer, sizeof(buffer)); // MT-level is safe
  }
  return stream << buffer;*/
#else
  // longest possible string is "255.255.255.255"
  return stream << inet_ntoa(*(struct in_addr*)&address); // MT-level is safe but uses static buffer
#endif // HAVE_INET_IPV6
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value) {
  return value.toFormatStream(stream);
}
