/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_INPUT_STREAM_H

#include <base/io/InputStream.h>
#include <base/io/FileDescriptor.h>

/**
  Input stream where source is a file descriptor.

  @author René Møller Fonseca
  @version 1.0
*/

class FileDescriptorInputStream : public virtual InputStream, public FileDescriptor {
private:

  /** Specifies that the end of the stream has been reached. */
  bool eof;
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
    Returns true if the end of the file descriptor has been reached.
  */
  inline bool atEnd() const throw(IOException) {return eof;}

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Fills the buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
    @return The actual number of bytes read.
  */
  unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
  */
  unsigned int skip(unsigned int count) throw(IOException);

  /**
    Sets the non-blocking flag of the file descriptor.
  */
  void setNonBlocking(bool value) throw(IOException);

  /**
    Writes a string representation of a FileDescriptorInputStream object to a format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorInputStream& value);
};

/**
  Writes a string representation of a FileDescriptorInputStream object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorInputStream& value);

#endif
