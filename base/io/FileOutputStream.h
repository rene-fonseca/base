/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H

#include <base/io/FileDescriptorOutputStream.h>
#include <base/io/FileNotFound.h>
#include <base/io/BindException.h>
#include <base/string/String.h>

/**
  File output stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FileOutputStream : public FileDescriptorOutputStream {
public:

  /** The flags. */
  enum {CREATE = 0x01, TRUNCATE = 0x02, APPEND = 0x04, NONBLOCK = 0x08, SYNC = 0x10};
  /** Permissions. */
  enum {RWXU = 0x700, RUSR = 0x400, WUSR = 0x200, XUSR = 0x100, RWXG = 0x70, RGRP = 0x40, WGRP = 0x20, XGRP = 0x10, RWXO = 0x7, ROTH = 0x4, WOTH = 0x2, XOTH = 0x1};

private:

  /** The path of the file. */
  String<> path;
public:

  /**
    Initializes the file output stream.

    @param path The name of the file.
    @param flags The flags used to open the file (CREATE, TRUNCATE, APPEND, NONBLOCK, and SYNC).
    @param permissions Specifies the permissions to be used if the file is created.
  */
  FileOutputStream(const String<>& path, unsigned int flags, unsigned int permissions = 0x640) throw(FileNotFound);

  /**
    Returns the path of the file.
  */
  const String<>& getPath() const throw();

  /**
    Writes a string representation of a FileOutputStream object to a stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileOutputStream& value);
};

/**
  Writes a string representation of a FileOutputStream object to a stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileOutputStream& value);

#endif
