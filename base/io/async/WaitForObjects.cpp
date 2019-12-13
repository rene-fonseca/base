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
#include <base/io/async/WaitForObjects.h>
#include <base/Profiler.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

#if 0

#include <windows.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

WaitForObjects::WaitForObjects() noexcept
{
}

unsigned int WaitForObjects::getMaximumNumberOfObjects() const throw()
{
  return MAXIMUM_WAIT_OBJECTS;
}

void WaitForObjects::registerObject(Object* object) throw(OutOfDomain)
{ // FIXME: exception name
  bassert(object, OutOfDomain());
  bassert(handles.getSize() == MAXIMUM_WAIT_OBJECTS, OutOfDomain());
  // throw exception if object already in array
  objects.append(object);
  // TAG: handles.append(object->getHandle());
}

void WaitForObjects::deregisterObject(Object* object)
{
// throw exception if not present
// handles.remove(i);
// objects.remove(i);
}

void WaitForObjects::deregisterAll() noexcept
{
  objects.removeAll();
  handles.removeAll();
}
  
Object* WaitForObjects::waitForAny() const throw()
{
  Profiler::WaitTask profile("WaitForObjects::waitForAny()");
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), FALSE, INFINITE, TRUE);
  } while (result == WAIT_IO_COMPLETION);
  // FIXME: abandoned
  return objects.getElements[result - WAIT_OBJECT_0];
}
  
Object* WaitForObjects::waitForAny(unsigned int timeout) const throw()
{
  Profiler::WaitTask profile("WaitForObjects::waitForAny()");
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), FALSE, timeout, TRUE);
  } while (result == WAIT_IO_COMPLETION);
  if (result == WAIT_TIMEOUT) {
    return 0;
  } else {
    return objects.getElements[result - WAIT_OBJECT_0];
  }
}

void WaitForObjects::waitForAll() const throw()
{
  Profiler::WaitTask profile("WaitForObjects::waitForAll()");
  DWORD result = 0;
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), TRUE, INFINITE, TRUE);
  } while (result == WAIT_IO_COMPLETION);
}
  
bool WaitForObjects::waitForAll(unsigned int timeout) const throw()
{
  Profiler::WaitTask profile("WaitForObjects::waitForAll()");
  DWORD result = 0;
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), TRUE, timeout, TRUE);
  } while (result == WAIT_IO_COMPLETION);
  return (result != WAIT_TIMEOUT);
}
  
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

#endif
