/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/IEEE1394.h>
#include <base/concurrency/Thread.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define _DK_SDU_MIP__BASE__IEEE_1394_IMPL UnibrainIEEE1394
#  include <base/communication/UnibrainIEEE1394.cpp>
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
#  define _DK_SDU_MIP__BASE__IEEE_1394_IMPL LinuxRawIEEE1394
#  include <base/communication/LinuxRawIEEE1394.cpp>
#else
#  define _DK_SDU_MIP__BASE__IEEE_1394_IMPL DummyIEEE1394
#  include <base/communication/DummyIEEE1394.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IEEE1394::IEEE1394() throw(IEEE1394Exception) {
  ieee1394impl = new _DK_SDU_MIP__BASE__IEEE_1394_IMPL();
}

void IEEE1394::IsochronousRequestImpl::setOptions(unsigned int options) throw(IEEE1394Exception) {
  assert(
    status == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  this->options = options;
}

void IEEE1394::IsochronousRequestImpl::reset() throw(IEEE1394Exception) {
  assert(
    status != PENDING,
    bindCause(IEEE1394Exception("Request is pending", this), IEEE1394::REQUEST_NOT_PENDING)
  );
  resetContext();
  status = READY;
}

IEEE1394::IsochronousRequestImpl::~IsochronousRequestImpl() throw() { // TAG: is this required
}

IEEE1394::IsochronousReadRequestImpl::IsochronousReadRequestImpl() throw()
  : subchannel(0),
    buffer(0),
    bufferSize(0),
    receivedPackets(0) {
  // TAG: subchannel must be reserved - at least one reserved channel
}

void IEEE1394::IsochronousReadRequestImpl::setSubchannel(unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception) {
  assert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  assert(subchannel <= IEEE1394::BROADCAST, OutOfDomain(this));
  this->subchannel = subchannel;
}

void IEEE1394::IsochronousReadRequestImpl::setBuffer(char* buffer, unsigned int size) throw(IEEE1394Exception) {
  assert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  this->buffer = buffer;
  this->bufferSize = size;
}

void IEEE1394::IsochronousReadFixedPacketsRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  assert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  assert(payload % sizeof(IEEE1394::Quadlet) == 0, OutOfDomain(this));
  this->payload = payload;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setNumberOfPackets(unsigned int packets) throw(OutOfDomain, IEEE1394Exception) {
  assert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  this->numberOfPackets = packets;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setHeaderSize(unsigned int size) throw(OutOfDomain, IEEE1394Exception) {
  assert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  assert(payload % sizeof(IEEE1394::Quadlet) == 0, OutOfDomain(this));
  this->headerSize = size;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  assert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  assert(payload % sizeof(IEEE1394::Quadlet) == 0, OutOfDomain(this));
  this->payload = payload;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setBuffer(char* buffer, unsigned int size, char* secondaryBuffer) throw(IEEE1394Exception) {
  IsochronousReadRequestImpl::setBuffer(buffer, size);
  this->secondaryBuffer = secondaryBuffer;
}

IEEE1394::IsochronousWriteRequestImpl::IsochronousWriteRequestImpl() throw()
  : buffer(0),
    bufferSize(0),
    numberOfPackets(0),
    speed(100),
    transmittedPackets(0) {
}

void IEEE1394::IsochronousWriteRequestImpl::setBuffer(const char* buffer, unsigned int size, unsigned int numberOfPackets) throw(IEEE1394Exception) {
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  this->buffer = buffer;
  this->bufferSize = size;
  this->numberOfPackets = numberOfPackets;
}

// void IEEE1394::IsochronousReadChannel::queue(IsochronousReadRequest& request) throw(IEEE1394Exception) {
//   ReferenceCountedObjectPointer<IsochronousReadRequestImpl> request = request.getContext();
//   void* requestImpl = context.getValue();
  
//   IsochronousReadPacketsRequestImpl* readPacketsRequest = dynamic_cast<IsochronousReadPacketsRequestImpl*>(requestImpl);
//   if (readPacketsRequest) {
//     queue(readPacketsRequest);
//     return;
//   }
//   IsochronousReadFixedPacketsRequestImpl* readFixedPacketsRequest = dynamic_cast<IsochronousReadFixedPacketsRequestImpl*>(requestImpl);
//   if (readFixedPacketsRequest) {
//     queue(readFixedPacketsRequest);
//     return;
//   }
//   IsochronousReadFixedDataRequestImpl* readFixedDataRequest = dynamic_cast<IsochronousReadFixedDataRequestImpl*>(requestImpl);
//   if (readFixedDataRequest) {
//     queue(readFixedDataRequest);
//     return;
//   }
//   throw IEEE1394Exception("Invalid request", this);
// }

void IEEE1394::IsochronousWriteRequestImpl::setSpeed(unsigned int speed) throw(OutOfDomain, IEEE1394Exception) {
  assert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  switch (speed) {
  case 100:
  case 200:
  case 400:
  case 800:
  case 1600:
  case 3200:
    this->speed = speed;
    break;
  default:
    throw OutOfDomain(this);
  }
}

void IEEE1394::IsochronousWriteDataRequestImpl::setBuffer(const char* buffer, unsigned int size, unsigned int numberOfPackets, const char* secondaryBuffer) throw(IEEE1394Exception) {
  IsochronousWriteRequestImpl::setBuffer(buffer, size, numberOfPackets);
  this->secondaryBuffer = secondaryBuffer;
}

Array<EUI64> IEEE1394::getNodes() throw() {
  Array<EUI64> result;
  for (unsigned int id = 0; id < numberOfNodes; ++id) {
    result.append(nodes[id].guid);
  }
  return result;
}

void IEEE1394::open() throw(IEEE1394Exception) {
  ieee1394impl->open();
  reload();
}

void IEEE1394::open(const EUI64& adapter) throw(IEEE1394Exception) {
  ieee1394impl->open(adapter);
  reload();
}

EUI64 IEEE1394::getLocalIdentifier(unsigned int physicalId) const throw(OutOfDomain) {
  assert(physicalId < numberOfNodes, OutOfDomain(this));
  return nodes[physicalId].guid;
}

EUI64 IEEE1394::getIdentifier(unsigned short node) throw(IEEE1394Exception) {
  IEEE1394::Quadlet crc;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM,
    Cast::getCharAddress(crc),
    sizeof(crc)
  );
  
  assert(
    ((crc >> 24) * sizeof(IEEE1394::Quadlet)) >= sizeof(IEEE1394::BusInfo),
    bindCause(IEEE1394Exception("No general configuration ROM", this), IEEE1394::NO_GENERAL_CONFIGURATION_ROM)
  );
  
  BusInfo busInfo;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK,
    Cast::getCharAddress(busInfo),
    sizeof(busInfo)
  );

  assert(
    busInfo.name == 0x31333934,
    bindCause(IEEE1394Exception("No general configuration ROM", this), IEEE1394::NO_GENERAL_CONFIGURATION_ROM)
  );
  
  return EUI64(busInfo.guid);
}

unsigned int IEEE1394::getMaximumPayload(unsigned short node) throw(IEEE1394Exception) {
  Quadlet quadlet;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM,
    Cast::getCharAddress(quadlet),
    sizeof(quadlet)
  );
  assert(
    ((quadlet >> 24) * sizeof(Quadlet)) >= sizeof(BusInfo),
    bindCause(IEEE1394Exception("No general configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );

  BusInfo busInfo;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK,
    Cast::getCharAddress(busInfo),
    sizeof(busInfo)
  );
  assert(busInfo.name == 0x31333934, bindCause(IEEE1394Exception(this), INVALID_BUS_INFORMATION_BLOCK));
  return 1 << ((busInfo.flags >> 12) & 0x0f + 1);
}

unsigned int IEEE1394::getCapabilities(unsigned short node) throw(IEEE1394Exception) {
  try {
    Quadlet quadlet;
    read(
      node,
      IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM,
      Cast::getCharAddress(quadlet),
      sizeof(quadlet)
    );
    
    if (((quadlet >> 24) * sizeof(Quadlet)) >= sizeof(BusInfo)) {
      BusInfo busInfo;
      read(
        node,
        IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK,
        Cast::getCharAddress(busInfo),
        sizeof(busInfo)
      );

      if (busInfo.name == 0x31333934) {
        unsigned int capabilities = 0;
        capabilities |= ((busInfo.flags >> 31) & 1) ? ISOCHRONOUS_RESOURCE_MANAGER : 0;
        capabilities |= ((busInfo.flags >> 30) & 1) ? CYCLE_MASTER : 0;
        capabilities |= ((busInfo.flags >> 29) & 1) ? ISOCHRONOUS_TRANSMISSION : 0;
        capabilities |= ((busInfo.flags >> 28) & 1) ? BUS_MASTER : 0;
//    capabilities |= (false) ? POWER_MANAGEMENT : 0; // TAG: fixme
        return capabilities;
      }
    }
  } catch (IEEE1394Exception& e) {
  }
  return 0;
}

unsigned int IEEE1394::getVendorId(unsigned short node) throw(IEEE1394Exception) {
  ConfigurationIntro config;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM,
    Cast::getCharAddress(config),
    sizeof(config)
  );
  
  if ((config.crc >> 24) == 1) { // check for minimal ROM
    return config.crc & 0x00ffffff;
  }
  
  assert(
    ((config.crc >> 24) * sizeof(IEEE1394::Quadlet) >= 2) && // check for general ROM format
    (config.busInfo.name == 0x31333934), // "1394"
    bindCause(IEEE1394Exception("Invalid configuration ROM", this), IEEE1394::INVALID_BUS_INFORMATION_BLOCK)
  );

  const uint64 rootDirectoryAddress =
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM + ((config.crc >> 24) + 1) * sizeof(IEEE1394::Quadlet);
  IEEE1394::Quadlet quadlet;
  
  read(
    node,
    rootDirectoryAddress,
    Cast::getCharAddress(quadlet),
    sizeof(quadlet)
  );
  unsigned int numberOfEntries = quadlet >> 16;
  for (unsigned int entry = 1; entry <= numberOfEntries; ++entry) { // find mandatory Vendor_ID entry
    const uint64 entryAddress = rootDirectoryAddress + entry * sizeof(quadlet);
    read(node, entryAddress, Cast::getCharAddress(quadlet), sizeof(quadlet));
    IEEE1394::CSRKeyType keyType = getCSRKeyType(quadlet);
    IEEE1394::CSRKeyValue keyValue = getCSRKeyValue(quadlet);
    if ((keyType == IEEE1394::KEY_TYPE_IMMEDIATE) && (keyValue == IEEE1394::KEY_MODULE_VENDOR_ID)) {
      return quadlet & 0x00ffffff;
      // TAG: Basler camera has 0x0c as keyValue
    }
  }
  throw IEEE1394Exception("Invalid configuration ROM", this);
}

int IEEE1394::getPhysicalId(const EUI64& guid) throw() {
  for (unsigned int id = 0; id < numberOfNodes; ++id) {
    if (nodes[id].guid == guid) {
      return id;
    }
  }
  return -1; // not found
}

String IEEE1394::getDescription(unsigned short node) throw(IEEE1394Exception) {
  ConfigurationIntro config;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM,
    Cast::getCharAddress(config),
    sizeof(config)
  );

  assert(
    ((config.crc >> 24) * sizeof(IEEE1394::Quadlet) >= 2) && // check for general ROM format
    (config.busInfo.name == 0x31333934), // "1394"
    IEEE1394Exception(this)
  );

  const uint64 rootDirectoryAddress =
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM + ((config.crc >> 24) + 1) * sizeof(IEEE1394::Quadlet);
  IEEE1394::Quadlet quadlet;

  // read root directory entries
  read(node, rootDirectoryAddress, Cast::getCharAddress(quadlet), sizeof(quadlet));
  unsigned int numberOfEntries = quadlet >> 16;
  for (unsigned int entry = 1; entry <= numberOfEntries; ++entry) {
    const uint64 entryAddress = rootDirectoryAddress + entry * sizeof(quadlet);
    read(node, entryAddress, Cast::getCharAddress(quadlet), sizeof(quadlet));
    
    IEEE1394::CSRKeyValue keyValue = getCSRKeyValue(quadlet);
    if (keyValue == IEEE1394::KEY_TEXTUAL) {
      IEEE1394::CSRKeyType keyType = getCSRKeyType(quadlet);
      if (keyType == IEEE1394::KEY_TYPE_LEAF) {
        const uint64 leafAddress = entryAddress + (quadlet & 0x00ffffff) * sizeof(IEEE1394::Quadlet);
        read(node, leafAddress, Cast::getCharAddress(quadlet), sizeof(quadlet));
        unsigned int leafSize = quadlet >> 16;
        if (leafSize > 2) {
          char leaf[leafSize * sizeof(IEEE1394::Quadlet)];
          read(node, leafAddress + sizeof(quadlet), leaf, leafSize * sizeof(quadlet));
          if (!((leaf[0] == 0) && (leaf[1] == 0) && (leaf[2] == 0) && (leaf[3] == 0) &&
                (leaf[4] == 0) && (leaf[5] == 0) && (leaf[6] == 0) && (leaf[7] == 0))) {
            break;
          }
          return String(leaf + 2 * sizeof(quadlet), (leafSize - 2) * sizeof(quadlet));
        }
      } else if (keyType == IEEE1394::KEY_TYPE_DIRECTORY) {
        // TAG: search for a nice leaf
      }
    }
  }
  return String();
}

