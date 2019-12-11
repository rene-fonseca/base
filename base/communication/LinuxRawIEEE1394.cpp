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
#include <base/communication/LinuxRawIEEE1394.h>
#include <base/Cast.h>
#include <base/UnexpectedFailure.h>
#include <base/concurrency/Thread.h> // TAG: remove when done
#include <base/string/ANSIEscapeSequence.h>
#include <base/filesystem/FileSystem.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0

#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__UNIX)
#  error LinuxRawIEEE1394Impl not supported by flavor
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

namespace LinuxRawIEEE1394Impl {
  
  const Literal DEVICE_NAME = Literal("raw1394");
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
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
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

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
      case IEEE1394Impl::ACK_COMPLETE:
        status = IEEE1394Impl::STATUS_OK;
        break;
      case IEEE1394Impl::ACK_PENDING:
        switch (error & 0xffff) {
        case IEEE1394Impl::RCODE_COMPLETE:
          status = IEEE1394Impl::STATUS_OK;
          break;
        case IEEE1394Impl::RCODE_CONFLICT_ERROR:
          status = IEEE1394Impl::STATUS_CONFLICT;
          break;
        case IEEE1394Impl::RCODE_DATA_ERROR: // TAG: check spec
        case IEEE1394Impl::RCODE_TYPE_ERROR: // TAG: check spec
        case IEEE1394Impl::RCODE_ADDRESS_ERROR:
        default:
          status = IEEE1394Impl::STATUS_INTERNAL;
        }
        break;
      case IEEE1394Impl::ACK_BUSY_X:
      case IEEE1394Impl::ACK_BUSY_A:
      case IEEE1394Impl::ACK_BUSY_B:
        status = IEEE1394Impl::STATUS_BUSY;
        break;
      case IEEE1394Impl::ACK_DATA_ERROR:
      case IEEE1394Impl::ACK_TYPE_ERROR:
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
    fout << "request: " << EOL
         << indent(2) << "type: " << request.type << EOL
         << indent(2) << "error: " << request.error << EOL
         << indent(2) << "misc: "
         << HEX << setWidth(10) << ZEROPAD << request.misc << EOL
         << indent(2) << "generation: " << request.generation << EOL
         << indent(2) << "size: " << request.size << EOL
         << indent(2) << "address: "
         << HEX << setWidth(18) << ZEROPAD << request.address << EOL
         << indent(2) << "tag: "
         << HEX << setWidth(18) << ZEROPAD << request.tag << EOL
         << indent(2) << "sendBuffer: "
         << HEX << setWidth(18) << ZEROPAD << request.sendBuffer << EOL
         << indent(2) << "receiveBuffer: "
         << HEX << setWidth(18) << ZEROPAD << request.receiveBuffer << EOL
         << ENDL;
  }
  
}; // end of LinuxRawIEEE1394Impl namespace

LinuxRawIEEE1394::LinuxRawIEEE1394() throw(IEEE1394Exception) {
  handle = ::open(
    FileSystem::toAbsolutePath(
      FileSystem::getFolder(FileSystem::DEVICES),
      LinuxRawIEEE1394Impl::DEVICE_NAME
    ).getElements(),
    O_RDWR
  );
  bassert(handle >= 0, IEEE1394Exception("Unable to open device", this));
  
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
    throw IEEE1394Exception("Incompatibility.", this);
  }

  fcpListener = 0;
  for (unsigned int i = 0; i < IEEE1394Impl::ISOCHRONOUS_CHANNELS; ++i) {
    isochronousChannels[i].listener = 0;
    isochronousChannels[i].completed = true;
    isochronousChannels[i].busy = false;
    isochronousChannels[i].status = IEEE1394Impl::STATUS_OK;
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
  bassert(::write(handle, &request, sizeof(request)) >= 0, IEEE1394Exception("Unable to reset bus", this));
  // dequeue
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
      throw IEEE1394Exception("Unable to get adapters.", this);
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
          for (unsigned int j = 7; (j > 2) && (++artificialGuid[j] == 0x00); --j) { // next artificial guid
            artificialGuid[j]++; // maximum number of adapters is small
          }
        }
        return result;
      }
      break;
    case LinuxRawIEEE1394Impl::ERROR_GENERATION:
      this->generation = request.generation;
      break;
    default:
      throw IEEE1394Exception("Unable to get adapters.", this);
    }
  }
}

