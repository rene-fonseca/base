/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__VALIDIFIED_RESULT_H
#define _DK_SDU_MIP__BASE__VALIDIFIED_RESULT_H

#include <base/AutomationObject.h>
#include <base/InvalidException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation object used to specify the validity of a result if the type of the
  result is not able to represent an invalid condition.
  
  @short Validified result.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class ValidifiedResult : public AutomationObject {
private:

  /** The value. */
  TYPE value;
  /** The validity. */
  bool valid;
public:

  /**
    Initializes object as invalid.
  */
  inline ValidifiedResult() throw() : valid(false) {
  }
  
  /**
    Initializes object as valid.
  */
  inline ValidifiedResult(const TYPE& _value, bool _valid = true) throw()
    : value(_value), valid(_valid) {
  }

  /**
    Initializes object by other object.
  */
  inline ValidifiedResult(const ValidifiedResult& copy) throw()
    : value(copy.value), valid(copy.valid) {
  }
  
  /**
    Returns the value. Raises InvalidException if result is invalid.
  */
  inline TYPE getValue() const throw(InvalidException) {
    assert(valid, InvalidException(this));
    return value;
  }
  
  /**
    Returns true if the result is valid.
  */
  inline bool isValid() const throw() {
    return valid;
  }

  /**
    Returns true if the result is invalid.
  */
  inline bool isInvalid() const throw() {
    return !valid;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif