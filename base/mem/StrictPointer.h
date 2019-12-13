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

#include <base/mem/NullPointer.h>
#include <base/NotCopyable.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Automation pointer which cannot be copied and which is protected from direct
  access. The automation pointer may only be used with valid pointers (i.e. not
  nullptr).
  
  @short Strict pointer.
  @ingroup memory
  @version 1.0
*/

template<class TYPE>
class StrictPointer : public NotCopyable {
private:

  /** Pointer to object. */
  TYPE* value = nullptr;
public:
  
  /**
    Initializes the automation pointer.
    
    @param value The object pointer to be automated.
  */
  inline StrictPointer(TYPE* _value) : value(_value)
  {
    if (!value) {
      throw NullPointer(this);
    }
  }
  
  /**
    Returns true if the pointers are equal.
  */
  inline bool operator==(const StrictPointer& compare) const noexcept
  {
    return value == compare.value;
  }
  
  /**
    Returns true if the pointers are non-equal.
  */
  inline bool operator!=(const StrictPointer& compare) const noexcept
  {
    return value != compare.value;
  }
  
  /**
    Returns object for modifying access.
  */
  inline TYPE* operator->() noexcept
  {
    return value;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const TYPE* operator->() const noexcept
  {
    return value;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
