/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__INTEGER_H
#define _DK_SDU_MIP__BASE__INTEGER_H

#include <base/Object.h>
#include <base/OutOfRange.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>
#include <base/string/StringOutputStream.h>
#include <limits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Signed integer (32 bits or more).

  @author René Møller Fonseca
  @version 1.0
*/

class Integer : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the minimum value. */
  static const int MINIMUM = INT_MIN;
  /** Specifies the maximum value. */
  static const int MAXIMUM = INT_MAX;
protected:

  /** The value. */
  int value;
public:

  /**
    Initializes the integer as zero.
  */
  inline Integer() throw() : value(0) {}

  /**
    Initializes the integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  inline Integer(int value) throw() : value(value) {}

  /**
    Initializes integer from string.

    @param str The string.
  */
  Integer(String<> str) throw(InvalidFormat);

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
  inline int getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(int value) throw() {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator int() const throw() {
    return value;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
