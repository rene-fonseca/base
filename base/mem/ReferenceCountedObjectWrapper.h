/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This is a wrapper for a Reference Counted Object. Use this class if you need
  to reference count an object of a class that is not a subclass of
  ReferenceCountedObject. Allocate objects on the heap not the stack.
  
  @short Reference counted object wrapper.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedObjectWrapper : public ReferenceCountedObject {
private:

  /** Ordinary object. */
  TYPE* object;
public:

  /**
    Initializes the wrapper.

    @param value The object to be reference counted.
  */
  inline ReferenceCountedObjectWrapper(TYPE* value) throw()
    : object(value) {
  }

  /**
    Returns the object.
  */
  inline TYPE* operator->() throw() {
    return object;
  }

  /**
    Returns the object.
  */
  inline TYPE& operator*() throw(NullPointer) {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }

  /**
    Type cast to pointer.
  */
  inline operator TYPE*() const throw() {
    return object;
  }

  /**
    Destroys the wrapper.
  */
  inline ~ReferenceCountedObjectWrapper() /*throw(...)*/ {
    if (object) {
      delete object;
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
