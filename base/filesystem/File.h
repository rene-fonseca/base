/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FILE_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FILE_H

#include <base/Object.h>
#include <base/filesystem/FileException.h>
#include <base/string/String.h>
#include <base/Date.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  File.

  @author René Møller Fonseca
  @version 1.0
*/

class File : public Object {
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
  File(const String& path) throw(FileException);
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
