/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/net/InetInterface.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <nb30.h>
#  undef interface
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
#  include <netinet/in.h>
#  include <sys/ioctl.h>
#  include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace internal {
  
  class InetInterface {
  public:

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    struct ASTAT {
      ADAPTER_STATUS status;
      NAME_BUFFER nameBuffer[NCBNAMSZ];
    };
#endif

    static inline InetAddress getAddress(const struct sockaddr& address) throw() {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
      switch (address.sa_family) {
      case AF_INET:
        return InetAddress(
          Cast::getAddress(
            Cast::pointer<const struct sockaddr_in*>(&address)->sin_addr
          ),
          InetAddress::IP_VERSION_4
        );
      case AF_INET6:
        return InetAddress(
          Cast::getAddress(
            Cast::pointer<const struct sockaddr_in6*>(&address)->sin6_addr
          ),
          InetAddress::IP_VERSION_6
        );
      default:
        return InetAddress();
      }
#else
      if (address.sa_family == AF_INET) {
        return InetAddress(
          Cast::getAddress(
            Cast::pointer<const struct sockaddr_in*>(&address)->sin_addr
          ),
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
    throw NetworkException(
      "Unable to get interfaces",
      Type::getType<InetInterface>()
    );
  }
  try {
    interfaces.add(String(ni->if_name), ni->if_index);
  } catch (...) {
    if_freenameindex(ni); // MT-safe
    throw;
  }
#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to get interfaces",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current =
    Cast::pointer<const INTERFACE_INFO*>(
      Thread::getLocalStorage()->getElements()
    );
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (Cast::pointer<struct sockaddr*>(&current->iiAddress)->sa_family != AF_INET) { // TAG: AF_INET6
      continue;
    }
    StringOutputStream stream;
    stream << index << FLUSH;
    interfaces.add(stream.getString(), index);
    ++current;
  }
#elif 1 || (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  try {
    struct ifconf ifc;
    ifc.ifc_len = Thread::getLocalStorage()->getSize();
    ifc.ifc_buf = reinterpret_cast<char*>(Thread::getLocalStorage()->getElements());
    if (ioctl(handle, SIOCGIFCONF, &ifc)) {
      close(handle);
      throw NetworkException(
        "Unable to get interfaces",
        Type::getType<InetInterface>()
      );
    }
    struct ifreq* current = ifc.ifc_req;
    int offset = 0;
    while (offset < ifc.ifc_len) {
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
        interfaces.add(
          String(current->ifr_name, IFNAMSIZ),
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
          current->ifr_ifindex
#else
          current->ifr_index
#endif
        );
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
  List<InetInterface> interfaces;
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct if_nameindex* ni;
  if ((ni = if_nameindex()) == 0) { // MT-safe
    throw NetworkException(
      "Unable to get interfaces",
      Type::getType<InetInterface>()
    );
  }
  // TAG: fixme
  InetInterface interface;
  interface.index = ni->if_index;
  interface.name = ni->if_name;
  interfaces.append(interface);
  if_freenameindex(ni); // MT-safe
#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to get interfaces",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current =
    Cast::pointer<const INTERFACE_INFO*>(
      Thread::getLocalStorage()->getElements()
    );
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (Cast::pointer<struct sockaddr*>(&current->iiAddress)->sa_family != AF_INET) { // TAG: AF_INET6
      continue;
    }
    InetInterface interface;
    
    unsigned int flags = 0;
    flags |= (current->iiFlags & IFF_UP) ? InetInterface::UP : 0;
    flags |= (current->iiFlags & IFF_LOOPBACK) ? InetInterface::LOOPBACK : 0;
    flags |= (current->iiFlags & IFF_POINTTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
    flags |= (current->iiFlags & IFF_BROADCAST) ? InetInterface::BROADCAST : 0;
    flags |= (current->iiFlags & IFF_MULTICAST) ? InetInterface::MULTICAST : 0;
    interface.flags = flags;
    
    interface.address =
      internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current->iiAddress));
    interface.netmask =
      internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current->iiNetmask));
    interface.broadcast =
      internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current->iiBroadcastAddress));
    
    StringOutputStream stream;
    stream << internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current->iiAddress))
           << FLUSH;
    interfaces.append(interface);
    ++current;
  }
