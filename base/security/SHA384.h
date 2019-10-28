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
  Implementation of the SHA-384 message-digest algorithm (see FIPS PUB 180-2).
  This class takes a message of arbitrary length and produces a 128-bit
  "fingerprint"/"message digest" of the message. According to the RFC it is
  computationally infeasible to produce two messages having the same message
  digest, or to produce any message having a given prespecified target message
  digest.

  The SHA-384 is designed to have the following properties: it is computationally
  infeasible to find a message which corresponds to a given message digest, or
  to find two different messages which produce the same message digest.

  @code
  String message = "abcdefghijklmnopqrstuvwxyz";
  SHA384 checksum;
  checksum.push(Cast::pointer<const uint8*>(message.getElements()), message.getLength());
  checksum.pushEnd();
  fout << "message digest: " << checksum.getValue() << ENDL;
  @endcode

  @short SHA-384 message-digest.
  @ingroup security
  @see MD5Sum SHA1 SHA256 SHA512
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SHA384 : public Object, public PushInterface {
public:
  
  /** Internal block size in bytes. */
  static const unsigned int BLOCK_SIZE = 128;
  /** Specifies the maximum length (in bytes) of the original message. */
  static const uint64 MAXIMUM_SIZE = (1ULL << (64 - 3)) - 1;
  /** The message schedule size. */
  static const unsigned int MESSAGE_SCHEDULE = 80;
private:
  
  /** Message digest buffer. */
  uint64 messageDigest[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  /** The total number of bytes pushed. */
  uint64 totalSize = 0;
  /** Temporary container for incomplete 16 word block. */
  uint8 buffer[BLOCK_SIZE];
  /** The number of bytes in the buffer. */
  unsigned int bytesInBuffer = 0;

  /** Push one block (16 words). */
  void pushBlock(const uint8* block) noexcept;

  static const uint64 K[MESSAGE_SCHEDULE];
  
  static inline uint64 rotateLeft(uint64 value, unsigned int bits) noexcept {
    return (value << bits) | (value >> (64 - bits));
  }
  
  static inline uint64 rotateRight(uint64 value, unsigned int bits) noexcept {
    return (value >> bits) | (value << (64 - bits));
  }

  static inline uint64 ch(uint64 x, uint64 y, uint64 z) noexcept {
    return (x & y) ^ (~x & z);
  }
  
  static inline uint64 maj(uint64 x, uint64 y, uint64 z) noexcept {
    return (x & y) ^ (x & z) ^ (y & z);
  }
  
  static inline uint64 sum0(uint64 x) noexcept {
    return rotateRight(x, 28) ^ rotateRight(x, 34) ^ rotateRight(x, 39);
  }
  
  static inline uint64 sum1(uint64 x) noexcept {
    return rotateRight(x, 14) ^ rotateRight(x, 18) ^ rotateRight(x, 41);
  }
  
  static inline uint64 sigma0(uint64 x) noexcept {
    return rotateRight(x, 1) ^ rotateRight(x, 8) ^ (x >> 7);
  }
  
  static inline uint64 sigma1(uint64 x) noexcept {
    return rotateRight(x, 19) ^ rotateRight(x, 61) ^ (x >> 6);
  }
public:

  /**
    Initializes object.
  */
  SHA384() noexcept;

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
  void pushEnd() noexcept;

  /**
    Returns the total size of the original message.
  */
  inline uint64 getTotalSize() const noexcept {
    return totalSize;
  }

  /**
    Returns the digest of the given message.
  */
  static String getDigest(const uint8* buffer, MemorySize size)
  {
    SHA384 digest;
    digest.push(buffer, size);
    digest.pushEnd();
    return digest.getValue(); // TAG: return as bytes
  }

  /** Returns the size of the digest in bytes. */
  MemorySize getDigestSize() const noexcept;

  /** Returns n'th byte of the digest. */
  uint8 getDigest(MemorySize index) const;

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
