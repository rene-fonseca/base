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

#include <base/communication/CommunicationsPort.h>
#include <base/communication/EUI64.h>
#include <base/communication/IEEE1394Impl.h>
#include <base/communication/IEEE1394Exception.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/mem/Reference.h>
#include <base/collection/Array.h>
#include <base/Cast.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  IEEE-1394 (also known as FireWire (Apple Computer) and i.LINK (Sony Corp.))
  support. See 1394 Trade Association (<a href="http://www.1394ta.org">
  http://www.1394ta.org</a>).
  
  @short IEEE-1394 (FireWire)
  @ingroup communications
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API IEEE1394 : public CommunicationsPort, public IEEE1394Common {
public:

  /** The CSR base address. */
  static const uint64 CSR_BASE_ADDRESS = 0xfffff0000000ULL;
  
  /** Control and status register (CSR). */
  enum CSRRegister {
    STATE_CLEAR = 0x0000,
    STATE_SET = 0x0004,
    NODE_IDS = 0x0008,
    RESET_START = 0x000c,
    SPLIT_TIMEOUT_HI = 0x0018,
    SPLIT_TIMEOUT_LO = 0x001c,
    ARGUMENT_HI = 0x0020,
    ARGUMENT_LO = 0x0024,
    TEST_START = 0x0028,
    TEST_STATUS = 0x002c,
    BUS_DEPENDENT = 0x0200,
    CYCLE_TIME = 0x0200,
    BUS_TIME = 0x0204,
    POWER_FAIL_IMMINENT = 0x0208,
    POWER_SOURCE = 0x020c,
    BUSY_TIMEOUT = 0x0210,
    BUS_MANAGER_ID = 0x021c,
    BANDWIDTH_AVAILABLE = 0x0220,
    CHANNELS_AVAILABLE_HI = 0x0224,
    CHANNELS_AVAILABLE_LO = 0x0228,
    BROADCAST_CHANNEL = 0x0234,
    CONFIGURATION_ROM = 0x0400,
    BUS_INFO_BLOCK = CONFIGURATION_ROM + 0x04,
    BUS_INFO_NAME = BUS_INFO_BLOCK,
    BUS_INFO_FLAGS = BUS_INFO_BLOCK + 0x04,
    BUS_INFO_GUID = BUS_INFO_BLOCK + 0x08,
    FCP_COMMAND_FRAME = 0x0b00,
    FCP_COMMAND_FRAME_END = 0x0d00,
    FCP_RESPONSE_FRAME = 0x0d00,
    FCP_RESPONSE_FRAME_END = 0x0f00,
    TOPOLOGY_MAP = 0x1000,
    TOPOLOGY_MAP_END = 0x1400,
    SPEED_MAP = 0x2000,
    SPEED_MAP_END = 0x3000
  };

  /** Control and status key type (see CSR Architecture). */
  enum CSRKeyType {
    KEY_TYPE_IMMEDIATE = 0x00,
    KEY_TYPE_OFFSET = 0x01,
    KEY_TYPE_LEAF = 0x02,
    KEY_TYPE_DIRECTORY = 0x03
  };
  
  /** Control and status key value (see CSR Architecture). */
  enum CSRKeyValue {
    KEY_00 = 0x00,
    KEY_TEXTUAL = 0x01,
    KEY_BUS_DEPENDENT_INFORMATION = 0x02,
    KEY_MODULE_VENDOR_ID = 0x03,
    KEY_MODULE_HARDWARE_VERSION = 0x04,
    KEY_SPECIFICATION_ID = 0x05,
    KEY_MODULE_SOFTWARE_VERSION = 0x06,
    KEY_MODULE_DEPENDENT_INFORMATION = 0x07,
    KEY_NODE_VENDOR_ID = 0x08,
    KEY_NODE_HARDWARE_VERSION = 0x09,
    KEY_NODE_SPECIFICATION_ID = 0x0a,
    KEY_NODE_SOFTWARE_VERSION = 0x0b,
    KEY_NODE_CAPABILITIES = 0x0c,
    KEY_NODE_UNIQUE_ID = 0x0d,
    KEY_NODE_UNITS_EXTENT = 0x0e,
    KEY_NODE_MEMORY_EXTENT = 0x0f,
    KEY_NODE_DEPENDENT_INFORMATION = 0x10,
    KEY_UNIT_DIRECTORY = 0x11,
    KEY_UNIT_SPECIFICATION_ID = 0x12,
    KEY_UNIT_SOFTWARE_VERSION = 0x13,
    KEY_UNIT_DEPENDENT_INFORMATION = 0x14,
    KEY_MODEL = 0x17,
    KEY_INSTANCE = 0x18,
    KEY_KEYWORD = 0x19,
    KEY_FEATURE = 0x1a,
    KEY_3f = 0x3f
  };

  /** Power class. */
  enum PowerClass {
    /**
      The node does not need power from the bus and does not repeat power.
    */
    POWER_NO_REPEAT,
    /**
      The node is self-powered and provides a minimum of 15W on the bus.
    */
    POWER_PROVIDES_15W,
    /**
      The node is self-powered and provides a minimum of 30W on the bus.
    */
    POWER_PROVIDES_30W,
    /**
      The node is self-powered and provides a minimum of 45W on the bus.
    */
    POWER_PROVIDES_45W,
    /**
      The node may be powered from the bus and uses up to 1W.
    */
    POWER_CONSUMES_1W,
    /**
      The node may be powered from the bus and uses up to 1W plus an additional
      2W to enable higher layers.
    */
    POWER_CONSUMES_3W,
    /**
      The node may be powered from the bus and uses up to 1W plus an additional
      5W to enable higher layers.
    */
    POWER_CONSUMES_6W,
    /**
      The node may be powered from the bus and uses up to 1W plus an additional
      9W to enable higher layers.
    */
    POWER_CONSUMES_10W
  };
  
  /** The role of a node. */
  enum Role {
    BUS_MANAGER, /**< Bus manager. */
    ISOCHRONOUS_RESOURCE_MANAGER, /**< Isochronous resource manager. */
    CYCLE_MASTER, /**< Cycle master. */
    ROOT /** The root of the bus. */
  };
  
  /**
    Returns the CSR key for the specified CSR key and value.
    
    @param type The type of the CSR key.
    @param value The value of the CSR key.
  */
  static inline uint8 makeCSRkey(CSRKeyType type, CSRKeyValue value) noexcept
  {
    return (static_cast<uint8>(type) << 6) | static_cast<uint8>(value);
  }

  /**
    Returns the CSR key type of the specified quadlet (native byte order).
  */
  static inline CSRKeyType getCSRKeyType(uint32 quadlet) noexcept
  {
    return static_cast<CSRKeyType>(quadlet >> (24 + 6));
  }

  /**
    Returns the CSR key value of the specified quadlet (native byte order).
  */
  static inline CSRKeyValue getCSRKeyValue(uint32 quadlet) noexcept
  {
    return static_cast<CSRKeyValue>((quadlet >> 24) & ((1 << 6) - 1));
  }
  
  /**
    Returns the maximum payload for asynchronous packets for the specified
    speed.
  */
  static inline unsigned int getMaximumAsyncPayloadForSpeed(
    Speed speed) noexcept
  {
    return 1 << (static_cast<unsigned int>(speed) + 9);
  }
  
  /**
    Returns the maximum payload for isochronous packets for the specified
    speed.
  */
  static inline unsigned int getMaximumIsoPayloadForSpeed(
    Speed speed) noexcept
  {
    return 1 << (static_cast<unsigned int>(speed) + 10);
  }

  /**
    This exception is raised on bus resets. The reset must be acknowledged
    to bring the IEEE-1394 interface back into the working operating mode.
    
    @short IEEE 1394 bus reset exception.
    @ingroup exceptions communication
    @version 1.0
  */
  class BusReset {
  public:
  };

  /** Exception causes. */
  enum ExceptionCause {
    NODE_NOT_PRESENT,
    NO_GENERAL_CONFIGURATION_ROM,
    INVALID_BUS_INFORMATION_BLOCK,
    INVALID_ROOT_DIRECTORY_BLOCK,
    INVALID_DEVICE_INDEPENDENT_BLOCK,
    INVALID_DEVICE_DEPENDENT_BLOCK,
    UNABLE_TO_OPEN_CHANNEL,
    INVALID_REQUEST,
    REQUEST_NOT_READY,
    REQUEST_NOT_PENDING,
    UNABLE_TO_READ,
    UNABLE_TO_WRITE,
    LAST_CAUSE
  };
  
  /** Capability flags of nodes. */
  enum Capability {
    ISOCHRONOUS_RESOURCE_MANAGER_CAPABLE = 1 << 0,
    CYCLE_MASTER_CAPABLE = 1 << 1,
    ISOCHRONOUS_TRANSACTION_CAPABLE = 1 << 2,
    BUS_MASTER_CAPABLE = 1 << 3,
    POWER_MANAGER_CAPABLE = 1 << 4
  };

  /** Port state. */
  enum PortState {
    PORT_NOT_CONNECTED,
    PORT_CONNECTED_TO_OHTER_LAYER,
    PORT_CONNECTED_TO_PARENT,
    PORT_CONNECTED_TO_CHILD
  };
  
  /* Isochronous data packet header. */
  struct IsochronousPacketHeader {
    unsigned int length : 16; /**< The length of the packet. */
    unsigned int tag : 2; /**< Isochronous data format tag. */
    unsigned int channel : 6; /** The isochronous channel. */
    unsigned int tcode : 4; /** Transaction code. */
    unsigned int sy : 4; /** Synchronization code. */
  };

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct BusInfo {
    BigEndian<uint32> name; // "1394"
    BigEndian<uint32> flags;
    uint8 guid[8];
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct ConfigurationIntro {
    BigEndian<uint32> crc;
    BusInfo busInfo;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
private:

  /** The IEEE 1394 implementation. */
  Reference<IEEE1394Impl> ieee1394impl;

  inline IEEE1394(IEEE1394Impl* _ieee1394impl) noexcept
    : ieee1394impl(_ieee1394impl)
  {
  }
protected:

  // TAG: this does not belong here
  inline unsigned int getBits(
    unsigned int value,
    unsigned int offset,
    unsigned int size) noexcept
  {
    return (value >> offset) & ((1 << size) - 1);
  }
  
  /* This structure describes a node of the IEEE 1394 bus. */
  struct NodeDescriptor {
    /** Specifies that the node is present. */
    bool present = false;
    /** Specifies that the node has an active link and transaction layer. */
    bool link = false;
    /**
      Specifies that the node is a contender for the bus manager or isochronous
      resource manager.
    */
    bool contender = false;
    /** The gap count. */
    unsigned int gapCount = 0;
    /** The physical speed of the node. */
    Speed speed = S100;
    /** The link speed of the node. */
    Speed linkSpeed = S100;
    /** The GUID of the node if available. */
    EUI64 guid;
    /** Capabilities. */
    unsigned int capabilities = 0;
    /** The IEEE 1394 specification. */
    Standard standard = STANDARD_UNSPECIFIED;
    /** The number of ports. */
    unsigned int numberOfPorts = 0;
    /** The port connections. */
    PortState ports[3 + 3 * 8] = { PORT_NOT_CONNECTED };
    /** Specifies that the node initiated the last reset. */
    bool initiatedReset = false;
    /** The power class of the node. */
    PowerClass powerClass = POWER_NO_REPEAT;
    /** The maximum payload. */
    unsigned int maximumPayload = 0;
  };

  /** Holds the reset generation number. */
  uint32 resetGeneration = 0;
  /** The number of nodes of the local bus. */
  unsigned int numberOfNodes = 0;
  /** Description of the nodes of the local bus. */
  NodeDescriptor nodes[63];
  /** The physical id of the adapter. */
  unsigned int localId = 0;
  /** The physical id of the current bus manager. */
  unsigned int busManagerId = 0;
  /** The physical id of the current isochronous resource manager. */
  unsigned int isochronousResourceManagerId = 0;
  /** The physical id of the current cycle master. */
  unsigned int cycleMasterId = 0;
  /** The maximum speeds of the nodes. */
  Speed speedMap[63][64] = { S100 };
  /** Mask specifying the nodes with the link layer activated. */
  uint64 linkActiveNodes = 0;
  /** Mask specifying the contenders. */
  uint64 contenderNodes = 0;

  /**
    Returns the guid of the specified node.
  */
  inline EUI64 getEUI64(unsigned short node)
  {
    Quadlet guid[2];
    ieee1394impl->read(
      node,
      IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_GUID,
      Cast::getAddress(guid[0]), sizeof(guid[0])
    );
    ieee1394impl->read(
      node,
      IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_GUID + sizeof(Quadlet),
      Cast::getAddress(guid[1]), sizeof(guid[1])
    );
    return EUI64(Cast::getAddress(guid[0]));
  }
  
  /**
    Loads the topology map from the bus manager.
  */
  void loadTopologyMap();

  /**
    Loads the speed map from the bus manager.
  */
  void loadSpeedMap();
  
  /**
    Reload information from local bus.
  */
  void reload();
public:
  
  /**
    Check if the bus has been reset.
  */
  void checkResetGeneration();
public:

  /**
    Returns a convenient string representation of the node id (e.g. "local:0"
    or "123:broadcast").

    @param nodeId The node id.
  */
  static String getAsString(unsigned short nodeId) noexcept;
  
  /**
    Returns the node id of the node with the desired role.
    
    @param role The role of the desired node.
    @param busId The bus id of the bus ([0; 1023]). The default is the local
    bus.

    @return Returns the broadcast id for the specified bus if no node is found.
  */
  unsigned short findRole(
    Role role,
    unsigned int busId = LOCAL_BUS);
  
  /**
    Returns the number of nodes of the local bus.
  */
  inline unsigned int getNumberOfNodes() const noexcept
  {
    return numberOfNodes;
  }

  /**
    Returns the physical id of the local node (i.e. the adapter).
  */
  inline unsigned int getLocalId() const noexcept
  {
    return localId;
  }

  /**
    Returns the physical id of the root node.
  */
  inline unsigned int getRootNode() const noexcept
  {
    return numberOfNodes - 1; // the largest physical id
  }

  /**
    Returns the physical id of the cycle master.
  */
  inline unsigned int getCycleMaster() const noexcept
  {
    return cycleMasterId;
  }
  
  /**
    Returns the physical id of the bus manager.
  */
  inline unsigned int getBusManager() const noexcept
  {
    return busManagerId;
  }

  /**
    Returns the physical id of the isochronous resource manager.
  */
  inline unsigned int getIsochronousResourceManager() const noexcept
  {
    return isochronousResourceManagerId;
  }

  /**
    Returns the cycle time (in units of 125 micro seconds) of the specified
    node. The node should be isochronous capable.
    
    @param node The node id.
  */
  unsigned int getCycleTime(unsigned short node);

  /**
    Returns the bus time in seconds of the specified node. The node should be
    cycle master capable.
    
    @param node The node id.
  */
  unsigned int getBusTime(unsigned short node);

  /**
    Returns the available bandwidth from the current isochronous resource
    manager.
  */
  unsigned int getAvailableBandwidth();
  
  /**
    Returns the available isochronous channels from the current isochronous
    reource manager.
  */
  uint64 getAvailableIsochronousChannels();
  
  /**
    Returns the maximum physical speed of the specified node.

    @param physicalId The physical id of the node.
  */
  Speed getMaximumSpeed(unsigned int physicalId) const;

  /**
    Returns the maximum link speed of the specifed local node.

    @param physicalId The physical id of the node.
  */
  Speed getMaximumLinkSpeed(unsigned int physicalId) const;
  
  /**
    Returns the maximum speed between the specified local nodes.

    @param a The physical id of the first node.
    @param b The physical id of the second node.
  */
  Speed getMaximumSpeedBetweenNodes(unsigned int a, unsigned int b) const;

  /**
    Returns the maximum speed to the specified node.

    @param physicalId The physical id of the node.
  */
  inline Speed getMaximumSpeedToNode(
    unsigned int physicalId) const
  {
    return getMaximumSpeedBetweenNodes(localId, physicalId);
  }

  /**
    Returns the maximum broadcast speed.
  */
  Speed getBroadcastSpeed() const;
  
  /**
    Returns the maximum speed supported by all the specified nodes (bit mask).
  */
  Speed getMaximumSpeed(uint64 nodes) const noexcept;
  
  /**
    Creates a IEEE 1394 object with the specified implementation.
  */
  template<class IMPL>
  static inline IEEE1394 make() noexcept
  {
    return IEEE1394(new IMPL());
  }
  
  /**
    Initializes IEEE 1394 with the default implementation.
  */
  IEEE1394();
  
  /**
    Initializes IEEE 1394 from other IEEE 1394 object.
  */
  inline IEEE1394(const IEEE1394& copy) noexcept
    : ieee1394impl(copy.ieee1394impl)
  {
  }
  
  /**
    Assignment of IEEE 1394 from other IEEE 1394 object.
  */
  inline IEEE1394& operator=(const IEEE1394& assign) noexcept
  {
    ieee1394impl = assign.ieee1394impl;
    return *this;
  }

  /**
    Returns true if the bus has been reset.
  */
  bool hasBeenReset() const noexcept
  {
    return ieee1394impl->hasBeenReset();
  }
  
  /**
    Acknowledges the bus reset.
  */
  inline void acknowledgeReset() noexcept
  {
    ieee1394impl->acknowledgeReset();
  }

  /**
    Resets the bus.
  */
  inline void resetBus()
  {
    ieee1394impl->resetBus();
  }
  
  /**
    Returns the adapters available.
  */
  inline Array<EUI64> getAdapters()
  {
    return ieee1394impl->getAdapters();
  }

  /**
    Opens a connection to the primary adapter.
  */
  void open();
  
  /**
    Opens a connection to the specified adapter.
    
    @param adapter The id of the adapter.
  */
  void open(const EUI64& adapter);
  
  /**
    Closes the handle to the adapter. The adapter is destroyed when all handles
    have been closed.
  */
  inline void close()
  {
    ieee1394impl->close();
  }
  
  /**
    Returns the guid's of the available nodes on the bus. The cached guid are
    used.
  */
  Array<EUI64> getNodes() noexcept;
  
  /**
    Returns the unique identifier of the specified local node. The returned
    guid is invalid if the node does not have a general configuration ROM.
    
    @param physicalId The physical id of the local node.
  */
  EUI64 getLocalIdentifier(unsigned int physicalId) const;
  
  /**
    Returns the IEEE 1394 standard of the specified node.
    
    @param node The node id.
  */
  Standard getCompliance(unsigned short node);
  
  /**
    Returns the unique identifier of the specified node. Raises
    IEEE1394Exception if the node has not general configuration ROM.
    
    @param node The node id.
  */
  EUI64 getIdentifier(unsigned short node);

  /**
    Returns the maximum payload in bytes for asynchronous write transmissions
    addressed to the specified node.
    
    @param node The node id.
  */
  unsigned int getMaximumPayload(unsigned short node);
  
  /**
    Returns the capabilities of the specified node.
    
    @param node The node id.
  */
  unsigned int getCapabilities(unsigned short node);
  
  /**
    Returns the 24 bit module vendor id of the specified node. Please note that
    this vendor id does not have to be identical to the vendor id of the EUI-64
    identifier.
    
    @param node The node id.
  */
  unsigned int getVendorId(unsigned short node);
  
  /**
    Returns the physical id ([0; 63[) of the node with the specified guid. The
    guid is allowed to be the invalid guid.
    
    @param guid The unique id of the node.

    @return -1 is not found on the local bus.
  */
  int getPhysicalId(const EUI64& guid) noexcept;
  
  /**
    Returns a description of the specified node if available in the
    configuration ROM.
    
    @param node The node id.
  */
  String getDescription(unsigned short node);

  /**
    Returns the keywords (separated by space) of the specified node if
    available in the configuration ROM.
    
    @param node The node id.
  */
  String getKeywords(unsigned short node);
  
  /**
    Returns the current error status.
  */
  inline unsigned int getStatus() const
  {
    return ieee1394impl->getStatus();
  }
  
  /**
    Returns the size of the FIFO.
  */
  inline unsigned int getFIFOSize() const
  {
    return ieee1394impl->getFIFOSize();
  }
  
  /**
    Returns true if the node with the specified physical id is present on the
    local bus.
    
    @param physicalId The physical id of the node [0; 63[.
    
    @return As a special case this method returns false for any invalid
    physical id.
  */
  inline bool isPresent(unsigned int physicalId) const noexcept
  {
    return physicalId < numberOfNodes;
  }

  /**
    Returns the nodes which have the link layer activated as a bit mask.
  */
  inline uint64 getLinkActiveNodes() const noexcept
  {
    return linkActiveNodes;
  }
  
  /**
    Returns true if the specified node has the link layer activated.

    @param physicalId The physical id of the node [0; 63[.
  */
  inline bool isLinkLayerActive(unsigned int physicalId) const
  {
    if (!(physicalId < numberOfNodes)) {
      _throw OutOfDomain(this);
    }
    return nodes[physicalId].link;
  }

  /**
    Returns the contender nodes as a bit mask.
  */
  inline uint64 getContenders() const noexcept
  {
    return contenderNodes;
  }
  
  /**
    Returns true if the specified node is a contended.
    
    @param physicalId The physical id of the node [0; 63[.
  */
  inline bool isContender(unsigned int physicalId) const
  {
    if (!(physicalId < numberOfNodes)) {
      _throw OutOfDomain(this);
    }
    return nodes[physicalId].contender;
  }
  
  /**
    Returns true if the local node is transaction capable.

    @param physicalId The physical id the node.    
  */
  inline bool isTransactionCapable(unsigned int physicalId) const
  {
    if (!(physicalId < numberOfNodes)) {
      _throw OutOfDomain(this);
    }
    return nodes[physicalId].link;
  }

  /**
    Returns the maximum payload of the specified local node.

    @param physicalId The physical id the node.
  */
  inline unsigned int getGapCount(unsigned int physicalId) const
  {
    if (!(physicalId < numberOfNodes)) {
      _throw OutOfDomain(this);
    }
    return nodes[physicalId].gapCount;
  }

  /**
    Returns the maximum payload of the specified local node.

    @param physicalId The physical id the node.
  */
  inline unsigned int getNumberOfPorts(unsigned int physicalId) const
  {
    if (!(physicalId < numberOfNodes)) {
      _throw OutOfDomain(this);
    }
    return nodes[physicalId].numberOfPorts;
  }

  /**
    Returns the state of the port of the local node.
    
    @param physicalId The physical id of the node.
    @param port The port of the node.
  */
  inline PortState getPortState(unsigned int physicalId, unsigned int port) const
  {
    bassert(
      (physicalId < numberOfNodes) && (port < nodes[physicalId].numberOfPorts),
      OutOfDomain(this)
    );
    return nodes[physicalId].ports[port];
  }
  
  /**
    Returns the maximum payload of the specified local node.

    @param physicalId The physical id the node.
  */
  inline unsigned int getLocalMaximumPayload(unsigned int physicalId) const
  {
    if (!(physicalId < numberOfNodes)) {
      _throw OutOfDomain(this);
    }
    return nodes[physicalId].maximumPayload;
  }
  
  /**
    Returns the power class of the specified local node.

    @param physicalId The physical id the node.
  */
  inline PowerClass getPowerClass(unsigned int physicalId) const
  {
    if (!(physicalId < numberOfNodes)) {
      _throw OutOfDomain(this);
    }
    return nodes[physicalId].powerClass;
  }
  
  /**
    Read quadlet from node.

    @param node The node id of source node.
    @param offset The offset of the quadlet from the CSR base address.
    
    @return The quadlet in native byte order.
  */
  inline uint32 getQuadlet(unsigned short node, uint32 offset)
  {
    Quadlet quadlet;
    ieee1394impl->read(
      node,
      IEEE1394::CSR_BASE_ADDRESS + offset,
      Cast::getAddress(quadlet),
      sizeof(quadlet)
    );
    return quadlet;
  }

  /**
    Read data from device.

    @param node The node id of source node.
    @param address The base address of the memory region to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read.
  */
  inline void read(
    unsigned short node,
    uint64 address,
    uint8* buffer,
    unsigned int size)
  {
    ieee1394impl->read(node, address, buffer, size);
  }

  /**
    Write data to device.

    @param node The node id of destination node.
    @param address The base address of the memory region to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write.
  */
  inline void write(
    unsigned short node,
    uint64 address,
    const uint8* buffer,
    unsigned int size)
  {
    ieee1394impl->write(node, address, buffer, size);
  }

  /**
    Read data from device. This method is only used for debugging and
    development.

    @param node The node id of source node.
    @param address The base address of the memory region to read from (must be
    a quadlet boundary).
    @param buffer The data buffer.
    @param size The number of quadlets to read (not bytes!). // TAG: FIXME
    @param value The default value to store in the buffer for unsuccessful
    reads.
    
    @return The number of quadlets read successfully.
  */
  inline unsigned int read(
    unsigned short node,
    uint64 address,
    uint32* buffer,
    unsigned int size,
    uint32 value)
  {
    return ieee1394impl->read(node, address, buffer, size, value);
  }
  
  /**
    Returns an isochronous read channel.

    @param maximumPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this
    channel.
  */
  inline IsochronousReadChannel getReadChannel(
    unsigned int maximumPacketsPerRequest,
    uint64 subchannels)
  {
    return ieee1394impl->getReadChannel(maximumPacketsPerRequest, subchannels);
  }
  
  /**
    Returns an isochronous write channel.

    @param maximumPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this
    channel.
  */
  inline IsochronousWriteChannel getWriteChannel(
    unsigned int maximumPacketsPerRequest,
    uint64 subchannels)
  {
    return ieee1394impl->getWriteChannel(maximumPacketsPerRequest, subchannels);
  }

  inline bool wait(unsigned int milliseconds)
  {
    return ieee1394impl->wait(milliseconds);
  }
  
  inline void dequeue()
  {
    ieee1394impl->dequeue();
  }
  
  inline void registerFCPListener(FunctionControlProtocolListener* listener)
  {
    ieee1394impl->registerFCPListener(listener);
  }
  
  inline void unregisterFCPListener()
  {
    ieee1394impl->unregisterFCPListener();
  }

  inline void readIsochronous(
    unsigned int channel,
    IsochronousChannelListener* listener
  )
  {
    unsigned int maximumPayload = getMaximumIsoPayloadForSpeed(
      getMaximumSpeed(getLocalId())
    ); // in bytes
    ieee1394impl->readIsochronous(channel, maximumPayload, listener);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
