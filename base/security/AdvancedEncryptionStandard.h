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
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Implementation of the Advanced Encryption Standard (AES) as specified by
  FIPS-197.
  
  @short Advanced Encryption Standard (AES) implementation.
  @ingroup security
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AdvancedEncryptionStandard : public Object {
public:
  
  /** Cipher. */
  enum Cipher {
    CIPHER_128, /**< 128 bit cipher. */
    CIPHER_192, /**< 192 bit cipher. */
    CIPHER_256 /**< 256 bit cipher. */
  };
  
  class _COM_AZURE_DEV__BASE__API Cipher128Traits {
  public:
    
    /** The key length. */
    static const unsigned int Nk = 4;
    /** The block size. */
    static const unsigned int Nb = 4;
    /** The number of rounds. */
    static const unsigned int Nr = 10;
    /** Internal block size in bytes. */
    static const unsigned int BLOCK_SIZE = 4 * Nb;
  };
  
  class _COM_AZURE_DEV__BASE__API Cipher192Traits {
  public:
    
    /** The key length. */
    static const unsigned int Nk = 6;
    /** The block size. */
    static const unsigned int Nb = 4;
    /** The number of rounds. */
    static const unsigned int Nr = 12;
    /** Internal block size in bytes. */
    static const unsigned int BLOCK_SIZE = 4 * Nb;
  };

  class _COM_AZURE_DEV__BASE__API Cipher256Traits {
  public:
    
    /** The key length. */
    static const unsigned int Nk = 8;
    /** The block size. */
    static const unsigned int Nb = 4;
    /** The number of rounds. */
    static const unsigned int Nr = 14;
    /** Internal block size in bytes. */
    static const unsigned int BLOCK_SIZE = 4 * Nb;
  };
private:
  
  class _COM_AZURE_DEV__BASE__API Cipher128 : public Cipher128Traits {
  public:
    
    void operator()(
      uint8* dest, const uint8* src, const uint8* schedule) throw();
  };
  
  class _COM_AZURE_DEV__BASE__API InverseCipher128 : public Cipher128Traits {
  public:
    
    void operator()(
      uint8* dest, const uint8* src, const uint8* schedule) throw();
  };

  class _COM_AZURE_DEV__BASE__API Cipher192 : public Cipher192Traits {
  public:
    
    void operator()(
      uint8* dest, const uint8* src, const uint8* schedule) throw();
  };

  class _COM_AZURE_DEV__BASE__API InverseCipher192 : public Cipher192Traits {
  public:
    
    void operator()(
      uint8* dest, const uint8* src, const uint8* schedule) throw();
  };

  class _COM_AZURE_DEV__BASE__API Cipher256 : public Cipher256Traits {
  public:
    
    void operator()(
      uint8* dest, const uint8* src, const uint8* schedule) throw();
  };

  class _COM_AZURE_DEV__BASE__API InverseCipher256 : public Cipher256Traits {
  public:
    
    void operator()(
      uint8* dest, const uint8* src, const uint8* schedule) throw();
  };
  
  /** Cipher. */
  Cipher cipher;
  /** Specifies if the inverse cipher should be used. */
  bool inverse = false;
  /** The key schedule. */
  uint8 schedule[4 * Cipher256Traits::Nb * (Cipher256Traits::Nr + 1)]; // worst
  /** Temporary container for incomplete 16 word block. */
  uint8 buffer[Cipher256Traits::BLOCK_SIZE]; // worst case
  /** The number of bytes in the buffer. */
  unsigned int bytesInBuffer = 0;
  
  template<class CIPHER>
  inline unsigned int cipherPush(
    uint8* dest, const uint8* src, unsigned int size) throw() {
    const unsigned int BLOCK_SIZE = CIPHER::BLOCK_SIZE;
    const uint8* first = dest;
    if ((size >= BLOCK_SIZE) ||
        ((size + bytesInBuffer) >= BLOCK_SIZE)) {
      CIPHER cipher;
      if (bytesInBuffer > 0) { // do we need to empty internal buffer
        copy(buffer + bytesInBuffer, src, BLOCK_SIZE - bytesInBuffer);
        cipher(dest, buffer, schedule); // internal buffer
        dest += BLOCK_SIZE;
        src += BLOCK_SIZE - bytesInBuffer;
        size -= BLOCK_SIZE - bytesInBuffer;
        bytesInBuffer = 0;
      }
      while (size >= BLOCK_SIZE) {
        cipher(dest, src, schedule); // external buffer
        dest += BLOCK_SIZE;
        src += BLOCK_SIZE;
        size -= BLOCK_SIZE;
      }
    }
    copy(buffer + bytesInBuffer, src, size);
    bytesInBuffer += size;
    return dest - first;
  }

  template<class CIPHER>
  inline unsigned int cipherPushEnd(
    uint8* dest, unsigned int size) throw() {
    const unsigned int BLOCK_SIZE = CIPHER::BLOCK_SIZE;
    ASSERT(bytesInBuffer < BLOCK_SIZE);
    if (size <= BLOCK_SIZE) {
      return 0;
    }
    CIPHER cipher;
    if (bytesInBuffer > 0) {
      while (bytesInBuffer < BLOCK_SIZE) {
        buffer[bytesInBuffer++] = 0x00;
      }
      cipher(dest, buffer, schedule);
      bytesInBuffer = 0;
    }
    return BLOCK_SIZE;
  }
public:
  
  /**
    Returns the key length in bytes for the specified cipher.
  */
  static inline unsigned int getKeyLenght(Cipher cipher) throw() {
    static const unsigned int LENGTHS[] = {
      128/8, 192/8, 256/8
    };
    return LENGTHS[cipher];
  }
  
  /**
    Initializes encryption object.
    
    @param key The key.
    @param cipher The cipher.
    @param inverse Specifies if the inverse cipher should be used. The default
    is false.
  */
  AdvancedEncryptionStandard(
    const uint8* key, Cipher cipher, bool inverse = false) throw();
  
  /**
    Encrypts the specified data. May be called multiple times.
    
    @param dest The cipher text.
    @param src The buffer holding the data to be encrypted (plain text).
    @param size The number of octets in the buffer.
    @return The number of bytes written.
  */
  unsigned int push(uint8* dest, const uint8* src, unsigned int size) throw();
  
  /**
    This function should be invoked when the entire message has been pushed.
    Do NOT use push() after invoking this function.
    
    @param dest The cipher text.
    @param size The number of octets in the buffer.
    @return The number of bytes written.
  */
  unsigned int pushEnd(uint8* dest, unsigned int size) throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
