/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/security/User.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/string/WideString.h>
#include <base/NotImplemented.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  include <aclapi.h>
#  include <lm.h>

#  if (!defined(SID_MAX_SUB_AUTHORITIES))
#    define SID_MAX_SUB_AUTHORITIES 15
#  endif
#  if (!defined(SECURITY_MAX_SID_SIZE))
#    define SECURITY_MAX_SID_SIZE (sizeof(SID) - sizeof(DWORD) + (SID_MAX_SUB_AUTHORITIES * sizeof(DWORD)))
#  endif
#else // unix
#  include <sys/types.h>
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <pwd.h>
#endif
#  include <unistd.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

User User::getCurrentUser() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PSECURITY_DESCRIPTOR securityDescriptor = nullptr;
  PSID ownerSID = nullptr;
  bassert(::GetSecurityInfo(
           ::GetCurrentProcess(),
           SE_KERNEL_OBJECT,
           OWNER_SECURITY_INFORMATION,
           &ownerSID,
           nullptr,
           nullptr,
           nullptr,
           &securityDescriptor
         ) == ERROR_SUCCESS,
         UserException(Type::getType<User>())
  );
  User result = User(ownerSID);
  ::LocalFree(securityDescriptor);
  return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return User();
#else // unix
  return User(static_cast<unsigned long>(::getuid()));
#endif // flavor
}

User::User(unsigned long _id) : integralId(_id) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _throw OutOfDomain("Invalid user id.", this);
#else
//   bassert(
//     integralId <= PrimitiveTraits<uid_t>::MAXIMUM,
//     OutOfDomain("Invalid user id.", this)
//   );
#endif
}

User::User(const void* _id)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (_id == 0) {
    integralId = INVALID;
    return;
  }
  bassert(::IsValidSid((PSID)_id) != 0, OutOfDomain("Invalid user.", this));
  unsigned int size = ::GetLengthSid((PSID)_id);
  id = new ReferenceCountedAllocator<uint8>(size);
  copy(id->getElements(), Cast::pointer<const uint8*>(_id), size);
  integralId = 0;
#else // unix
  _throw OutOfDomain("Invalid user.", this);
#endif // flavor
}

User::User(const User& copy) noexcept
  : integralId(copy.integralId), id(copy.id)
{
}

User& User::operator=(const User& assign) noexcept
{
  id = assign.id;
  integralId = assign.integralId;
  return *this;
}

bool User::operator==(const User& compare) const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!isValid() || (!compare.isValid())) {
    return !isValid() && !compare.isValid();
  }
  return ::EqualSid((PSID)id->getElements(), (PSID)compare.id->getElements()) != 0;
#else // unix
  return Cast::extract<uid_t>(integralId) ==
    Cast::extract<uid_t>(compare.integralId);
#endif
}

User::User(const String& name)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SID_NAME_USE sidType;
  uint8 sid[SECURITY_MAX_SID_SIZE];
  DWORD size = sizeof(sid);
  bassert(::LookupAccountName(nullptr,
                             ToWCharString(name),
                             &sid,
                             &size,
                             0,
                             0,
                             &sidType) != 0,
         UserException("Unable to lookup name.", this)
  );
  bassert(sidType == SidTypeUser, UserException("Not a user.", this));
  id = new ReferenceCountedAllocator<uint8>(size);
  copy(id->getElements(), sid, size);
  integralId = 0;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else // unix
  // long sysconf(_SC_GETPW_R_SIZE_MAX);
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  struct passwd pw;
  struct passwd* entry = nullptr;
  int result = ::getpwnam_r(
    name.getElements(),
    &pw,
    (char*)buffer.getElements(),
    buffer.getSize()/sizeof(char),
    &entry
  );
  bassert(result == 0, UserException(this));
  integralId = static_cast<unsigned long>(entry->pw_uid);
#endif // flavor
}