void LinuxRawIEEE1394::open() throw(IEEE1394Exception) {
  unsigned char primaryAdapter[8] = {0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01};
  open(EUI64(primaryAdapter));
}

void LinuxRawIEEE1394::open(const EUI64& adapter) throw(IEEE1394Exception) {
  const unsigned char* id = adapter.getBytes();
  bassert(
    (id[0] == 0xff) && (id[1] == 0xff) && (id[2] == 0xff) && (id[3] == 0x00),
    IEEE1394Exception("Adapter not present", this)
  );
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_OPEN_ADAPTER;
  request.generation = generation;
  request.misc = ((((((static_cast<unsigned int>(id[4]) << 8) | id[5]) << 8) | id[6]) << 8) | id[7]) - 1;
  
  // TAG: must be atomic
  bassert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to queue request", this)
  );
  bassert(
    ::read(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to dequeue response", this)
  );
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
      // isochronousResourceManagerId = (request.misc >> 8) & 0x3f;
      break;
    }
    generation = request.generation;
    break;
  case LinuxRawIEEE1394Impl::ERROR_GENERATION:
    generation = request.generation;
    throw UnexpectedFailure("Synchronization problem.", this);
    break;
  default:
    throw IEEE1394Exception("Unable to open adapter.", this);
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

unsigned int LinuxRawIEEE1394::getStatus() const throw(IEEE1394Exception) {
  return status;
}

unsigned int LinuxRawIEEE1394::getFIFOSize() const throw(IEEE1394Exception) {
  return 1024;
}

void LinuxRawIEEE1394::dequeueResponse() throw(IEEE1394Exception) {
  // TAG: need timeout support
  // TAG: must be MT-safe
  
  LinuxRawIEEE1394Impl::Request request;
  if (::read(handle, &request, sizeof(request)) != sizeof(request)) { // blocking
    throw IEEE1394Exception("Unable to wait for event.", this);
  }
  
  switch (request.type) {
  case LinuxRawIEEE1394::EVENT_BUS_RESET:
    switch (protocol) {
    case LinuxRawIEEE1394::PROTOCOL_4:
      numberOfNodes = request.misc & 0xff;
      // isochronousResourceManagerId = (request.misc >> 8) & 0x3f;
      localId = (request.misc >> 16) & ((1 << 6) - 1);
      break;
    case LinuxRawIEEE1394::PROTOCOL_3:
      numberOfNodes = request.misc & 0xffff;
      localId = (request.misc >> 16) & ((1 << 6) - 1);
      break;
    }
    generation = request.generation; // TAG: need to notify IEEE1394 object
    break;
  case LinuxRawIEEE1394::EVENT_ISO_RECEIVE:
    {
      const LittleEndian<uint32>* quadlet =
        Cast::getPointer<const LittleEndian<uint32>*>(request.receiveBuffer);
      const unsigned int tcode = (*quadlet >> 4) & 0x0f;
      const unsigned int channel = (*quadlet >> 8) & 0x3f;
      const unsigned int size = *quadlet >> 16;
      bassert(
        (tcode == IEEE1394Impl::TCODE_ISOCHRONOUS_DATA_BLOCK) &&
        isochronousChannels[channel].listener &&
        (size <= isochronousChannels[channel].buffer.getSize()),
        UnexpectedFailure("Invalid isochronous packet", this)
      );
      isochronousChannels[channel].listener->onIsochronousPacket(
        Cast::getPointer<const uint8*>(request.receiveBuffer),
        size
      );
    }
    break;
  case LinuxRawIEEE1394::EVENT_FCP_REQUEST:
    bassert(fcpListener, UnexpectedFailure("Invalid FCP response", this)); // TAG: is this ok
    if (fcpListener) {
      unsigned short nodeId = request.misc & 0xffff;
      bool response = (request.misc >> 16) != 0;
      // TAG: should we copy buffer
      if (response) {
        fcpListener->onFCPResponse(
          nodeId,
          Cast::getPointer<const uint8*>(request.receiveBuffer),
          request.size
        );
      } else {
        fcpListener->onFCPRequest(
          nodeId,
          Cast::getPointer<const uint8*>(request.receiveBuffer),
          request.size
        );
      }
    }
    break;
  default:
    // TAG: possibly ignore response?
    LinuxRawIEEE1394Impl::RequestContext* requestContext =
      Cast::getPointer<LinuxRawIEEE1394Impl::RequestContext*>(request.tag);
    bassert(requestContext, UnexpectedFailure("Invalid response", this));
    switch (requestContext->type) {
    case LinuxRawIEEE1394::REQUEST_ASYNC_READ:
    case LinuxRawIEEE1394::REQUEST_ASYNC_WRITE:
    case LinuxRawIEEE1394::REQUEST_LOCK:
    case LinuxRawIEEE1394::REQUEST_LOCK64:
    case LinuxRawIEEE1394::REQUEST_FCP_LISTEN:
    case LinuxRawIEEE1394::REQUEST_ISO_SEND:
    case LinuxRawIEEE1394::REQUEST_ISO_LISTEN:
      requestContext->status = LinuxRawIEEE1394Impl::mapErrorToStatus(request.error);
      requestContext->dequeued = true;
      break;
    default: // not built-in request
      throw UnexpectedFailure("Invalid context of response.", this);
    }
  }
}

