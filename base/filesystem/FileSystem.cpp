/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/filesystem/FileSystem.h>
#include <base/concurrency/Thread.h>

#if defined(__win32__)
#else // __unix__
  #include <unistd.h>
  #include <sys/types.h>
  #include <limits.h> // defines PATH_MAX
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String getCurrentFolder() throw(Exception) {
  Allocator<char>* buffer = Thread::getLocalStorage();
  ASSERT(buffer->getSize() > PATH_MAX);
  if (::getcwd(buffer->getElements(), buffer->getSize())) {
    throw Exception("Unable to get current folder");
  }
  return String(buffer->getElements());
}

void setCurrentFolder(const String& path) throw(Exception) {
  if (::chdir(path)) {
    throw Exception("Unable to set current folder");
  }
}

bool FileSystem::fileExists(const String& path) throw() {
  return true; // no impl
}

bool FileSystem::folderExists(const String& path) throw() {
  return true; // no impl
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
