/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__INTEGER_H
#define _DK_SDU_MIP__BASE__INTEGER_H

#include <base/Object.h>
#include <base/Type.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Signed integer (32 bits or more).

  @short Signed integer.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.01
*/

class Integer : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the minimum value. */
  static const int MINIMUM = Int::MINIMUM;
  /** Specifies the maximum value. */
  static const int MAXIMUM = Int::MAXIMUM;
protected:

  /** The value. */
  int value;
public:

  /**
    Returns the value of the integer string representation.

    @param str The string representation.
  */
  static int parse(const String& str) throw(InvalidFormat);

  /**
    Initializes the integer as zero.
  */
  inline Integer() throw() : value(0) {}

  /**
    Initializes the integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  Integer(int value) throw();

  /**
    Initializes integer from string.

    @param str The string.
  */
  inline Integer(const String& str) throw(InvalidFormat) : value(parse(str)) {}

  /**
    Copy constructor. Initializes a new Integer from other Integer object.
  */
  inline Integer(const Integer& copy) throw() : value(copy.value) {}

  /**
    Assignment of integer to this integer.
  */
  inline Integer& operator=(const Integer& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of native type to this integer.
  */
  inline Integer& operator=(int value) throw() {
    this->value = value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline int getValue() const throw() {return value;}

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(int value) throw() {this->value = value;}

  /**
    Casts integer to native type.
  */
  inline operator int() const throw() {return value;}
};

inline Integer::Integer(int v) throw() : value(v) {}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