// TAG: add exception OutOfDomain
void LinuxRawIEEE1394::read(
  unsigned short node,
  uint64 address,
  uint8* buffer,
  unsigned int size) throw(IEEE1394Exception) {
  bassert(size % sizeof(IEEE1394Impl::Quadlet) == 0, OutOfDomain(this));
  
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
    for (unsigned int attempt = 0; ; ++attempt) {
      requestContext.dequeued = false;
      request.generation = generation;
      bassert(
        ::write(handle, &request, sizeof(request)) == sizeof(request),
        IEEE1394Exception("Unable to request read", this)
      );
      while (!requestContext.dequeued) { // wait for completion
        dequeueResponse();
      }
      if (requestContext.status == IEEE1394Impl::STATUS_TIMEOUT) {
        bassert(
          attempt < IEEE1394Impl::MAXIMUM_ATTEMPTS,
          IEEE1394Exception(this)
        );
        fout << setForeground(ANSIEscapeSequence::BLUE) << bold()
             << "<TIMEOUT>" << normal() << FLUSH;
        Thread::millisleep(10);
        continue;
      }
      bassert(requestContext.status == IEEE1394Impl::STATUS_OK, IEEE1394Exception(this));
      break;
    }
    
    *buffer++ = quadlet[0];
    *buffer++ = quadlet[1];
    *buffer++ = quadlet[2];
    *buffer++ = quadlet[3];
    
    request.address += sizeof(IEEE1394Impl::Quadlet);
  }
}

void LinuxRawIEEE1394::write(
  unsigned short node,
  uint64 address,
  const uint8* buffer,
  unsigned int size) throw(IEEE1394Exception) {
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ASYNC_WRITE;
  requestContext.dequeued = false;
  
  // asynchronous write request
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ASYNC_WRITE;
  request.generation = generation;
  request.tag = Cast::getOffset(&requestContext);
  request.address = (static_cast<uint64>(node) << 48) |
    (address & ((static_cast<uint64>(1) << 48) - 1));
  request.size = size;
  request.sendBuffer = Cast::getOffset(buffer);
  bassert(
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
  // bassert((node < IEEE1394::BROADCAST) && (address % sizeof(*buffer) == 0), OutOfDomain(this));
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
    request.address = (static_cast<uint64>(node) << 48) |
      (address & ((static_cast<uint64>(1) << 48) - 1));
    unsigned int attempt = 0;
    while (++attempt <= IEEE1394Impl::MAXIMUM_ATTEMPTS) {
      requestContext.dequeued = false;
      
      bassert(
        ::write(handle, &request, sizeof(request)) == sizeof(request),
        IEEE1394Exception("Unable to request read", this)
      );
      while (!requestContext.dequeued) { // wait for completion
        dequeueResponse();
      }
      if (requestContext.status == IEEE1394Impl::STATUS_OK) {
        break;
      }
      if (requestContext.status == IEEE1394Impl::STATUS_TIMEOUT) {
        fout << setForeground(ANSIEscapeSequence::BLUE) << bold()
             << "<TIMEOUT>" << normal() << FLUSH;
        Thread::millisleep(10);
      }
      
      // TAG: should throw exception BusReset on bus reset?
    }
    if (requestContext.status == IEEE1394Impl::STATUS_OK) {
      *buffer++ = quadlet;
      ++hits;
    } else {
      *buffer++ = value;
    }
    address += sizeof(quadlet);
  }
  return hits;
}

