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
  Owner automation pointer. Not MT-safe.

  @short Owner pointer.
  @version 1.0
*/

template<class TYPE>
class Owner : public AutomationObject {
private:

  mutable TYPE* object = nullptr;

  inline TYPE* detach() const noexcept
  {
    TYPE* object = this->object;
    this->object = nullptr;
    return object;
  }
public:

  inline Owner() noexcept
  {
  }

  inline Owner(const Owner& value) noexcept : object(value.detach())
  {
  }

  inline Owner(TYPE* _object) noexcept : object(_object)
  {
  }

  inline Owner& operator=(const Owner& value) noexcept
  {
    if (&value != this) {
      object = value.detach();
    }
    return *this;
  }

  inline Owner& operator=(TYPE* object) noexcept
  {
    this->object = object;
    return *this;
  }

  /**
    Detaches the object.
  */
  inline TYPE* detach() noexcept
  {
    TYPE* object = this->object;
    this->object = nullptr;
    return object;
  }

  /**
    Returns true if the object is valid.
  */
  inline bool isValid() const noexcept
  {
    return object != nullptr;
  }

  inline TYPE& operator*()
  {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }

  inline const TYPE& operator*() const
  {
    if (!object) {
      throw NullPointer(this);
    }
    return *object;
  }

  inline TYPE* operator->()
  {
    if (!object) {
      throw NullPointer(this);
    }
    return object;
  }

  inline const TYPE* operator->() const
  {
    if (!object) {
      throw NullPointer(this);
    }
    return object;
  }

  /**
    Destroys the object.
  */
  void destroy()
  {
    if (object) {
      TYPE* oldObject = object;
      object = nullptr;
      delete oldObject;
    }
  }

  /**
    Destroys the object.
  */
  inline ~Owner()
  {
    destroy();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
