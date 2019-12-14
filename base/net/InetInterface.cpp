/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/net/InetInterface.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>

// TAG: remove index attribute/support only rely on name of interface

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <nb30.h>
#  undef interface
#else // unix
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#    define BSD_COMP
#    include <sys/sockio.h>
#  endif
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <net/if.h>
#  include <netinet/in.h>
#  include <sys/ioctl.h>
#  include <unistd.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace internal {
  
  class InetInterface {
  public:

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    struct ASTAT {
      ADAPTER_STATUS status;
      NAME_BUFFER nameBuffer[NCBNAMSZ];
    };
#endif
    
    static inline InetAddress getAddress(const struct sockaddr& address) noexcept
    {
      switch (address.sa_family) {
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
      case AF_INET6:
        return InetAddress(
          Cast::getAddress(
            Cast::pointer<const struct sockaddr_in6*>(&address)->sin6_addr
          ),
          InetAddress::IP_VERSION_6
        );
#endif
      case AF_INET:
        return InetAddress(
          Cast::getAddress(
            Cast::pointer<const struct sockaddr_in*>(&address)->sin_addr
          ),
          InetAddress::IP_VERSION_4
        );
      default:
        return InetAddress();
      }
    }
  };
}; // end of namespace internal

Array<Association<String, unsigned int> > InetInterface::getInterfaceNames() noexcept
{
  Array<Association<String, unsigned int> > interfaces;
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  struct if_nameindex* ni = nullptr;
  if ((ni = if_nameindex()) == 0) { // MT-safe
    throw NetworkException(
      "Unable to get interfaces.",
      Type::getType<InetInterface>()
    );
  }
  try {
    const struct if_nameindex* current = ni;
    while (current->if_index) {
      interfaces.append(Association<String, unsigned int>(String(current->if_name), current->if_index));
      ++current;
    }
  } catch (...) {
    if_freenameindex(ni); // MT-safe
    throw;
  }
  if_freenameindex(ni); // MT-safe
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned = 0;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        buffer.getElements(),
        buffer.getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to get interfaces.",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current = Cast::pointer<const INTERFACE_INFO*>(buffer.getElements());
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (Cast::pointer<struct sockaddr*>(&current->iiAddress)->sa_family != AF_INET) { // TAG: AF_INET6
      continue;
    }
    interfaces.append(Association<String, unsigned int>(StringOutputStream() << index, index));
    ++current;
  }
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN))
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  try {
    PrimitiveStackArray<char> buffer(1024);
    struct ifconf ifc;
    ifc.ifc_len = buffer.size();
    ifc.ifc_buf = buffer;
    if (ioctl(handle, SIOCGIFCONF, &ifc)) {
      close(handle);
      throw NetworkException(
        "Unable to get interfaces.",
        Type::getType<InetInterface>()
      );
    }
#  if (defined(SIOCGIFINDEX))
    struct ifreq* current = ifc.ifc_req;
    int offset = 0;
    while (offset < ifc.ifc_len) {
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
        interfaces.append(Association<String, unsigned int>(
          String(current->ifr_name, IFNAMSIZ),
#    if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
          current->ifr_ifindex
#    else
          current->ifr_index
#    endif
        ));
      }
      ++current;
      offset += sizeof(struct ifreq);
    }
#  else
    struct ifreq* current = ifc.ifc_req;
    int offset = 0;
    int index = 1;
    while (offset < ifc.ifc_len) {
      interfaces.add(Association<String, unsigned int>(String(current->ifr_name, IFNAMSIZ), index++));
      ++current;
      offset += sizeof(struct ifreq);
    }
#  endif
  } catch (...) {
    close(handle);
    throw;
  }
  close(handle);
#else
// #  warning InetInterface::getInterfaceNames() not supported
#endif
  return interfaces;
}

