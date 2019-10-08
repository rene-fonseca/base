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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Character.
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Character : public virtual Object {
protected:

  /** The value. */
  char value = '\0';
public:

  /**
    Initializes the character as '\0'.
  */
  inline Character() throw() {
  }

  /**
    Initializes the character. Implicit initialization with native type allowed.

    @param value The desired value.
  */
  inline Character(char _value) throw() : value(_value) {
  }

  /**
    Initializes character from other character object.
  */
  inline Character(const Character& copy) throw() : value(copy.value) {
  }

  /**
    Assignment of character to this object.
  */
  inline Character& operator=(const Character& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of native type to this object.
  */
  inline Character& operator=(char value) throw() {
    this->value = value;
    return *this;
  }

  /**
    Gets the value of the character.
  */
  inline char getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the character.

    @param value The desired value.
  */
  inline void setValue(char value) throw() {
    this->value = value;
  }

  /**
    Casts character to native type.
  */
  inline operator char() const throw() {
    return value;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
