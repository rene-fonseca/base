/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/net/InetInterface.h>
#include <base/concurrency/Thread.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <winsock2.h>
#elif 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <net/if.h>
#else // unix
#  if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
#    define BSD_COMP
#  endif
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <net/if.h>
#  include <sys/ioctl.h>
#  include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace internal {
  
  class InetInterface {
  public:
    
    static inline InetAddress getAddress(const struct sockaddr& address) throw() {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
      switch (address.sa_family) {
      case AF_INET:
        return InetAddress(
          Cast::getAddress(Cast::pointer<const struct sockaddr_in*>(&address)->sin_addr),
          InetAddress::IP_VERSION_4
        );
      case AF_INET6:
        return InetAddress(
          Cast::getAddress(Cast::pointer<const struct sockaddr_in6*>(&address)->sin6_addr),
          InetAddress::IP_VERSION_6
        );
      default:
        return InetAddress();
      }
#else
      if (address.sa_family == AF_INET) {
        return InetAddress(
          Cast::getAddress(Cast::pointer<const struct sockaddr_in*>(&address)->sin_addr),
          InetAddress::IP_VERSION_4
        );
      } else {
        return InetAddress();
      }
#endif
    }
  };
}; // end of namespace internal

HashTable<String, unsigned int> InetInterface::getInterfaceNames() throw() {
  HashTable<String, unsigned int> interfaces;
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct if_nameindex* ni;
  if ((ni = if_nameindex()) == 0) { // MT-safe
    throw NetworkException("Unable to get interfaces", Type::getType<InetInterface>());
  }
  try {
    interfaces.add(String(ni->if_name), ni->if_index);
  } catch (...) {
    if_freenameindex(ni); // MT-safe
    throw;
  }
#elif 0 && (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        bytesReturned,
        0
        0
      )) {
    close(handle);
    throw NetworkException("Unable to get interfaces", Type::getType<InetInterface>());
  }
  close(handle);
  const INTERFACE_INFO* interface =
    Cast::pointer<const INTERFACE_INFO*>(Thread::getLocalStorage()->getElements());
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(interfaces[0]);
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (sa.sin_family != AF_INET) {
      continue;
    }
    InetInterface i;
    
    StringOutputStream stream;
    stream << InetAddress(
      getAddress(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_addr),
      InetAddress::IP_VERSION_4
    ) << FLUSH;
    interfaces.append(InetInterface(index, stream.getString()));
    ++interface;
  }
#elif 1 || (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  try {
    struct ifconf ifc;
    ifc.ifc_len = Thread::getLocalStorage()->getSize();
    ifc.ifc_buf = Thread::getLocalStorage()->getElements();
    if (ioctl(handle, SIOCGIFCONF, &ifc)) {
      close(handle);
      throw NetworkException("Unable to get interfaces", Type::getType<InetInterface>());
    }
    struct ifreq* current = ifc.ifc_req;
    int offset = 0;
    while (offset < ifc.ifc_len) {
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
        interfaces.add(String(current->ifr_name, IFNAMSIZ), current->ifr_index);
      }
      ++current;
      offset += sizeof(struct ifreq);
    }
  } catch (...) {
    close(handle);
    throw;
  }
  close(handle);
#else
#  warning InetInterface::getInterfaceNames() not supported
#endif
  return interfaces;
}

List<InetInterface> InetInterface::getInterfaces() throw(NetworkException) {
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  List<InetInterface> interfaces;
  struct if_nameindex* ni;
  if ((ni = if_nameindex()) == 0) { // MT-safe
    throw NetworkException("Unable to get interfaces", Type::getType<InetInterface>());
  }
  InetInterface interface;
  interface.index = ni->if_index;
  interface.name = ni->if_name;
  interfaces.append(interface);
  if_freenameindex(ni); // MT-safe
  return interfaces;
#elif 0 && (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        bytesReturned,
        0
        0
      )) {
    close(handle);
    throw NetworkException("Unable to get interfaces", Type::getType<InetInterface>());
  }
  close(handle);
  const INTERFACE_INFO* interface =
    Cast::pointer<const INTERFACE_INFO*>(Thread::getLocalStorage()->getElements());
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(interfaces[0]);
  List<InetInterface> interfaces;
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (sa.sin_family != AF_INET) {
      continue;
    }
    InetInterface i;
    
    StringOutputStream stream;
    stream << InetAddress(
      getAddress(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_addr),
      InetAddress::IP_VERSION_4
    ) << FLUSH;
    interfaces.append(InetInterface(index, stream.getString()));
    ++interface;
  }
  return interfaces;
