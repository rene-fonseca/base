/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/communication/LinuxRawIEEE1394.h>
#include <base/Cast.h>
#include <base/UnexpectedFailure.h>

#if (_DK_SDU_MIP__BASE__FLAVOR != _DK_SDU_MIP__BASE__UNIX)
#  error LinuxRawIEEE1394Impl not supported by flavor
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace LinuxRawIEEE1394Impl {
  
  const char DEVICE_PATH[] = "/dev/raw1394"; // TAG: need something like FileSystem::getFolder(FOLDER_DEVICES)?

  struct Request {
    uint32 type;
    int32 error;
    uint32 misc;
    uint32 generation;
    uint32 size;
    uint64 address;
    uint64 tag;
    uint64 sendBuffer;
    uint64 receiveBuffer;
  } _DK_SDU_MIP__BASE__PACKED;

  enum Error {
    ERROR_NONE = 0,
    ERROR_INCOMPATIBLE = -1001,
    ERROR_STATE_ORDER = -1002,
    ERROR_GENERATION = -1003,
    ERROR_INVALID_ARGUMENT = -1004,
    ERROR_MEMORY_FAULT = -1005,
    ERROR_ALREADY = -1006,
    ERROR_EXCESSIVE = -1020,
    ERROR_UNTIDY_SIZE = -1021,
    ERROR_SEND_ERROR = -1100,
    ERROR_ABORTED = -1101,
    ERROR_TIMEOUT = -1102
  };
  
  enum Acknowledge {
    ACK_COMPLETED = 0x1,
    ACK_PENDING = 0x2,
    ACK_BUSY_X = 0x3,
    ACK_BUSY_A = 0x4,
    ACK_BUSY_B = 0x5,
    ACK_DATA_ERROR = 0xd,
    ACK_TYPE_ERROR = 0xe
  };

  enum RCode {
    RCODE_COMPLETE = 0x0,
    RCODE_CONFLICT_ERROR = 0x4,
    RCODE_DATA_ERROR = 0x5,
    RCODE_TYPE_ERROR = 0x6,
    RCODE_ADDRESS_ERROR = 0x7
  };

  inline unsigned int mapErrorToStatus(int error) throw() {
    unsigned int status = IEEE1394Common::STATUS_ERROR;

    if (error <= 0) { // TAG: is this ok
      switch (error) {
      case ERROR_NONE:
        status = IEEE1394Common::STATUS_OK;
        break;
      case ERROR_INCOMPATIBLE:
        status = IEEE1394Common::STATUS_INCOMPATIBLE;
        break;
      case ERROR_GENERATION:
      case ERROR_INVALID_ARGUMENT:
      case ERROR_MEMORY_FAULT:
      case ERROR_UNTIDY_SIZE:
        status = IEEE1394Common::STATUS_INTERNAL;
        break;
      case ERROR_ALREADY:
        status = IEEE1394Common::STATUS_CONFLICT;
        break;      
      case ERROR_ABORTED:
        status = IEEE1394Common::STATUS_ABORTED;
        break;
      case ERROR_TIMEOUT:
        status = IEEE1394Common::STATUS_TIMEOUT;
        break;
      }
    } else {
      switch (error >> 16) {
      case LinuxRawIEEE1394Impl::ACK_COMPLETED:
        status = IEEE1394Impl::STATUS_OK;
        break;
      case LinuxRawIEEE1394Impl::ACK_PENDING:
        switch (error & 0xffff) {
        case LinuxRawIEEE1394Impl::RCODE_COMPLETE:
          status = IEEE1394Impl::STATUS_OK;
          break;
        case LinuxRawIEEE1394Impl::RCODE_CONFLICT_ERROR:
          status = IEEE1394Impl::STATUS_CONFLICT;
          break;
        case LinuxRawIEEE1394Impl::RCODE_DATA_ERROR: // TAG: check spec
        case LinuxRawIEEE1394Impl::RCODE_TYPE_ERROR: // TAG: check spec
        case LinuxRawIEEE1394Impl::RCODE_ADDRESS_ERROR:
        default:
          status = IEEE1394Impl::STATUS_INTERNAL;
        }
        break;
      case LinuxRawIEEE1394Impl::ACK_BUSY_X:
      case LinuxRawIEEE1394Impl::ACK_BUSY_A:
      case LinuxRawIEEE1394Impl::ACK_BUSY_B:
        status = IEEE1394Impl::STATUS_BUSY;
        break;
      case LinuxRawIEEE1394Impl::ACK_DATA_ERROR:
      case LinuxRawIEEE1394Impl::ACK_TYPE_ERROR:
      default:
        status = IEEE1394Impl::STATUS_INTERNAL;
      }
    }
    return status;
  }
  
  struct Card {
    uint32 nodes;
    uint8 name[32];
  };
  
  struct RequestContext {
    unsigned int type;
    bool dequeued;
    int status;
  };

  void dumpRequest(const Request& request) throw() {
    fout << "request: "<< EOL
         << "  type: " << request.type << EOL
         << "  error: " << request.error << EOL
         << "  misc: " << HEX << setWidth(10) << ZEROPAD << request.misc << EOL
         << "  generation: " << request.generation << EOL
         << "  size: " << request.size << EOL
         << "  address: " << HEX << setWidth(18) << ZEROPAD << request.address << EOL
         << "  tag: " << HEX << setWidth(18) << ZEROPAD << request.tag << EOL
         << "  sendBuffer: " << HEX << setWidth(18) << ZEROPAD << request.sendBuffer << EOL
         << "  receiveBuffer: " << HEX << setWidth(18) << ZEROPAD << request.receiveBuffer << EOL
         << ENDL;
  }
  
}; // end of LinuxRawIEEE1394Impl namespace

