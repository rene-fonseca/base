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

#include <base/Object.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short Character.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Character : public virtual Object {
protected:

  /** The value. */
  char value = '\0';
public:

  /**
    Initializes the character as '\0'.
  */
  inline Character() noexcept
  {
  }

  /**
    Initializes the character. Implicit initialization with native type allowed.

    @param value The desired value.
  */
  inline Character(char _value) noexcept
    : value(_value)
  {
  }

  /**
    Initializes character from other character object.
  */
  inline Character(const Character& copy) noexcept
    : value(copy.value)
  {
  }

  /**
    Assignment of character to this object.
  */
  inline Character& operator=(const Character& assign) noexcept
  {
    value = assign.value;
    return *this;
  }

  /**
    Assignment of native type to this object.
  */
  inline Character& operator=(char value) noexcept
  {
    this->value = value;
    return *this;
  }

  /**
    Gets the value of the character.
  */
  inline char getValue() const noexcept
  {
    return value;
  }

  /**
    Sets the value of the character.

    @param value The desired value.
  */
  inline void setValue(char value) noexcept
  {
    this->value = value;
  }

  /**
    Casts character to native type.
  */
  inline operator char() const noexcept
  {
    return value;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