#elif 1 || (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  List<InetInterface> interfaces;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  int numberOfInterfaces;
  if (ioctl(handle, SIOCGIFNUM, &numberOfInterfaces) != 0) {
    close(handle);
    throw NetworkException("Unable to get interfaces", Type::getType<InetInterface>());
  }
  try {
    struct ifconf ifc;
    ifc.ifc_len = Thread::getLocalStorage()->getSize();
    ifc.ifc_buf = Thread::getLocalStorage()->getElements();
    if (ioctl(handle, SIOCGIFCONF, &ifc)) {
      close(handle);
      throw NetworkException("Unable to get interfaces", Type::getType<InetInterface>());
    }
    
    struct ifreq* current = ifc.ifc_req;
    int offset = 0;
    while (offset < ifc.ifc_len) {
      InetInterface interface;
      interface.name = String(current->ifr_name, IFNAMSIZ);
      unsigned int flags = 0;
      if (ioctl(handle, SIOCGIFFLAGS, current) == 0) {
        flags |= (current->ifr_flags & IFF_UP) ? InetInterface::UP : 0;
        flags |= (current->ifr_flags & IFF_LOOPBACK) ? InetInterface::LOOPBACK : 0;
        flags |= (current->ifr_flags & IFF_POINTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
        flags |= (current->ifr_flags & IFF_BROADCAST) ? InetInterface::BROADCAST : 0;
        flags |= (current->ifr_flags & IFF_MULTICAST) ? InetInterface::MULTICAST : 0;
        interface.flags = flags;
      }
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
        interface.index = current->ifr_index;
      }
      if (ioctl(handle, SIOCGIFADDR, current) == 0) {
        interface.address = internal::InetInterface::getAddress(current->ifr_addr);
      }
      if ((flags & InetInterface::BROADCAST) && (ioctl(handle, SIOCGIFBRDADDR, current) == 0)) {
        interface.broadcast = internal::InetInterface::getAddress(current->ifr_broadaddr);
      }
      if ((flags & InetInterface::POINT_TO_POINT) && (ioctl(handle, SIOCGIFDSTADDR, current) == 0)) {
        interface.destination = internal::InetInterface::getAddress(current->ifr_dstaddr);
      }
      if (ioctl(handle, SIOCGIFMETRIC, current) == 0) {
        interface.metric = current->ifr_metric;
      }
      if (ioctl(handle, SIOCGENADDR, current) == 0) {
        fout << HEX << (uint8)current->ifr_enaddr[0] << ' '
             << HEX << (uint8)current->ifr_enaddr[1] << ' '
             << HEX << (uint8)current->ifr_enaddr[2] << ' '
             << HEX << (uint8)current->ifr_enaddr[3] << ' '
             << HEX << (uint8)current->ifr_enaddr[4] << ' '
             << HEX << (uint8)current->ifr_enaddr[5] << ' ' << ENDL;
        interface.ethernet.setMAC48(Cast::pointer<const uint8*>(current->ifr_enaddr));
      }
      interfaces.append(interface);
      ++current;
      offset += sizeof(struct ifreq);
    }
  } catch (...) {
    close(handle);
    throw;
  }
  close(handle);
  return interfaces;
#else
#  warning InetInterface::getInterfaces() not supported
  return List<InetInterface>();
#endif
}

unsigned int InetInterface::getIndexByName(const String& name) throw(NetworkException) {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  unsigned int index = if_nametoindex(name.getElements());
  assert(index > 0, NetworkException("Unable to resolve interface", Type::getType<InetInterface>()));
  return index;
#else
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  int numberOfInterfaces;
  if (ioctl(handle, SIOCGIFNUM, &numberOfInterfaces) != 0) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (String(current->ifr_name, IFNAMSIZ) == name) {
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
        close(handle);
        return req.ifr_index;
      }
      break;
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
  throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
#endif
}