uint32 LinuxRawIEEE1394::lock(unsigned short node, uint64 address, LockInstruction instruction, uint32 argument, uint32 data) throw(IEEE1394Exception) {
  bassert((getPhysicalId(node) < IEEE1394Impl::BROADCAST) && (address % sizeof(Quadlet) == 0), IEEE1394Exception(this));

  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_LOCK;
  requestContext.dequeued = false;
  
  IEEE1394Impl::Quadlet buffer[2];
  Quadlet result;
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_LOCK;
  request.generation = generation;
  request.tag = Cast::getOffset(&requestContext);
  request.address = (static_cast<uint64>(node) << 48) | (address & ((static_cast<uint64>(1) << 48) - 1));
  request.sendBuffer = Cast::getOffset(&buffer);
  request.receiveBuffer = Cast::getOffset(&result);
  
  switch (instruction) {
  case IEEE1394Impl::MASK_SWAP:
    buffer[0] = argument;
    buffer[1] = data;
    request.size = 2 * sizeof(Quadlet);
    request.misc = 1;
    break;
  case IEEE1394Impl::COMPARE_SWAP:
    buffer[0] = argument;
    buffer[1] = data;
    request.size = 2 * sizeof(Quadlet);
    request.misc = 2;
    break;
  case IEEE1394Impl::FETCH_ADD:
    buffer[0] = data;
    request.size = sizeof(Quadlet);
    request.misc = 3;
    break;
  case IEEE1394Impl::LITTLE_FETCH_ADD:
    buffer[0] = data;
    request.size = sizeof(Quadlet);
    request.misc = 4;
    break;
  case IEEE1394Impl::BOUNDED_ADD:
    buffer[0] = argument;
    buffer[1] = data;
    request.size = 2 * sizeof(Quadlet);
    request.misc = 5;
    break;
  case IEEE1394Impl::WRAP_ADD:
    buffer[0] = argument;
    buffer[1] = data;
    request.size = 2 * sizeof(Quadlet);
    request.misc = 6;
    break;
  }
  
  if (::write(handle, &request, sizeof(request)) != sizeof(request)) {
    throw IEEE1394Exception("Unable to request lock.", this);
  }
  
  // wait for completion
  while (!requestContext.dequeued) { // TAG: need time out support
    dequeueResponse();
  }
  status = requestContext.status;
  return result;
}
 
IEEE1394Common::IsochronousReadChannel LinuxRawIEEE1394::getReadChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
  throw IEEE1394Exception("getReadChannel is not impl.", this);
}

IEEE1394Common::IsochronousWriteChannel LinuxRawIEEE1394::getWriteChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
  throw IEEE1394Exception("getWriteChannel is not impl.", this);
}

LinuxRawIEEE1394::~LinuxRawIEEE1394() {
  ::close(handle);
}

void LinuxRawIEEE1394::registerFCPListener(FunctionControlProtocolListener* listener) throw(IEEE1394Exception) {
  if (!listener) {
    throw NullPointer(this);
  }
  
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_FCP_LISTEN;
  requestContext.dequeued = false;

  fcpListener = listener;

  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_FCP_LISTEN;
  request.generation = generation;
  request.misc = 1; // start
  request.tag = Cast::getOffset(&requestContext);
  request.size = sizeof(fcpBuffer);
  request.receiveBuffer = Cast::getOffset(fcpBuffer);
  bassert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to regiser FCP listener", this)
  );
  
  while (!requestContext.dequeued) { // TAG: need time out support
    dequeueResponse();
  }
}