#elif 1 || (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces;
// #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to get interfaces",
//       Type::getType<InetInterface>()
//     );
//   }
  try {
    struct ifconf ifc;
    ifc.ifc_len = Thread::getLocalStorage()->getSize();
    ifc.ifc_buf = Thread::getLocalStorage()->getElements();
    if (ioctl(handle, SIOCGIFCONF, &ifc)) {
      close(handle);
      throw NetworkException(
        "Unable to get interfaces",
        Type::getType<InetInterface>()
      );
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
#if defined(IFF_DYNAMIC)
        flags |= (req.ifr_flags & IFF_DYNAMIC) ? InetInterface::DYNAMIC : 0;
#endif
        interface.flags = flags;
      }
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
        interface.index = current->ifr_ifindex;
#else
        interface.index = current->ifr_index;
#endif
      }
      if (ioctl(handle, SIOCGIFADDR, current) == 0) {
        interface.address =
          internal::InetInterface::getAddress(current->ifr_addr);
      }
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
      if (ioctl(handle, SIOCGIFNETMASK, current) == 0) {
        interface.netmask =
          internal::InetInterface::getAddress(current->ifr_netmask);
      }
#endif
      if ((flags & InetInterface::BROADCAST) &&
          (ioctl(handle, SIOCGIFBRDADDR, current) == 0)) {
        interface.broadcast =
          internal::InetInterface::getAddress(current->ifr_broadaddr);
      }
      if ((flags & InetInterface::POINT_TO_POINT) &&
          (ioctl(handle, SIOCGIFDSTADDR, current) == 0)) {
        interface.destination =
          internal::InetInterface::getAddress(current->ifr_dstaddr);
      }
      if (ioctl(handle, SIOCGIFMETRIC, current) == 0) {
        interface.metric = current->ifr_metric;
      }
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
      if (ioctl(handle, SIOCGIFHWADDR, current) == 0) {
        interface.ethernet.setMAC48(
          Cast::getAddress(current->ifr_hwaddr.sa_data[0])
        );
      }
#else
      if (ioctl(handle, SIOCGENADDR, current) == 0) {
        interface.ethernet.setMAC48(
          Cast::getAddress(current->ifr_enaddr)
        );
      }
#endif
      interfaces.append(interface);
      ++current;
      offset += sizeof(struct ifreq);
    }
  } catch (...) {
    close(handle);
    throw;
  }
  close(handle);
#else
#  warning InetInterface::getInterfaces() not supported
#endif
  return interfaces;
}

unsigned int InetInterface::getIndexByName(const String& name) throw(NetworkException) {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  unsigned int index = if_nametoindex(name.getElements());
  bassert(index > 0, NetworkException("Unable to resolve interface", Type::getType<InetInterface>()));
  return index;
#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: fixme
  throw NetworkException(Type::getType<InetInterface>());
#else
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces;
// #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    );
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (String(current->ifr_name, IFNAMSIZ) == name) {
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
        close(handle);
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
        return req.ifr_ifindex;
#else
        return req.ifr_index;
#endif
      }
      break;
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
  throw NetworkException(
    "Unable to resolve interface",
    Type::getType<InetInterface>()
  );
#endif
}

unsigned int InetInterface::getIndexByAddress(const InetAddress& address) throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
  }
  closesocket(handle);
  const INTERFACE_INFO* current =
    Cast::pointer<const INTERFACE_INFO*>(
      Thread::getLocalStorage()->getElements()
    );
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (
      internal::InetInterface::getAddress(
        *Cast::pointer<struct sockaddr*>(&current[index].iiAddress)
      ) == address
    ) {
      return index;
    }
  }
  throw NetworkException("Unable to resolve interface", Type::getType<InetInterface>());
#else
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces;
// #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    );
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFADDR, current) == 0) {
      if (internal::InetInterface::getAddress(current->ifr_addr) == address) {
        if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
          close(handle);
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
          return current->ifr_ifindex;
#else
          return current->ifr_index;
#endif
        }
        break;
      }
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
  throw NetworkException(
    "Unable to resolve interface",
    Type::getType<InetInterface>()
  );
#endif
}

String InetInterface::getName(unsigned int index) throw(NetworkException) {
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  char name[IFNAMSIZ];
  bassert(
    if_indextoname(index, name) != 0,
    NetworkException("Unable to resolve interface", Type::getType<InetInterface>())
  );
  return String(name, IFNAMSIZ);
#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current =
    Cast::pointer<const INTERFACE_INFO*>(
      Thread::getLocalStorage()->getElements()
    );
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  bassert(
    index < numberOfInterfaces,
    NetworkException("Unable to resolve interface", Type::getType<InetInterface>())
  );
  StringOutputStream stream;
  stream << index << FLUSH;
  return stream.getString();
#else
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces;
// #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    );
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
      if (current->ifr_ifindex == index) {
#else
      if (current->ifr_index == index) {
#endif
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
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct ifreq req;
  bassert(
    if_indextoname(index, req.ifr_name) != 0,
    NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    )
  );
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  if (ioctl(handle, SIOCGIFADDR, &req) != 0) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    );
  }
  close(handle);
  return internal::InetInterface::getAddress(req.ifr_addr);
#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current =
    Cast::pointer<const INTERFACE_INFO*>(
      Thread::getLocalStorage()->getElements()
    );
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  bassert(
    index < numberOfInterfaces,
    NetworkException("Unable to resolve interface", Type::getType<InetInterface>())
  );
  return internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current[index].iiAddress));
