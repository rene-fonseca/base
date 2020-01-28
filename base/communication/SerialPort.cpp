/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/communication/SerialPort.h>
#include <base/communication/CommunicationsException.h>
#include <base/string/StringOutputStream.h>
#include <base/string/WideString.h>
#include <base/UnexpectedFailure.h>
#include <base/io/EndOfFile.h>
#include <base/Profiler.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadStreamContext.h> // platform specific
#  include <base/platforms/win32/AsyncWriteStreamContext.h> // platform specific
#  define NO_STRICT
  // I don't get it: in STRICT mode handles are of type int but in NO_STRICT
  // mode the handles are of size void*. This is a problem on 64 bit platforms
  // where int and void* may be of different sizes.
#  include <windows.h>
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
#  include <sys/types.h>
#  include <sys/stat.h> // open
#  include <fcntl.h> // open
#  include <unistd.h> // close, read...
#  include <errno.h> // errno
#  include <limits.h> // SSIZE_MAX

#  if !defined(SSIZE_MAX)
#    define SSIZE_MAX (1024*1024)
#  endif

#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SerialPort::SerialPortHandle::~SerialPortHandle()
{
  if (isValid()) { // dont try to close if handle is invalidated
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (!::CloseHandle(getHandle())) {
      _throw CommunicationsException("Unable to close port.", this);
    }
#else // unix
    if (::close(getHandle())) {
      _throw CommunicationsException("Unable to close port.", this);
    }
#endif // flavor
  }
}

List<String> SerialPort::getPorts() noexcept
{
  List<String> result;
  // Unicode::ToWCharString(stream.getString()).native();

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // FIXME: use registry to determine the available ports
  unsigned int failures = 0;
  for (unsigned int i = 1; i <= 256; ++i) { // only the first 256 ports
    String id = StringOutputStream() << "com" << i;
    HANDLE handle = ::CreateFile(ToWCharString(id),
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
      result.append(id); // add to list
    }
  }
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
    unsigned int failures = 0;
    for (unsigned int i = 0; i < 256; ++i) { // only the first 256 ports
      StringOutputStream stream;
      stream << "/dev/ttyS" << i << FLUSH;
    }
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
    unsigned int failures = 0;
    for (unsigned int i = 0; i < 256; ++i) { // only the first 256 ports
      StringOutputStream stream;
      stream << "/dev/ttyS" << i << FLUSH;
    }
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
    unsigned int failures = 0;
    for (unsigned int i = 0; i < 256; ++i) { // only the first 256 ports
      StringOutputStream stream;
      stream << "/dev/ttyS" << i << FLUSH;
    }
  #endif // os
#endif // flavor
  return result;
}

SerialPort::SerialPort(const String& _name)
  : name(_name)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle = ::CreateFileW(ToWCharString(name),
                               GENERIC_READ | GENERIC_WRITE,
                               0, // comm devices must be opened w/exclusive-access
                               0, // no security attributes
                               OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                               FILE_FLAG_OVERLAPPED, // overlapped I/O
                               0 // must be 0 for comm devices
  );
  bassert(handle != OperatingSystem::INVALID_HANDLE, CommunicationsException("Unable to open serial port.", this));
  this->handle = new Handle(handle);
#endif // flavor
}

String SerialPort::getName() const noexcept
{
  return name;
}

void SerialPort::close()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(::CloseHandle(handle->getHandle()) != 0, CommunicationsException());
  handle = new SerialPortHandle(OperatingSystem::INVALID_HANDLE);
#endif // flavor
}

unsigned int SerialPort::getBaudRate() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DCB dcb;
  bassert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  return dcb.BaudRate;
#else // unix
  return 0; // TAG: fixme
#endif // flavor
}

unsigned int SerialPort::getDataBits() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DCB dcb;
  bassert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  return dcb.ByteSize;
#else // unix
  return 0; // TAG: fixme
#endif // flavor
}

unsigned int SerialPort::getParity() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DCB dcb;
  bassert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
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
    _throw UnexpectedFailure(this); // we should never end up here
  }