LinuxRawIEEE1394::LinuxRawIEEE1394() throw(IEEE1394Exception) {
  handle = ::open(LinuxRawIEEE1394Impl::DEVICE_PATH, O_RDWR);
  assert(handle >= 0, IEEE1394Exception("Unable to open device", this));
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_INITIALIZE;
  request.misc = LinuxRawIEEE1394::PROTOCOL_DEFAULT;

  bool error = false;
  
  protocol = LinuxRawIEEE1394::PROTOCOL_DEFAULT;
  if (!error && (::write(handle, &request, sizeof(request)) != sizeof(request))) {
    error = true;
  }
  if (!error && (::read(handle, &request, sizeof(request)) != sizeof(request))) {
    error = true;
  }

  if (!error && (request.error == LinuxRawIEEE1394Impl::ERROR_INCOMPATIBLE) && (request.misc == LinuxRawIEEE1394::PROTOCOL_3)) {
    protocol = LinuxRawIEEE1394::PROTOCOL_3;
    if (!error && (::write(handle, &request, sizeof(request)) != sizeof(request))) {
      error = true;
    }
    if (!error && (::read(handle, &request, sizeof(request)) != sizeof(request))) {
      error = true;
    }
  }
  
  status = LinuxRawIEEE1394Impl::mapErrorToStatus(request.error);
  generation = request.generation;
  if (error || (status != IEEE1394Impl::STATUS_OK)) {
    ::close(handle);
    throw IEEE1394Exception("Incompatibility", this);
  }
}

bool LinuxRawIEEE1394::hasBeenReset() const throw() {
  return false;
}

void LinuxRawIEEE1394::acknowledgeReset() throw() {
}

void LinuxRawIEEE1394::resetBus() throw(IEEE1394Exception) {
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_RESET_BUS;
  request.generation = generation;
  assert(::write(handle, &request, sizeof(request)) >= 0, IEEE1394Exception("Unable to reset bus", this));
  // TAG: read new node ids
}

