/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__MEMORY_DUMP_H
#define _DK_SDU_MIP__BASE_MEM__MEMORY_DUMP_H

#include <base/string/FormatOutputStream.h>
#include <base/OutOfDomain.h>
#include <base/Cast.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class describes a block of memory to be dumped to a format output stream.
  
  @short Memory dump descriptor.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class MemoryDump : public Object {
private:

  /** The address of the memory block. */
  const uint8* memory;
  /** Specifies the size of the memory block. */
  MemorySize size;
  /** The options. */
  unsigned int options;
  /** Specifies the size of a word. */
  unsigned int wordSize;
  /** Specifies the global offset. */
  unsigned long long offset;
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
//     assert(
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
  void setGlobalOffset(unsigned long long offset) throw(OutOfDomain) {
    assert(offset % 16 == 0, OutOfDomain(this));
    this->offset = offset;
  }

  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const MemoryDump& value) throw(IOException);
};

inline MemoryDump::MemoryDump(const uint8* _memory, unsigned int _size, unsigned int _options) throw()
  : memory(_memory), size(_size), options(_options), wordSize(1) {
}

/**
  Writes the memory to the specified format output stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const MemoryDump& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
