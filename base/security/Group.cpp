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
  #include <windows.h>
  #include <lm.h>
#else // unix
  #include <sys/types.h>
  #include <grp.h>
  #include <pwd.h>
  #include <unistd.h>
#endif // flavor

#if (_DK_SDU_MIP__BASE__OS != _DK_SDU_MIP__BASE__CYGWIN)
  #define _DK_SDU_MIP__BASE__HAVE_GETGRNAM_R
  #define _DK_SDU_MIP__BASE__HAVE_GETGRGID_R
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
class GroupImpl {
public:

  union AnyId {
    void* opaque;
    gid_t system;
  };

  static inline void* getOpaque(gid_t id) throw() {
    GroupImpl::AnyId anyId;
    anyId.opaque = 0;
    anyId.system= id;
    return anyId.opaque;
  }
  
  static inline gid_t getSystem(void* id) throw() {
    GroupImpl::AnyId anyId;
    anyId.system = 0;
    anyId.opaque = id;
    return anyId.system;
  }
};
#endif // unix

Group::Group(const void* _id) throw(OutOfDomain) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((_id != 0) && (::IsValidSid((PSID)_id) != 0), OutOfDomain("Invalid group id", this));
  DWORD size = ::GetLengthSid((PSID)_id);
  id = new char[size];
  copy<char>((char*)id, (const char*)_id, size);
#else // unix
  assert((unsigned long)_id <= PrimitiveTraits<gid_t>::MAXIMUM, OutOfDomain("Invalid group id", this));
  id = (void*)_id; // we only cast away const 'cause we do not dereference the pointer
#endif // flavor
}

Group::Group(const Group& _copy) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (_copy.id == Group::INVALID) {
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

Group& Group::operator=(const Group& eq) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (&eq != this) { // protect against self assignment
    if (id != Group::INVALID) {
      delete[] (char*)id;
    }
    if (eq.id == Group::INVALID) {
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

bool Group::operator==(const Group& eq) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if ((id == Group::INVALID) || (eq.id == Group::INVALID)) {
    return (id == Group::INVALID) && (eq.id == Group::INVALID);
  }
  return ::EqualSid((PSID)id, (PSID)eq.id) != 0;
#else // unix
  return id == eq.id;
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
    id = GroupImpl::getOpaque(entry->gr_gid);
  #else
    #warning Group::Group(const String& name) uses non-reentrant getgrnam
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrnam(name.getElements());
    assert(entry != 0, GroupException(this));
    id = GroupImpl::getOpaque(entry->gr_gid);
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
  int result = ::getpwuid_r(GroupImpl::getSystem(id), &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result == 0, GroupException(this));
  id = GroupImpl::getOpaque(entry->pw_gid);
#endif // flavor
}

String Group::getName() const throw(GroupException) {
  if (id == User::INVALID) {
    return MESSAGE("UNKNOWN");
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SID_NAME_USE sidType;
  char name[4096]; // TAG: what is the maximum size
  DWORD nameSize = sizeof(name);
  char domainName[64]; // TAG: what is the maximum size
  DWORD domainNameSize = sizeof(domainName);
  assert(::LookupAccountSid(0,
                            (PSID)id,
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
    int result = ::getgrgid_r(GroupImpl::getSystem(id), &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    return String(entry->gr_name);
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(GroupImpl::getSystem(id));
    assert(entry != 0, GroupException(this));
    return String(entry->gr_name);
  #endif
#endif // flavor
}

Array<String> Group::getMembers() const throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Array<String> result;

  SID_NAME_USE sidType;
  char name[4096]; // TAG: what is the maximum size
  DWORD nameSize = sizeof(name);
  char domainName[64]; // TAG: what is the maximum size
  DWORD domainNameSize = sizeof(domainName);
  assert(::LookupAccountSid(0,
                            (PSID)id,
                            name,
                            &nameSize,
                            domainName,
                            &domainNameSize,
                            &sidType) != 0,
         GroupException("Unable to lookup name", this)
  );
//   if (domainName[0] != 0) {
//     return String(domainName) + MESSAGE("\\") + String(name);
//   } else {
//     return String(name); // TAG: does nameSize hold length of name
//   }

  WideString wideName(name);
  GROUP_USERS_INFO_0* buffer = 0;
  DWORD numberOfEntries = 0;
  DWORD totalEntries = 0;
  NET_API_STATUS status = ::NetGroupGetUsers(0, // use local machine
                                             wideName.getElements(),
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
    int result = ::getgrgid_r(GroupImpl::getSystem(id), &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    Array<String> members;
    char** memberName = entry->gr_mem;
    while (memberName) {
      members.append(*memberName++);
    }
    return members;
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(GroupImpl::getSystem(id));
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

Group::~Group() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (id != Group::INVALID) {
    delete[] (char*)id;
  }
#else // unix
#endif // flavor
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Group& value) throw(IOException) {
  const void* opaqueId = value.getId();
  if (opaqueId == Group::INVALID) {
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