String IEEE1394::getKeywords(unsigned short node) throw(IEEE1394Exception) {
  // TAG: also search subdirectories?
  ConfigurationIntro config;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM,
    Cast::getCharAddress(config),
    sizeof(config)
  );
  
  assert(
    ((config.crc >> 24) * sizeof(IEEE1394::Quadlet) >= 2) && // check for general ROM format
    (config.busInfo.name == 0x31333934), // "1394"
    IEEE1394Exception(this)
  );

  const uint64 rootDirectoryAddress =
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM + ((config.crc >> 24) + 1) * sizeof(IEEE1394::Quadlet);
  IEEE1394::Quadlet quadlet;

  // read root directory entries
  read(node, rootDirectoryAddress, Cast::getCharAddress(quadlet), sizeof(quadlet));
  unsigned int numberOfEntries = quadlet >> 16;
  for (unsigned int entry = 1; entry <= numberOfEntries; ++entry) {
    const uint64 entryAddress = rootDirectoryAddress + entry * sizeof(quadlet);
    read(node, entryAddress, Cast::getCharAddress(quadlet), sizeof(quadlet));

    IEEE1394::CSRKeyValue keyValue = getCSRKeyValue(quadlet);
    IEEE1394::CSRKeyType keyType = getCSRKeyType(quadlet);
    if ((keyType == IEEE1394::KEY_TYPE_LEAF) && (keyValue == IEEE1394::KEY_KEYWORD)) { // Keyword_Leaf
      const uint64 leafAddress = entryAddress + (quadlet & 0x00ffffff) * sizeof(IEEE1394::Quadlet);
      read(node, leafAddress, Cast::getCharAddress(quadlet), sizeof(quadlet));
      unsigned int leafSize = quadlet >> 16;
      if (leafSize > 2) {
        char leaf[leafSize * sizeof(IEEE1394::Quadlet)];
        read(node, leafAddress + sizeof(quadlet), leaf, leafSize * sizeof(quadlet));
        String result;
        for (unsigned int i = 0; i < leafSize * sizeof(IEEE1394::Quadlet);) {
          unsigned int j = i;
          while ((j < leafSize * sizeof(IEEE1394::Quadlet)) && leaf[j]) {
            ++j;
          }
          if (j == i) {
            break;
          }
          result += String(leaf[i]);
          result += MESSAGE(" ");
        }
        return result; // TAG: remove last space
      }
    }
  }
  return String();
}

