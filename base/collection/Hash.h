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
  Hash operator.
  
  @short Hash function object.
  @see HashSet HashTable
  @ingroup collections
  @version 1.0
*/

template<class TYPE>
class Hash {
public:

  /**
    Returns the hash value of the specified object.
  */
  inline unsigned long operator()(const TYPE& value) noexcept
  {
    return value;
  }
};

template<>
class Hash<void*> {
public:

  inline unsigned long operator()(void* value) noexcept
  {
    MemorySize offset =
      static_cast<const uint8*>(value) - static_cast<const uint8*>(nullptr);
    // TAG: least significant bits are likely to be 0
    if (sizeof(unsigned long) >= sizeof(offset)) {
      return static_cast<unsigned long>(1500450271 * ((offset >> 8) + (offset << 8)));
    } else {
      return static_cast<unsigned long>(1500450271 * ((offset >> 8) + (offset << 8)) +
        (offset >> (sizeof(offset) * 8/2)));
    }
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
