/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FILE_INFO_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FILE_INFO_H

#include <base/Object.h>
#include <base/filesystem/FileSystemException.h>
#include <base/string/String.h>
#include <base/Date.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  File information wrapper class.

  @short File information
  @ingroup filesystem
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileInfo : public Object {
private:

  /** The path of the file. */
  String path;
  /** The size of the file in bytes. */
  long long size;
  /** The last modification time. */
  Date modification;
  /** The last access time. */
  Date access;
  /** The last change time. */
  Date change;
public:

  /**
    Initializes a file.

    @param path the path of the file.
  */
  FileInfo(const String& path) throw(FileSystemException);
/*
  use flock
  void lock() throw(FileException);
  void unlock() throw(FileException);
  bool tryLock() throw(FileException);
*/
  /**
    Returns the size of the file in bytes.
  */
  long long getSize() throw();

  /**
    Returns the last modification time of the file.
  */
  Date getLastModification() throw();

  /**
    Returns the last access time of the file.
  */
  Date getLastAccess() throw();

  /**
    Returns the last chnage time of the file.
  */
  Date getLastChange() throw();

  /**
    Returns the path of the file.
  */
  String getPath() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
