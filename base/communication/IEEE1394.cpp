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
#include <base/ByteOrder.h>
#include <base/concurrency/Thread.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void IEEE1394::IsochronousRequestImpl::setOptions(unsigned int options) throw(IEEE1394Exception) {
  assert(status == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  this->options = options;
}

void IEEE1394::IsochronousRequestImpl::reset() throw(IEEE1394Exception) {
  assert(status != PENDING, bindCause(IEEE1394Exception("Request is pending", this), IEEE1394::REQUEST_NOT_PENDING));
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
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  assert(subchannel <= IEEE1394::BROADCAST, OutOfDomain(this));
  this->subchannel = subchannel;
}

void IEEE1394::IsochronousReadRequestImpl::setBuffer(char* buffer, unsigned int size) throw(IEEE1394Exception) {
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  this->buffer = buffer;
  this->bufferSize = size;
}

void IEEE1394::IsochronousReadFixedPacketsRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  assert(payload % sizeof(IEEE1394::Quadlet) == 0, OutOfDomain(this));
  this->payload = payload;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setNumberOfPackets(unsigned int packets) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  this->numberOfPackets = packets;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setHeaderSize(unsigned int size) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
  assert(payload % sizeof(IEEE1394::Quadlet) == 0, OutOfDomain(this));
  this->headerSize = size;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
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
  assert(getStatus() == READY, bindCause(IEEE1394Exception("Request not ready", this), IEEE1394::REQUEST_NOT_READY));
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

namespace IEEE1394Impl {

  enum NodeCapability {
    BURST_TRANSFER = 1 << 0
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
  
};

bool IEEE1394::hasBeenReset() const throw() { // TAG: not implemented
  return false;
}

void IEEE1394::acknowledgeReset() throw() { // TAG: not implemented
}

Array<EUI64> IEEE1394::getNodes() throw(IEEE1394Exception) {
  uint8 guid[IEEE1394::BROADCAST][8];
  unsigned int numberOfDevices;

  for (unsigned int attempt = 1; attempt <= IEEE1394::MAXIMUM_ATTEMPTS; ++attempt) {
    numberOfDevices = 0;
    uint64 linkOnNodes = getLinkOnNodes();

    try {
      for (unsigned int i = 0; linkOnNodes && (i < IEEE1394::BROADCAST); i++) {
        if (linkOnNodes & (1ULL << i) == 0) {
          continue;
        }
        linkOnNodes &= ~(1ULL << i);
        
        try {
          IEEE1394Impl::ConfigurationIntro config;
          read(i, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM, getCharAddress(config), sizeof(config));
          if (((config.crc >> 24) * sizeof(Quadlet)) < sizeof(IEEE1394Impl::BusInfo)) {
            continue; // general ROM format not present
          }
          if (config.busInfo.name != 0x31333934) {
            continue; // invalid block
          }
          copy<unsigned char>(guid[numberOfDevices++], config.busInfo.guid, sizeof(config.busInfo.guid));
        } catch (IEEE1394Exception& e) {
        }
      }
      break;
    } catch (BusReset& e) {
      // need to acknowledge
    }
  }
  
  Array<EUI64> result;
  for (unsigned int i = 0; i < numberOfDevices; ++i) {
    result.append(EUI64(guid[i]));
  }
  return result;
}

EUI64 IEEE1394::getIdentifier(unsigned int node) throw(OutOfDomain, IEEE1394Exception) {
  assert(node < IEEE1394::BROADCAST, OutOfDomain(this));

  Quadlet quadlet;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM, getCharAddress(quadlet), sizeof(quadlet));
  assert(
    ((quadlet >> 24) * sizeof(Quadlet)) >= sizeof(IEEE1394Impl::BusInfo),
    bindCause(IEEE1394Exception("Device has no configuration ROM", this), IEEE1394::NO_GENERAL_CONFIGURATION_ROM)
  );
  
  IEEE1394Impl::BusInfo busInfo;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK, getCharAddress(busInfo), sizeof(busInfo));
  
  return EUI64(busInfo.guid);
}

