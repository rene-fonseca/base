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
#include <base/Primitives.h>
#include <base/io/PushInterface.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Implementation of the 32 bit Cyclic Redundancy Check (CRC-32) algorithm as
  described by ISO 3309 (also see RFC 1952).

  Should only be used for compatibility with legacy systems.
  
  @short CRC-32.
  @ingroup security
  @see Adler32 MD5Sum SHA1
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API CRC32 : public Object, public PushInterface {
private:

  /** The current checksum. */
  uint32 checksum = 0;
  /** The total number of bytes pushed. */
  uint64 totalSize = 0;
public:

  /** Lookup table of CRCs of all 8-bit messages. */
  static const uint32 LOOKUP[256];

  /**
    Initializes object with an initial checksum of 0.
  */
  CRC32() noexcept;

  /**
    Updates the checksum corresponding to the specified data. May be called
    multiple times.

    @param buffer The buffer holding the data.
    @param size The number of octets in the buffer.
  */
  MemorySize push(const uint8* buffer, MemorySize size) noexcept;
  
  /**
    Does nothing.
  */
  inline void pushEnd() noexcept
  {
  }

  /**
    Returns the total size of the original message.
  */
  inline uint64 getTotalSize() const noexcept
  {
    return totalSize;
  }
  
  /**
    Returns the checksum.
  */
  inline uint32 getValue() const noexcept
  {
    return checksum;
  }

  /**
    Sets the checksum.
  */
  inline void setValue(uint32 value) noexcept
  {
    checksum = value;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
