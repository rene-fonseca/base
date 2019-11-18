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
#include <base/io/Pipe.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/string/String.h>
#include <base/Profiler.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/types.h>
#  include <sys/time.h> // defines timeval on Linux systems
#  include <sys/stat.h>
#  include <sys/ioctl.h>
#  include <limits.h> // defines PIPE_BUF...
#  include <unistd.h>
#  include <errno.h>
#  include <string.h> // required on solaris 'cause FD_ZERO uses memset
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
#    warning ioctl is not supported (CYGWIN)
#  elif (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__MACOS)
#    include <stropts.h> // defines FLUSH macros
#  endif
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Pair<Pipe, Pipe> Pipe::make() throw(PipeException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // create two named pipes with unique names (one for input and one for output - may be the same handle)
  HANDLE ihandle = ::CreateFile(
    L"\\\\.\\pipe\\???i", // file name
    GENERIC_READ | SYNCHRONIZE, // access mode FIXME
    FILE_SHARE_DELETE, // share mode
    0, // security descriptor
    CREATE_NEW, // how to create (fail is already exists)
    FILE_FLAG_OVERLAPPED | FILE_FLAG_DELETE_ON_CLOSE, // file attributes
    0
  );
  HANDLE ohandle = ::CreateFile(
    L"\\\\.\\pipe\\???o", // file name - generate unique name - formal name - time - prefix
    GENERIC_WRITE | SYNCHRONIZE, // access mode FIXME
    FILE_SHARE_DELETE, // share mode
    0, // security descriptor
    CREATE_NEW, // how to create (fail is already exists)
    FILE_FLAG_OVERLAPPED | FILE_FLAG_DELETE_ON_CLOSE, // file attributes
    0
  );
  // FIXME: remember to close before raising an exception
  bassert(ihandle != OperatingSystem::INVALID_HANDLE, IOException("Unable to make pipes", Type::getType<Pipe>()));
  bassert(ohandle != OperatingSystem::INVALID_HANDLE, IOException("Unable to make pipes", Type::getType<Pipe>()));
  Pipe p;
  Pipe q;
  p.fd = new PipeHandle(ihandle);
  q.fd = new PipeHandle(ohandle);
  return makePair(p, q);
#else // unix
  OperatingSystem::Handle handles[2];
  if (::pipe(handles)) {
    throw PipeException("Unable to create pipe", Type::getType<Pipe>());
  }
  Pipe p;
  Pipe q;
  p.fd = new PipeHandle(handles[0]);
  q.fd = new PipeHandle(handles[1]);
  return makePair(p, q);
#endif // flavor
}



Pipe::PipeHandle::~PipeHandle() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (isValid()) {
    if (::CloseHandle(getHandle())) {
      throw PipeException("Unable to close pipe", this);
    }
  }
#else // unix
  if (isValid()) {
    if (::close(getHandle())) {
      throw PipeException("Unable to close pipe", this);
    }
  }
#endif
}



Pipe::Pipe() throw() : fd(PipeHandle::invalid), end(false) {
}

void Pipe::close() throw(PipeException) {
  fd = PipeHandle::invalid;
  end = true;
}

unsigned int Pipe::getBufferSize() const throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = 0;
  GetNamedPipeInfo(fd->getHandle(), 0, &result, 0, 0); // TAG: separate input and output buffer sizes
  return result;
#else // unix
  return PIPE_BUF;
#endif
}

bool Pipe::atEnd() const throw(PipeException) {
  return end;
}

unsigned int Pipe::available() const throw(PipeException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD bytesAvailable = 0;
  if (!::PeekNamedPipe(fd->getHandle(), 0, 0, 0, &bytesAvailable, 0)) {
    throw PipeException("Unable to get available bytes", this);
  }
  return bytesAvailable;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = fstat64(fd->getHandle(), &status);
  #else
    struct stat status;
    int result = ::fstat(fd->getHandle(), &status);
  #endif // LFS
    bassert(result == 0, IOException("Unable to get available bytes", this));
    return status.st_size;
#endif
}

unsigned int Pipe::skip(unsigned int count) throw(PipeException)
{
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  unsigned int bytesSkipped = 0;
  while (bytesSkipped < count) {
    unsigned int bytesToRead = minimum<MemorySize>(count - bytesSkipped, buffer.getSize());
    bytesSkipped += read(buffer.getElements(), bytesToRead);
  }
  return bytesSkipped;
}

void Pipe::flush() throw(PipeException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::FlushFileBuffers(fd->getHandle())) {
    throw PipeException("Unable to flush pipe", this);
  }
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
//    #warning Pipe::flush() not supported (CYGWIN)
  #else
    int command = FLUSHW;
    if (::ioctl(fd->getHandle(), I_FLUSH, &command)) {
      throw PipeException("Unable to flush pipe", this);
    }
  #endif
#endif
}

unsigned int Pipe::read(
  uint8* buffer,
  unsigned int bytesToRead,
  bool nonblocking) throw(PipeException) {
  // TAG: currently always blocks
  // select wait mode with SetNamedPipeHandleState for win32
  bassert(!end, EndOfFile(this));
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::ReadFile(
      fd->getHandle(),
      buffer,
      bytesToRead,
      &result,
      0
    );
    if (!success) { // has error occured
      DWORD error = ::GetLastError();
      if (error == ERROR_BROKEN_PIPE) { // eof
        result = 0;
      } else if (error == ERROR_NO_DATA) { // no data available (only in non-blocking mode)
        return bytesRead;
      } else {
        throw PipeException("Unable to read from pipe", this);
      }
    }
#else // unix
    int result = ::read(
      fd->getHandle(),
      buffer,
      minimum<size_t>(bytesToRead, SSIZE_MAX)
    );
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw PipeException("Unable to read from pipe", this);
      }
    }
#endif
    if (result == 0) { // has end been reached
      end = true;
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

unsigned int Pipe::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  bool nonblocking) throw(PipeException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesToWrite) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::WriteFile(
      fd->getHandle(),
      buffer,
      bytesToWrite,
      &result,
      0
    );
    if (!success) {
      throw PipeException("Unable to write to pipe", this);
    }
#else // unix
    int result = ::write(fd->getHandle(), buffer, minimum<size_t>(bytesToWrite, SSIZE_MAX));
    if (result < 0) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      case EPIPE:
        throw EndOfFile(this);
      default:
        throw PipeException("Unable to write to pipe", this);
      }
    }
#endif
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

void Pipe::wait() const throw(PipeException)
{
  Profiler::WaitTask profilerTask("Pipe::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(fd->getHandle(), INFINITE);
  BASSERT(result == WAIT_OBJECT_0);
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  int result = ::select(fd->getHandle() + 1, &rfds, 0, 0, 0);
  if (result == -1) {
    throw PipeException("Unable to wait for input", this);
  }
#endif
}

bool Pipe::wait(unsigned int timeout) const throw(PipeException)
{
  Profiler::WaitTask profile("Pipe::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(fd->getHandle(), timeout); // FIXME:
  return result == WAIT_OBJECT_0;
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(fd->getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == -1) {
    throw PipeException("Unable to wait for input", this);
  }
  return result; // return true if data available
#endif
}

Pipe::~Pipe()
{
  flush();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
