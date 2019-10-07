/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/IEEE1394.h>
#include <base/concurrency/Thread.h>
#include <base/mathematics/Math.h>
#include <base/string/StringOutputStream.h>

// TAG: hops between nodes
//   traverse topology and count hops
// TAG: need CRC class
// TAG: get state of port of node (physical id)
// TAG: get number of ports of node (physical id)
// TAG: max repeated delay: 144+delay*20ns see 6.1 of P1394a
// TAG: worst case repeater delay: 0.144 + 2 * delay/BASE_RATE (us)
// TAG: detect errors: loop in topology
// TAG: cmc bit of bus info block 
// need cached variable for largest block read size to a local node
//   unsigned int maximumPayload?
//   function of maximum speed between nodes
//   function of max_ROM field (assume quadlet by default)

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String IEEE1394::getAsString(unsigned short nodeId) throw() {
  StringOutputStream stream;
  unsigned int busId = getBusId(nodeId);
  if (busId == IEEE1394::LOCAL_BUS) {
    stream << "local";
  } else {
    stream << busId;
  }
  stream << ':';
  unsigned int physicalId = IEEE1394Common::getPhysicalId(nodeId);
  if (physicalId == IEEE1394::BROADCAST) {
    stream << "broadcast";
  } else {
    stream << physicalId;
  }
  stream << FLUSH;
  return stream.getString();
}

IEEE1394::IEEE1394() throw(IEEE1394Exception) {
  ieee1394impl = IEEE1394Impl::getDefault();
}

