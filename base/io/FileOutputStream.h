/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H

#include <base/io/OutputStream.h>
#include <base/io/File.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  File output stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FileOutputStream : public OutputStream {
public:

  /** The flags. */
  enum {CREATE = 0x01, TRUNCATE = 0x02, APPEND = 0x04, NONBLOCK = 0x08, SYNC = 0x10};
  /** Permissions. */
  enum {RWXU = 0x700, RUSR = 0x400, WUSR = 0x200, XUSR = 0x100, RWXG = 0x70, RGRP = 0x40, WGRP = 0x20, XGRP = 0x10, RWXO = 0x7, ROTH = 0x4, WOTH = 0x2, XOTH = 0x1};
private:

  /** The file. */
  File file;
public:

  /**
    Initializes the file output stream.

    @param path The name of the file.
    @param options The flags used to open the file (CREATE, TRUNCATE, APPEND, NONBLOCK, and SYNC).
    @param permissions Specifies the permissions to be used if the file is created.
  */
  FileOutputStream(const String& path, unsigned int options, unsigned int permissions = 0x640) throw(FileNotFound);

  FileOutputStream(const FileOutputStream& copy) throw();

  FileOutputStream& operator=(const FileOutputStream& eq) throw();

  /**
    Forces any buffered bytes to be written out.
  */
  inline void flush() throw(IOException) {file.flush();}

  inline unsigned int write(char* buffer, unsigned int count, bool nonblocking = false) throw(IOException) {
    file.write(buffer, count, nonblocking);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
