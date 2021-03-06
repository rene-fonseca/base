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
#include <base/security/Group.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/string/WideString.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  include <lm.h>
#else // unix
#  include <sys/types.h>
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <grp.h>
#  include <pwd.h>
#endif
#  include <unistd.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Group::Group(unsigned long _id)
  : integralId(_id)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _throw OutOfDomain("Invalid user id.", this);
#else // unix
//   bassert(
//     static_cast<gid_t>(integralId) <= PrimitiveTraits<gid_t>::MAXIMUM,
//     OutOfDomain("Invalid group id.", this)
//   );
#endif // flavor
}

Group::Group(const void* _id)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (_id == 0) {
    integralId = INVALID;
    return;
  }
  bassert(::IsValidSid((PSID)_id) != 0, OutOfDomain("Invalid group.", this));
  unsigned int size = ::GetLengthSid((PSID)_id);
  id = new ReferenceCountedAllocator<uint8>(size);
  copy(id->getElements(), Cast::pointer<const uint8*>(_id), size);
  integralId = 0;
#else // unix
  _throw OutOfDomain("Invalid group id.", this);
#endif // flavor
}

Group::Group(const Group& copy) noexcept
  : integralId(copy.integralId), id(copy.id)
{
}

Group& Group::operator=(const Group& assign) noexcept
{
  id = assign.id;
  integralId = assign.integralId;
  return *this;
}

bool Group::operator==(const Group& compare) const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!isValid() || (!compare.isValid())) {
    return !isValid() && !compare.isValid();
  }
  return ::EqualSid((PSID)id->getElements(), (PSID)compare.id->getElements()) != 0;
#else // unix
  return Cast::extract<gid_t>(integralId) ==
    Cast::extract<gid_t>(compare.integralId);
#endif
}

Group::Group(const String& name)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__HAVE_GETGRNAM_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Thread::UseThreadLocalBuffer _buffer;
    Allocator<uint8>& buffer = _buffer;
    struct group grp;
    struct group* entry = nullptr;
    int result = ::getgrnam_r(
      name.getElements(),
      &grp,
      (char*)buffer.getElements(),
      buffer.getSize()/sizeof(char),
      &entry
    );
    bassert(result == 0, GroupException(this));
    integralId = static_cast<unsigned long>(entry->gr_gid);
  #else
    // #warning Group::Group(const String& name) uses non-reentrant getgrnam
    // long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrnam(name.getElements());
    bassert(entry != 0, GroupException(this));
    integralId = static_cast<unsigned long>(entry->gr_gid);
  #endif
#endif // flavor
}

Group::Group(const User& user)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#else // unix
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  struct passwd pw;
  struct passwd* entry = nullptr;
  int result = ::getpwuid_r(
    Cast::extract<uid_t>(user.getIntegralId()),
    &pw,
    (char*)buffer.getElements(),
    buffer.getSize()/sizeof(char),
    &entry
  );
  bassert(result == 0, GroupException(this));
  integralId = static_cast<unsigned long>(entry->pw_gid);
#endif // flavor
}

