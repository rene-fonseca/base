/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__CRC32_H
#define _DK_SDU_MIP__BASE_SECURITY__CRC32_H

#include <base/Object.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of the 32 bit Cyclic Redundancy Check (CRC-32) algorithm as
  described by ISO 3309 (also see RFC 1952).
  
  @ingroup security
  @short CRC-32.
  @see MD5Sum SHA1
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class CRC32 : public Object {
private:

  /** The current checksum. */
  uint32 checksum;
public:

  /** Lookup table of CRCs of all 8-bit messages. */
  static const uint32 LOOKUP[256];

  /**
    Initializes object with an initial checksum of 0.
  */
  CRC32() throw();

  /**
    Updates the checksum corresponding to the specified data. May be called
    multiple times.

    @param buffer The buffer holding the data.
    @param size The number of octets in the buffer.
  */
  void push(const char* buffer, unsigned int size) throw();

  /**
    This function should be invoked when the entire message has been pushed.
    Do NOT use push() after invoking this function. This method only provided
    for compatibility with MD5SUM and SHA1 classes.
  */
  void pushEnd() throw();

  /**
    Returns the checksum.
  */
  inline uint32 getValue() const throw() {
    return checksum;
  }

  /**
    Sets the checksum.
  */
  inline void setValue(uint32 value) throw() {
    checksum = value;
  }
  
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
