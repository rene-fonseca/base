/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_IMPL_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_IMPL_H

#include <base/communication/IEEE1394Common.h>
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  IEEE 1394 implementation.
  
  @short IEEE 1394 implementation.
  @ingroup communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IEEE1394Impl : public ReferenceCountedObject, public IEEE1394Common {
public:

  /**
    Returns the default IEEE 1394 implementation.
  */
  static IEEE1394Impl* getDefault() throw();
  
  /**
    Returns true if the bus has been reset since last check.
  */
  virtual bool hasBeenReset() const throw() = 0;

  /**
    Acknowledges the bus reset.
  */
  virtual void acknowledgeReset() throw() = 0;

  /**
    Resets the bus.
  */
  virtual void resetBus() throw(IEEE1394Exception) = 0;
    
  /**
    Returns the adapters available.
  */
  virtual Array<EUI64> getAdapters() throw(IEEE1394Exception) = 0;
  
  /**
    Opens a connection to the primary adapter.
  */
  virtual void open() throw(IEEE1394Exception) = 0;
  
  /**
    Opens a connection to the specified adapter.
    
    @param adapter The id of the adapter.
  */
  virtual void open(const EUI64& adapter) throw(IEEE1394Exception) = 0;
  
  /**
    Closes this handle to the adapter. The adapter is destroyed when all handles
    have been closed.
  */
  virtual void close() throw(IEEE1394Exception) = 0;

  /**
    Returns the physical id of the adapter.
  */
  virtual unsigned int getLocalId() const throw() = 0;

  /**
    Returns the number of nodes on the local bus.
  */
  virtual unsigned int getNumberOfNodes() const throw() = 0;
  
  /**
    Returns the current error status.
  */
  virtual unsigned int getStatus() const throw(IEEE1394Exception) = 0;

  /**
    Returns the size of the FIFO.
  */
  virtual unsigned int getFIFOSize() const throw(IEEE1394Exception) = 0;
  
  /**
    Read data from device.

    @param node The node id of source node.
    @param address The base address of the memory region to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read (must be an integral number of quadlets).
  */
  virtual void read(unsigned short node, uint64 address, char* buffer, unsigned int size) throw(IEEE1394Exception) = 0;

  /**
    Write data to device.

    @param node The node id of destination node.
    @param address The base address of the memory region to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write (must be an integral number of quadlets).
  */
  virtual void write(unsigned short node, uint64 address, const char* buffer, unsigned int size) throw(IEEE1394Exception) = 0;

  /**
    Read data from device. This method is only used for debugging and development.

    @param node The node id of source node.
    @param address The base address of the memory region to read from (must be a quadlet boundary).
    @param buffer The data buffer.
    @param size The number of quadlets to read (not bytes!).
    @param value The default value to store in the buffer for unsuccessful reads.
    
    @return The number of quadlets read successfully.
  */
  virtual unsigned int read(unsigned short node, uint64 address, uint32* buffer, unsigned int size, uint32 value) throw(IEEE1394Exception) = 0;

  /**
    Returns an isochronous read channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this channel.
  */
  virtual IsochronousReadChannel getReadChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) = 0;
  
  /**
    Returns an isochronous write channel.

    @param maxPacketsPerRequest The maximum number of packets per request.
    @param subchannel Mask specifying the subchannel to reserve for this channel.
  */
  virtual IsochronousWriteChannel getWriteChannel(
    unsigned int maxPacketsPerRequest,
    uint64 subchannels) throw(IEEE1394Exception) = 0;

  /**
    Starts an isochronous packet listener.
    
    @param channel The isochronous channel [0; 63].
    @param maximumPayload The maximum payload of a packet.
    @param listener The listener to be notified on incomming packets.
  */
  virtual void readIsochronous(
    unsigned int channel,
    unsigned int maximumPayload,
    IsochronousChannelListener* listener) throw(OutOfDomain, IEEE1394Exception) = 0;

  virtual bool wait(unsigned int milliseconds) throw(OutOfDomain, IEEE1394Exception) = 0;
  
  virtual void dequeue() throw(IEEE1394Exception) = 0;
  
  virtual void registerFCPListener(FunctionControlProtocolListener* listener) throw(IEEE1394Exception) = 0;
  
  virtual void unregisterFCPListener() throw(IEEE1394Exception) = 0;

  /**
    Destroys the IEEE 1394 implementation.
  */
  virtual ~IEEE1394Impl() throw(IEEE1394Exception);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
