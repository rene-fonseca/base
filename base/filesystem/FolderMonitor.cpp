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
#include <base/filesystem/FileSystemException.h>
#include <base/ResourceHandle.h>
#include <base/Profiler.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class FolderMonitorHandle : public ResourceHandle {
public:
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle = 0;
#else
#endif

  ~FolderMonitorHandle()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (::FindCloseChangeNotification(handle)) { // should never fail
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
#else
#endif
  }
};

namespace {

  inline FolderMonitorHandle* toFolderMonitorHandle(const AnyReference& handle)
  {
    if (handle) { // TAG: we do not want a dynamic cast - use static cast
      return handle.cast<FolderMonitorHandle>().getValue();
    }
    return nullptr;
  }
}

FolderMonitor::FolderMonitor(const String& path)
{
  Profiler::ResourceCreateTask profile("FolderMonitor::FolderMonitor()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle = ::FindFirstChangeNotification(
    ToWCharString(path),
    FALSE, // do not watch subfolders
    FILE_NOTIFY_CHANGE_FILE_NAME |
    FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_ATTRIBUTES |
    FILE_NOTIFY_CHANGE_SIZE |
    FILE_NOTIFY_CHANGE_LAST_WRITE
  );
  bassert(handle != OperatingSystem::INVALID_HANDLE, ResourceException(this));
  Reference<FolderMonitorHandle> _handle = new FolderMonitorHandle();
  _handle->handle = handle;
  this->handle = _handle;
#else // unix
#endif // flavor
}

bool FolderMonitor::isSignaled() const
{
  FolderMonitorHandle* handle = toFolderMonitorHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle, 0);
  if (result == WAIT_OBJECT_0) {
    ::FindNextChangeNotification(handle->handle); // should never fail
  }
  return result == WAIT_OBJECT_0;
#else // unix
  return false;
#endif // flavor
}

void FolderMonitor::wait() const
{
  Profiler::WaitTask profile("FolderMonitor::wait()");
  
  FolderMonitorHandle* handle = toFolderMonitorHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::WaitForSingleObject(handle->handle, INFINITE);
  ::FindNextChangeNotification(handle->handle); // should never fail
#else // unix
#endif // flavor
}

bool FolderMonitor::wait(unsigned int milliseconds) const
{
  Profiler::WaitTask profile("FolderMonitor::wait()");
  
  FolderMonitorHandle* handle = toFolderMonitorHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(handle->handle, minimum(milliseconds, 999999999U));
  ::FindNextChangeNotification(handle->handle); // should never fail
  return result == WAIT_OBJECT_0;
#else // unix
  return false;
#endif // flavor
}

FolderMonitor::~FolderMonitor() noexcept
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
