/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_H

#include <base/communication/CommunicationsPort.h>
#include <base/communication/EUI64.h>
#include <base/communication/IEEE1394Impl.h>
#include <base/communication/IEEE1394Exception.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/ByteOrder.h>
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  IEEE-1394 (also known as FireWire (Apple Computer) and i.LINK (Sony Corp.))
  support. See 1394 Trade Association (http://www.1394ta.org).
  
  @short IEEE-1394 (FireWire)
  @ingroup communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class IEEE1394 : public CommunicationsPort, public IEEE1394Common {
public:

  /** Specifies the maximum number of retries. */
  static const unsigned int MAXIMUM_ATTEMPTS = 5;
  /** The CSR base address. */
  static const uint64 CSR_BASE_ADDRESS = 0xfffff0000000ULL;
  /** Broadcast id. */
  static const unsigned int BROADCAST = 63;
  
  typedef BigEndian<uint32> Quadlet;

  /** Control and status register (CSR). */
  enum CSRRegister {
    STATE_CLEAR = 0x000,
    STATE_SET = 0x004,
    NODE_IDS = 0x008,
    RESET_START = 0x00c,
    SPLIT_TIMEOUT_HI = 0x018,
    SPLIT_TIMEOUT_LO = 0x01c,
    ARGUMENT_HI = 0x0020,
    ARGUMENT_LO = 0x0024,
    TEST_START = 0x028,
    TEST_STATUS = 0x02c,
    CYCLE_TIME = 0x200,
    BUS_TIME = 0x204,
    POWER_FAIL_IMMINENT = 0x208,
    POWER_SOURCE = 0x20c,
    BUSY_TIMEOUT = 0x210,
    BANDWIDTH_AVAILABLE = 0x200,
    CHANNELS_AVAILABLE_HI = 0x224,
    CHANNELS_AVAILABLE_LO = 0x228,
    CONFIGURATION_ROM = 0x400,
    BUS_INFO_BLOCK = 0x404,
    FCP_COMMAND = 0xb00,
    FCP_RESPONSE = 0xd00,
    TOPOLOGY_MAP = 0x1000,
    SPEED_MAP = 0x2000
  };

  /** Control and status key type (see CSR Architecture). */
  enum CSRKeyType {
    IMMEDIATE = 0x00,
    OFFSET = 0x01,
    LEAF = 0x02,
    DIRECTORY = 0x03
  };
  
  /** Control and status key value (see CSR Architecture). */
  enum CSRKeyValue {
    TEXTUAL = 0x01,
    BUS_DEPENDENT_INFORMATION = 0x02,
    MODULE_VENDOR_ID = 0x03,
    MODULE_HARDWARE_VERSION = 0x04,
    SPECIFICATION_ID = 0x05,
    MODULE_SOFTWARE_VERSION = 0x06,
    MODULE_DEPENDENT_INFORMATION = 0x07,
    NODE_VENDOR_ID = 0x08,
    NODE_HARDWARE_VERSION = 0x09,
    NODE_SPECIFICATION_ID = 0x0a,
    NODE_SOFTWARE_VERSION = 0x0b,
    NODE_CAPABILITIES = 0x0c,
    NODE_UNIQUE_ID = 0x0d,
    NODE_UNITS_EXTENT = 0x0e,
    NODE_MEMORY_EXTENT = 0x0f,
    NODE_DEPENDENT_INFORMATION = 0x10,
    UNIT_DIRECTORY = 0x11,
    UNIT_SPECIFICATION_ID = 0x12,
    UNIT_SOFTWARE_VERSION = 0x13,
    UNIT_DEPENDENT_INFORMATION = 0x14
  };

  static inline unsigned char makeCSRkey(CSRKeyType type, CSRKeyValue value) throw() {
    return (static_cast<unsigned char>(type) << 6) | static_cast<unsigned char>(value);
  }
  
  /**
    This exception is raised on bus resets. The reset must be acknowledged
    to bring the IEEE-1394 interface back into the working operating mode.
    
    @short IEEE 1394 bus reset exception.
    @ingroup exceptions communication
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class BusReset {
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
    ISOCHRONOUS_RESOURCE_MANAGER = 1 << 0,
    CYCLE_MASTER = 1 << 1,
    ISOCHRONOUS_TRANSMISSION = 1 << 2,
    BUS_MASTER = 1 << 3,
    POWER_MANAGEMENT = 1 << 4
  };

  struct BusInfo {
    BigEndian<uint32> name;
    BigEndian<uint32> flags;
    uint8 guid[8];
  } _DK_SDU_MIP__BASE__PACKED;

  struct RootDirectory {
    BigEndian<uint32> crc;
    BigEndian<uint32> vendorId;
    BigEndian<uint32> capabilities;
    BigEndian<uint32> nodeOffset;
    BigEndian<uint32> deviceDirectoryOffset;
  } _DK_SDU_MIP__BASE__PACKED;

  struct DeviceIndependentDirectory {
    BigEndian<uint32> crc;
    BigEndian<uint32> specification;
    BigEndian<uint32> version;
    BigEndian<uint32> dependentOffset;
  } _DK_SDU_MIP__BASE__PACKED;

  struct DeviceDependentDirectory {
    BigEndian<uint32> crc;
    BigEndian<uint32> commandRegisters;
    BigEndian<uint32> vendorNameLeaf;
    BigEndian<uint32> modelNameLeaf;
  } _DK_SDU_MIP__BASE__PACKED;

  struct ConfigurationIntro {
    BigEndian<uint32> crc;
    BusInfo busInfo;
  } _DK_SDU_MIP__BASE__PACKED;
private:

  /** The IEEE 1394 implementation. */
  ReferenceCountedObjectPointer<IEEE1394Impl> ieee1394impl;

  inline IEEE1394(IEEE1394Impl* _ieee1394impl) throw() : ieee1394impl(_ieee1394impl) {
  }
protected:

  bool reset;
  EUI64 adapterGuid;
  unsigned int adapterId;
  uint64 presentNodes;
  uint64 linkOnNodes;
  Descriptor node[63];
public:

  /**
    Creates a IEEE 1394 object with the specified implementation.
  */
  template<class IMPL>
  static inline IEEE1394 make() throw() {
    return IEEE1394(new IMPL());
  }
  
  /**
    Initializes IEEE 1394 with the default implementation.
  */
  IEEE1394() throw(IEEE1394Exception);
  
  /**
    Initializes IEEE 1394 from other IEEE 1394 object.
  */
  inline IEEE1394(const IEEE1394& copy) throw() : ieee1394impl(copy.ieee1394impl) {
  }
  
  /**
    Assignment of IEEE 1394 from other IEEE 1394 object.
  */
  inline IEEE1394& operator=(const IEEE1394& eq) throw() {
    ieee1394impl = eq.ieee1394impl;
    return *this;
  }

  /**
    Returns true if the bus has been reset.
  */
  bool hasBeenReset() const throw() {
    return ieee1394impl->hasBeenReset();
  }
  
  /**
    Acknowledges the bus reset.
  */
  inline void acknowledgeReset() throw() {
    ieee1394impl->acknowledgeReset();
  }
  
  /**
    Returns the adapters available.
  */
  inline Array<EUI64> getAdapters() throw(IEEE1394Exception) {
    return ieee1394impl->getAdapters();
  }

  /**
    Opens a connection to the primary adapter.
  */
  inline void open() throw(IEEE1394Exception) {
    ieee1394impl->open();
  }
  
  /**
    Opens a connection to the specified adapter.
    
    @param adapter The id of the adapter.
  */
  inline void open(const EUI64& adapter) throw(IEEE1394Exception) {
    ieee1394impl->open(adapter);
  }
  
  /**
    Closes the handle to the adapter. The adapter is destroyed when all handles
    have been closed.
  */
  inline void close() throw(IEEE1394Exception) {
    ieee1394impl->close();
  }
  
  /**
    Returns the guid's of the available nodes on the bus.
  */
  Array<EUI64> getNodes() throw(IEEE1394Exception);
  
  /**
    Returns the GUID of the adapter.
  */
  inline EUI64 getGuid() const throw() {
    return adapterGuid;
  }

  /**
    Returns the physical id of the adapter.
  */
  inline unsigned int getPhysicalId() const throw() {
    return adapterId;
  }
  
  /**
    Returns the IEEE 1394 standard of the adapter.
  */
  inline Standard getCompliance() const throw(IEEE1394Exception) {
    return ieee1394impl->getCompliance();
  }
  
  /**
    Returns the vendor name of the specified node.
    
    @param node The physical id of the node [0; 63[.
  */
  String getVendorName(unsigned int node) throw(IEEE1394Exception);

  /**
    Returns the model name of the specified node.
    
    @param node The physical id of the node [0; 63[.
  */
  String getModelName(unsigned int node) throw(IEEE1394Exception);
  
  /**
    Returns the unique identifier of the specified node.

    @param node The physical id of the node [0; 63[.
  */
  EUI64 getIdentifier(unsigned int node) throw(OutOfDomain, IEEE1394Exception);

  /**
    Returns the maximum payload in bytes for asynchronous write transmissions
    addressed to the specified node.
    
    @param node The physical id of the node [0; 63[.
  */
  unsigned int getMaximumPayload(unsigned int node) throw(IEEE1394Exception);
  
  /**
    Returns the capabilities of the specified node.
    
    @param node The physical id of the node [0; 63[.
  */
  unsigned int getCapabilities(unsigned int node) throw(IEEE1394Exception);
  
  /**
    Returns the 24 bit module vendor id of the specified node. Please note that
    this vendor id does not have to be identical to the vendor id of the EUI-64
    identifier.
    
    @param node The physical id of the node [0; 63[.
  */
  unsigned int getVendorId(unsigned int node) throw(IEEE1394Exception);
  
  /**
    Returns the physical id ([0; 63[) of the specified device.
    
    @param guid The unique id of the device.

    @return -1 is not found.
  */
  int getPhysicalId(const EUI64& guid) throw(IEEE1394Exception);
  
  /**
    Returns the current error status.
  */
  inline unsigned int getStatus() const throw(IEEE1394Exception) {
    return ieee1394impl->getStatus();
  }
  
  /**
    Returns the size of the FIFO.
  */
  inline unsigned int getFIFOSize() const throw(IEEE1394Exception) {
    return ieee1394impl->getFIFOSize();
  }
  
  /**
    Returns the present nodes on the bus.
  */
  inline uint64 getPresentNodes() const throw(IEEE1394Exception) {
    return ieee1394impl->getPresentNodes();
  }

  /**
    Returns true if the specified node is a contended for...

    @param node The physical id of the node [0; 63[.
  */
  inline bool isPresent(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
    assert(node < BROADCAST, OutOfDomain(this));
    return (getPresentNodes() >> node) & 1;
  }

  /**
    Returns the nodes which have the link layer activated.
  */
  inline uint64 getLinkOnNodes() const throw(IEEE1394Exception) {
    return ieee1394impl->getLinkOnNodes();
  }
  
  /**
    Returns true if the specified node has the link layer activated.

    @param node The physical id of the node [0; 63[.
  */
  inline bool isLinkLayerActivated(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
    assert(node < BROADCAST, OutOfDomain(this));
    return (getLinkOnNodes() >> node) & 1;  
  }

  /**
    Returns the contender nodes.
  */
  inline uint64 getContenders() const throw(IEEE1394Exception) {
    return ieee1394impl->getContenders();
  }
  
  /**
    Returns true if the specified node is a contended.
    
    @param node The physical id of the node [0; 63[.
  */
  inline bool isContender(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {    
    assert(node < BROADCAST, OutOfDomain(this));
    return (getContenders() >> node) & 1;
  }
  
  /**
    Returns the maximum speed of a node.
    
    @param node The physical id the the node.
  */
  inline unsigned int getSpeedOfNode(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
    return ieee1394impl->getSpeedOfNode(node);
  }
  
  /**
    Returns the maximum speed for communication with node.

    @param node The physical id the the node.
  */
  inline unsigned int getMaximumSpeedToNode(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
    return ieee1394impl->getMaximumSpeedToNode(node);
  }
  
  /**
    Returns the maximum broadcast speed.
  */
  inline unsigned int getBroadcastSpeed() const throw(IEEE1394Exception) {
    return ieee1394impl->getBroadcastSpeed();
  }
  
  /**
    Read data from device.

    @param node The physical id of source node.
    @param address The base address of the memory region to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read.
  */
  inline unsigned int read(unsigned short node, uint64 address, char* buffer, unsigned int size) const throw(OutOfDomain, IOException) {
    return ieee1394impl->read(node, address, buffer, size);
  }

  /**
    Write data to device.

    @param node The physical id of destination node.
    @param address The base address of the memory region to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write.
  */
  inline unsigned int write(unsigned short node, uint64 address, const char* buffer, unsigned int size) throw(OutOfDomain, IOException) {
    return ieee1394impl->write(node, address, buffer, size);
  }

  /**
    Read data from device. This method is only used for debugging and development.

    @param node The physical id of source node.
    @param address The base address of the memory region to read from (must be a quadlet boundary).
    @param buffer The data buffer.
    @param size The number of quadlets to read (not bytes!).
    @param value The default value to store in the buffer for unsuccessful reads.
    
    @return The number of quadlets read successfully.
  */
  inline unsigned int read(unsigned short node, uint64 address, uint32* buffer, unsigned int size, uint32 value) const throw(OutOfDomain, IEEE1394Exception) {
    return ieee1394impl->read(node, address, buffer, size, value);
  }
  
  /**
    Returns an isochronous read channel.

    @param maximumPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this channel.
  */
  inline IsochronousReadChannel getReadChannel(unsigned int maximumPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
    return ieee1394impl->getReadChannel(maximumPacketsPerRequest, subchannels);
  }
  
  /**
    Returns an isochronous write channel.

    @param maximumPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this channel.
  */
  inline IsochronousWriteChannel getWriteChannel(unsigned int maximumPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
    return ieee1394impl->getWriteChannel(maximumPacketsPerRequest, subchannels);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
