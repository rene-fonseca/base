/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/SerialPort.h>
#include <base/communication/CommunicationsException.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #define NO_STRICT
  // I don't get it: in STRICT mode handles are of type int but in NO_STRICT
  // mode the handles are of size void*. This is a problem on 64 bit platforms
  // where int and void* may be of different sizes.
  #include <windows.h>
#elif (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  #include <sys/types.h> // open
  #include <sys/stat.h> // open
  #include <fcntl.h> // open
  #include <unistd.h> // read
  #include <errno.h> // errno
  #include <limits.h> // SSIZE_MAX

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
  #endif // os
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

List<String> SerialPort::getPorts() throw() {
  List<String> result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  // FIXME: use registry to determine the available ports
  unsigned int failures = 0;
  for (unsigned int i = 1; i <= 256; ++i) { // only the first 256 ports
    StringOutputStream stream;
    stream << MESSAGE("com") << i << FLUSH;
    HANDLE handle = CreateFile(stream.getString(),
                               0, // query only
                               0, // comm devices must be opened w/exclusive-access
                               0, // no security attributes
                               OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                               0, // no overlapped I/O
                               0 // must be 0 for comm devices
    );
    if (handle == INVALID_HANDLE_VALUE) {
      if (++failures > 16) {
        break; // stop if we get too many consecutive failures
      }
    } else {
      failures = 0;
      CloseHandle(handle);
      result.append(stream.getString()); // add to list
    }
  }
#endif // flavour
  return result;
}

SerialPort::SerialPort(const String& n) throw(CommunicationsException) : name(n) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  handle = CreateFile(name,
                               GENERIC_READ | GENERIC_WRITE,
                               0, // comm devices must be opened w/exclusive-access
                               0, // no security attributes
                               OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                               FILE_FLAG_OVERLAPPED, // overlapped I/O
                               0 // must be 0 for comm devices
  );
  assert(handle != INVALID_HANDLE_VALUE, CommunicationsException("Unable to open serial port"));
#endif // flavour
}

String SerialPort::getName() const throw() {
  return name;
}

void SerialPort::close() throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert(CloseHandle(handle) != 0, CommunicationsException());
#endif // flavour
}

unsigned int SerialPort::getBaudRate() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(GetCommState(handle, &dcb) != 0, CommunicationsException());
  return dbc.BaudRate;
#endif // flavour
}

unsigned int SerialPort::getDataBits() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(GetCommState(handle, &dcb) != 0, CommunicationsException());
  return dcb.ByteSize;
#endif // flavour
}

unsigned int SerialPort::getParity() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(GetCommState(handle, &dcb) != 0, CommunicationsException());
  switch (dcb.Parity)
  case EVENPARITY:
    return Parity::EVEN;
  case MARKPARITY:
    return Parity::MARK;
  case NOPARITY:
    return Parity::NO;
  case ODDPARITY:
    return Parity::ODD;
  case SPACEPARITY:
    return Parity::SPACE;
  default:
    return Parity::UNSPECIFIED;
  }
#endif // flavour
}

unsigned int SerialPort::getStopBits() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(GetCommState(handle, &dcb) != 0, CommunicationsException());
  switch (dcb.StopBits)
  case ONESTOPBIT:
    return StopBits::ONE;
  case ONE5STOPBITS:
    return StopBits::ONE_HALF;
  case TWOSTOPBITS:
    return StopBits::TWO;
  default:
    return StopBits::UNSPECIFIED;
  }
#endif // flavour
}

bool SerialPort::isCD() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#endif // flavour
}

bool SerialPort::isCTS() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DWORD state;
  assert(GetModemStatus(handle, &state) != 0, CommunicationsException());
  return (state & MS_CTS_ON) != 0;
#endif // flavour
}

bool SerialPort::isDSR() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DWORD state;
  assert(GetModemStatus(handle, &state) != 0, CommunicationsException());
  return (state & MS_DTS_ON) != 0;
