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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Byte helpers.

  @ingroup security
*/

class _COM_AZURE_DEV__BASE__API Bytes {
public:

  /** Converts words to byte sequence. */
  template<typename TYPE>
  static inline void convertWordsToBytes(uint8* dest, const TYPE* words, MemorySize size)
  {
    const TYPE* end = words + size;
    while (words != end) {
      const TYPE word = *words++;
      for (MemorySize i = 0; i < 8 * sizeof(TYPE); i += 8) {
        *dest++ = static_cast<uint8>((word >> i) & 0xff);
      }
    }
  }

  /** Converts words to byte sequence. */
  template<typename TYPE, MemorySize SIZE>
  static inline void convertWordsToBytes(uint8* dest, const TYPE (&words)[SIZE])
  {
    convertWordsToBytes(dest, words, SIZE);
  }

  /** Converts words in big endian to byte sequence. */
  template<typename TYPE>
  static inline void convertWordsToBytesBigEndian(uint8* dest, const TYPE* words, MemorySize size)
  {
    const TYPE* end = words + size;
    while (words != end) {
      const TYPE word = *words++;
      for (MemorySize i = 8 * sizeof(TYPE); i > 0; ) {
        i -= 8;
        *dest++ = static_cast<uint8>((word >> i) & 0xff);
      }
    }
  }

  /** Converts words in big endian to byte sequence. */
  template<typename TYPE, MemorySize SIZE>
  static inline void convertWordsToBytesBigEndian(uint8* dest, const TYPE(&words)[SIZE])
  {
    convertWordsToBytesBigEndian(dest, words, SIZE);
  }

  /** Returns bytes as hex string. */
  static String getAsHex(const uint8* src, MemorySize size);

  /** Returns bytes as Base64 string. */
  static String getAsBase64(const uint8* src, MemorySize size);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
