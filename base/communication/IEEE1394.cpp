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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void IEEE1394::IsochronousRequestImpl::setOptions(unsigned int options) throw(IEEE1394Exception) {
  assert(status == READY, IEEE1394Exception("Request not ready", this));
  this->options = options;
}

void IEEE1394::IsochronousRequestImpl::reset() throw(IEEE1394Exception) {
  assert(status != PENDING, IEEE1394Exception("Request is pending", this));
  resetContext();
  status = READY;
}

IEEE1394::IsochronousRequestImpl::~IsochronousRequestImpl() throw() {
  Trace::message("IEEE1394::IsochronousRequestImpl::~IsochronousRequestImp");
}

IEEE1394::IsochronousReadRequestImpl::IsochronousReadRequestImpl() throw()
  : subchannel(0),
    buffer(0),
    bufferSize(0),
    receivedPackets(0) {
  // TAG: subchannel must be reserved - at least one reserved channel
}

void IEEE1394::IsochronousReadRequestImpl::setSubchannel(unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  assert(subchannel <= 63, OutOfDomain(this));
  this->subchannel = subchannel;
}

void IEEE1394::IsochronousReadRequestImpl::setBuffer(char* buffer, unsigned int size) throw(IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  this->buffer = buffer;
  this->bufferSize = size;
}

void IEEE1394::IsochronousReadFixedPacketsRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  assert(payload % 4 == 0, OutOfDomain(this));
  this->payload = payload;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setNumberOfPackets(unsigned int packets) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  this->numberOfPackets = packets;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setHeaderSize(unsigned int size) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  assert(payload % 4 == 0, OutOfDomain(this));
  this->headerSize = size;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  assert(payload % 4 == 0, OutOfDomain(this));
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
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
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
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
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
  
  struct BusInfo {
    BigEndian<uint32> id31333934;
    BigEndian<uint32> flags;
    uint8 guid[8];
  } _DK_SDU_MIP__BASE__PACKED;

  struct RootDirectory {
    BigEndian<uint32> crc;
    BigEndian<uint32> vendorId;
    BigEndian<uint32> flags;
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
  uint8 guid[63][8];
  unsigned int numberOfDevices;

  for (unsigned int attempt = 1; attempt <= IEEE1394::MAXIMUM_ATTEMPTS; ++attempt) {
    numberOfDevices = 0;
    uint64 linkOnNodes = getLinkOnNodes();

    try {
      for (unsigned int i = 0; linkOnNodes && (i < 63); i++) { // 63 is broadcast id
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
          if (config.busInfo.id31333934 != 0x31333934) {
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
    IEEE1394Exception("Device has no guid", this)
  );
  
  IEEE1394Impl::BusInfo busInfo;
  read(node, IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_INFO_BLOCK, getCharAddress(busInfo), sizeof(busInfo));
  
  return EUI64(busInfo.guid);
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
