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

#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class binds together a native string literal and its length.
  
  @short A wide literal string.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API WideLiteral {
private:

  /** The literal. */
  const wchar* literal = nullptr;
  /** The length of the literal. */
  const MemorySize length = 0;
public:
  
  /**
    Initializes the literal.
  */
  template<MemorySize SIZE>
  inline WideLiteral(const wchar (&value)[SIZE]) noexcept
    : literal(value), length(SIZE - 1)
  {
  }
  
  /**
    Returns the literal.
  */
  inline const wchar* getValue() const noexcept
  {
    return literal;
  }

  /**
    Returns the length of the literal.
  */
  inline MemorySize getLength() const noexcept
  {
    return length;
  }

  /**
    Returns true if not empty.
  */
  inline operator bool() const noexcept
  {
    return length != 0;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
