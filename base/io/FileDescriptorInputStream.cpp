/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileDescriptorInputStream.h>
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

FileDescriptorInputStream::FileDescriptorInputStream() throw() :
  FileDescriptor(), eof(true) {
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
  unsigned int totalBytesRead = 0;

#if defined(__win32__)
  while (totalBytesRead < size) {
    DWORD bytesRead;
    BOOL success = ReadFile((void*)fd->getHandle(), &buffer[totalBytesRead], size - totalBytesRead, &bytesRead, NULL);

    if (success) {
      if (bytesRead == 0) {
        if (eof) {
          throw EndOfFile();
        }
        eof = true; // remember end of file
        return totalBytesRead;
      }
      totalBytesRead += bytesRead;
    } else { // error occured
      throw IOException("Unable to read from file descriptor");
    }
  }
#else // __unix__
  while (totalBytesRead < size) {
    int result = ::read(getHandle(), &buffer[totalBytesRead], (size - totalBytesRead) % SSIZE_MAX);

    if (result == 0) { // has end of file been reached
      if (eof) {
        throw EndOfFile();
      }
      eof = true; // remember end of file
      return totalBytesRead; // early return
    } else if (result == -1) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        // try again
        break;
      case EAGAIN: // no data available (only in non-blocking mode)
        return totalBytesRead; // early return
      default:
        throw IOException("Unable to read from file descriptor");
      }
    } else {
      totalBytesRead += result;
    }
  }
#endif

  return totalBytesRead;
}

unsigned int FileDescriptorInputStream::skip(unsigned int count) throw(IOException) {
  unsigned int temp = count;
  char buffer[1024]; // USE THREAD LOCAL STORAGE INSTEAD
  while (temp) {
    temp -= read((char*)&buffer, sizeof(buffer) <? temp);
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

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorInputStream& value) {
  return stream << "class/FileDescriptorInputStream{"
                << "eof=" << value.eof << ";"
                << "handle=" << value.fd->getHandle()
                << "}";
}
