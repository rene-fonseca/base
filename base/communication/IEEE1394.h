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
#include <base/communication/EUI64.h>
#include <base/communication/IEEE1394Exception.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/collection/Array.h>
#include <base/ByteOrder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  IEEE-1394 (also known as FireWire (Apple Computer) and i.LINK (Sony Corp.))
  support. See 1394 Trade Association (http://www.1394ta.org).
  
  @short IEEE-1394 (FireWire)
  @ingroup communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IEEE1394 : public CommunicationsPort {
public:

  /** Specifies the maximum number of retries. */
  static const unsigned int MAXIMUM_ATTEMPTS = 5;
  /** The CSR base address. */
  static const uint64 CSR_BASE_ADDRESS = 0xfffff0000000ULL;
  /** Broadcast id. */
  static const unsigned int BROADCAST = 63;
  
  typedef BigEndian<uint32> Quadlet;
  
  enum CSRRegister {
    STATE_CLEAR = 0x000,
    STATE_SET = 0x004,
    NODE_IDS = 0x008,
    RESET_START = 0x00c,
    SPLIT_TIMEOUT_HI = 0x018,
    SPLIT_TIMEOUT_LO = 0x01c,
    ARGUMENT_HI = 0x0020,
    ARGUMENT_LO = 0x0024,
    TEST_START = 0x028,
    TEST_STATUS = 0x02c,
    CYCLE_TIME = 0x200,
    BUS_TIME = 0x204,
    POWER_FAIL_IMMINENT = 0x208,
    POWER_SOURCE = 0x20c,
    BUSY_TIMEOUT = 0x210,
    CONFIGURATION_ROM = 0x400,
    BUS_INFO_BLOCK = 0x404,
    TOPOLOGY_MAP = 0x1000,
    SPEED_MAP = 0x2000
  };
  
  /**
    This exception is raised on bus resets. The reset must be acknowledged
    to bring the IEEE-1394 interface back into the working operating mode.
    
    @short IEEE 1394 bus reset exception.
    @ingroup exceptions communication
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class BusReset {
  };



  /** IEEE 1394 standard. */
  enum Standard {
    STANDARD_UNSPECIFIED,
    STANDARD_IEEE_1394,
    STANDARD_IEEE_1394A,
    STANDARD_IEEE_1394B
  };

  enum Flag {
    ASYNCHRONOUS_TRANSMISSION = 1,
    ISOCHRONOUS_TRANSMISSION = 2
  };

  struct Descriptor {
    unsigned int flags;
    EUI64 guid;
    Standard standard;
  };
protected:

  bool reset;
  EUI64 adapterGuid;
  unsigned int adapterId;
  uint64 presentNodes;
  uint64 linkOnNodes;
  Descriptor node[63];
public:
  
  /**
    Returns true if the bus has been reset.
  */
  virtual bool hasBeenReset() const throw(); // TAG: fixme
  
  /**
    Acknowledges the bus reset.
  */
  virtual void acknowledgeReset() throw(); // TAG: fixme
  
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
    Closes the handle to the adapter. The adapter is destroyed when all handles
    have been closed.
  */
  virtual void close() throw(IEEE1394Exception) = 0;
  
  /**
    Returns the guid's of the available nodes on the bus.
  */
  Array<EUI64> getNodes() throw(IEEE1394Exception);

  /**
    Returns the GUID of the adapter.
  */
  inline EUI64 getGuid() const throw() {
    return adapterGuid;
  }

  /**
    Returns the physical id of the adapter.
  */
  inline unsigned int getPhysicalId() const throw() {
    return adapterId;
  }
  
  /**
    Returns the vendor id of the adapter.
  */
  virtual unsigned int getVendorId() const throw(IEEE1394Exception) = 0;
  
  /**
    Returns the product id of the adapter.
  */
  virtual unsigned int getProductId() const throw(IEEE1394Exception) = 0;

  /**
    Returns the IEEE 1394 standard of the adapter.
  */
  virtual Standard getCompliance() const throw(IEEE1394Exception) = 0;

  /**
    Returns the vendor name of the specified node.
    
    @param node The physical id of the node [0; 63[.
  */
  String getVendorName(unsigned int node) throw(IEEE1394Exception);

  /**
    Returns the model name of the specified node.
    
    @param node The physical id of the node [0; 63[.
  */
  String getModelName(unsigned int node) throw(IEEE1394Exception);
  
  /**
    Returns the unique identifier of the specified node.

    @param node The physical id of the node [0; 63[.
  */
  EUI64 getIdentifier(unsigned int node) throw(OutOfDomain, IEEE1394Exception);

  /**
    Returns the physical id ([0; 63[) of the specified device.
    
    @param guid The unique id of the device.

    @return -1 is not found.
  */
  int getPhysicalId(const EUI64& guid) throw(IEEE1394Exception);
  
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
    Returns true if the specified node is a contended for...

    @param node The physical id of the node [0; 63[.
  */
  inline bool isPresent(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
    assert(node < BROADCAST, OutOfDomain(this));
    return (getPresentNodes() >> node) & 1;
  }

  /**
    Returns the nodes which have the link layer activated.
  */
  virtual uint64 getLinkOnNodes() const throw(IEEE1394Exception) = 0;

  /**
    Returns true if the specified node has the link layer activated.

    @param node The physical id of the node [0; 63[.
  */
  inline bool isLinkLayerActivated(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
    assert(node < BROADCAST, OutOfDomain(this));
    return (getLinkOnNodes() >> node) & 1;  
  }

  /**
    Returns the contender nodes.
  */
  virtual uint64 getContenders() const throw(IEEE1394Exception) = 0;
  
  /**
    Returns true if the specified node is a contended.
    
    @param node The physical id of the node [0; 63[.
  */
  inline bool isContender(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {    
    assert(node < BROADCAST, OutOfDomain(this));
    return (getContenders() >> node) & 1;
  }
  
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

    virtual ~IsochronousRequestImpl() throw();
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
  private:

    /** The maximum payload in bytes per packet. */
    unsigned int payload;
  public:

    /**
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const throw() {
      return payload;
    }
    
    /**
      Sets the maximum payload in bytes per packet.
    */
    void setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception);
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

    /** The desired number of packets to be received. */
    unsigned int numberOfPackets;
    /** The size of the header in bytes per packet. */
    unsigned int headerSize;
    /** The maximum payload in bytes per packet. */
    unsigned int payload;
    /** The secondary buffer for header data. */
    char* secondaryBuffer;
  public:
    
    /**
      Returns the desired number of packets to be received.
    */
    inline unsigned int getNumberOfPackets() const throw() {
      return numberOfPackets;
    }
    
    /**
      Sets the desired number of packets to be received.
    */
    void setNumberOfPackets(unsigned int packets) throw(OutOfDomain, IEEE1394Exception);
    
    /**
      Returns the size of the header in bytes per packet.
    */
    inline unsigned int getHeaderSize() const throw() {
      return headerSize;
    }
    
    /**
      Sets the size of the header in bytes per packet (the size must be an integral number of 32 bit words).
    */
    void setHeaderSize(unsigned int size) throw(OutOfDomain, IEEE1394Exception);
    
    /**
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const throw() {
      return payload;
    }
    
    /**
      Sets the maximum payload in bytes per packet (the payload must be an integral number of 32 bit words).
    */
    void setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception);
    
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



  class IsochronousReadRequest;
  
  /**
    Isochronous read packets request (READ_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous read packets request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousReadPacketsRequest : public Object {
    friend class IEEE1394;
    friend class IsochronousReadRequest;
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
      return context->getBufferSize();
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
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
    */
    inline void setBuffer(char* buffer, unsigned int size) throw(IEEE1394Exception) {
      context->setBuffer(buffer, size);
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
    friend class IEEE1394;
    friend class IsochronousReadRequest;
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
      return context->getBufferSize();
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
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const throw() {
      return context->getPayload();
    }

    /**
      Sets the payload in bytes per packet.
    */
    inline void setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
      context->setPayload(payload);
    }

    /**
      Returns the buffer.
    */
    inline char* getBuffer() const throw() {
      return context->getBuffer();
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
    */
    inline void setBuffer(char* buffer, unsigned int size) throw(IEEE1394Exception) {
      context->setBuffer(buffer, size);
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
    friend class IEEE1394;
    friend class IsochronousReadRequest;
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
      return context->getBufferSize();
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
      Returns the desired number of packets to be received.
    */
    inline unsigned int getNumberOfPackets() const throw() {
      return context->getNumberOfPackets();
    }
    
    /**
      Sets the desired number of packets to be received.
    */
    inline void setNumberOfPackets(unsigned int packets) throw(OutOfDomain, IEEE1394Exception) {
      context->setNumberOfPackets(packets);
    }

    /**
      Returns the size of the header in bytes per packet.
    */
    inline unsigned int getHeaderSize() const throw() {
      return context->getHeaderSize();
    }
    
    /**
      Sets the size of the header in bytes per packet (the size must be an integral number of 32 bit words).
    */
    inline void setHeaderSize(unsigned int size) throw(OutOfDomain, IEEE1394Exception) {
      context->setHeaderSize(size);
    }
    
    /**
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const throw() {
      return context->getPayload();
    }

    /**
      Sets the payload in bytes per packet (the size must be an integral number of 32 bit words).
    */
    inline void setPayload(unsigned int payload) throw(OutOfDomain, IEEE1394Exception) {
      context->setPayload(payload);
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
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param secondaryBuffer The secondary buffer.
    */
    inline void setBuffer(char* buffer, unsigned int size, char* secondaryBuffer) throw(IEEE1394Exception) {
      context->setBuffer(buffer, size, secondaryBuffer);
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
    friend class IEEE1394;
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousReadRequestImpl> context;
  public:

    /**
      Initializes read request.
    */
    inline IsochronousReadRequest(IsochronousReadRequestImpl* _context) throw() // TAG: must be private
      : context(_context) {
    }

    /**
      Initialize read request from read packets request.
    */
    inline IsochronousReadRequest(const IsochronousReadPacketsRequest& request) throw()
      : context(request.context.getValue()) {
    }

    /**
      Initialize read request from read packets request.
    */
    inline IsochronousReadRequest(const IsochronousReadFixedPacketsRequest& request) throw()
      : context(request.context.getValue()) {
    }

    /**
      Initialize read request from read packets request.
    */
    inline IsochronousReadRequest(const IsochronousReadFixedDataRequest& request) throw()
      : context(request.context.getValue()) {
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



  class IsochronousWriteRequest;
  
  /**
    Isochronous write packets request (WRITE_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous write packets request.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class IsochronousWritePacketsRequest : public Object {
    friend class IEEE1394;
    friend class IsochronousWriteRequest;
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
      return context->getBufferSize();
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
    friend class IEEE1394;
    friend class IsochronousWriteRequest;
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
      return context->getBufferSize();
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
    friend class IEEE1394;
    friend class IsochronousWriteRequest;
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousWriteDataRequestImpl> context;
  public:

    /**
      Initializes write request.
    */
    inline IsochronousWriteDataRequest(IsochronousWriteDataRequestImpl* _context) throw() // TAG: must be private
      : context(_context) {
    }

    /**
      Initializes write request from other write request.
    */
    inline IsochronousWriteDataRequest(const IsochronousWriteDataRequest& copy) throw()
      : context(copy.context) {
    }

    /**
      Assignment of write request by write request.
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
      return context->getBufferSize();
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
    friend class IEEE1394;
  private:

    /** Context information. */
    ReferenceCountedObjectPointer<IsochronousWriteRequestImpl> context;
  public:

    /**
      Initializes write request.
    */
    inline IsochronousWriteRequest(IsochronousWriteRequestImpl* _context) throw() // TAG: must be private
      : context(_context) {
    }

    /**
      Initialize write request from write packets request.
    */
    inline IsochronousWriteRequest(const IsochronousWritePacketsRequest& request) throw()
      : context(request.context.getValue()) {
    }

    /**
      Initialize write request from write fixed packets request.
    */
    inline IsochronousWriteRequest(const IsochronousWriteFixedPacketsRequest& request) throw()
      : context(request.context.getValue()) {
    }

    /**
      Initialize write request from write data request.
    */
    inline IsochronousWriteRequest(const IsochronousWriteDataRequest& request) throw()
      : context(request.context.getValue()) {
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
  
  
  
protected:
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousReadPacketsRequestImpl>
  getContext(IsochronousReadPacketsRequest& request) throw() {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousReadFixedPacketsRequestImpl>
  getContext(IsochronousReadFixedPacketsRequest& request) throw() {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousReadFixedDataRequestImpl>
  getContext(IsochronousReadFixedDataRequest& request) throw() {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousReadRequestImpl>
  getContext(IsochronousReadRequest& request) throw() {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousWritePacketsRequestImpl>
  getContext(IsochronousWritePacketsRequest& request) throw() {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousWriteFixedPacketsRequestImpl>
  getContext(IsochronousWriteFixedPacketsRequest& request) throw() {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousWriteDataRequestImpl>
  getContext(IsochronousWriteDataRequest& request) throw() {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline ReferenceCountedObjectPointer<IsochronousWriteRequestImpl>
  getContext(IsochronousWriteRequest& request) throw() {
    return request.context;
  }
public:

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
      Queues the specified read request.
    */
    virtual void queue(IsochronousReadPacketsRequest& request) throw(IEEE1394Exception) = 0;

    /**
      Queues the specified read request.
    */
    virtual void queue(IsochronousReadFixedPacketsRequest& request) throw(IEEE1394Exception) = 0;

    /**
      Queues the specified read request.
    */
    virtual void queue(IsochronousReadFixedDataRequest& request) throw(IEEE1394Exception) = 0;
    
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
      Queues the specified write request.
    */
    virtual void queue(IsochronousWritePacketsRequest& request) throw(IEEE1394Exception) = 0;

    /**
      Queues the specified write request.
    */
    virtual void queue(IsochronousWriteFixedPacketsRequest& request) throw(IEEE1394Exception) = 0;

    /**
      Queues the specified write request.
    */
    virtual void queue(IsochronousWriteDataRequest& request) throw(IEEE1394Exception) = 0;

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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