String Group::getName() const
{
  if (!isValid()) {
    return Literal("<unknown>");
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SID_NAME_USE sidType;
  wchar name[UNLEN+1]; // TAG: what is the maximum size
  DWORD nameSize = getArraySize(name);
  wchar domainName[DNLEN+1]; // TAG: what is the maximum size
  DWORD domainNameSize = getArraySize(domainName);
  bassert(
    ::LookupAccountSid(
      0,
      (PSID)id->getElements(),
      name,
      &nameSize,
      domainName,
      &domainNameSize,
      &sidType
    ) != 0,
    GroupException("Unable to lookup name.", this)
  );
  if (domainName[0] != L'\0') {
    return String(domainName) + MESSAGE("\\") + String(name);
  } else {
    return String(static_cast<const wchar*>(name)); // TAG: does nameSize hold length of name
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return String();
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__HAVE_GETGRNAM_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Thread::UseThreadLocalBuffer _buffer;
    Allocator<uint8>& buffer = _buffer;
    struct group grp;
    struct group* entry = nullptr;
    int result = ::getgrgid_r(
      Cast::extract<gid_t>(integralId),
      &grp,
      (char*)buffer.getElements(),
      buffer.getSize()/sizeof(char),
      &entry
    );
    bassert(result == 0, GroupException(this));
    return String(entry->gr_name);
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(Cast::extract<gid_t>(integralId));
    bassert(entry != 0, GroupException(this));
    return String(entry->gr_name);
  #endif
#endif // flavor
}

Array<String> Group::getMembers() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Array<String> result;

  SID_NAME_USE sidType;
  WCHAR name[GNLEN+1];
  DWORD nameSize = getArraySize(name);
  WCHAR domainName[DNLEN+1];
  DWORD domainNameSize = getArraySize(domainName);
  bassert(
    ::LookupAccountSidW(
      NULL,
      (PSID)id->getElements(),
      name,
      &nameSize,
      domainName,
      &domainNameSize,
      &sidType
    ) != 0,
    GroupException("Unable to lookup name.", this)
  );
  
  GROUP_USERS_INFO_0* buffer = nullptr;
  DWORD numberOfEntries = 0;
  DWORD totalEntries = 0;
  NET_API_STATUS status = ::NetGroupGetUsers(
    NULL, // use local machine
    name,
    0,
    (LPBYTE*)&buffer,
    MAX_PREFERRED_LENGTH,
    &numberOfEntries,
    &totalEntries,
    0
  );
  bassert(
    (status == NERR_Success) || (status == ERROR_MORE_DATA),
    GroupException("Unable to get members.", this)
  );
  if (buffer != 0) {
    const GROUP_USERS_INFO_0* p = buffer;
    const GROUP_USERS_INFO_0* end = p + numberOfEntries;
    while (p < end) {
      result.append(String(p->grui0_name));
      ++p;
    }
    ::NetApiBufferFree(buffer);
  }
  return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return Array<String>();
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__HAVE_GETGRGID_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Thread::UseThreadLocalBuffer _buffer;
    Allocator<uint8>& buffer = _buffer;
    struct group grp;
    struct group* entry = nullptr;
    int result = ::getgrgid_r(
      Cast::extract<gid_t>(integralId),
      &grp,
      (char*)buffer.getElements(),
      buffer.getSize()/sizeof(char),
      &entry
    );
    bassert(result == 0, GroupException(this));
    Array<String> members;
    char** memberName = entry->gr_mem;
    while (memberName) {
      members.append(NativeString(*memberName++));
    }
    return members;
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(Cast::extract<gid_t>(integralId));
    bassert(entry != 0, GroupException(this));
    Array<String> members;
    char** memberName = entry->gr_mem;
    while (memberName) {
      members.append(NativeString(*memberName++));
    }
    return members;
  #endif
#endif // flavor
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Group& value)
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
    s << HEX << ZEROPAD << PREFIX
      << static_cast<unsigned int>(identifier->Value[0])
      << HEX << ZEROPAD << NOPREFIX
      << static_cast<unsigned int>(identifier->Value[1])
      << HEX << ZEROPAD << NOPREFIX
      << static_cast<unsigned int>(identifier->Value[2])
      << HEX << ZEROPAD << NOPREFIX
      << static_cast<unsigned int>(identifier->Value[3])
      << HEX << ZEROPAD << NOPREFIX
      << static_cast<unsigned int>(identifier->Value[4])
      << HEX << ZEROPAD << NOPREFIX
      << static_cast<unsigned int>(identifier->Value[5]);
  }
  
  // write subauthorities
  const unsigned int numberOfSubAuthorities = *::GetSidSubAuthorityCount(sid);
  for (unsigned int i = 0; i < numberOfSubAuthorities; i++) {
    s << '-' << *::GetSidSubAuthority(sid, i);
  }
  s << FLUSH;
  return stream << s.getString();
#else // unix
  return stream << value.getIntegralId();
#endif // flavor
}

unsigned long Hash<Group>::operator()(const Group& value) noexcept {
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

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__EMCC)
extern "C" {

int getgrgid_r(gid_t gid, struct group *grp, char *buffer, size_t bufsize, struct group **result)
{
  return -1;
}

int getpwuid_r(uid_t uid, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result)
{
  return -1;
}

};

#endif
