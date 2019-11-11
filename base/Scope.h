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

#include <base/AutomationObject.h>
#include <base/mem/NullPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Scope automation pointer.

  @short Scope pointer.
  @version 1.0
*/

template<class TYPE>
class Scope : public AutomationObject {
private:

  TYPE* object = nullptr;
public:

  inline Scope() noexcept
  {
  }

  inline Scope(TYPE* _object) noexcept : object(_object)
  {
  }

  inline Scope& operator=(TYPE* object) noexcept
  {
    this->object = object;
    return *this;
  }

  /**
    Returns true if the object is valid.
  */
  inline bool isValid() const noexcept
  {
    return object;
  }

  inline TYPE& operator*() throw(NullPointer)
  {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }

  inline const TYPE& operator*() const throw(NullPointer)
  {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }

  inline TYPE* operator->() throw(NullPointer)
  {
    if (!object) {
      throw NullPointer(this);
    }
    return object;
  }

  inline const TYPE* operator->() const throw(NullPointer)
  {
    if (!object) {
      throw NullPointer(this);
    }
    return object;
  }

  void destroy()
  {
    if (object) {
      TYPE* oldObject = object;
      object = nullptr;
      delete oldObject;
    }
  }

  inline ~Scope()
  {
    destroy();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
