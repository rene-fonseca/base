/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/string/String.h>
#include <base/OutOfRange.h>
#include <base/io/PushInterface.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of the MD5 message-digest algorithm (see RFC 1321).
  This class takes a message of arbitrary length and produces a 128-bit
  "fingerprint"/"message digest" of the message. According to the RFC it is
  computationally infeasible to produce two messages having the same message
  digest, or to produce any message having a given prespecified target message
  digest.

  @code
  String message = "abcdefghijklmnopqrstuvwxyz";
  MD5Sum checksum;
  checksum.push(Cast::pointer<const uint8*>(message.getElements()), message.getLength());
  checksum.pushEnd();
  fout << "message digest: " << checksum.getValue() << ENDL;
  @endcode

  @short MD5 message-digest.
  @ingroup security
  @see SHA1
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API MD5Sum : public Object, public PushInterface {
public:

  /** Internal block size in bytes. */
  static const unsigned int BLOCK_SIZE = 64;
  /** Specifies the maximum length (in bytes) of the original message. */
  static const uint64 MAXIMUM_SIZE = (1ULL << (64 - 3)) - 1;
private:

  /** Message digest buffer. */
  unsigned int messageDigest[4] = {0, 0, 0, 0};
  /** The total number of bytes pushed. */
  uint64 totalSize = 0;
  /** Temporary container for incomplete 16 word block. */
  uint8 buffer[BLOCK_SIZE];
  /** The number of bytes in the buffer. */
  unsigned int bytesInBuffer = 0;

  /** Push one block (16 words). */
  void pushBlock(const uint8* block) throw();
public:

  /**
    Initializes object.
  */
  MD5Sum() throw();

  /**
    Updates the checksum corresponding to the specified data. May be called
    multiple times. Raises OutOfRange if the total number of octets exceed the
    limit specified by MAXIMUM_SIZE.

    @param buffer The buffer holding the data.
    @param size The number of octets in the buffer.
  */
  unsigned int push(const uint8* buffer, unsigned int size) throw(OutOfRange);
  
  /**
    This function should be invoked when the entire message has been pushed.
    Do NOT use push() after invoking this function.
  */
  void pushEnd() throw();
  
  /**
    Returns the total size of the original message.
  */
  inline uint64 getTotalSize() const throw() {
    return totalSize;
  }
  
  /**
    Returns the message digest encoded in hex. This is only valid after
    pushEnd() has been invoked.
  */
  String getValue() const throw();

  /**
    Returns the message digest encoded in Base64. This is only valid after
    pushEnd() has been invoked.
  */
  String getBase64() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
