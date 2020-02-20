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
#include <base/string/String.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Base64 encoding (see RFC 2045 section 6.8).

  @short Base64 encoding support.
  @ingroup security
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API Base64 : public Object {
public:

  /**
    Base64 encode descriptor.
    
    @short Base64 encode descriptor.
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API Descriptor : public Object {
  private:

    /** The bytes to be encoded. */
    const char* buffer = nullptr;
    /** The size of the buffer. */
    MemorySize size = 0;
  public:

    /**
      Initializes descriptor.
      
      @param buffer The bytes to be encoded.
      @param size The size of the buffer.
    */
    Descriptor(const char* buffer, MemorySize size) noexcept;
    
    /**
      Returns the size of the buffer.
    */
    inline MemorySize getSize() const noexcept
    {
      return size;
    }
    
    /**
      Returns the bytes to be encoded.
    */
    inline const char* getBytes() const noexcept
    {
      return buffer;
    }
  };

  static constexpr unsigned int BITS_PER_CHARACTER = 6;

  /** Lookup table for mapping digits into values. */
  static const signed char VALUES[256];
  /** Digits of the Base64 encoding. */
  static const char DIGITS[64];
  /** The special character used to denote unused digits. */
  static const char PAD = '=';
  
  /**
    Returns the digit for the specified value. The value returned is undefined
    if the value is not in the range [0;63].
  */
  static inline char valueToDigit(unsigned int value) noexcept
  {
    return DIGITS[static_cast<uint8>(value & 0x3f)];
  }
  
  /**
    Returns the value [0;63] of the specified digit.

    @return -1 if not a valid Base64 digit.
  */
  static inline int valueToDigit(char digit) noexcept
  {
    return VALUES[static_cast<unsigned char>(digit)];
  }

  /**
    Returns the Base64 encoded data.

    @param buffer Buffer holding the data.
    @param size The number of octets in the buffer.
  */
  static String encode(const uint8* buffer, MemorySize size) noexcept;

  /**
    Returns the Base64 encoded data.

    @param bytes Buffer holding the data.
  */
  static inline String encode(const String& bytes) noexcept
  {
    return encode(bytes.getBytes(), bytes.getLength());
  }
};

inline Base64::Descriptor::Descriptor(const char* _buffer, MemorySize _size) noexcept
  : buffer(_buffer), size(_size)
{
}

/**
  Writes the buffer to the format output stream as described in RFC 2045.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Base64::Descriptor& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
