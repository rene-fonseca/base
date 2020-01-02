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

#include <base/AutomationObject.h>
#include <base/mem/NullPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Automation pointer that allows transfer of ownership and automatic deletion
  of the object. Do not construct more than one automation pointer from the
  'normal' pointer.

  @code
  void MyOtherClass::myOtherMethod(MyResource resource) {
    // ...
    bassert(condition, MyException(this));
    // ...
  }
  
  void MyClass::myMethod() {
    OwnershipPointer<MyResource> resource = new MyResource();
    MyOtherClass* myOtherObject = getOtherObject();
    myOtherObject->myOtherMethod(resource);
    MyResource* result = resource.relinquishOwnership();
    // ...
  }
  @endcode
  
  @short Ownership automation pointer.
  @ingroup memory
  @see ArrayOwnershipPointer
  @version 1.0
*/

template<class TYPE>
class OwnershipPointer : public AutomationObject {
private:

  /** Pointer to object. */
  TYPE* object = nullptr;
public:

  /**
    Initializes the ownership pointer as nullptr.
  */
  inline OwnershipPointer() noexcept
  {
  }
  
  /**
    Initializes the ownership pointer.
    
    @param object The object pointer to be automated.
  */
  inline OwnershipPointer(TYPE* _object) noexcept : object(_object)
  {
  }
  
  /**
    Copy constructor. Transfers ownership from copy to this object. The
    original owner loses the ownership.
  */
  inline OwnershipPointer(OwnershipPointer& copy) noexcept
    : object(copy.relinquishOwnership())
  {
  }
  
  /**
    Assignment operator.
  */
  inline OwnershipPointer& operator=(OwnershipPointer& assign)
  {
    if (&assign != this) { // protect against self assignment
      if (object) {
        delete object;
      }
      object = assign.relinquishOwnership();
    }
    return *this;
  }
  
  /**
    Assignment operator.
  */
  template<class POLY>
  inline OwnershipPointer& operator=(OwnershipPointer<POLY>& assign)
  {
    if (assign.object != object) { // protect against self assignment
      if (object) {
        delete object;
      }
      object = assign.relinquishOwnership();
    }
    return *this;
  }

  /**
    Assignment operator.
  */
  inline OwnershipPointer& operator=(TYPE* object)
  {
    if (this->object) {
      delete this->object;
    }
    this->object = object;
    return *this;
  }

  /**
    Makes the automation pointer relinquish its ownership. Please note that the
    pointer may not have the ownership to begin with.
    
    @return Pointer to object.
  */
  inline TYPE* relinquishOwnership() noexcept
  {
    TYPE* temp = object;
    object = nullptr;
    return temp;
  }
  
  /**
    Returns the pointer to mutable object.
  */
  inline TYPE* getValue() noexcept
  {
    return object;
  }

  /**
    Returns the pointer to constant object.
  */
  inline const TYPE* getValue() const noexcept
  {
    return object;
  }
  
  /**
    Returns mutable object.
  */
  inline TYPE& operator*()
  {
    if (!object) {
      _throw NullPointer(this);
    }
    return *object;
  }

  /**
    Returns constant object.
  */
  inline const TYPE& operator*() const
  {
    if (!object) {
      _throw NullPointer(this);
    }
    return *object;
  }
  
  /**
    Returns object for modifying access.
  */
  inline TYPE* operator->()
  {
    if (!object) {
      _throw NullPointer(this);
    }
    return object;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const TYPE* operator->() const
  {
    if (!object) {
      _throw NullPointer(this);
    }
    return object;
  }
  
  /**
    Destroys the ownership pointer (and the object).
  */
  inline ~OwnershipPointer()
  {
    if (object) {
      delete object;
      object = nullptr;
    }
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
