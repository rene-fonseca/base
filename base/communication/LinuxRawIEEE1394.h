/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__LINUX_RAW_IEEE_1394_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__LINUX_RAW_IEEE_1394_H

#include <base/communication/IEEE1394Impl.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Linux (RAW) IEEE 1394 implementation.
  
  @short Linux (RAW) IEEE 1394 implementation.
  @ingroup communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class LinuxRawIEEE1394 : public IEEE1394Impl {
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
  
  /** Device handle. */
  OperatingSystem::Handle handle;
  /** The protocol (API). */
  Protocol protocol;
  /** The current status. */
  unsigned int status;
  /** The number of nodes of the local bus. */
  unsigned int numberOfNodes;
  /** The physical id of the local node. */
  unsigned int localId;
  /** Request identification. */
  uint32 generation;

  /** Dequeues one response from the driver. */
  void dequeueResponse() throw(IEEE1394Exception);
public:

  void resetBus() throw(IEEE1394Exception);
  
  
  
  /**
    Initializes IEEE 1394 implementation.
  */
  LinuxRawIEEE1394() throw(IEEE1394Exception);
  
  /**
    Returns true if the bus has been reset since last check.
  */
  bool hasBeenReset() const throw();

  /**
    Acknowledges the bus reset.
  */
  void acknowledgeReset() throw();

  /**
    Returns the adapters available.
  */
  Array<EUI64> getAdapters() throw(IEEE1394Exception);
  
  /**
    Opens a connection to the primary adapter.
  */
  void open() throw(IEEE1394Exception);
  
  /**
    Opens a connection to the specified adapter.
    
    @param adapter The id of the adapter.
  */
  void open(const EUI64& adapter) throw(IEEE1394Exception);
  
  /**
    Closes this handle to the adapter. The adapter is destroyed when all handles
    have been closed.
  */
  void close() throw(IEEE1394Exception);
  
  /**
    Returns the physical id of the adapter.
  */
  unsigned int getLocalId() const throw();

  /**
    Returns the number of nodes on the local bus.
  */
  unsigned int getNumberOfNodes() const throw();

  /**
    Returns the IEEE 1394 standard of the adapter.
  */
  Standard getCompliance() const throw(IEEE1394Exception);

  /**
    Returns the current error status.
  */
  unsigned int getStatus() const throw(IEEE1394Exception);

  /**
    Returns the size of the FIFO.
  */
  unsigned int getFIFOSize() const throw(IEEE1394Exception);

  /**
    Read data from device.

    @param node The node id of source node.
    @param address The base address of the memory region to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read (must be an integral number of quadlets).
  */
  void read(unsigned short node, uint64 address, char* buffer, unsigned int size) throw(IEEE1394Exception);

  /**
    Write data to device.

    @param node The node id of destination node.
    @param address The base address of the memory region to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write (must be an integral number of quadlets).
  */
  void write(unsigned short node, uint64 address, const char* buffer, unsigned int size) throw(IEEE1394Exception);

  /**
    Read data from device. This method is only used for debugging and development.

    @param node The node id of source node.
    @param address The base address of the memory region to read from (must be a quadlet boundary).
    @param buffer The data buffer.
    @param size The number of quadlets to read (not bytes!).
    @param value The default value to store in the buffer for unsuccessful reads.
    
    @return The number of quadlets read successfully.
  */
  unsigned int read(unsigned short node, uint64 address, uint32* buffer, unsigned int size, uint32 value) throw(IEEE1394Exception);

  // TAG: fixme
  void lock(unsigned short node, uint64 address, unsigned int size) throw(IEEE1394Exception);

  /**
    Returns an isochronous read channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this channel.
  */
  IsochronousReadChannel getReadChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception);
  
  /**
    Returns an isochronous write channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this channel.
  */
  IsochronousWriteChannel getWriteChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception);

  /**
    Destroys the IEEE 1394 implementation.
  */
  ~LinuxRawIEEE1394() throw(IEEE1394Exception);

  // TAG: FIXME
  void readIsochronous(char* buffer, unsigned int size, unsigned int channel) throw(OutOfDomain, IEEE1394Exception);

  // TAG: FIXME
  void writeIsochronous(const char* buffer, unsigned int size, unsigned int channel, unsigned int tag, unsigned int sy, Speed speed) throw(OutOfDomain, IEEE1394Exception);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
