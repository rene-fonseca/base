/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_OUTPUT_STREAM_H

#include "OutputStream.h"
#include "FileDescriptor.h"

/**
  The FileDescriptorOutputStream class serves as a general handle to a sink of
  bytes within the operatingsystem (e.g. file, socket and pipe). This class is
  normally not used directly by the application.

  @author René Møller Fonseca
  @version 1.0
*/

class FileDescriptorOutputStream : public OutputStream, public virtual FileDescriptor {
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
  FileDescriptorOutputStream(FileDescriptorOutputStream& copy) throw();

  /**
    Assignment of file descriptor output stream to this file descriptor output stream.
  */
  FileDescriptorOutputStream& operator=(FileDescriptorOutputStream& eq) throw();

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
  unsigned int write(const char* buffer, unsigned int size) throw(IOException);

  /**
    Writes a string representation of a FileDescriptorOutputStream object to a format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorOutputStream& value);
};

/**
  Writes a string representation of a FileDescriptorOutputStream object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorOutputStream& value);

#endif