void IEEE1394::IsochronousRequestImpl::setOptions(unsigned int options) throw(IEEE1394Exception) {
  bassert(
    status == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  this->options = options;
}

void IEEE1394::IsochronousRequestImpl::reset() throw(IEEE1394Exception) {
  bassert(
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

void IEEE1394::IsochronousReadRequestImpl::setSubchannel(
  unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception) {
  bassert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  bassert(subchannel <= IEEE1394::BROADCAST, OutOfDomain(this));
  this->subchannel = subchannel;
}

void IEEE1394::IsochronousReadRequestImpl::setBuffer(
  uint8* buffer, unsigned int size) throw(IEEE1394Exception) {
  bassert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  this->buffer = buffer;
  this->bufferSize = size;
}

void IEEE1394::IsochronousReadFixedPacketsRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  bassert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  bassert(payload % sizeof(Quadlet) == 0, OutOfDomain(this));
  this->payload = payload;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setNumberOfPackets(unsigned int packets) throw(OutOfDomain, IEEE1394Exception) {
  bassert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  this->numberOfPackets = packets;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setHeaderSize(unsigned int size) throw(OutOfDomain, IEEE1394Exception) {
  bassert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  bassert(payload % sizeof(Quadlet) == 0, OutOfDomain(this));
  this->headerSize = size;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  bassert(
    getStatus() == READY,
    bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY)
  );
  bassert(payload % sizeof(Quadlet) == 0, OutOfDomain(this));
  this->payload = payload;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setBuffer(
  uint8* buffer,
  unsigned int size,
  uint8* secondaryBuffer) throw(IEEE1394Exception) {
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

void IEEE1394::IsochronousWriteRequestImpl::setBuffer(
  const uint8* buffer,
  unsigned int size,
  unsigned int numberOfPackets) throw(IEEE1394Exception) {
  bassert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  this->buffer = buffer;
  this->bufferSize = size;
  this->numberOfPackets = numberOfPackets;
}

// void IEEE1394::IsochronousReadChannel::queue(IsochronousReadRequest& request) throw(IEEE1394Exception) {
//   Reference<IsochronousReadRequestImpl> request = request.getContext();
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
  bassert(
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

void IEEE1394::IsochronousWriteDataRequestImpl::setBuffer(
  const uint8* buffer,
  unsigned int size,
  unsigned int numberOfPackets,
  const uint8* secondaryBuffer) throw(IEEE1394Exception) {
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
  bassert(physicalId < numberOfNodes, OutOfDomain(this));
  return nodes[physicalId].guid;
}

IEEE1394::Standard IEEE1394::getCompliance(unsigned short node) throw(IEEE1394Exception) {
  try {
    uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
    if ((crc >> 24) == 1) {
      return IEEE1394::STANDARD_IEEE_1394;
    }
    if ((crc >> 24) * sizeof(Quadlet) < sizeof(BusInfo)) {
      return IEEE1394::STANDARD_IEEE_1394;
    }
    uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
    if (name != 0x31333934) { // "1394"
      return IEEE1394::STANDARD_IEEE_1394;
    }
    uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
    const unsigned int generation = getBits(flags, 4, 4);
    if (generation == 0) {
      return IEEE1394::STANDARD_IEEE_1394;
    }
  } catch (IEEE1394Exception&) {
    return IEEE1394::STANDARD_IEEE_1394;
  }
  return IEEE1394::STANDARD_IEEE_1394A;
}

EUI64 IEEE1394::getIdentifier(unsigned short node) throw(IEEE1394Exception) {
  uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
  bassert(
    (crc >> 24) >= 4,  // minimum size for general format
    bindCause(IEEE1394Exception("No general configuration ROM", this), IEEE1394::NO_GENERAL_CONFIGURATION_ROM)
  );
  uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
  bassert(
    name == 0x31333934, // "1394"
    bindCause(IEEE1394Exception("No general configuration ROM", this), IEEE1394::NO_GENERAL_CONFIGURATION_ROM)    
  );
  return getEUI64(node);
}


unsigned int IEEE1394::getMaximumPayload(unsigned short node) throw(IEEE1394Exception) {
  uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
  // minimum size for general format
  bassert(
    ((crc >> 24) * sizeof(Quadlet)) >= sizeof(BusInfo),
    bindCause(IEEE1394Exception("No general configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );
  uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
  bassert(
    name == 0x31333934, // "1394"
    bindCause(IEEE1394Exception("No general configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );
  uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
  unsigned int max = getBits(flags, 12, 4); // max_rec field
  return (max > 0) ? (1 << (max + 1)) : (1 << (1 + 1));
}

unsigned int IEEE1394::getCapabilities(unsigned short node) throw(IEEE1394Exception) {
  unsigned int capabilities = 0;
  try {
    uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
    // minimum size for general format
    if (((crc >> 24) * sizeof(Quadlet)) >= sizeof(BusInfo)) {
      uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
      if (name == 0x31333934) { // "1394"
        uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
        capabilities |= ((flags >> 31) & 1) ? ISOCHRONOUS_RESOURCE_MANAGER_CAPABLE : 0;
        capabilities |= ((flags >> 30) & 1) ? CYCLE_MASTER_CAPABLE : 0;
        capabilities |= ((flags >> 29) & 1) ? ISOCHRONOUS_TRANSACTION_CAPABLE : 0;
        capabilities |= ((flags >> 28) & 1) ? BUS_MASTER_CAPABLE : 0; // must be set if power manager capable
        capabilities |= ((flags >> 27) & 1) ? POWER_MANAGER_CAPABLE : 0;
      }
    }
  } catch (IEEE1394Exception&) {
  }
  return capabilities;
}

unsigned int IEEE1394::getVendorId(unsigned short node) throw(IEEE1394Exception) {
  uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
  if ((crc >> 24) == 1) { // check for minimal ROM
    return crc & 0x00ffffff;
  }

  uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
  
  bassert(
    ((crc >> 24) * sizeof(Quadlet) >= sizeof(BusInfo)) && // check for general ROM format
    (name == 0x31333934), // "1394"
    bindCause(IEEE1394Exception("Invalid configuration ROM", this), IEEE1394::INVALID_BUS_INFORMATION_BLOCK)
  );
  
  const uint32 rootDirectoryOffset =
    IEEE1394::CONFIGURATION_ROM + ((crc >> 24) + 1) * sizeof(Quadlet);
  uint32 quadlet = getQuadlet(node, rootDirectoryOffset);
  unsigned int numberOfEntries = quadlet >> 16;
  for (unsigned int entry = 1; entry <= numberOfEntries; ++entry) { // find mandatory Vendor_ID entry
    const uint32 entryOffset = rootDirectoryOffset + entry * sizeof(Quadlet);
    uint32 quadlet = getQuadlet(node, entryOffset);
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
  uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
  // minimum size for general format
  bassert(
    ((crc >> 24) * sizeof(Quadlet)) >= sizeof(BusInfo),
    bindCause(IEEE1394Exception("No general configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );
  uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
  bassert(
    name == 0x31333934, // "1394"
    bindCause(IEEE1394Exception("No general configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );
  const uint32 rootDirectoryOffset =
    IEEE1394::CONFIGURATION_ROM + ((crc >> 24) + 1) * sizeof(Quadlet);
  
  // read root directory entries
  uint32 quadlet;
  unsigned int numberOfEntries = getQuadlet(node, rootDirectoryOffset) >> 16;
  for (unsigned int entry = 1; entry <= numberOfEntries; ++entry) {
    const uint32 entryOffset = rootDirectoryOffset + entry * sizeof(Quadlet);
    quadlet = getQuadlet(node, entryOffset);
    
    IEEE1394::CSRKeyValue keyValue = getCSRKeyValue(quadlet);
    if (keyValue == IEEE1394::KEY_TEXTUAL) {
      IEEE1394::CSRKeyType keyType = getCSRKeyType(quadlet);
      if (keyType == IEEE1394::KEY_TYPE_LEAF) {
        uint32 leafOffset = entryOffset + (quadlet & 0x00ffffff) * sizeof(Quadlet);
        const unsigned int leafSize = getQuadlet(node, leafOffset) >> 16;
        if (leafSize > 2) {
          SimpleBuffer<Quadlet> leaf(leafSize); // must be big endian
          for (unsigned int i = 0; i < leafSize; ++i) {
            leafOffset += sizeof(Quadlet);
            leaf[i] = getQuadlet(node, leafOffset);
          }
          if (!((leaf[0] == 0) && (leaf[1] == 0))) {
            break; // unsupported format
          }
          return String(Cast::getCharAddress(leaf[2]), (leafSize - 2) * sizeof(Quadlet));
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
  uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
  // minimum size for general format
  bassert(
    ((crc >> 24) * sizeof(Quadlet)) >= sizeof(BusInfo),
    bindCause(IEEE1394Exception("No general configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );
  uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
  bassert(
    name == 0x31333934, // "1394"
    bindCause(IEEE1394Exception("No general configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );
  const uint32 rootDirectoryOffset =
    IEEE1394::CONFIGURATION_ROM + ((crc >> 24) + 1) * sizeof(Quadlet);
  
  // read root directory entries
  uint32 quadlet;
  unsigned int numberOfEntries = getQuadlet(node, rootDirectoryOffset) >> 16;
  for (unsigned int entry = 1; entry <= numberOfEntries; ++entry) {
    const uint32 entryOffset = rootDirectoryOffset + entry * sizeof(Quadlet);
    quadlet = getQuadlet(node, entryOffset);

    IEEE1394::CSRKeyValue keyValue = getCSRKeyValue(quadlet);
    IEEE1394::CSRKeyType keyType = getCSRKeyType(quadlet);
    if ((keyType == IEEE1394::KEY_TYPE_LEAF) &&
        (keyValue == IEEE1394::KEY_KEYWORD)) { // Keyword_Leaf
      uint32 leafOffset = entryOffset + (quadlet & 0x00ffffff) * sizeof(Quadlet);
      const unsigned int leafSize = getQuadlet(node, leafOffset) >> 16;
      SimpleBuffer<Quadlet> leafWords(leafSize); // must be big endian
      for (unsigned int i = 0; i < leafSize; ++i) {
        leafOffset += sizeof(Quadlet);
        leafWords[i] = getQuadlet(node, leafOffset);
      }
      const char* leaf = Cast::getCharAddress(leafWords);
      String result;
      for (unsigned int i = 0; i < leafSize * sizeof(Quadlet);) {
        unsigned int j = i;
        while ((j < leafSize * sizeof(Quadlet)) && leaf[j]) {
          ++j;
        }
        if (j == i) {
          break; // all words have been read
        }
        if (i > 0) {
          result += " ";
        }
        result += String(leaf[i]);
      }
      return result;
    } else if (keyType == IEEE1394::KEY_TYPE_DIRECTORY) {
      // TAG: search for a nice leaf
    }
  }
  return String();
}

void IEEE1394::checkResetGeneration() throw(IEEE1394Exception) {
  // TAG: IEEE 1394 implementation should provide reset detection support
  try {
    if (!nodes[busManagerId].guid.isInvalid()) {
      EUI64 guid = getIdentifier(makeNodeId(busManagerId));
      
      if (guid == nodes[busManagerId].guid) {
        // get generation from topology map
        uint32 generation =
          getQuadlet(makeNodeId(busManagerId), IEEE1394::TOPOLOGY_MAP + sizeof(Quadlet));
        if (generation == resetGeneration) {
          return;
        }
      }
    }
  } catch (IEEE1394Exception&) {
  }
  reload(); // responsible for setting the new generation number
}

unsigned short IEEE1394::findRole(Role role, unsigned int busId) throw(OutOfDomain, IEEE1394Exception) {
  bassert(busId <= IEEE1394::LOCAL_BUS, OutOfDomain(this));
  switch (role) {
  case IEEE1394::CYCLE_MASTER: // must/should be root eventually
    for (int id = IEEE1394::BROADCAST; id > 0;) {
      --id;
      const unsigned short node = makeNodeId(id, busId);
      try {
        uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
        if ((crc >> 24) * sizeof(Quadlet) >= sizeof(BusInfo)) { // minimum size for general format
          uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
          if (name == 0x31333934) { // "1394"
            uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
            if (((flags >> 28) & (4|2)) == (4|2)) { // cmc and isc flags
              if (getBits(getQuadlet(node, IEEE1394::STATE_CLEAR), 8, 1)) { // cmstr
                return node;
              }
            }
          }
        }
      } catch (IEEE1394Exception&) {
      }
    }
    break;
  case IEEE1394::ISOCHRONOUS_RESOURCE_MANAGER:
    for (int id = IEEE1394::BROADCAST; id > 0;) {
      --id;
      const unsigned short node = makeNodeId(id, busId);
      try {
        /*uint32 cycleTime =*/ getQuadlet(node, IEEE1394::CYCLE_TIME); // isochronous capable
        uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
        if ((crc >> 24) < 4) { // minimum size for general format
          continue;
        }
        uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
        if (name != 0x31333934) { // "1394"
          continue;
        }
        uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
        if (((flags >> 28) & 8) == 8) { // irmc
          return node;
        }
//          uint32 temp = getQuadlet(node, IEEE1394::BUS_MANAGER_ID);
//          if (IEEE1394Common::getPhysicalId(temp) == id) {
//            return node;
//          }
      } catch (IEEE1394Exception&) {
      }
    }
    break;
  case IEEE1394::BUS_MANAGER:
    for (unsigned int id = IEEE1394::BROADCAST; id > 0;) {
      --id;
      const unsigned short node = makeNodeId(id, busId);
      try {
        /*uint32 cycleTime =*/ getQuadlet(node, IEEE1394::CYCLE_TIME); // isochronous capable
        uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
        if ((crc >> 24) < 4) { // minimum size for general format
          continue;
        }
        uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
        if (name != 0x31333934) { // "1394"
          continue;
        }
        uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
        // bus manager, irm, cycle master, and isochronous capable
        if (((flags >> 28) & (8|4|2|1)) == (8|4|2|1)) {
          continue;
        }
        uint32 temp = getQuadlet(node, IEEE1394::BUS_MANAGER_ID);
        if (IEEE1394Common::getPhysicalId(temp) == id) {
          return node;
        }
      } catch (IEEE1394Exception&) {
      }
    }
    break;
  case IEEE1394::ROOT:
    for (unsigned int id = IEEE1394::BROADCAST; id > 0;) {
      --id;
      try {
        uint32 temp = getQuadlet(makeNodeId(id, busId), IEEE1394::NODE_IDS);
        if (temp == static_cast<uint32>(makeNodeId(id, IEEE1394::LOCAL_BUS) << 16)) {
          return makeNodeId(id, busId);
        }
      } catch (IEEE1394Exception&) {
      }
    }
    // all nodes could be transaction incapable or non present
    break;
  }
  return makeNodeId(IEEE1394::BROADCAST, busId); // not found/present
}

void IEEE1394::reload() throw(IEEE1394Exception) {
  for (unsigned int attempts = 10; ; --attempts) { // max wait is 10*100ms
    // invalidate current attributes
    numberOfNodes = 0;
    localId = IEEE1394::BROADCAST;
    busManagerId = IEEE1394::BROADCAST;
    isochronousResourceManagerId = IEEE1394::BROADCAST;
    cycleMasterId = IEEE1394::BROADCAST;
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

    // TAG: wait for no reset (time out)
    
    numberOfNodes = ieee1394impl->getNumberOfNodes();
    localId = ieee1394impl->getLocalId();
    bassert((numberOfNodes > 0) && (attempts > 0), IEEE1394Exception("Unable to query local bus", this));
    
    // TAG: could check topology map of local node if present
    
    // the irm is the candiate with the largest phy id (likely to be root)
    for (int id = numberOfNodes - 1; id >= 0; --id) {
      try {
        const unsigned short node = makeNodeId(id);
        uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
        if ((crc >> 24) * sizeof(Quadlet) >= sizeof(BusInfo)) { // minimum size for general format
          uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
          if (name == 0x31333934) { // "1394"
            uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
            if (((flags >> 28) & 8) == 8) { // irmc flag
              isochronousResourceManagerId = id;
              break;
            }
          }
        }
      } catch (IEEE1394Exception&) {
        break; // not an irm candidate
      }
    }

    // find cycle master (very likely to be root)
    for (int id = numberOfNodes - 1; id >= 0; --id) {
      try {
        const unsigned short node = makeNodeId(id);
        uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
        if ((crc >> 24) * sizeof(Quadlet) >= sizeof(BusInfo)) { // minimum size for general format
          uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
          if (name == 0x31333934) { // "1394"
            uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
            if (((flags >> 28) & (4|2)) == (4|2)) { // cmc and isc flags
              if (getBits(getQuadlet(node, IEEE1394::STATE_CLEAR), 8, 1)) { // cmstr
                cycleMasterId = id;
                break;
              }
            }
          }
        }
      } catch (IEEE1394Exception&) {
        break; // not an cycle master candidate
      }
    }
    
    // only look for bus manager if irm is present
    if (isochronousResourceManagerId == IEEE1394::BROADCAST) {
      break; // unmanaged bus
    }
    
    unsigned int id = IEEE1394Common::getPhysicalId(
      getQuadlet(makeNodeId(isochronousResourceManagerId), IEEE1394::BUS_MANAGER_ID)
    );
    
    ASSERT(id < IEEE1394::BROADCAST); // IRM present
    
    try {
      const unsigned short node = makeNodeId(id);
      uint32 crc = getQuadlet(node, IEEE1394::CONFIGURATION_ROM);
      if ((crc >> 24) * sizeof(Quadlet) >= sizeof(BusInfo)) { // minimum size for general format
        uint32 name = getQuadlet(node, IEEE1394::BUS_INFO_NAME);
        if (name == 0x31333934) { // "1394"
          uint32 flags = getQuadlet(node, IEEE1394::BUS_INFO_FLAGS);
          if (((flags >> 28) & 9) == 9) { // bmc flag
            busManagerId = id;
          }
        }
      }
    } catch (IEEE1394Exception&) {
      break; // not a bus manager candidate
    }
    
    try {
      uint32 savedGeneration = getQuadlet(makeNodeId(busManagerId), IEEE1394::TOPOLOGY_MAP + sizeof(Quadlet));
      loadTopologyMap();
      loadSpeedMap(); // TAG: deprecated in later specification
      
      // get generation from topology map
      uint32 generation = getQuadlet(makeNodeId(busManagerId), IEEE1394::TOPOLOGY_MAP + sizeof(Quadlet));
      if (generation != savedGeneration) {
        Thread::millisleep(100);
      }
      resetGeneration = generation;
      break; // success
    } catch (IEEE1394Exception&) {
      break;
    }
  }
}

void IEEE1394::loadTopologyMap() throw(IEEE1394Exception) {
  const unsigned short node = makeNodeId(busManagerId);
  
  uint32 crc = getQuadlet(node, IEEE1394::TOPOLOGY_MAP);
  
  const unsigned int length = crc >> 16;
  bassert(length >= 3, IEEE1394Exception(this));
  
  uint32 count = getQuadlet(node, IEEE1394::TOPOLOGY_MAP + 2 * sizeof(Quadlet));
  // numberOfNodes = count >> 16;
  const unsigned int selfIds = count & 0xffff;
  
  bassert(numberOfNodes < IEEE1394::BROADCAST, IEEE1394Exception(this));
  bassert(
    (selfIds >= 1) && (selfIds <= (63 * 9) && (selfIds <= length - 2)),
    IEEE1394Exception(this)
  );
  
  // get topology map
  SimpleBuffer<uint32> ids(selfIds);
  uint32 offset = IEEE1394::TOPOLOGY_MAP + 3 * sizeof(Quadlet);
  for (unsigned int i = 0; i < selfIds; ++i) {
    ids[i] = getQuadlet(node, offset);
    offset += sizeof(Quadlet);
  }
  
  // get crc
  uint32 quadlet = getQuadlet(node, IEEE1394::TOPOLOGY_MAP); 
  bassert(quadlet == crc, IEEE1394Exception("Unable to load topology map", this));
  
  // copy to internal representation
  unsigned int physicalId = 0; // the current physical id
  for (unsigned int i = 0; i < selfIds;) {
    uint32 current = ids[i++];
    
    bassert(
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
    nodes[physicalId].gapCount = getBits(current, 16, 6);
    nodes[physicalId].speed = static_cast<Speed>(getBits(current, 14, 2));
//     selfId.delay = getBits(current, 12, 2);
    nodes[physicalId].powerClass = static_cast<PowerClass>(getBits(current, 8, 3));
    nodes[physicalId].initiatedReset = getBits(current, 1, 1);

    try {
      nodes[physicalId].standard = IEEE1394::STANDARD_IEEE_1394;
      uint32 crc = getQuadlet(makeNodeId(physicalId), IEEE1394::CONFIGURATION_ROM);
      if ((crc >> 24) * sizeof(Quadlet) >= sizeof(BusInfo)) {
        uint32 name = getQuadlet(makeNodeId(physicalId), IEEE1394::BUS_INFO_NAME);
        if (name == 0x31333934) { // "1394"
          uint32 flags = getQuadlet(makeNodeId(physicalId), IEEE1394::BUS_INFO_FLAGS);
          nodes[physicalId].capabilities =
            (((flags >> 31) & 1) ? ISOCHRONOUS_RESOURCE_MANAGER_CAPABLE : 0) |
            (((flags >> 30) & 1) ? CYCLE_MASTER_CAPABLE : 0) |
            (((flags >> 29) & 1) ? ISOCHRONOUS_TRANSACTION_CAPABLE : 0) |
            (((flags >> 28) & 1) ? BUS_MASTER_CAPABLE : 0) |
            (((flags >> 27) & 1) ? POWER_MANAGER_CAPABLE : 0);
          const unsigned int generation = getBits(flags, 4, 4);
          if (generation != 0) {
            nodes[physicalId].standard = IEEE1394::STANDARD_IEEE_1394A;
          }
          nodes[physicalId].linkSpeed = static_cast<Speed>(getBits(flags, 0, 3));
        }
      }
      nodes[physicalId].guid = getEUI64(makeNodeId(physicalId));
    } catch (IEEE1394Exception&) {
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

unsigned int IEEE1394::getCycleTime(
  unsigned short node) throw(IEEE1394Exception) {
  Quadlet quadlet;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CYCLE_TIME,
    Cast::getAddress(quadlet),
    sizeof(quadlet)
  );
  unsigned int units = (quadlet >> 12) & ((1 << 13) - 1); // 125 micro seconds per unit
  unsigned int seconds = (quadlet >> (12 + 13));
  return seconds * 8000 + units;
}

unsigned int IEEE1394::getBusTime(
  unsigned short node) throw(IEEE1394Exception) {
  Quadlet quadlet;
  read(
    node,
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_TIME,
    Cast::getAddress(quadlet),
    sizeof(quadlet)
  );
  return quadlet;
}

unsigned int IEEE1394::getAvailableBandwidth() throw(IEEE1394Exception) {
  Quadlet quadlet;
  read(
    makeNodeId(isochronousResourceManagerId),
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BANDWIDTH_AVAILABLE,
    Cast::getAddress(quadlet),
    sizeof(quadlet)
  );
  return quadlet & ((1 << 13) - 1);
}

uint64 IEEE1394::getAvailableIsochronousChannels() throw(IEEE1394Exception) {
  uint32 high = getQuadlet(
    makeNodeId(isochronousResourceManagerId),
    IEEE1394::CHANNELS_AVAILABLE_HI
  );
  uint32 low = getQuadlet(
    makeNodeId(isochronousResourceManagerId),
    IEEE1394::CHANNELS_AVAILABLE_LO
  );
  return Math::getBitReversal((static_cast<uint64>(high) << 32) | low);
}

void IEEE1394::loadSpeedMap() throw(IEEE1394Exception) {
  const unsigned short node = makeNodeId(busManagerId);
  uint32 crc = getQuadlet(node, IEEE1394::SPEED_MAP);
  if ((crc >> 16) != ((64 * 62 + 62 + 3)/4 + 1)) {
    throw IEEE1394Exception("Invalid speed map", this);
  }
  
  // get speeds
  Quadlet speeds[(64 * 62 + 62 + 3)/4];
  unsigned int numberOfSpeeds = 64 * numberOfNodes + numberOfNodes;
  uint64 address =
    IEEE1394::CSR_BASE_ADDRESS + IEEE1394::SPEED_MAP + 2 * sizeof(Quadlet);
  try {
    read(
      node,
      address,
      Cast::getAddress(speeds),
      (numberOfSpeeds + sizeof(Quadlet) - 1)/sizeof(Quadlet)*sizeof(Quadlet)
    );
  } catch (IEEE1394Exception&) {
    for (unsigned int i = 0; i < (numberOfSpeeds + sizeof(Quadlet) - 1)/sizeof(Quadlet); ++i) {
      read(node, address, Cast::getAddress(speeds[i]), sizeof(speeds[i]));
      address += sizeof(speeds[i]);
    }
  }
  
  // get crc
  uint32 quadlet = getQuadlet(node, IEEE1394::SPEED_MAP);
  bassert(quadlet == crc, IEEE1394Exception("Unable to load speed map", this));
  
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

IEEE1394::Speed IEEE1394::getMaximumSpeed(
  unsigned int physicalId) const throw(OutOfDomain) {
  bassert(physicalId < numberOfNodes, OutOfDomain(this));
  return nodes[physicalId].speed; // physical speed
}

IEEE1394::Speed IEEE1394::getMaximumLinkSpeed(
  unsigned int physicalId) const throw(OutOfDomain) {
  bassert(physicalId < numberOfNodes, OutOfDomain(this));  
  return nodes[physicalId].linkSpeed;
}

IEEE1394::Speed IEEE1394::getMaximumSpeed(uint64 nodes) const throw() {
  Speed result = IEEE1394::S3200;
  for (unsigned int id = 0; id < numberOfNodes; ++id) { // ignore non-present nodes
    if ((nodes >> id) & 1) {
      result = minimum(result, speedMap[id][id]);
    }
  }
  return result;
}

IEEE1394::Speed IEEE1394::getMaximumSpeedBetweenNodes(
  unsigned int a, unsigned int b) const throw(OutOfDomain) {
  bassert((a < numberOfNodes) && (b < numberOfNodes), OutOfDomain(this));
  return speedMap[a][b]; // or speedMap[b][a]
}

IEEE1394::Speed IEEE1394::getBroadcastSpeed() const throw(OutOfDomain) {
  bassert(localId < numberOfNodes, OutOfDomain(this));
  return speedMap[localId][IEEE1394::BROADCAST];
}

uint64 IEEE1394::IsochronousReadChannelImpl::getSubchannels(
) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::cancel() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadPacketsRequest IEEE1394::IsochronousReadChannelImpl::getReadPacketsRequest() const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadFixedPacketsRequest
  IEEE1394::IsochronousReadChannelImpl::getReadFixedPacketsRequest(
  ) const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadFixedDataRequest
  IEEE1394::IsochronousReadChannelImpl::getReadFixedDataRequest(
  ) const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(
  IsochronousReadRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(
  IsochronousReadPacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(
  IsochronousReadFixedPacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(
  IsochronousReadFixedDataRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousReadChannelImpl::queue(
  Allocator<IsochronousReadRequest>& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousReadRequest
  IEEE1394::IsochronousReadChannelImpl::dequeue() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

unsigned int IEEE1394::IsochronousReadChannelImpl::dequeue(
  unsigned int requests,
  unsigned int microseconds) throw(OutOfDomain, IEEE1394Exception) {
  bassert(microseconds <= 999999999, OutOfDomain(this));
  throw IEEE1394Exception("Channel is closed", this);
}

bool IEEE1394::IsochronousReadChannelImpl::wait(
  unsigned int microseconds) throw(OutOfDomain, IEEE1394Exception) {
  bassert(microseconds <= 999999999, OutOfDomain(this));
  throw IEEE1394Exception("Channel is closed", this);
}

uint64 IEEE1394::IsochronousWriteChannelImpl::getSubchannels(
) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::cancel() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWritePacketsRequest
  IEEE1394::IsochronousWriteChannelImpl::getWritePacketsRequest(
  ) const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWriteFixedPacketsRequest
  IEEE1394::IsochronousWriteChannelImpl::getWriteFixedPacketsRequest(
  ) const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWriteDataRequest
  IEEE1394::IsochronousWriteChannelImpl::getWriteDataRequest(
  ) const throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(
  IsochronousWriteRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(
  IsochronousWritePacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(
  IsochronousWriteFixedPacketsRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(
  IsochronousWriteDataRequest& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

void IEEE1394::IsochronousWriteChannelImpl::queue(
  Allocator<IsochronousWriteRequest>& request) throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}

IEEE1394::IsochronousWriteRequest
  IEEE1394::IsochronousWriteChannelImpl::dequeue() throw(IEEE1394Exception) {
  throw IEEE1394Exception("Channel is closed", this);
}
    
bool IEEE1394::IsochronousWriteChannelImpl::wait(
  unsigned int microseconds) throw(OutOfDomain, IEEE1394Exception) {
  bassert(microseconds <= 999999999, OutOfDomain(this));
  throw IEEE1394Exception("Channel is closed", this);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
