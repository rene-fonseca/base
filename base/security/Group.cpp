/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/Group.h>
#include <base/concurrency/Thread.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#include <windows.h>
#else // unix
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Group::Group(unsigned long long _id) throw() : id(_id) {
}

Group::Group(const String& name) throw(Exception) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  //long sysconf(_SC_GETGR_R_SIZE_MAX);
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct group grp;
  struct group* entry;
//  assert(id < Maximum<gid_t>(), Exception("Invalid group"));
  int result = ::getgrnam_r(name.getElements(), &grp, buffer->getElements(), buffer->getSize(), &entry);
  assert(result == 0, Exception(this));
  id = entry->gr_gid;
#endif // flavor
}

Group::Group(const User& user) throw(Exception) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct passwd pw;
  struct passwd* entry;
//  assert(id < Maximum<uid_t>(), Exception("Invalid user"));
  int result = ::getpwuid_r(id, &pw, buffer->getElements(), buffer->getSize(), &entry);
  assert(result, Exception(this));
  id = entry->pw_gid;
#endif // flavor
}

Group::Group(const Group& copy) throw() : id(copy.id) {
}

String Group::getName() const throw(Exception) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  //long sysconf(_SC_GETGR_R_SIZE_MAX);
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct group grp;
  struct group* entry;
//  assert(id < Maximum<gid_t>(), Exception("Invalid group"));
  int result = ::getgrgid_r(id, &grp, buffer->getElements(), buffer->getSize(), &entry);
  assert(result == 0, Exception(this));
  return String(entry->gr_name);
#endif // flavor
}

Array<String> Group::getMembers() const throw(Exception) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  //long sysconf(_SC_GETGR_R_SIZE_MAX);
  Allocator<char>* buffer = Thread::getLocalStorage();
  struct group grp;
  struct group* entry;
//  assert(id < Maximum<gid_t>(), Exception("Invalid group"));
  int result = ::getgrgid_r(id, &grp, buffer->getElements(), buffer->getSize(), &entry);
  assert(result == 0, Exception(this));
  Array<String> members;
  char** memberName = entry->gr_mem;
  while (memberName) {
    members.append(*memberName++);
  }
  return members;
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
