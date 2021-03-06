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

#include <base/string/String.h>
#include <base/OutOfRange.h>
#include <base/io/PushInterface.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Implementation of the SHA-1 message-digest algorithm (see FIPS PUB 180-2).
  This class takes a message of arbitrary length and produces a 128-bit
  "fingerprint"/"message digest" of the message. According to the RFC it is
  computationally infeasible to produce two messages having the same message
  digest, or to produce any message having a given prespecified target message
  digest.

  The SHA-1 is designed to have the following properties: it is computationally
  infeasible to find a message which corresponds to a given message digest, or
  to find two different messages which produce the same message digest.

  @code
  String message = "abcdefghijklmnopqrstuvwxyz";
  SHA1 checksum;
  checksum.push(message.getBytes(), message.getLength());
  checksum.pushEnd();
  fout << "message digest: " << checksum.getValue() << ENDL;
  @endcode

  @short SHA-1 message-digest.
  @ingroup security
  @see MD5Sum SHA256 SHA384 SHA512
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SHA1 : public Object, public PushInterface {
public:

  /** Internal block size in bytes. */
  static const unsigned int BLOCK_SIZE = 64;
  /** Specifies the maximum length (in bytes) of the original message. */
  static const uint64 MAXIMUM_SIZE = (1ULL << (64 - 3)) - 1;
  /** The message schedule size. */
  static const unsigned int MESSAGE_SCHEDULE = 80;
private:
  
  /** Message digest buffer. */
  uint32 messageDigest[5] = {0, 0, 0, 0, 0};
  /** The total number of bytes pushed. */
  uint64 totalSize = 0;
  /** Temporary container for incomplete 16 word block. */
  uint8 buffer[BLOCK_SIZE];
  /** The number of bytes in the buffer. */
  unsigned int bytesInBuffer = 0;
  
  /** Push one block (16 words). */
  void pushBlock(const uint8* block) noexcept;
  
  static inline uint32 rotate(uint32 value, unsigned int bits) noexcept
  {
    return (value << bits) | (value >> (32 - bits));
  }
  
  static inline void translate1(uint32 a, uint32& b, uint32 c, uint32 d, uint32& e, uint32 word) noexcept
  {
    e += rotate(a, 5) + ((b & c) | (~b & d)) + word + 0x5a827999;
    b = rotate(b, 30);
  }

  static inline void translate2(uint32 a, uint32& b, uint32 c, uint32 d, uint32& e, uint32 word) noexcept
  {
    e += rotate(a, 5) + (b ^ c ^ d) + word + 0x6ed9eba1;
    b = rotate(b, 30);
  }

  static inline void translate3(uint32 a, uint32& b, uint32 c, uint32 d, uint32& e, uint32 word) noexcept
  {
    e += rotate(a, 5) + ((b & c) | (b & d) | (c & d)) + word + 0x8f1bbcdc;
    b = rotate(b, 30);
  }

  static inline void translate4(uint32 a, uint32& b, uint32 c, uint32 d, uint32& e, uint32 word) noexcept
  {
    e += rotate(a, 5) + (b ^ c ^ d) + word + 0xca62c1d6;
    b = rotate(b, 30);
  }
public:

  /**
    Initializes object.
  */
  SHA1() noexcept;

  /**
    Updates the checksum corresponding to the specified data. May be called
    multiple times. Raises OutOfRange if the total number of octets exceed the
    limit specified by MAXIMUM_SIZE.

    @param buffer The buffer holding the data.
    @param size The number of octets in the buffer.
  */
  MemorySize push(const uint8* buffer, MemorySize size);
  
  /**
    This function should be invoked when the entire message has been pushed.
    Do NOT use push() after invoking this function.
  */
  void pushEnd() noexcept;

  /**
    Returns the total size of the original message.
  */
  inline uint64 getTotalSize() const noexcept {
    return totalSize;
  }

  /**
    Returns the message digest encoded in hex. This is only valid after
    pushEnd() has been invoked.
  */
  String getValue() const noexcept;

  /**
    Returns the message digest encoded in Base64. This is only valid after
    pushEnd() has been invoked.
  */
  String getBase64() const noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
