/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/Trace.h>
#include <base/FloatingPoint.h>
#include <stdio.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptorOutputStream standardOutputStream(FileDescriptor::getStandardOutput());
FormatOutputStream fout(standardOutputStream);

FileDescriptorOutputStream standardErrorStream(FileDescriptor::getStandardError());
FormatOutputStream ferr(standardErrorStream);

FormatOutputStream::FormatOutputStream(OutputStream& out, unsigned int size) throw(BindException) :
  BufferedOutputStream(out, size) {

  defaultIntegerBase = Symbols::DECIMAL;
  defaultRealBase = Symbols::DECIMAL;
  defaultRealStyle = Symbols::FIXED;
  defaultFlags = Symbols::PREFIX | Symbols::NECESSARY | Symbols::POSIX;
  defaultWidth = 0;
  defaultPrecision = 6;
  endOfLine = Symbols::UNIXEOL;
  prepareForField();
}

FormatOutputStream& FormatOutputStream::setWidth(unsigned int width) throw() {
  SynchronizeExclusively();
  this->width = minimum(width, MAXIMUM_WIDTH);
  return *this;
}

FormatOutputStream& FormatOutputStream::setPrecision(unsigned int precision) throw() {
  SynchronizeExclusively();
  flags &= ~Symbols::NECESSARY;
  this->precision = minimum(precision, MAXIMUM_PRECISION);
  return *this;
}

