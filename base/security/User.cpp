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
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <sys/types.h>
  #include <pwd.h>
  #include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

//BOOL CheckTokenMembership(HANDLE TokenHandle, PSID SidToCheck, PBOOL IsMember);
/*  static User getUser() throw() {
    char name[UNLEN + 1];
    BOOL result = ::GetUserName(name, sizeof(name));
    return User(name);

    HANDLE token;
    BOOL ::OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, &token);

    BOOL res = ::GetTokenInformation(HANDLE TokenHandle,
    TOKEN_INFORMATION_CLASS TokenInformationClass,
    LPVOID TokenInformation,
    DWORD TokenInformationLength,
    PDWORD ReturnLength
    );
*/

User User::getCurrentUser() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return User(PrimitiveTraits<unsigned long long>::MAXIMUM);
#else // unix
  uid_t uid = ::getuid();
  return User(uid);
#endif // flavor
}

User::User(unsigned long long _id) throw(OutOfDomain) : id(_id) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // not implemented
#else // unix
  assert(id <= PrimitiveTraits<uid_t>::MAXIMUM, OutOfDomain("Invalid user id", this));
#endif // flavor
}

User::User(const User& copy) throw() : id(copy.id) {
}

User::User(const String& name) throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  //long sysconf(_SC_GETPW_R_SIZE_MAX);
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwnam_r(name.getElements(), &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result, UserException(this));
  id = entry->pw_uid;
#endif // flavor
}

String User::getName() const throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwuid_r(id, &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result, UserException(this));
  return String(entry->pw_name);
#endif // flavor
}

String User::getHomeFolder() const throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwuid_r(id, &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result, UserException(this));
  return String(entry->pw_dir);
#endif // flavor
}

bool User::isMemberOf(const Group& group) throw(UserException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(this);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
