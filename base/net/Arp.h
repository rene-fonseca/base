/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/net/NetworkException.h>
#include <base/net/InetAddress.h>
#include <base/communication/EUI64.h>
#include <base/collection/HashTable.h>

#if 0
win32
CreateIpNetEntry, DeleteIpNetEntry, MIB_IPNETTABLE, SetIpNetEntry GetIpNetTable, MIB_IPNETROW
DWORD SendARP(
  IPAddr DestIP,
  IPAddr SrcIP,
  PULONG pMacAddr,
  PULONG PhyAddrLen
);
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class represents an Internet Protocol network interface.

  @short Internet Protocol network interface.
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Arp : public Object {
private:
  
  /** Specifies the index of the network interface. */
  unsigned int index = 0;
  /** Specifies the name of the network interface. */
  String name;
  /** Flags. */
  unsigned int flags = 0;
  /** Address. */
  InetAddress address;
  /** Netmask. */
  InetAddress netmask;
  /** Broadcast address. */
  InetAddress broadcast;
  /** Destination address. */
  InetAddress destination;
  /** Metric. */
  unsigned int metric = 0;
  /** Ethernet address. */
  EUI64 ethernet;
public:

  // EUI64 InetAddress
  
  /**
    Returns the names and indices of the available interfaces.
  */
  static HashTable<String, unsigned int> getInterfaceNames() noexcept;
  
  /**
    Returns all of the Internet Protocol network interfaces of the host.
  */
  static List<InetInterface> getInterfaces() throw(NetworkException);
  
  /**
    Returns the index of the specified interface.
  */
  static unsigned int getIndexByName(const String& name) throw(NetworkException);
  
  /**
    Returns the index of the interface associated with the specified address.
  */
  static unsigned int getIndexByAddress(const InetAddress& address) throw(NetworkException);
  
  /**
    Returns the name of the specified interface.
  */
  static String getName(unsigned int index) throw(NetworkException);
  
  /**
    Returns the address of the specified interface.
  */
  static InetAddress getAddress(unsigned int index) throw(NetworkException);
  
  /**
    Initializes interface.
  */
  InetInterface() noexcept;

  /**
    Initializes interface by name.
  */
  InetInterface(const String& name) throw(NetworkException);
  
  /**
    Initializes interface by address.
  */
  InetInterface(const InetAddress& address) noexcept;
  
  /**
    Copy constructor.
  */
  InetInterface(const InetInterface& copy) noexcept;

  /**
    Returns the index of the network interface.
  */
  inline unsigned int getIndex() const noexcept {
    return index;
  }
  
  /**
    Returns the name of the network interface.
  */
  inline const String& getName() const noexcept {
    return name;
  }
  
  /**
    Returns the flags.
  */
  inline unsigned int getFlags() const noexcept {
    return flags;
  }
  
  /**
    Returns the address.
  */
  inline const InetAddress& getAddress() const noexcept {
    return address;
  }
  
  /**
    Returns the network mask.
  */
  inline const InetAddress& getNetmask() const noexcept {
    return netmask;
  }
  
  /**
    Returns the broadcast address. Only valid if BROADCAST is set.
  */
  inline const InetAddress& getBroadcast() const noexcept {
    return broadcast;
  }
  
  /**
    Returns the destination address. Only valid if POINT_TO_POINT is set.
  */
  inline const InetAddress& getDestination() const noexcept {
    return destination;
  }
  
  /**
    Returns the ethernet address.
  */
  inline const EUI64& getEthernetAddress() const noexcept {
    return ethernet;
  }

  /**
    Returns the metric.
  */
  inline unsigned int getMetric() const noexcept {
    return metric;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
