/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <config.h>
#include <base/net/InetInterface.h>

#if defined(HAVE_INET_IPV6)
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <net/if.h>
#endif

List<InetInterface> InetInterface::getInetInterfaces() throw(NetworkException) {
#if defined(HAVE_INET_IPV6)
  List<InetInterface> interfaces;
  struct if_nameindex* ni;
  if ((ni = if_nameindex()) == NULL) { // MT-safe
    throw NetworkException("Unable to get interfaces");
  }
  interfaces.append(InetInterface(ni->if_index, ni->if_name));
  if_freenameindex(ni); // MT-safe
  return interfaces;
#else
  #warning getInetInterfaces not implemented
#endif
}

InetInterface::InetInterface(unsigned int i, const char* n) : index(i), name(n) {
}

InetInterface::InetInterface(const InetInterface& copy) throw() : index(copy.index), name(copy.name) {
}

unsigned int InetInterface::getIndex() const throw() {
  return index;
}

const String<>& InetInterface::getName() const throw() {
  return name;
}
