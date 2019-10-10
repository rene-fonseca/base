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

#include <base/io/OutputStream.h>
#include <base/io/FileDescriptor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The FileDescriptorOutputStream class serves as a general handle to a sink of
  bytes within the operatingsystem (e.g. file, socket and pipe). This class is
  normally not used directly by the application.

  @short Filedescriptor output stream.
  @ingroup io
  @version 1.0
*/

class FileDescriptorOutputStream : public virtual OutputStream,
                                   public FileDescriptor {
public:

  /**
    Initializes file descriptor output stream as invalid.
  */
  FileDescriptorOutputStream() throw();

  /**
    Initializes file descriptor output stream from a file descriptor.

    @param fd The file descriptor.
  */
  FileDescriptorOutputStream(const FileDescriptor& fd) throw();

  /**
    Initializes file descriptor output stream from other file descriptor output stream.
  */
  FileDescriptorOutputStream(const FileDescriptorOutputStream& copy) throw();

  /**
    Assignment of file descriptor output stream to this file descriptor output
    stream.
  */
  FileDescriptorOutputStream& operator=(const FileDescriptorOutputStream& eq) throw();

  /**
    Closes the stream and releases any system resources associated with the
    stream.
  */
  void close() throw(IOException) {
    FileDescriptor::close();
  }

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException);

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false) throw(IOException);

  /**
    Releases the file descriptor.
  */
  ~FileDescriptorOutputStream();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
