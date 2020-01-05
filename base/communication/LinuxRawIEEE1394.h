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

#include <base/communication/IEEE1394Impl.h>
#include <base/OperatingSystem.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Linux (RAW) IEEE 1394 implementation.
  
  @short Linux (RAW) IEEE 1394 implementation.
  @ingroup communications
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LinuxRawIEEE1394 : public IEEE1394Impl {
private:
  
  enum RequestType {
    REQUEST_INITIALIZE = 1,
    REQUEST_GET_ADAPTERS = 2,
    REQUEST_OPEN_ADAPTER = 3,
    REQUEST_ASYNC_READ = 100,
    REQUEST_ASYNC_WRITE = 101,
    REQUEST_LOCK = 102,
    REQUEST_LOCK64 = 103,
    REQUEST_ISO_SEND = 104,
    REQUEST_ISO_LISTEN = 200,
    REQUEST_FCP_LISTEN = 201,
    REQUEST_RESET_BUS = 202,
    EVENT_BUS_RESET = 10000,
    EVENT_ISO_RECEIVE = 10001,
    EVENT_FCP_REQUEST = 10002
  };
  
  enum Protocol {
    PROTOCOL_3 = 3,
    PROTOCOL_4 = 4,
    PROTOCOL_DEFAULT = PROTOCOL_4
  };
  
  /** Specifies the maximum number of adapters. */
  static const unsigned int MAXIMUM_NUMBER_OF_ADAPTERS = 256;
  
  class IsochronousContext {
  public:

    Allocator<uint8> buffer;
    IsochronousChannelListener* listener = nullptr;
    bool completed = false;
    bool busy = false;
    Status status;
  };

  /** Device handle. */
  OperatingSystem::Handle handle;
  /** The protocol (API). */
  Protocol protocol = PROTOCOL_DEFAULT;
  /** The current status. */
  unsigned int status = 0;
  /** The number of nodes of the local bus. */
  unsigned int numberOfNodes = 0;
  /** The physical id of the local node. */
  unsigned int localId = 0;
  /** Request identification. */
  uint32 generation = 0;
  /** FCP listener. */
  FunctionControlProtocolListener* fcpListener = nullptr;
  /** FCP transaction buffer. */
  uint8 fcpBuffer[512];
  /** Isochronous channel descriptors. */
  IsochronousContext isochronousChannels[IEEE1394Impl::ISOCHRONOUS_CHANNELS];
public:

  void resetBus();
  
  
  
  /**
    Initializes IEEE 1394 implementation.
  */
  LinuxRawIEEE1394();
  
  /**
    Returns true if the bus has been reset since last check.
  */
  bool hasBeenReset() const;

  /**
    Acknowledges the bus reset.
  */
  void acknowledgeReset();

  /**
    Returns the adapters available.
  */
  Array<EUI64> getAdapters();
  
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
    Closes this handle to the adapter. The adapter is destroyed when all
    handles have been closed.
  */
  void close();
  
  /**
    Returns the physical id of the adapter.
  */
  unsigned int getLocalId() const;

  /**
    Returns the number of nodes on the local bus.
  */
  unsigned int getNumberOfNodes() const;

  /**
    Returns the current error status.
  */
  unsigned int getStatus() const;

  /**
    Returns the size of the FIFO.
  */
  unsigned int getFIFOSize() const;

  /**
    Read data from device.

    @param node The node id of the source node.
    @param address The base address of the memory region to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read (must be an integral number of
    quadlets).
  */
  void read(
    unsigned short node,
    uint64 address,
    uint8* buffer,
    unsigned int size);

  /**
    Write data to device.

    @param node The node id of the destination node.
    @param address The base address of the memory region to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write (must be an integral number of
    quadlets).
  */
  void write(
    unsigned short node,
    uint64 address,
    const uint8* buffer,
    unsigned int size);

  /**
    Read data from device. This method is only used for debugging and
    development.

    @param node The node id of the source node.
    @param address The base address of the memory region to read from (must be
    a quadlet boundary).
    @param buffer The data buffer.
    @param size The number of quadlets to read (not bytes!).
    @param value The default value to store in the buffer for unsuccessful
    reads.
    
    @return The number of quadlets read successfully.
  */
  unsigned int read(
    unsigned short node,
    uint64 address,
    uint32* buffer,
    unsigned int size,
    uint32 value);

  /**
    This methods provides atomic write access to quadlet.
    
    @param node The node id of the node.
    @param address The address of the quadlet (must be a quadlet boundary).
    @param instruction The atomic instruction.
    @param argument The argument to the instruction (ignored by FETCH_ADD and
    LITTLE_FETCH_ADD).
    @param data The data value use by the instruction.
    
    @return The result.
  */
  uint32 lock(
    unsigned short node,
    uint64 address,
    LockInstruction instruction,
    uint32 argument,
    uint32 data);

  /**
    Returns an isochronous read channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this
    channel.
  */
  IsochronousReadChannel getReadChannel(
    unsigned int maxPacketsPerRequest,
    uint64 subchannels);
  
  /**
    Returns an isochronous write channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this
    channel.
  */
  IsochronousWriteChannel getWriteChannel(
    unsigned int maxPacketsPerRequest,
    uint64 subchannels);

  /**
    Destroys the IEEE 1394 implementation.
  */
  ~LinuxRawIEEE1394();

  /**
    Wait for an event or until the timeout period expires.
    
    @param milliseconds The timeout period.
    
    @return True if event is ready to be dequeued.
  */
  bool wait(unsigned int milliseconds);
  
  /**
    Dequeues one response from the driver.
  */
  void dequeueResponse();

  void dequeue();

  void registerFCPListener(
    FunctionControlProtocolListener* listener);
  
  void unregisterFCPListener();

  void readIsochronous(
    unsigned int channel,
    unsigned int maximumPayload,
    IsochronousChannelListener* listener
  );

  void writeIsochronous(
    const uint8* buffer,
    unsigned int size,
    unsigned int channel,
    unsigned int tag,
    unsigned int sy,
    Speed speed);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
