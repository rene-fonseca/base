/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/string/String.h>
#include <base/Date.h>
#include <base/security/User.h>
#include <base/security/Group.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  File information wrapper class.

  @short File information
  @ingroup filesystem
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API FileInfo : public Object {
public:

  /** Sticky bit. */
  enum Sticky {
    SET_UID = 0x4000, /**< Set user id on execution. */
    SET_GID = 0x2000, /**< Set group id on execution. */
    RESTRICT = 0x1000, /**< Restrict flag. */
    STICKY_MASK = SET_UID|SET_GID|RESTRICT /**< All sticky bits. */
  };
  
  /** Permissions. */
  enum Permissions {
    RUSR = 0x400, /**< Read permission for the owner of the file. */
    WUSR = 0x200, /**< Write permission for the owner of the file. */
    XUSR = 0x100, /**< Execute permission for the owner of the file. */
    /** Read, write, and execute permissions for the owner of the file. */
    RWXU = RUSR|WUSR|XUSR,
    /** Read permission for the group associated with the file. */
    RGRP = 0x40,
    /** Write permission for the group associated with the file. */
    WGRP = 0x20,
    /** Execute permission for the group associated with the file. */
    XGRP = 0x10,
    /**
      Read, write, and execute permissions for the group associated with the
      file.
    */
    RWXG = RGRP|WGRP|XGRP,
    ROTH = 0x4, /**< Read permission for other users. */
    WOTH = 0x2, /**< Write permission for other users. */
    XOTH = 0x1, /**< Execute permission for other users. */
    /** Read, write, and execute permissions for other users. */
    RWXO = ROTH|WOTH|XOTH,
    PERMISSION_MASK = RWXU|RWXG|RWXO /**< Any access. */
  };
private:

  /** The path of the file. */
  String path;
  /** The size of the file in bytes. */
  long long size = 0;
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
  unsigned int mode = 0;
  /** Number of links to the file. */
  unsigned int links = 0;
public:

  /**
    Initializes a file.

    @param path the path of the file.
  */
  FileInfo(const String& path);
  
  /**
    Returns the size of the file in bytes.
  */
  inline long long getSize() const noexcept
  {
    return size;
  }
  
  /**
    Returns the owner of the file.
  */
  inline const User& getOwner() const noexcept
  {
    return owner;
  }
  
  /**
    Returns the group of the file.
  */
  inline const Group& getGroup() const noexcept
  {
    return group;
  }

  /**
    Returns the mode of the file.

    @return The maximum possible value if mode isn't supported.
  */
  inline unsigned int getMode() const noexcept
  {
    return mode;
  }

  /**
    Returns the number of links to the file.

    @return 0 is not supported.
  */
  inline unsigned int getLinks() const noexcept
  {
    return links;
  }
  
  /**
    Returns the last modification time of the file.
  */
  inline const Date& getLastModification() const noexcept
  {
    return modification;
  }
  
  /**
    Returns the last access time of the file.
  */
  inline const Date& getLastAccess() const noexcept
  {
    return access;
  }
  
  /**
    Returns the last chnage time of the file.
  */
  inline const Date& getLastChange() const noexcept
  {
    return change;
  }
  
  /**
    Returns the path of the file.
  */
  inline const String& getPath() const noexcept
  {
    return path;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
