/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OWNER_H
#define _DK_SDU_MIP__BASE__OWNER_H

#include <base/AutomationObject.h>
#include <base/mem/NullPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Owner automation pointer. Not MT-safe.

  @short Owner pointer.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Owner : public AutomationObject {
private:

  mutable TYPE* object;

  inline TYPE* detach() const throw() {
    TYPE* object = this->object;
    this->object = 0;
    return object;
  }
public:

  inline Owner() throw() : object(0) {
  }

  inline Owner(const Owner& value) throw() : object(value.detach()) {
  }

  inline Owner(TYPE* _object) throw() : object(_object) {
  }

  inline Owner& operator=(const Owner& value) throw() {
    if (&value != this) {
      object = value.detach();
    }
    return *this;
  }

  inline Owner& operator=(TYPE* object) throw() {
    this->object = object;
    return *this;
  }

  /**
    Detaches the object.
  */
  inline TYPE* detach() throw() {
    TYPE* object = this->object;
    this->object = 0;
    return object;
  }

  /**
    Returns true if the object is valid.
  */
  inline bool isValid() const throw() {
    return object != 0;
  }

  inline TYPE& operator*() throw(NullPointer) {
    assert(object, NullPointer(this));
    return *object;
  }

  inline const TYPE& operator*() const throw(NullPointer) {
    assert(object, NullPointer(this));
    return *object;
  }

  inline TYPE* operator->() throw(NullPointer) {
    assert(object, NullPointer(this));
    return object;
  }

  inline const TYPE* operator->() const throw(NullPointer) {
    assert(object, NullPointer(this));
    return object;
  }

  /**
    Destroys the object.
  */
  inline void destroy() /*throw(...)*/ {
    if (object) {
      delete object;
      object = 0;
    }
  }

  /**
    Destroys the object.
  */
  inline ~Owner() /*throw(...)*/ {
    destroy();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
