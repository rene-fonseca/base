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
//#include <base/io/async/WaitForMultipleObjects.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

WaitForObjects::WaitForObjects() throw() {
}

unsigned int WaitForObjects::getMaximumNumberOfObjects() const throw() {
  return MAXIMUM_WAIT_OBJECTS;
}
  
void WaitForObjects::registerObject(Object* object) throw(OutOfDomain) { // FIXME: exception name
  bassert(handles.getSize() == MAXIMUM_WAIT_OBJECTS, OutOfDomain());
  // throw exception if object already in array
  objects.append(object);
  handles.append(object.getHandle());
}

void WaitForObjects::deregisterObject(Object* object) throw(...) {
// throw exception if not present
// handles.remove(i);
// objects.remove(i);
}

void WaitForObjects::deregisterAll() throw() {
  objects.removeAll();
  handles.removeAll();
}
  
Object* WaitForObjects::waitForAny() const throw() {
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), FALSE, INFINITE, TRUE);
  } while (result == WAIT_IO_COMPLETION);
  // FIXME: abandoned
  return objects.getElements[result - WAIT_OBJECT_0];
}
  
Object* WaitForObjects::waitForAny(unsigned int timeout) const throw() {
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), FALSE, timeout, TRUE);
  } while (result == WAIT_IO_COMPLETION);
  if (result == WAIT_TIMEOUT) {
    return 0;
  } else {
    return objects.getElements[result - WAIT_OBJECT_0];
  }
}

void WaitForObjects::waitForAll() const throw() {
  DWORD result;
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), TRUE, INFINITE, TRUE);
  } while (result == WAIT_IO_COMPLETION);
}
  
bool WaitForObjects::waitForAll(unsigned int timeout) const throw() {
  DWORD result;
  do {
    ::WaitForMultipleObjectsEx(numberOfHandles, handles.getElements(), TRUE, timeout, TRUE);
  } while (result == WAIT_IO_COMPLETION);
  return (result != WAIT_TIMEOUT);
}
  
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
