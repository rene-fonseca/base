/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/MemoryDump.h>
#include <base/string/ASCIITraits.h>
#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const MemoryDump& value) throw(IOException) {
  const unsigned int bytesPerRow = 16;
  const unsigned int rowsPerHeader = 16;
  const unsigned int wordSize = value.wordSize;
  const unsigned int options = value.options;
  
  const uint8* src = value.memory;
  uint64 offsetMask = PrimitiveTraits<uint64>::MAXIMUM;
  for (unsigned int i = 64/4; i > value.offsetDigits; --i) {
    offsetMask >>= 4;
  }
  String underlineOffset = MESSAGE("----------------");
  underlineOffset.forceToLength(value.offsetDigits);
    
  unsigned int rows = (value.size + bytesPerRow - 1)/bytesPerRow;
  StringOutputStream s;
  
  for (unsigned int row = 0; row < rows; ++row) {
    if (row % rowsPerHeader == 0) {
      if ((options & MemoryDump::HEADER) &&
          ((row == 0) || (options & MemoryDump::REPEAT_HEADER))) {
        if (row > 0) {
          stream << EOL;
        }
        
        if (options & MemoryDump::OFFSET) {
          stream << " offset   ";
        } else if (options & MemoryDump::ADDRESS) {
          stream << "  addr    ";
        }
        
        switch (wordSize) {
        case 2:
          stream << "   0    2    4    6    8    a    c    e";
          break;
        case 4:
          stream << "       0        4        8        c";
          break;
        case 8:
          stream << "               0                8";
          break;
        default:
          stream << " 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f";
          break;
        }

        if (options & MemoryDump::CHARS) {
          stream << "  0123456789abcdef";
        }
        stream << EOL;
        
        if ((options & MemoryDump::OFFSET) ||
            (options & MemoryDump::ADDRESS)) {
          stream << underlineOffset << "  ";
        }
        
        switch (wordSize) {
        case 2:
          stream << "---- ---- ---- ---- ---- ---- ---- ----";
          break;
        case 4:
          stream << "-------- -------- -------- --------";
          break;
        case 8:
          stream << "---------------- ----------------";
          break;
        default:
          stream << "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --";
          break;
        }
        
        if (options & MemoryDump::CHARS) {
          stream << "  ----------------";
        }
        
        stream << EOL;
      }
    }
    
    if (options & MemoryDump::OFFSET) {
      stream << HEX << setWidth(value.offsetDigits) << NOPREFIX << ZEROPAD
             << (((src - value.memory) + value.offset) & offsetMask) << "  ";
    } else if (options & MemoryDump::ADDRESS) {
      stream << HEX << setWidth(value.offsetDigits) << NOPREFIX << ZEROPAD
             << src << "  ";
    }

    bool lastRow = ((row + 1) == rows);
    unsigned int bytes = lastRow ? (value.size % bytesPerRow) : bytesPerRow;
    if (bytes == 0) {
      bytes = bytesPerRow;
    }
    unsigned int spaces = 0;
    
    switch (wordSize) {
    case 2:
      for (unsigned int i = 0; i < bytes/sizeof(uint16); ++i) {
        if (i > 0) {
          stream << ' ';
        }
        stream << HEX << setWidth(4) << NOPREFIX << ZEROPAD << Cast::pointer<uint16*>(src)[i];
      }
      spaces = (bytesPerRow/sizeof(uint16) - bytes/sizeof(uint16)) * 5;
      break;
    case 4:
      for (unsigned int i = 0; i < bytes/sizeof(uint32); ++i) {
        if (i > 0) {
          stream << ' ';
        }
        stream << HEX << setWidth(8) << NOPREFIX << ZEROPAD << Cast::pointer<uint32*>(src)[i];
      }
      spaces = (bytesPerRow/sizeof(uint32) - bytes/sizeof(uint32)) * 9;
      break;
    case 8:
      for (unsigned int i = 0; i < bytes/sizeof(uint64); ++i) {
        if (i > 0) {
          stream << ' ';
        }
        stream << HEX << setWidth(16) << NOPREFIX << ZEROPAD << Cast::pointer<uint64*>(src)[i];
      }
      spaces = (bytesPerRow/sizeof(uint64) - bytes/sizeof(uint64)) * 17;
      break;
    default:
      for (unsigned int i = 0; i < bytes/sizeof(uint8); ++i) {
        if (i > 0) {
          stream << ' ';
        }
        stream << HEX << setWidth(2) << NOPREFIX << ZEROPAD << src[i];
      }
      spaces = (bytesPerRow - bytes) * 3;
    }

    while (spaces--) { // for the last row
      stream << ' ';
    }
    
    if (options & MemoryDump::CHARS) {
      stream << "  ";
      
      const uint8* chars = src;
      for (unsigned int i = 0; i < bytes/sizeof(uint8); ++i) {
        char value = *chars++;
        stream << (ASCIITraits::isPrintable(value) ? value : '.');
      }
    }
    
    stream << EOL;
    
    src += bytes;
  }
  
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
