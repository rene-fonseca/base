/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/security/User.h>
#include <base/security/Group.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  File information wrapper class.

  @short File information
  @ingroup filesystem
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class FileInfo : public Object {
public:
  
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
  /** The owner of the file. */
  User owner;
  /** The group associated with the file. */
  Group group;
  /** The mode of the file. */
  unsigned int mode;
  /** Number of links to the file. */
  unsigned int links;
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
  long long getSize() const throw();
  
  /**
    Returns the owner of the file.
  */
  const User& getOwner() const throw() {
    return owner;
  }
  
  /**
    Returns the group of the file.
  */
  const Group& getGroup() const throw() {
    return group;
  }

  /**
    Returns the mode of the file.

    @return The maximum possible value if mode isn't supported.
  */
  inline unsigned int getMode() const throw() {
    return mode;
  }

  /**
    Returns the number of links to the file.

    @return 0 is not supported.
  */
  inline unsigned int getLinks() const throw() {
    return links;
  }
  
  /**
    Returns the last modification time of the file.
  */
  Date getLastModification() const throw();
  
  /**
    Returns the last access time of the file.
  */
  Date getLastAccess() const throw();
  
  /**
    Returns the last chnage time of the file.
  */
  Date getLastChange() const throw();
  
  /**
    Returns the path of the file.
  */
  String getPath() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
