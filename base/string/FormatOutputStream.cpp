/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/FloatingPoint.h>
#include <base/concurrency/ExclusiveSynchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

const FormatOutputStream::Context FormatOutputStream::DEFAULT_CONTEXT = {
  DEFAULT_FLAGS,
  DEFAULT_EOL,
  DEFAULT_INTEGER_BASE,
  DEFAULT_REAL_BASE,
  DEFAULT_REAL_STYLE,
  DEFAULT_RADIX_POSITION,
  DEFAULT_JUSTIFICATION,
  DEFAULT_WIDTH,
  DEFAULT_PRECISION
};

FormatOutputStream::FormatOutputStream(OutputStream& out, unsigned int size) throw(BindException)
  : BufferedOutputStream(out, size), defaultContext(DEFAULT_CONTEXT), context(DEFAULT_CONTEXT) {
}

FormatOutputStream& FormatOutputStream::setRadixPosition(unsigned int position) throw() {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  context.justification = Symbols::RADIX;
  context.radixPosition = position;
  return *this;
}

FormatOutputStream& FormatOutputStream::setJustification(Symbols::Justification justification) throw() {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  context.justification = justification;
  return *this;
}

FormatOutputStream& FormatOutputStream::setWidth(unsigned int width) throw() {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  context.width = minimum(width, MAXIMUM_WIDTH);
  return *this;
}

FormatOutputStream& FormatOutputStream::setPrecision(unsigned int precision) throw() {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  context.flags &= ~Symbols::NECESSARY;
  context.precision = minimum(precision, MAXIMUM_PRECISION);
  return *this;
}

