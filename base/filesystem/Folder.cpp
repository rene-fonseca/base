/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/filesystem/Folder.h>
#include <base/concurrency/Thread.h>

#if defined(__win32__)
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dirent.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Folder::Folder(const String& path) throw(FolderException) : path(path) {
#if defined(__win32__)
#else // __unix__
  struct stat buffer;
  if (stat(path, &buffer) || (!S_ISDIR(buffer.st_mode))) {
    throw FolderException("Not a folder");
  }
  access = buffer.st_atime;
  modification = buffer.st_mtime;
  change = buffer.st_ctime;
#endif
}

Folder Folder::getParent() const throw(FolderException) {
  return *this; // not impl
}

Array<String> Folder::getEntries() const throw(FolderException) {
  Array<String> result;
  Allocator<char>* buffer = Thread::getLocalStorage();

  DIR* directory;

  if ((directory = ::opendir(path)) == 0) {
    throw FolderException("Unable to open folder");
  }

  while (true) {
    int status;
    struct dirent* entry;

    if ((status = ::readdir_r(directory, (struct dirent*)buffer->getElements(), &entry)) != 0) {
      ::closedir(directory);
      throw FolderException("Unable to read folder entry");
    }

    if (entry == 0) { // stop if last entry has been read
      break;
    }

    result.append(String(entry->d_name));
  }

  if (::closedir(directory) != 0) {
    throw FolderException("Unable to close folder");
  }

  return result;
}

Date Folder::getLastModification() const throw() {
  return modification;
}

Date Folder::getLastAccess() const throw() {
  return access;
}

Date Folder::getLastChange() const throw() {
  return change;
}

String Folder::getPath() const throw() {
  return path;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
