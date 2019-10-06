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

#include <base/collection/Hash.h>
#include <base/mem/NullPointer.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer which protects the pointer value from direct access. The
  automation pointer may only be used with valid pointers (i.e. not 0).
  
  @short Protected pointer.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class ProtectedPointer {
private:

  /** Pointer to object. */
  TYPE* value;
public:
  
  /**
    Initializes the automation pointer.
    
    @param value The object pointer to be automated.
  */
  inline ProtectedPointer(TYPE* _value) throw(NullPointer) : value(_value) {
    bassert(value, NullPointer(this));
  }
  
  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ProtectedPointer(const ProtectedPointer& copy) throw()
    : value(copy.value) {
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ProtectedPointer(const ProtectedPointer<POLY>& copy) throw()
    : value(copy.getValue()) {
  }
  
  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline ProtectedPointer& operator=(const ProtectedPointer& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline ProtectedPointer& operator=(
    const ProtectedPointer<POLY>& eq) throw() {
    value = eq.value;
    return *this;
  }
  
  /**
    Returns true if the pointers are equal.
  */
  inline bool operator==(const ProtectedPointer& eq) const throw() {
    return value == eq.value;
  }
  
  /**
    Returns true if the pointers are non-equal.
  */
  inline bool operator!=(const ProtectedPointer& eq) const throw() {
    return value != eq.value;
  }
  
  /**
    Returns object for modifying access.
  */
  inline TYPE* operator->() throw() {
    return value;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const TYPE* operator->() const throw() {
    return value;
  }
};

template<class TYPE>
class Relocateable<ProtectedPointer<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<void*>::IS_RELOCATEABLE;
};

template<class TYPE>
class Hash<ProtectedPointer<TYPE> > {
public:

  inline unsigned long operator()(
    const ProtectedPointer<TYPE>& value) throw() {
    Hash<void*> hash;
    return hash(value.getValue());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