// TAG: select full name domain/user with option: LOCAL prefix?, BUILTIN prefix (no)?
String User::getName(bool fallback) const
{
  if (!isValid()) {
    return Literal("<unknown>");
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SID_NAME_USE sidType;
  wchar name[UNLEN+1];
  DWORD nameSize = getArraySize(name);
  wchar domainName[DNLEN+1];
  DWORD domainNameSize = getArraySize(domainName);
  if (::LookupAccountSid(
        nullptr,
        (PSID)id->getElements(),
        name,
        &nameSize,
        domainName,
        &domainNameSize,
        &sidType) == 0
  ) {
    bassert(fallback, UserException("Unable to lookup name.", this));
    StringOutputStream s;
    s << *this << FLUSH;
    return s.getString();
  }
  if (domainName[0] != L'\0') {
    return String(domainName) + MESSAGE("/") + String(name);
  } else {
    return String(static_cast<const wchar*>(name)); // TAG: does nameSize hold length of name
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return String();
#else // unix
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  struct passwd pw;
  struct passwd* entry = nullptr;
  int result = ::getpwuid_r(
    Cast::extract<uid_t>(integralId),
    &pw,
    (char*)buffer.getElements(),
    buffer.getSize()/sizeof(char),
    &entry
  );
  if (result != 0) {
    bassert(fallback, UserException("Unable to lookup name.", this));
    StringOutputStream s;
    s << *this << FLUSH;
    return s.getString();
  }
  return String(entry->pw_name);
#endif // flavor
}

String User::getHomeFolder() const
{
  bassert(isValid(), UserException(this));
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: fixme
//   BOOL GetUserProfileDirectory(
//     HANDLE hToken,
//     LPTSTR lpProfileDir,
//     LPDWORD lpcchSize
//   );
  _throw NotImplemented(this);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  _throw NotImplemented(this);
#else // unix  
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  struct passwd pw;
  struct passwd* entry = nullptr;
  int result = ::getpwuid_r(
    Cast::extract<uid_t>(integralId),
    &pw,
    (char*)buffer.getElements(),
    buffer.getSize()/sizeof(char),
    &entry
  );
  bassert(result == 0, UserException(this));
  return String(entry->pw_dir);
#endif // flavor
}

bool User::isAdmin() const {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return false;
//   BOOL isMember = FALSE;
//   if (id != User::INVALID) {
//     SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
//     PSID group;
//     bassert((::AllocateAndInitializeSid(
//              &NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
//              0, 0, 0, 0, 0, 0, &group) != 0) &&
//            (::CheckTokenMembership(0, group, &isMember) != 0),
//            UserException(this));
//     ::FreeSid(group);
//   }
//   return isMember == TRUE;
#else // unix
  return integralId == 0; // root
#endif // flavor
}

bool User::isMemberOf(const Group& group) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _throw NotImplemented(this);
#else // unix
  _throw NotImplemented(this);
#endif // flavor
}

Array<String> User::getGroups() {
  Array<String> result;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(isValid(), UserException(this));
  SID_NAME_USE sidType;
  WCHAR name[UNLEN+1];
  DWORD nameSize = sizeof(name);
  WCHAR domainName[DNLEN+1];
  DWORD domainNameSize = sizeof(domainName);
  bassert(::LookupAccountSidW(0,
                             (PSID)id->getElements(), // must be valid
                             name,
                             &nameSize,
                             domainName,
                             &domainNameSize,
                             &sidType) == 0,
         UserException("Unable to lookup name.", this)
  );
  
  GROUP_USERS_INFO_0* buffer = nullptr;
  DWORD numberOfEntries = 0;
  DWORD totalEntries = 0;
  NET_API_STATUS status = ::NetUserGetGroups(0,
                                             name,
                                             0,
                                             (LPBYTE*)&buffer,
                                             MAX_PREFERRED_LENGTH,
                                             &numberOfEntries,
                                             &totalEntries);
  bassert((status == NERR_Success) || (status == ERROR_MORE_DATA), UserException(this));
  if (buffer != 0) {
    const GROUP_USERS_INFO_0* p = buffer;
    const GROUP_USERS_INFO_0* end = p + numberOfEntries;
    while (p < end) {
      result.append(String(p->grui0_name));
      ++p;
    }
    ::NetApiBufferFree(buffer);
  }
#else // unix
  bassert(isValid(), UserException(this));
  _throw NotImplemented(this);
#endif // flavor
  return result;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const User& value)
{
  if (!value.isValid()) {
    return stream << "<unknown>";
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  StringOutputStream s;
  PSID sid = (PSID)value.getId(); // must be valid
  
  // write prefix and revision number
  s << "S-" << ((SID*)sid)->Revision << '-';
  
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
  return stream << value.getIntegralId();
#endif
}

unsigned long Hash<User>::operator()(const User& value) noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!value.id.isValid()) {
    return 0;
  }
  const uint8* src = value.id->getElements();
  const uint8* end = src + value.id->getSize();
  unsigned long result = 5381; // TAG: fixme
  while (src < end) {
    result = /*33*/ 31 /*65599*/ * result + static_cast<unsigned char>(*src++);
  }
  return result;
#else // unix
  return value.getIntegralId();
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