List<InetInterface> InetInterface::getInterfaces()
{
  List<InetInterface> interfaces;
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  struct if_nameindex* ni = nullptr;
  if ((ni = if_nameindex()) == 0) { // MT-safe
    throw NetworkException(
      "Unable to get interfaces.",
      Type::getType<InetInterface>()
    );
  }
  try {
    const struct if_nameindex* current = ni;
    while (current->if_index) {
      InetInterface interface;
      interface.index = current->if_index;
      interface.name = current->if_name;
      interfaces.append(interface);
      ++current;
    }
  } catch (...) {
    if_freenameindex(ni); // MT-safe
    throw;
  }
  if_freenameindex(ni); // MT-safe
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned = 0;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        buffer.getElements(),
        buffer.getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to get interfaces.",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current = Cast::pointer<const INTERFACE_INFO*>(buffer.getElements());
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  for (unsigned int index = 0; index < numberOfInterfaces; ++index) {
    if (Cast::pointer<struct sockaddr*>(&current->iiAddress)->sa_family != AF_INET) { // TAG: AF_INET6
      continue;
    }
    InetInterface interface;
    
    unsigned int flags = 0;
    flags |= (current->iiFlags & IFF_UP) ? InetInterface::UP : 0;
    flags |= (current->iiFlags & IFF_LOOPBACK) ? InetInterface::LOOPBACK : 0;
#if (defined(IFF_POINTTOPOINT))
    flags |= (current->iiFlags & IFF_POINTTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
#endif
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
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS))
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  try {
    struct ifconf ifc;
    ifc.ifc_len = buffer.getSize()/sizeof(char);
    ifc.ifc_buf = (char*)buffer.getElements();
    if (ioctl(handle, SIOCGIFCONF, &ifc)) {
      close(handle);
      throw NetworkException(
        "Unable to get interfaces.",
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
#if (defined(IFF_POINTTOPOINT))
        flags |= (current->ifr_flags & IFF_POINTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
#endif
        flags |= (current->ifr_flags & IFF_BROADCAST) ? InetInterface::BROADCAST : 0;
        flags |= (current->ifr_flags & IFF_MULTICAST) ? InetInterface::MULTICAST : 0;
#if (defined(IFF_DYNAMIC))
        flags |= (current->ifr_flags & IFF_DYNAMIC) ? InetInterface::DYNAMIC : 0;
#endif
        interface.flags = flags;
      }
#if (defined(SIOCGIFINDEX))
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
        interface.index = current->ifr_ifindex;
#  else
        interface.index = current->ifr_index;
#  endif
      }
#endif
      if (ioctl(handle, SIOCGIFADDR, current) == 0) {
        interface.address =
          internal::InetInterface::getAddress(current->ifr_addr);
      }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
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
#if (defined(IFF_POINTTOPOINT))
      if ((flags & InetInterface::POINT_TO_POINT) &&
          (ioctl(handle, SIOCGIFDSTADDR, current) == 0)) {
        interface.destination =
          internal::InetInterface::getAddress(current->ifr_dstaddr);
      }
#endif
      if (ioctl(handle, SIOCGIFMETRIC, current) == 0) {
        interface.metric = current->ifr_metric;
      }
#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN))
      if (ioctl(handle, SIOCGIFHWADDR, current) == 0) {
        interface.ethernet.setMAC48(
          Cast::getAddress(current->ifr_hwaddr.sa_data[0])
        );
      }
#else
      if (ioctl(handle, SIOCGENADDR, current) == 0) { // ignore error
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
// #  warning InetInterface::getInterfaces() not supported
#endif
  return interfaces;
}

unsigned int InetInterface::getIndexByName(const String& name)
{
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  unsigned int index = if_nametoindex(name.getElements());
  bassert(
    index > 0,
    NetworkException("Unable to resolve interface.", Type::getType<InetInterface>())
  );
  return index;
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: fixme
  throw NetworkException(Type::getType<InetInterface>());
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#else
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;

  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces = 0;
// #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface.",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = (int)buffer.getSize();
  ifc.ifc_buf = (char*)buffer.getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    );
  }
#if (defined(SIOCGIFINDEX))
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  while (offset < ifc.ifc_len) {
    if (String(current->ifr_name, IFNAMSIZ) == name) {
      if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
        close(handle);
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
        return current->ifr_ifindex;
#  else
        return current->ifr_index;
#  endif
      }
      break;
    }
    ++current;
    offset += sizeof(*current);
  }
  close(handle);
#else
  close(handle);
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
  int index = 1;
  while (offset < ifc.ifc_len) {
    if (String(current->ifr_name, IFNAMSIZ) == name) {
      return index;
    }
    ++index;
    ++current;
    offset += sizeof(*current);
  }
#endif
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#endif
}

unsigned int InetInterface::getIndexByAddress(const InetAddress& address)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned = 0;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        buffer.getElements(),
        buffer.getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException("Unable to resolve interface.", Type::getType<InetInterface>());
  }
  closesocket(handle);
  const INTERFACE_INFO* current = Cast::pointer<const INTERFACE_INFO*>(buffer.getElements());
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
  throw NetworkException("Unable to resolve interface.", Type::getType<InetInterface>());
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#else
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces = 0;
// #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface.",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = (int)buffer.getSize()/sizeof(char);
  ifc.ifc_buf = (char*)buffer.getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    );
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
#if (defined(SIOCGIFINDEX))
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFADDR, current) == 0) {
      if (internal::InetInterface::getAddress(current->ifr_addr) == address) {
        if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
          close(handle);
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
          return current->ifr_ifindex;
#  else
          return current->ifr_index;
#  endif
        }
        break;
      }
    }
    ++current;
    offset += sizeof(*current);
  }
