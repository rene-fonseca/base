/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__CHARACTER_H
#define _DK_SDU_MIP__BASE__CHARACTER_H

#include <base/Object.h>

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
  inline Character() throw() : val('\0') {}

  /**
    Initializes the character. Implicit initialization with native type allowed.

    @param value The desired value.
  */
  inline Character(char value) throw() : val(value) {}

  /**
    Initializes character from other character object.
  */
  inline Character(const Character& copy) throw() : val(copy.val) {}

  /**
    Assignment of character to this object.
  */
  inline Character& operator=(const Character eq) throw() {val = eq.val;}

  /**
    Assignment of native type to this object.
  */
  inline Character& operator=(char eq) throw() {val = eq;}

  /**
    Gets the value of the character.
  */
  inline char getValue() const throw() {return val;}

  /**
    Sets the value of the character.

    @param value The desired value.
  */
  inline void setValue(char value) throw() {val = value;}

  /**
    Casts character to native type.
  */
  inline operator char() const throw() {return val;}
};

#endif
