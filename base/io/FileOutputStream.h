/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H

#include "base/Object.h"
#include "OutputStream.h"
#include "FileNotFound.h"
#include "FileDescriptor.h"
#include "BindException.h"

/**
  File output stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FileOutputStream: public Object, public OutputStream {
protected:

  /** File descriptor. */
  FileDescriptor* fd;
public:

  /**
    Initializes the file output stream.

    @param name The name of the file.
    @param options (CREATE, REPLACE, APPEND, NONBLOCK).
  */
  FileOutputStream(const char* name) throw(FileNotFound);

  /**
    Initializes the file input stream with file descriptor as source of stream.

    @param handle The file descriptor.
  */
  FileOutputStream(int handle) throw(BindException);

  /**
    Closes the output stream and releases any system resources associated with the stream.
  */
  void close() throw(IOException);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException);

  /**
    Writes a single value to the stream.

    @param value The value to be written.
  */
  void write(char value) throw(IOException);

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
  */
  void write(const char* buffer, unsigned int size) throw(IOException);

  /**
    Destroys the file output stream.
  */
  ~FileOutputStream();
};

#endif
