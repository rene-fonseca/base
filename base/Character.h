/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__CHARACTER_H
#define _DK_SDU_MIP__BASE__CHARACTER_H

#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Character.

  @author René Møller Fonseca
  @version 1.0
*/

class Character : public virtual Object {
protected:

  /** The value. */
  char value;
public:

  /**
    Initializes the character as '\0'.
  */
  inline Character() throw() : value('\0') {}

  /**
    Initializes the character. Implicit initialization with native type allowed.

    @param value The desired value.
  */
  inline Character(char value) throw() : value(value) {}

  /**
    Initializes character from other character object.
  */
  inline Character(const Character& copy) throw() : value(copy.value) {}

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

#endif
