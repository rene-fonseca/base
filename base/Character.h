/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__CHARACTER_H
#define _DK_SDU_MIP__BASE__CHARACTER_H

#include "Object.h"

/**
  Character.

  @author René Møller Fonseca
  @version 1.0
*/

class Character : public Object, public Copyable {
protected:

  /** The value. */
  char val;
public:

  /**
    Initializes the character as '\0'.
  */
  inline Character() throw() : val('\0') {};

  /**
    Initializes the character. Implicit initialization allowed.

    @param value The desired value.
  */
  inline Character(char value) throw() : val(value) {};

  /**
    Copy constructor. Initializes a new Character from other Character object.
  */
  Character(const Character& copy) throw(); // use default copy object policy

  /**
    Assignment operator.
  */
  Character& operator=(int value) const throw(); // use default assignment object policy

  /**
    Gets the value of the integer.
  */
  inline char getValue() const throw() {return val;};

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(char value) throw() {val = value;};

  /**
    Casts integer to native type.
  */
  inline operator char() const throw() {return val;};
};

#endif
