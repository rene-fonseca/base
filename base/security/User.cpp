/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/security/User.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #include <aclapi.h>
#else // unix
  #include <sys/types.h>
  #include <pwd.h>
  #include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

User User::getCurrentUser() throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SECURITY_DESCRIPTOR* securityDescriptor;
  PSID ownerSID;
  assert(::GetSecurityInfo(
           ::GetCurrentProcess(),
           SE_KERNEL_OBJECT,
           OWNER_SECURITY_INFORMATION,
           &ownerSID,
           0,
           0,
           0,
           &securityDescriptor
         ) == ERROR_SUCCESS,
         UserException(Type::getType<User>())
  );
  User result = User(ownerSID);
  ::LocalFree(securityDescriptor);
  return result;
#else // unix
  uid_t uid = ::getuid();
  return User((void*)(ptrditt_t)uid);
#endif // flavor
}

User::User(const void* _id) throw(OutOfDomain) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((_id != 0) && (::IsValidSid((PSID)_id) != 0), OutOfDomain("Invalid user id", this));
  // check if user sid
  DWORD size = ::GetLengthSid((PSID)_id);
  id = new char[size];
  copy<char>((char*)id, (const char*)_id, size);
#else // unix
  assert((unsigned long)id <= PrimitiveTraits<uid_t>::MAXIMUM, OutOfDomain("Invalid user id", this));
  id = (void*)(ptrditt_t)_id;
#endif // flavor
}

User::User(const User& _copy) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (_copy.id == User::INVALID) {
    id = _copy.id;
    return;
  }
  DWORD size = ::GetLengthSid((PSID)_copy.id);
  id = new char[size];
  copy<char>((char*)id, (const char*)_copy.id, size);
#else // unix
  id = _copy.id;
#endif // flavor
}

User& User::operator=(const User& eq) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (&eq != this) { // protect against self assignment
    if (id != User::INVALID) {
      delete[] (char*)id;
    }
    if (eq.id == User::INVALID) {
      id = eq.id;
    } else {
      DWORD size = ::GetLengthSid((PSID)eq.id);
      id = new char[size];
      copy<char>((char*)id, (const char*)eq.id, size);
    }
  }
#else // unix
  id = eq.id;
#endif // flavor  
  return *this;
}

bool User::operator==(const User& eq) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if ((id == User::INVALID) || (eq.id == User::INVALID)) {
    return (id == User::INVALID) && (eq.id == User::INVALID);
  }
  return ::EqualSid((PSID)id, (PSID)eq.id) != 0;
#else // unix
  return id == eq.id;
#endif
}

User::User(const String& name) throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SID_NAME_USE sidType;
  char sid[4096];
  DWORD size = sizeof(sid);
  assert(::LookupAccountName(0,
                             name.getElements(),
                             &sid,
                             &size,
                             0,
                             0,
                             &sidType) != 0,
         UserException("Unable to lookup name", this)
  );
  assert(sidType == SidTypeUser, UserException("Not a user", this));
  id = new char[size];
  copy<char>((char*)id, sid, size);
#else // unix
  //long sysconf(_SC_GETPW_R_SIZE_MAX);
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwnam_r(name.getElements(), &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result == 0, UserException(this));
  id = (void*)(ptrditt_t)entry->pw_uid;
#endif // flavor
}

// TAG: select full name domain/user with option: LOCAL prefix?, BUILTIN prefix (no)?
String User::getName(bool fallback) const throw(UserException) {
  if (id == User::INVALID) {
    return MESSAGE("UNKNOWN");
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SID_NAME_USE sidType;
  char name[4096]; // TAG: what is the maximum size
  DWORD nameSize = sizeof(name);
  char domainName[64]; // TAG: what is the maximum size
  DWORD domainNameSize = sizeof(domainName);
  if (::LookupAccountSid(0,
                         (PSID)id, // must be valid
                         name,
                         &nameSize,
                         domainName,
                         &domainNameSize,
                         &sidType) == 0) {
    assert(fallback, UserException("Unable to lookup name", this));
    StringOutputStream s;
    s << *this << FLUSH;
    return s.getString();
  }
  // ASSERT((sidType == SidTypeUser) || ...???); // check type
  if (domainName[0] != 0) {
    return String(domainName) + MESSAGE("\\") + String(name);
  } else {
    return String(name); // TAG: does nameSize hold length of name
  }
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwuid_r((uid_t)id, &pw, buffer->getElements(), buffer->getSize(), &entry);
  if (result != 0) {
    assert(fallback, UserException("Unable to lookup name", this));
    StringOutputStream s;
    s << *this << FLUSH;
    return s.getString();
  }
  return String(entry->pw_name);
#endif // flavor
}

String User::getHomeFolder() const throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(this);
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwuid_r((uid_t)id, &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result == 0, UserException(this));
  return String(entry->pw_dir);
#endif // flavor
}

bool User::isAdmin() const throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return false;
//   BOOL isMember = FALSE;
//   if (id != User::INVALID) {
//     SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
//     PSID group;
//     assert((::AllocateAndInitializeSid(
//              &NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
//              0, 0, 0, 0, 0, 0, &group) != 0) &&
//            (::CheckTokenMembership(0, group, &isMember) != 0),
//            UserException(this));
//     ::FreeSid(group);
//   }
//   return isMember == TRUE;
#else // unix
  return id == 0; // root
#endif // flavor
}

bool User::isMemberOf(const Group& group) throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(this);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

User::~User() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (id != User::INVALID) {
    delete[] (char*)id;
  }
#else // unix
#endif // flavor
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const User& value) throw(IOException) {
  const void* opaqueId = value.getId();
  if (opaqueId == User::INVALID) {
    return stream << MESSAGE("UNKNOWN");
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  StringOutputStream s;
  PSID sid = (PSID)opaqueId; // must be valid
  
  // write prefix and revision number
  s << MESSAGE("S-") << ((SID*)sid)->Revision << '-';
  
  // write identifier authority
  PSID_IDENTIFIER_AUTHORITY identifier = ::GetSidIdentifierAuthority(sid);
  if ((identifier->Value[0] == 0) && (identifier->Value[1] == 0)) {
    s << DEC << static_cast<unsigned int>(identifier->Value[5]) +
      (static_cast<unsigned int>(identifier->Value[4]) << 8) +
      (static_cast<unsigned int>(identifier->Value[3]) << 16) +
      (static_cast<unsigned int>(identifier->Value[2]) << 24);
  } else {
    s << HEX << ZEROPAD << PREFIX << static_cast<unsigned int>(identifier->Value[0])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[1])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[2])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[3])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[4])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[5]);
  }
  
  // write subauthorities
  const unsigned int numberOfSubAuthorities = *::GetSidSubAuthorityCount(sid);
  for (unsigned int i = 0; i < numberOfSubAuthorities; i++) {
    s << '-' << *::GetSidSubAuthority(sid, i);
  }
  s << FLUSH;
  return stream << s.getString();
#else
  return stream << (unsigned long)opaqueId;
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
