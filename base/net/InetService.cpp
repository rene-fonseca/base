/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/InetService.h>
#include <netdb.h>
#include <netinet/in.h>

unsigned short InetService::getByName(const String<>& name, const String<>& protocol) throw() {
  struct servent* sp;
#if defined(__sgi__) || defined(__solaris__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyname_r((const char*)name, (const char*)protocol, &result, buffer, sizeof(buffer));
#elif defined(__linux__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyname_r((const char*)name, (const char*)protocol, &result, buffer, sizeof(buffer), &sp);
#else
  #warning Using MT-unsafe getservbyname
  sp = getservbyname((const char*)name, (const char*)protocol);
#endif
  return sp ? ntohs(sp->s_port) : 0;
}

String<> InetService::getByPort(unsigned short port, const String<>& protocol) throw() {
  struct servent* sp;
#if defined(__sgi__) || defined(__solaris__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyport_r(htons(port), (const char*)protocol, &result, buffer, sizeof(buffer));
#elif defined(__linux__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyport_r(htons(port), (const char*)protocol, &result, buffer, sizeof(buffer), &sp);
#else
  #warning Using MT-unsafe getservbyport
  sp = getservbyport(htons(port), (const char*)protocol);
#endif
  return sp ? String<>(sp->s_name) : String<>();
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

InetService::InetService(const InetService& copy) throw() :
  name(copy.name), port(copy.port), protocol(copy.protocol) {
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
