/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_H

#include <base/communication/CommunicationsPort.h>
#include <base/communication/IEEE1394Exception.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  IEEE-1394 (also known as FireWire (Apple Computer) and iLINK (Sony Corp.))
  support. See 1394 Trade Association (http://www.1394ta.org).
  
  @short IEEE-1394 (FireWire)
  @ingroup communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IEEE1394 : public CommunicationsPort {
public:

  /** Broadcast id. */
  static const unsigned int BROADCAST = 63;
  
  /**
    Globally Unique Identifier (GUID).

    @ingroup communications
    @short Globally unique identifier.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class GloballyUniqueIdentifier {
  private:
    
    /** Unique identifier. */
    unsigned char id[8];
  public:

    /**
      Initializes identifier as invalid.
    */
    GloballyUniqueIdentifier() throw();

    /**
      Initializes identifier.
    */
    GloballyUniqueIdentifier(const unsigned char value[8]) throw();
    
    /**
      Initializes identifier from other identifier.
    */
    GloballyUniqueIdentifier(const GloballyUniqueIdentifier& copy) throw();
    
    /**
      Assignment of identifier by identifier.
    */
    GloballyUniqueIdentifier& operator=(const GloballyUniqueIdentifier& eq) throw();
    
    /**
      Returns the identifier.
    */
    inline const unsigned char* getBytes() const throw() {
      return reinterpret_cast<const unsigned char*>(&id);
    }
  };



  /**
    Returns the adapters available.
  */
  virtual Array<GloballyUniqueIdentifier> getAdapters() throw(IEEE1394Exception) = 0;

  /**
    Opens a connection to the primary adapter.
  */
  virtual void open() throw(IEEE1394Exception) = 0;
  
  /**
    Opens a connection to the specified adapter.
    
    @param adapter The id of the adapter.
  */
  virtual void open(const GloballyUniqueIdentifier& adapter) throw(IEEE1394Exception) = 0;
  
  /**
    Closes the handle to the adapter. The adapter is destroyed when all handles
    have been closed.
  */
  virtual void close() throw(IEEE1394Exception) = 0;
  
  /**
    Returns the devices on the bus.
  */
  virtual Array<GloballyUniqueIdentifier> getDevices() throw(IEEE1394Exception) = 0;
  
  /**
    Returns the unique identifier of the specified node.

    @param physicalId The physical id of the node [0; 63[.
  */
  virtual GloballyUniqueIdentifier getIdentifier(unsigned int physicalId) throw(OutOfDomain, IEEE1394Exception) = 0;
  
  /**
    Returns the current error status.
  */
  virtual unsigned int getStatus() const throw(IEEE1394Exception) = 0;
  
  /**
    Returns the size of the FIFO.
  */
  virtual unsigned int getFIFOSize() const throw(IEEE1394Exception) = 0;
  
  /**
    Returns true if the specified node is a contended for...

    @param physicalId The id of the node [0; 63[.
  */
  virtual bool isPresent(unsigned int physicalId) const throw(OutOfDomain, IEEE1394Exception) = 0;
  
  /**
    Returns true if the specified node is a contended for...

    @param physicalId The id of the node [0; 63[.
  */
  virtual bool isLinkLayerActivated(unsigned int physicalId) const throw(OutOfDomain, IEEE1394Exception) = 0;

  /**
    Returns true if the specified node is a contended for...

    @param physicalId The id of the node [0; 63[.
  */
  virtual bool isContended(unsigned int physicalId) const throw(OutOfDomain, IEEE1394Exception) = 0;
  
  /**
    Returns the maximum speed of a node.
  */
  virtual unsigned int getSpeedOfNode(unsigned int physicalId) const throw(OutOfDomain, IEEE1394Exception) = 0;
  
  /**
    Returns the maximum speed for communication with node.
  */
  virtual unsigned int getMaximumSpeedToNode(unsigned int physicalId) const throw(OutOfDomain, IEEE1394Exception) = 0;
  
  /**
    Returns the maximum broadcast speed.
  */
  virtual unsigned int getBroadcastSpeed() const throw(IEEE1394Exception) = 0;
  
  /**
    Read data from device.

    @param node The physical id of source node.
    @param offset The offset of the data to read from.
    @param buffer The data buffer.
    @param size The number of bytes to read.
  */
  virtual unsigned int read(unsigned short node, uint64 offset, char* buffer, unsigned int size) throw(OutOfDomain, IOException) = 0;

  /**
    Write data to device.

    @param node The physical id of destination node.
    @param offset The offset of the data to write to.
    @param buffer The data buffer.
    @param size The number of bytes to write.
  */
  virtual unsigned int write(unsigned short node, uint64 offset, const char* buffer, unsigned int size) throw(OutOfDomain, IOException) = 0;



  /** Isochronous request type. */
  enum IsochronousRequestType {
    INVALID_REQUEST, /**< Invalid request. */
    READ_PACKETS_REQUEST, /**< Read packets request. */
    READ_FIXED_PACKETS_REQUEST, /**< Read fixed packets request. */
    READ_FIXED_DATA_REQUEST, /**< Read fixed data request. */
    WRITE_PACKETS_REQUEST, /**< Write packets request. */
    WRITE_FIXED_PACKETS_REQUEST, /**< Write fixed packets request. */
    WRITE_DATA_REQUEST /**< Write data request. */
  };

  /** The status of an isochronous request. */
  enum IsochronousRequestStatus {
    READY, /**< The request has not been queued. */
    PENDING, /**< The request is pending. */
    ERROR, /**< An error occured. */
    TIMEDOUT, /**< The request timed out. */
    COMPLETED /**< Request has been completed without errors. */
  };

  /** Isochronous request options. */
  enum IsochronousRequestOption {
    SWAP_BYTE_ORDER = 1 /**< Activates byte order swapping on quadlet level. */
  };
  
  /**
    Isochronous request descriptor.
    
    @ingroup communications
    @short Isochronous request descriptor.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousRequestImpl : public ReferenceCountedObject {
  private:

    /** Options. */
    unsigned int options;
    /** The current status of the request. */
    IsochronousRequestStatus status;
  protected:

    /** Reset any context information. */
    virtual void resetContext() throw() {
    }

    /** Sets the status. */
    inline void setStatus(IsochronousRequestStatus status) throw() {
      this->status = status;
    }
  public:

    inline IsochronousRequestImpl() throw() : options(0), status(READY) {
    }
    
    inline IsochronousRequestImpl(unsigned int _options) throw()
      : options(_options), status(READY) {
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const throw() {
      return options;
    }
    
    /**
      Sets the options of the request.
    */
    void setOptions(unsigned int options) throw(IEEE1394Exception);
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const throw() {
      return status == PENDING;
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const throw() {
      return status;
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    void reset() throw(IEEE1394Exception);
  };

  /**
    Base class of read requests contexts.
    
    @ingroup communications
    @short Isochronous read request context.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadRequestImpl : public IsochronousRequestImpl {
  private:

    /** The subchannel. */
    unsigned int subchannel;
    /** The buffer. */
    char* buffer;
    /** The size of the buffer in bytes. */
    unsigned int bufferSize;
    /** The number of received packets. */
    unsigned int receivedPackets;
  protected:
    
    /**
      Sets the number of received packets.
    */
    inline void setReceivedPackets(unsigned int receivedPackets) throw() {
      this->receivedPackets = receivedPackets;
    }

    virtual void resetContext() throw() {
      receivedPackets = 0;
    }
  public:    

    IsochronousReadRequestImpl() throw();

    /**
      Returns the channel.
    */
    inline unsigned int getSubchannel() throw() {
      return subchannel;
    }
    
    /**
      Sets the subchannel.
    */
    void setSubchannel(unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception);
    
    /**
      Returns the buffer.
    */
    inline char* getBuffer() const throw() {
      return buffer;
    }
    
    /**
      Returns the size of the buffer in bytes.
    */
    unsigned int getBufferSize() const throw() {
      return bufferSize;
    }

    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
    */
    void setBuffer(char* buffer, unsigned int size) throw(IEEE1394Exception);
    
    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const throw() {
      return receivedPackets;
    }
  };

  /**
    Isochronous read packets request context.
    
    @ingroup communications
    @short Isochronous read packets request context.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadPacketsRequestImpl : public IsochronousReadRequestImpl {
  };

  /**
    Isochronous read fixed packets request context.
    
    @ingroup communications
    @short Isochronous read packets request context.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadFixedPacketsRequestImpl : public IsochronousReadRequestImpl {
  };

  /**
    Isochronous read fixed data request context.
    
    @ingroup communications
    @short Isochronous read fixed data request context.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadFixedDataRequestImpl : public IsochronousReadRequestImpl {
  private:

    /** The secondary buffer for header data. */
    char* secondaryBuffer;
  public:
    
    /**
      Returns the secondary buffer used for header data.
    */
    inline char* getSecondaryBuffer() const throw() {
      return secondaryBuffer;
    }

    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param secondaryBuffer The secondary buffer.
    */
    void setBuffer(char* buffer, unsigned int size, char* secondaryBuffer) throw(IEEE1394Exception);    
  };

  /**
    Base class of write requests.
    
    @ingroup communications
    @short Isochronous write request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWriteRequestImpl : public IsochronousRequestImpl {
  private:

    /** The buffer. */
    const char* buffer;
    /** The size of the buffer in bytes. */
    unsigned int bufferSize;
    /** The number of packets in the buffer. */
    unsigned int numberOfPackets;
    /** The transmission speed. */
    unsigned int speed;
    /** The number of transmitted packets. */
    unsigned int transmittedPackets;
  protected:

    void setTransmittedPackets(unsigned int transmittedPackets) throw() {
      this->transmittedPackets = transmittedPackets;
    }

    virtual void resetContext() throw() {
      transmittedPackets = 0;
    }
  public:

    IsochronousWriteRequestImpl() throw();
    
    /**
      Returns the buffer.
    */
    inline const char* getBuffer() const throw() {
      return buffer;
    }
    
    /**
      Returns the size of the buffer in bytes.
    */
    inline unsigned int getBufferSize() const throw() {
      return bufferSize;
    }

    /**
      Returns the number of packets in the buffer.
    */
    inline unsigned int getNumberOfPackets() const throw() {
      return numberOfPackets;
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param numberOfPackets The number of packets in the buffer.
    */
    void setBuffer(const char* buffer, unsigned int size, unsigned int numberOfPackets) throw(IEEE1394Exception);
    
    /**
      Returns the transmission speed.
    */
    inline unsigned int getSpeed() const throw() {
      return speed;
    }
    
    /**
      Sets the transmissoin speed.
    */
    void setSpeed(unsigned int speed) throw(OutOfDomain, IEEE1394Exception);
    
    /**
      Returns the number of transmitted packets.
    */
    inline unsigned int getTransmittedPackets() const throw() {
      return transmittedPackets;
    }
  };

  /**
    Isochronous write packets request context.
    
    @ingroup communications
    @short Isochronous write packets request context.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWritePacketsRequestImpl : public IsochronousWriteRequestImpl {
  };

  /**
    Isochronous write fixed packets request context.
    
    @ingroup communications
    @short Isochronous write fixed packets request context.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWriteFixedPacketsRequestImpl : public IsochronousWriteRequestImpl {
  };
  
  /**
    Isochronous write data request context.
    
    @ingroup communications
    @short Isochronous write data request context.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWriteDataRequestImpl : public IsochronousWriteRequestImpl {
  private:

    /** The secondary buffer for header data. */
    const char* secondaryBuffer;
  public:
    
    /**
      Returns the secondary buffer used for header data.
    */
    inline const char* getSecondaryBuffer() const throw() {
      return secondaryBuffer;
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param numberOfPackets The number of packets in the buffer.
      @param secondaryBuffer The secondary buffer.
    */
    void setBuffer(const char* buffer, unsigned int size, unsigned int numberOfPackets, const char* secondaryBuffer) throw(IEEE1394Exception);
  };



  /**
    Isochronous read packets request (READ_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous read packets request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadPacketsRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousReadPacketsRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousReadPacketsRequest(IsochronousReadPacketsRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadPacketsRequest(const IsochronousReadPacketsRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadPacketsRequest& operator=(const IsochronousReadPacketsRequest& eq) throw() {
      context = eq.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const throw() {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const throw() {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const throw() {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() throw(IEEE1394Exception) {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const throw() {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const throw() {
      context->getBufferSize();
    }

    /**
      Returns the subchannel.
    */
    inline unsigned int getSubchannel() throw() {
      return context->getSubchannel();
    }

    /**
      Sets the subchannel.
    */
    inline void setSubchannel(unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception) {
      context->setSubchannel(subchannel);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const throw() {
      return context->getReceivedPackets();
    }

    /**
      Returns the buffer.
    */
    inline char* getBuffer() const throw() {
      return context->getBuffer();
    }
  };
  
  /**
    Isochronous read fixed packets request (READ_FIXED_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous read fixed packets request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadFixedPacketsRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousReadFixedPacketsRequestImpl> context;
  public:
    
    /**
      Initializes object.
    */
    inline IsochronousReadFixedPacketsRequest(IsochronousReadFixedPacketsRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadFixedPacketsRequest(const IsochronousReadFixedPacketsRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadFixedPacketsRequest& operator=(const IsochronousReadFixedPacketsRequest& eq) throw() {
      context = eq.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const throw() {
      return context.isValid();
    }

    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const throw() {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const throw() {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() throw(IEEE1394Exception) {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const throw() {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const throw() {
      context->getBufferSize();
    }

    /**
      Returns the subchannel.
    */
    inline unsigned int getSubchannel() throw() {
      return context->getSubchannel();
    }
    
    /**
      Sets the subchannel.
    */
    inline void setSubchannel(unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception) {
      context->setSubchannel(subchannel);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const throw() {
      return context->getReceivedPackets();
    }

    /**
      Returns the buffer.
    */
    inline char* getBuffer() const throw() {
      return context->getBuffer();
    }
  };
  
  /**
    Isochronous read fixed data request (READ_FIXED_DATA_REQUEST).
    
    @ingroup communications
    @short Isochronous read fixed data request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadFixedDataRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousReadFixedDataRequestImpl> context;
  public:
    
    /**
      Initializes object.
    */
    inline IsochronousReadFixedDataRequest(IsochronousReadFixedDataRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadFixedDataRequest(const IsochronousReadFixedDataRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadFixedDataRequest& operator=(const IsochronousReadFixedDataRequest& eq) throw() {
      context = eq.context;
      return *this;
    }
    
    /**
      Returns true if request is valid.
    */
    inline bool isValid() const throw() {
      return context.isValid();
    }

    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const throw() {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const throw() {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() throw(IEEE1394Exception) {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const throw() {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const throw() {
      context->getBufferSize();
    }

    /**
      Returns the subchannel.
    */
    inline unsigned int getSubchannel() throw() {
      return context->getSubchannel();
    }

    /**
      Sets the subchannel.
    */
    inline void setSubchannel(unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception) {
      context->setSubchannel(subchannel);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const throw() {
      return context->getReceivedPackets();
    }

    /**
      Returns the buffer.
    */
    inline char* getBuffer() const throw() {
      return context->getBuffer();
    }
    
    /**
      Returns the secondary buffer used for header data.
    */
    inline char* getSecondaryBuffer() const throw() {
      return context->getSecondaryBuffer();
    }
  };

  /**
    Isochronous read request.
    
    @ingroup communications
    @short Isochronous read request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousReadRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousReadRequest(IsochronousReadRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadRequest(const IsochronousReadRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadRequest& operator=(const IsochronousReadRequest& eq) throw() {
      context = eq.context;
      return *this;
    }
    
    /**
      Returns the context.
    */
    inline ReferenceCountedObjectPointer<IsochronousReadRequestImpl> getContext() const throw() {
      return context;
    }

    /**
      Returns the type of the request.
    */
    inline IsochronousRequestType getType() const throw() {
      if (dynamic_cast<IsochronousReadPacketsRequestImpl*>(context.getValue())) {
        return READ_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousReadFixedPacketsRequestImpl*>(context.getValue())) {
        return READ_FIXED_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousReadFixedDataRequestImpl*>(context.getValue())) {
        return READ_FIXED_DATA_REQUEST;
      } else {
        return INVALID_REQUEST;
      }
    }
    
    /**
      Casts request to an isochronous read packets request (READ_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous packets request.
    */
    IsochronousReadPacketsRequest getIsochronousReadPacketsRequest() const throw() {
      return IsochronousReadPacketsRequest(dynamic_cast<IsochronousReadPacketsRequestImpl*>(context.getValue()));
    }
    
    /**
      Casts request to an isochronous read fixed packets request (READ_FIXED_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous fixed packets request.
    */
    IsochronousReadFixedPacketsRequest getIsochronousReadFixedPacketsRequest() const throw() {
      return IsochronousReadFixedPacketsRequest(dynamic_cast<IsochronousReadFixedPacketsRequestImpl*>(context.getValue()));
    }

    /**
      Casts request to an isochronous read fixed data request (READ_FIXED_DATA_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous fixed data request.
    */
    IsochronousReadFixedDataRequest getIsochronousReadFixedDataRequest() const throw() {
      return IsochronousReadFixedDataRequest(dynamic_cast<IsochronousReadFixedDataRequestImpl*>(context.getValue()));
    }
  };



  /**
    Isochronous write packets request (WRITE_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous write packets request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWritePacketsRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousWritePacketsRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousWritePacketsRequest(IsochronousWritePacketsRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousWritePacketsRequest(const IsochronousWritePacketsRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousWritePacketsRequest& operator=(const IsochronousWritePacketsRequest& eq) throw() {
      context = eq.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const throw() {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const throw() {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const throw() {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() throw(IEEE1394Exception) {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const throw() {
      return context->getOptions();
    }    
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const throw() {
      context->getBufferSize();
    }
    
    /**
      Returns the buffer.
    */
    inline const char* getBuffer() const throw() {
      return context->getBuffer();
    }
    
    /**
      Returns the transmission speed.
    */
    inline unsigned int getSpeed() const throw(OutOfDomain, IEEE1394Exception) {
      return context->getSpeed();
    }

    /**
      Sets the transmission speed.
    */
    inline void setSpeed(unsigned int speed) throw(OutOfDomain, IEEE1394Exception) {
      context->setSpeed(speed);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getTransmittedPackets() const throw() {
      return context->getTransmittedPackets();
    }
  };

  /**
    Isochronous write fixed packets request (WRITE_FIXED_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous write fixed packets request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWriteFixedPacketsRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousWriteFixedPacketsRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousWriteFixedPacketsRequest(IsochronousWriteFixedPacketsRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousWriteFixedPacketsRequest(const IsochronousWriteFixedPacketsRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousWriteFixedPacketsRequest& operator=(const IsochronousWriteFixedPacketsRequest& eq) throw() {
      context = eq.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const throw() {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const throw() {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const throw() {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() throw(IEEE1394Exception) {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const throw() {
      return context->getOptions();
    }    
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const throw() {
      context->getBufferSize();
    }
    
    /**
      Returns the buffer.
    */
    inline const char* getBuffer() const throw() {
      return context->getBuffer();
    }

    /**
      Returns the transmission speed.
    */
    inline unsigned int getSpeed() const throw(OutOfDomain, IEEE1394Exception) {
      return context->getSpeed();
    }

    /**
      Sets the transmission speed.
    */
    inline void setSpeed(unsigned int speed) throw(OutOfDomain, IEEE1394Exception) {
      context->setSpeed(speed);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getTransmittedPackets() const throw() {
      return context->getTransmittedPackets();
    }
  };

  /**
    Isochronous write data request (WRITE_DATA_REQUEST).
    
    @ingroup communications
    @short Isochronous write data request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWriteDataRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousWriteDataRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousWriteDataRequest(IsochronousWriteDataRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousWriteDataRequest(const IsochronousWriteDataRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousWriteDataRequest& operator=(const IsochronousWriteDataRequest& eq) throw() {
      context = eq.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const throw() {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const throw() {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const throw() {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() throw(IEEE1394Exception) {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const throw() {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const throw() {
      context->getBufferSize();
    }
    
    /**
      Returns the buffer.
    */
    inline const char* getBuffer() const throw() {
      return context->getBuffer();
    }
    
    /**
      Returns the transmission speed.
    */
    inline unsigned int getSpeed() const throw(OutOfDomain, IEEE1394Exception) {
      return context->getSpeed();
    }

    /**
      Sets the transmission speed.
    */
    inline void setSpeed(unsigned int speed) throw(OutOfDomain, IEEE1394Exception) {
      context->setSpeed(speed);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getTransmittedPackets() const throw() {
      return context->getTransmittedPackets();
    }
  };

  /**
    Isochronous write request.

    @ingroup communications
    @short Isochronous write request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWriteRequest : public Object {
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousWriteRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousWriteRequest(IsochronousWriteRequestImpl* _context) throw()
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousWriteRequest(const IsochronousWriteRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousWriteRequest& operator=(const IsochronousWriteRequest& eq) throw() {
      context = eq.context;
      return *this;
    }
    
    /**
      Returns the context.
    */
    inline ReferenceCountedObjectPointer<IsochronousWriteRequestImpl> getContext() const throw() {
      return context;
    }

    /**
      Returns the type of the request.
    */
    inline IsochronousRequestType getType() const throw() {
      if (dynamic_cast<IsochronousWritePacketsRequestImpl*>(context.getValue())) {
        return WRITE_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousWriteFixedPacketsRequestImpl*>(context.getValue())) {
        return WRITE_FIXED_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousWriteDataRequestImpl*>(context.getValue())) {
        return WRITE_DATA_REQUEST;
      } else {
        return INVALID_REQUEST;
      }
    }
    
    /**
      Casts request to an isochronous write packets request (WRITE_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous packets request.
    */
    IsochronousWritePacketsRequest getIsochronousWritePacketsRequest() const throw() {
      return IsochronousWritePacketsRequest(dynamic_cast<IsochronousWritePacketsRequestImpl*>(context.getValue()));
    }
    
    /**
      Casts request to an isochronous write fixed packets request (WRITE_FIXED_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous fixed packets request.
    */
    IsochronousWriteFixedPacketsRequest getIsochronousWriteFixedPacketsRequest() const throw() {
      return IsochronousWriteFixedPacketsRequest(dynamic_cast<IsochronousWriteFixedPacketsRequestImpl*>(context.getValue()));
    }

    /**
      Casts request to an isochronous write data request (WRITE_DATA_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous fixed data request.
    */
    IsochronousWriteDataRequest getIsochronousWriteDataRequest() const throw() {
      return IsochronousWriteDataRequest(dynamic_cast<IsochronousWriteDataRequestImpl*>(context.getValue()));
    }
  };
  


  /**
    Isochronous communication channel.
    
    @ingroup communications
    @short Isochronous communication channel.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousChannel : public Object {
  };
  
  /**
    Isochronous read channel.

    @ingroup communications
    @short Isochronous read channel.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadChannel : public IsochronousChannel {
  public:
    
    /**
      Returns the reserved subchannels.
    */
    virtual uint64 getSubchannels() throw(IEEE1394Exception) = 0;

    /**
      Cancels all pending requests.
    */
    virtual void cancel() throw(IEEE1394Exception) = 0;
    
    /**
      Closes the channel.
    */
    virtual void close() throw(IEEE1394Exception) = 0;

    /**
      Returns a read packet request object (READ_PACKETS_REQUEST).
    */    
    virtual IsochronousReadPacketsRequest getReadPacketsRequest() const throw() = 0;
    
    /**
      Returns a read fixed packet request object (READ_FIXED_PACKETS_REQUEST).
    */    
    virtual IsochronousReadFixedPacketsRequest getReadFixedPacketsRequest() const throw() = 0;
    
    /**
      Returns a read fixed data request object (READ_FIXED_DATA_REQUEST).
    */    
    virtual IsochronousReadFixedDataRequest getReadFixedDataRequest() const throw() = 0;

    /**
      Queues the specified read request.
    */
    virtual void queue(IsochronousReadRequest& request) throw(IEEE1394Exception) = 0;
    
    /**
      Queues the specified read requests.
    */
    virtual void queue(Allocator<IsochronousReadRequest>& request) throw(IEEE1394Exception) = 0;

    /**
      Returns the next completed request.

      @return INVALID_REQUEST if no request is available in the completion queue.
    */
    virtual IsochronousReadRequest dequeue() throw(IEEE1394Exception) = 0;
    
    /**
      Wait for an event.

      @param nanoseconds The timeout period [0; 999999999].
      
      @return True if event occured within the specified timeout period and otherwise false.
    */
    virtual bool wait(unsigned int nanoseconds) throw(OutOfDomain) = 0;
  };
  
  /**
    Isochronous write channel.

    @ingroup communications
    @short Isochronous write channel.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWriteChannel : public IsochronousChannel {
  public:

    /**
      Returns the reserved subchannels.
    */
    virtual uint64 getSubchannels() throw(IEEE1394Exception) = 0;

    /**
      Cancels all pending requests.
    */
    virtual void cancel() throw(IEEE1394Exception) = 0;
    
    /**
      Closes the channel.
    */
    virtual void close() throw(IEEE1394Exception) = 0;
    
    /**
      Returns a write packet request object (WRITE_PACKETS_REQUEST).
    */    
    virtual IsochronousWritePacketsRequest getWritePacketsRequest() const throw() = 0;
    
    /**
      Returns a write fixed packet request object (WRITE_FIXED_PACKETS_REQUEST).
    */
    virtual IsochronousWriteFixedPacketsRequest getWriteFixedPacketsRequest() const throw() = 0;
    
    /**
      Returns a write data request object (WRITE_DATA_REQUEST).
    */
    virtual IsochronousWriteDataRequest getWriteDataRequest() const throw() = 0;
    
    /**
      Queues the specified write request.
    */
    virtual void queue(IsochronousWriteRequest& request) throw(IEEE1394Exception) = 0;

    /**
      Queues the specified write requests.
    */
    virtual void queue(Allocator<IsochronousWriteRequest>& request) throw(IEEE1394Exception) = 0;

    /**
      Returns the next completed request.

      @return INVALID_REQUEST if no request is available in the completion queue.
    */
    virtual IsochronousWriteRequest dequeue() throw(IEEE1394Exception) = 0;
    
    /**
      Wait for an event.

      @param nanoseconds The timeout period [0; 999999999].
      
      @return True if event occured within the specified timeout period and otherwise false.
    */
    virtual bool wait(unsigned int nanoseconds) throw(OutOfDomain) = 0;
  };
};

/**
  Writes the globally unique identifier to the format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const IEEE1394::GloballyUniqueIdentifier& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
