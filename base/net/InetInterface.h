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
#include <base/collection/List.h>
#include <base/collection/Array.h>
#include <base/collection/Association.h>
#include <base/net/NetworkException.h>
#include <base/net/InetAddress.h>
#include <base/communication/EUI64.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class represents an Internet Protocol network interface.

  @short Internet Protocol network interface.
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API InetInterface : public virtual Object {
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
  
  enum Flag {
    UP = 1,
    LOOPBACK = 2,
    POINT_TO_POINT = 4,
    BROADCAST = 8,
    MULTICAST = 16,
    DYNAMIC = 32
  };

  /**
    Returns the names and indices of the available interfaces.
  */
  static Array<Association<String, unsigned int> > getInterfaceNames();
  
  /**
    Returns all of the Internet Protocol network interfaces of the host.
  */
  static List<InetInterface> getInterfaces();
  
  /**
    Returns the index of the specified interface.
  */
  static unsigned int getIndexByName(const String& name);
  
  /**
    Returns the index of the interface associated with the specified address.
  */
  static unsigned int getIndexByAddress(const InetAddress& address);
  
  /**
    Returns the name of the specified interface.
  */
  static String getName(unsigned int index);
  
  /**
    Returns the address of the specified interface.
  */
  static InetAddress getAddress(unsigned int index);
  
  /**
    Initializes interface.
  */
  InetInterface() noexcept;

  /**
    Initializes interface by name.
  */
  InetInterface(const String& name);
  
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
  inline unsigned int getIndex() const noexcept
  {
    return index;
  }
  
  /**
    Returns the name of the network interface.
  */
  inline const String& getName() const noexcept
  {
    return name;
  }
  
  /**
    Returns the flags.
  */
  inline unsigned int getFlags() const noexcept
  {
    return flags;
  }
  
  /**
    Returns the address.
  */
  inline const InetAddress& getAddress() const noexcept
  {
    return address;
  }
  
  /**
    Returns the network mask.
  */
  inline const InetAddress& getNetmask() const noexcept
  {
    return netmask;
  }
  
  /**
    Returns the broadcast address. Only valid if BROADCAST is set.
  */
  inline const InetAddress& getBroadcast() const noexcept
  {
    return broadcast;
  }
  
  /**
    Returns the destination address. Only valid if POINT_TO_POINT is set.
  */
  inline const InetAddress& getDestination() const noexcept
  {
    return destination;
  }
  
  /**
    Returns the ethernet address.
  */
  inline const EUI64& getEthernetAddress() const noexcept
  {
    return ethernet;
  }

  /**
    Returns the metric.
  */
  inline unsigned int getMetric() const noexcept
  {
    return metric;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
