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
#include <base/Functor.h>
#include <base/CastException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Automation pointer which checks the pointer value before use.
  
  @short Checked automation pointer.
  @ingroup memory
  @version 1.0
*/

template<class TYPE>
class CheckedPointer {
private:

  /** Pointer to object. */
  TYPE* value = nullptr;
public:
  
  /**
    Initializes the automation pointer as nullptr.
  */
  inline CheckedPointer() noexcept
  {
  }
  
  /**
    Initializes the automation pointer.
    
    @param value The object pointer to be automated.
  */
  inline CheckedPointer(TYPE* _value) noexcept : value(_value)
  {
  }

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline CheckedPointer(const CheckedPointer& copy) noexcept
    : value(copy.value)
  {
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline CheckedPointer(const CheckedPointer<POLY>& copy) noexcept
    : value(copy.getValue())
  {
  }
  
  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline CheckedPointer& operator=(const CheckedPointer& compare) noexcept
  {
    value = compare.value;
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline CheckedPointer& operator=(const CheckedPointer<POLY>& compare) noexcept
  {
    value = compare.value;
    return *this;
  }

  /**
    Invalidates the pointer.
  */
  inline void invalidate() noexcept
  {
    value = nullptr;
  }

  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const noexcept
  {
    return value != nullptr;
  }
  
  /**
    Returns true if the pointers are equal.
  */
  inline bool operator==(const CheckedPointer& compare) const noexcept
  {
    return value == compare.value;
  }
  
  /**
    Returns true if the pointers are non-equal.
  */
  inline bool operator!=(const CheckedPointer& compare) const noexcept
  {
    return value != compare.value;
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
    Dynamic cast to the specified type. Raises CastException if the reference
    is invalid or the reference cannot be cast to the specified type.
  */
  template<class POLY>
  inline CheckedPointer<POLY> cast()
  {
    POLY* temp = dynamic_cast<POLY*>(value);
    if (!temp) {
      _throw CastException(this);
    }
    return temp;
  }
  
  /**
    Returns mutable object.
  */
  inline TYPE& operator*()
  {
    if (!value) {
      _throw NullPointer(this);
    }
    return *value;
  }
  
  /**
    Returns constant object.
  */
  inline const TYPE& operator*() const
  {
    if (!value) {
      _throw NullPointer(this);
    }
    return *value;
  }
  
  /**
    Returns object for modifying access.
  */
  inline TYPE* operator->()
  {
    if (!value) {
      _throw NullPointer(this);
    }
    return value;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const TYPE* operator->() const
  {
    if (!value) {
      _throw NullPointer(this);
    }
    return value;
  }
  
  /**
    Returns true if the pointer is valid (i.e. not nullptr).
  */
  inline operator bool() const noexcept
  {
    return value;
  }
};

template<class TYPE>
class IsRelocateable<CheckedPointer<TYPE> > : public IsRelocateable<TYPE*> {
};

template<class TYPE>
class Hash<CheckedPointer<TYPE> > {
public:

  inline unsigned long operator()(const CheckedPointer<TYPE>& value) noexcept {
    Hash<void*> hash;
    return hash(value.getValue());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
