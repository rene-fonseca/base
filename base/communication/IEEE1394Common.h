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

#include <base/communication/EUI64.h>
#include <base/communication/IEEE1394Exception.h>
#include <base/OutOfDomain.h>
#include <base/mem/Reference.h>
#include <base/ByteOrder.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Common IEEE 1394 types.
  
  @short Common IEEE 1394 types.
  @ingroup communications
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IEEE1394Common {
public:
  
  /** IEEE 1394 standard. */
  enum Standard {
    STANDARD_UNSPECIFIED,
    STANDARD_IEEE_1394,
    STANDARD_IEEE_1394A,
    STANDARD_IEEE_1394B
  };

  /** Isochronous request type. */
  enum IsochronousRequestType {
    NOT_A_REQUEST, /**< Invalid request. */
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

  /** IEEE 1394 adapter status. */
  enum Status {
    STATUS_OK, /**< No error. */
    STATUS_INCOMPATIBLE, /**< Implementation is incompatible with background API or driver. */
    STATUS_INTERNAL, /**< Failure due to an internal error in the implementation. */
    STATUS_CONFLICT, /**< Indicates that a resource is already in use. */
    STATUS_BUSY, /**< Indicates that the device is currently busy. */
    STATUS_ABORTED, /**< Request was aborted. */
    STATUS_TIMEOUT, /**< Request timed out. */
    STATUS_ERROR /**< An unspecified error. */
  };

  /** The transfer speed. */
  enum Speed {
    S100, /**< The base rate. 100Mbps (98.304Mbps). */
    S200, /**< 200Mbps (196.608Mbps). */
    S400, /**< 400Mbps (393.216Mbps). */
    S800, /**< 800Mbps (786.432Mbps). */
    S1600, /**< 1600Mbps (1572.864Mbps). */
    S3200 /**< 3200Mbps (3145.728Mbps). */
  };

  /** Acknowledge code. */
  enum AcknowledgeCode {
    ACK_RESERVED_0,
    ACK_COMPLETE,
    ACK_PENDING,
    ACK_RESERVED_3,
    ACK_BUSY_X,
    ACK_BUSY_A,
    ACK_BUSY_B,
    ACK_RESERVED_7,
    ACK_RESERVED_8,
    ACK_RESERVED_9,
    ACK_RESERVED_10,
    ACK_RESERVED_11,
    ACK_RESERVED_12,
    ACK_DATA_ERROR,
    ACK_TYPE_ERROR,
    ACK_RESERVED_15
  };
  
  /** Response code. */
  enum ResponseCode {
    RCODE_COMPLETE, /**< The command has been completed. */
    RCODE_RESERVED_1,
    RCODE_RESERVED_2,
    RCODE_RESERVED_3,
    RCODE_CONFLICT_ERROR, /**< Resource conflict. */
    RCODE_DATA_ERROR, /**< Hardware error. */
    RCODE_TYPE_ERROR, /**< Unsupported or invalid transaction. */
    RCODE_ADDRESS_ERROR, /**< Invalid address. */
    RCODE_RESERVED_8,
    RCODE_RESERVED_9,
    RCODE_RESERVED_10,
    RCODE_RESERVED_11,
    RCODE_RESERVED_12,
    RCODE_RESERVED_13,
    RCODE_RESERVED_14,
    RCODE_RESERVED_15
  };

  /** Transaction code. */
  enum TransactionCode {
    TCODE_WRITE_REQUEST_FOR_DATA_QUADLET,
    TCODE_WRITE_REQUEST_FOR_DATA_BLOCK,
    TCODE_WRITE_RESPONSE,
    TCODE_RESERVED_3,
    TCODE_READ_REQUEST_FOR_DATA_QUADLET,
    TCODE_READ_REQUEST_FOR_DATA_BLOCK,
    TCODE_READ_RESPONSE_FOR_DATA_QUADLET,
    TOCDE_READ_RESPONSE_FOR_DATA_BLOCK,
    TCODE_CYCLE_START,
    TCODE_LOCK_REQUEST,
    TCODE_ISOCHRONOUS_DATA_BLOCK,
    TCODE_LOCK_RESPONSE,
    TCODE_RESERVED_12,
    TCODE_RESERVED_13,
    TCODE_RESERVED_14,
    TCODE_RESERVED_15
  };
  
  /** Atomic access instruction. */
  enum LockInstruction {
    MASK_SWAP, /**< new = data | (old & ~argument). */
    COMPARE_SWAP, /**< if (old == argument) new = data. */
    FETCH_ADD, /**< new = old + data (big endian). */
    LITTLE_FETCH_ADD, /**< new = old + data (little endian). */
    BOUNDED_ADD, /**< if (old != argument) new = old + data. */
    WRAP_ADD /**< new = (old != argument) ? old + data : data. */
  };

  /** Isochronous request options. */
  enum IsochronousRequestOption {
    SWAP_BYTE_ORDER = 1 /**< Activates byte order swapping on quadlet level. */
  };

  /** The base size of a register. */
  typedef BigEndian<uint32> Quadlet;
  
  /** Broadcast id. */
  static const unsigned int BROADCAST = 63;
  /** The id of the local bus. */
  static const unsigned int LOCAL_BUS = 1023;
  /** The number of isochronous channels. */
  static const unsigned int ISOCHRONOUS_CHANNELS = 64;
  /** Specifies the maximum number of retries. */
  static const unsigned int MAXIMUM_ATTEMPTS = 5;

  /**
    Returns node id for the specified physical id and bus id.
    
    @param physicalId The physical id [0; 63].
    @param busId The bus id. The default is the local bus (i.e. 0x3ff).
  */
  static inline unsigned short makeNodeId(unsigned int physicalId, unsigned int busId = 0x3ff) noexcept {
    return ((busId & 0x3ff) << 6) | (physicalId & 0x3f);
  }

  /**
    Returns the bus id of the specified node id.
  */
  static inline unsigned int getBusId(unsigned short node) noexcept {
    return node >> 6;
  }

  /**
    Returns true if the bus id of the node id is the local bus.
  */
  static bool isLocalBus(unsigned short node) noexcept {
    return getBusId(node) == LOCAL_BUS;
  }

  /**
    Returns the physical id of the specified node id.
  */
  static inline unsigned int getPhysicalId(unsigned short node) noexcept {
    return node & 0x3f;
  }
  
  /**
    Returns true if the physical id of the node id is the broadcast id.
  */
  static bool isBroadcast(unsigned short node) noexcept {
    return getPhysicalId(node) == BROADCAST;
  }
  
  /**
    Isochronous request descriptor.
    
    @ingroup communications
    @short Isochronous request descriptor.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousRequestImpl : public ReferenceCountedObject {
  private:

    /** Options. */
    unsigned int options = 0;
    /** The current status of the request. */
    IsochronousRequestStatus status = READY;
  protected:

    /** Reset any context information. */
    virtual void resetContext() noexcept {
    }

    /** Sets the status. */
    inline void setStatus(IsochronousRequestStatus status) noexcept {
      this->status = status;
    }
  public:

    inline IsochronousRequestImpl() noexcept {
    }
    
    inline IsochronousRequestImpl(unsigned int _options) noexcept
      : options(_options) {
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const noexcept {
      return options;
    }
    
    /**
      Sets the options of the request.
    */
    void setOptions(unsigned int options);
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept {
      return status == PENDING;
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept {
      return status;
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    void reset();

    virtual ~IsochronousRequestImpl() noexcept;
  };

  /**
    Base class of read requests contexts.
    
    @ingroup communications
    @short Isochronous read request context.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadRequestImpl : public IsochronousRequestImpl {
  private:

    /** The subchannel. */
    unsigned int subchannel = 0;
    /** The buffer. */
    uint8* buffer = nullptr;
    /** The size of the buffer in bytes. */
    unsigned int bufferSize = 0;
    /** The number of received packets. */
    unsigned int receivedPackets = 0;
  protected:
    
    /**
      Sets the number of received packets.
    */
    inline void setReceivedPackets(unsigned int receivedPackets) noexcept {
      this->receivedPackets = receivedPackets;
    }

    virtual void resetContext() noexcept {
      receivedPackets = 0;
    }
  public:

    IsochronousReadRequestImpl() noexcept;

    /**
      Returns the channel.
    */
    inline unsigned int getSubchannel() noexcept {
      return subchannel;
    }
    
    /**
      Sets the subchannel.
    */
    void setSubchannel(
      unsigned int subchannel);
    
    /**
      Returns the buffer.
    */
    inline uint8* getBuffer() const noexcept {
      return buffer;
    }
    
    /**
      Returns the size of the buffer in bytes.
    */
    unsigned int getBufferSize() const noexcept {
      return bufferSize;
    }

    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
    */
    void setBuffer(uint8* buffer, unsigned int size);
    
    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const noexcept {
      return receivedPackets;
    }
  };

  /**
    Isochronous read packets request context.
    
    @ingroup communications
    @short Isochronous read packets request context.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadPacketsRequestImpl : public IsochronousReadRequestImpl {
  };

  /**
    Isochronous read fixed packets request context.
    
    @ingroup communications
    @short Isochronous read packets request context.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadFixedPacketsRequestImpl : public IsochronousReadRequestImpl {
  private:

    /** The maximum payload in bytes per packet. */
    unsigned int payload = 0;
  public:

    /**
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const noexcept {
      return payload;
    }
    
    /**
      Sets the maximum payload in bytes per packet.
    */
    void setPayload(
      unsigned int payload);
  };

  /**
    Isochronous read fixed data request context.
    
    @ingroup communications
    @short Isochronous read fixed data request context.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadFixedDataRequestImpl : public IsochronousReadRequestImpl {
  private:

    /** The desired number of packets to be received. */
    unsigned int numberOfPackets = 0;
    /** The size of the header in bytes per packet. */
    unsigned int headerSize = 0;
    /** The maximum payload in bytes per packet. */
    unsigned int payload = 0;
    /** The secondary buffer for header data. */
    uint8* secondaryBuffer = nullptr;
  public:
    
    /**
      Returns the desired number of packets to be received.
    */
    inline unsigned int getNumberOfPackets() const noexcept {
      return numberOfPackets;
    }
    
    /**
      Sets the desired number of packets to be received.
    */
    void setNumberOfPackets(
      unsigned int packets);
    
    /**
      Returns the size of the header in bytes per packet.
    */
    inline unsigned int getHeaderSize() const noexcept {
      return headerSize;
    }
    
    /**
      Sets the size of the header in bytes per packet (the size must be an
      integral number of 32 bit words).
    */
    void setHeaderSize(
      unsigned int size);
    
    /**
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const noexcept {
      return payload;
    }
    
    /**
      Sets the maximum payload in bytes per packet (the payload must be an
      integral number of 32 bit words).
    */
    void setPayload(
      unsigned int payload);
    
    /**
      Returns the secondary buffer used for header data.
    */
    inline uint8* getSecondaryBuffer() const noexcept {
      return secondaryBuffer;
    }

    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param secondaryBuffer The secondary buffer.
    */
    void setBuffer(
      uint8* buffer,
      unsigned int size,
      uint8* secondaryBuffer);
  };

  /**
    Base class of write requests.
    
    @ingroup communications
    @short Isochronous write request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWriteRequestImpl : public IsochronousRequestImpl {
  private:

    /** The buffer. */
    const uint8* buffer = nullptr;
    /** The size of the buffer in bytes. */
    unsigned int bufferSize = 0;
    /** The number of packets in the buffer. */
    unsigned int numberOfPackets = 0;
    /** The transmission speed. */
    unsigned int speed = 0;
    /** The number of transmitted packets. */
    unsigned int transmittedPackets = 0;
  protected:

    void setTransmittedPackets(unsigned int transmittedPackets) noexcept {
      this->transmittedPackets = transmittedPackets;
    }

    virtual void resetContext() noexcept {
      transmittedPackets = 0;
    }
  public:

    IsochronousWriteRequestImpl() noexcept;
    
    /**
      Returns the buffer.
    */
    inline const uint8* getBuffer() const noexcept {
      return buffer;
    }
    
    /**
      Returns the size of the buffer in bytes.
    */
    inline unsigned int getBufferSize() const noexcept {
      return bufferSize;
    }

    /**
      Returns the number of packets in the buffer.
    */
    inline unsigned int getNumberOfPackets() const noexcept {
      return numberOfPackets;
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param numberOfPackets The number of packets in the buffer.
    */
    void setBuffer(
      const uint8* buffer,
      unsigned int size,
      unsigned int numberOfPackets);
    
    /**
      Returns the transmission speed.
    */
    inline unsigned int getSpeed() const noexcept {
      return speed;
    }
    
    /**
      Sets the transmissoin speed.
    */
    void setSpeed(unsigned int speed);
    
    /**
      Returns the number of transmitted packets.
    */
    inline unsigned int getTransmittedPackets() const noexcept {
      return transmittedPackets;
    }
  };

  /**
    Isochronous write packets request context.
    
    @ingroup communications
    @short Isochronous write packets request context.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWritePacketsRequestImpl : public IsochronousWriteRequestImpl {
  };

  /**
    Isochronous write fixed packets request context.
    
    @ingroup communications
    @short Isochronous write fixed packets request context.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWriteFixedPacketsRequestImpl : public IsochronousWriteRequestImpl {
  };
  
  /**
    Isochronous write data request context.
    
    @ingroup communications
    @short Isochronous write data request context.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWriteDataRequestImpl : public IsochronousWriteRequestImpl {
  private:

    /** The secondary buffer for header data. */
    const uint8* secondaryBuffer = nullptr;
  public:
    
    /**
      Returns the secondary buffer used for header data.
    */
    inline const uint8* getSecondaryBuffer() const noexcept {
      return secondaryBuffer;
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param numberOfPackets The number of packets in the buffer.
      @param secondaryBuffer The secondary buffer.
    */
    void setBuffer(
      const uint8* buffer,
      unsigned int size,
      unsigned int numberOfPackets,
      const uint8* secondaryBuffer);
  };



  class IsochronousReadRequest;
  
  /**
    Isochronous read packets request (READ_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous read packets request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadPacketsRequest : public Object {
    friend class IEEE1394Common;
    friend class IsochronousReadRequest;
  private:

    /** Context information. */
    Reference<IsochronousReadPacketsRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousReadPacketsRequest(
      IsochronousReadPacketsRequestImpl* _context) noexcept
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadPacketsRequest(
      const IsochronousReadPacketsRequest& copy) noexcept
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadPacketsRequest& operator=(
      const IsochronousReadPacketsRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const noexcept {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const noexcept {
      return context->getBufferSize();
    }

    /**
      Returns the subchannel.
    */
    inline unsigned int getSubchannel() noexcept {
      return context->getSubchannel();
    }

    /**
      Sets the subchannel.
    */
    inline void setSubchannel(
      unsigned int subchannel) {
      context->setSubchannel(subchannel);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const noexcept {
      return context->getReceivedPackets();
    }

    /**
      Returns the buffer.
    */
    inline uint8* getBuffer() const noexcept {
      return context->getBuffer();
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
    */
    inline void setBuffer(
      uint8* buffer, unsigned int size) {
      context->setBuffer(buffer, size);
    }
  };
  
  /**
    Isochronous read fixed packets request (READ_FIXED_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous read fixed packets request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadFixedPacketsRequest : public Object {
    friend class IEEE1394Common;
    friend class IsochronousReadRequest;
  private:

    /** Context information. */
    Reference<IsochronousReadFixedPacketsRequestImpl> context;
  public:
    
    /**
      Initializes object.
    */
    inline IsochronousReadFixedPacketsRequest(
      IsochronousReadFixedPacketsRequestImpl* _context) noexcept
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadFixedPacketsRequest(
      const IsochronousReadFixedPacketsRequest& copy) noexcept
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadFixedPacketsRequest& operator=(
      const IsochronousReadFixedPacketsRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept
    {
      return context.isValid();
    }

    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept
    {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept
    {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset()
    {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const noexcept
    {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const noexcept {
      return context->getBufferSize();
    }

    /**
      Returns the subchannel.
    */
    inline unsigned int getSubchannel() noexcept {
      return context->getSubchannel();
    }
    
    /**
      Sets the subchannel.
    */
    inline void setSubchannel(
      unsigned int subchannel) {
      context->setSubchannel(subchannel);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const noexcept {
      return context->getReceivedPackets();
    }

    /**
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const noexcept {
      return context->getPayload();
    }

    /**
      Sets the payload in bytes per packet.
    */
    inline void setPayload(
      unsigned int payload) {
      context->setPayload(payload);
    }

    /**
      Returns the buffer.
    */
    inline uint8* getBuffer() const noexcept {
      return context->getBuffer();
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
    */
    inline void setBuffer(
      uint8* buffer, unsigned int size) {
      context->setBuffer(buffer, size);
    }
  };

  /**
    Isochronous read fixed data request (READ_FIXED_DATA_REQUEST).
    
    @ingroup communications
    @short Isochronous read fixed data request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadFixedDataRequest : public Object {
    friend class IEEE1394Common;
    friend class IsochronousReadRequest;
  private:

    /** Context information. */
    Reference<IsochronousReadFixedDataRequestImpl> context;
  public:

    /**
      Initializes invalid request.
    */
    inline IsochronousReadFixedDataRequest() noexcept {
    }
    
    /**
      Initializes object.
    */
    inline IsochronousReadFixedDataRequest(
      IsochronousReadFixedDataRequestImpl* _context) noexcept
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadFixedDataRequest(
      const IsochronousReadFixedDataRequest& copy) noexcept
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadFixedDataRequest& operator=(const IsochronousReadFixedDataRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }
    
    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept {
      return context.isValid();
    }

    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const noexcept {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const noexcept {
      return context->getBufferSize();
    }

    /**
      Returns the subchannel.
    */
    inline unsigned int getSubchannel() noexcept {
      return context->getSubchannel();
    }

    /**
      Sets the subchannel.
    */
    inline void setSubchannel(
      unsigned int subchannel) {
      context->setSubchannel(subchannel);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getReceivedPackets() const noexcept {
      return context->getReceivedPackets();
    }

    /**
      Returns the desired number of packets to be received.
    */
    inline unsigned int getNumberOfPackets() const noexcept {
      return context->getNumberOfPackets();
    }
    
    /**
      Sets the desired number of packets to be received.
    */
    inline void setNumberOfPackets(
      unsigned int packets) {
      context->setNumberOfPackets(packets);
    }

    /**
      Returns the size of the header in bytes per packet.
    */
    inline unsigned int getHeaderSize() const noexcept {
      return context->getHeaderSize();
    }
    
    /**
      Sets the size of the header in bytes per packet (the size must be an
      integral number of 32 bit words).
    */
    inline void setHeaderSize(
      unsigned int size) {
      context->setHeaderSize(size);
    }
    
    /**
      Returns the maximum payload in bytes per packet.
    */
    inline unsigned int getPayload() const noexcept {
      return context->getPayload();
    }

    /**
      Sets the payload in bytes per packet (the size must be an integral number
      of 32 bit words).
    */
    inline void setPayload(
      unsigned int payload) {
      context->setPayload(payload);
    }
    
    /**
      Returns the buffer.
    */
    inline uint8* getBuffer() const noexcept {
      return context->getBuffer();
    }
    
    /**
      Returns the secondary buffer used for header data.
    */
    inline uint8* getSecondaryBuffer() const noexcept {
      return context->getSecondaryBuffer();
    }
    
    /**
      Sets the buffer.

      @param buffer The location of the buffer.
      @param size The size of the buffer in bytes.
      @param secondaryBuffer The secondary buffer.
    */
    inline void setBuffer(
      uint8* buffer,
      unsigned int size,
      uint8* secondaryBuffer) {
      context->setBuffer(buffer, size, secondaryBuffer);
    }
  };

  /**
    Isochronous read request.
    
    @ingroup communications
    @short Isochronous read request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadRequest : public Object {
    friend class IEEE1394Common;
  private:

    /** Context information. */
    Reference<IsochronousReadRequestImpl> context;
  public:

    /**
      Initializes read request.
    */
    inline IsochronousReadRequest(IsochronousReadRequestImpl* _context) noexcept // TAG: must be private
      : context(_context) {
    }

    /**
      Initialize read request from read packets request.
    */
    inline IsochronousReadRequest(
      const IsochronousReadPacketsRequest& request) noexcept
      : context(request.context.getValue()) {
    }

    /**
      Initialize read request from read packets request.
    */
    inline IsochronousReadRequest(
      const IsochronousReadFixedPacketsRequest& request) noexcept
      : context(request.context.getValue()) {
    }

    /**
      Initialize read request from read packets request.
    */
    inline IsochronousReadRequest(
      const IsochronousReadFixedDataRequest& request) noexcept
      : context(request.context.getValue()) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousReadRequest(const IsochronousReadRequest& copy) noexcept
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousReadRequest& operator=(const IsochronousReadRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }
    
    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() {
      context->reset();
    }

    /**
      Returns the type of the request.
    */
    inline IsochronousRequestType getType() const noexcept {
      if (dynamic_cast<IsochronousReadPacketsRequestImpl*>(context.getValue())) {
        return READ_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousReadFixedPacketsRequestImpl*>(context.getValue())) {
        return READ_FIXED_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousReadFixedDataRequestImpl*>(context.getValue())) {
        return READ_FIXED_DATA_REQUEST;
      } else {
        return NOT_A_REQUEST;
      }
    }
    
    /**
      Casts request to an isochronous read packets request
      (READ_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous
      packets request.
    */
    IsochronousReadPacketsRequest
      getIsochronousReadPacketsRequest() const noexcept {
      return IsochronousReadPacketsRequest(
        dynamic_cast<IsochronousReadPacketsRequestImpl*>(context.getValue()
        )
      );
    }
    
    /**
      Casts request to an isochronous read fixed packets request
      (READ_FIXED_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous
      fixed packets request.
    */
    IsochronousReadFixedPacketsRequest
      getIsochronousReadFixedPacketsRequest() const noexcept {
      return IsochronousReadFixedPacketsRequest(
        dynamic_cast<IsochronousReadFixedPacketsRequestImpl*>(
          context.getValue()
        )
      );
    }

    /**
      Casts request to an isochronous read fixed data request
      (READ_FIXED_DATA_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous
      fixed data request.
    */
    IsochronousReadFixedDataRequest
      getIsochronousReadFixedDataRequest() const noexcept {
      return IsochronousReadFixedDataRequest(
        dynamic_cast<IsochronousReadFixedDataRequestImpl*>(context.getValue())
      );
    }
  };



  class IsochronousWriteRequest;
  
  /**
    Isochronous write packets request (WRITE_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous write packets request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWritePacketsRequest : public Object {
    friend class IEEE1394Common;
    friend class IsochronousWriteRequest;
  private:

    /** Context information. */
    Reference<IsochronousWritePacketsRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousWritePacketsRequest(
      IsochronousWritePacketsRequestImpl* _context) noexcept
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousWritePacketsRequest(
      const IsochronousWritePacketsRequest& copy) noexcept
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousWritePacketsRequest& operator=(const IsochronousWritePacketsRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const noexcept {
      return context->getOptions();
    }    
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const noexcept {
      return context->getBufferSize();
    }
    
    /**
      Returns the buffer.
    */
    inline const uint8* getBuffer() const noexcept {
      return context->getBuffer();
    }
    
    /**
      Returns the transmission speed.
    */
    inline unsigned int
      getSpeed() const {
      return context->getSpeed();
    }

    /**
      Sets the transmission speed.
    */
    inline void setSpeed(
      unsigned int speed) {
      context->setSpeed(speed);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getTransmittedPackets() const noexcept {
      return context->getTransmittedPackets();
    }
  };

  /**
    Isochronous write fixed packets request (WRITE_FIXED_PACKETS_REQUEST).
    
    @ingroup communications
    @short Isochronous write fixed packets request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWriteFixedPacketsRequest : public Object {
    friend class IEEE1394Common;
    friend class IsochronousWriteRequest;
  private:

    /** Context information. */
    Reference<IsochronousWriteFixedPacketsRequestImpl> context;
  public:

    /**
      Initializes object.
    */
    inline IsochronousWriteFixedPacketsRequest(
      IsochronousWriteFixedPacketsRequestImpl* _context) noexcept
      : context(_context) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousWriteFixedPacketsRequest(
      const IsochronousWriteFixedPacketsRequest& copy) noexcept
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousWriteFixedPacketsRequest& operator=(const IsochronousWriteFixedPacketsRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const noexcept {
      return context->getOptions();
    }    
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const noexcept {
      return context->getBufferSize();
    }
    
    /**
      Returns the buffer.
    */
    inline const uint8* getBuffer() const noexcept {
      return context->getBuffer();
    }

    /**
      Returns the transmission speed.
    */
    inline unsigned int
      getSpeed() const {
      return context->getSpeed();
    }

    /**
      Sets the transmission speed.
    */
    inline void setSpeed(
      unsigned int speed) {
      context->setSpeed(speed);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getTransmittedPackets() const noexcept {
      return context->getTransmittedPackets();
    }
  };

  /**
    Isochronous write data request (WRITE_DATA_REQUEST).
    
    @ingroup communications
    @short Isochronous write data request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWriteDataRequest : public Object {
    friend class IEEE1394Common;
    friend class IsochronousWriteRequest;
  private:

    /** Context information. */
    Reference<IsochronousWriteDataRequestImpl> context;
  public:

    /**
      Initializes write request.
    */
    inline IsochronousWriteDataRequest(
      IsochronousWriteDataRequestImpl* _context) noexcept // TAG: must be private
      : context(_context)
    {
    }

    /**
      Initializes write request from other write request.
    */
    inline IsochronousWriteDataRequest(
      const IsochronousWriteDataRequest& copy) noexcept
      : context(copy.context)
    {
    }

    /**
      Assignment of write request by write request.
    */
    inline IsochronousWriteDataRequest& operator=(const IsochronousWriteDataRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }

    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept
    {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept
    {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept
    {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() {
      context->reset();
    }
    
    /**
      Returns the options of the request.
    */
    inline unsigned int getOptions() const noexcept {
      return context->getOptions();
    }
    
    /**
      Returns the size of the buffer.
    */
    inline unsigned int getBufferSize() const noexcept {
      return context->getBufferSize();
    }
    
    /**
      Returns the buffer.
    */
    inline const uint8* getBuffer() const noexcept {
      return context->getBuffer();
    }
    
    /**
      Returns the transmission speed.
    */
    inline unsigned int
      getSpeed() const {
      return context->getSpeed();
    }

    /**
      Sets the transmission speed.
    */
    inline void setSpeed(
      unsigned int speed) {
      context->setSpeed(speed);
    }

    /**
      Returns the number of received packets.
    */
    inline unsigned int getTransmittedPackets() const noexcept {
      return context->getTransmittedPackets();
    }
  };

  /**
    Isochronous write request.

    @ingroup communications
    @short Isochronous write request.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWriteRequest : public Object {
    friend class IEEE1394Common;
  private:

    /** Context information. */
    Reference<IsochronousWriteRequestImpl> context;
  public:

    /**
      Initializes write request.
    */
    inline IsochronousWriteRequest(
      IsochronousWriteRequestImpl* _context) noexcept // TAG: must be private
      : context(_context) {
    }

    /**
      Initialize write request from write packets request.
    */
    inline IsochronousWriteRequest(
      const IsochronousWritePacketsRequest& request) noexcept
      : context(request.context.getValue()) {
    }

    /**
      Initialize write request from write fixed packets request.
    */
    inline IsochronousWriteRequest(
      const IsochronousWriteFixedPacketsRequest& request) noexcept
      : context(request.context.getValue()) {
    }

    /**
      Initialize write request from write data request.
    */
    inline IsochronousWriteRequest(
      const IsochronousWriteDataRequest& request) noexcept
      : context(request.context.getValue()) {
    }

    /**
      Initializes object from other object.
    */
    inline IsochronousWriteRequest(const IsochronousWriteRequest& copy) noexcept
      : context(copy.context) {
    }

    /**
      Assignment of object by object.
    */
    inline IsochronousWriteRequest& operator=(const IsochronousWriteRequest& assign) noexcept
    {
      context = assign.context;
      return *this;
    }
    
    /**
      Returns true if request is valid.
    */
    inline bool isValid() const noexcept {
      return context.isValid();
    }
    
    /**
      Returns true if the request is pending for completion.
    */
    inline bool isPending() const noexcept {
      return context->isPending();
    }
    
    /**
      Returns the current status of the request.
    */
    inline IsochronousRequestStatus getStatus() const noexcept {
      return context->getStatus();
    }
    
    /**
      Resets the request. Raises IEEE1394Exception is request is pending.
    */
    inline void reset() {
      context->reset();
    }

    /**
      Returns the type of the request.
    */
    inline IsochronousRequestType getType() const noexcept {
      if (dynamic_cast<IsochronousWritePacketsRequestImpl*>(context.getValue())) {
        return WRITE_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousWriteFixedPacketsRequestImpl*>(context.getValue())) {
        return WRITE_FIXED_PACKETS_REQUEST;
      } else if (dynamic_cast<IsochronousWriteDataRequestImpl*>(context.getValue())) {
        return WRITE_DATA_REQUEST;
      } else {
        return NOT_A_REQUEST;
      }
    }
    
    /**
      Casts request to an isochronous write packets request
      (WRITE_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous
      packets request.
    */
    IsochronousWritePacketsRequest
      getIsochronousWritePacketsRequest() const noexcept {
      return IsochronousWritePacketsRequest(
        dynamic_cast<IsochronousWritePacketsRequestImpl*>(context.getValue())
      );
    }
    
    /**
      Casts request to an isochronous write fixed packets request
      (WRITE_FIXED_PACKETS_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous
      fixed packets request.
    */
    IsochronousWriteFixedPacketsRequest
      getIsochronousWriteFixedPacketsRequest() const noexcept {
      return IsochronousWriteFixedPacketsRequest(
        dynamic_cast<IsochronousWriteFixedPacketsRequestImpl*>(
          context.getValue()
        )
      );
    }

    /**
      Casts request to an isochronous write data request (WRITE_DATA_REQUEST).
      
      @return Returns an invalid object if the request is not an isochronous
      fixed data request.
    */
    IsochronousWriteDataRequest
      getIsochronousWriteDataRequest() const noexcept {
      return IsochronousWriteDataRequest(
        dynamic_cast<IsochronousWriteDataRequestImpl*>(context.getValue())
      );
    }
  };
  
  /**
    Interface implemented by isochronous read channels.
    
    @ingroup communications
    @short Isochronous read channel.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousReadChannelImpl : public ReferenceCountedObject {
  public:
    
    /**
      Returns the reserved subchannels.
    */
    virtual uint64 getSubchannels();

    /**
      Cancels all pending requests.
    */
    virtual void cancel();
    
    /**
      Returns a read packet request object (READ_PACKETS_REQUEST).
    */    
    virtual IsochronousReadPacketsRequest
      getReadPacketsRequest() const;
    
    /**
      Returns a read fixed packet request object (READ_FIXED_PACKETS_REQUEST).
    */    
    virtual IsochronousReadFixedPacketsRequest
      getReadFixedPacketsRequest() const;
    
    /**
      Returns a read fixed data request object (READ_FIXED_DATA_REQUEST).
    */    
    virtual IsochronousReadFixedDataRequest
      getReadFixedDataRequest() const;

    /**
      Queues the specified read request.
    */
    virtual void queue(
      IsochronousReadRequest& request);

    /**
      Queues the specified read request.
    */
    virtual void queue(
      IsochronousReadPacketsRequest& request);

    /**
      Queues the specified read request.
    */
    virtual void queue(
      IsochronousReadFixedPacketsRequest& request);

    /**
      Queues the specified read request.
    */
    virtual void queue(
      IsochronousReadFixedDataRequest& request);
    
    /**
      Queues the specified read requests.
    */
    virtual void queue(
      Allocator<IsochronousReadRequest>& request);

    /**
      Returns the next completed request.

      @return NOT_A_REQUEST if no request is available in the completion queue.
    */
    virtual IsochronousReadRequest dequeue();

    /**
      Dequeues the specified number of requests.
      
      @param request The number of request to dequeue.
      @param microseconds The timeout period in microseconds [0; 999999999].

      @return The number of dequeued requests.
    */
    virtual unsigned int dequeue(
      unsigned int requests,
      unsigned int microseconds);

    /**
      Wait for an event.

      @param microseconds The timeout period in microseconds [0; 999999999].
      
      @return True if event occured within the specified timeout period and
      otherwise false.
    */
    virtual bool wait(
      unsigned int microseconds);
  };
  
  /**
    Interface implemented by isochronous write channels.
    
    @ingroup communications
    @short Isochronous write channel.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API IsochronousWriteChannelImpl : public ReferenceCountedObject {
  public:

    /**
      Returns the reserved subchannels.
    */
    virtual uint64 getSubchannels();

    /**
      Cancels all pending requests.
    */
    virtual void cancel();
    
    /**
      Returns a write packet request object (WRITE_PACKETS_REQUEST).
    */    
    virtual IsochronousWritePacketsRequest
      getWritePacketsRequest() const;
    
    /**
      Returns a write fixed packet request object
      (WRITE_FIXED_PACKETS_REQUEST).
    */
    virtual IsochronousWriteFixedPacketsRequest
      getWriteFixedPacketsRequest() const;
    
    /**
      Returns a write data request object (WRITE_DATA_REQUEST).
    */
    virtual IsochronousWriteDataRequest
      getWriteDataRequest() const;
    
    /**
      Queues the specified write request.
    */
    virtual void queue(
      IsochronousWriteRequest& request);

    /**
      Queues the specified write request.
    */
    virtual void queue(
      IsochronousWritePacketsRequest& request);

    /**
      Queues the specified write request.
    */
    virtual void queue(
      IsochronousWriteFixedPacketsRequest& request);

    /**
      Queues the specified write request.
    */
    virtual void queue(
      IsochronousWriteDataRequest& request);

    /**
      Queues the specified write requests.
    */
    virtual void queue(
      Allocator<IsochronousWriteRequest>& request);

    /**
      Returns the next completed request.

      @return NOT_A_REQUEST if no request is available in the completion queue.
    */
    virtual IsochronousWriteRequest dequeue();
    
    /**
      Wait for an event.

      @param microseconds The timeout period in microseconds [0; 999999999].
      
      @return True if event occured within the specified timeout period and
      otherwise false.
    */
    virtual bool wait(
      unsigned int microseconds);
  };
protected:
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousReadPacketsRequestImpl>
  getContext(IsochronousReadPacketsRequest& request) noexcept {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousReadFixedPacketsRequestImpl>
  getContext(IsochronousReadFixedPacketsRequest& request) noexcept {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousReadFixedDataRequestImpl>
  getContext(IsochronousReadFixedDataRequest& request) noexcept {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousReadRequestImpl>
  getContext(IsochronousReadRequest& request) noexcept {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousWritePacketsRequestImpl>
  getContext(IsochronousWritePacketsRequest& request) noexcept {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousWriteFixedPacketsRequestImpl>
  getContext(IsochronousWriteFixedPacketsRequest& request) noexcept {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousWriteDataRequestImpl>
  getContext(IsochronousWriteDataRequest& request) noexcept {
    return request.context;
  }
  
  /**
    Returns the context.
  */
  static inline Reference<IsochronousWriteRequestImpl>
  getContext(IsochronousWriteRequest& request) noexcept {
    return request.context;
  }
public:
  
  class _COM_AZURE_DEV__BASE__API IsochronousReadChannel {
  private:
    
    /** Isochronous read channel implementation. */
    Reference<IsochronousReadChannelImpl> readChannel;
  public:

    /**
      Initialize channel as closed.
    */
    inline IsochronousReadChannel() noexcept
      : readChannel(new IsochronousReadChannelImpl()) {
    }
    
    /**
      Initializes read channel.
    */
    inline IsochronousReadChannel(
      IsochronousReadChannelImpl* _readChannel) noexcept
      : readChannel(_readChannel)
    {
      if (!_readChannel) {
        throw NullPointer(this);
      }
    }
    
    /**
      Initialize channel from other channel.
    */
    inline IsochronousReadChannel(const IsochronousReadChannel& copy) noexcept
      : readChannel(copy.readChannel) {
    }

    /**
      Assignment of channel by channel.
    */
    inline IsochronousReadChannel& operator=(const IsochronousReadChannel& assign) noexcept {
      readChannel = assign.readChannel;
      return *this;
    }
    
    /**
      Returns the reserved subchannels.
    */
    inline uint64 getSubchannels() {
      return readChannel->getSubchannels();
    }
    
    /**
      Cancels all pending requests.
    */
    inline void cancel() {
      readChannel->cancel();
    }
    
    /**
      Closes the channel.
    */
    inline void close() {
      readChannel = new IsochronousReadChannelImpl();
    }

    /**
      Returns a read packet request object (READ_PACKETS_REQUEST).
    */    
    inline IsochronousReadPacketsRequest
      getReadPacketsRequest() const noexcept {
      return readChannel->getReadPacketsRequest();
    }
    
    /**
      Returns a read fixed packet request object (READ_FIXED_PACKETS_REQUEST).
    */    
    inline IsochronousReadFixedPacketsRequest
      getReadFixedPacketsRequest() const noexcept {
      return readChannel->getReadFixedPacketsRequest();
    }
    
    /**
      Returns a read fixed data request object (READ_FIXED_DATA_REQUEST).
    */    
    inline IsochronousReadFixedDataRequest
      getReadFixedDataRequest() const noexcept {
      return readChannel->getReadFixedDataRequest();
    }

    /**
      Queues the specified read request.
    */
    inline void queue(
      IsochronousReadRequest& request) {
      readChannel->queue(request);
    }
    
    /**
      Queues the specified read request.
    */
    inline void queue(
      IsochronousReadPacketsRequest& request) {
      readChannel->queue(request);
    }
    
    /**
      Queues the specified read request.
    */
    inline void queue(
      IsochronousReadFixedPacketsRequest& request) {
      readChannel->queue(request);
    }
    
    /**
      Queues the specified read request.
    */
    inline void queue(
      IsochronousReadFixedDataRequest& request) {
      readChannel->queue(request);
    }
    
    /**
      Queues the specified read requests.
    */
    inline void queue(
      Allocator<IsochronousReadRequest>& request) {
      readChannel->queue(request);
    }
    
    /**
      Returns the next completed request.

      @return NOT_A_REQUEST if no request is available in the completion queue.
    */
    inline IsochronousReadRequest dequeue() {
      return readChannel->dequeue();
    }
    
    /**
      Dequeues the specified number of requests.
      
      @param request The number of request to dequeue.
      @param microseconds The timeout period in microseconds [0; 999999999].

      @return The number of dequeued requests.
    */
    inline unsigned int dequeue(
      unsigned int requests,
      unsigned int microseconds) {
      return readChannel->dequeue(requests, microseconds);
    }
    
    /**
      Wait for an event.

      @param microseconds The timeout period in microseconds [0; 999999999].
      
      @return True if event occured within the specified timeout period and
      otherwise false.
    */
    inline bool wait(unsigned int microseconds) {
      return readChannel->wait(microseconds);
    }
  };
  
  class _COM_AZURE_DEV__BASE__API IsochronousWriteChannel {
  private:
    
    /** Isochronous write channel implementation. */
    Reference<IsochronousWriteChannelImpl> writeChannel;
  public:

    /**
      Initializes channel as closed.
    */
    inline IsochronousWriteChannel() noexcept
      : writeChannel(new IsochronousWriteChannelImpl()) {
    }

    /**
      Initializes write channel.
    */
    inline IsochronousWriteChannel(
      IsochronousWriteChannelImpl* _writeChannel) noexcept
      : writeChannel(_writeChannel)
    {
      if (!_writeChannel) {
        throw NullPointer(this);
      }
    }

    /**
      Initializes channel from other channel.
    */
    inline IsochronousWriteChannel(const IsochronousWriteChannel& copy) noexcept
      : writeChannel(copy.writeChannel) {
    }

    /**
      Assignment of channel by channel.
    */
    inline IsochronousWriteChannel& operator=(const IsochronousWriteChannel& assign) noexcept
    {
      writeChannel = assign.writeChannel;
      return *this;
    }

    /**
      Returns the reserved subchannels.
    */
    inline uint64 getSubchannels()
    {
      return writeChannel->getSubchannels();
    }

    /**
      Cancels all pending requests.
    */
    inline void cancel()
    {
      writeChannel->cancel();
    }
    
    /**
      Closes the channel.
    */
    inline void close()
    {
      writeChannel = new IsochronousWriteChannelImpl();
    }
    
    /**
      Returns a write packet request object (WRITE_PACKETS_REQUEST).
    */    
    inline IsochronousWritePacketsRequest
      getWritePacketsRequest() const noexcept {
      return writeChannel->getWritePacketsRequest();
    }
    
    /**
      Returns a write fixed packet request object
      (WRITE_FIXED_PACKETS_REQUEST).
    */
    inline IsochronousWriteFixedPacketsRequest
      getWriteFixedPacketsRequest() const noexcept {
      return writeChannel->getWriteFixedPacketsRequest();
    }
    
    /**
      Returns a write data request object (WRITE_DATA_REQUEST).
    */
    inline IsochronousWriteDataRequest getWriteDataRequest() const noexcept {
      return writeChannel->getWriteDataRequest();
    }
    
    /**
      Queues the specified write request.
    */
    inline void queue(
      IsochronousWriteRequest& request) {
      return writeChannel->queue(request);
    }

    /**
      Queues the specified write request.
    */
    inline void queue(
      IsochronousWritePacketsRequest& request) {
      return writeChannel->queue(request);
    }

    /**
      Queues the specified write request.
    */
    inline void queue(
      IsochronousWriteFixedPacketsRequest& request) {
      return writeChannel->queue(request);
    }

    /**
      Queues the specified write request.
    */
    inline void queue(
      IsochronousWriteDataRequest& request) {
      return writeChannel->queue(request);
    }

    /**
      Queues the specified write requests.
    */
    inline void queue(
      Allocator<IsochronousWriteRequest>& request) {
      return writeChannel->queue(request);
    }
    
    /**
      Returns the next completed request.

      @return NOT_A_REQUEST if no request is available in the completion queue.
    */
    inline IsochronousWriteRequest dequeue() {
      return writeChannel->dequeue();
    }
    
    /**
      Wait for an event.

      @param microseconds The timeout period in microseconds [0; 999999999].
      
      @return True if event occured within the specified timeout period and
      otherwise false.
    */
    inline bool wait(unsigned int microseconds) {
      return writeChannel->wait(microseconds);
    }
  };

  /**
    @short Isochronous channel listener.
  */
  class _COM_AZURE_DEV__BASE__API IsochronousChannelListener {
  public:
    
    /**
      Invoked on an incoming isochronous packet.
      
      @param buffer The buffer.
      @param size The size of the data.
      
      @return False to stop the listener.
    */
    virtual bool onIsochronousPacket(
      const uint8* buffer,
      unsigned int size) noexcept = 0;
  };

  /**
    @short Function Control Protocol (FCP) listener.
  */
  class _COM_AZURE_DEV__BASE__API FunctionControlProtocolListener {
  public:
    
    /**
      Invoked on an incoming FCP request.
      
      @param nodeId The id of the source node.
      @param buffer The buffer.
      @param size The size of the data.      
    */
    virtual void onFCPRequest(
      unsigned short nodeId,
      const uint8* buffer,
      unsigned int size) noexcept = 0;

    /**
      Invoked on an incoming FCP response.
      
      @param nodeId The id of the source node.
      @param buffer The buffer.
      @param size The size of the data.      
    */
    virtual void onFCPResponse(
      unsigned short nodeId,
      const uint8* buffer,
      unsigned int size) noexcept = 0;
  };
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
