/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__HASH_H
#define _DK_SDU_MIP__BASE_COLLECTION__HASH_H

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Hash operator.
  
  @short Hash
  @see HashSet HashTable
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Hash {
public:

  /**
    Returns the hash value of the specified object.
  */
  inline unsigned long operator()(const TYPE& value) throw() {
    return value;
  }
};

template<>
class Hash<void*> {
public:

  inline unsigned long operator()(void* value) throw() {
    MemorySize offset =
      static_cast<const uint8*>(value) - static_cast<const uint8*>(0);
    // TAG: least significant bits are likely to be 0
    if (sizeof(unsigned long) >= sizeof(offset)) {
      return 1500450271 * ((offset >> 8) + (offset << 8));
    } else {
      return 1500450271 * ((offset >> 8) + (offset << 8)) +
        (offset >> (sizeof(offset) * 8/2));
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