void IEEE1394::checkResetGeneration() throw(IEEE1394Exception) {
  // IEEE 1394 implementation should provide reset detection support
  try {
    EUI64 guid = getIdentifier(makeNodeId(busManagerId));
    
    if (guid == nodes[busManagerId].guid) {
      // get generation from topology map
      IEEE1394::Quadlet generation;
      read(
        makeNodeId(busManagerId),
        IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP + sizeof(IEEE1394::Quadlet),
        Cast::getCharAddress(generation),
        sizeof(generation)
      );

      if (generation == resetGeneration) {
        return;
      }
    }
  } catch (IEEE1394Exception& e) {
  }
  reload(); // responsible for setting the new generation number
}

void IEEE1394::reload() throw(IEEE1394Exception) {
  for (unsigned int attempts = 5; ; --attempts) {
    // invalidate current attributes
    numberOfNodes = 0;
    localId = IEEE1394::BROADCAST;
    busManagerId = IEEE1394::BROADCAST;
    isochronousResourceManagerId = IEEE1394::BROADCAST;
    linkActiveNodes = 0;
    contenderNodes = 0;

    for (unsigned id = 0; id < getArraySize(nodes); ++id) {
      nodes[id].present = false;
      nodes[id].link  = false;
      nodes[id].contender = false;
      nodes[id].guid = EUI64();
      nodes[id].standard = IEEE1394::STANDARD_UNSPECIFIED;
      nodes[id].initiatedReset = false;
      
      for (unsigned int port = 0; port < getArraySize(nodes[id].ports); ++port) {
        nodes[id].ports[port] = IEEE1394::PORT_NOT_CONNECTED;
      }
    }
    
    const unsigned int nodes = ieee1394impl->getNumberOfNodes();
    assert((nodes > 0) && (attempts > 0), IEEE1394Exception("Unable to query local bus", this));
    
    try {      
      // find bus manager
      for (int id = nodes - 1; id >= 0; --id) {
        try {
          IEEE1394::Quadlet quadlet = 0x3f; // TAG: Raw module does not return with error
          read(
            makeNodeId(id),
            IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_MANAGER_ID,
            Cast::getCharAddress(quadlet),
            sizeof(quadlet)
          );
          if ((quadlet & ((1 << 6) - 1)) == id) {
            busManagerId = id;
            break;
          }
        } catch (IEEE1394Exception& e) {
        }
      }
      if (busManagerId == IEEE1394::BROADCAST) {
        busManagerId = 0;
      }
      
      loadTopologyMap();
      loadSpeedMap(); // TAG: deprecated in later specification
      
      // get generation from topology map
      IEEE1394::Quadlet generation;
      read(
        makeNodeId(busManagerId),
        IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP + sizeof(IEEE1394::Quadlet),
        Cast::getCharAddress(generation),
        sizeof(generation)
      );
      
      localId = ieee1394impl->getLocalId();
      resetGeneration = generation;
      break; // success
    } catch (IEEE1394Exception& e) {
      Thread::millisleep(25);
    }
  }
}