#else
  int index = 1;
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFADDR, current) == 0) {
      if (internal::InetInterface::getAddress(current->ifr_addr) == address) {
        return index;
      }
    }
    ++index;
    ++current;
    offset += sizeof(*current);
  }
#endif
  close(handle);
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#endif
}

String InetInterface::getName(unsigned int index)
{
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  char name[IFNAMSIZ];
  bassert(
    if_indextoname(index, name) != 0,
    NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    )
  );
  return String(name, IFNAMSIZ);
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned = 0;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        buffer.getElements(),
        buffer.getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current = Cast::pointer<const INTERFACE_INFO*>(buffer.getElements());
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  bassert(
    index < numberOfInterfaces,
    NetworkException("Unable to resolve interface.", Type::getType<InetInterface>())
  );
  StringOutputStream stream;
  stream << index << FLUSH;
  return stream.getString();
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#else
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces = 0;
// #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface.",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = (int)buffer.getSize();
  ifc.ifc_buf = (char*)buffer.getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    );
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
#if (defined(SIOCGIFINDEX))
  while (offset < ifc.ifc_len) {
    if (ioctl(handle, SIOCGIFINDEX, current) == 0) {
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
      if (current->ifr_ifindex == index)
#  else
      if (current->ifr_index == index)
#  endif
      {
        close(handle);
        return String(current->ifr_name, IFNAMSIZ);
      }
    }
    ++current;
    offset += sizeof(*current);
  }
#else
  unsigned int currentIndex = 1;
  while (offset < ifc.ifc_len) {
    if (currentIndex == index) {
      close(handle);
      return String(current->ifr_name, IFNAMSIZ);
    }
    ++index;
    ++current;
    offset += sizeof(*current);
  }
#endif
  close(handle);
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#endif
}

InetAddress InetInterface::getAddress(unsigned int index)
{
#if (defined(_COM_AZURE_DEV__BASE__INET_IPV6))
  struct ifreq req;
  bassert(
    if_indextoname(index, req.ifr_name) != 0,
    NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    )
  );
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  if (ioctl(handle, SIOCGIFADDR, &req) != 0) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    );
  }
  close(handle);
  return internal::InetInterface::getAddress(req.ifr_addr);
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned = 0;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        buffer.getElements(),
        buffer.getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current = Cast::pointer<const INTERFACE_INFO*>(buffer.getElements());
  const unsigned int numberOfInterfaces = bytesReturned/sizeof(*current);
  bassert(
    index < numberOfInterfaces,
    NetworkException("Unable to resolve interface.", Type::getType<InetInterface>())
  );
  return internal::InetInterface::getAddress(*Cast::pointer<struct sockaddr*>(&current[index].iiAddress));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#else
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  struct ifreq req;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
//   int numberOfInterfaces = 0;
// #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
//   const int command = SIOCGIFCOUNT;
// #else
//   const int command = SIOCGIFNUM;
// #endif
//   if (ioctl(handle, command, &numberOfInterfaces) != 0) {
//     close(handle);
//     throw NetworkException(
//       "Unable to resolve interface.",
//       Type::getType<InetInterface>()
//     );
//   }
  struct ifconf ifc;
  ifc.ifc_len = (int)buffer.getSize();
  ifc.ifc_buf = (char*)buffer.getElements();
  if (ioctl(handle, SIOCGIFCONF, &ifc)) {
    close(handle);
    throw NetworkException(
      "Unable to resolve interface.",
      Type::getType<InetInterface>()
    );
  }
  const struct ifreq* current = ifc.ifc_req;
  int offset = 0;