FormatOutputStream& FormatOutputStream::operator<<(Action action) throw(IOException) {
  static const char* CR_STR = "\r";
  static const char* LF_STR = "\n";
  static const char* LFCR_STR = "\n\r";
  static const char* CRLF_STR = "\r\n";

  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  switch (action) {
  case BIN:
    context.integerBase = Symbols::BINARY;
    break;
  case OCT:
    context.integerBase = Symbols::OCTAL;
    break;
  case DEC:
    context.integerBase = Symbols::DECIMAL;
    break;
  case HEX:
    context.integerBase = Symbols::HEXADECIMAL;
    break;
  case FBIN:
    context.realBase = Symbols::BINARY;
    break;
  case FOCT:
    context.realBase = Symbols::OCTAL;
    break;
  case FDEC:
    context.realBase = Symbols::DECIMAL;
    break;
  case FHEX:
    context.realBase = Symbols::HEXADECIMAL;
    break;
  case SCIENTIFIC:
    context.realStyle = Symbols::SCIENTIFIC;
    break;
  case FIXED:
    context.realStyle = Symbols::FIXED;
    break;
  case ENGINEERING:
    context.realStyle = Symbols::ENGINEERING;
    break;
  case ZEROPAD:
    context.flags |= Symbols::ZEROPAD;
    break;
  case NOZEROPAD:
    context.flags &= ~Symbols::ZEROPAD;
    break;
  case PREFIX:
    context.flags |= Symbols::PREFIX;
    break;
  case NOPREFIX:
    context.flags &= ~Symbols::PREFIX;
    break;
  case GROUPING:
    context.flags |= Symbols::GROUPING;
    break;
  case NOGROUPING:
    context.flags &= ~Symbols::GROUPING;
    break;
  case PLUS:
    context.flags |= Symbols::PLUS;
    break;
  case FPLUS:
    context.flags |= Symbols::FPLUS;
    break;
  case PLUSEXP:
    context.flags |= Symbols::PLUSEXP;
    break;
  case ZEROPADEXP:
    context.flags |= Symbols::ZEROPADEXP;
    break;
  case NECESSARY:
    context.flags |= Symbols::NECESSARY;
    break;
  case UPPER:
    context.flags |= Symbols::UPPER;
    break;
  case LOWER:
    context.flags &= ~Symbols::UPPER;
    break;
  case POSIX:
    context.flags |= Symbols::POSIX;
    break;
  case LOCALE:
    context.flags &= ~Symbols::POSIX;
    break;
  case DEPENDENT:
    context.justification = Symbols::DEPENDENT;
    break;
  case LEFT:
    context.justification = Symbols::LEFT;
    break;
  case RIGHT:
    context.justification = Symbols::RIGHT;
    break;
  case UNIXEOL:
    context.endOfLine = Symbols::UNIXEOL;
    break;
  case WINDOWSEOL:
    context.endOfLine = Symbols::WINDOWSEOL;
    break;
  case MACEOL:
    context.endOfLine = Symbols::MACEOL;
    break;
  case CR:
    write(CR_STR, 1); // may throw IOException
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
    switch (context.endOfLine) {
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
    switch (context.endOfLine) {
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

FormatOutputStream& FormatOutputStream::getContext(Context& context) throw() {
}

FormatOutputStream& FormatOutputStream::setContext(const Context& context) throw() {
}

void FormatOutputStream::addCharacterField(const char* buffer, unsigned int size) throw(IOException) {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);

  Symbols::Justification justification;
  switch (context.justification) {
  case Symbols::RIGHT:
    justification = Symbols::RIGHT;
    break;
  case Symbols::LEFT:
    justification = Symbols::LEFT;
    break;
  case Symbols::RADIX: // no radix - use default
  case Symbols::DEPENDENT:
    justification = Symbols::LEFT; // TAG: is this locale specific
  }

  if (justification == Symbols::LEFT) {
    write(buffer, size); // write characters
  }
  if (size < context.width) { // write blanks if required
    unfoldValue(' ', context.width - size);
  }
  if (context.justification == Symbols::RIGHT) {
    write(buffer, size); // write characters
  }
  context = defaultContext;
}

void FormatOutputStream::addIntegerField(const char* buffer, unsigned int size, bool isSigned) throw(IOException) {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  unsigned int requiredWidth = size;

  if (isSigned && ((context.integerBase != Symbols::BINARY) && (context.integerBase != Symbols::HEXADECIMAL))) {
    ++requiredWidth; // "-"
  }

  if ((context.flags & Symbols::PREFIX) != 0) { // is prefix requested
    switch (context.integerBase) {
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

  Symbols::Justification justification;
  switch (context.justification) {
  case Symbols::LEFT:
    justification = Symbols::LEFT;
    break;
  case Symbols::RIGHT:
    justification = Symbols::RIGHT;
    break;
  case Symbols::RADIX: // no radix - use default
  case Symbols::DEPENDENT:
    justification = Symbols::RIGHT; // TAG: is this locale specific
  }

  unsigned int pads = (requiredWidth >= context.width) ? 0 : (context.width - requiredWidth);

  if (justification == Symbols::RIGHT) {
    if ((pads > 0) && ((context.flags & Symbols::ZEROPAD) == 0)) { // write blanks if required
      unfoldValue(' ', pads);
    }
  }

  if (isSigned && ((context.integerBase != Symbols::BINARY) && (context.integerBase != Symbols::HEXADECIMAL))) {
    write("-", 1); // write sign
  }

  if ((context.flags & Symbols::PREFIX) != 0) { // is prefix requested
    switch (context.integerBase) {
    case Symbols::BINARY:
      write("0", 1);
      write(((context.flags & FormatOutputStream::Symbols::UPPER) == 0) ? "b" : "B", 1);
      break;
    case Symbols::OCTAL:
      write("0", 1);
      break;
    case Symbols::DECIMAL:
      break;
    case Symbols::HEXADECIMAL:
      write("0", 1);
      write(((context.flags & FormatOutputStream::Symbols::UPPER) == 0) ? "x" : "X", 1);
      break;
    }
  }

  if ((pads > 0) && ((context.flags & Symbols::ZEROPAD) != 0)) { // do we need to extend field with digits
    unsigned int value = 0;
    if (isSigned) {
      switch (context.integerBase) {
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
    char digit = ASCIITraits::valueToDigit(value, (context.flags & FormatOutputStream::Symbols::UPPER) != 0);
    unfoldValue(digit, pads);
  }

  write(buffer, size); // write late buffer
  if (justification == Symbols::LEFT) {
    unfoldValue(' ', pads);
  }
  context = defaultContext;
}

FormatOutputStream::~FormatOutputStream() throw(IOException) {
}

FormatOutputStream& operator<<(FormatOutputStream& stream, bool value) throw(IOException) {
  return stream << (value ? MESSAGE("true") : MESSAGE("false")); // TAG: need locale support
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
      unsigned short int temp = *pointer_cast<const unsigned short int*>(&value); // no sign
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
      unsigned short int temp = *pointer_cast<const unsigned short int*>(&value); // no sign
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
      unsigned short int temp = *pointer_cast<const unsigned short int*>(&value); // no sign
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
      unsigned int temp = *pointer_cast<const unsigned int*>(&value); // no sign
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
      unsigned int temp = *pointer_cast<const unsigned int*>(&value); // no sign
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
      unsigned int temp = *pointer_cast<const unsigned int*>(&value); // no sign
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
      unsigned long temp = *pointer_cast<const unsigned long*>(&value); // no sign
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
      unsigned long temp = *pointer_cast<const unsigned long*>(&value); // no sign
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
      unsigned long temp = *pointer_cast<const unsigned long*>(&value); // no sign
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
      unsigned long long int temp = *pointer_cast<const unsigned long long int*>(&value); // no sign
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
      unsigned long long int temp = *pointer_cast<const unsigned long long int*>(&value); // no sign
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
      unsigned long long int temp = *pointer_cast<const unsigned long long int*>(&value); // no sign
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



class Sequence {
private:

  const unsigned int* value;
  const unsigned int size;
public:

  Sequence(unsigned int* _value, unsigned int _size) throw() : value(_value), size(_size) {
  }

  inline const unsigned int* getValue() const throw() {
    return value;
  }

  inline unsigned int getSize() const throw() {
    return size;
  }
};

FormatOutputStream& operator<<(FormatOutputStream& stream, const Sequence& value) throw() {
  const unsigned int* begin = value.getValue();
  const unsigned int* src = begin + value.getSize();
  while (src > begin) {
    if (*--src != 0) {
      ++src;
      while (src > begin) {
        stream << HEX << NOPREFIX << ZEROPAD << setWidth(sizeof(unsigned int) * 2) << *--src;
      }
      return stream;
    }
  }
  return stream << HEX << NOPREFIX << ZEROPAD << setWidth(sizeof(unsigned int) * 2) << 0;
}



class LargeInteger {
public:

  static inline void clear(unsigned int* value, unsigned int size) throw() {
    fill(value, size, 0U);
  }

  static inline void assign(/*restrict*/ unsigned int* dest, /*restrict*/ const unsigned int* src, unsigned int size) throw() {
    copy(dest, src, size);
  }

  static inline unsigned int divide(unsigned int* value, unsigned int size, unsigned int divisor) throw() {
    unsigned int* word = value + size;
    unsigned int remainder = 0;
    while (--word >= value) {
      unsigned long long temp = (static_cast<unsigned long long>(remainder) << (sizeof(unsigned int) * 8)) + *word;
      *word = temp/divisor;
      remainder = temp % divisor;
    }
    return remainder;
  }

  static inline void setBit(unsigned int* value, unsigned int size, unsigned int bit) throw() {
    ASSERT(bit < (size * sizeof(unsigned int) * 8));
    fill(value, size, 0U);
    value[bit/(sizeof(unsigned int) * 8)] = 1U << (bit % (sizeof(unsigned int) * 8));
  }

  static inline bool addBit(unsigned int* value, unsigned int size, unsigned int bit) throw() {
    ASSERT(bit < (size * sizeof(unsigned int) * 8));
    value += bit/(sizeof(unsigned int) * 8);
    unsigned int carrier = 1 << (bit % (sizeof(unsigned int) * 8));
    for (const unsigned int* end = value + size; value < end; ++value) {
      unsigned long long temp = *value + carrier;
      *value = temp;
      carrier = (temp & (1ULL << (sizeof(unsigned int) * 8))) ? 1 : 0; // TAG: check generated asm code
      if (carrier == 0) {
        return false;
      }
    }
    return true;
  }

  static inline void leftShift(unsigned int* value, unsigned int size, unsigned int shift) throw() {
    unsigned int bitShift = shift % (sizeof(unsigned int) * 8);
    unsigned int wordShift = shift/(sizeof(unsigned int) * 8);

    // start from last non-zero value + wordSize (but do not exceed end)
    unsigned int* dest = value + size - 1;
    const unsigned int* src = dest - wordShift;

    if (bitShift != 0) {
      unsigned int invBitShift = (sizeof(unsigned int) * 8) - bitShift;
      while (src > value) {
        *dest = (*src << bitShift) | (*--src >> invBitShift);
        --dest;
      }
      *dest = *src << bitShift; // final (shift in zeros)
    } else {
      for (; src >= value; --src, --dest) {
        *dest = *src;
      }
    }
    fill(value, wordShift, 0U); // mask beginning of value
  }
  
  // TAG: fix this - prevent overflow
  static inline void rightShift(unsigned int* value, unsigned int size, unsigned int shift) throw() {
    unsigned int bitShift = shift % (sizeof(unsigned int) * 8);
    unsigned int wordShift = shift/(sizeof(unsigned int) * 8);
    const unsigned int* src = value + wordShift;
    unsigned int* dest = value;
    
    if (bitShift != 0) {
      unsigned int nextBitShift = (sizeof(unsigned int) * 8) - bitShift; // 0 < nextBitShift < (sizeof(unsigned int) * 8)
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

  static inline bool add(unsigned int* value, unsigned int size, unsigned int a) throw() {
    const unsigned int* end = value + size;
    unsigned int carrier = a;
    for (; value < end; ++value) {
      unsigned long long temp = *value + carrier;
      *value = temp;
      carrier = temp >> (sizeof(unsigned int) * 8);
    }
    return carrier > 0;
  }
  
  static inline bool add(/*restrict*/ unsigned int* value, /*restrict*/ const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = value + size;
    unsigned int carrier = 0;
    for (; value < end; ++value, ++right) {
      unsigned long long temp = static_cast<unsigned long long>(*value) + *right + carrier;
      *value = temp;
      carrier = temp >> (sizeof(unsigned int) * 8);
    }
    return carrier > 0;
  }

  static inline bool subtract(/*restrict*/ unsigned int* value, /*restrict*/ const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = value + size;
    unsigned int borrow = 0;
    for (; value < end; ++value, ++right) {
      ASSERT(borrow <= 1);
      unsigned long long temp = (1ULL << (sizeof(unsigned int) * 8)) + *value - *right - borrow;
      *value = temp;
      borrow = (temp >> (sizeof(unsigned int) * 8)) ? 0 : 1;
    }
    return borrow > 0;
  }

  static inline bool checkOverflow(/*restrict*/ const unsigned int* left, /*restrict*/ const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = left + size;
    unsigned int carrier = 0;
    for (; left < end; ++left, ++right) {
      unsigned long long temp = static_cast<unsigned long long>(*left) + *right + carrier;
      carrier = temp >> (sizeof(unsigned int) * 8);
    }
    return carrier > 0;
  }

  static inline bool multiply(unsigned int* value, unsigned int size, unsigned int m) throw() {
    const unsigned int* end = value + size;
    unsigned int carrier = 0;
    for (; value < end; ++value) {
      unsigned long long temp = static_cast<unsigned long long>(*value) * m + carrier;
      *value = temp;
      carrier = temp >> (sizeof(unsigned int) * 8);
    }
    return carrier > 0;
  }

  static inline unsigned int getSize(const unsigned int* value, unsigned int size) throw() {
    const unsigned int* src = value + size; // start at end of value
    while (src > value) {
      if (*--src != 0) {
        return src - value + 1;
      }
    }
    return 0; // all words are zero
  }

  static inline unsigned int getBitSize(const unsigned int* value, unsigned int size) throw() {
    const unsigned int* src = value + size; // start at end of value
    while (src > value) {
      if (*--src != 0) {
        unsigned int temp = *src;
        for (int bit = (sizeof(unsigned int) * 8) - 1; bit >= 0; --bit) {
          if (temp >> bit) {
            return (src - value) * (sizeof(unsigned int) * 8) + bit + 1;
          }
        }
      }
    }
    return 0; // all bits are zero
  }

  static inline bool isZero(const unsigned int* value, unsigned int size) throw() {
    for (const unsigned int* end = value + size; value < end; ++value) {
      if (*value != 0) {
        return false;
      }
    }
    return true;
  }

  static inline bool lessThan(/*restrict*/ const unsigned int* left, /*restrict*/ const unsigned int* right, unsigned int size) throw() {
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

  static inline bool equal(/*restrict*/ const unsigned int* left, /*restrict*/ const unsigned int* right, unsigned int size) throw() {
    const unsigned int* end = left + size;
    while (left < end) {
      if (*left++ != *right++) {
        return false;
      }
    }
    return true;
  }

  // may remainder be the same as dividend - I think so
  static inline void divide(/*restrict*/ unsigned int* quotient, unsigned int* remainder, const unsigned int* dividend, /*restrict*/ const unsigned int* divisor, unsigned int size) throw() {
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
      // assert temp >= divisor
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

}; // LargeInteger



enum CutMode {
  CUT_MODE_NOGARBAGE,
  CUT_MODE_RELATIVE
};

void convertFloatingPoint(unsigned int significant, unsigned int precision, CutMode cutMode, FormatOutputStream::Symbols::RealStyle realStyle, /*restrict*/ unsigned int* mantissa, unsigned int mantissaSize, int base2Exponent, /*restrict*/ byte* buffer, unsigned int& numberOfDigits, int& exponent) throw() {
  // TAG: there is plenty room for optimization
  static const unsigned int power[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

  numberOfDigits = 0;
  exponent = 0; // TAG: fixme could already have been initialized (only in FASTEST mode)

  ASSERT(significant > 0);
  --significant; // TAG: why this

  bool roundUp = false;
  const unsigned int* nonZero = findPredicate(mantissa, mantissaSize, bind2Second(NotEqual<unsigned int>(), 0));
  if (!nonZero) {
    buffer[numberOfDigits++] = 0;
    return;
  }

  const unsigned int shiftS = maximum<int>(-(base2Exponent - significant), 0); // max(0, -(e-p))
  const unsigned int shiftR = maximum<int>(base2Exponent - significant, 0); // max(e-p, 0)

  // TAG: in debug mode use worst case size
  // number of words in large integers
  unsigned int integerSize = (maximum<int>(maximum<int>(shiftS, shiftR), maximum<int>(base2Exponent, significant)) /* value bits */
                                        + 2 /* additional shifts */
                                        + 4 /* multiplication with 10, addition with 10, sum of integers */
                                        + sizeof(unsigned int)*8 - 1 /* round up */
                                       )/(sizeof(unsigned int)*8);
  ASSERT((integerSize > 0) && (integerSize <= 513));

  // allocate integers on stack (potentially 10kb)
  unsigned int S[integerSize];
  unsigned int R[integerSize];
  unsigned int Mminus[integerSize];
  unsigned int Mdouble[integerSize]; // 2 * M- (only initialized if required)
  unsigned int temp[integerSize];

  LargeInteger::clear(R, integerSize);
  LargeInteger::assign(R, mantissa, mantissaSize);

  const bool unequalGap = ((nonZero - mantissa) == mantissaSize) &&
    (mantissa[mantissaSize - 1] == (1 << ((significant - 1) % (sizeof(unsigned int) * 8))));
  if (unequalGap) { // f == 1 << (p-1) // TAG: does this work for denormalized values
    LargeInteger::setBit(S, integerSize, shiftS + 2); // S = 2*S_paper
    LargeInteger::leftShift(R, integerSize, shiftR + 2); // R = 2*R_paper
  } else {
    LargeInteger::setBit(S, integerSize, shiftS + 1); // S = 2*S_paper
  LargeInteger::assign(temp, R, integerSize);
    LargeInteger::leftShift(R, integerSize, shiftR + 1); // R = 2*R_paper
  }
  LargeInteger::setBit(Mminus, integerSize, shiftR); // Mminus = M-


  LargeInteger::assign(temp, S, integerSize);
  LargeInteger::add(temp, integerSize, 10 - 1);
  LargeInteger::divide(temp, integerSize, 10); // ceil(S/B)
  while (LargeInteger::lessThan(R, temp, integerSize)) { // R < ceil(S/B) => R < (S+B-1)/B
    LargeInteger::multiply(R, integerSize, 10); // R = R * B // TAG: optimize
    --exponent;
  }

  int alpha = -exponent;
  while (alpha > 0) {
    unsigned int n = minimum<int>(alpha, 9);
    alpha -= n;
    LargeInteger::multiply(Mminus, integerSize, power[n]); // M- = M- * B^n
  }

  unsigned int* Mplus = Mminus;
  if (unequalGap) { // f == 1 << (p-1) // TAG: does this work for denormalized values
    Mplus = Mdouble; // redirect to separate storage
    LargeInteger::assign(Mplus, Mminus, integerSize);
    LargeInteger::leftShift(Mplus, integerSize, 1); // Mplus = M+
  }

  int cutPlace;

  bool adjusted = false;
  while (true) {
    LargeInteger::assign(temp, R, integerSize);
    LargeInteger::add(temp, Mplus, integerSize);
    while (!LargeInteger::lessThan(temp, S, integerSize)) { // 2*R + M+ >= 2*S
      adjusted = false;
      LargeInteger::multiply(S, integerSize, 10); // S = S * B // TAG: need optimization
      ++exponent;
    }

    if (adjusted) {
      break;
    }

    switch (cutMode) {
    case CUT_MODE_NOGARBAGE: // TAG: works with SCIENTIFIC and ENGINEERING but not FIXED
      cutPlace = (significant + 1)/3; // N = 2 + floor[significant/log2(10)] => N < 3 + significant/3
      break;
    case CUT_MODE_RELATIVE:
      {
        switch (realStyle) {
        case FormatOutputStream::Symbols::SCIENTIFIC:
          cutPlace = precision + 1; // one digit before radix
          break;
        case FormatOutputStream::Symbols::FIXED:
          cutPlace = precision + exponent;
          if (cutPlace < 0) {
            return; // empty buffer
          }
          break;
        case FormatOutputStream::Symbols::ENGINEERING: // 1-3 digits before radix character
          if (exponent >= 0) {
            cutPlace = precision + exponent % 3; // round towards -infinity
          } else {
            cutPlace = precision + (3 - -exponent % 3) % 3; // round towards -infinity
          }
          break;
        }

        LargeInteger::assign(temp, S, integerSize); // y = S
        int alpha = -cutPlace;
        if (alpha > 0) {
          while (alpha > 0) {
            unsigned int n = minimum<int>(alpha, 9);
            alpha -= n;
            LargeInteger::multiply(temp, integerSize, power[n]); // y = y * B^n
          }
        } else {
          while (alpha < 0) {
            unsigned int n = minimum<int>(-alpha, 9);
            alpha += n;
            LargeInteger::add(temp, integerSize, power[n] - 1);
            LargeInteger::divide(temp, integerSize, power[n]); // y = ceil(y/B^n)
          }
        }

        if (LargeInteger::lessThan(Mminus, temp, integerSize)) {
          LargeInteger::assign(Mminus, temp, integerSize); // M- = max(y, M-)
        }
        if (unequalGap) { // check if M- and M+ differ
          if (LargeInteger::lessThan(Mplus, temp, integerSize)) {
            LargeInteger::assign(Mplus, temp, integerSize); // M+ = max(y, M+)
          } else if (LargeInteger::equal(Mplus, temp, integerSize)) {
            roundUp = true;
          }
        } else {
          if (LargeInteger::equal(Mplus, temp, integerSize)) {
            roundUp = true;
          }
        }
        break;
      }
    }
    adjusted = true;
  }

  bool low = false;
  bool high = false;
  while (!low && !high) { // assert: R < S
    LargeInteger::multiply(R, integerSize, 10); // R = R * B
    LargeInteger::multiply(Mminus, integerSize, 10); // M- = M- * B
    if (unequalGap) { // only if M- and M+ differ
      LargeInteger::multiply(Mplus, integerSize, 10); // M+ = M+ * B
    }

    LargeInteger::divide(temp, R, R, S, integerSize); // U = R/S and R=R%S
    if (numberOfDigits < cutPlace) {
      buffer[numberOfDigits++] = temp[0]; // assert: U < B
    }

    low = LargeInteger::lessThan(R, Mminus, integerSize); // 2*R < M-
    LargeInteger::assign(temp, R, integerSize); // TAG: need alternative
    LargeInteger::add(temp, Mplus, integerSize); // temp = 2*R + M+
    if (roundUp) {
      high = !LargeInteger::lessThan(temp, S, integerSize); // 2*R >= 2*S - M+ <=> 2*R + M+ >= 2*S
    } else {
      high = LargeInteger::lessThan(S, temp, integerSize); // 2*R > 2*S - M+ <=> 2*R + M+ > 2*S
    }
    if (numberOfDigits >= cutPlace) {
      break;
    }
  }

  if (low && !high) {
  } else if (high && !low) {
    ++buffer[numberOfDigits - 1];
  } else {
    LargeInteger::leftShift(R, integerSize, 1); // R = 2*R // TAG: need ordinary compare
    if (LargeInteger::lessThan(S, R, integerSize)) { // 2*R > S
      ++buffer[numberOfDigits - 1];
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
  unsigned int mantissa[(FloatRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8)];
  int exponent;
  unsigned int flags;
  analyseFloatingPoint(representation.fields, precision, mantissa, exponent, flags);
  stream.writeFloatingPointType(precision, mantissa, (FloatRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8), exponent, flags);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, double value) throw(IOException) {
  union {
    double primitive;
    DoubleRepresentation fields;
  } representation;
  representation.primitive = value;

  unsigned int precision;
  unsigned int mantissa[(DoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8)];
  int exponent;
  unsigned int flags;
  analyseFloatingPoint(representation.fields, precision, mantissa, exponent, flags);
  stream.writeFloatingPointType(precision, mantissa, (DoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8), exponent, flags);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, long double value) throw(IOException) {
  union {
    long double primitive;
    LongDoubleRepresentation fields;
  } representation;
  representation.primitive = value;

  unsigned int precision;
  unsigned int mantissa[(LongDoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8)];
  int exponent;
  unsigned int flags;
  analyseFloatingPoint(representation.fields, precision, mantissa, exponent, flags);
  stream.writeFloatingPointType(precision, mantissa, (LongDoubleRepresentation::SIGNIFICANT + (sizeof(unsigned int) * 8) - 1)/(sizeof(unsigned int) * 8), exponent, flags);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const void* value) throw(IOException) {
  return stream << HEX << PREFIX << ZEROPAD << reinterpret_cast<unsigned long>(value);
}

void FormatOutputStream::writeFloatingPointType(unsigned int significant, unsigned int* mantissa, unsigned int mantissaSize, int base2Exponent, unsigned int valueFlags) throw(IOException) {
  char buffer[128 + 2 + significant/3]; // N = 2 + floor[n/log2(10)] => N < 2 + n/3 // TAG: 128 should be calculated
  char* output = buffer;
  const char* radix = 0;
  unsigned int flags = context.flags;

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

      byte digitBuffer[(significant + 1)/3]; // N = 2 + floor[n/log2(10)] => N < 3 + n/3 // TAG: check if stack is aligned
      unsigned int numberOfDigits;
      int exponent;
      CutMode cutMode;
      if ((flags & Symbols::NECESSARY) != 0) {
        cutMode = CUT_MODE_NOGARBAGE;
      } else {
        cutMode = CUT_MODE_RELATIVE;
      }

      convertFloatingPoint(significant, context.precision, cutMode, context.realStyle, mantissa, mantissaSize, base2Exponent, digitBuffer, numberOfDigits, exponent);
      bool showExponent = true;

      int adjustedExponent = exponent;
      switch (context.realStyle) {
      case Symbols::SCIENTIFIC: // one digit before radix character
        if ((valueFlags & FP_ZERO) == 0) {
          adjustedExponent = exponent - 1;
        }
        break;
      case Symbols::FIXED:
        if ((flags & Symbols::NECESSARY) != 0) { // NOGARBAGE - precision is not valid
          if ((exponent >= -3) && (exponent < 10)) {
            adjustedExponent = 0;
            showExponent = false;
          } else {
            adjustedExponent = exponent - 1;
          }
        } else if (exponent <= maximum<int>(context.precision, numberOfDigits)) { // TAG: need number of significant digits (base 10)
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

      const byte* digit = digitBuffer;
      const byte* endDigit = digit + numberOfDigits;
      int digitsBeforeRadix = ((valueFlags & FP_ZERO) == 0) ? maximum(exponent - adjustedExponent, 0) : 1;
      int denormalizingZeros = minimum(maximum(adjustedExponent - exponent, 0), context.precision);

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
        if (totalDigitsAfterRadix < context.precision) {
          trailingZeros += context.precision - totalDigitsAfterRadix;
          totalDigitsAfterRadix += trailingZeros;
        }
      }

      if ((totalDigitsAfterRadix > 0) || (context.justification == Symbols::RADIX)) {
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
    ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
    unsigned int length = (output - buffer);
    if (context.width <= length) {
      write(buffer, length); // write characters
    } else {
      unsigned int invertedLength = context.width - length;

      Symbols::Justification justification = context.justification;
      if (context.justification == Symbols::DEPENDENT) {
        justification = Symbols::RIGHT; // TAG: is this locale specific
      }

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
        if (context.radixPosition >= beforeRadix) {
          prefixLength = minimum(context.radixPosition - beforeRadix, invertedLength); // right justify if radix position is too big
          unfoldValue(' ', prefixLength);
        }
        write(buffer, length); // write characters
        unfoldValue(' ', invertedLength - prefixLength);
        break;
      }
    }
    context = defaultContext;
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
