/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/io/FileDescriptorInputStream.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/Trace.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>

  #ifndef SSIZE_MAX
    #define SSIZE_MAX (1024*1024)
  #endif
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptorInputStream::FileDescriptorInputStream() throw() :
  FileDescriptor(), eof(false) {
}

FileDescriptorInputStream::FileDescriptorInputStream(const FileDescriptor& fd) throw() :
  FileDescriptor(fd), eof(false) {
}

FileDescriptorInputStream::FileDescriptorInputStream(const FileDescriptorInputStream& copy) throw() :
  FileDescriptor(copy), eof(copy.eof) {
}

FileDescriptorInputStream& FileDescriptorInputStream::operator=(const FileDescriptorInputStream& eq) throw() {
  if (&eq != this) { // protect against self assignment
    fd = eq.fd;
    eof = eq.eof;
  }
  return *this;
}

unsigned int FileDescriptorInputStream::available() const throw(IOException) {
#if defined(__win32__)
  // use GetFileSizeEx instead
  DWORD highWord;
  return GetFileSize((void*)getHandle(), &highWord);
#else // __unix__
  // pipes: returns current bytes in buffer
  // files: returns the total size in bytes
  struct stat status;
  if (::fstat(getHandle(), &status) != 0) {
    throw IOException("Unable to get status of file descriptor");
  }
  return status.st_size;
#endif
}

unsigned int FileDescriptorInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  if (eof) {
    throw EndOfFile();
  }
#if defined(__win32__)
  DWORD bytesRead;
  BOOL success = ReadFile((void*)fd->getHandle(), buffer, size, &bytesRead, NULL);
  if (success) {
    if (bytesRead == 0) { // has end of file been reached
      eof = true; // remember end of file
      return 0;
    }
    return bytesRead;
  } else { // error occured
    throw IOException("Unable to read from file descriptor");
  }
#else // __unix__
  int result = ::read(getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
  if (result == 0) { // has end of file been reached
    eof = true; // remember end of file
    return 0; // return
  } else if (result < 0) { // has an error occured
    switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
    case EINTR: // interrupted by signal before any data was read
      return 0; // try later
    case EAGAIN: // no data available (only in non-blocking mode)
      return 0; // try later
    default:
      throw IOException("Unable to read from pipe");
    }
  }
  return result;
#endif
}

unsigned int FileDescriptorInputStream::skip(unsigned int count) throw(IOException) {
  unsigned int temp = count;
  Allocator<char>* buffer = Thread::getLocalStorage();
  while (temp) {
    temp -= read(buffer->getElements(), buffer->getSize() <? temp);
  }
  return count - temp;
}

void FileDescriptorInputStream::setNonBlocking(bool value) throw(IOException) {
#if defined(__win32__)
#else // __unix__
  int flags = getFlags();
  if (value) {
    if (flags & O_NONBLOCK == 0) { // do we need to set flag
      setFlags(flags | O_NONBLOCK);
    }
  } else {
    if (flags & O_NONBLOCK != 0) { // do we need to clear flag
      setFlags(flags & ~O_NONBLOCK);
    }
  }
#endif
}

void FileDescriptorInputStream::wait() const throw(IOException) {
#if defined(__win32__)
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  int result = ::select(fd->getHandle() + 1, &rfds, NULL, NULL, NULL);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
#endif
}

bool FileDescriptorInputStream::wait(unsigned int timeout) const throw(IOException) {
#if defined(__win32__)
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(fd->getHandle() + 1, &rfds, NULL, NULL, &tv);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
  return result; // return true if data available
#endif
}

FileDescriptorInputStream::~FileDescriptorInputStream() {
  TRACE_MEMBER();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorInputStream& value) {
  return stream << "class/FileDescriptorInputStream{"
                << "eof=" << value.eof << ";"
                << "handle=" << value.fd->getHandle()
                << "}";
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
