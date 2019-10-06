/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
  Implementation of the SHA-256 message-digest algorithm (see FIPS PUB 180-2).
  This class takes a message of arbitrary length and produces a 128-bit
  "fingerprint"/"message digest" of the message. According to the RFC it is
  computationally infeasible to produce two messages having the same message
  digest, or to produce any message having a given prespecified target message
  digest.

  The SHA-256 is designed to have the following properties: it is
  computationally infeasible to find a message which corresponds to a given
  message digest, or to find two different messages which produce the same
  message digest.

  @code
  String message = "abcdefghijklmnopqrstuvwxyz";
  SHA256 checksum;
  checksum.push(Cast::pointer<const uint8*>(message.getElements()), message.getLength());
  checksum.pushEnd();
  fout << "message digest: " << checksum.getValue() << ENDL;
  @endcode

  @short SHA-256 message-digest.
  @ingroup security
  @see MD5Sum SHA1 SHA384 SHA512
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SHA256 : public Object, public PushInterface {
public:

  /** Internal block size in bytes. */
  static const unsigned int BLOCK_SIZE = 64;
  /** Specifies the maximum length (in bytes) of the original message. */
  static const uint64 MAXIMUM_SIZE = (1ULL << (64 - 3)) - 1;
  /** The message schedule size. */
  static const unsigned int MESSAGE_SCHEDULE = 64;
private:

  /** Message digest buffer. */
  uint32 messageDigest[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  /** The total number of bytes pushed. */
  uint64 totalSize = 0;
  /** Temporary container for incomplete 16 word block. */
  uint8 buffer[BLOCK_SIZE];
  /** The number of bytes in the buffer. */
  unsigned int bytesInBuffer = 0;

  /** Push one block (16 words). */
  void pushBlock(const uint8* block) throw();
  
  static const uint32 K[MESSAGE_SCHEDULE];
  
  static inline uint32 rotateLeft(uint32 value, unsigned int bits) throw() {
    return (value << bits) | (value >> (32 - bits));
  }
  
  static inline uint32 rotateRight(uint32 value, unsigned int bits) throw() {
    return (value >> bits) | (value << (32 - bits));
  }

  static inline uint32 ch(uint32 x, uint32 y, uint32 z) throw() {
    return (x & y) ^ (~x & z);
  }
  
  static inline uint32 maj(uint32 x, uint32 y, uint32 z) throw() {
    return (x & y) ^ (x & z) ^ (y & z);
  }
  
  static inline uint32 sum0(uint32 x) throw() {
    return rotateRight(x, 2) ^ rotateRight(x, 13) ^ rotateRight(x, 22);
  }
  
  static inline uint32 sum1(uint32 x) throw() {
    return rotateRight(x, 6) ^ rotateRight(x, 11) ^ rotateRight(x, 25);
  }
  
  static inline uint32 sigma0(uint32 x) throw() {
    return rotateRight(x, 7) ^ rotateRight(x, 18) ^ (x >> 3);
  }
  
  static inline uint32 sigma1(uint32 x) throw() {
    return rotateRight(x, 17) ^ rotateRight(x, 19) ^ (x >> 10);
  }
public:
  
  /**
    Initializes object.
  */
  SHA256() throw();
  
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
  uint64 getTotalSize() const throw();

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
