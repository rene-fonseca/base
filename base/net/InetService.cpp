/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/InetService.h>

#if defined(__win32__)
  #include <winsock.h>
#else // __unix__
  #include <netdb.h>
  #include <netinet/in.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

unsigned short InetService::getByName(const String& name, const String& protocol) throw() {
  struct servent* sp;
#if defined(__win32__)
  sp = getservbyname(name.getElements(), protocol.getElements()); // MT-safe
#elif defined(__sgi__) || defined(__solaris__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyname_r(name.getElements(), protocol.getElements(), &result, buffer, sizeof(buffer));
#elif defined(__linux__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyname_r(name.getElements(), protocol.getElements(), &result, buffer, sizeof(buffer), &sp);
#else
  #warning Using MT-unsafe getservbyname
  sp = getservbyname(name.getElements(), protocol.getElements());
#endif
  return sp ? ntohs(sp->s_port) : 0;
}

String InetService::getByPort(unsigned short port, const String& protocol) throw() {
  struct servent* sp;
#if defined(__win32__)
  sp = getservbyport(htons(port), protocol.getElements()); // MT-safe
#elif defined(__sgi__) || defined(__solaris__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  sp = getservbyport_r(htons(port), protocol.getElements(), &result, buffer, sizeof(buffer));
#elif defined(__linux__)
  struct servent result;
  char buffer[1024]; // how big should this buffer be
  getservbyport_r(htons(port), protocol.getElements(), &result, buffer, sizeof(buffer), &sp);
#else
  #warning Using MT-unsafe getservbyport
  sp = getservbyport(htons(port), protocol.getElements());
#endif
  return sp ? String(sp->s_name) : String();
}

InetService::InetService(const String& name, const String& protocol) throw(ServiceNotFound) {
  if ((port = getByName(name.getElements(), protocol.getElements())) == 0) {
    throw ServiceNotFound("Unable to resolve service by name");
  }
  this->name = name;
  this->protocol = protocol;
}

InetService::InetService(unsigned short port, const String& protocol) throw(ServiceNotFound) {
  if ((name = getByPort(port, protocol.getElements())) == "") {
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

const String& InetService::getName() const throw() {
  return name;
}

unsigned short InetService::getPort() const throw() {
  return port;
}

const String& InetService::getProtocol() const throw() {
  return protocol;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const InetService& value) {
  return stream << "class/InetService{"
                << "name=" << value.name << ","
                << "port=" << value.port << ","
                << "protocol=" << value.protocol
                << "}";
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
