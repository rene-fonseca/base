/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/communication/SerialPort.h>
#include <base/communication/CommunicationsException.h>
#include <base/string/StringOutputStream.h>
#include <base/UnexpectedFailure.h>
#include <base/io/EndOfFile.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <base/platforms/win32/AsyncReadStreamContext.h> // platform specific
  #include <base/platforms/win32/AsyncWriteStreamContext.h> // platform specific
  #define NO_STRICT
  // I don't get it: in STRICT mode handles are of type int but in NO_STRICT
  // mode the handles are of size void*. This is a problem on 64 bit platforms
  // where int and void* may be of different sizes.
  #include <windows.h>
#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
  #include <sys/types.h>
  #include <sys/stat.h> // open
  #include <fcntl.h> // open
  #include <unistd.h> // close, read...
  #include <errno.h> // errno
  #include <limits.h> // SSIZE_MAX

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
  #endif // os
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SerialPort::SerialPortHandle::~SerialPortHandle() throw(CommunicationsException) {
  if (isValid()) { // dont try to close if handle is invalidated
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (!::CloseHandle(getHandle())) {
      throw CommunicationsException("Unable to close port", this);
    }
#else // unix
    if (::close(getHandle())) {
      throw CommunicationsException("Unable to close port", this);
    }
#endif // flavor
  }
}

List<String> SerialPort::getPorts() throw() {
  List<String> result;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // FIXME: use registry to determine the available ports
  unsigned int failures = 0;
  for (unsigned int i = 1; i <= 256; ++i) { // only the first 256 ports
    StringOutputStream stream;
    stream << MESSAGE("com") << i << FLUSH;
    HANDLE handle = ::CreateFile(stream.getString().getElements(),
                                 0, // query only
                                 0, // comm devices must be opened w/exclusive-access
                                 0, // no security attributes
                                 OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                                 0, // no overlapped I/O
                                 0 // must be 0 for comm devices
    );
    if (handle == OperatingSystem::INVALID_HANDLE) {
      if (++failures > 16) {
        break; // stop if we get too many consecutive failures
      }
    } else {
      failures = 0;
      ::CloseHandle(handle);
      result.append(stream.getString()); // add to list
    }
  }
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    unsigned int failures = 0;
    for (unsigned int i = 0; i < 256; ++i) { // only the first 256 ports
      StringOutputStream stream;
      stream << MESSAGE("/dev/ttyS") << i << FLUSH;
    }
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    unsigned int failures = 0;
    for (unsigned int i = 0; i < 256; ++i) { // only the first 256 ports
      StringOutputStream stream;
      stream << MESSAGE("/dev/ttyS") << i << FLUSH;
    }
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
    unsigned int failures = 0;
    for (unsigned int i = 0; i < 256; ++i) { // only the first 256 ports
      StringOutputStream stream;
      stream << MESSAGE("/dev/ttyS") << i << FLUSH;
    }
  #endif // os
#endif // flavor
  return result;
}

SerialPort::SerialPort(const String& _name) throw(CommunicationsException) : name(_name) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HANDLE handle = ::CreateFile(name.getElements(),
                               GENERIC_READ | GENERIC_WRITE,
                               0, // comm devices must be opened w/exclusive-access
                               0, // no security attributes
                               OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                               FILE_FLAG_OVERLAPPED, // overlapped I/O
                               0 // must be 0 for comm devices
  );
  assert(handle != OperatingSystem::INVALID_HANDLE, CommunicationsException("Unable to open serial port", this));
  this->handle = new Handle(handle);
#endif // flavor
}

String SerialPort::getName() const throw() {
  return name;
}

void SerialPort::close() throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::CloseHandle(handle->getHandle()) != 0, CommunicationsException());
  handle = new SerialPortHandle(OperatingSystem::INVALID_HANDLE);
#endif // flavor
}

unsigned int SerialPort::getBaudRate() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  return dcb.BaudRate;
#endif // flavor
}

unsigned int SerialPort::getDataBits() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  return dcb.ByteSize;
#endif // flavor
}

unsigned int SerialPort::getParity() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  switch (dcb.Parity) {
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
    throw UnexpectedFailure(this); // we should never end up here
  }
#endif // flavor
}

unsigned int SerialPort::getStopBits() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DCB dcb;
  assert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  switch (dcb.StopBits) {
  case ONESTOPBIT:
    return StopBits::ONE;
  case ONE5STOPBITS:
    return StopBits::ONE_HALF;
  case TWOSTOPBITS:
    return StopBits::TWO;
  default:
    throw UnexpectedFailure(this); // we should never end up here
  }
#endif // flavor
}

bool SerialPort::isCD() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#endif // flavor
}

bool SerialPort::isCTS() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD state;
  assert(::GetCommModemStatus(handle->getHandle(), &state) != 0, CommunicationsException());
  return (state & MS_CTS_ON) != 0;
#endif // flavor
}

bool SerialPort::isDSR() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD state;
  assert(::GetCommModemStatus(handle->getHandle(), &state) != 0, CommunicationsException());
  return (state & MS_DSR_ON) != 0;
#endif // flavor
}

bool SerialPort::isDTR() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#endif // flavor
}

bool SerialPort::isRI() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD state;
  assert(::GetCommModemStatus(handle->getHandle(), &state) != 0, CommunicationsException());
  return (state & MS_RING_ON) != 0;
#endif // flavor
}

bool SerialPort::isRTS() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#endif // flavor
}

