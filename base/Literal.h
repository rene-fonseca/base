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
  This class binds together a native string literal and its length.
  
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
  template<MemorySize SIZE>
  inline Literal(const char (&value)[SIZE]) throw()
    : literal(value), length(SIZE - 1) {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
  }
  
  /**
    Returns the literal.
  */
  inline const char* getValue() const throw() {
    return literal;
  }
  
  /**
    Returns the length of the literal.
  */
  inline MemorySize getLength() const throw() {
    return length;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
