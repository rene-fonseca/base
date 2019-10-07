/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/filesystem/FolderMonitor.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FolderMonitor::FolderMonitor(const String& path) throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  handle = ::FindFirstChangeNotification(
    path.getElements(),
    FALSE, // do not watch subfolders
    FILE_NOTIFY_CHANGE_FILE_NAME |
    FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_ATTRIBUTES |
    FILE_NOTIFY_CHANGE_SIZE |
    FILE_NOTIFY_CHANGE_LAST_WRITE
  );
  bassert(handle != OperatingSystem::INVALID_HANDLE, ResourceException(this));
#else // unix
#endif // flavor
}

bool FolderMonitor::isSignaled() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle, 0);
  if (result == WAIT_OBJECT_0) {
    ::FindNextChangeNotification(handle); // should never fail
  }
  return result == WAIT_OBJECT_0;
#else // unix
  return false;
#endif // flavor
}

void FolderMonitor::wait() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::WaitForSingleObject(handle, INFINITE);
  ::FindNextChangeNotification(handle); // should never fail
#else // unix
#endif // flavor
}

bool FolderMonitor::wait(unsigned int milliseconds) const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle, minimum(milliseconds, 999999999U));
  ::FindNextChangeNotification(handle); // should never fail
  return result == WAIT_OBJECT_0;
#else // unix
  return false;
#endif // flavor
}

FolderMonitor::~FolderMonitor() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::FindCloseChangeNotification(handle); // should never fail
#else // unix
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
