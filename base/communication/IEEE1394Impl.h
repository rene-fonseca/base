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
    Returns true if the bus has been reset since last check.
  */
  virtual bool hasBeenReset() const throw() = 0;

  /**
    Acknowledges the bus reset.
  */
  virtual void acknowledgeReset() throw() = 0;
  
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
    Returns the IEEE 1394 standard of the adapter.
  */
  virtual Standard getCompliance() const throw(IEEE1394Exception) = 0;

  /**
    Returns the current error status.
  */
  virtual unsigned int getStatus() const throw(IEEE1394Exception) = 0;

  /**
    Returns the size of the FIFO.
  */
  virtual unsigned int getFIFOSize() const throw(IEEE1394Exception) = 0;

  /**
    Returns the present nodes on the bus.
  */
  virtual uint64 getPresentNodes() const throw(IEEE1394Exception) = 0;
  
  /**
    Returns the nodes which have the link layer activated.
  */
  virtual uint64 getLinkOnNodes() const throw(IEEE1394Exception) = 0;

  /**
    Returns the contender nodes.
  */
  virtual uint64 getContenders() const throw(IEEE1394Exception) = 0;
  
  /**
    Returns the maximum speed of a node.
    
    @param node The physical id the the node.
  */
  virtual unsigned int getSpeedOfNode(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) = 0;
  
  /**
    Returns the maximum speed for communication with node.
    
    @param node The physical id the the node.
  */
  virtual unsigned int getMaximumSpeedToNode(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) = 0;
  
  /**
    Returns the maximum broadcast speed.
  */
  virtual unsigned int getBroadcastSpeed() const throw(IEEE1394Exception) = 0;
  
  /**
    Read data from device.

    @param node The physical id of source node.
    @param address The base address of the memory region to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read.
  */
  virtual unsigned int read(unsigned short node, uint64 address, char* buffer, unsigned int size) const throw(OutOfDomain, IOException) = 0;

  /**
    Write data to device.

    @param node The physical id of destination node.
    @param address The base address of the memory region to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write.
  */
  virtual unsigned int write(unsigned short node, uint64 address, const char* buffer, unsigned int size) throw(OutOfDomain, IOException) = 0;

  /**
    Read data from device. This method is only used for debugging and development.

    @param node The physical id of source node.
    @param address The base address of the memory region to read from (must be a quadlet boundary).
    @param buffer The data buffer.
    @param size The number of quadlets to read (not bytes!).
    @param value The default value to store in the buffer for unsuccessful reads.
    
    @return The number of quadlets read successfully.
  */
  virtual unsigned int read(unsigned short node, uint64 address, uint32* buffer, unsigned int size, uint32 value) const throw(OutOfDomain, IEEE1394Exception) = 0;

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
  virtual IsochronousWriteChannel getWriteChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
