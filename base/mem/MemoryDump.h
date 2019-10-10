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

#include <base/string/FormatOutputStream.h>
#include <base/OutOfDomain.h>
#include <base/Cast.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class describes a block of memory to be dumped to a format output
  stream.
  
  @short Memory dump descriptor.
  @ingroup memory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API MemoryDump : public Object {
private:

  /** The address of the memory block. */
  const uint8* memory = nullptr;
  /** Specifies the size of the memory block. */
  MemorySize size = 0;
  /** The options. */
  unsigned int options = 0;
  /** Specifies the size of a word. */
  unsigned int wordSize = 0;
  /** The number of significat digits in the offset. */
  unsigned int offsetDigits = 0;
  /** Specifies the global offset. */
  uint64 offset = 0;
public:
  
  enum Option {
    HEADER = 1, /**< Enables the header. */
    REPEAT_HEADER = 2, /**< Enables repeat of header every 16 rows. */
    ADDRESS = 4, /**< Enables output of address. */
    OFFSET = 8, /**< Enables output of offset. */
    CHARS = 16 /**< Enable character output. */
  };
  
  /**
    Initializes the descriptor with a word size of 1 and the global offset equal to 0.
    
    @param memory The address of the memory to dump.
    @param size The size of the memory block in bytes.
    @param options The options. Header, repeat of header, offset, and characters are enabled by default.
  */
  MemoryDump(const uint8* memory, unsigned int size, unsigned int options = HEADER|REPEAT_HEADER|OFFSET|CHARS) throw();
  
  /**
    Sets the word size (1, 2, 4, or 8).
  */
  inline void setWordSize(unsigned int value) throw(OutOfDomain) {
//     bassert(
//       (size % value == 0) && (Cast::getOffset(memory) % value == 0) &&
//       ((value == sizeof(uint8)) || (value == sizeof(uint16)) ||
//        (value == sizeof(uint32)) || (value == sizeof(uint64))),
//       OutOfDomain(this)
//     );
    wordSize = value;
  }

  /**
    Sets the global offset.
  */
  void setOffsetDigits(unsigned int digits) throw(OutOfDomain) {
    bassert((digits >= 4) && (digits <= 16), OutOfDomain(this));
    offsetDigits = digits;
  }
  
  /**
    Sets the global offset.
  */
  void setGlobalOffset(uint64 offset) throw(OutOfDomain) {
    bassert(offset % 16 == 0, OutOfDomain(this));
    this->offset = offset;
  }
  
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const MemoryDump& value) throw(IOException);
};

inline MemoryDump::MemoryDump(const uint8* _memory, unsigned int _size, unsigned int _options) throw()
  : memory(_memory), size(_size), options(_options), wordSize(1), offsetDigits(8), offset(0) {
}

/**
  Writes the memory to the specified format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const MemoryDump& value) throw(IOException);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
