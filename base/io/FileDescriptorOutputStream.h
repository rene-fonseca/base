/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_OUTPUT_STREAM_H

#include <base/io/OutputStream.h>
#include <base/io/FileDescriptor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The FileDescriptorOutputStream class serves as a general handle to a sink of
  bytes within the operatingsystem (e.g. file, socket and pipe). This class is
  normally not used directly by the application.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileDescriptorOutputStream : public virtual OutputStream, public FileDescriptor {
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
    Assignment of file descriptor output stream to this file descriptor output stream.
  */
  FileDescriptorOutputStream& operator=(const FileDescriptorOutputStream& eq) throw();

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
  unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(IOException);

  /**
    Release file descriptor.
  */
  ~FileDescriptorOutputStream();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