Array<EUI64> LinuxRawIEEE1394::getAdapters() throw(IEEE1394Exception) {
  LinuxRawIEEE1394Impl::Card cards[MAXIMUM_NUMBER_OF_ADAPTERS]; // TAG: reimpl if can detect # of adapters
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_GET_ADAPTERS;
  request.generation = generation;
  request.receiveBuffer = Cast::getOffset(&cards);
  request.size = sizeof(cards);

  bool error = false;
  
  // TAG: should be dequeued like any other response
  
  while (true) {
    if ((::write(handle, &request, sizeof(request)) != sizeof(request)) ||
        (::read(handle, &request, sizeof(request)) != sizeof(request))) {
      throw IEEE1394Exception("Unable to get adapters", this);
    }
    switch (request.error) {
    case LinuxRawIEEE1394Impl::ERROR_NONE:
      {
        Array<EUI64> result;
        unsigned char artificialGuid[8];
        artificialGuid[0] = 0xff;
        artificialGuid[1] = 0xff;
        artificialGuid[2] = 0xff;
        artificialGuid[3] = 0x00;
        artificialGuid[4] = 0x00;
        artificialGuid[5] = 0x00;
        artificialGuid[6] = 0x00;
        artificialGuid[7] = 0x01; // 0x00 is invalid
        for (unsigned int i = 0; i < request.misc; ++i) {
          result.append(EUI64(artificialGuid));
          for (unsigned int i = 7; (i > 2) && (++artificialGuid[i] == 0x00); --i) { // next artificial guid
            artificialGuid[i]++; // maximum number of adapters is small
          }
        }
        return result;
      }
      break;
    case LinuxRawIEEE1394Impl::ERROR_GENERATION:
      this->generation = request.generation;
      break;
    default:
      throw IEEE1394Exception("Unable to get adapters", this);
    }
  }
}

void LinuxRawIEEE1394::open() throw(IEEE1394Exception) {
  unsigned char primaryAdapter[8] = {0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01};
  open(EUI64(primaryAdapter));
}

void LinuxRawIEEE1394::open(const EUI64& adapter) throw(IEEE1394Exception) {
  const unsigned char* id = adapter.getBytes();
  assert(
    (id[0] == 0xff) && (id[1] == 0xff) && (id[2] == 0xff) && (id[3] == 0x00),
    IEEE1394Exception("Adapter not present", this)
  );
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_OPEN_ADAPTER;
  request.generation = generation;
  request.misc = ((((((static_cast<unsigned int>(id[4]) << 8) | id[5]) << 8) | id[6]) << 8) | id[7]) - 1;
  
  // TAG: must be atomic
  assert(::write(handle, &request, sizeof(request)) == sizeof(request), IEEE1394Exception("Unable to queue request", this));
  assert(::read(handle, &request, sizeof(request)) == sizeof(request), IEEE1394Exception("Unable to dequeue response", this));
  // TAG: dequeue until generation? while (request.generation < generation);
 
  switch (request.error) {
  case LinuxRawIEEE1394Impl::ERROR_NONE:
    switch (protocol) {
    case LinuxRawIEEE1394::PROTOCOL_3:
      numberOfNodes = request.misc & 0xffff;
      localId = (request.misc >> 16) & ((1 << 6) - 1);
      break;
    case LinuxRawIEEE1394::PROTOCOL_4:
      numberOfNodes = request.misc & 0xff;
      localId = (request.misc >> 16) & ((1 << 6) - 1);
      // isochronousResourceManagerNodeId = ((request.misc >> 8) & 0xff) | 0xffc0;
      break;
    }
    generation = request.generation;
    break;
  case LinuxRawIEEE1394Impl::ERROR_GENERATION:
    generation = request.generation;
    throw UnexpectedFailure("Synchronization problem", this);
    break;
  default:
    throw IEEE1394Exception("Unable to open adapter", this);
  }
}

void LinuxRawIEEE1394::close() throw(IEEE1394Exception) {
  // close port?
}

unsigned int LinuxRawIEEE1394::getLocalId() const throw() {
  return localId;
}

unsigned int LinuxRawIEEE1394::getNumberOfNodes() const throw() {
  return numberOfNodes;
}

IEEE1394Common::Standard LinuxRawIEEE1394::getCompliance() const throw(IEEE1394Exception) {
  return STANDARD_IEEE_1394;
}

unsigned int LinuxRawIEEE1394::getStatus() const throw(IEEE1394Exception) {
  return status;
}

