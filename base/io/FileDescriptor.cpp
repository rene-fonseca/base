/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileDescriptor.h"
#include "EndOfFile.h"
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef SSIZE_MAX
#define SSIZE_MAX (1024*1024)
#endif

FileDescriptor::FileDescriptor(int handle) {
  this->handle = handle;
  eof = false;
}

unsigned int FileDescriptor::available() throw(IOException) {
  // use poll instead?

  // watch fd to see if it has input
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(handle, &rfds);

  // do not block
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  int result = select(1, &rfds, NULL, NULL, &tv);
  // signal could have happened.

  if (result == 1) {
    return 1; // has much data is available???
    // data is available now
    // FD_ISSET(0, &rfds) will be true
  } else {
    return 0;
  }
}

void FileDescriptor::close() throw(IOException) {
  if (::close(handle) != 0) {
    throw IOException("Unable to close file descriptor");
  }
}

int FileDescriptor::read() throw(IOException) {
  char buffer;
  unsigned int result = read(&buffer, sizeof(buffer));
  if (result == 0) {
    return -1; // EOF or non-blocking read
  } else {
    return buffer;
  }
}

unsigned int FileDescriptor::read(char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesRead = 0;

  while (totalBytesRead < size) {
    int result = ::read(handle, &buffer[totalBytesRead], (size - totalBytesRead) % SSIZE_MAX);

    if (result == 0) { // has end of file been reached
      if (eof) {
        throw EndOfFile("Attempt to read beyond end of file");
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
        throw IOException("Unable to read from file decriptor");
      }
    } else {
      totalBytesRead += result;
    }
  }

  return totalBytesRead;
}

unsigned int FileDescriptor::write(char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesWritten = 0;

  while (totalBytesWritten < size) {
    int result = ::write(handle, &buffer[totalBytesWritten], (size - totalBytesWritten) % SSIZE_MAX);

    if (result == 0) {
      // is it possible to end up here? can result in infinite loop
    } else if (result == -1) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        // try again
        break;
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
        return totalBytesWritten; // early return
        break;
      default:
        throw IOException("Unable to write to file decriptor");
      }
    } else {
      totalBytesWritten += result;
    }
  }

  return totalBytesWritten;
}

FileDescriptor::~FileDescriptor() {
}
