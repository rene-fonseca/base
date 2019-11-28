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

#include <base/Functor.h>
// #include <base/mem/DynamicMemory.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The base class of all non-abstract classes.
  
  @short Object.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Object {
private:

#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  unsigned int objectId = 0;
#endif
public:

  /**
    Initializes object.
  */
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  inline Object() noexcept
    : objectId(Debug::allocateUniqueId())
  {
  }
#elif 0
  inline Object() noexcept
  {
  }
#endif

#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  inline Object(const Object& copy) noexcept
    : objectId(Debug::allocateUniqueId())
  {
  }
#elif 0
  inline Object(const Object& copy) noexcept
  {
  }
#endif

#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  Object& operator=(const Object& assign) noexcept
  {
    return *this;
  }
#elif 0
  Object& operator=(const Object& assign) noexcept
  {
    return *this;
  }
#endif

  // causes problems for multiple inheritance _COM_AZURE_DEV__BASE__OVERRIDE_ALLOC();
};

template<>
class IsUninitializeable<Object> : public IsUninitializeable<unsigned int> {
}; 

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