void SerialPort::setParameters(unsigned int baudRate, unsigned int dataBits, unsigned int parity, unsigned int stopBits) throw(NotSupported, CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static const unsigned int mapParity[] = {EVENPARITY, MARKPARITY, NOPARITY, ODDPARITY, SPACEPARITY};
  static const unsigned int mapStopBits[] = {ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS};
  DCB dcb;
  assert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  dcb.BaudRate = baudRate;
  dcb.ByteSize = dataBits;
  dcb.Parity = mapParity[parity]; // FIXME
  dcb.StopBits = mapStopBits[stopBits]; // FIXME
  assert(::SetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  // need to throw NotSupported
#endif // flavor
}

void SerialPort::setFlowControlMode(unsigned int flowMode) throw(CommunicationsException) {
}

void SerialPort::sendBreak(unsigned int milliseconds) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetCommBreak(handle->getHandle());
  ::Sleep(milliseconds); // FIXME: what if INFINITE
  ::ClearCommBreak(handle->getHandle());
#endif // flavor
}

void SerialPort::setDTR(bool state) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::EscapeCommFunction(handle->getHandle(), state ? (SETDTR) : (CLRDTR)) != 0, CommunicationsException());
#endif // flavor
}

void SerialPort::setRTS(bool state) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::EscapeCommFunction(handle->getHandle(), state ? (SETRTS) : (CLRRTS)) != 0, CommunicationsException());
#endif // flavor
}

bool SerialPort::isReadTimeoutSupported() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return true;
#endif // flavor
}

bool SerialPort::isWriteTimeoutSupported() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return true;
#endif // flavor
}

unsigned int SerialPort::getReadTimeout() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  assert(::GetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
  return timeouts.ReadTotalTimeoutConstant;
#endif // flavor
}

unsigned int SerialPort::getWriteTimeout() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  assert(::GetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
  return timeouts.WriteTotalTimeoutConstant;
#endif // flavor
}

void SerialPort::setReadTimeout(unsigned int milliseconds) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  clear(timeouts);
  timeouts.ReadTotalTimeoutConstant = milliseconds;
  assert(::SetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
#endif // flavor
}

void SerialPort::setWriteTimeout(unsigned int milliseconds) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  clear(timeouts);
  timeouts.WriteTotalTimeoutConstant = milliseconds;
  assert(::SetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
#endif // flavor
}

unsigned int SerialPort::getInputBufferSize() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert(::GetCommProperties(handle->getHandle(), &properties) != 0, CommunicationsException());
  return properties.dwCurrentRxQueue;
#endif // flavor
}

void SerialPort::setInputBufferSize(unsigned int size) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert((::GetCommProperties(handle->getHandle(), &properties) != 0) &&
         (::SetupComm(handle->getHandle(), size, properties.dwCurrentTxQueue) != 0), CommunicationsException());
#endif // flavor
}

unsigned int SerialPort::getOutputBufferSize() const throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert(::GetCommProperties(handle->getHandle(), &properties) != 0, CommunicationsException());
  return properties.dwCurrentTxQueue;
#endif // flavor
}

void SerialPort::setOutputBufferSize(unsigned int size) throw(CommunicationsException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COMMPROP properties;
  assert((::GetCommProperties(handle->getHandle(), &properties) != 0) &&
         (::SetupComm(handle->getHandle(), properties.dwCurrentRxQueue, size) != 0), CommunicationsException());
#endif // flavor
}

void SerialPort::asyncCancel() throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::CancelIo(handle->getHandle());
#else // unix
#endif // flavor
}

AsynchronousReadOperation SerialPort::read(char* buffer, unsigned int bytesToRead, AsynchronousReadEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncReadStreamContext(handle->getHandle(), buffer, bytesToRead, listener);
#endif // flavor
}

AsynchronousWriteOperation SerialPort::write(const char* buffer, unsigned int bytesToWrite, AsynchronousWriteEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncWriteStreamContext(handle->getHandle(), buffer, bytesToWrite, listener);
#endif // flavor
}

unsigned int SerialPort::read(char* buffer, unsigned int bytesToRead, bool nonblocking) throw(IOException) {
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::ReadFile(handle->getHandle(), buffer, bytesToRead, &result, 0);
    if (!success) { // has error occured
      if (::GetLastError() == ERROR_BROKEN_PIPE) {
        result = 0;
      } else {
        throw IOException("Unable to read from object", this);
      }
    }
#else // unix
    int result = ::read(handle->getHandle(), buffer, minimum<unsigned int>(bytesToRead, SSIZE_MAX));
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw IOException("Unable to read from object", this);
      }
    }
#endif // flavor
    if (result == 0) { // has end been reached
      if (bytesToRead > 0) {
        throw EndOfFile(this); // attempt to read beyond end of stream
      }
    }
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
  }
  return bytesRead;
}

unsigned int SerialPort::write(const char* buffer, unsigned int bytesToWrite, bool nonblocking) throw(IOException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesToWrite) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::WriteFile(handle->getHandle(), buffer, bytesToWrite, &result, 0);
    if (!success) {
      throw IOException("Unable to write to object", this);
    }
#else // unix
    int result = ::write(handle->getHandle(), buffer, minimum<unsigned int>(bytesToWrite, SSIZE_MAX));
    if (result < 0) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw IOException("Unable to write to object", this);
      }
    }
#endif // flavor
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

SerialPort::~SerialPort() throw(CommunicationsException) {
  close();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
