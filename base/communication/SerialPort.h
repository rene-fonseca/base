/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__SERIAL_PORT_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__SERIAL_PORT_H

#include <base/communication/CommunicationsPort.h>
#include <base/OperatingSystem.h>
#include <base/NotSupported.h>
#include <base/collection/List.h>
#include <base/io/Handle.h>
#include <base/io/async/AsynchronousInputStream.h>
#include <base/io/async/AsynchronousOutputStream.h>
#include <base/mem/Reference.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  An RS-232 serial communications port. SerialPort describes the low-level
  interface to a serial communications port made available by the underlying
  system. SerialPort defines the minimum required functionality for serial
  communications ports.

  @short Serial port.
  @see CommunicationPort.
  @ingroup communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SerialPort : public CommunicationsPort, public virtual AsynchronousIOStream {
private:

  Reference<Handle> handle;
  const String name;

  class SerialPortHandle : public Handle {
  public:

    /** Initializes file handle. */
    inline SerialPortHandle(OperatingSystem::Handle handle) throw()
      : Handle(handle) {
    }
    
    /** Releases the resources used by the serial port. */
    ~SerialPortHandle() throw(CommunicationsException);
  };
public:

  class Parity {
  public:

    /** Parity. */
    enum {
      EVEN, /**< Even parity. */
      MARK, /**< Mark. */
      NO, /**< No parity. */
      ODD, /**< Odd parity. */
      SPACE /**< Space. */
    };
  };

  class StopBits {
  public:
    
    /** The defined stop bits. */
    enum {
      ONE, /**< 1 stop bit. */
      ONE_HALF, /**< 1.5 stop bits. */
      TWO /**< 2 stop bits. */
    };
  };

  enum Event {
    BREAK, /**< Break event. */
    CD, /**< Carrier Detect (CD) event. */
    CTS, /**< Clear To Send (CTS) event. */
    DSR, /**< Data Set Ready (DSR) event. */
    ERROR, /**< Error event. */
    RING, /**< Ring Indicator (RI) event. */
    RLSD, /**< Received Line Signal Detector (RLSD) event. */
    INPUT_AVAILABLE, /**< Input data is available event. */
    OUTPUT_EMPTY /**< Output buffer is empty event. */
  };

  /**
    Returns the available ports.
  */
  static List<String> getPorts() throw();

  /**
    Initializes the serial communication port.

    @param name The name of the serial port (e.g. "com1" under Windows).
  */
  SerialPort(const String& name) throw(CommunicationsException);

  /**
    Returns the name of the communications port.
  */
  String getName() const throw();

  /**
    Registers a SerialPortEventListener object to listen for SerialEvents.
  */
//  void addEventListener(SerialPortEventListener);

  /**
    Returns the currently configured baud rate.
  */
  unsigned int getBaudRate() const throw(CommunicationsException);

  /**
    Returns the currently configured number of data bits.
  */
  unsigned int getDataBits() const throw(CommunicationsException);

  /**
    Returns the currently configured flow control mode.
  */
  unsigned int getFlowControlMode() const throw(CommunicationsException);

  /**
    Returns the currently configured parity setting.
  */
  unsigned int getParity() const throw(CommunicationsException);

  /**
    Returns the currently configured stop bits.
  */
  unsigned int getStopBits() const throw(CommunicationsException);

  /**
    Sets the serial port parameters.
  */
  void setParameters(unsigned int baudRate, unsigned int dataBits, unsigned int parity, unsigned int stopBits) throw(NotSupported, CommunicationsException);

  /**
    Sets the flow control mode.
  */
  void setFlowControlMode(unsigned int flowMode) throw(CommunicationsException);
  
  /**
    Returns the state of the CD (Carrier Detect) bit in the UART, if supported by the underlying implementation.
  */
  bool isCD() const throw(CommunicationsException);

  /**
    Returns the state of the CTS (Clear To Send) bit in the UART, if supported by the underlying implementation.
  */
  bool isCTS() const throw(CommunicationsException);

  /**
    Returns the state of the DSR (Data Set Ready) bit in the UART, if supported by the underlying implementation.
  */
  bool isDSR() const throw(CommunicationsException);

  /**
    Returns the state of the DTR (Data Terminal Ready) bit in the UART, if supported by the underlying implementation.
  */
  bool isDTR() const throw(CommunicationsException);

  /**
    Returns the state of the RI (Ring Indicator) bit in the UART, if supported by the underlying implementation.
  */
  bool isRI() const throw(CommunicationsException);

  /**
    Returns the state of the RTS (Request To Send) bit in the UART, if supported by the underlying implementation.
  */
  bool isRTS() const throw(CommunicationsException);

  /**
    Closes the communications port.
  */
  void close() throw(CommunicationsException);

  /**
    Sends a break of the specified duration.

    @param milliseconds The duration of the break in milliseconds.
  */
  void sendBreak(unsigned int milliseconds) throw(CommunicationsException);

  /**
    Sets or clears the Data Terminal Ready (DTR) signal.
  */
  void setDTR(bool state) throw(CommunicationsException);

  /**
    Sets or clears the Request To Send (RTS) signal.
  */
  void setRTS(bool state) throw(CommunicationsException);

  /**
    Returns true if read timeout is supported.
  */
  bool isReadTimeoutSupported() const throw(CommunicationsException);

  /**
    Returns true if write timeout is supported.
  */
  bool isWriteTimeoutSupported() const throw(CommunicationsException);

  /**
    Returns the maximum read timeout period.
  */
  unsigned int getReadTimeout() const throw(CommunicationsException);

  /**
    Returns the the maximum write timeout period.
  */
  unsigned int getWriteTimeout() const throw(CommunicationsException);

  /**
    Sets the maximum read timeout period.
  */
  void setReadTimeout(unsigned int milliseconds) throw(CommunicationsException);

  /**
    Sets the maximum write timeout period.
  */
  void setWriteTimeout(unsigned int milliseconds) throw(CommunicationsException);

  /**
    Sets the input buffer size.
  */
  void setInputBufferSize(unsigned int size) throw(CommunicationsException);

  /**
    Returns the input buffer size.
  */
  unsigned int getInputBufferSize() const throw(CommunicationsException);

  /**
    Sets the output buffer size.
  */
  void setOutputBufferSize(unsigned int size) throw(CommunicationsException);

  /**
    Returns the output buffer size.
  */
  unsigned int getOutputBufferSize() const throw(CommunicationsException);
  
  unsigned int read(char* buffer, unsigned int bytesToRead, bool nonblocking) throw(IOException);
  
  unsigned int write(const char* buffer, unsigned int bytesToWrite, bool nonblocking) throw(IOException);

  void asyncCancel() throw(AsynchronousException);
  
  AsynchronousReadOperation read(
    char* buffer,
    unsigned int bytesToRead,
    AsynchronousReadEventListener* listener) throw(AsynchronousException);

  AsynchronousWriteOperation write(
    const char* buffer,
    unsigned int bytesToWrite,
    AsynchronousWriteEventListener* listener) throw(AsynchronousException);
  
  /**
    Destroys the serial port object.
  */
  virtual ~SerialPort() throw(CommunicationsException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