void IEEE1394::loadTopologyMap() throw(IEEE1394Exception) {
  const unsigned short node = makeNodeId(busManagerId);
  
  IEEE1394::Quadlet crc;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP,
    Cast::getCharAddress(crc),
    sizeof(crc)
  );

  const unsigned int length = crc >> 16;
  assert(length >= 3, IEEE1394Exception(this));
  
  IEEE1394::Quadlet count;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP + 2 * sizeof(IEEE1394::Quadlet),
    Cast::getCharAddress(count),
    sizeof(count)
  );
  
  numberOfNodes = count >> 16;
  const unsigned int selfIds = count & 0xffff;
  
  assert(numberOfNodes < IEEE1394::BROADCAST, IEEE1394Exception(this));
  assert(
    (selfIds >= 1) && (selfIds <= (63 * 9) && (selfIds <= length - 2)),
    IEEE1394Exception(this)
  );
  
  // get topology map
  IEEE1394::Quadlet ids[selfIds];
  uint64 address =
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP + 3 * sizeof(IEEE1394::Quadlet);
  for (unsigned int i = 0; i < selfIds; ++i) {
    read(node, address, Cast::getCharAddress(ids[i]), sizeof(ids[i]));
    address += sizeof(IEEE1394::Quadlet);
  }
  
  // get crc
  IEEE1394::Quadlet quadlet;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP,
    Cast::getCharAddress(quadlet),
    sizeof(quadlet)
  );
  
  assert(quadlet == crc, IEEE1394Exception("Unable to load topology map", this));
  
  // copy to internal representation
  unsigned int physicalId = 0; // the current physical id
  for (unsigned int i = 0; i < selfIds;) {
    uint32 current = ids[i++];
    
    assert(
      (getBits(current, 30, 2) == 2) && // self-id identifier
      (getBits(current, 24, 6) == physicalId) && // same node
      (getBits(current, 23, 1) == 0), // initial self-id
      IEEE1394Exception("Invalid topology map", this)
    );

    nodes[physicalId].present = true;
    nodes[physicalId].link = getBits(current, 22, 1);
    linkActiveNodes |= getBits(current, 22, 1) << physicalId;
    nodes[physicalId].contender = getBits(current, 11, 1);
    contenderNodes |= getBits(current, 11, 1) << physicalId;
    nodes[physicalId].speed = static_cast<Speed>(getBits(current, 14, 2));
    nodes[physicalId].standard = IEEE1394::STANDARD_IEEE_1394;
    nodes[physicalId].initiatedReset = getBits(current, 1, 1);
//  other value are:
//     selfId.gapCount = getBits(current, 16, 6);
//     selfId.delay = getBits(current, 12, 2);
//     selfId.power = getBits(current, 8, 3);

    try {
      nodes[physicalId].guid = getIdentifier(makeNodeId(physicalId));
    } catch (IEEE1394Exception& e) {
      // guid already invalid
    }
    
    unsigned int port = 0;
    nodes[physicalId].ports[port++] = static_cast<PortState>(getBits(current, 6, 2));
    nodes[physicalId].ports[port++] = static_cast<PortState>(getBits(current, 4, 2));
    nodes[physicalId].ports[port++] = static_cast<PortState>(getBits(current, 2, 2));

    // read "more" packets
    if (i < selfIds) {
      uint32 previous = current;
      current = ids[i];
      for (unsigned int n = 0; ((i < selfIds) && // has self-id's
                                (getBits(previous, 0, 1) == 1) && // "more" is expected
                                (getBits(current, 24, 6) == physicalId) && // same node
                                (getBits(current, 23, 1) == 1) && // "more" packet
                                (getBits(current, 20, 3) == n)); ++n) { // expected more
        if (n < 3) { // only use first 3 "more" self-ids
          for (unsigned int field = 8; field > 0; --field) {
            nodes[physicalId].ports[port++] = // ports has room for 3+3*8 elements
              static_cast<PortState>(getBits(current, 2 * field + 2, 2));
          }
        }
        previous = current;
        current = ids[i++];
      }
    }
    ++physicalId;
  }
}