unsigned int InetInterface::getIndexByAddress(const InetAddress& address) throw(NetworkException) {
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  int numberOfInterfaces;
  if (ioctl(handle, SIOCGIFNUM, &numberOfInterfaces) != 0) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFADDR, current) == 0) {
      if (internal::InetInterface::getAddress(current->ifr_addr) == address) {
        if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
          close(handle);
          return current->ifr_index;
        }
        break;
      }
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
  throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
}

String InetInterface::getName(unsigned int index) throw(NetworkException) {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  char name[IFNAMSIZ];
  assert(
    if_indextoname(index, name) != 0,
    NetworkException("Unable to resolve interface", Type::getType<InetInterface>())
  );
  return String(name, IFNAMSIZ);
#else
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  int numberOfInterfaces;
  if (ioctl(handle, SIOCGIFNUM, &numberOfInterfaces) != 0) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
      if (current->ifr_index == index) {
        close(handle);
        return String(name, IFNAMSIZ);
      }
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
  throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
#endif
}

InetAddress InetInterface::getAddress(unsigned int index) throw(NetworkException) {
  struct ifreq req;
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  assert(
    if_indextoname(index, req.ifr_name) != 0,
    NetworkException("Unable to resolve interface", Type::getType<InetInterface>())
  );
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  if (ioctl(handle, SIOCGIFADDR, &req) != 0) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  close(handle);
  return internal::InetInterface::getAddress(req.ifr_addr);
#else
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  int numberOfInterfaces;
  if (ioctl(handle, SIOCGIFNUM, &numberOfInterfaces) != 0) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFADDR, current) == 0) {
      if (current->ifr_index == index) {
        close(handle);
        return internal::InetInterface::getAddress(req.ifr_addr);
      }
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
  throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
#endif
}

InetInterface::InetInterface() throw()
  : index(0), flags(0), metric(0) {
}

InetInterface::InetInterface(const String& name) throw(NetworkException)
  : index(0), flags(0), metric(0) {
  assert(name.getLength() <= IFNAMSIZ, NetworkException(this));
  struct ifreq req;
  copy(req.ifr_name, name.getElements(), name.getLength()); // TAG how to init this
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  this->name = name;
  if (ioctl(handle, SIOCGIFFLAGS, &req) == 0) {
    flags |= (req.ifr_flags & IFF_UP) ? InetInterface::UP : 0;
    flags |= (req.ifr_flags & IFF_LOOPBACK) ? InetInterface::LOOPBACK : 0;
    flags |= (req.ifr_flags & IFF_POINTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
    flags |= (req.ifr_flags & IFF_BROADCAST) ? InetInterface::BROADCAST : 0;
    flags |= (req.ifr_flags & IFF_MULTICAST) ? InetInterface::MULTICAST : 0;
  }
  if (ioctl(handle, SIOCGIFINDEX, &req) == 0) {
    index = req.ifr_index;
  }
  if (ioctl(handle, SIOCGIFADDR, &req) == 0) {
    address = internal::InetInterface::getAddress(req.ifr_addr);
  }
  if ((flags & InetInterface::BROADCAST) && (ioctl(handle, SIOCGIFBRDADDR, &req) == 0)) {
    broadcast = internal::InetInterface::getAddress(req.ifr_broadaddr);
  }
  if ((flags & InetInterface::POINT_TO_POINT) && (ioctl(handle, SIOCGIFDSTADDR, &req) == 0)) {
    destination = internal::InetInterface::getAddress(req.ifr_dstaddr);
  }
  if (ioctl(handle, SIOCGIFMETRIC, &req) == 0) {
    metric = req.ifr_metric;
  }
  if (ioctl(handle, SIOCGENADDR, &req) == 0) {
    ethernet.setMAC48(Cast::pointer<const uint8*>(req.ifr_enaddr));
  }
  close(handle);
}

InetInterface::InetInterface(const InetInterface& copy) throw()
  : index(copy.index),
    name(copy.name),
    flags(copy.flags),
    address(copy.address),
    broadcast(copy.broadcast),
    destination(copy.destination),
    metric(copy.metric) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
