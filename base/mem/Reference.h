/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_H

#include <base/CastException.h>
#include <base/Functor.h>
#include <base/collection/Hash.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer for reference counting objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. This pointer can only be used to reference count
  objects of type ReferenceCountedObject but is faster than ReferenceCounter.
  The Reference class is relocateable.
  
  @short Automation pointer that counts the number of references to an object.
  @ingroup memory
  @see ReferenceCountedObject ReferenceCounter
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Reference {
private:
  
  /** Pointer to shared reference counted object. */
  TYPE* value;
public:

  /**
    Initializes an automation pointer as invalid (i.e. 0).
  */
  inline Reference() throw() : value(0) {
  }

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.
    
    @code
    Reference<MyClass> object = new MyClass();
    @endcode
    
    @param value The desired pointer value.
  */
  inline Reference(TYPE* _value) throw() : value(_value) {
    if (value) {
      ReferenceCountedObject::ReferenceImpl(*value).addReference();
    }
  }
  
  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline Reference(const Reference& copy) throw() : value(copy.value) {
    if (value) {
      ReferenceCountedObject::ReferenceImpl(*value).addReference();
    }
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline Reference(const Reference<POLY>& copy) throw()
    : value(copy.getValue()) {
    if (value) {
      ReferenceCountedObject::ReferenceImpl(*value).addReference();
    }
  }

  /**
    Returns true if the object may be cast to the specified type.
  */
  template<class POLY>
  inline bool isType() const throw() {
    return dynamic_cast<const POLY*>(value);
  }
  
  /**
    Dynamic cast to the specified type. Raises CastException if the reference
    is invalid or the reference cannot be cast to the specified type.
  */
  template<class POLY>
  inline Reference<POLY> cast() throw(CastException) {
    POLY* temp = dynamic_cast<POLY*>(value);
    assert(temp, CastException(this));
    return temp;
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline Reference& operator=(const Reference& eq) /*throw(...)*/ {
    setValue(eq.value); // no need to protect against self assignment
    return *this;
  }
  
  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline Reference& operator=(const Reference<POLY>& eq) /*throw(...)*/ {
    setValue(eq.getValue()); // no need to protect against self assignment
    return *this;
  }

  /**
    Assignment of normal pointer to this automation pointer.
  */
  inline Reference& operator=(TYPE* eq) throw() {
    setValue(eq);
    return *this;
  }

  /**
    Returns true if the references are equal.
  */
  inline bool operator==(const Reference& eq) const throw() {
    return value == eq.value;
  }

  /**
    Returns true if the references are non-equal.
  */
  inline bool operator!=(const Reference& eq) const throw() {
    return value != eq.value;
  }
  
  /**
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules aren't violated. Invocation of this method
    should be avoided.
  */
  inline TYPE* getValue() const throw() {
    return value;
  }
  
  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(TYPE* _value) /*throw(...)*/ {
    if (_value) { // skip if pointer is invalid
      ReferenceCountedObject::ReferenceImpl(*_value).addReference();
    }
    if (value) { // skip if pointer is invalid
      if (ReferenceCountedObject::ReferenceImpl(*value).removeReference()) {
        delete value;
      }
    }
    value = _value;
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
    the default copy constructor for this to work.
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
    Invalidates the reference.
  */
  inline void invalidate() /*throw(...)*/ {
    if (value) { // skip if pointer is invalid
      if (ReferenceCountedObject::ReferenceImpl(*value).removeReference()) {
        delete value;
      }
    }
    value = 0;
  }
  
  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const throw() {
    return value != 0;
  }

  /**
    Returns the reference counted object.
    
    @deprecated
  */
  inline TYPE& operator*() throw(NullPointer) {
    if (!value) {
      throw NullPointer(this);
    }
    return *value;
  }
  
  /**
    Returns the reference counted object.

    @deprecated
  */
  inline const TYPE& operator*() const throw(NullPointer) {
    if (!value) {
      throw NullPointer(this);
    }
    return *value;
  }

  /**
    Returns the reference counted object.
  */
  inline TYPE* operator->() throw() {
    return value;
  }
  
  /**
    Returns the reference counted object.
  */
  inline const TYPE* operator->() const throw() {
    return value;
  }

  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline operator bool() const throw() {
    return value;
  }
  
  /**
    Destroys the automation pointer.
  */
  inline ~Reference() /*throw(...)*/ {
    if (value) { // skip if pointer is invalid
      if (ReferenceCountedObject::ReferenceImpl(*value).removeReference()) {
        delete value;
      }
    }
  }
};

template<class TYPE>
class Relocateable<Reference<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<void*>::IS_RELOCATEABLE;
};

template<class TYPE>
class Hash<Reference<TYPE> > {
public:

  inline unsigned long operator()(const Reference<TYPE>& value) throw() {
    Hash<void*> hash;
    return hash(value.getValue());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
