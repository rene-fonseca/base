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

#include <base/collection/Hash.h>
#include <base/mem/NullPointer.h>
#include <base/CastException.h>
#include <base/Functor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Simple automation pointer which guarantees proper initialization.
  
  @short Simple pointer.
  @ingroup memory
  @version 1.0
*/

template<class TYPE>
class Pointer {
private:
  
  /** Pointer to object. */
  TYPE* value = nullptr;
public:
  
  /**
    Initializes the automation pointer as invalid (i.e. nullptr).
  */
  inline Pointer() noexcept
  {
  }
  
  /**
    Initializes the automation pointer.
    
    @param value The object pointer to be automated.
  */
  inline Pointer(TYPE* _value) noexcept
    : value(_value)
  {
  }
  
  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline Pointer(const Pointer& copy) noexcept : value(copy.value)
  {
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline Pointer(const Pointer<POLY>& copy) noexcept
    : value(copy.getValue()) {
  }
  
  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline Pointer& operator=(const Pointer& eq) noexcept
  {
    value = eq.value;
    return *this;
  }
  
  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline Pointer& operator=(const Pointer<POLY>& eq) noexcept
  {
    value = eq.getValue();
    return *this;
  }
  
  /**
    Returns the pointer value for modifying access.
  */
  inline TYPE* const getValue() noexcept
  {
    return value;
  }
  
  /**
    Returns the pointer value for non-modifying access.
  */
  inline const TYPE* const getValue() const noexcept
  {
    return value;
  }
  
  /**
    Invalidates the pointer.
  */
  inline void invalidate() noexcept
  {
    value = nullptr;
  }
  
  /**
    Returns true if the pointer is valid (not nullptr).
  */
  inline bool isValid() const noexcept
  {
    return value;
  }
  
  /**
    Returns true if the object may be cast to the specified type.
  */
  template<class POLY>
  inline bool isType() const noexcept
  {
    return dynamic_cast<const POLY*>(value);
  }
  
  /**
    Dynamic cast to the specified type.
  */
  template<class POLY>
  inline Pointer<POLY> cast() const throw(CastException)
  {
    const POLY* result = dynamic_cast<const POLY*>(value);
    bassert(result, CastException(this));
    return result;
  }
  
  /**
    Returns true if the pointers are equal.
  */
  inline bool operator==(const Pointer& eq) const noexcept
  {
    return value == eq.value;
  }
  
  /**
    Returns true if the pointers are non-equal.
  */
  inline bool operator!=(const Pointer& eq) const noexcept
  {
    return value != eq.value;
  }
  
  /**
    Dereference the automation pointer for modifying access.
  */
  inline TYPE* const operator->() noexcept
  {
    return value;
  }
  
  /**
    Dereferences the automation pointer for non-modifying access.
  */
  inline const TYPE* const operator->() const noexcept
  {
    return value;
  }
  
  /**
    Returns true if the pointer is valid (not nullptr).
  */
  inline operator bool() const noexcept
  {
    return value;
  }
};

template<class TYPE>
class Relocateable<Pointer<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<void*>::IS_RELOCATEABLE;
};

template<class TYPE>
class Hash<Pointer<TYPE> > {
public:

  inline unsigned long operator()(const Pointer<TYPE>& value) noexcept {
    Hash<TYPE*> hash;
    return hash(value.getValue());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
