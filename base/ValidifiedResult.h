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

#include <base/AutomationObject.h>
#include <base/InvalidException.h>
#include <base/Base.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Automation object used to specify the validity of a result if the type of the
  result is not able to represent an invalid condition.
  
  @short Validified result.
  @version 1.0
*/

template<class TYPE>
class ValidifiedResult : public AutomationObject {
private:

  /** The value. */
  TYPE value;
  /** The validity. */
  bool valid = false;
public:

  /**
    Initializes object as invalid.
  */
  inline ValidifiedResult()
  {
  }
  
  /**
    Initializes object as valid.
  */
  inline ValidifiedResult(const TYPE& _value, bool _valid = true)
    : value(_value), valid(_valid)
  {
  }

  /**
    Initializes object by other object.
  */
  inline ValidifiedResult(const ValidifiedResult& copy)
    : value(copy.value), valid(copy.valid)
  {
  }

  /**
    Initializes object by other object.
  */
  inline ValidifiedResult(ValidifiedResult&& move)
    : value(std::move(move.value)), valid(move.valid)
  {
  }
  
  /**
    Initializes object by other object.
  */
  inline ValidifiedResult& operator=(const ValidifiedResult& assign)
  {
    if (&assign != this) {
      value = assign.value;
      valid = assign.valid;
    }
    return *this;
  }
  
  /**
    Returns the value. Raises InvalidException if result is invalid.
  */
  inline const TYPE& getValue() const
  {
    if (!valid) {
      throw InvalidException(this);
    }
    return value;
  }
  
  /**
    Returns the value if valid and otherwise the given defaultValue.
  */
  inline const TYPE& getValue(const TYPE& defaultValue) const noexcept
  {
    return valid ? value : defaultValue;
  }
    
  /**
    Returns true if the result is valid.
  */
  inline bool isValid() const noexcept
  {
    return valid;
  }

  /**
    Returns true if the result is invalid.
  */
  inline bool isInvalid() const noexcept
  {
    return !valid;
  }

  inline FormatOutputStream& operator<<(FormatOutputStream& stream) const throw(IOException)
  {
    if (valid) {
      return stream << static_cast<const TYPE&>(value);
    } else {
      return stream << "BAD";
    }
  }
};

/**
  Writes the specified ValidifiedResult to the format output stream.
*/
template<class TYPE>
inline FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const ValidifiedResult<TYPE>& value) throw(IOException)
{
  return value.operator<<(stream);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
