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
#include <base/security/Group.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/string/WideString.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  include <lm.h>
#else // unix
#  include <sys/types.h>
#  include <grp.h>
#  include <pwd.h>
#  include <unistd.h>
#endif // flavor

#if (_DK_SDU_MIP__BASE__OS != _DK_SDU_MIP__BASE__CYGWIN)
#  define _DK_SDU_MIP__BASE__HAVE_GETGRNAM_R
#  define _DK_SDU_MIP__BASE__HAVE_GETGRGID_R
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Group::Group(unsigned long _id) throw(OutOfDomain) : integralId(_id) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw OutOfDomain("Invalid user id", this);
#else // unix
  assert(integralId <= PrimitiveTraits<uid_t>::MAXIMUM, OutOfDomain("Invalid group id", this));
#endif // flavor
}

Group::Group(const void* _id) throw(OutOfDomain) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (_id == 0) {
    integralId = getMaximum(integralId);
    return;
  }
  assert(::IsValidSid((PSID)_id) != 0, OutOfDomain("Invalid group id", this));
  unsigned int size = ::GetLengthSid((PSID)_id);
  id = new ReferenceCountedAllocator<uint8>(size);
  copy(id->getElements(), Cast::pointer<const uint8*>(_id), size);
  integralId = 0;
#else // unix
  throw OutOfDomain("Invalid group id", this);
#endif // flavor
}

Group::Group(const Group& copy) throw() : integralId(copy.integralId), id(copy.id) {
}

Group& Group::operator=(const Group& eq) throw() {
  id = eq.id;
  integralId = eq.integralId;
  return *this;
}

bool Group::operator==(const Group& eq) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!isValid() || (!eq.isValid())) {
    return !isValid() && !eq.isValid();
  }
  return ::EqualSid((PSID)id->getElements(), (PSID)eq.id->getElements()) != 0;
#else // unix
  return integralId == eq.integralId;
#endif
}

Group::Group(const String& name) throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(this);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_GETGRNAM_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Allocator<char>* buffer = Thread::getLocalStorage();
    struct group grp;
    struct group* entry;
    int result = ::getgrnam_r(name.getElements(), &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    integralId = Cast::container<unsigned long>(entry->gr_gid);
  #else
    #warning Group::Group(const String& name) uses non-reentrant getgrnam
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrnam(name.getElements());
    assert(entry != 0, GroupException(this));
    integralId = Cast::container<unsigned long>(entry->gr_gid);
  #endif
#endif // flavor
}

Group::Group(const User& user) throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(this);
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwuid_r(Cast::extract<uid_t>(user.getIntegralId()), &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result == 0, GroupException(this));
  integralId = Cast::container<unsigned long>(entry->pw_gid);
#endif // flavor
}

String Group::getName() const throw(GroupException) {
  if (!isValid()) {
    return MESSAGE("<unknown>");
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SID_NAME_USE sidType;
  char name[UNLEN+1]; // TAG: what is the maximum size
  DWORD nameSize = sizeof(name);
  char domainName[DNLEN+1]; // TAG: what is the maximum size
  DWORD domainNameSize = sizeof(domainName);
  assert(::LookupAccountSid(0,
                            (PSID)id->getElements(),
                            name,
                            &nameSize,
                            domainName,
                            &domainNameSize,
                            &sidType) != 0,
         GroupException("Unable to lookup name", this)
  );
  if (domainName[0] != 0) {
    return String(domainName) + MESSAGE("\\") + String(name);
  } else {
    return String(name); // TAG: does nameSize hold length of name
  }
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_GETGRNAM_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Allocator<char>* buffer = Thread::getLocalStorage();
    struct group grp;
    struct group* entry;
    int result = ::getgrgid_r(Cast::extract<gid_t>(integralId), &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    return String(entry->gr_name);
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(Cast::extract<gid_t>(integralId));
    assert(entry != 0, GroupException(this));
    return String(entry->gr_name);
  #endif
#endif // flavor
}

Array<String> Group::getMembers() const throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Array<String> result;

  SID_NAME_USE sidType;
  WCHAR name[GNLEN+1];
  DWORD nameSize = sizeof(name);
  WCHAR domainName[DNLEN+1];
  DWORD domainNameSize = sizeof(domainName);
  assert(::LookupAccountSidW(0,
                             (PSID)id->getElements(),
                             name,
                             &nameSize,
                             domainName,
                             &domainNameSize,
                             &sidType) != 0,
         GroupException("Unable to lookup name", this)
  );
  
  GROUP_USERS_INFO_0* buffer = 0;
  DWORD numberOfEntries = 0;
  DWORD totalEntries = 0;
  NET_API_STATUS status = ::NetGroupGetUsers(0, // use local machine
                                             name,
                                             0,
                                             (LPBYTE*)&buffer,
                                             MAX_PREFERRED_LENGTH,
                                             &numberOfEntries,
                                             &totalEntries,
                                             0
  );
  assert((status == NERR_Success) || (status == ERROR_MORE_DATA), GroupException("Unable to get members", this));
  if (buffer != 0) {
    const GROUP_USERS_INFO_0* p = buffer;
    const GROUP_USERS_INFO_0* end = p + numberOfEntries;
    while (p < end) {
      result.append(WideString::getMultibyteString(p->grui0_name));
      ++p;
    }
    ::NetApiBufferFree(buffer);
  }
  return result;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_GETGRGID_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Allocator<char>* buffer = Thread::getLocalStorage();
    struct group grp;
    struct group* entry;
    int result = ::getgrgid_r(Cast::extract<gid_t>(integralId), &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    Array<String> members;
    char** memberName = entry->gr_mem;
    while (memberName) {
      members.append(*memberName++);
    }
    return members;
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(Cast::extract<gid_t>(integralId));
    assert(entry != 0, GroupException(this));
    Array<String> members;
    char** memberName = entry->gr_mem;
    while (memberName) {
      members.append(*memberName++);
    }
    return members;
  #endif
#endif // flavor
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Group& value) throw(IOException) {
  if (!value.isValid()) {
    return stream << MESSAGE("<unknown>");
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  StringOutputStream s;
  PSID sid = (PSID)value.getId(); // must be valid
  
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
#else // unix
  return stream << value.getIntegralId();
#endif // flavor
}

unsigned long Hash<Group>::operator()(const Group& value) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