#if (defined(SIOCGIFINDEX))
  while (offset < ifc.ifc_len) {
    // TAG: fixme - invoke SIOCGIFINDEX first
    if (ioctl(handle, SIOCGIFADDR, current) == 0) {
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
      if (current->ifr_ifindex == index) {
#  else
      if (current->ifr_index == index) {
#  endif
        close(handle);
        return internal::InetInterface::getAddress(req.ifr_addr);
      }
    }
    ++current;
    offset += sizeof(*current);
  }
#else
  unsigned int currentIndex = 1;
  while (offset < ifc.ifc_len) {
    if (currentIndex == index) {
      if (ioctl(handle, SIOCGIFADDR, current) == 0) {
        close(handle);
        return internal::InetInterface::getAddress(req.ifr_addr);
      }
    }
    ++index;
    ++current;
    offset += sizeof(*current);
  }
#endif
  close(handle);
  throw NetworkException(
    "Unable to resolve interface.",
    Type::getType<InetInterface>()
  );
#endif
}

InetInterface::InetInterface() noexcept
  : index(0), flags(0), metric(0)
{
}

InetInterface::InetInterface(const String& name)
  : index(0), flags(0), metric(0)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  DWORD bytesReturned = 0;
  if (::WSAIoctl(
        handle,
        SIO_GET_INTERFACE_LIST,
        0,
        0,
        buffer.getElements(),
        buffer.getSize(),
        &bytesReturned,
        0,
        0)) {
    closesocket(handle);
    throw NetworkException(
      "Unable to get interfaces.",
      Type::getType<InetInterface>()
    );
  }
  closesocket(handle);
  const INTERFACE_INFO* current = Cast::pointer<const INTERFACE_INFO*>(buffer.getElements());
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
#if (defined(IFF_POINTTOPOINT))
    flags |= (current->iiFlags & IFF_POINTTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
#endif
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
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  bassert(name.getLength() <= IFNAMSIZ, NetworkException(this));
  struct ifreq req;
  copy(req.ifr_name, name.getElements(), name.getLength()); // TAG how to init this
  int handle = socket(PF_INET, SOCK_STREAM, 0);
  this->name = name;
  if (ioctl(handle, SIOCGIFFLAGS, &req) == 0) {
    flags |= (req.ifr_flags & IFF_UP) ? InetInterface::UP : 0;
    flags |= (req.ifr_flags & IFF_LOOPBACK) ? InetInterface::LOOPBACK : 0;
#if (defined(IFF_POINTTOPOINT))
    flags |= (req.ifr_flags & IFF_POINTOPOINT) ? InetInterface::POINT_TO_POINT : 0;
#endif
    flags |= (req.ifr_flags & IFF_BROADCAST) ? InetInterface::BROADCAST : 0;
    flags |= (req.ifr_flags & IFF_MULTICAST) ? InetInterface::MULTICAST : 0;
#if (defined(IFF_DYNAMIC))
    flags |= (req.ifr_flags & IFF_DYNAMIC) ? InetInterface::DYNAMIC : 0;
#endif
  }
#if (defined(SIOCGIFINDEX))
  if (ioctl(handle, SIOCGIFINDEX, &req) == 0) {
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    index = req.ifr_ifindex;
#  else
    index = req.ifr_index;
#  endif
  }
#endif
  if (ioctl(handle, SIOCGIFADDR, &req) == 0) {
    address = internal::InetInterface::getAddress(req.ifr_addr);
  }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  if (ioctl(handle, SIOCGIFNETMASK, &req) == 0) {
    netmask = internal::InetInterface::getAddress(req.ifr_netmask);
  }
#endif
  if ((flags & InetInterface::BROADCAST) &&
      (ioctl(handle, SIOCGIFBRDADDR, &req) == 0)) {
    broadcast = internal::InetInterface::getAddress(req.ifr_broadaddr);
  }
#if (defined(IFF_POINTTOPOINT))  
  if ((flags & InetInterface::POINT_TO_POINT) &&
      (ioctl(handle, SIOCGIFDSTADDR, &req) == 0)) {
    destination = internal::InetInterface::getAddress(req.ifr_dstaddr);
  }
#endif
  if (ioctl(handle, SIOCGIFMETRIC, &req) == 0) {
    metric = req.ifr_metric;
  }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__OPENBSD)

#if 0 // #include <ifaddrs.h>
  struct ifaddrs *ifap = NULL;
  if (getifaddrs(&ifap) == 0) {
    for (struct ifaddrs* p = ifap; p; p = p->ifa_next) {
      if (p->ifa_addr->sa_family == AF_LINK) {
        struct sockaddr_dl* sdp = (struct sockaddr_dl*) p->ifa_addr;
        ethernet.setMAC48(Cast::getAddress(sdp->sdl_data + sdp->sdl_nlen));
        freeifaddrs(ifap);
        break;
      }
    }
    freeifaddrs(ifap);
  }
#endif

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN))
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

InetInterface::InetInterface(const InetInterface& copy) noexcept
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
