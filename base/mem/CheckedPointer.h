/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__CHECKED_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__CHECKED_POINTER_H

#include <base/collection/Hash.h>
#include <base/mem/NullPointer.h>
#include <base/Functor.h>
#include <base/CastException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer which checks the pointer value before use.
  
  @short Checked automation pointer.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class CheckedPointer {
private:

  /** Pointer to object. */
  TYPE* value;
public:
  
  /**
    Initializes the automation pointer as 0.
  */
  inline CheckedPointer() throw() : value(0) {
  }
  
  /**
    Initializes the automation pointer.
    
    @param value The object pointer to be automated.
  */
  inline CheckedPointer(TYPE* _value) throw() : value(_value) {
  }

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline CheckedPointer(const CheckedPointer& copy) throw()
    : value(copy.value) {
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline CheckedPointer(const CheckedPointer<POLY>& copy) throw()
    : value(copy.getValue()) {
  }
  
  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline CheckedPointer& operator=(const CheckedPointer& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline CheckedPointer& operator=(const CheckedPointer<POLY>& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Invalidates the pointer.
  */
  inline void invalidate() throw() {
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
    Returns true if the pointers are equal.
  */
  inline bool operator==(const CheckedPointer& eq) const throw() {
    return value == eq.value;
  }
  
  /**
    Returns true if the pointers are non-equal.
  */
  inline bool operator!=(const CheckedPointer& eq) const throw() {
    return value != eq.value;
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
  inline CheckedPointer<POLY> cast() throw(CastException) {
    POLY* temp = dynamic_cast<POLY*>(value);
    assert(temp, CastException(this));
    return temp;
  }
  
  /**
    Returns mutable object.
  */
  inline TYPE& operator*() throw(NullPointer) {
    assert(value, NullPointer(this));
    return *value;
  }
  
  /**
    Returns constant object.
  */
  inline const TYPE& operator*() const throw(NullPointer) {
    assert(value, NullPointer(this));
    return *value;
  }
  
  /**
    Returns object for modifying access.
  */
  inline TYPE* operator->() throw(NullPointer) {
    assert(value, NullPointer(this));
    return value;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const TYPE* operator->() const throw(NullPointer) {
    assert(value, NullPointer(this));
    return value;
  }
  
  /**
    Returns true if the pointer is valid (i.e. not 0).
  */
  inline operator bool() const throw() {
    return value;
  }
};

template<class TYPE>
class Relocateable<CheckedPointer<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<void*>::IS_RELOCATEABLE;
};

template<class TYPE>
class Hash<CheckedPointer<TYPE> > {
public:

  inline unsigned long operator()(const CheckedPointer<TYPE>& value) throw() {
    Hash<void*> hash;
    return hash(value.getValue());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
