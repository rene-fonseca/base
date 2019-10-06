/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/AutomationObject.h>
#include <base/mem/NullPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Scope automation pointer.

  @short Scope pointer.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Scope : public AutomationObject {
private:

  TYPE* object = nullptr;
public:

  inline Scope() throw() {
  }

  inline Scope(TYPE* _object) throw() : object(_object) {
  }

  inline Scope& operator=(TYPE* object) throw() {
    this->object = object;
    return *this;
  }

  /**
    Returns true if the object is valid.
  */
  inline bool isValid() const throw() {
    return object;
  }

  inline TYPE& operator*() throw(NullPointer) {
    bassert(object, NullPointer(this));
    return *object;
  }

  inline const TYPE& operator*() const throw(NullPointer) {
    bassert(object, NullPointer(this));
    return *object;
  }

  inline TYPE* operator->() throw(NullPointer) {
    bassert(object, NullPointer(this));
    return object;
  }

  inline const TYPE* operator->() const throw(NullPointer) {
    bassert(object, NullPointer(this));
    return object;
  }

  inline void destroy() throw(...) {
    if (object) {
      delete object;
      object = nullptr;
    }
  }

  inline ~Scope() throw(...) {
    destroy();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
