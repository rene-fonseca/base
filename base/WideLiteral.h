/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__WIDE_LITERAL_H
#define _DK_SDU_MIP__BASE__WIDE_LITERAL_H

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short A wide literal string.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class WideLiteral {
private:

  /** The literal. */
  const wchar* literal;
  /** The length of the literal. */
  const MemorySize length;
public:

  /**
    Initializes the literal.
  */
  inline WideLiteral(const wchar* _literal, MemorySize _length) throw()
    : literal(_literal), length(_length) {
  }

  /**
    Returns the literal.
  */
  inline operator const wchar*() const throw() {
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
  Returns a WideLiteral object for the specified literal.
  
  @relates WideLiteral
*/
template<MemorySize SIZE>
inline WideLiteral literal(const wchar (&value)[SIZE]) throw() {
  return WideLiteral(value, SIZE - 1);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
