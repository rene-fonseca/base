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
  of an array. Do not construct more than one automation pointer from the
  'normal' pointer.
  
  @code
  void MyOtherClass::myOtherMethod(MyResource resource) throw(MyException)
  {
    // ...
    bassert(condition, MyException(this));
    // ...
  }
  
  void MyClass::myMethod() throw(MemoryException, MyException)
  {
    ArrayOwnershipPointer<int> buffer = new int[BUFFER_SIZE];
    MyOtherClass* myOtherObject = getOtherObject();
    myOtherObject->myOtherMethod(resource);
    int* direct = buffer.relinquishOwnership();
    // ...
  }
  @endcode
  
  @short Array ownership automation pointer.
  @ingroup memory
  @see OwnershipPointer
  @version 1.0
*/

template<class TYPE>
class ArrayOwnershipPointer : public AutomationObject {
private:

  /** Pointer to object. */
  TYPE* object = nullptr;
public:

  /**
    Initializes the ownership pointer as nullptr.
  */
  inline ArrayOwnershipPointer() noexcept
  {
  }
  
  /**
    Initializes the ownership pointer.
    
    @param object The object pointer to be automated.
  */
  inline ArrayOwnershipPointer(TYPE* _object) noexcept : object(_object)
  {
  }
  
  /**
    Copy constructor. Transfers ownership from copy to this object. The
    original owner loses the ownership.
  */
  inline ArrayOwnershipPointer(ArrayOwnershipPointer& copy) noexcept
    : object(copy.relinquishOwnership())
  {
  }
  
  /**
    Assignment operator.
  */
  inline ArrayOwnershipPointer& operator=(ArrayOwnershipPointer& assign)
  {
    if (&assign != this) { // protect against self assignment
      if (object) {
        delete[] object;
      }
      object = assign.relinquishOwnership();
    }
    return *this;
  }
  
  /**
    Assignment operator.
  */
  template<class POLY>
  inline ArrayOwnershipPointer& operator=(ArrayOwnershipPointer<POLY>& assign)
  {
    if (assign.object != object) { // protect against self assignment
      if (object) {
        delete[] object; // TAG: refactor
      }
      object = assign.relinquishOwnership();
    }
    return *this;
  }

  /**
    Assignment operator.
  */
  inline ArrayOwnershipPointer& operator=(TYPE* object)
  {
    if (this->object) {
      delete[] this->object;
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
  inline TYPE& operator*() throw(NullPointer)
  {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }

  /**
    Returns constant object.
  */
  inline const TYPE& operator*() const throw(NullPointer)
  {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }
  
  /**
    Returns object for modifying access.
  */
  inline TYPE* operator->() noexcept
  {
    return object;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const TYPE* operator->() const noexcept
  {
    return object;
  }
  
  /**
    Destroys the ownership pointer (and the object).
  */
  inline ~ArrayOwnershipPointer()
  {
    if (object) {
      delete[] object;
    }
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