#endif // flavour
}

bool SerialPort::isDTR() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#endif // flavour
}

bool SerialPort::isRI() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DWORD state;
  assert(GetModemStatus(handle, &state) != 0, CommunicationsException());
  return (state & MS_RING_ON) != 0;
#endif // flavour
}

bool SerialPort::isRTS() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#endif // flavour
}

void SerialPort::setParameters(unsigned int baudRate, unsigned int dataBits, unsigned int parity, unsigned int stopBits) throw(NotSupported, CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  static const unsigned int mapParity[] = {EVENPARITY, MARKPARITY, NOPARITY, ODDPARITY, SPACEPARITY};
  static const unsigned int mapStopBits[] = {ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS};
  DCB dcb;
  assert(GetCommState(handle, &dcb) != 0, CommunicationsException());
  dcb.BaudRate = baudRate;
  dcb.ByteSize = dataBits;
  dcb.Parity = mapParity[parity]; // FIXME
  dcb.StopBits = mapStopBits[stopBits]; // FIXME
  result = SetCommState(handle, &dcb);
  assert(result != 0, CommunicationsException());
  // need to throw NotSupported
#endif // flavour
}

void SerialPort::sendBreak(unsigned int milliseconds) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SetCommBreak(handle);
  Sleep(milliseconds); // FIXME: what if INFINITE
  ClearCommBreak(handle);
#endif // flavour
}

void SerialPort::setDTR(bool state) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert(EscapeCommFunction(handle, state ? : SETDTR : CLRDTR) != 0, CommunicationsException());
#endif // flavour
}

void SerialPort::setRTS(bool state) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert(EscapeCommFunction(handle, state ? : SETRTS : CLRRTS) != 0, CommunicationsException());
#endif // flavour
}

bool SerialPort::isReadTimeoutSupported() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return true;
#endif // flavour
}

bool SerialPort::isWriteTimeoutSupported() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return true;
#endif // flavour
}

unsigned int SerialPort::getReadTimeout() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  assert(GetCommTimeouts(handle, &timeouts) != 0, CommunicationsException());
  return timeouts.ReadTotalTimeoutConstant;
#endif // flavour
}

unsigned int SerialPort::getWriteTimeout() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  assert(GetCommTimeouts(handle, &timeouts) != 0, CommunicationsException());
  return timeouts.WriteTotalTimeoutConstant;
#endif // flavour
}

void SerialPort::setReadTimeout(unsigned int milliseconds) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  clear(timeouts);
  timeouts.ReadTotalTimeoutConstant = milliseconds;
  assert(SetCommTimeouts(handle, &timeouts) != 0, CommunicationsException());
#endif // flavour
}

void SerialPort::setWriteTimeout(unsigned int milliseconds) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  clear(timeouts);
  timeouts.WriteTotalTimeoutConstant = milliseconds;
  assert(SetCommTimeouts(handle, &timeouts) != 0, CommunicationsException());
#endif // flavour
}

unsigned int SerialPort::getInputBufferSize() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert(GetCommProperties(handle, &properties) != 0, CommunicationsException());
  return properties.dwCurrentRxQueue;
#endif // flavour
}

void SerialPort::setInputBufferSize(unsigned int size) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert((GetCommProperties(handle, &properties) != 0) &&
           (SetupComm(handle, size, properties.dwCurrentTxQueue) != 0), CommunicationsException());
#endif // flavour
}

unsigned int SerialPort::getOutputBufferSize() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert(GetCommProperties(handle, &properties) != 0, CommunicationsException());
  return properties.dwCurrentTxQueue;
#endif // flavour
}

void SerialPort::setOutputBufferSize(unsigned int size) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert((GetCommProperties(handle, &properties) != 0) &&
           (SetupComm(handle, properties.dwCurrentRxQueue, size) != 0), CommunicationsException());
#endif // flavour
}

SerialPort::~SerialPort() throw(CommunicationsException) {
  close();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