unsigned int IEEE1394::getCycleTime(unsigned short node) throw(IEEE1394Exception) {
  IEEE1394::Quadlet quadlet;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CYCLE_TIME,
    Cast::getCharAddress(quadlet),
    sizeof(quadlet)
  );
  unsigned int units = (quadlet >> 12) & ((1 << 13) - 1); // 125 micro seconds per unit
  unsigned int seconds = (quadlet >> (12 + 13));
  return seconds * 8000 + units;
}

unsigned int IEEE1394::getBusTime(unsigned short node) throw(IEEE1394Exception) {
  IEEE1394::Quadlet quadlet;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_TIME,
    Cast::getCharAddress(quadlet),
    sizeof(quadlet)
  );
  return quadlet;
}

unsigned int IEEE1394::getAvailableBandwidth() throw(IEEE1394Exception) {
  IEEE1394::Quadlet quadlet;
  read(
    makeNodeId(isochronousResourceManagerId),
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BANDWIDTH_AVAILABLE,
    Cast::getCharAddress(quadlet),
    sizeof(quadlet)
  );
  return quadlet & ((1 << 13) - 1);
}

uint64 IEEE1394::getAvailableIsochronousChannels() throw(IEEE1394Exception) {
  IEEE1394::Quadlet channels[2];
  read(
    makeNodeId(isochronousResourceManagerId),
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CHANNELS_AVAILABLE_HI,
    Cast::getCharAddress(channels),
    sizeof(channels)
  );
  return (static_cast<uint64>(channels[1]) << 32) | channels[0];
}

