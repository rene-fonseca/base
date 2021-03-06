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
#include <base/string/FormatOutputStream.h>
#include <base/collection/Array.h>
#include <base/collection/InvalidKey.h>
#include <base/security/Trustee.h>

#if defined(SYNCHRONIZE)
#  error Check that windows headers are included after framework headers
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  undef READ_ATTRIBUTES
#  undef SYNCHRONIZE
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  An entry within an Access Control List (ACL) describing the access rights for
  a single trustee.
  
  @short Entry of access control list (ACL).
  @ingroup security
  @see AccessControlList
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AccessControlEntry : public Object {
public:

  /** The fundamental permissions. */
  enum ExplicitPermission {
    /** Execute permission. */
    EXECUTE = 1,

    /** Read content permission. */
    READ_DATA = EXECUTE << 1,
    /** Read basic attributes permission. */
    READ_ATTRIBUTES = READ_DATA << 1,
    /** Read extended attributes permission. */
    READ_EXTENDED_ATTRIBUTES = READ_ATTRIBUTES << 1,
    /** Permissions to read permissions. */
    READ_PERMISSIONS = READ_EXTENDED_ATTRIBUTES << 1,
    
    /** Permission to change content of object. */
    WRITE_DATA = READ_PERMISSIONS << 1,
    /** Permission to add content to object. */
    ADD_CONTENT = WRITE_DATA << 1,
    /** Permissions to change basic attributes. */
    CHANGE_ATTRIBUTES = ADD_CONTENT << 1,
    /** Permission to change extended attributes. */
    CHANGE_EXTENDED_ATTRIBUTES = CHANGE_ATTRIBUTES << 1,
    /** Permission to change permissions of object. */
    CHANGE_PERMISSIONS = CHANGE_EXTENDED_ATTRIBUTES << 1,
    /** Permission to change the owner of an object. */
    CHANGE_OWNER = CHANGE_PERMISSIONS << 1,
    
    /** Permission to remove the object. */
    REMOVE = CHANGE_OWNER << 1,
    /** Permission to remove subcomponents. */
    REMOVE_COMPONENT = REMOVE << 1,
    
    /** Synchronization permission. */
    SYNCHRONIZE = REMOVE_COMPONENT << 1,
    
    /** Permission to append data to file. */
    APPEND_DATA = ADD_CONTENT,
    
    /** Permission to traverse folder. */
    TRAVERSE_FOLDER = EXECUTE,
    /** Permission to read content of folder. */
    LIST_FOLDER = READ_DATA,
    /** Permission to add file in folder. */
    ADD_FILE = WRITE_DATA,
    /** Permission to add subfolder. */
    CREATE_FOLDERS = APPEND_DATA,
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
    AccessMask allowed = 0;
    /** The denied permissions. */
    AccessMask denied = 0;
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
  AccessControlEntry() noexcept;
  
  /**
    Initializes ACE with specified trustee and no allowed access (and no denied
    access).
  */
  AccessControlEntry(const Trustee& trustee) noexcept;
  
  /**
    Initializes access control entry (ACE) with the specified allowed (and no
    denied) permissions.

    @param trustee The trustee.
    @param allowed The allowed permissions (e.g. READ, WRITE, ...).
  */
  AccessControlEntry(const Trustee& trustee, AccessMask allowed) noexcept;
  
  /**
    Initializes access control entry (ACE) with the specified allowed and
    denied permissions.

    @param trustee The trustee.
    @param permissions The allowed/denied permissions (e.g. READ, WRITE, ...).
  */
  AccessControlEntry(
    const Trustee& trustee, const Permissions& permissions) noexcept;
  
  /**
    Initialization of object from other object.
  */
  AccessControlEntry(const AccessControlEntry& copy) noexcept;
  
  /**
    Assignment of object by object.
  */
  AccessControlEntry& operator=(const AccessControlEntry& assign) noexcept;
  
  /**
    Returns the permissions.
  */
  inline const Permissions& getPermissions() const noexcept
  {
    return permissions;
  }
  
  /**
    Replaces the permissions. The denied permissions take precedence over
    allowed permissions if they conflict.
    
    @param permissions The new permissions.
  */
  void replace(const Permissions& permissions) noexcept;
  
  /**
    Filters out any permissions missing from the specified mask.
  */
  void filter(AccessMask permissions) noexcept;
  
  /**
    Grants the specified permissions.

    @param allowed The permissions to allow.
  */
  void grant(AccessMask allowed) noexcept;
  
  /**
    Marks the specified permissions as denied.

    @param denied The permissions to deny.
  */
  void revoke(AccessMask denied) noexcept;
  
  /**
    Combines the specified permissions with the ACE.
    
    @param permissions The permissions to merge with.
  */
  void combine(const Permissions& permissions) noexcept;
  
  /**
    Returns the trustee.
  */
  inline const Trustee& getTrustee() const noexcept
  {
    return trustee;
  }

  /**
    Returns a string representation of the access mask.
  */
  static String maskToString(AccessMask mask);
};

/**
  Writes an Access Control Entry (ACE) to a format output stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const AccessControlEntry& ace);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
