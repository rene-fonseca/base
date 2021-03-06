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

#include <base/Object.h>
#include <base/mem/DynamicMemory.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The base class of all dynamically allocated classes.

  @short Dynamic object.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DynamicObject {
private:

  enum {
    STATE_VALID = 0x8f7d6b59, // less likely to happen
    STATE_INVALID = 0x91a3b5c7 // less likely to happen
  };
  
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  uint32 valid = STATE_VALID;
#endif
public:

  /**
    Initializes the object.
  */
  inline DynamicObject() noexcept
  {
  }

  /**
    Returns true if the object is valid.
  */
  bool isValidObject() const noexcept;

  /**
    Destroys the object.
  */
  virtual inline ~DynamicObject() noexcept(false)
  {
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
    BASSERT(valid == STATE_VALID);
    valid = STATE_INVALID;
#endif
  }
  
  _COM_AZURE_DEV__BASE__OVERRIDE_ALLOC();
};

template<>
class IsUninitializeable<DynamicObject> : public IsUninitializeable<uint32> {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