unsigned int LinuxRawIEEE1394::getFIFOSize() const throw(IEEE1394Exception) {
  return 1024;
}

uint64 LinuxRawIEEE1394::getPresentNodes() const throw(IEEE1394Exception) {
  return (1ULL << numberOfNodes) - 1; // this works because numberOfNodes < 64
}
  
uint64 LinuxRawIEEE1394::getLinkOnNodes() const throw(IEEE1394Exception) {
  return 0;
}

uint64 LinuxRawIEEE1394::getContenders() const throw(IEEE1394Exception) {
  return 0;
}
  
unsigned int LinuxRawIEEE1394::getSpeedOfNode(unsigned short node) const throw(IEEE1394Exception) {
  // look up in speed map for old std
  return 100;
}
  
unsigned int LinuxRawIEEE1394::getMaximumSpeedToNode(unsigned short node) const throw(IEEE1394Exception) {
  // look up in speed map for old std
  return 100;
}
  
unsigned int LinuxRawIEEE1394::getBroadcastSpeed() const throw(IEEE1394Exception) {
  // look up in speed map for old std
  return 100;
}

void LinuxRawIEEE1394::dequeueResponse() throw(IEEE1394Exception) {
  // TAG: need timeout support
  // TAG: must be MT-safe
  
  LinuxRawIEEE1394Impl::Request request;
  if (::read(handle, &request, sizeof(request)) != sizeof(request)) { // blocking
    throw IEEE1394Exception("Unable to wait for event", this);
  }

//   LinuxRawIEEE1394Impl::dumpRequest(request);
    
  switch (request.type) {
  case LinuxRawIEEE1394::EVENT_BUS_RESET:
    switch (protocol) {
    case LinuxRawIEEE1394::PROTOCOL_4:
      numberOfNodes = request.misc & 0xff;
      // isochronousResourceManagerNodeId = ((request.misc >> 8) & 0xff) | 0xffc0;
      localId = (request.misc >> 16) & ((1 << 6) - 1);
      break;
    case LinuxRawIEEE1394::PROTOCOL_3:
      numberOfNodes = request.misc & 0xffff;
      localId = (request.misc >> 16) & ((1 << 6) - 1);
      break;
    }
    generation = request.generation;

    fout << "EVENT: BUS RESET" << ENDL; // TAG: what to do
    break;
  case LinuxRawIEEE1394::EVENT_ISO_RECEIVE:
    {
      const IEEE1394Impl::Quadlet* quadlet =
        Cast::pointer<const IEEE1394Impl::Quadlet*>(Cast::getPointer(request.receiveBuffer));
      unsigned int channel = (*quadlet >> 8) & 0x3f;
      fout << "EVENT: Received isochronous packet" << EOL
           << "  channel: " << channel << EOL
           << "  buffer: " << request.receiveBuffer << EOL
           << "  packet size: " << "??? look in packet" << EOL
           << "  size: " << request.size << ENDL;
    }
    break;
  case LinuxRawIEEE1394::EVENT_FCP_REQUEST:
    fout << "FCP EVENT" << ENDL;
    //fcp_handler(request.misc & 0xffff, request.misc >> 16, request.size, request.send or receive buffer);
    break;
  default:
    LinuxRawIEEE1394Impl::RequestContext* requestContext =
      Cast::pointer<LinuxRawIEEE1394Impl::RequestContext*>(Cast::getPointer(request.tag));
    assert(requestContext, UnexpectedFailure("Invalid response", this));
    switch (requestContext->type) {
    case LinuxRawIEEE1394::REQUEST_ASYNC_READ:
    case LinuxRawIEEE1394::REQUEST_ASYNC_WRITE:
    case LinuxRawIEEE1394::REQUEST_LOCK:
      requestContext->dequeued = true;
      requestContext->status = LinuxRawIEEE1394Impl::mapErrorToStatus(request.error);
//       fout << "Dequeue request: " << requestContext->type << EOL
//            << "  error: " << HEX << setWidth(10) << ZEROPAD << request.error << EOL
//            << "  status: " << requestContext->status << ENDL;
      break;
    default:
      throw UnexpectedFailure("Invalid context of response", this);
    }
  }
}

