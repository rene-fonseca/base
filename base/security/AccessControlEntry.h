/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__ACCESS_CONTROL_ENTRY_H
#define _DK_SDU_MIP__BASE_SECURITY__ACCESS_CONTROL_ENTRY_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/string/FormatOutputStream.h>
#include <base/collection/Array.h>
#include <base/collection/InvalidKey.h>
#include <base/security/Trustee.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  An entry within an Access Control List (ACL) describing the access rights for
  a single trustee.
  
  @ingroup security
  @short Entry of access control list (ACL).
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AccessControlEntry : public Object {
public:

  /** The permissions. */
  enum Permission {
    NO = 0, /**< Trustee does not have any access. */
    READ = 1 << 0, /**< Read permission. */
    WRITE = 1 << 1, /**< Write permission. */
    EXECUTE = 1 << 2, /**< Execute permission. */
    
    TRAVERSE_FOLDER = 1 << 3,
    EXECUTE_FILE = TRAVERSE_FOLDER,
    LIST_FOLDER = 1 << 4,
    READ_DATA = LIST_FOLDER,
    READ_ATTRIBUTES = 1 << 5,
    READ_EXTENDED_ATTRIBUTES = 1 << 6,
    
    CREATE_FILES = 1 << 7,
    WRITE_DATA = CREATE_FILES,
    CREATE_FOLDERS = 1 << 8,
    APPEND_DATA = CREATE_FOLDERS,
    WRITE_ATTRIBUTES = 1 << 9,
    WRITE_EXTENDED_ATTRIBUTES = 1 << 10,
    
    DELETE_SUBFOLDERS_AND_FILES = 1 << 11,
    DELETE = 1 << 12,
    READ_PERMISSIONS = 1 << 13,
    CHANGE_PERMISSIONS = 1 << 14,
    TAKE_OWNERSHIP = 1 << 15,
    
    READ_AND_EXECUTE = READ|EXECUTE, // TAG: FIXME: +TRAVERSE_FOLDER EXECUTE_FILE, +LIST_FOLDER/READ_DATA, +READ_ATTRIBUTES, +READ_EXTENDED_ATTRIBUTES, +READ_PERMISSIONS
    MODIFY = READ|WRITE, // TAG: FIXME: -DELETE_SUBFOLDERS_AND_FILES, -CHANGE_PERMISSIONS, -TAKE_OWNERSHIP
    FULL = READ|WRITE|EXECUTE, /**< Trustee has full access. */
  };
  
  /** The access mask. */
  typedef uint64 AccessMask;
  
  /** Access masks. */
  struct Permissions {
    /** The allowed permissions. */
    uint64 allowed;
    /** The denied permissions. */
    uint64 denied;
  };
private:
  
  /** The trustee (e.g. user and group). */
  Trustee trustee;
  /** The permissions. */
  Permissions permissions;
public:
  
  /**
    Initializes ACE as EVERYONE with all permissions denied.
  */
  AccessControlEntry() throw();
  
  /**
    Initializes ACE with specified trustee and no allowed access (and no denied access).
  */
  AccessControlEntry(const Trustee& trustee) throw();
  
  /**
    Initializes access control entry (ACE) with the specified allowed (and no
    denied) permissions.

    @param trustee The trustee.
    @param allowed The allowed permissions (e.g. READ, WRITE, ...).
  */
  AccessControlEntry(const Trustee& trustee, AccessMask allowed) throw();
  
  /**
    Initializes access control entry (ACE) with the specified allowed and denied
    permissions.

    @param trustee The trustee.
    @param permissions The allowed/denied permissions (e.g. READ, WRITE, ...).
  */
  AccessControlEntry(const Trustee& trustee, const Permissions& permissions) throw();
  
  /**
    Initialization of object from other object.
  */
  AccessControlEntry(const AccessControlEntry& copy) throw();
  
  /**
    Assignment of object by object.
  */
  AccessControlEntry& operator=(const AccessControlEntry& eq) throw();
  
  /**
    Returns the permissions.
  */
  inline const Permissions& getPermissions() const throw() {
    return permissions;
  }
  
  /**
    Replaces the permissions. The denied permissions take precedence over
    allowed permissions if they conflict.
    
    @param permissions The new permissions.
  */
  void replace(const Permissions& permissions) throw();
  
  /**
    Revokes the
  */
  void revoke(unsigned int permissions) throw();
  
  /**
    Grants the specified permissions.

    @param allowed The permissions to allow.
  */
  void grant(AccessMask allowed) throw();
  
  /**
    Marks the specified permissions as denied.

    @param denied The permissions to deny.
  */
  void deny(AccessMask denied) throw();
  
  /**
    Combines the specified permissions with the ACE.
    
    @param permissions The permissions to merge with.
  */
  void combine(const Permissions& permissions) throw();
  
  /**
    Returns the trustee.
  */
  inline Trustee getTrustee() const throw() {
    return trustee;
  }
};

/**
  Writes an Access Control Entry (ACE) to a format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const AccessControlEntry& ace) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
