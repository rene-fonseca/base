/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileDescriptorOutputStream.h>
#include <base/io/EndOfFile.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <errno.h>

  #ifndef SSIZE_MAX
    #define SSIZE_MAX (1024*1024)
  #endif
#endif

FileDescriptorOutputStream::FileDescriptorOutputStream() throw() :
  FileDescriptor() {
}

FileDescriptorOutputStream::FileDescriptorOutputStream(const FileDescriptor& fd) throw() :
  FileDescriptor(fd) {
}

FileDescriptorOutputStream::FileDescriptorOutputStream(const FileDescriptorOutputStream& copy) throw() :
  FileDescriptor(copy) {
}

FileDescriptorOutputStream& FileDescriptorOutputStream::operator=(const FileDescriptorOutputStream& eq) throw() {
  if (&eq != this) { // protect against self assignment
    fd = eq.fd;
  }
  return *this;
}

void FileDescriptorOutputStream::flush() throw(IOException) {
#if defined(__win32__)
  // Handle to a console output cannot be flushed 'cause it isn't buffered, aarrgh
  if (!isValid()) {
    throw IOException("Unable to flush file descriptor");
  }
  FlushFileBuffers((void*)fd->getHandle()); // yes ignore any error
#else // __unix__
  fsync(fd->getHandle());
//  if (ioctl(fd->getHandle(), I_FLUSH, FLUSHRW) != 0) {
//    throw IOException("Unable to flush stream");
//  }
#endif
}

unsigned int FileDescriptorOutputStream::write(const char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesWritten = 0;

#if defined(__win32__)
  while (totalBytesWritten < size) {
    unsigned int bytesWritten;
    int test = fd->getHandle();
    bool success = WriteFile((void*)fd->getHandle(), &buffer[totalBytesWritten], size - totalBytesWritten, (DWORD*)&bytesWritten, NULL);

    if (success) {
      if (bytesWritten == 0) {
        return totalBytesWritten;
      }
      totalBytesWritten += bytesWritten;
    } else { // error occured
      DWORD errcode = GetLastError();
      throw IOException("Unable to write to file descriptor");
    }
  }
#else // __unix__
  while (totalBytesWritten < size) {
    int result = ::write(fd->getHandle(), &buffer[totalBytesWritten], (size - totalBytesWritten) % SSIZE_MAX);

    if (result == 0) {
      // do not know if it is possible to end up here
      return totalBytesWritten;
    } else if (result == -1) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        // try again
        break;
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
        return totalBytesWritten; // early return
        break;
      default:
        throw IOException("Unable to write to file descriptor");
      }
    } else {
      totalBytesWritten += result;
    }
  }
#endif
  return totalBytesWritten;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorOutputStream& value) {
  return stream << "class/FileDescriptorOutputStream{"
                << "handle=" << value.fd->getHandle()
                << "}";
}
