/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <config.h>
#include "InetAddress.h"
#include "base/Functor.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // define IP address
#include <netdb.h>
#include <arpa/inet.h> // defines inet_ntop...
#include <unistd.h> // defines gethostname

String<> InetAddress::getLocalHost() {
  char name[NI_MAXHOST]; // MAXHOSTNAMELEN was not defined in netdb.h
  gethostname(name, sizeof(name));
  return String<>(name);
}

List<InetAddress> InetAddress::getAddressesByName(const String<>& name) throw(HostNotFound) {
  List<InetAddress> result;

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
      result.add(InetAddress(addr, IPv4));
      break;
    case PF_INET6:
      addr = (char*)&((struct sockaddr_in6*)(i->ai_addr))->sin6_addr;
      result.add(InetAddress(addr, IPv6));
      break;
    default:
      throw NetworkException("Unsupported family");
    }
    i = i->ai_next; // go to the next info entry
  }

  freeaddrinfo(ai); // release resources - MT-level is safe
  return result;
}

InetAddress::InetAddress() throw() {
  fill<char>((char*)&address, sizeof(address), 0); // set to the unspecified address
}

InetAddress::InetAddress(const char* addr, Family family) throw() {
  setAddress(addr, family);
}

InetAddress::InetAddress(const String<>& addr) throw(InvalidFormat) {
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
}

InetAddress::InetAddress(const InetAddress& copy) throw() : address(copy.address) {
}

InetAddress& InetAddress::operator=(const InetAddress& eq) throw() {
  if (&eq == this) { // protect against self assignment
    address = eq.address;
  }
  return *this;
}

const char* InetAddress::getAddress() const throw() {
  return (char*)&address;
}

String<> InetAddress::getHostName(bool fullyQualified) const throw(HostNotFound) {
  struct sockaddr_in6 addr;
  fill<char>((char*)&addr, sizeof(addr), 0);
#ifdef SIN6_LEN
  addr.sin6_len = sizeof(addr);
#endif
  addr.sin6_family = AF_INET6;
  copy<char>((char*)&addr.sin6_addr, (const char*)&address, sizeof(address));
  char hostname[NI_MAXHOST]; // includes space for terminator

  if (getnameinfo((sockaddr*)&addr, sizeof(addr), hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD | (fullyQualified ? 0 : NI_NOFQDN)) != 0) {
    throw HostNotFound("Unable to resolve IP address");
  }

  return String<>(hostname);
}

bool InetAddress::operator==(const InetAddress& eq) throw() {
  return IN6_ARE_ADDR_EQUAL(&address, &eq.address);
}

bool InetAddress::isUnspecified() const throw() {
  return IN6_IS_ADDR_UNSPECIFIED(&address);
}

bool InetAddress::isLoopback() const throw() {
  return IN6_IS_ADDR_LOOPBACK(&address);
}

bool InetAddress::isMulticast() const throw() {
  return IN6_IS_ADDR_MULTICAST(&address);
}

bool InetAddress::isLinkLocal() const throw() {
  return IN6_IS_ADDR_LINKLOCAL(&address);
}

bool InetAddress::isSiteLocal() const throw() {
  return IN6_IS_ADDR_SITELOCAL(&address);
}

bool InetAddress::isV4Mapped() const throw() {
  return IN6_IS_ADDR_V4MAPPED(&address);
}

bool InetAddress::isV4Compatible() const throw() {
  return IN6_IS_ADDR_V4COMPAT(&address);
}

void InetAddress::setAddress(const char* addr, Family family) throw() {
  switch (family) {
  case IPv4:
    // make IPv4-mapped IPv6 address (network byte order)
    ((uint32_t*)(&address))[0] = 0x00000000;
    ((uint32_t*)(&address))[1] = 0x00000000;
    ((uint32_t*)(&address))[2] = htonl(0x0000ffff);
    ((uint32_t*)(&address))[3] = ((uint32_t*)addr)[0];
    break;
  case IPv6:
    copy<char>((char*)&address, addr, sizeof(address));
    break;
  }
}

FormatOutputStream& InetAddress::toFormatStream(FormatOutputStream& stream) const {
  char buffer[INET6_ADDRSTRLEN]; // longest possible string is "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"
  if (isV4Mapped()) {
    inet_ntop(AF_INET, &((uint32_t*)(&address))[3], buffer, sizeof(buffer)); // MT-level is safe
  } else {
    inet_ntop(AF_INET6, &address, buffer, sizeof(buffer)); // MT-level is safe
  }
  return stream << buffer;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetAddress& value) {
  return value.toFormatStream(stream);
}
