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
#include <base/io/PipeException.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/ResourceHandle.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/types.h>
#  include <sys/time.h> // defines timeval on Linux systems
#  include <sys/stat.h>
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR)
#  include <sys/ioctl.h>
#endif
#  include <limits.h> // defines PIPE_BUF...
#  include <unistd.h>
#  include <errno.h>
#  include <string.h> // required on solaris 'cause FD_ZERO uses memset
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
#    warning ioctl is not supported (CYGWIN)
#  elif (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__MACOS)
#    if defined(__has_include)
#      if __has_include(<stropts.h>)
#        include <stropts.h> // defines FLUSH macros
#        define _COM_AZURE_DEV__BASE__USE_FLUSH
#      endif
#    endif
#  endif

#  if !defined(SSIZE_MAX)
#    define SSIZE_MAX (1024*1024)
#  endif

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS)
namespace {
  int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds,
             struct timeval *restrict timeout)
  {
    return EINVAL;
  }
}
#else
#  include <sys/select.h>
#endif

#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class PipeHandle : public ResourceHandle {
public:
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle = INVALID_HANDLE_VALUE;
#else
  int handle = -1;
#endif
  /** Specifies that the end has been reached. */
  bool end = false;

  void close()
  {
    if (end) {
      return;
    }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (::CloseHandle(handle)) {
      _throw PipeException("Unable to close pipe.", this);
    }
    handle = INVALID_HANDLE_VALUE;
#else // unix
    if (::close(handle)) {
      _throw PipeException("Unable to close pipe.", this);
    }
    handle = -1;
#endif
    end = true;
  }

  ~PipeHandle()
  {
    close();
  }
};

namespace {

  inline PipeHandle* toPipeHandle(const AnyReference& handle)
  {
    if (handle) { // TAG: we do not want a dynamic cast - use static cast
      return handle.cast<PipeHandle>().getValue();
    }
    return nullptr;
  }
}

Pair<Pipe, Pipe> Pipe::make()
{
  Pipe p;
  Pipe q;

  Profiler::ResourceCreateTask iprofile("Pipe::make()");
  Profiler::ResourceCreateTask oprofile("Pipe::make()");

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
  bassert(ihandle != OperatingSystem::INVALID_HANDLE, IOException("Unable to make pipes.", Type::getType<Pipe>()));
  bassert(ohandle != OperatingSystem::INVALID_HANDLE, IOException("Unable to make pipes.", Type::getType<Pipe>()));

  Reference<PipeHandle> _ihandle = new PipeHandle();
  Reference<PipeHandle> _ohandle = new PipeHandle();
  _ihandle->handle = ihandle;
  _ohandle->handle = ohandle;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
  return Pair<Pipe, Pipe>(p, q);
#else // unix
  OperatingSystem::Handle handles[2];
  if (::pipe(handles)) {
    _throw PipeException("Unable to create pipe.", Type::getType<Pipe>());
  }

  Reference<PipeHandle> _ihandle = new PipeHandle();
  Reference<PipeHandle> _ohandle = new PipeHandle();
  _ihandle->handle = handles[0];
  _ohandle->handle = handles[1];
#endif // flavor

  iprofile.setHandle(*_ihandle);
  oprofile.setHandle(*_ohandle);
  return makePair<Pipe, Pipe>(p, q);
}



Pipe::Pipe() noexcept
{
}

void Pipe::close()
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  handle->close();
}

unsigned int Pipe::getBufferSize() const
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = 0;
  GetNamedPipeInfo(handle->handle, 0, &result, 0, 0); // TAG: separate input and output buffer sizes
  return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
  return 0;
#else // unix
  return PIPE_BUF;
#endif
}

bool Pipe::atEnd() const
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  return handle->end;
}

