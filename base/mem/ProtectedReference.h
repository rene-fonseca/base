/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Functor.h>
#include <base/collection/Hash.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>
#include <base/CastException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer for reference counting objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. This pointer can only be used to reference count
  objects of type ReferenceCountedObject but is faster than ReferenceCounter.
  The ProtectedReference class is relocateable.
  
  @short Automation pointer that counts the number of references to an object.
  @ingroup memory
  @see ReferenceCountedObject ReferenceCounter
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class ProtectedReference {
private:
  
  /** Pointer to shared reference counted object. */
  TYPE* value = nullptr;
  
  /**
    Sets the pointer value of this automation pointer. Raises the exceptions
    raised by the destructor of the object.
  */
  inline void setValue(TYPE* value) {
    ReferenceCountedObject::ReferenceImpl(*value).addReference();
    if (ReferenceCountedObject::ReferenceImpl(*this->value).removeReference()) {
      delete value;
    }
    this->value = value;
  }
public:

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized. Raises NullPointer is
    value is invalid.
    
    @param value The desired pointer value.
  */
  inline ProtectedReference(TYPE* _value) throw(NullPointer) : value(_value) {
    bassert(value, NullPointer(this));
    ReferenceCountedObject::ReferenceImpl(*value).addReference();
  }
  
  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ProtectedReference(const ProtectedReference& copy) throw()
    : value(copy.value) {
    ReferenceCountedObject::ReferenceImpl(*value).addReference();
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ProtectedReference(const ProtectedReference<POLY>& copy) throw()
    : value(copy.getValue()) {
    ReferenceCountedObject::ReferenceImpl(*value).addReference();
  }
  
  /**
    Dynamic cast. Raises CastException if unable to cast.
  */
  template<class POLY>
  inline ProtectedReference<POLY> cast() throw(CastException) {
    POLY* result = dynamic_cast<POLY*>(value);
    bassert(result, CastException(this));
    return result;
  }
  
  /**
    Dynamic cast. Raises CastException if unable to cast.
  */
  template<class POLY>
  inline const ProtectedReference<POLY> cast() const throw(CastException) {
    const POLY* result = dynamic_cast<const POLY*>(value);
    bassert(result, CastException(this));
    return result;
  }
  
  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism. Raises the exceptions raised by the destructor of the
    object.
  */
  inline ProtectedReference& operator=(
    const ProtectedReference& eq) {
    setValue(eq.value); // no need to protect against self assignment
    return *this;
  }
  
  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism. Raises the exceptions raised by the destructor of the
    object.
  */
  template<class POLY>
  inline ProtectedReference& operator=(
    const ProtectedReference<POLY>& eq) {
    setValue(eq.getValue()); // no need to protect against self assignment
    return *this;
  }
  
  /**
    Returns true if the references are equal.
  */
  inline bool operator==(const ProtectedReference& eq) const throw() {
    return value == eq.value;
  }

  /**
    Returns true if the references are non-equal.
  */
  inline bool operator!=(const ProtectedReference& eq) const throw() {
    return value != eq.value;
  }
  
  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return value &&
      ReferenceCountedObject::ReferenceImpl(*value).isMultiReferenced();
  }

  /**
    Makes a new copy of the reference counted object if referenced by more than
    one automation pointer. This member function is invocated by some classes
    before a object is modified. The reference counted object must implement
    the default copy constructor for this to work. Raises the exceptions raised
    by the default copy constructor of the object.
  */
  inline void copyOnWrite() /*throw(...)*/ {
    if (isMultiReferenced()) { // do we have the object for our self
      // remove one reference (no need to delete object since multi-referenced)
      TYPE* temp = new TYPE(*value);
      ReferenceCountedObject::ReferenceImpl(*temp).addReference();
      ReferenceCountedObject::ReferenceImpl(*value).removeReference();
      value = temp;
    }
  }
  
  /**
    Returns the reference counted object for modifying access.
  */
  inline TYPE* operator->() throw() {
    return value;
  }
  
  /**
    Returns the reference counted object for non-modifying access.
  */
  inline const TYPE* operator->() const throw() {
    return value;
  }
  
  /**
    Destroys the automation pointer. Raises the exceptions raised by the
    destructor of the object.
  */
  inline ~ProtectedReference() {
    if (ReferenceCountedObject::ReferenceImpl(*value).removeReference()) {
      delete value;
    }
  }
};

template<class TYPE>
class Relocateable<ProtectedReference<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<void*>::IS_RELOCATEABLE;
};

template<class TYPE>
class Hash<ProtectedReference<TYPE> > {
public:

  inline unsigned long operator()(
    const ProtectedReference<TYPE>& value) throw() {
    Hash<void*> hash;
    return hash(value.getValue());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE