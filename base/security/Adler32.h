/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__ADLER32_H
#define _DK_SDU_MIP__BASE_SECURITY__ADLER32_H

#include <base/Object.h>
#include <base/Primitives.h>
#include <base/io/PushInterface.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of the Adler-32 checksum algorithm as specified by RFC 1950.
  
  @short Adler-32.
  @ingroup security
  @see CRC32
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Adler32 : public Object, public PushInterface {
private:
  
  /** The current checksum. */
  uint32 checksum;
  /** The total number of bytes pushed. */
  unsigned long long totalSize;
public:
  
  /** The largest prime smaller than 65536. */
  static const unsigned int BASE = 65521;
  
  /**
    Initializes object.
  */
  Adler32() throw();
  
  /**
    Updates the checksum corresponding to the specified data. May be called
    multiple times.
    
    @param buffer The buffer holding the data.
    @param size The number of octets in the buffer.
  */
  unsigned int push(const uint8* buffer, unsigned int size) throw();  
  
  /**
    Returns the total size of the original message.
  */
  unsigned long long getTotalSize() const throw();
  
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
