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

#include <base/communication/IEEE1394Common.h>
#include <base/collection/Array.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  IEEE 1394 implementation.
  
  @short IEEE 1394 implementation.
  @ingroup communications
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IEEE1394Impl : public ReferenceCountedObject, public IEEE1394Common {
public:

  /**
    Returns the default IEEE 1394 implementation.
  */
  static IEEE1394Impl* getDefault() noexcept;
  
  /**
    Returns true if the bus has been reset since last check.
  */
  virtual bool hasBeenReset() const = 0;

  /**
    Acknowledges the bus reset.
  */
  virtual void acknowledgeReset() = 0;

  /**
    Resets the bus.
  */
  virtual void resetBus() = 0;
    
  /**
    Returns the adapters available.
  */
  virtual Array<EUI64> getAdapters() = 0;
  
  /**
    Opens a connection to the primary adapter.
  */
  virtual void open() = 0;
  
  /**
    Opens a connection to the specified adapter.
    
    @param adapter The id of the adapter.
  */
  virtual void open(const EUI64& adapter) = 0;
  
  /**
    Closes this handle to the adapter. The adapter is destroyed when all
    handles have been closed.
  */
  virtual void close() = 0;

  /**
    Returns the physical id of the adapter.
  */
  virtual unsigned int getLocalId() const = 0;

  /**
    Returns the number of nodes on the local bus.
  */
  virtual unsigned int getNumberOfNodes() const = 0;
  
  /**
    Returns the current error status.
  */
  virtual unsigned int getStatus() const = 0;

  /**
    Returns the size of the FIFO.
  */
  virtual unsigned int getFIFOSize() const = 0;
  
  /**
    Read data from device.

    @param node The node id of source node.
    @param address The base address of the memory region to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read (must be an integral number of
    quadlets).
  */
  virtual void read(
    unsigned short node,
    uint64 address,
    uint8* buffer,
    unsigned int size) = 0;

  /**
    Write data to device.

    @param node The node id of destination node.
    @param address The base address of the memory region to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write (must be an integral number of
    quadlets).
  */
  virtual void write(
    unsigned short node,
    uint64 address,
    const uint8* buffer,
    unsigned int size) = 0;

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
  virtual unsigned int read(
    unsigned short node,
    uint64 address,
    uint32* buffer,
    unsigned int size,
    uint32 value) = 0;

  /**
    Returns an isochronous read channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this
    channel.
  */
  virtual IsochronousReadChannel getReadChannel(
    unsigned int maxPacketsPerRequest,
    uint64 subchannels) = 0;
  
  /**
    Returns an isochronous write channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this
    channel.
  */
  virtual IsochronousWriteChannel getWriteChannel(
    unsigned int maxPacketsPerRequest,
    uint64 subchannels) = 0;

  /**
    Starts an isochronous packet listener.
    
    @param channel The isochronous channel [0; 63].
    @param maximumPayload The maximum payload of a packet.
    @param listener The listener to be notified on incoming packets.
  */
  virtual void readIsochronous(
    unsigned int channel,
    unsigned int maximumPayload,
    IsochronousChannelListener* listener
  ) = 0;

  virtual bool wait(
    unsigned int milliseconds) = 0;
  
  virtual void dequeue() = 0;
  
  virtual void registerFCPListener(
    FunctionControlProtocolListener* listener) = 0;
  
  virtual void unregisterFCPListener() = 0;

  /**
    Destroys the IEEE 1394 implementation.
  */
  virtual ~IEEE1394Impl();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
