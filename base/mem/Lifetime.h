/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__LIFETIME_H
#define _DK_SDU_MIP__BASE_MEM__LIFETIME_H

#include <base/mem/NullPointer.h>

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
class Lifetime {
public:
  
  /** Object type. */
  typedef TYPE Value;
  /** Pointer to objcet type. */
  typedef TYPE* Pointer;
  /** Reference to object type. */
  typedef TYPE& Reference;
private:

  /** Pointer to object. */
  Pointer object;
  
  Lifetime(const Lifetime& copy) throw();
  
  Lifetime& operator=(const Lifetime& eq) throw();
public:
  
  /**
    Initializes the automation pointer.
    
    @param object The object pointer to be automated.
  */
  inline Lifetime(Pointer _object) throw(NullPointer) : object(_object) {
    assert(object, NullPointer(this));
  }
  
  /**
    Returns mutable object.
  */
  inline Reference operator*() throw() {
    return *object;
  }
  
  /**
    Returns constant object.
  */
  inline const Reference operator*() const throw() {
    return *object;
  }
  
  /**
    Returns object for modifying access.
  */
  inline Pointer operator->() throw() {
    return object;
  }
  
  /**
    Returns object for non-modifying access.
  */
  inline const Pointer operator->() const throw() {
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

#endif