void LinuxRawIEEE1394::unregisterFCPListener() throw(IEEE1394Exception) {
  fcpListener = 0;
  
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_FCP_LISTEN;
  requestContext.dequeued = false;
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_FCP_LISTEN;
  request.generation = generation;
  request.misc = 0; // stop
  request.tag = Cast::getOffset(&requestContext);
  bassert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to unregister FCP listener", this)
  );
  
  while (!requestContext.dequeued) { // wait for completion
    dequeueResponse();
  }
  status = requestContext.status;
}

void LinuxRawIEEE1394::readIsochronous(unsigned int channel, unsigned int maximumPayload, IsochronousChannelListener* listener) throw(OutOfDomain, IEEE1394Exception) {
  bassert(listener, OutOfDomain(this)); // TAG: NullPointer
  bassert((maximumPayload > 0) && ((maximumPayload % sizeof(Quadlet)) == 0), OutOfDomain(this));
  bassert(channel < IEEE1394Impl::ISOCHRONOUS_CHANNELS, OutOfDomain(this));
  bassert(!isochronousChannels[channel].busy, IEEE1394Exception("Isochronous channel conflict", this));
  
  // TAG: need lock for isochronousChannels[channel]
  isochronousChannels[channel].busy = true;
  isochronousChannels[channel].buffer.setSize(maximumPayload);
  isochronousChannels[channel].listener = listener;
  
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ISO_LISTEN;
  requestContext.dequeued = false;

  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ISO_LISTEN;
  request.generation = generation;
  request.misc = channel;
  request.tag = Cast::getOffset(&requestContext);
  request.size = maximumPayload;
  request.receiveBuffer = Cast::getOffset(isochronousChannels[channel].buffer.getElements());
  bassert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to enable isochronous channel listener", this)
  );

  while (!requestContext.dequeued) { // TAG: need time out support
    dequeueResponse();
  }
  bassert(
    requestContext.status == IEEE1394Impl::STATUS_OK,
    IEEE1394Exception("Unable to enable isochronous channel listener", this)
  );

  // listen for incoming packets
  isochronousChannels[channel].completed = false;
  try {
    while (!isochronousChannels[channel].completed) { // TAG: need time out support
      dequeueResponse();
    }
  } catch (IEEE1394Exception&) { // still need to disable listener
  }

  isochronousChannels[channel].listener = 0; // prevent invocation of listener method
  
  // stop listener
  requestContext.dequeued = false;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ISO_LISTEN;
  request.generation = generation;
  request.misc = ~channel; // disable
  request.tag = Cast::getOffset(&requestContext);
  bassert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to disable isochronous channel listener", this)
  );
  
  while (!requestContext.dequeued) { // wait for completion
    dequeueResponse();
  }
  status = isochronousChannels[channel].status;
  bassert(
    requestContext.status == IEEE1394Impl::STATUS_OK,
    IEEE1394Exception("Unable to disable isochronous channel listener", this)
  );
  
  // do not release on failure to disable listener
  isochronousChannels[channel].buffer.setSize(0);
  isochronousChannels[channel].busy = false;
}

bool LinuxRawIEEE1394::wait(unsigned int milliseconds) throw(OutOfDomain, IEEE1394Exception) {
  bassert(milliseconds <= 999999999, OutOfDomain(this));
  struct pollfd fd;
  fd.fd = handle;
  fd.events = POLLIN;
  fd.revents = 0;
  int result = 0;
  do {
    result = ::poll(&fd, 1, milliseconds);
  } while (result < 0);
  return result > 0;
}

void LinuxRawIEEE1394::dequeue() throw(IEEE1394Exception) {
  dequeueResponse();
}