#else
  struct ifreq req;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces;
// #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = Thread::getLocalStorage()->getSize();
  ifc.ifc_buf = Thread::getLocalStorage()->getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface",
      Type::getType<InetInterface>()
    );
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFADDR, current) == 0) {
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
      if (current->ifr_ifindex == index) {
#else
      if (current->ifr_index == index) {
#endif
        close(handle);
        return internal::InetInterface::getAddress(req.ifr_addr);
      }
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
  throw NetworkException(
    "Unable to resolve interface",
    Type::getType<InetInterface>()
  );
#endif
}

InetInterface::InetInterface() throw()
  : index(0), flags(0), metric(0) {
}

InetInterface::InetInterface(const String& name) throw(NetworkException)
  : index(0), flags(0), metric(0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        Thread::getLocalStorage()->getElements(),
        Thread::getLocalStorage()->getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to get interfaces",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current =
    Cast::pointer<const INTERFACE_INFO*>(
      Thread::getLocalStorage()->getElements()
    );
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (Cast::pointer<struct sockaddr*>(&current->iiAddress)->sa_family != AF_INET) { // TAG: AF_INET6
      continue;
    }

    StringOutputStream stream;
    stream << index << FLUSH;

    if (stream.getString() == name) {
      continue;
    }
    
    unsigned int flags = 0;
    flags |= (current->iiFlags & IFF_UP) ? InetInterface::UP : 0;
    flags |= (current->iiFlags & IFF_LOOPBACK) ? InetInterface::LOOPBACK : 0;
    flags |= (current->iiFlags & IFF_POINTTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
    flags |= (current->iiFlags & IFF_BROADCAST) ? InetInterface::BROADCAST : 0;
    flags |= (current->iiFlags & IFF_MULTICAST) ? InetInterface::MULTICAST : 0;
    this->flags = flags;
    
    address =
      internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current->iiAddress));
    netmask =
      internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current->iiNetmask));
    broadcast =
      internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current->iiBroadcastAddress));
    
    NCB ncb;
    clear(ncb);
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = 0;
    ::Netbios(&ncb);
    clear(ncb);
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = 0;
    
    copy<char>((char*)ncb.ncb_callname, "*               ", NCBNAMSZ);
    internal::InetInterface::ASTAT adapter;
    ncb.ncb_buffer = Cast::getAddress(adapter);
    ncb.ncb_length = sizeof(adapter);
    
    if (::Netbios(&ncb) == 0) {
      ethernet.setMAC48(Cast::getAddress(adapter.status.adapter_address[0]));
    }
    
    ++current;
  }
#else
  bassert(name.getLength() <= IFNAMSIZ, NetworkException(this));
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
#if defined(IFF_DYNAMIC)
    flags |= (req.ifr_flags & IFF_DYNAMIC) ? InetInterface::DYNAMIC : 0;
#endif
  }
  if (ioctl(handle, SIOCGIFINDEX, &req) == 0) {
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    index = req.ifr_ifindex;
#else
    index = req.ifr_index;
#endif
  }
  if (ioctl(handle, SIOCGIFADDR, &req) == 0) {
    address = internal::InetInterface::getAddress(req.ifr_addr);
  }
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  if (ioctl(handle, SIOCGIFNETMASK, &req) == 0) {
    netmask = internal::InetInterface::getAddress(req.ifr_netmask);
  }
#endif
  if ((flags & InetInterface::BROADCAST) &&
      (ioctl(handle, SIOCGIFBRDADDR, &req) == 0)) {
    broadcast = internal::InetInterface::getAddress(req.ifr_broadaddr);
  }
  if ((flags & InetInterface::POINT_TO_POINT) &&
      (ioctl(handle, SIOCGIFDSTADDR, &req) == 0)) {
    destination = internal::InetInterface::getAddress(req.ifr_dstaddr);
  }
  if (ioctl(handle, SIOCGIFMETRIC, &req) == 0) {
    metric = req.ifr_metric;
  }
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  if (ioctl(handle, SIOCGIFHWADDR, &req) == 0) {
    ethernet.setMAC48(Cast::getAddress(req.ifr_hwaddr));
  }
#else
  if (ioctl(handle, SIOCGENADDR, &req) == 0) {
    ethernet.setMAC48(Cast::getAddress(req.ifr_enaddr));
  }
#endif
  close(handle);
#endif
}

InetInterface::InetInterface(const InetInterface& copy) throw()
  : index(copy.index),
    name(copy.name),
    flags(copy.flags),
    address(copy.address),
    netmask(copy.netmask),
    broadcast(copy.broadcast),
    destination(copy.destination),
    metric(copy.metric),
    ethernet(copy.ethernet) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE