/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__ACCESS_CONTROL_LIST_H
#define _DK_SDU_MIP__BASE_SECURITY__ACCESS_CONTROL_LIST_H

#include <base/Object.h>
#include <base/security/AccessControlEntry.h>
#include <base/string/String.h>
#include <base/string/FormatOutputStream.h>
#include <base/collection/Array.h>
#include <base/collection/InvalidKey.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Access Control List (ACL).

  @short Access Control List (ACL).
  @ingroup security
  @see AccessControlEntry
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AccessControlList : public Object {
private:
  
  /** The ACL entries. */
  Array<AccessControlEntry> acl;
  /** The associated owner. */
  Trustee owner;
  /** The primary group. */
  Trustee group;
public:

  /** Permissions. */
  typedef AccessControlEntry::Permissions Permissions;
  
//   /** Flags specifying how the permissions are applied to the existing entries. */
//   enum Merge {
//     REPLACE, /**< Specifies that the ACE should be replaced for the matching trustee (otherwise appended to ACL). */
//     REVOKE_ACCESS /**< Specifies that the permission flags are merged using the AND operator. */
//   };
  
  /**
    Initialize ACL with no entries.
  */
  AccessControlList() throw();
  
  /**
    Initializes object from other object.
  */
  AccessControlList(const AccessControlList& copy) throw();

  /**
    Assignment of object by object.
  */
  AccessControlList& operator=(const AccessControlList& eq) throw();

  /**
    Returns true if the ACL is empty.
  */
  inline bool isEmpty() const throw() {
    return acl.isEmpty();
  }

  /**
    Returns the number of entries in the list.
  */
  inline unsigned int getSize() const throw() {
    return acl.getSize();
  }
  
  /**
    Adds an entry to the ACL.
    
    @param entry The access control entry.
    @param replace Specifies whether to replace or merge (using the AND
    operator) the permissions. The default is to merge.

    @return True, if the trustee already was contained within the ACL.
  */
  bool add(const AccessControlEntry& entry, bool replace = false) throw();
  
  /**
    Removes the entry at the specified index.
  */
  void remove(unsigned int index) throw(OutOfRange);
  
  /**
    Removes a trustee from the list.
    
    @returns False, if the list has no entry for the specified trustee.
  */
  bool remove(const Trustee& trustee) throw();
  
  /**
    Returns true if an entry is available for the specified trustee.
  */
  bool hasTrustee(const Trustee& trustee) const throw();
  
  /**
    Returns the access control entry for the specified trustee. Raises
    InvalidKey if no entry is available for the specified truste.
  */
  const AccessControlEntry& getACE(const Trustee& trustee) const throw(InvalidKey);

  /**
    Returns the associated owner.
  */
  inline Trustee getOwner() throw() {
    return owner;
  }

  /**
    Set the associated owner.
  */
  inline void setOwner(const Trustee& owner) throw() {
    this->owner = owner;
  }

  /**
    Returns the associated primary group.
  */
  inline Trustee getGroup() throw() {
    return group;
  }
  
  /**
    Set the associated primary group.
  */
  inline void setGroup(const Trustee& group) throw() {
    this->group = group;
  }
  
  /**
    Returns the access of the trustee as specified by this ACL.
  */
  Permissions getEffectiveAccess(const Trustee& trustee) const throw();
  
  /**
    Returns an enumerator for non-modifying access.
  */
  Array<AccessControlEntry>::ReadEnumerator getReadEnumerator() const throw();
};

/**
  Writes an ACL to a format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const AccessControlList& acl) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
