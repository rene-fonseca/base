/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_H
#define _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_H

#include "base/Object.h"
#include "IOException.h"

/**
  The FileDescriptor class serves as a general handle to a source or sink of bytes within the operatingsystem (e.g. file, socket and pipe). This class is normally not used directly by the application.

  @author René Møller Fonseca
  @version 1.0
*/
class FileDescriptor : public Object {
protected:

  /** Handle to file descriptor. */
  int handle;
  /** Specifies that the end of file has been reached. */
  bool eof;
public:

  /**
    Initializes the file descriptor.

    @param handle Handle to file descriptor.
  */
  FileDescriptor(int handle);

  unsigned int available() throw(IOException);

  /**
    Synchronizes a file's in-core data with that on disk. ??????????? socket???
  */
  void flush() throw(IOException);

  /**
    Closes the file descriptor.
  */
  void close() throw(IOException);

  int read() throw(IOException);

  /**
    @return The actual number of bytes read.
  */
  unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
  */
  unsigned int write(char* buffer, unsigned int size) throw(IOException);

  /**
    Destroys the file descriptor.
  */
  ~FileDescriptor();
};

#endif
