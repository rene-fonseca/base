/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_FILE_OUTPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_FILE_OUTPUT_STREAM_H

#include "base/Object.h"
#include "OutputStream.h"
#include "FileNotFound.h"
#include "FileDescriptor.h"

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
    Destroys the file output stream.
  */
  ~FileOutputStream();
};

#endif