#else // unix
  return Parity::NO; // TAG: fixme
#endif // flavor
}

unsigned int SerialPort::getStopBits() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DCB dcb;
  bassert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  switch (dcb.StopBits) {
  case ONESTOPBIT:
    return StopBits::ONE;
  case ONE5STOPBITS:
    return StopBits::ONE_HALF;
  case TWOSTOPBITS:
    return StopBits::TWO;
  default:
    _throw UnexpectedFailure(this); // we should never end up here
  }
#else // unix
  return 0; // TAG: fixme
#endif // flavor
}

bool SerialPort::isCD() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return false; // TAG: fixme
#else // unix
  return false; // TAG: fixme
#endif // flavor
}

bool SerialPort::isCTS() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD state = 0;
  bassert(::GetCommModemStatus(handle->getHandle(), &state) != 0, CommunicationsException());
  return (state & MS_CTS_ON) != 0;
#else // unix
  return false; // TAG: fixme
#endif // flavor
}

bool SerialPort::isDSR() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD state = 0;
  bassert(::GetCommModemStatus(handle->getHandle(), &state) != 0, CommunicationsException());
  return (state & MS_DSR_ON) != 0;
#else // unix
  return false; // TAG: fixme
#endif // flavor
}

bool SerialPort::isDTR() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return false; // TAG: fixme
#else // unix
  return false; // TAG: fixme
#endif // flavor
}

bool SerialPort::isRI() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD state = 0;
  bassert(::GetCommModemStatus(handle->getHandle(), &state) != 0, CommunicationsException());
  return (state & MS_RING_ON) != 0;
#else // unix
  return false; // TAG: fixme
#endif // flavor
}

bool SerialPort::isRTS() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return false; // TAG: fixme
#else // unix
  return false; // TAG: fixme
#endif // flavor
}

void SerialPort::setParameters(unsigned int baudRate, unsigned int dataBits, unsigned int parity,
  unsigned int stopBits)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static const unsigned int mapParity[] = {EVENPARITY, MARKPARITY, NOPARITY, ODDPARITY, SPACEPARITY};
  static const unsigned int mapStopBits[] = {ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS};
  DCB dcb;
  bassert(::GetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  dcb.BaudRate = baudRate;
  dcb.ByteSize = dataBits;
  dcb.Parity = mapParity[parity]; // FIXME
  dcb.StopBits = mapStopBits[stopBits]; // FIXME
  bassert(::SetCommState(handle->getHandle(), &dcb) != 0, CommunicationsException());
  // need to throw NotSupported
#else // unix
  // TAG: fixme
#endif // flavor
}

void SerialPort::setFlowControlMode(unsigned int flowMode)
{
}

void SerialPort::sendBreak(unsigned int milliseconds)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::SetCommBreak(handle->getHandle());
  ::Sleep(milliseconds); // FIXME: what if INFINITE
  ::ClearCommBreak(handle->getHandle());
#endif // flavor
}

void SerialPort::setDTR(bool state)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(::EscapeCommFunction(handle->getHandle(), state ? (SETDTR) : (CLRDTR)) != 0, CommunicationsException());
#endif // flavor
}

void SerialPort::setRTS(bool state)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(::EscapeCommFunction(handle->getHandle(), state ? (SETRTS) : (CLRRTS)) != 0, CommunicationsException());
#endif // flavor
}

bool SerialPort::isReadTimeoutSupported() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#endif // flavor
  return true;
}

bool SerialPort::isWriteTimeoutSupported() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#endif // flavor
  return true;
}

unsigned int SerialPort::getReadTimeout() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  bassert(::GetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
  return timeouts.ReadTotalTimeoutConstant;
#else
  return 0;
#endif // flavor
}

unsigned int SerialPort::getWriteTimeout() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  bassert(::GetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
  return timeouts.WriteTotalTimeoutConstant;
#else
  return 0;
#endif // flavor
}

