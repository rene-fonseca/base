/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InetService.h"
#include <netdb.h>
#include <netinet/in.h>

unsigned short InetService::getByName(const String<>& name, const String<>& protocol) throw() {
#ifdef __linux__
  struct servent result;
  struct servent* pResult;
  char buffer[1024]; // how big should this buffer be
  getservbyname_r((const char*)name, (const char*)protocol, &result, buffer, sizeof(buffer), &pResult);
  return pResult ? ntohs(result.s_port) : 0;
#elif __solaris__
  struct servent result;
  struct servent* pResult;
  char buffer[1024]; // how big should this buffer be
  pResult = getservbyname_r((const char*)name, (const char*)protocol, &result, buffer, sizeof(buffer));
  return pResult ? ntohs(pResult->s_port) : 0;
#elif __CYGWIN__
  // WARNING: NOT MT-SAFE
  struct servent* pResult;
  pResult = getservbyname((const char*)name, (const char*)protocol);
  return pResult ? ntohs(pResult->s_port) : 0;
#else
  #err "Operating system not supported"
#endif
}

String<> InetService::getByPort(unsigned short port, const String<>& protocol) throw() {
#ifdef __linux__
  struct servent result;
  struct servent* pResult;
  char buffer[1024]; // how big should this buffer be
  getservbyport_r(htons(port), (const char*)protocol, &result, buffer, sizeof(buffer), &pResult);
  return pResult ? String<>(result.s_name) : String<>();
#elif __solaris__
  struct servent result;
  struct servent* pResult;
  char buffer[1024]; // how big should this buffer be
  pResult = getservbyport_r(htons(port), (const char*)protocol, &result, buffer, sizeof(buffer));
  return pResult ? String<>(pResult->s_name) : String<>();
#elif __CYGWIN__
  // WARNING: NOT MT-SAFE
  struct servent* pResult;
  pResult = getservbyport(htons(port), (const char*)protocol);
  return pResult ? String<>(pResult->s_name) : String<>();
#else
  #err "Operating system not supported"
#endif
}

InetService::InetService(const String<>& name, const String<>& protocol) throw(ServiceNotFound) {
  if ((port = getByName(name, protocol)) == 0) {
    throw ServiceNotFound("Unable to resolve service by name");
  }
  this->name = name;
  this->protocol = protocol;
}

InetService::InetService(unsigned short port, const String<>& protocol) throw(ServiceNotFound) {
  if ((name = getByPort(port, protocol)) == "") {
    throw ServiceNotFound("Unable to resolve service by port");
  }
  this->port = port;
  this->protocol = protocol;
}

InetService::InetService(const InetService& copy) throw() {
  name = copy.name;
  port = copy.port;
  protocol = copy.protocol;
}

InetService& InetService::operator=(const InetService& eq) throw() {
  if (&eq != this) { // protect against self assignment
    name = eq.name;
    port = eq.port;
    protocol = eq.protocol;
  }
  return *this;
}

const String<>& InetService::getName() const throw() {
  return name;
}

unsigned short InetService::getPort() const throw() {
  return port;
}

const String<>& InetService::getProtocol() const throw() {
  return protocol;
}

InetService::~InetService() throw() {
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetService& value) {
  return stream << "class/InetService{"
                << "name=" << value.name << ","
                << "port=" << value.port << ","
                << "protocol=" << value.protocol
                << "}";
}
