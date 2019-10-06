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

#include <base/mem/NullPointer.h>
#include <base/NotCopyable.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer which guarantees a valid pointer (not 0). The object is
  destroyed when the automation pointer falls out of scope.
  
  @short Lifetime automation pointer.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Lifetime : public NotCopyable {
private:

  /** Pointer to object. */
  TYPE* object = nullptr;
public:
  
  /**
    Initializes the automation pointer.
    
    @param object The object pointer to be automated.
  */
  inline Lifetime(TYPE* _object) throw(NullPointer) : object(_object) {
    bassert(object, NullPointer(this));
  }
  
  /**
    Returns mutable object.
  */
  inline TYPE& operator*() throw() {
    return *object;
  }
  
  /**
    Returns constant object.
  */
  inline const TYPE& operator*() const throw() {
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
    Destroys the automation pointer (and the object).
  */
  inline ~Lifetime() /*throw(...)*/ {
    delete object;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
