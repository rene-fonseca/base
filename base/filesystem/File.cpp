/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/filesystem/File.h>

#if defined(__win32__)
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

File::File(const String& path) throw(FileException) : path(path) {
#if defined(__win32__)
#else // __unix__
  struct stat buffer;
  if (stat(path, &buffer) || (!S_ISREG(buffer.st_mode))) {
    throw FileException("Not a file");
  }
  size = buffer.st_size;
  access = buffer.st_atime;
  modification = buffer.st_mtime;
  change = buffer.st_ctime;
#endif
}

long long File::getSize() throw() {
  return size;
}

Date File::getLastModification() throw() {
  return modification;
}

Date File::getLastAccess() throw() {
  return access;
}

Date File::getLastChange() throw() {
  return change;
}

String File::getPath() throw() {
  return path;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
