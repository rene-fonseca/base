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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
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

Group::Group(unsigned long long _id) throw(OutOfDomain) : id(_id) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // not implemented
#else // unix
  assert(id <= PrimitiveTraits<gid_t>::MAXIMUM, OutOfDomain("Invalid group id", this));
#endif // flavor
}

Group::Group(const String& name) throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE__GETGRNAM_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Allocator<char>* buffer = Thread::getLocalStorage();
    struct group grp;
    struct group* entry;
    int result = ::getgrnam_r(name.getElements(), &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    id = entry->gr_gid;
  #else
    #warning Using non-reentrant getgrnam (CYGWIN)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrnam(name.getElements());
    assert(entry != 0, GroupException(this));
    id = entry->gr_gid;
  #endif
#endif // flavor
}

Group::Group(const User& user) throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwuid_r(id, &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result, GroupException(this));
  id = entry->pw_gid;
#endif // flavor
}

Group::Group(const Group& copy) throw() : id(copy.id) {
}

String Group::getName() const throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE__GETGRNAM_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Allocator<char>* buffer = Thread::getLocalStorage();
    struct group grp;
    struct group* entry;
    int result = ::getgrgid_r(id, &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    return String(entry->gr_name);
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(id);
    assert(entry != 0, GroupException(this));
    return String(entry->gr_name);
  #endif
#endif // flavor
}

Array<String> Group::getMembers() const throw(GroupException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE__GETGRGID_R)
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    Allocator<char>* buffer = Thread::getLocalStorage();
    struct group grp;
    struct group* entry;
    int result = ::getgrgid_r(id, &grp, buffer->getElements(), buffer->getSize(), &entry);
    assert(result == 0, GroupException(this));
    Array<String> members;
    char** memberName = entry->gr_mem;
    while (memberName) {
      members.append(*memberName++);
    }
    return members;
  #else
    //long sysconf(_SC_GETGR_R_SIZE_MAX);
    struct group* entry = ::getgrgid(id);
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
