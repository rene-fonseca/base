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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Dummy IEEE 1394 implementation.
  
  @short Dummy IEEE 1394 implementation.
  @ingroup communications
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DummyIEEE1394 : public IEEE1394Impl {
public:

  /**
    Initializes IEEE 1394 implementation.
  */
  DummyIEEE1394();
  
  /**
    Returns true if the bus has been reset since last check.
  */
  bool hasBeenReset() const noexcept;

  /**
    Acknowledges the bus reset.
  */
  void acknowledgeReset() noexcept;

  /**
    Resets the bus.
  */
  void resetBus();

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
    Closes this handle to the adapter. The adapter is destroyed when all handles
    have been closed.
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

    @param node The node id of source node.
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

    @param node The node id of destination node.
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

    @param node The node id of source node.
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
  
  void readIsochronous(
    unsigned int channel,
    unsigned int maximumPayload,
    IsochronousChannelListener* listener
  );

  bool wait(unsigned int milliseconds);
  
  void dequeue();
  
  void registerFCPListener(
    FunctionControlProtocolListener* listener);
  
  void unregisterFCPListener();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
