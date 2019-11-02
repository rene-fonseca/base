/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/AccessControlEntry.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

AccessControlEntry::AccessControlEntry() noexcept
  : trustee(Trustee::EVERYONE, 0)
{
  permissions.allowed = 0;
  permissions.denied = 0;
}

AccessControlEntry::AccessControlEntry(
  const Trustee& _trustee, AccessMask allowed) noexcept
  : trustee(_trustee)
{
  permissions.allowed = allowed;
  permissions.denied = 0;
}

AccessControlEntry::AccessControlEntry(
  const Trustee& _trustee, const Permissions& _permissions) noexcept
  : trustee(_trustee), permissions(_permissions)
{
}

AccessControlEntry::AccessControlEntry(const AccessControlEntry& copy) noexcept
  : trustee(copy.trustee), permissions(copy.permissions)
{
}

AccessControlEntry& AccessControlEntry::operator=(const AccessControlEntry& assign) noexcept
{
  trustee = assign.trustee;
  permissions = assign.permissions;
  return *this;
}

void AccessControlEntry::replace(const Permissions& permissions) noexcept
{
  this->permissions.denied = permissions.denied;
  this->permissions.allowed = permissions.allowed & ~permissions.denied;
}

void AccessControlEntry::filter(AccessMask mask) noexcept
{
  permissions.allowed &= mask;
  permissions.denied &= mask;
}

void AccessControlEntry::grant(AccessMask allowed) noexcept
{
  permissions.denied &= ~allowed;
  permissions.allowed |= allowed;
}

void AccessControlEntry::revoke(AccessMask denied) noexcept
{
  permissions.allowed &= ~denied;
  permissions.denied |= denied;
}

void AccessControlEntry::combine(const Permissions& permissions) noexcept
{
  this->permissions.denied &= ~permissions.allowed;
  this->permissions.allowed |= permissions.allowed;
  this->permissions.allowed &= ~permissions.denied;
  this->permissions.denied |= permissions.denied;
}

String AccessControlEntry::maskToString(AccessMask mask)
{
  String result; // TAG: should be optimized using iterator
  mask &= FULL; // filters out SYNCHRONIZE
  
  if (mask == FULL) {
    return Literal("F");
  } else if (mask == MODIFY) {
    return Literal("M");
  }

  if (mask & READ) {
    result += MESSAGE("R");
    if ((mask & READ) != READ) {
      result += '[';
      if (mask & READ_DATA) {
        result += MESSAGE("D");
      }
      if (mask & READ_ATTRIBUTES) {
        result += MESSAGE("At");
      }
      if (mask & READ_EXTENDED_ATTRIBUTES) {
        result += MESSAGE("Eat");
      }
      if (mask & READ_PERMISSIONS) {
        result += MESSAGE("P");
      }
      result += ']';
    }
  }
  
  if (mask & WRITE) {
    result += MESSAGE("W");
    if ((mask & WRITE) != WRITE) {
      result += '[';
      if (mask & WRITE_DATA) {
        result += MESSAGE("D");
      }
      if (mask & ADD_CONTENT) {
        result += MESSAGE("A");
      }
      if (mask & CHANGE_ATTRIBUTES) {
        result += MESSAGE("At");
      }
      if (mask & CHANGE_EXTENDED_ATTRIBUTES) {
        result += MESSAGE("Ea");
      }
      if (mask & CHANGE_PERMISSIONS) {
        result += MESSAGE("P");
      }
      if (mask & CHANGE_OWNER) {
        result += MESSAGE("O");
      }
      if (mask & REMOVE) {
        result += MESSAGE("R");
      }
      if (mask & REMOVE_COMPONENT) {
        result += MESSAGE("Rc");
      }
      result += ']';
    }
  }
  
  if (mask & EXECUTE) {
    result += MESSAGE("X");
  }
  
  return result;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const AccessControlEntry& ace) throw(IOException)
{
  StringOutputStream s;
  s << ace.getTrustee().getName() << ':';
  
  AccessControlEntry::AccessMask mask = ace.getPermissions().allowed;
  if (mask != 0) {
    s << AccessControlEntry::maskToString(mask);
  }
  mask = ace.getPermissions().denied;
  if (mask != 0) {
    s << '-' << AccessControlEntry::maskToString(mask);
  }
  s << FLUSH;
  return stream << s.getString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
