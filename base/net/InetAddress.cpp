/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InetAddress.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // define IP address
#include <netdb.h> // defines getipnodebyname...
#include <arpa/inet.h> // defines inet_ntop...
#include <unistd.h> // defines gethostname
#include <string.h>

String InetAddress::getLocalHost() {
  char name[NI_MAXHOST]; // MAXHOSTNAMELEN was not define in netdb.h
  gethostname(name, sizeof(name));
  return String(name);
}
/*
void InetAddress::getAddressesByName(const String& name, Collection<InetAddress>& collection) throw(HostNotFound) {
  int errorCode;
  if ((struct hostent* hp = getipnodebyname(name, AF_INET6, AI_DEFAULT, &errorCode)) == NULL) {
    throw HostNotFound("Unable to look up IP address by name");
  }
  for (char** p = hp->h_addr_list; *p != NULL; ++p) {
    collection.add(InetAddress(*p)); // cast to (byte[16]*)
  }
  freehostent(hp);
}
*/

InetAddress::InetAddress() throw() {
  memset(&address, 0, 16); // unspecified address
}

InetAddress::InetAddress(const char* addr, Family family) throw() {
  switch (family) {
  case IPv4:
    // make IPv4-mapped IPv6 address (network byte order)
    ((uint32_t*)(&address))[0] = 0x00000000;
    ((uint32_t*)(&address))[1] = 0x00000000;
    ((uint32_t*)(&address))[2] = htonl(0x0000ffff);
    ((uint32_t*)(&address))[3] = ((uint32_t*)addr)[0];
    break;
  case IPv6:
    memcpy(&address, addr, 16);
    break;
  }
}

InetAddress::InetAddress(const String& addr) throw(InvalidFormat) {
  struct in_addr temp;
  if (inet_pton(AF_INET, addr.getBytes(), &temp) > 0) { // try IPv4 format
    // make IPv4-mapped IPv6 address (network byte order)
    ((uint32_t*)(&address))[0] = 0x00000000;
    ((uint32_t*)(&address))[1] = 0x00000000;
    ((uint32_t*)(&address))[2] = htonl(0x0000ffff);
    ((uint32_t*)(&address))[3] = ((uint32_t*)(&temp))[0]; // temp is in network byte order
  } else if (inet_pton(AF_INET6, addr.getBytes(), &address) > 0) { // try IPv6 format
    // IPv6 address already copied to address field
  } else {
    throw InvalidFormat("Not a valid IP address");
  }
}

InetAddress::InetAddress(const InetAddress& copy) throw() {
  memcpy(&address, &copy.address, sizeof(address));
}

InetAddress& InetAddress::operator=(const InetAddress& eq) throw() {
  if (&eq == this) { // protect against self assignment
    memcpy(&address, &eq.address, sizeof(address));
  }
  return *this;
}

String InetAddress::getHostName() const throw(HostNotFound) {
  // is the flags argument appropriate
/*  int errorCode;
  struct hostent* hp;
  if ((hp = getipnodebyaddr(address, sizeof(struct in6_addr), AF_INET6, 0, &errorCode)) == NULL) {
    throw HostNotFound("Unable to look up name by IP address");
  }
  String str(hp->h_name);
  freehostent(hp);
  return str;*/
  return String("NOT IMPLEMENTED");
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

const char* InetAddress::getBytes() const throw() {
  return (char*)&address;
}

FormatOutputStream& InetAddress::toStream(FormatOutputStream& stream) const {
  char buffer[INET6_ADDRSTRLEN]; // longest string is ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255
  if (isV4Mapped()) {
    inet_ntop(AF_INET, &((uint32_t*)(&address))[3], (char*)&buffer, sizeof(buffer)); // MT-level is safe
  } else {
    inet_ntop(AF_INET6, &address, (char*)&buffer, sizeof(buffer)); // MT-level is safe
  }
  return stream << buffer;
}

InetAddress::~InetAddress() throw() {
}
