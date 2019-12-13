/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/collection/List.h>
#include <base/collection/Array.h>
#include <base/net/Socket.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: must support sockets and events

class _COM_AZURE_DEV__BASE__API WaitForObjects : public Object {
private:
  
  List<Object*> objects;
  Array<OperatingSystem::Handle> handles;
public:

  /**
    Initializes synchronization object.
  */
  WaitForObjects() noexcept;

  /**
    Returns the maximum number of objects allowed.
  */
  unsigned int getMaximumNumberOfObjects() const noexcept;
  
  void registerObject(Object* object) throw(OutOfDomain);

  void deregisterObject(Object* object) throw(OutOfDomain);

  /**
  */
  void registerObject(Socket socket) throw(OutOfDomain);
  
  /**
  */
  void deregisterObject(Socket socket) noexcept;
  
  /**
  */
  void deregisterAll() noexcept;
  
  /**
  */
  Object* waitForAny() const noexcept;
  
  /**
    @param timeout The timeout period.
  */
  Object* waitForAny(unsigned int timeout) const noexcept;
  
  /**
    Wait for all the objects to be signaled.
  */
  void waitForAll() const noexcept;
  
  /**
    @param timeout The timeout period.
  */
  bool waitForAll(unsigned int timeout) const noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
