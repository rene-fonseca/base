/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/AccessControlList.h>
#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

AccessControlList::AccessControlList() throw() {
}

AccessControlList::AccessControlList(const AccessControlList& copy) throw()
  : acl(copy.acl), owner(copy.owner), group(copy.group) {
}

AccessControlList& AccessControlList::operator=(const AccessControlList& eq) throw() {
  acl = eq.acl;
  owner = eq.owner;
  group = eq.group;
  return *this;
}

bool AccessControlList::add(const AccessControlEntry& ace, bool replace) throw() {
  Array<AccessControlEntry>::Iterator i = acl.getBeginIterator();
  const Array<AccessControlEntry>::Iterator end = acl.getEndIterator();
  const Trustee& trustee = ace.getTrustee();
  while (i < end) {
    if (i->getTrustee() == trustee) {
      if (replace) {
        i->replace(ace.getPermissions());
      } else {
        i->combine(ace.getPermissions());
      }
      return true; // tell that trustee was found
    }
  }
  acl.append(ace); // when replace is set and not set
  return false;
}

void AccessControlList::remove(unsigned int index) throw(OutOfRange) {
  acl.remove(index);
}

bool AccessControlList::remove(const Trustee& trustee) throw() {
  Array<AccessControlEntry>::ReadIterator i = acl.getBeginIterator();
  const Array<AccessControlEntry>::ReadIterator end = acl.getEndIterator();
  while (i < end) {
    if (i->getTrustee() == trustee) {
      acl.remove(i - acl.getBeginIterator());
      return true;
    }
  }
  return false;
}

bool AccessControlList::hasTrustee(const Trustee& trustee) const throw() {
  Array<AccessControlEntry>::ReadIterator i = acl.getBeginIterator();
  const Array<AccessControlEntry>::ReadIterator end = acl.getEndIterator();
  while (i < end) {
    if (i->getTrustee() == trustee) {
      return true;
    }
  }
  return false;
}

const AccessControlEntry& AccessControlList::getACE(const Trustee& trustee) const throw(InvalidKey) {
  Array<AccessControlEntry>::ReadEnumerator enu = acl.getReadEnumerator();
  while (enu.hasNext()) {
    const AccessControlEntry* ace = enu.next();
    if (ace->getTrustee() == trustee) {
      return *ace;
    }
  }
  throw InvalidKey("Trustee not in ACL", this);
}

// need exception to indicate that isMemberOf failed
AccessControlList::Permissions AccessControlList::getEffectiveAccess(const Trustee& trustee) const throw() { // TAG: fix exception
  Permissions effective;
  effective.allowed = 0;
  effective.denied = 0;
  
  // TAG: what is the right way to determine the effective rights
  Array<AccessControlEntry>::ReadEnumerator enu = acl.getReadEnumerator();
  while (enu.hasNext()) {
    const AccessControlEntry* ace = enu.next();
    if ((ace->getTrustee() == trustee) ||
        (ace->getTrustee().isMemberOf(trustee))) { // returns false if trustee is not a group
      effective.allowed |= ace->getPermissions().allowed;
      effective.denied |= ace->getPermissions().denied;
    }
  }
  effective.allowed &= ~effective.denied; // denied permissions take precedence
  return effective;
}

Array<AccessControlEntry>::ReadEnumerator AccessControlList::getReadEnumerator() const throw() {
  return acl.getReadEnumerator();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const AccessControlList& acl) throw(IOException) {
  StringOutputStream s;
  Array<AccessControlEntry>::ReadEnumerator enu = acl.getReadEnumerator();
  // acl.getEffectiveAccess(acl.getOwner())
  // acl.getEffectiveAccess(acl.getGroup())
  while (enu.hasNext()) {
    s << *enu.next() << EOL;
  }
  s << FLUSH;
  return stream << s.getString();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
