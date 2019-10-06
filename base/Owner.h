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
  Owner automation pointer. Not MT-safe.

  @short Owner pointer.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Owner : public AutomationObject {
private:

  mutable TYPE* object = nullptr;

  inline TYPE* detach() const throw() {
    TYPE* object = this->object;
    this->object = nullptr;
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
    this->object = nullptr;
    return object;
  }

  /**
    Returns true if the object is valid.
  */
  inline bool isValid() const throw() {
    return object != nullptr;
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

  /**
    Destroys the object.
  */
  inline void destroy() {
    if (object) {
      delete object;
      object = nullptr;
    }
  }

  /**
    Destroys the object.
  */
  inline ~Owner() {
    destroy();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
