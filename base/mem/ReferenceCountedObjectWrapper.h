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

#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This is a wrapper for a Reference Counted Object. Use this class if you need
  to reference count an object of a class that is not a subclass of
  ReferenceCountedObject. Allocate objects on the heap not the stack.
  
  @short Reference counted object wrapper.
  @ingroup memory
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedObjectWrapper : public ReferenceCountedObject {
private:

  /** Ordinary object. */
  TYPE* object = nullptr;
public:

  /**
    Initializes the wrapper.

    @param value The object to be reference counted.
  */
  inline ReferenceCountedObjectWrapper(TYPE* value) noexcept
    : object(value) {
  }

  /**
    Returns the object.
  */
  inline TYPE* operator->() noexcept {
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
  inline operator TYPE*() const noexcept {
    return object;
  }

  /**
    Destroys the wrapper.
  */
  inline ~ReferenceCountedObjectWrapper() {
    if (object) {
      delete object;
    }
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
