/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FormatOutputStream.h"
#include <string.h>

const char FormatOutputStream::DIGITS[] = "0123456789abcdef";

FormatOutputStream::FormatOutputStream(OutputStream& out, unsigned int size) throw(BindException) :
  BufferedOutputStream(out, size) {
  throw BindException();

  defaultBase = DEC;
  defaultWidth = 0;
  defaultFlags = PREFIX;
  prepareForField();
}

FormatOutputStream& FormatOutputStream::operator<<(Action action) throw(IOException) {
  switch (action) {
  case BIN:
    base = BIN;
    break;
  case OCT:
    base = OCT;
    break;
  case DEC:
    base = DEC;
    break;
  case HEX:
    base = HEX;
    break;
  case ZEROPAD:
    flags |= ZEROPAD;
    break;
  case NOZEROPAD:
    flags &= ~ZEROPAD;
    break;
  case PREFIX:
    flags |= PREFIX;
    break;
  case NOPREFIX:
    flags &= ~PREFIX;
    break;
  case EOL:
    {
      char ch = '\n';
      write(&ch, sizeof(ch)); // may throw IOException
    }
    break;
  case FLUSH:
    flush(); // may throw IOException
    break;
  }
  return *this;
}

void FormatOutputStream::prepareForField() {
  flags = defaultFlags;
  width = defaultWidth;
  base = defaultBase;
}

void FormatOutputStream::addCharacterField(const char* buf, unsigned int size) throw(IOException) {
  static const char* white = "                ";

  if (size < width) { // write blanks if required
    unsigned int pads = width - size;
    while (true) {
      if (pads > sizeof(white)) {
        write(white, sizeof(white));
        pads -= sizeof(white);
      } else {
        write(white, pads);
        break;
      }
    }
  }

  write(buf, size); // write characters
  prepareForField();
}

void FormatOutputStream::addIntegerField(const char* buf, unsigned int size, bool isSigned) throw(IOException) {
  static const char* prefixString[] = {"0b", "0", "", "0x"};
  static const unsigned int prefixLength[] = {sizeof("0b"), sizeof("0"), sizeof(""), sizeof("0x")};
  static const unsigned int replicates = 32;
  static const char white[replicates + sizeof('\0')] = "                                ";
  static const char str0[replicates + sizeof('\0')] = "00000000000000000000000000000000";
  static const char str1[replicates + sizeof('\0')] = "11111111111111111111111111111111";
  static const char strf[replicates + sizeof('\0')] = "ffffffffffffffffffffffffffffffff";
  static const char* signedPad[] = {str1, str0, str0, strf};

  unsigned int requiredWidth = size;

  if (isSigned) {
    ++requiredWidth; // "-"
  }

  if (flags | PREFIX) {
    requiredWidth += prefixLength[base];
  }

  unsigned int pads = (requiredWidth <= width) ? 0 : (width - requiredWidth);

  if ((pads > 0) && !(flags & ZEROPAD)) { // write blanks if required
    while (true) {
      if (pads > sizeof(white)) {
        write(white, sizeof(white));
        pads -= sizeof(white);
      } else {
        write(white, pads);
        break;
      }
    }
  }

  write("-", 1); // write sign

  if (flags | PREFIX) { // is prefix requested
    write(prefixString[base], prefixLength[base]);
  }

  if ((pads > 0) && (flags | ZEROPAD)) { // do we need to extend field with digits
    const char* digit = isSigned ? signedPad[base] : str0; // determine pad digit
    while (true) {
      if (pads > 32) {
        write(digit, 32);
        pads -= 32;
      } else {
        write(digit, pads);
        break;
      }
    }
  }

  write(buf, size); // write late buffer
  prepareForField();
}

void FormatOutputStream::addDoubleField(const char* early, unsigned int earlySize, const char* late, unsigned int lateSize, bool isSigned) throw(IOException) {
  prepareForField();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, char value) {
  stream.addCharacterField(&value, 1);
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const char* value) {
  if (value != NULL) {
    unsigned int length = strlen(value);
    stream.addCharacterField(value, length);
  }
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, short int value) {
  char buffer[sizeof(short int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::BIN:
    {
      unsigned short int temp = (unsigned short int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x00000001]; // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::OCT:
    {
      unsigned short int temp = (unsigned short int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x00000007]; // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::DEC:
    {
      unsigned short int temp = (value >= 0) ? value : -value;
      do {
        *dest = FormatOutputStream::DIGITS[temp % 10]; // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::HEX:
    {
      unsigned short int temp = (unsigned short int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x0000000f]; // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned short int value) {
  char buffer[sizeof(unsigned short int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::BIN:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x00000001]; // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::OCT:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x00000007]; // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::DEC:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value % 10]; // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::HEX:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x0000000f]; // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, int value) {
  char buffer[sizeof(int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::BIN:
    {
      unsigned int temp = (unsigned int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x00000001]; // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::OCT:
    {
      unsigned int temp = (unsigned int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x00000007]; // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::DEC:
    {
      unsigned int temp = (value >= 0) ? value : -value;
      do {
        *dest = FormatOutputStream::DIGITS[temp % 10]; // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::HEX:
    {
      unsigned int temp = (unsigned int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x0000000f]; // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned int value) {
  char buffer[sizeof(unsigned int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::BIN:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x00000001]; // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::OCT:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x00000007]; // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::DEC:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value % 10]; // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::HEX:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x0000000f]; // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, long long int value) {
  char buffer[sizeof(long long int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::BIN:
    {
      unsigned long long int temp = (unsigned long long int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x00000001]; // get digit
        temp >>= 1; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::OCT:
    {
      unsigned long long int temp = (unsigned long long int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x00000007]; // get digit
        temp >>= 3; // bits per digit
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::DEC:
    {
      unsigned int temp = (value >= 0) ? value : -value;
      do {
        *dest = FormatOutputStream::DIGITS[temp % 10]; // get digit
        temp = temp/10;
        --dest;
      } while(temp > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::HEX:
    {
      unsigned long long int temp = (unsigned long long int)value; // no sign
      do {
        *dest = FormatOutputStream::DIGITS[temp & 0x0000000f]; // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long long int value) {
  char buffer[sizeof(unsigned long long int) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case FormatOutputStream::BIN:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x00000001]; // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::OCT:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x00000007]; // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::DEC:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value % 10]; // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case FormatOutputStream::HEX:
    {
      do {
        *dest = FormatOutputStream::DIGITS[value & 0x0000000f]; // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, float value) {
  return stream << "float to stream not implemented in FormatOutputStream.cpp";
}

FormatOutputStream& operator<<(FormatOutputStream& stream, double value) {
  return stream << "double to stream not implemented in FormatOutputStream.cpp";
}

FormatOutputStream& operator<<(FormatOutputStream& stream, long double value) {
  return stream << "long double to stream not implemented in FormatOutputStream.cpp";
}