void SerialPort::setReadTimeout(unsigned int milliseconds)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  clear(timeouts);
  timeouts.ReadTotalTimeoutConstant = milliseconds;
  bassert(::SetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
#endif // flavor
}

void SerialPort::setWriteTimeout(unsigned int milliseconds)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMTIMEOUTS timeouts;
  clear(timeouts);
  timeouts.WriteTotalTimeoutConstant = milliseconds;
  bassert(::SetCommTimeouts(handle->getHandle(), &timeouts) != 0, CommunicationsException());
#endif // flavor
}

unsigned int SerialPort::getInputBufferSize() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMPROP properties;
  clear(properties);
  bassert(::GetCommProperties(handle->getHandle(), &properties) != 0, CommunicationsException());
  return properties.dwCurrentRxQueue;
#else
  return 0;
#endif // flavor
}

void SerialPort::setInputBufferSize(unsigned int size)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMPROP properties;
  clear(properties);
  bassert((::GetCommProperties(handle->getHandle(), &properties) != 0) &&
         (::SetupComm(handle->getHandle(), size, properties.dwCurrentTxQueue) != 0), CommunicationsException());
#endif // flavor
}

unsigned int SerialPort::getOutputBufferSize() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMPROP properties;
  clear(properties);
  bassert(::GetCommProperties(handle->getHandle(), &properties) != 0, CommunicationsException());
  return properties.dwCurrentTxQueue;
#else
  return 0;
#endif // flavor
}

void SerialPort::setOutputBufferSize(unsigned int size)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  COMMPROP properties;
  clear(properties);
  bassert((::GetCommProperties(handle->getHandle(), &properties) != 0) &&
         (::SetupComm(handle->getHandle(), properties.dwCurrentRxQueue, size) != 0), CommunicationsException());
#endif // flavor
}

void SerialPort::asyncCancel()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::CancelIo(handle->getHandle());
#else // unix
#endif // flavor
}

AsynchronousReadOperation SerialPort::read(
  uint8* buffer,
  unsigned int bytesToRead,
  AsynchronousReadEventListener* listener)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncReadStreamContext(handle->getHandle(), buffer, bytesToRead, listener);
#else
  return AsynchronousReadOperation();
#endif // flavor
}

AsynchronousWriteOperation SerialPort::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  AsynchronousWriteEventListener* listener)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncWriteStreamContext(handle->getHandle(), buffer, bytesToWrite, listener);
#else
  return AsynchronousWriteOperation();
#endif // flavor
}

unsigned int SerialPort::read(
  uint8* buffer,
  unsigned int bytesToRead,
  bool nonblocking)
{
  Profiler::IOReadTask profile("SerialPort::read()");

  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::ReadFile(handle->getHandle(), buffer, bytesToRead, &result, 0);
    if (!success) { // has error occured
      if (::GetLastError() == ERROR_BROKEN_PIPE) {
        result = 0;
      } else {
        _throw IOException("Unable to read from object.", this);
      }
    }
#else // unix
    int result = (int)::read(handle->getHandle(), buffer, minimum<size_t>(bytesToRead, SSIZE_MAX));
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        _throw IOException("Unable to read from object.", this);
      }
    }
#endif // flavor
    if (result == 0) { // has end been reached
      if (bytesToRead > 0) {
        _throw EndOfFile(this); // attempt to read beyond end of stream
      }
    }
    profile.onBytesRead(result);
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
  }
  return bytesRead;
}

unsigned int SerialPort::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  bool nonblocking)
{
  Profiler::IOWriteTask profile("SerialPort::write()");

  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesToWrite) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::WriteFile(handle->getHandle(), buffer, bytesToWrite, &result, 0);
    if (!success) {
      _throw IOException("Unable to write to object.", this);
    }
#else // unix
    int result = (int)::write(handle->getHandle(), buffer, minimum<size_t>(bytesToWrite, SSIZE_MAX));
    if (result < 0) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        _throw IOException("Unable to write to object.", this);
      }
    }
#endif // flavor
    profile.onBytesWritten(result);
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

SerialPort::~SerialPort()
{
  close();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
