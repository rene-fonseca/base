/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InetService.h"
#include <netdb.h>
#include <netinet/in.h>

unsigned short InetService::getByName(const String& name, const String& protocol) throw() {
  // declaration of getservbyname_r varies from platform to platform
  struct servent* result;
  char buffer[1024]; // how big should this buffer be
  getservbyname_r((const char*)name, (const char*)protocol, result, buffer, sizeof(buffer), &result);
  return (result != NULL) ? ntohs(result->s_port) : 0;
}

String InetService::getByPort(unsigned short port, const String& protocol) throw() {
  // declaration of getservbyport_r varies from platform to platform
  struct servent* result;
  char buffer[1024]; // how big should this buffer be
  getservbyport_r(htons(port), (const char*)protocol, result, buffer, sizeof(buffer), &result);
  return (result != NULL) ? String(result->s_name) : String();
}

InetService::InetService(const String& name, const String& protocol) throw(ServiceNotFound) {
  if ((port = getByName(name, protocol)) == 0) {
    throw ServiceNotFound("Unable to resolve service by name");
  }
  this->name = name;
  this->protocol = protocol;
}

InetService::InetService(unsigned short port, const String& protocol) throw(ServiceNotFound) {
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

const String& InetService::getName() const throw() {
  return name;
}

unsigned short InetService::getPort() const throw() {
  return port;
}

const String& InetService::getProtocol() const throw() {
  return protocol;
}

FormatOutputStream& InetService::toStream(FormatOutputStream& stream) const {
  return stream << "{name=" << name << ",port=" << port << ",protocol=" << protocol << "}";
}

InetService::~InetService() throw() {
}