// TAG: add exception OutOfDomain
void LinuxRawIEEE1394::read(unsigned short node, uint64 address, char* buffer, unsigned int size) throw(IEEE1394Exception) {
  assert(size % sizeof(IEEE1394Impl::Quadlet) == 0, OutOfDomain(this));
  
  // TAG: use maximum async payload?
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ASYNC_READ;
  
  char quadlet[sizeof(IEEE1394Impl::Quadlet)]; // preserved byte order
  
  // asynchronous read request
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ASYNC_READ;
  request.tag = Cast::getOffset(&requestContext);
  request.size = sizeof(IEEE1394Impl::Quadlet);
  request.address = (static_cast<uint64>(node) << 48) | (address & ((static_cast<uint64>(1) << 48) - 1));
  request.receiveBuffer = Cast::getOffset(&quadlet);
  
  for (unsigned int i = size/sizeof(IEEE1394Impl::Quadlet); i > 0; --i) {
    requestContext.dequeued = false;
    request.generation = generation;
    assert(
      ::write(handle, &request, sizeof(request)) == sizeof(request),
      IEEE1394Exception("Unable to request read", this)
    );
    
    while (!requestContext.dequeued) { // wait for completion
      dequeueResponse();
    }
    status = requestContext.status;
    assert(status == IEEE1394Impl::STATUS_OK, IEEE1394Exception(this));
    
    *buffer++ = quadlet[0];
    *buffer++ = quadlet[1];
    *buffer++ = quadlet[2];
    *buffer++ = quadlet[3];
    
    request.address += sizeof(IEEE1394Impl::Quadlet);
  }
}

void LinuxRawIEEE1394::write(unsigned short node, uint64 address, const char* buffer, unsigned int size) throw(IEEE1394Exception) {
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ASYNC_WRITE;
  requestContext.dequeued = false;
  
  // asynchronous write request
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ASYNC_WRITE;
  request.generation = generation;
  request.tag = Cast::getOffset(&requestContext);
  request.address = (static_cast<uint64>(node) << 48) | (address & ((static_cast<uint64>(1) << 48) - 1));
  request.size = size;
  request.sendBuffer = Cast::getOffset(buffer);
  assert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to request write", this)
  );
  
  // wait for completion
  while (!requestContext.dequeued) {
    dequeueResponse();
  }
  status = requestContext.status;
}

unsigned int LinuxRawIEEE1394::read(unsigned short node, uint64 address, uint32* buffer, unsigned int size, uint32 value) throw(IEEE1394Exception) {
  // assert((node < IEEE1394::BROADCAST) && (address % sizeof(*buffer) == 0), OutOfDomain(this));
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ASYNC_READ;

  IEEE1394Impl::Quadlet quadlet;
  
  // asynchronous read request
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ASYNC_READ;
  request.generation = generation;
  request.tag = Cast::getOffset(&requestContext);
  request.size = sizeof(quadlet);
  request.receiveBuffer = Cast::getOffset(&quadlet);
  
  unsigned int hits = 0;
  
  for (unsigned int i = 0; i < size; ++i) {
    unsigned int attempt = 0;
    while (++attempt <= IEEE1394::MAXIMUM_ATTEMPTS) {
      requestContext.dequeued = false;
      request.address = (static_cast<uint64>(IEEE1394Impl::makeNodeId(node)) << 48) |
        (address & ((static_cast<uint64>(1) << 48) - 1));
      assert(
        ::write(handle, &request, sizeof(request)) == sizeof(request),
        IEEE1394Exception("Unable to request read", this)
      );
      while (!requestContext.dequeued) { // wait for completion
        dequeueResponse();
      }
      status = requestContext.status;
      if (requestContext.status == IEEE1394Impl::STATUS_OK) {
        break;
      }
      
      // TAG: should throw exception BusReset on bus reset?
    }
    if (status == IEEE1394Impl::STATUS_OK) {
      *buffer++ = quadlet;
      ++hits;
    } else {
      *buffer++ = value;
    }
    address += sizeof(quadlet);
  }
  return hits;
}