void IEEE1394::loadSpeedMap() throw(IEEE1394Exception) {
  const unsigned short node = makeNodeId(busManagerId);
  
  IEEE1394::Quadlet crc;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::SPEED_MAP,
    Cast::getCharAddress(crc),
    sizeof(crc)
  );
  
  if ((crc >> 16) != ((64 * 62 + 62 + 3)/4 + 1)) {
    throw IEEE1394Exception("Invalid speed map", this);
  }
  
  // get speeds
  IEEE1394::Quadlet speeds[(64 * 62 + 62 + 3)/4];
  unsigned int numberOfSpeeds = 64 * numberOfNodes + numberOfNodes;
  uint64 address =
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::SPEED_MAP + 2 * sizeof(IEEE1394::Quadlet);
  for (unsigned int i = 0; i < numberOfSpeeds; ++i) {
    read(node, address, Cast::getCharAddress(speeds[i]), sizeof(speeds[i]));
    address += sizeof(speeds[i]);
  }
  
  // get crc
  IEEE1394::Quadlet quadlet;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::SPEED_MAP,
    Cast::getCharAddress(quadlet),
    sizeof(quadlet)
  );
  
  assert(quadlet == crc, IEEE1394Exception("Unable to load speed map", this));
  
  static const unsigned int SHIFTS[4] = {24, 16, 8, 0};
  
  // copy to internal representation
  for (unsigned int i = 0; i < numberOfSpeeds; ++i) {
    IEEE1394::Speed speed;
    switch ((speeds[i/4] >> SHIFTS[i % 4]) & 0xff) {
    case 0:
      speed = IEEE1394::S100;
      break;
    case 1:
      speed = IEEE1394::S200;
      break;
    case 2:
      speed = IEEE1394::S400;
      break;
    default:
      speed = IEEE1394::S100; // unknown speed so we use the lowest speed
      break;
    }
    speedMap[i/64][i % 64] = speed;
  }
}

