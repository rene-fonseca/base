/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ARRAY_OWNERSHIP_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__ARRAY_OWNERSHIP_POINTER_H

#include <base/AutomationObject.h>
#include <base/mem/NullPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer that allows transfer of ownership and automatic deletion
  of an array. Do not construct more than one automation pointer from the
  'normal' pointer.
  
  @code
  void MyOtherClass::myOtherMethod(MyResource resource) throw(MyException) {
    // ...
    assert(condition, MyException(this));
    // ...
  }
  
  void MyClass::myMethod() throw(MemoryException, MyException) {
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
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class ArrayOwnershipPointer : public AutomationObject {
private:

  /** Pointer to object. */
  TYPE* object;
public:

  /**
    Initializes the ownership pointer as 0.
  */
  inline ArrayOwnershipPointer() throw() : object(0) {
  }
  
  /**
    Initializes the ownership pointer.
    
    @param object The object pointer to be automated.
  */
  inline ArrayOwnershipPointer(TYPE* _object) throw() : object(_object) {
  }
  
  /**
    Copy constructor. Transfers ownership from copy to this object. The
    original owner loses the ownership.
  */
  inline ArrayOwnershipPointer(ArrayOwnershipPointer& copy) throw()
    : object(copy.relinquishOwnership()) {
  }
  
  /**
    Assignment operator.
  */
  inline ArrayOwnershipPointer& operator=(
    ArrayOwnershipPointer& eq) /*throw(...)*/ {
    if (&eq != this) { // protect against self assignment
      if (object) {
        delete[] object;
      }
      object = eq.relinquishOwnership();
    }
    return *this;
  }
  
  /**
    Assignment operator.
  */
  template<class POLY>
  inline ArrayOwnershipPointer& operator=(
    ArrayOwnershipPointer<POLY>& eq) /*throw(...)*/ {
    if (eq.object != object) { // protect against self assignment
      if (object) {
        delete[] object;
      }
      object = eq.relinquishOwnership();
    }
    return *this;
  }

  /**
    Assignment operator.
  */
  inline ArrayOwnershipPointer& operator=(TYPE* object) /*throw(...)*/ {
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
  inline TYPE* relinquishOwnership() throw() {
    TYPE* temp = object;
    object = 0;
    return temp;
  }
  
  /**
    Returns the pointer to mutable object.
  */
  inline TYPE* getValue() throw() {
    return object;
  }

  /**
    Returns the pointer to constant object.
  */
  inline const TYPE* getValue() const throw() {
    return object;
  }
  
  /**
    Returns mutable object.
  */
  inline TYPE& operator*() throw(NullPointer) {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }

  /**
    Returns constant object.
  */
  inline const TYPE& operator*() const throw(NullPointer) {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }
  
  /**
    Returns object for modifying access.
  */
  inline TYPE* operator->() throw() {
    return object;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const TYPE* operator->() const throw() {
    return object;
  }
  
  /**
    Destroys the ownership pointer (and the object).
  */
  inline ~ArrayOwnershipPointer() /*throw(...)*/ {
    if (object) {
      delete[] object;
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