void LinuxRawIEEE1394::lock(unsigned short node, uint64 address, unsigned int size) throw(IEEE1394Exception) {
  unsigned int extcode;
  IEEE1394Impl::Quadlet data;
  IEEE1394Impl::Quadlet arg;
  IEEE1394Impl::Quadlet* result;
  
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_LOCK;
  requestContext.dequeued = false;
  
  IEEE1394Impl::Quadlet buffer[2];

  if ((extcode > 7) || (extcode == 0)) {
    throw IEEE1394Exception("extcode is invalid", this);
  }
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_LOCK;
  request.generation = generation;
  request.tag = Cast::getOffset(&requestContext);
  request.address = (static_cast<uint64>(node) << 48) | (address & ((static_cast<uint64>(1) << 48) - 1));
  request.sendBuffer = Cast::getOffset(&buffer);
  request.receiveBuffer = Cast::getOffset(&buffer);
  request.misc = extcode;
  
  switch (extcode) {
  case 3: // EXTCODE_FETCH_ADD
  case 4: // EXTCODE_LITTLE_ADD
    buffer[0] = data;
    request.size = 4;
    break;
  default:
    buffer[0] = arg;
    buffer[1] = data;
    request.size = 8;
    break;
  }
  
  if (::write(handle, &request, sizeof(request)) != sizeof(request)) {
    throw IEEE1394Exception("Unable to request lock", this);
  }
  
  // wait for completion
  while (!requestContext.dequeued) {
    dequeueResponse();
  }
  status = requestContext.status;
}
 
IEEE1394Common::IsochronousReadChannel LinuxRawIEEE1394::getReadChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
  throw IEEE1394Exception("getReadChannel is not impl", this);
}

IEEE1394Common::IsochronousWriteChannel LinuxRawIEEE1394::getWriteChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
  throw IEEE1394Exception("getWriteChannel is not impl", this);
}

LinuxRawIEEE1394::~LinuxRawIEEE1394() throw(IEEE1394Exception) {
  ::close(handle);
}

void LinuxRawIEEE1394::readIsochronous(char* buffer, unsigned int size, unsigned int channel) throw(OutOfDomain, IEEE1394Exception) {
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ASYNC_READ;
  requestContext.dequeued = false;
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ISO_LISTEN;
  request.generation = generation;
  request.misc = channel;
  request.tag = Cast::getOffset(&requestContext);
  request.size = size;
  request.receiveBuffer = Cast::getOffset(buffer);
  assert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to request read", this)
  );
  
  while (!requestContext.dequeued) { // wait for completion
    dequeueResponse();
  }
  status = requestContext.status;
}

void LinuxRawIEEE1394::writeIsochronous(const char* buffer, unsigned int size, unsigned int channel, unsigned int tag, unsigned int sy, unsigned int speed) throw(OutOfDomain, IEEE1394Exception) {

  switch (speed) { // TAG: use IEEE1394Common::Speed type?
  case 100:
  case 200:
  case 400:
    break;
  default:
    throw OutOfDomain(this);
  }
  
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ISO_SEND;
  requestContext.dequeued = false;
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ISO_SEND;
  request.generation = generation;
  request.misc = (tag << 16) | sy;
  request.tag = Cast::getOffset(&requestContext);
  request.address = (static_cast<uint64>(channel) << 48) | speed;
  request.size = size;
  request.sendBuffer = Cast::getOffset(buffer);
  assert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to request isochronous write", this)
  );
  while (!requestContext.dequeued) { // wait for completion
    dequeueResponse();
  }
  status = requestContext.status;
}

// void LinuxRawIEEE1394::startIsochronousReceive(unsigned int channel) throw(OutOfDomain, IEEE1394Exception) {
//   assert(channel <= BROADCAST, OutOfDomain(this));
//   readIsochronous(channel);
// }

// void LinuxRawIEEE1394::stopIsochronousReceive(unsigned int channel) throw(OutOfDomain, IEEE1394Exception) {
//   assert(channel <= BROADCAST, OutOfDomain(this));
//   readIsochronous(~channel);
// }

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