IEEE1394::Speed IEEE1394::getMaximumSpeed(unsigned int physicalId) const throw(OutOfDomain) {
  // TAG: physical speed or link layer speed
  assert(physicalId < numberOfNodes, OutOfDomain(this));
  return nodes[physicalId].speed; // physical speed
// return speedMap[physicalId][physicalId];
}

IEEE1394::Speed IEEE1394::getMaximumSpeedBetweenNodes(unsigned int a, unsigned int b) const throw(OutOfDomain) {
  assert((a < numberOfNodes) && (b < numberOfNodes), OutOfDomain(this));
  return speedMap[a][b]; // or speedMap[b][a]
}

IEEE1394::Speed IEEE1394::getBroadcastSpeed() const throw(OutOfDomain) {
  assert(localId < numberOfNodes, OutOfDomain(this));
  return speedMap[localId][IEEE1394::BROADCAST];
}

uint64 IEEE1394::IsochronousReadChannelImpl::getSubchannels() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::cancel() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadPacketsRequest IEEE1394::IsochronousReadChannelImpl::getReadPacketsRequest() const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadFixedPacketsRequest IEEE1394::IsochronousReadChannelImpl::getReadFixedPacketsRequest() const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadFixedDataRequest IEEE1394::IsochronousReadChannelImpl::getReadFixedDataRequest() const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(IsochronousReadRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(IsochronousReadPacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(IsochronousReadFixedPacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(IsochronousReadFixedDataRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(Allocator<IsochronousReadRequest>& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadRequest IEEE1394::IsochronousReadChannelImpl::dequeue() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

unsigned int IEEE1394::IsochronousReadChannelImpl::dequeue(unsigned int requests, unsigned int microseconds) throw(OutOfDomain, IEEE1394Exception) {
  assert(microseconds <= 999999999, OutOfDomain(this));
  throw IEEE1394Exception("Channel is closed", this);
}

bool IEEE1394::IsochronousReadChannelImpl::wait(unsigned int microseconds) throw(OutOfDomain, IEEE1394Exception) {
  assert(microseconds <= 999999999, OutOfDomain(this));
  throw IEEE1394Exception("Channel is closed", this);
}

uint64 IEEE1394::IsochronousWriteChannelImpl::getSubchannels() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::cancel() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWritePacketsRequest IEEE1394::IsochronousWriteChannelImpl::getWritePacketsRequest() const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWriteFixedPacketsRequest IEEE1394::IsochronousWriteChannelImpl::getWriteFixedPacketsRequest() const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWriteDataRequest IEEE1394::IsochronousWriteChannelImpl::getWriteDataRequest() const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(IsochronousWriteRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(IsochronousWritePacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(IsochronousWriteFixedPacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(IsochronousWriteDataRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(Allocator<IsochronousWriteRequest>& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWriteRequest IEEE1394::IsochronousWriteChannelImpl::dequeue() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}
    
bool IEEE1394::IsochronousWriteChannelImpl::wait(unsigned int microseconds) throw(OutOfDomain, IEEE1394Exception) {
  assert(microseconds <= 999999999, OutOfDomain(this));
  throw IEEE1394Exception("Channel is closed", this);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
