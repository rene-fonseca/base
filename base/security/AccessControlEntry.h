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
  
  @short Entry of access control list (ACL).
  @ingroup security
  @see AccessControlList
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AccessControlEntry : public Object {
public:

  /** The fundamental permissions. */
  enum ExplicitPermission {
    EXECUTE = 1, /**< Execute permission. */

    READ_DATA = EXECUTE << 1, /**< Read content permission. */
    READ_ATTRIBUTES = READ_DATA << 1, /**< Read basic attributes permission. */
    READ_EXTENDED_ATTRIBUTES = READ_ATTRIBUTES << 1, /**< Read extended attributes permission. */
    READ_PERMISSIONS = READ_EXTENDED_ATTRIBUTES << 1, /**< Permissions to read permissions. */
    
    WRITE_DATA = READ_PERMISSIONS << 1, /**< Permission to change content of object. */
    ADD_CONTENT = WRITE_DATA << 1, /**< Permission to add content to object. */
    CHANGE_ATTRIBUTES = ADD_CONTENT << 1, /**< Permissions to change basic attributes. */
    CHANGE_EXTENDED_ATTRIBUTES = CHANGE_ATTRIBUTES << 1, /**< Permission to change extended attributes. */
    CHANGE_PERMISSIONS = CHANGE_EXTENDED_ATTRIBUTES << 1, /**< Permission to change permissions of object. */
    CHANGE_OWNER = CHANGE_PERMISSIONS << 1, /**< Permission to change owner of object. */
    
    REMOVE = CHANGE_OWNER << 1, /**< Permission to change the owner of the object. */
    REMOVE_COMPONENT = REMOVE << 1, /**< Permission to remove subcomponent. */
    
    SYNCHRONIZE = REMOVE_COMPONENT << 1, /**< Synchronization permission. */
    
    APPEND_DATA = ADD_CONTENT, /**< Permission to append data to file. */
    
    TRAVERSE_FOLDER = EXECUTE, /**< Permission to traverse folder. */
    LIST_FOLDER = READ_DATA, /**< Permission to read content of folder. */
    ADD_FILE = WRITE_DATA, /**< Permission to add file in folder. */
    CREATE_FOLDERS = APPEND_DATA, /**< Permission to add subfolder. */
  };
  
  /** Composite permissions. */
  enum CompositePermission {
    /** Trustee does not have any access. */
    NO = 0,
    /** Permission to read object. */
    READ = READ_DATA|READ_ATTRIBUTES|READ_EXTENDED_ATTRIBUTES|READ_PERMISSIONS,
    /** Permission to change object. */
    WRITE = WRITE_DATA|APPEND_DATA|CHANGE_ATTRIBUTES|CHANGE_EXTENDED_ATTRIBUTES|CHANGE_PERMISSIONS|CHANGE_OWNER|REMOVE|REMOVE_COMPONENT,
    /** Permission to read and execute object. */
    READ_AND_EXECUTE = READ|EXECUTE,
    /** Permission to list and traverse a folder. */
    LIST_AND_TRAVERSE = READ_AND_EXECUTE,
    /** Permission to modify object. */
    MODIFY = READ_DATA|READ_ATTRIBUTES|READ_EXTENDED_ATTRIBUTES|READ_PERMISSIONS|WRITE_DATA|APPEND_DATA|CHANGE_ATTRIBUTES|CHANGE_EXTENDED_ATTRIBUTES|REMOVE|EXECUTE,
    /** Trustee has full access. */
    FULL = READ|WRITE|EXECUTE
  };
  
  /** The access mask. */
  typedef uint32 AccessMask;
  
  /* Access masks. */
  struct Permissions {
    /** The allowed permissions. */
    AccessMask allowed;
    /** The denied permissions. */
    AccessMask denied;
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
    Initializes ACE with specified trustee and no allowed access (and no denied
    access).
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
  AccessControlEntry(
    const Trustee& trustee, const Permissions& permissions) throw();
  
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
    Filters out any permissions missing from the specified mask.
  */
  void filter(AccessMask permissions) throw();
  
  /**
    Grants the specified permissions.

    @param allowed The permissions to allow.
  */
  void grant(AccessMask allowed) throw();
  
  /**
    Marks the specified permissions as denied.

    @param denied The permissions to deny.
  */
  void revoke(AccessMask denied) throw();
  
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

  /**
    Returns a string representation of the access mask.
  */
  static String maskToString(AccessMask mask) throw();
};

/**
  Writes an Access Control Entry (ACE) to a format output stream.
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const AccessControlEntry& ace) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