unsigned int IEEE1394::getMaximumPayload(unsigned int node) throw(IEEE1394Exception) {
  assert(node < IEEE1394::BROADCAST, OutOfDomain(this));
  
  Quadlet quadlet;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM, getCharAddress(quadlet), sizeof(quadlet));
  assert(
    ((quadlet >> 24) * sizeof(Quadlet)) >= sizeof(IEEE1394Impl::BusInfo),
    bindCause(IEEE1394Exception("Device has no configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );

  IEEE1394Impl::BusInfo busInfo;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK, getCharAddress(busInfo), sizeof(busInfo));
  assert(busInfo.name == 0x31333934, bindCause(IEEE1394Exception(this), INVALID_BUS_INFORMATION_BLOCK));
  return 1 << ((busInfo.flags >> 12) & 0x0f + 1);
}

unsigned int IEEE1394::getCapabilities(unsigned int node) throw(IEEE1394Exception) {
  assert(node < IEEE1394::BROADCAST, OutOfDomain(this));
  
  Quadlet quadlet;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM, getCharAddress(quadlet), sizeof(quadlet));
  assert(
    ((quadlet >> 24) * sizeof(Quadlet)) >= sizeof(IEEE1394Impl::BusInfo),
    bindCause(IEEE1394Exception("Device has no configuration ROM", this), NO_GENERAL_CONFIGURATION_ROM)
  );
  
  IEEE1394Impl::BusInfo busInfo;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK, getCharAddress(busInfo), sizeof(busInfo));
  
//   IEEE1394Impl::RootDirectory rootDirectory;
//   unsigned int rootDirectoryOffset = IEEE1394::BUS_INFO_BLOCK + (quadlet >> 24) * sizeof(Quadlet);
//   read(node, IEEE1394::CSR_BASE_ADDRESS + rootDirectoryOffset, getCharAddress(rootDirectory), sizeof(rootDirectory));
  
  unsigned int capabilities = 0;
  capabilities |= ((busInfo.flags >> 31) & 1) ? ISOCHRONOUS_RESOURCE_MANAGER : 0;
  capabilities |= ((busInfo.flags >> 30) & 1) ? CYCLE_MASTER : 0;
  capabilities |= ((busInfo.flags >> 29) & 1) ? ISOCHRONOUS_TRANSMISSION : 0;
  capabilities |= ((busInfo.flags >> 28) & 1) ? BUS_MASTER : 0;
//   capabilities |= (false) ? POWER_MANAGEMENT : 0; // TAG: fixme
  return capabilities;
}

unsigned int IEEE1394::getVendorId(unsigned int node) throw(IEEE1394Exception) {
  assert(node < IEEE1394::BROADCAST, OutOfDomain(this));
  
  Quadlet quadlet;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM, getCharAddress(quadlet), sizeof(quadlet));

  if ((quadlet >> 24) * sizeof(Quadlet) == sizeof(Quadlet)) { // check for minimal ROM
    return quadlet & 0x00ffffff;
  }
  
  assert(
    ((quadlet >> 24) * sizeof(Quadlet)) >= sizeof(IEEE1394Impl::BusInfo),
    bindCause(IEEE1394Exception("Device has invalid configuration ROM", this), IEEE1394::INVALID_BUS_INFORMATION_BLOCK)
  );
  
  IEEE1394Impl::RootDirectory rootDirectory;
  unsigned int rootDirectoryOffset = IEEE1394::BUS_INFO_BLOCK + (quadlet >> 24) * sizeof(Quadlet);
  read(node, IEEE1394::CSR_BASE_ADDRESS + rootDirectoryOffset, getCharAddress(rootDirectory), sizeof(rootDirectory));
//  assert((rootDirectory.vendorId >> 24) == 0x03, IEEE1394Exception(this)); // TAG: Basler has 0x0c in MSB
  return rootDirectory.vendorId & 0x00ffffff;
}

int IEEE1394::getPhysicalId(const EUI64& guid) throw(IEEE1394Exception) {
  uint64 presentNodes = getPresentNodes();
  
  for (unsigned int node = 0; presentNodes && (node < 64); ++node) {
    if (presentNodes & (1ULL << node) == 0) {
      continue;
    }
    presentNodes &= ~(1ULL << node);

    try {
      Quadlet quadlet;
      read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM, getCharAddress(quadlet), sizeof(quadlet));
      if (((quadlet >> 24) * sizeof(Quadlet)) < sizeof(IEEE1394Impl::BusInfo)) {
        continue;
      }
      
      IEEE1394Impl::BusInfo busInfo;
      read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK, getCharAddress(busInfo), sizeof(busInfo));

      if (compare(guid.getBytes(), busInfo.guid, sizeof(busInfo.guid)) == 0) {
        return node;
      }
    } catch (IOException& e) {
    }
  }
  return -1; // not found
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
