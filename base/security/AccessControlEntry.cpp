/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/AccessControlEntry.h>
#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

AccessControlEntry::AccessControlEntry() throw()
  : trustee(Trustee::EVERYONE, 0) {
  permissions.allowed = 0;
  permissions.denied = 0;
}

AccessControlEntry::AccessControlEntry(const Trustee& _trustee, const Permissions& _permissions) throw()
  : trustee(_trustee), permissions(_permissions) {
}

AccessControlEntry::AccessControlEntry(const AccessControlEntry& copy) throw()
  : trustee(copy.trustee), permissions(copy.permissions) {
}

AccessControlEntry& AccessControlEntry::operator=(const AccessControlEntry& eq) throw() {
  trustee = eq.trustee;
  permissions = eq.permissions;
  return *this;
}

void AccessControlEntry::replace(const Permissions& permissions) throw() {
  this->permissions.denied = permissions.denied;
  this->permissions.allowed = permissions.allowed & ~permissions.denied;
}

// TAG: FIXME
// void AccessControlEntry::revoke(AccessMask permissions) throw() {
//   permissions.allowed &= ~permissions;
// }

void AccessControlEntry::grant(AccessMask allowed) throw() {
  permissions.denied &= ~allowed;
  permissions.allowed |= allowed;
}

void AccessControlEntry::deny(AccessMask denied) throw() {
  permissions.allowed &= ~denied;
  permissions.denied |= denied;
}

void AccessControlEntry::combine(const Permissions& permissions) throw() {
  this->permissions.denied &= ~permissions.allowed;
  this->permissions.allowed |= permissions.allowed;
  this->permissions.allowed &= ~permissions.denied;
  this->permissions.denied |= permissions.denied;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const AccessControlEntry& ace) throw(IOException) {
  // TAG: FIXME
  StringOutputStream s;
  s << ace.getTrustee() << ':' << HEX << ace.getPermissions().allowed << '-' << HEX << ace.getPermissions().denied << FLUSH;
  return stream << s.getString();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