unsigned int Pipe::available() const
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD bytesAvailable = 0;
  if (!::PeekNamedPipe(handle->handle, 0, 0, 0, &bytesAvailable, 0)) {
    _throw PipeException("Unable to get available bytes.", this);
  }
  return bytesAvailable;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = fstat64(handle->handle, &status);
  #else
    struct stat status;
    int result = ::fstat(handle->handle, &status);
  #endif // LFS
    bassert(result == 0, IOException("Unable to get available bytes.", this));
    return (unsigned int)status.st_size;
#endif
}

unsigned int Pipe::skip(unsigned int count)
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  unsigned int bytesSkipped = 0;
  while (bytesSkipped < count) {
    MemorySize bytesToRead = minimum<MemorySize>(count - bytesSkipped, buffer.getSize());
    bytesSkipped += read(buffer.getElements(), (unsigned int)bytesToRead);
  }
  return bytesSkipped;
}

void Pipe::flush()
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Profiler::IOFlushTask profile("Pipe::flush()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::FlushFileBuffers(handle->handle)) {
    _throw PipeException("Unable to flush pipe.", this);
  }
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
//    #warning Pipe::flush() not supported (CYGWIN)
  #elif defined(_COM_AZURE_DEV__BASE__USE_FLUSH)
    int command = FLUSHW;
    if (::ioctl(handle->handle, I_FLUSH, &command)) {
      _throw PipeException("Unable to flush pipe.", this);
    }
  #endif
#endif
}

unsigned int Pipe::read(
  uint8* buffer,
  unsigned int bytesToRead,
  bool nonblocking)
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Profiler::IOReadTask profile("Pipe::read()", buffer);

  // TAG: currently always blocks
  // select wait mode with SetNamedPipeHandleState for win32
  bassert(!handle->end, EndOfFile(this));
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::ReadFile(
      handle->handle,
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
        _throw PipeException("Unable to read from pipe.", this);
      }
    }
#else // unix
    int result = (int)::read(
      handle->handle,
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
        _throw PipeException("Unable to read from pipe.", this);
      }
    }
#endif
    if (result == 0) { // has end been reached
      handle->end = true;
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

unsigned int Pipe::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  bool nonblocking)
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Profiler::IOWriteTask profile("Pipe::write()", buffer);

  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesToWrite) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::WriteFile(
      handle->handle,
      buffer,
      bytesToWrite,
      &result,
      0
    );
    if (!success) {
      _throw PipeException("Unable to write to pipe.", this);
    }
#else // unix
    int result = (int)::write(handle->handle, buffer, minimum<size_t>(bytesToWrite, SSIZE_MAX));
    if (result < 0) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      case EPIPE:
        _throw EndOfFile(this);
      default:
        _throw PipeException("Unable to write to pipe.", this);
      }
    }
#endif
    profile.onBytesWritten(result);
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

void Pipe::wait() const
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Profiler::WaitTask profilerTask("Pipe::wait()", handle);
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle->handle, INFINITE);
  BASSERT(result == WAIT_OBJECT_0);
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(handle->handle, &rfds);

  int result = ::select(handle->handle + 1, &rfds, 0, 0, 0);
  if (result == -1) {
    _throw PipeException("Unable to wait for input.", this);
  }
#endif
}

bool Pipe::wait(unsigned int timeout) const
{
  PipeHandle* handle = toPipeHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Profiler::WaitTask profile("Pipe::wait()", handle);
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle->handle, timeout);
  return result == WAIT_OBJECT_0;
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(handle->handle, &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(handle->handle + 1, &rfds, 0, 0, &tv);
  if (result == -1) {
    _throw PipeException("Unable to wait for input.", this);
  }
  return result; // return true if data available
#endif
}

Pipe::~Pipe()
{
  flush();
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Pipe) : public UnitTest {
public:

  TEST_PRIORITY(500);
  TEST_PROJECT("base/io");
  TEST_IMPACT(NORMAL);
  TEST_EXTERNAL();

  void run() override
  {
    Pipe p1;
  }
};

TEST_REGISTER(Pipe);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
