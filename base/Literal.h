/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__LITERAL_H
#define _DK_SDU_MIP__BASE__LITERAL_H

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short A string literal.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Literal {
private:

  /** The literal. */
  const char* literal;
  /** The length of the literal. */
  const MemorySize length;
public:

  /**
    Initializes the literal.
  */
  inline Literal(const char* _literal, MemorySize _length) throw()
    : literal(_literal), length(_length) {
  }

  /**
    Returns the literal.
  */
  inline operator const char*() const throw() {
    return literal;
  }

  /**
    Returns the length of the literal.
  */
  inline MemorySize getLength() const throw() {
    return length;
  }
};

/**
  Returns a Literal object for the specified literal.
  
  @relates Literal
*/
template<MemorySize SIZE>
inline Literal literal(const char (&value)[SIZE]) throw() {
  return Literal(value, SIZE - 1);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
