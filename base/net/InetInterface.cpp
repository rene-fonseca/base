/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/net/InetInterface.h>
#include <base/concurrency/Thread.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#elif defined(_DK_SDU_MIP__BASE__INET_IPV6)
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <net/if.h>
#else // Unix
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <net/if.h>
  #include <sys/ioctl.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

List<InetInterface> InetInterface::getInetInterfaces() throw(NetworkException) {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  List<InetInterface> interfaces;
  struct if_nameindex* ni;
  if ((ni = if_nameindex()) == NULL) { // MT-safe
    throw NetworkException("Unable to get interfaces");
  }
  interfaces.append(InetInterface(ni->if_index, ni->if_name));
  if_freenameindex(ni); // MT-safe
  return interfaces;
#elif (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__GNULINUX)
  List<InetInterface> interfaces;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  try {
    struct ifconf ifc;
    ifc.ifc_len = Thread::getLocalStorage()->getSize();
    ifc.ifc_buf = Thread::getLocalStorage()->getElements();
    if (ioctl(handle, SIOCGIFCONF, &ifc)) {
      throw NetworkException("Unable to get interfaces");
    }

    struct ifreq* current = ifc.ifc_req;
    int offset = 0;
    while (offset < ifc.ifc_len) {
      if (ioctl(handle, SIOCGIFINDEX, current)) {
        throw NetworkException("Unable to get interfaces");
      }
      interfaces.append(InetInterface(current->ifr_ifindex, current->ifr_name));
      ++current;
      offset += sizeof(struct ifreq);
    }
  } catch(...) {
    close(handle);
    throw;
  }
  close(handle);
  return interfaces;
#else
  #warning getInetInterfaces not implemented
  return List<InetInterface>();
#endif
}

InetInterface::InetInterface(unsigned int i, const char* n) : index(i), name(n) {
}

InetInterface::InetInterface(const InetInterface& copy) throw() : index(copy.index), name(copy.name) {
}

unsigned int InetInterface::getIndex() const throw() {
  return index;
}

String InetInterface::getName() const throw() {
  return name;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
