/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_OUTPUT_STREAM_H

#include <base/io/OutputStream.h>
#include <base/io/File.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Output stream with a file as the destination.

  @short File output stream.
  @ingroup io
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileOutputStream : public virtual Object, public OutputStream {
public:

  /** File initialization flags. */
  enum Options {
    CREATE = File::CREATE, /**< Specifies that the file should be created if it doesn't exist. */
    TRUNCATE = File::TRUNCATE, /**< Specifies that the file should be truncated if it already exists. */
    EXCLUSIVE = File::EXCLUSIVE, /**< Specifies that the file should be opened in exclusive mode. */
    ASYNCHRONOUS = File::ASYNCHRONOUS, /**< Specifies that the file should be opened for asynchronous IO (disables the synchronous IO interface). */
    APPEND = File::APPEND /**< Specifies that data should be appended to the file. */
  };

  // TAG: fix flags
  // NONBLOCK = 0x08, /**< Specifies that the file should be opened in non-blocking mode. */
  // SYNC = 0x10 /**< Specifies that the file should be opened in synchronous mode. */

  /** Permissions. */
  enum Permissions {
    RWXU = 0x700, /**< Read, write, and execute permissions for the owner of the file. */
    RUSR = 0x400, /**< Read permission for the owner of the file. */
    WUSR = 0x200, /**< Write permission for the owner of the file. */
    XUSR = 0x100, /**< Execute permission for the owner of the file. */
    RWXG = 0x70, /**< Read, write, and execute permissions for the group associated with the file. */
    RGRP = 0x40, /**< Read permission for the group associated with the file. */
    WGRP = 0x20, /**< Write permission for the group associated with the file. */
    XGRP = 0x10, /**< Execute permission for the group associated with the file. */
    RWXO = 0x7, /**< Read, write, and execute permissions for other users. */
    ROTH = 0x4, /**< Read permission for other users. */
    WOTH = 0x2, /**< Write permission for other users. */
    XOTH = 0x1 /**< Execute permission for other users. */
  };
private:

  /** The file. */
  File file;
public:

  /**
    Initializes the file output stream.

    @param path The name of the file.
    @param options The flags used to open the file (CREATE, TRUNCATE, APPEND,
    NONBLOCK, and SYNC).
    @param permissions Specifies the permissions to be used if the file is
    created.
  */
  FileOutputStream(
    const String& path,
    unsigned int options,
    unsigned int permissions = 0x640) throw(FileNotFound);

  FileOutputStream(const FileOutputStream& copy) throw();

  FileOutputStream& operator=(const FileOutputStream& eq) throw();

  void close() throw(IOException);

  /**
    Forces any buffered bytes to be written out.
  */
  inline void flush() throw(IOException) {
    file.flush();
  }

  inline unsigned int write(
    const uint8* buffer,
    unsigned int count,
    bool nonblocking = false) throw(IOException) {
    return file.write(buffer, count, nonblocking);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
