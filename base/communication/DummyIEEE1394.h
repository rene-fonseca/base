/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__DUMMY_IEEE_1394_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__DUMMY_IEEE_1394_H

#include <base/communication/IEEE1394Impl.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Dummy IEEE 1394 implementation.
  
  @short Dummy IEEE 1394 implementation.
  @ingroup communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DummyIEEE1394 : public IEEE1394Impl {
public:

  /**
    Initializes IEEE 1394 implementation.
  */
  DummyIEEE1394() throw(IEEE1394Exception);
  
  /**
    Returns true if the bus has been reset since last check.
  */
  bool hasBeenReset() const throw();

  /**
    Acknowledges the bus reset.
  */
  void acknowledgeReset() throw();

  /**
    Resets the bus.
  */
  void resetBus() throw(IEEE1394Exception);

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
  
  void readIsochronous(unsigned int channel, unsigned int maximumPayload, IsochronousChannelListener* listener) throw(OutOfDomain, IEEE1394Exception);

  bool wait(unsigned int milliseconds) throw(OutOfDomain, IEEE1394Exception);
  
  void dequeue() throw(IEEE1394Exception);
  
  void registerFCPListener(FunctionControlProtocolListener* listener) throw(IEEE1394Exception);
  
  void unregisterFCPListener() throw(IEEE1394Exception);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
