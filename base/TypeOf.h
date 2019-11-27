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

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short TypeOf operator.
  @version 1.0
*/

template<class TYPE>
class TypeOf {
private:

  /** Default assignment is prohibited. */
  TypeOf& operator=(const TypeOf& assign) noexcept;
public:

  /** The type. */
  typedef TYPE Type;
  
  /** Initializes type object. */
  inline TypeOf() noexcept
  {
  }
  
  /** Initializes type object from other object. */
  inline TypeOf(const TypeOf& copy) noexcept
  {
  }
  
  /** Initializes type object from value. */
  inline TypeOf(TYPE value) noexcept
  {
  }
  
  /** Returns true is the types are the same. */
  inline bool operator==(const TypeOf& value) const noexcept
  {
    return true;
  }
  
  /** Returns true is the types are the different. */
  inline bool operator!=(const TypeOf& value) const noexcept
  {
    return false;
  }
  
  /** Returns true is the types are the same. */
  template<class POLY>
  inline bool operator==(const TypeOf<POLY>& value) const noexcept
  {
    return false;
  }
  
  /** Returns true is the types are the different. */
  template<class POLY>
  inline bool operator!=(const TypeOf<POLY>& value) const noexcept
  {
    return true;
  }
};

/**
  Returns the specified type.
  
  @relates TypeOf
*/
template<class TYPE>
inline TypeOf<TYPE> type() noexcept
{
  return TypeOf<TYPE>();
}

/**
  Returns the specified type.
  
  @relates TypeOf
*/
template<class TYPE>
inline TypeOf<TYPE> type(TYPE) noexcept
{
  return TypeOf<TYPE>();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
