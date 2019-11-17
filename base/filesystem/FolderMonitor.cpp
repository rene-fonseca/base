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
#include <base/filesystem/FolderMonitor.h>
#include <base/Profiler.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FolderMonitor::FolderMonitor(const String& path) throw(ResourceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  handle = ::FindFirstChangeNotification(
    toWide(path).c_str(),
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle, 0);
  if (result == WAIT_OBJECT_0) {
    ::FindNextChangeNotification(handle); // should never fail
  }
  return result == WAIT_OBJECT_0;
#else // unix
  return false;
#endif // flavor
}

void FolderMonitor::wait() const throw()
{
  Profiler::WaitTask profile("FolderMonitor::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::WaitForSingleObject(handle, INFINITE);
  ::FindNextChangeNotification(handle); // should never fail
#else // unix
#endif // flavor
}

bool FolderMonitor::wait(unsigned int milliseconds) const throw()
{
  Profiler::WaitTask profile("FolderMonitor::wait()");
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle, minimum(milliseconds, 999999999U));
  ::FindNextChangeNotification(handle); // should never fail
  return result == WAIT_OBJECT_0;
#else // unix
  return false;
#endif // flavor
}

FolderMonitor::~FolderMonitor() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::FindCloseChangeNotification(handle); // should never fail
#else // unix
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
