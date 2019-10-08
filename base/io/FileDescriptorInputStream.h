/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/io/InputStream.h>
#include <base/io/FileDescriptor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Input stream where source is a file descriptor.

  @short File descriptor input stream.
  @ingroup io
  @version 1.0
*/

class FileDescriptorInputStream : public virtual InputStream, public FileDescriptor {
private:

  /** Specifies that the end of the stream has been reached. */
  bool end = false;
public:
  
  /**
    Initializes file descriptor input stream as invalid.
  */
  FileDescriptorInputStream() throw();

  /**
    Initializes file descriptor input stream from a file descriptor.

    @param fd The file descriptor.
  */
  FileDescriptorInputStream(const FileDescriptor& fd) throw();

  /**
    Initializes file descriptor input stream from other file descriptor input stream.
  */
  FileDescriptorInputStream(const FileDescriptorInputStream& copy) throw();

  /**
    Assignment of file descriptor input stream to this file descriptor input stream.
  */
  FileDescriptorInputStream& operator=(const FileDescriptorInputStream& eq) throw();

  /**
    Closes the stream and releases any system resources associated with the
    stream.
  */
  void close() throw(IOException) {
    FileDescriptor::close();
  }

  /**
    Returns true if the end of the file descriptor has been reached.
  */
  inline bool atEnd() const throw(IOException) {
    return end;
  }

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Fills the buffer with bytes from the stream. Blocks if asked to read more
    bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
    @return The actual number of bytes read.
  */
  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than
    available.

    @param count The number of bytes to skip.
  */
  unsigned int skip(unsigned int count) throw(IOException);

  /**
    Sets the non-blocking flag of the file descriptor.
  */
  void setNonBlocking(bool value) throw(IOException);

  /**
    Blocking wait for input to become available.
  */
  void wait() const throw(IOException);

  /**
    Blocking wait for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int timeout) const throw(IOException);

  /**
    Releases the file descriptor.
  */
  ~FileDescriptorInputStream();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