void LinuxRawIEEE1394::writeIsochronous(const uint8* buffer, unsigned int size, unsigned int channel, unsigned int tag, unsigned int sy, Speed speed) throw(OutOfDomain, IEEE1394Exception) {
  bassert((channel < IEEE1394Impl::ISOCHRONOUS_CHANNELS) && (speed <= IEEE1394Impl::S400), OutOfDomain(this));
  
  LinuxRawIEEE1394Impl::RequestContext requestContext;
  requestContext.type = LinuxRawIEEE1394::REQUEST_ISO_SEND;
  requestContext.dequeued = false;
  
  LinuxRawIEEE1394Impl::Request request;
  clear(request);
  request.type = LinuxRawIEEE1394::REQUEST_ISO_SEND;
  request.generation = generation;
  request.misc = (tag << 16) | sy;
  request.tag = Cast::getOffset(&requestContext);
  request.address = (static_cast<uint64>(channel) << 48) | static_cast<unsigned int>(speed);
  request.size = size;
  request.sendBuffer = Cast::getOffset(buffer);
  bassert(
    ::write(handle, &request, sizeof(request)) == sizeof(request),
    IEEE1394Exception("Unable to request isochronous write", this)
  );
  while (!requestContext.dequeued) { // wait for completion
    dequeueResponse();
  }
  status = requestContext.status;
}

#else

void LinuxRawIEEE1394::resetBus() throw(IEEE1394Exception) {
  throw NotSupported();
}

LinuxRawIEEE1394::LinuxRawIEEE1394() throw(IEEE1394Exception) {
  throw NotSupported();
}

bool LinuxRawIEEE1394::hasBeenReset() const throw() {
  throw NotSupported();
}

void LinuxRawIEEE1394::acknowledgeReset() throw() {
  throw NotSupported();
}

Array<EUI64> LinuxRawIEEE1394::getAdapters() throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::open() throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::open(const EUI64& adapter) throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::close() throw(IEEE1394Exception) {
  throw NotSupported();
}

unsigned int LinuxRawIEEE1394::getLocalId() const throw() {
  throw NotSupported();
}

unsigned int LinuxRawIEEE1394::getNumberOfNodes() const throw() {
  throw NotSupported();
}

unsigned int LinuxRawIEEE1394::getStatus() const throw(IEEE1394Exception) {
  throw NotSupported();
}

unsigned int LinuxRawIEEE1394::getFIFOSize() const throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::read(
  unsigned short node,
  uint64 address,
  uint8* buffer,
  unsigned int size) throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::write(
  unsigned short node,
  uint64 address,
  const uint8* buffer,
  unsigned int size) throw(IEEE1394Exception) {
  throw NotSupported();
}

unsigned int LinuxRawIEEE1394::read(
  unsigned short node,
  uint64 address,
  uint32* buffer,
  unsigned int size,
  uint32 value) throw(IEEE1394Exception) {
  throw NotSupported();
}

uint32 LinuxRawIEEE1394::lock(
  unsigned short node,
  uint64 address,
  LockInstruction instruction,
  uint32 argument,
  uint32 data) throw(IEEE1394Exception) {
  throw NotSupported();
}

LinuxRawIEEE1394::IsochronousReadChannel LinuxRawIEEE1394::getReadChannel(
  unsigned int maxPacketsPerRequest,
  uint64 subchannels) throw(IEEE1394Exception) {
  throw NotSupported();
}

LinuxRawIEEE1394::IsochronousWriteChannel LinuxRawIEEE1394::getWriteChannel(
  unsigned int maxPacketsPerRequest,
  uint64 subchannels) throw(IEEE1394Exception) {
  throw NotSupported();
}

LinuxRawIEEE1394::~LinuxRawIEEE1394() {
}

bool LinuxRawIEEE1394::wait(unsigned int milliseconds) throw(OutOfDomain, IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::dequeueResponse() throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::dequeue() throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::registerFCPListener(FunctionControlProtocolListener* listener) throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::unregisterFCPListener() throw(IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::readIsochronous(unsigned int channel, unsigned int maximumPayload, IsochronousChannelListener* listener) throw(OutOfDomain, IEEE1394Exception) {
  throw NotSupported();
}

void LinuxRawIEEE1394::writeIsochronous(const uint8* buffer, unsigned int size, unsigned int channel, unsigned int tag, unsigned int sy, Speed speed) throw(OutOfDomain, IEEE1394Exception) {
  throw NotSupported();
}

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
