/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H

#include "OutputStream.h"
#include "FileNotFound.h"
#include "FileDescriptor.h"
#include "BindException.h"
#include "base/string/String.h"

/**
  File output stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FileOutputStream : public OutputStream, protected FileDescriptor {
public:

  /** The flags. */
  enum {CREATE = 0x01, TRUNCATE = 0x02, APPEND = 0x04, NONBLOCK = 0x08, SYNC = 0x10};
  /** Permissions. */
  enum {RWXU = 0x700, RUSR = 0x400, WUSR = 0x200, XUSR = 0x100, RWXG = 0x70, RGRP = 0x40, WGRP = 0x20, XGRP = 0x10, RWXO = 0x7, ROTH = 0x4, WOTH = 0x2, XOTH = 0x1};

private:

  /** The path of the file. */
  String path;
public:

  /**
    Initializes the file output stream.

    @param path The name of the file.
    @param flags The flags used to open the file (CREATE, TRUNCATE, APPEND, NONBLOCK, and SYNC).
    @param permissions Specifies the permissions to be used if the file is created.
  */
  FileOutputStream(const String& path, unsigned int flags, unsigned int permissions = 0x640) throw(FileNotFound);

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
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
  */
  unsigned int write(const char* buffer, unsigned int size) throw(IOException);

  /**
    Destroys the file output stream.
  */
  ~FileOutputStream();
};

#endif