FormatOutputStream& FormatOutputStream::setRadixPosition(unsigned int position) throw() {
  SynchronizeExclusively();
  justification = Symbols::RADIX;
  this->radixPosition = position;
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(Action action) throw(IOException) {
  static const char* LF_STR = "\n";
  static const char* LFCR_STR = "\n\r";
  static const char* CRLF_STR = "\r\n";

  SynchronizeExclusively();
  switch (action) {
  case BIN:
    integerBase = Symbols::BINARY;
    break;
  case OCT:
    integerBase = Symbols::OCTAL;
    break;
  case DEC:
    integerBase = Symbols::DECIMAL;
    break;
  case HEX:
    integerBase = Symbols::HEXADECIMAL;
    break;
  case FBIN:
    realBase = Symbols::BINARY;
    break;
  case FOCT:
    realBase = Symbols::OCTAL;
    break;
  case FDEC:
    realBase = Symbols::DECIMAL;
    break;
  case FHEX:
    realBase = Symbols::HEXADECIMAL;
    break;
  case SCIENTIFIC:
    realStyle = Symbols::SCIENTIFIC;
    break;
  case FIXED:
    realStyle = Symbols::FIXED;
    break;
  case ENGINEERING:
    realStyle = Symbols::ENGINEERING;
    break;
  case ZEROPAD:
    flags |= Symbols::ZEROPAD;
    break;
  case NOZEROPAD:
    flags &= ~Symbols::ZEROPAD;
    break;
  case PREFIX:
    flags |= Symbols::PREFIX;
    break;
  case NOPREFIX:
    flags &= ~Symbols::PREFIX;
    break;
  case GROUPING:
    flags |= Symbols::GROUPING;
    break;
  case NOGROUPING:
    flags &= ~Symbols::GROUPING;
    break;
  case PLUS:
    flags |= Symbols::PLUS;
    break;
  case FPLUS:
    flags |= Symbols::FPLUS;
    break;
  case PLUSEXP:
    flags |= Symbols::PLUSEXP;
    break;
  case ZEROPADEXP:
    flags |= Symbols::ZEROPADEXP;
    break;
  case NECESSARY:
    flags |= Symbols::NECESSARY;
    break;
  case UPPER:
    flags |= Symbols::UPPER;
    break;
  case LOWER:
    flags &= ~Symbols::UPPER;
    break;
  case POSIX:
    flags |= Symbols::POSIX;
    break;
  case LOCALE:
    flags &= ~Symbols::POSIX;
    break;
  case LEFT:
    justification = Symbols::LEFT;
    break;
  case RIGHT:
    justification = Symbols::RIGHT;
    break;
  case UNIXEOL:
    endOfLine = Symbols::UNIXEOL;
    break;
  case WINDOWSEOL:
    endOfLine = Symbols::WINDOWSEOL;
    break;
  case MACEOL:
    endOfLine = Symbols::MACEOL;
    break;
  case LF:
    write(LF_STR, 1); // may throw IOException
    break;
  case CRLF:
    write(CRLF_STR, 2); // may throw IOException
    break;
  case LFCR:
    write(LFCR_STR, 2); // may throw IOException
    break;
  case EOL:
    switch (endOfLine) {
    case Symbols::UNIXEOL:
      write(LF_STR, 1); // may throw IOException
      break;
    case Symbols::WINDOWSEOL:
      write(CRLF_STR, 2); // may throw IOException
      break;
    case Symbols::MACEOL:
      write(LFCR_STR, 2); // may throw IOException
      break;
    }
    break;
  case FLUSH:
    flush(); // may throw IOException
    break;
  case ENDL:
    switch (endOfLine) {
    case Symbols::UNIXEOL:
      write(LF_STR, 1); // may throw IOException
      break;
    case Symbols::WINDOWSEOL:
      write(CRLF_STR, 2); // may throw IOException
      break;
    case Symbols::MACEOL:
      write(LFCR_STR, 2); // may throw IOException
      break;
    }
    flush(); // may throw IOException
    break;
  case PUSH:
    // push to context stack
    break;
  case POP:
    // pop to context stack
    break;
  }
  return *this;
}

/*
  FormatOutputStream& beginEntity() throw(IOException) {
    push entity buffer
    setup new entity buffer
    set ENTITY flag
  }

  FormatOutputStream& endEntity() throw(IOException) {
    reset ENTITY flag
    prepareForField();
    free entity buffer
    pop entity buffer
  }
*/

void FormatOutputStream::prepareForField() {
  // copy default context to current context
  /*
    if (ENTITY flag is set) {
      restore context
    } else {
      write intermediate buffer to output stream
      restore context
    }
  */
  integerBase = defaultIntegerBase;
  realBase = defaultRealBase;
  realStyle = defaultRealStyle;
  flags = defaultFlags;
  width = defaultWidth;
  precision = defaultPrecision;
  justification = Symbols::RIGHT;
}

FormatOutputStream& FormatOutputStream::getContext(Context& context) throw() {
}

FormatOutputStream& FormatOutputStream::setContext(const Context& context) throw() {
}

void FormatOutputStream::addCharacterField(const char* buffer, unsigned int size) throw(IOException) {
  SynchronizeExclusively();
  if (justification == Symbols::LEFT) { // left justification
    write(buffer, size); // write characters
  }
  if (size < width) { // write blanks if required
    unfoldValue(' ', width - size);
  }
  if (justification != Symbols::LEFT) { // right justification (could also be Symbols::RADIX)
    write(buffer, size); // write characters
  }
  prepareForField();
}

void FormatOutputStream::addIntegerField(const char* buffer, unsigned int size, bool isSigned) throw(IOException) {
  SynchronizeExclusively();
  unsigned int requiredWidth = size;

  if (isSigned) {
    ++requiredWidth; // "-"
  }

  if ((flags & Symbols::PREFIX) != 0) { // is prefix requested
    switch (integerBase) {
    case Symbols::BINARY:
      requiredWidth += 2;
      break;
    case Symbols::OCTAL:
      requiredWidth += 1;
      break;
    case Symbols::DECIMAL:
      requiredWidth += 0;
      break;
    case Symbols::HEXADECIMAL:
      requiredWidth += 2;
      break;
    }
  }

  unsigned int pads = (requiredWidth >= width) ? 0 : (width - requiredWidth);

  if (justification != Symbols::LEFT) { // right justify if RADIX or RIGHT
    if ((pads > 0) && ((flags & Symbols::ZEROPAD) == 0)) { // write blanks if required
      unfoldValue(' ', pads);
    }
  }

  if (isSigned) {
    write("-", 1); // write sign
  }

  if ((flags & Symbols::PREFIX) != 0) { // is prefix requested
    switch (integerBase) {
    case Symbols::BINARY:
      write("0", 1);
      write(((flags & FormatOutputStream::Symbols::UPPER) == 0) ? "b" : "B", 1);
      break;
    case Symbols::OCTAL:
      write("0", 1);
      break;
    case Symbols::DECIMAL:
      break;
    case Symbols::HEXADECIMAL:
      write("0", 1);
      write(((flags & FormatOutputStream::Symbols::UPPER) == 0) ? "x" : "X", 1);
      break;
    }
  }

  if ((pads > 0) && ((flags & Symbols::ZEROPAD) != 0)) { // do we need to extend field with digits
    unsigned int value = 0;
    if (isSigned) {
      switch (integerBase) {
      case Symbols::BINARY:
        value = 0x01;
        break;
      case Symbols::OCTAL:
        value = 0x00;
        break;
      case Symbols::DECIMAL:
        value = 0x00;
        break;
      case Symbols::HEXADECIMAL:
        value = 0x0f;
        break;
      }
    }
    char digit = ASCIITraits::valueToDigit(value, (flags & FormatOutputStream::Symbols::UPPER) != 0);
    unfoldValue(digit, pads);
  }

  write(buffer, size); // write late buffer
  if (justification == Symbols::LEFT) {
    unfoldValue(' ', pads);
  }
  prepareForField();
}

FormatOutputStream::~FormatOutputStream() throw(IOException) {
  TRACE_MEMBER();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, bool value) throw(IOException) {
  return stream << (value ? "true" : "false"); // TAG: need locale support
}

FormatOutputStream& operator<<(FormatOutputStream& stream, char value) throw(IOException) {
  stream.addCharacterField(&value, 1);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const char* value) throw(IOException) {
  if (value) {
    unsigned int length = strlen(value); // TAG: replace strlen
    stream.addCharacterField(value, length);
  }
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, short int value) throw(IOException) {
  char buffer[sizeof(short int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned short int temp = (unsigned short int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned short int temp = (unsigned short int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned short int temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned short int temp = (unsigned short int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned short int value) throw(IOException) {
  char buffer[sizeof(unsigned short int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, int value) throw(IOException) {
  char buffer[sizeof(int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned int temp = (unsigned int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned int temp = (unsigned int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned int temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned int temp = (unsigned int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned int value) throw(IOException) {
  char buffer[sizeof(unsigned int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, long value) throw(IOException) {
  char buffer[sizeof(long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned long temp = (unsigned long)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned long temp = (unsigned long)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned long temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned long temp = (unsigned long)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long value) throw(IOException) {
  char buffer[sizeof(unsigned long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, long long int value) throw(IOException) {
  char buffer[sizeof(long long int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      unsigned long long int temp = (unsigned long long int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000001); // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      unsigned long long int temp = (unsigned long long int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x00000007); // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      unsigned int temp = (value >= 0) ? value : -value;
      do {
        *dest = ASCIITraits::valueToDigit(temp % 10); // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      unsigned long long int temp = (unsigned long long int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        temp >>= 4; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), value < 0);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long long int value) throw(IOException) {
  char buffer[sizeof(unsigned long long int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0); // get bits of digit
        value >>= 4; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(dest, sizeof(buffer) - (dest - buffer), false);
  return stream;
}





namespace LargeInteger {

  inline void clear(unsigned int* value, unsigned size) throw() {
    for (const unsigned int* end = value + size; value < end; ++value) {
      *value = 0;
    }
  }

  inline void assign(unsigned int* dest, const unsigned int* src, unsigned size) throw() {
    if (src != dest) {
      for (const unsigned int* end = dest + size; dest < end; ++src, ++dest) {
        *dest = *src;
      }
    }
  }

  inline unsigned int divide(unsigned int* value, unsigned int size, unsigned int divisor) throw() {
    unsigned int* word = value + size;
    unsigned int remainder = 0;
    while (--word >= value) {
      unsigned long long temp = (static_cast<unsigned long long>(remainder) << 32) + *word;
      *word = temp/divisor;
      remainder = temp%divisor;
    }
    return remainder;
  }

  inline void setBit(unsigned int* value, unsigned int size, unsigned int bit) throw() {
    ASSERT(bit < (size*32));
    unsigned int* dest = value;
    for (const unsigned int* end = value + size; dest < end; ++dest) {
      *dest = 0;
    }
    value[bit/32] = 1U << (bit % 32);
  }

  inline bool addBit(unsigned int* value, unsigned int size, unsigned int bit) throw() {
    ASSERT(bit < (size*32));
    const unsigned int* end = value + size;
    value += bit/32;
    unsigned int carrier = 1 << (bit%32);
    for (; value < end; ++value) {
      unsigned long long temp = *value + carrier;
      *value = temp;
      carrier = temp >> 32;
    }
    return carrier > 0;
  }

  inline void leftShift(unsigned int* value, unsigned int size, unsigned int shift) throw() {
    unsigned int bitShift = shift % 32;
    unsigned int wordShift = shift/32;
    const unsigned int* src = value;
    unsigned int* dest = value + wordShift;
    const unsigned int* end = value + size;

    if (bitShift != 0) {
      unsigned int previousBitShift = 32 - bitShift;
      unsigned int previousValue = 0;
      for (; dest < end; ++src, ++dest) {
        unsigned int temp = *src;
        *dest = (temp << bitShift) | previousValue;
        previousValue = temp >> previousBitShift;
      }
    } else {
      for (; dest < end; ++src, ++dest) {
        *dest = *src;
      }
    }
    fill(value, wordShift, 0U); // mask beginning of value
  }

  // TAG: fix this - prevent overflow
  inline void rightShift(unsigned int* value, unsigned int size, unsigned int shift) throw() {
    unsigned int bitShift = shift % 32;
    unsigned int wordShift = shift/32;
    const unsigned int* src = value + wordShift;
    unsigned int* dest = value;

    if (bitShift != 0) {
      unsigned int nextBitShift = 32 - bitShift; // 0 < nextBitShift < 32
      for (const unsigned int* end = value + (size - wordShift - 1); dest < end; ++dest) {
        unsigned int temp = *src >> bitShift;
        ++src;
        temp |= *src << nextBitShift;
        *dest = temp;
      }
      *dest++ = *src >> bitShift;
    } else {
      for (const unsigned int* end = value + (size - wordShift); dest < end; ++src, ++dest) {
        *dest = *src;
      }
    }
    for (const unsigned int* end = value + size; dest < end; ++dest) { // mask end of value
      *dest = 0;
    }
  }

  inline bool add(unsigned int* value, unsigned int size, unsigned int a) throw() {
    const unsigned int* end = value + size;
    unsigned int carrier = a;
    for (; value < end; ++value) {
      unsigned long long temp = *value + carrier;
      *value = temp;
      carrier = temp >> 32;
    }
    return carrier > 0;
  }

  inline bool add(unsigned int* value, const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = value + size;
    unsigned int carrier = 0;
    for (; value < end; ++value, ++right) {
      unsigned long long temp = static_cast<unsigned long long>(*value) + *right + carrier;
      *value = temp;
      carrier = temp >> 32;
    }
    return carrier > 0;
  }

  inline bool subtract(unsigned int* value, const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = value + size;
    unsigned int borrow = 0;
    for (; value < end; ++value, ++right) {
      ASSERT(borrow <= 1);
      unsigned long long temp = (1ULL << 32) + *value - *right - borrow;
      *value = temp;
      borrow = (temp >> 32) ? 0 : 1;
    }
    return borrow > 0;
  }

  inline bool checkOverflow(const unsigned int* left, const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = left + size;
    unsigned int carrier = 0;
    for (; left < end; ++left, ++right) {
      unsigned long long temp = static_cast<unsigned long long>(*left) + *right + carrier;
      carrier = temp >> 32;
    }
    return carrier > 0;
  }

  inline bool multiply(unsigned int* value, unsigned int size, unsigned int m) throw() {
    const unsigned int* end = value + size;
    unsigned int carrier = 0;
    for (; value < end; ++value) {
      unsigned long long temp = static_cast<unsigned long long>(*value) * m + carrier;
      *value = temp;
      carrier = temp >> 32;
    }
    return carrier > 0;
  }

  inline unsigned int getSize(const unsigned int* value, unsigned int size) throw() {
    value += size; // start at end of value
    for (unsigned int i = 0; i < size; ++i) {
      if (*--value != 0) {
        return size - i;
      }
    }
    return 0; // all words are zero
  }

  inline unsigned int getBitSize(const unsigned int* value, unsigned int size) throw() {
    const unsigned int* current = value + size; // start at end of value
    while (current >= value) {
      unsigned int temp = *--current;
      if (temp != 0) {
        for (unsigned int bit = 0; bit < 32; ++bit) {
          if ((temp << bit) & 0x80000000) {
            return (current - value) * 32 + (32 - bit);
          }
        }
      }
    }
    return 0; // all bits are zero
  }

  inline bool isZero(const unsigned int* value, unsigned int size) throw() {
    for (const unsigned int* end = value + size; value < end; ++value) {
      if (*value != 0) {
        return false;
      }
    }
    return true;
  }

  inline bool lessThan(const unsigned int* left, const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = left;
    left += size;
    right += size;
    while (left > end) {
      --left;
      --right;
      if (*left < *right) {
        return true;
      } else if (*left > *right) {
        return false;
      }
    }
    return false;
  }

  inline bool equal(const unsigned int* left, const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = left + size;
    while (left < end) {
      if (*left++ != *right++) {
        return false;
      }
    }
    return true;
  }

  // may remainder be the same as dividend - I think so
  inline void divide(unsigned int* quotient, unsigned int* remainder, const unsigned int* dividend, const unsigned int* divisor, unsigned int size) throw() {
    unsigned int temp[size];
    clear(quotient, size);
    unsigned int* tempDividend = remainder;
    assign(tempDividend, dividend, size);
    assign(temp, divisor, size);

    if (lessThan(tempDividend, divisor, size)) {
      return;
    }

    unsigned int divisorBitSize = getBitSize(divisor, size);
    unsigned int positionOfDivisor = getBitSize(tempDividend, size) - divisorBitSize;
    leftShift(temp, size, positionOfDivisor);

    unsigned int reducedSize = size;

    while (!lessThan(tempDividend, divisor, reducedSize)) {
      // assert: temp >= divisor
      unsigned int newPosition = getBitSize(tempDividend, reducedSize) - divisorBitSize; // 0 <= newPosition < positionOfDivisor
      rightShift(temp, reducedSize, positionOfDivisor - newPosition);
      positionOfDivisor = newPosition;

      if (lessThan(tempDividend, temp, reducedSize)) { // shift = 0 => return value is false
        rightShift(temp, reducedSize, 1);
        --positionOfDivisor;
      }

      addBit(quotient, size, positionOfDivisor); // must be size - is any carrier possible when divisor > 0

      subtract(tempDividend, temp, reducedSize);
    }
    // tempDividend is remainder now
  }

}; // end of namespace



void convertFloatingPoint(unsigned int precision, unsigned int* mantissa, unsigned int mantissaSize, int base2Exponent, byte* buffer, unsigned int& numberOfDigits, int& exponent) throw() {
  numberOfDigits = 0;
  exponent = 0;

  if (precision == 0) {
    return;
  }
  --precision;

  bool unequalGap = true;
  for (unsigned int i = 0; i < mantissaSize - 1; ++i) {
    unequalGap &= (mantissa[i] == 0);
  }
  if (unequalGap) {
    if (mantissa[mantissaSize - 1] == 0) {
      buffer[numberOfDigits++] = 0;
      return;
    } else if (mantissa[mantissaSize - 1] == (1 << ((precision - 1)%32))) {
    } else {
      unequalGap = false;
    }
  }

  unsigned int shift = maximum<int>(-(base2Exponent - precision), 0);
  unsigned int valueShift = maximum<int>(base2Exponent - precision, 0);

  unsigned int integerSize = (maximum<int>(maximum<int>(base2Exponent, -base2Exponent), precision)+31)/32 + 1 + 1; // number of words in large integers
  // TAG: fix calc of integerSize (use worst case) depends on *(B)^?

  unsigned int S[integerSize];
  unsigned int R[integerSize];
  unsigned int Mminus[integerSize];
  unsigned int Mplus[integerSize];
  unsigned int temp[integerSize];

  LargeInteger::clear(R, integerSize);
  LargeInteger::assign(R, mantissa, mantissaSize);

  if (unequalGap) { // f == 1 << (p-1) // TAG: does this work for denormalized values
    LargeInteger::setBit(S, integerSize, shift + 2); // S = 2*S_paper
    LargeInteger::leftShift(R, integerSize, valueShift + 2); // R = 2*R_paper
    LargeInteger::setBit(Mplus, integerSize, valueShift + 1); // Mplus = M+
    LargeInteger::setBit(Mminus, integerSize, valueShift); // Mminus = M-
  } else {
    LargeInteger::setBit(S, integerSize, shift + 1); // S = 2*S_paper
    LargeInteger::leftShift(R, integerSize, valueShift + 1); // R = 2*R_paper
    LargeInteger::setBit(Mplus, integerSize, valueShift); // Mplus = M+
    LargeInteger::setBit(Mminus, integerSize, valueShift); // Mminus = M-
  }

  LargeInteger::assign(temp, S, integerSize); // TAG: need faster algo
  LargeInteger::add(temp, integerSize, 10 - 1);
  LargeInteger::divide(temp, integerSize, 10);// ceil(S/B)
  while (LargeInteger::lessThan(R, temp, integerSize)) { // R < ceil(S/B) => R < (S+B-1)/B
    LargeInteger::multiply(R, integerSize, 10); // R = R * B
    LargeInteger::multiply(Mminus, integerSize, 10); // M- = M- * B
    LargeInteger::multiply(Mplus, integerSize, 10); // M+ = M+ * B
    --exponent;
  }

  LargeInteger::assign(temp, R, integerSize); // TAG: need faster algo
  LargeInteger::add(temp, Mplus, integerSize);
  while (!LargeInteger::lessThan(temp, S, integerSize)) { // 2*R + M+ >= 2*S
    LargeInteger::multiply(S, integerSize, 10); // S = S * B
    ++exponent;
  }

  bool low = false;
  bool high = false;
  while (!low && !high) { // assert: R < S
    LargeInteger::multiply(R, integerSize, 10); // R = R * B
    LargeInteger::multiply(Mminus, integerSize, 10); // M- = M- * B
    LargeInteger::multiply(Mplus, integerSize, 10); // M+ = M+ * B

    LargeInteger::divide(temp, R, R, S, integerSize); // U = R/S and R=R%S
    buffer[numberOfDigits++] = temp[0]; // assert: U < B

    low = LargeInteger::lessThan(R, Mminus, integerSize); // R < M-/2
    LargeInteger::assign(temp, R, integerSize); // TAG: need alternative - temp = R + M+/2
    LargeInteger::assign(temp, Mplus, integerSize);
    high = LargeInteger::lessThan(S, temp, integerSize); // 2*R > 2*S - M+ <=> R + (M+/2) > S
  }

  //bool unspecifiedRound = false;
  bool carrier = false;
  if (low && !high) {
  } else if (high && !low) {
    carrier = true;
  } else {
    LargeInteger::leftShift(R, integerSize, 1); // R = 2 * R // TAG: need ordinary compare
    if (LargeInteger::lessThan(S, R, integerSize)) { // 2 * R > S
      carrier = true;
    }
    //unspecifiedRound = true; // TAG: should this be taken into account
  }

  // TAG: possible overflow - I thought that the algorithm should prevent this
  if (carrier) {
    for (unsigned int i = numberOfDigits - 1; i >= 0; --i) { // handle carrier
      if (buffer[i] == (10 - 1)) {
        --numberOfDigits; // remove trailing zero and continue with carrier
      } else {
        ++buffer[i];
        break;
      }
    }
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, float value) throw(IOException) {
  union {
    float primitive;
    FloatRepresentation fields;
  } representation;
  representation.primitive = value;

  unsigned int precision;
  unsigned int mantissa[(FloatRepresentation::SIGNIFICANT + 31)/32];
  int exponent;
  unsigned int flags;
  analyseFloatingPoint(representation.fields, precision, mantissa, exponent, flags);
  stream.writeFloatingPointType(precision, mantissa, (FloatRepresentation::SIGNIFICANT + 31)/32, exponent, flags);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, double value) throw(IOException) {
  union {
    double primitive;
    DoubleRepresentation fields;
  } representation;
  representation.primitive = value;

  unsigned int precision;
  unsigned int mantissa[(DoubleRepresentation::SIGNIFICANT + 31)/32];
  int exponent;
  unsigned int flags;
  analyseFloatingPoint(representation.fields, precision, mantissa, exponent, flags);
  stream.writeFloatingPointType(precision, mantissa, (DoubleRepresentation::SIGNIFICANT + 31)/32, exponent, flags);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, long double value) throw(IOException) {
  union {
    long double primitive;
    LongDoubleRepresentation fields;
  } representation;
  representation.primitive = value;

  unsigned int precision;
  unsigned int mantissa[(LongDoubleRepresentation::SIGNIFICANT + 31)/32];
  int exponent;
  unsigned int flags;
  analyseFloatingPoint(representation.fields, precision, mantissa, exponent, flags);
  stream.writeFloatingPointType(precision, mantissa, (LongDoubleRepresentation::SIGNIFICANT + 31)/32, exponent, flags);
  return stream;
}

void FormatOutputStream::writeFloatingPointType(unsigned int significant, unsigned int* mantissa, unsigned int mantissaSize, int base2Exponent, unsigned int valueFlags) throw(IOException) {
  char buffer[128]; // TAG: use maximum limit
  char* output = buffer;
  const char* radix = 0;
  unsigned int flags = this->flags;

  if ((valueFlags & FP_NAN) != 0) {
    if ((flags & Symbols::UPPER) == 0) {
      copy(output, "nan", sizeof("nan") - 1); // TAG: I guess this should be locale specific
      output += sizeof("nan") - 1;
    } else {
      copy(output, "NAN", sizeof("NAN") - 1); // TAG: I guess this should be locale specific
      output += sizeof("NAN") - 1;
    }
  } else {

    if ((flags & Symbols::POSIX) != 0) { // use POSIX
      if ((valueFlags & FP_NEGATIVE) != 0) {
        *output++ = '-';
      } else if ((flags & Symbols::FPLUS) != 0) { // show plus if sign is forced
        *output++ = '+';
      }
    } else { // use locale
      // numeric or currency
      // const char* str = (valueFlags & FP_NEGATIVE) ? locale->getBeginNegative() : locale->getBeginPositive();
      // copy to output
    }

    if ((valueFlags & FP_INFINITY) != 0) {
      if ((flags & Symbols::UPPER) == 0) {
        copy(output, "infinity", sizeof("infinity") - 1); // TAG: I guess this should be locale specific
        output += sizeof("infinity") - 1;
      } else {
        copy(output, "INFINITY", sizeof("INFINITY") - 1); // TAG: I guess this should be locale specific
        output += sizeof("INFINITY") - 1;
      }
    } else {
      byte digitBuffer[128]; // TAG: use maximum limit
      unsigned int numberOfDigits;
      int exponent;
      convertFloatingPoint(significant, mantissa, mantissaSize, base2Exponent, digitBuffer, numberOfDigits, exponent);
      int adjustedExponent = exponent;
      bool showExponent = true;

      switch (realStyle) {
      case Symbols::SCIENTIFIC: // one digit before radix character
        if ((valueFlags & FP_ZERO) == 0) {
          adjustedExponent = exponent - 1;
        }
        break;
      case Symbols::FIXED:
        if (exponent <= static_cast<int>(maximum<int>(precision, numberOfDigits))) { // TAG: need number of significant digits (base 10)
          adjustedExponent = 0;
          showExponent = false;
        } else {
          adjustedExponent = exponent - 1;
        }
        break;
      case Symbols::ENGINEERING: // 1-3 digits before radix character
        if (exponent >= 0) {
          adjustedExponent = (exponent - 1)/3*3; // round towards -infinity
        } else {
          adjustedExponent = (exponent - 3)/3*3; // round towards -infinity
        }
        break;
      }

      if ((flags & Symbols::NECESSARY) == 0) { // should we use precision
        int cutDigits = precision + (exponent - adjustedExponent);
        if (cutDigits < 1) {
          numberOfDigits = 0;
	} else if (cutDigits < numberOfDigits) { // cut off digits
          bool carrier = false;
          if (digitBuffer[cutDigits] > 4) {
            carrier = true;
          } else if ((digitBuffer[cutDigits] == 4) && ((cutDigits + 1) < numberOfDigits)) {
            carrier = true;
            for (unsigned int i = cutDigits + 1; (i < numberOfDigits) && carrier; ++i) {
              carrier = (digitBuffer[i] == (10 - 1));
            }
          }
          numberOfDigits = minimum<int>(numberOfDigits, cutDigits);

          if (carrier) {
            for (unsigned int i = numberOfDigits - 1; i >= 0; --i) { // handle carrier
              if (digitBuffer[i] == (10 - 1)) {
                --numberOfDigits; // remove trailing zero and continue with carrier
              } else {
                ++digitBuffer[i];
                carrier = false;
                break;
              }
            }
          }

          if (carrier) { // => numberOfDigits = 0
            digitBuffer[numberOfDigits++] = 1;
            --exponent; // TAG: problem need to re-adjust?
          }
        }
      }

      const byte* digit = digitBuffer;
      const byte* endDigit = digit + numberOfDigits;
      int digitsBeforeRadix = ((valueFlags & FP_ZERO) == 0) ? maximum(exponent - adjustedExponent, 0) : 1;
      int denormalizingZeros = minimum(maximum(adjustedExponent - exponent, 0), precision);

      if (digitsBeforeRadix > 0) {
        if ((flags & Symbols::POSIX) != 0) { // use posix (no grouping char specified)
          for (int i = digitsBeforeRadix - 1; i >= 0; --i) { // write all but one digit
            *output++ = (digit < endDigit) ? ASCIITraits::valueToDigit(*digit++) : ASCIITraits::valueToDigit(0);
          }
        } else {
          const char* groupingStr = ","; // = locale.getGrouping(); // TAG: fix
          for (int i = digitsBeforeRadix - 1; i >= 0; --i) { // write all but one digit
            *output++ = (digit < endDigit) ? ASCIITraits::valueToDigit(*digit++) : ASCIITraits::valueToDigit(0);
            if (i == 0) {
              break; // do not write last grouping separator (that is the radix position)
	    }
            if (((flags & Symbols::GROUPING) != 0) && (i % 3 == 0)) {
              for (const char* p = groupingStr; *p != 0; ++p, ++output) {
                *output = *p;
	      }
            }
          }
        }
      } else { // special case where 0 not contained in digit buffer
        *output++ = ASCIITraits::valueToDigit(0);
      }

      unsigned int digitsAfterRadix = (numberOfDigits > digitsBeforeRadix) ? numberOfDigits - digitsBeforeRadix : 0;
      unsigned int totalDigitsAfterRadix = digitsAfterRadix + denormalizingZeros;
      int trailingZeros = 0;

      if ((flags & Symbols::NECESSARY) == 0) {
        if (totalDigitsAfterRadix < precision) {
          trailingZeros += precision - totalDigitsAfterRadix;
          totalDigitsAfterRadix += trailingZeros;
        }
      }

      if ((totalDigitsAfterRadix > 0) || (justification == Symbols::RADIX)) {
        radix = output; // remember position of radix for later justification
        // only show radix char if followed by digits or used in justification
        if ((flags & Symbols::POSIX) != 0) {
          *output++ = '.';
	} else { // use locale
          //for (const char* p = locale.getDecimalPoint(); *p != 0; ++p, ++output) {
          //  *output = *p;
          //}
        }
        for (int i = denormalizingZeros; i > 0; --i) {
          *output++ = ASCIITraits::valueToDigit(0);
        }
        for (int i = digitsAfterRadix; i > 0; --i) { // write significant digits
          *output++ = ASCIITraits::valueToDigit(*digit++);
        }
        for (int i = trailingZeros; i > 0; --i) {
          *output++ = ASCIITraits::valueToDigit(0);
        }
      }

      if (showExponent) {
        *output++ = ((flags & Symbols::UPPER) == 0) ? 'e' : 'E';

        if (adjustedExponent < 0) {
          *output++ = '-';
        } else if ((flags & Symbols::PLUSEXP) != 0) { // only show positive exponent explicitly if forced
          *output++ = '+';
        }

        static const unsigned int BASE10_EXPONENTS[] = {1, 10, 100, 1000};
        bool writeDigit = ((flags & Symbols::ZEROPADEXP) != 0);
        unsigned int temp = (adjustedExponent >= 0) ? adjustedExponent : -adjustedExponent;
        for (int i = 3; i >= 0; --i) { // TAG: need limit as arg of method?
          unsigned int digitValue = temp/BASE10_EXPONENTS[i] % 10;
          if (writeDigit || (digitValue != 0) || (i == 0)) {
            writeDigit = true; // first non-zero digit forces following zero-digits to be written
            *output++ = ASCIITraits::valueToDigit(digitValue);
          }
        }
      }

      if ((flags & Symbols::POSIX) == 0) { // use locale
        // numeric or monetary
        // const char* str = (valueFlags & FP_NEGATIVE) ? locale->getEndNegative() : locale->getEndPositive();
        // copy to output
      }
    }
  }

  {
    SynchronizeExclusively();
    unsigned int length = (output - buffer);
    if (width <= length) {
      write(buffer, length); // write characters
    } else {
      unsigned int invertedLength = width - length;
      switch (justification) {
      case Symbols::LEFT:
        write(buffer, length); // write characters
        unfoldValue(' ', invertedLength);
        break;
      case Symbols::RIGHT:
        unfoldValue(' ', invertedLength);
        write(buffer, length); // write characters
        break;
      case Symbols::RADIX:
        ASSERT(radix);
        unsigned int beforeRadix = radix - buffer; // character before radix (excluding)
        unsigned int prefixLength = 0; // left justify by default
        if (radixPosition >= beforeRadix) {
          prefixLength = minimum(radixPosition - beforeRadix, invertedLength); // right justify if radix position is too big
          unfoldValue(' ', prefixLength);
        }
        write(buffer, length); // write characters
        unfoldValue(' ', invertedLength - prefixLength);
        break;
      }
    }
    prepareForField();
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
