/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__STRICT_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__STRICT_POINTER_H

#include <base/mem/NullPointer.h>
#include <base/NotCopyable.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer which cannot be copied and which is protected from direct
  access. The automation pointer may only be used with valid pointers (i.e. not
  0).
  
  @short Strict pointer.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class StrictPointer : public NotCopyable {
private:

  /** Pointer to object. */
  TYPE* value;
public:
  
  /**
    Initializes the automation pointer.
    
    @param value The object pointer to be automated.
  */
  inline StrictPointer(TYPE* _value) throw(NullPointer) : value(_value) {
    assert(value, NullPointer(this));
  }
  
  /**
    Returns true if the pointers are equal.
  */
  inline bool operator==(const StrictPointer& eq) const throw() {
    return value == eq.value;
  }
  
  /**
    Returns true if the pointers are non-equal.
  */
  inline bool operator!=(const StrictPointer& eq) const throw() {
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
