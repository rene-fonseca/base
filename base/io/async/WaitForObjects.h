/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__WAIT_FOR_OBJECTS_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__WAIT_FOR_OBJECTS_H

#include <base/Object.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: must support sockets and events

class WaitForObjects : public Object {
private:
  
  Allocater<Object*> objects;
  Allocater<OperatingSystem::Handle> handles;
public:

  /**
    Initializes synchronization object.
  */
  WaitForObjects() throw();

  /**
    Returns the maximum number of objects allowed.
  */
  unsigned int getMaximumNumberOfObjects() const throw();
  
  /**
  */
  void registerObject(Socket socket) throw(OutOfDomain);
  
  /**
  */
  void deregisterObject(Socket socket) throw();
  
  /**
  */
  void deregisterAll() throw();
  
  /**
  */
  Object* waitForAny() const throw();
  
  /**
    @param timeout The timeout period.
  */
  Object* waitForAny(unsigned int timeout) const throw();
  
  /**
    Wait for all the objects to be signaled.
  */
  void waitForAll() const throw();
  
  /**
    @param timeout The timeout period.
  */
  bool waitForAll(unsigned int timeout) const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
