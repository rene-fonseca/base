/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/Trace.h>
#include <string.h>
#include <stdio.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptorOutputStream standardOutputStream(FileDescriptor::getStandardOutput());
FormatOutputStream fout(standardOutputStream);

FileDescriptorOutputStream standardErrorStream(FileDescriptor::getStandardError());
FormatOutputStream ferr(standardErrorStream);

FormatOutputStream::FormatOutputStream(OutputStream& out, unsigned int size) throw(BindException) :
  BufferedOutputStream(out, size) {

  defaultBase = DEC;
  defaultWidth = 0;
  defaultFlags = PREFIX;
  prepareForField();
}

FormatOutputStream& FormatOutputStream::operator<<(Action action) throw(IOException) {
  SynchronizeExclusively();
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
  case ENDL:
    {
      char ch = '\n';
      write(&ch, sizeof(ch)); // may throw IOException
    }
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
  SynchronizeExclusively();
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
  static const unsigned int prefixLength[] = {sizeof("0b")-1, sizeof("0")-1, sizeof("")-1, sizeof("0x")-1};
  static const unsigned int replicates = 32;
  static const char white[replicates + sizeof('\0')] = "                                ";
  static const char str0[replicates + sizeof('\0')] = "00000000000000000000000000000000";
  static const char str1[replicates + sizeof('\0')] = "11111111111111111111111111111111";
  static const char strf[replicates + sizeof('\0')] = "ffffffffffffffffffffffffffffffff";
  static const char* signedPad[] = {str1, str0, str0, strf};

  SynchronizeExclusively();
  unsigned int requiredWidth = size;

  if (isSigned) {
    ++requiredWidth; // "-"
  }

  if (flags | PREFIX) {
    requiredWidth += prefixLength[base];
  }

  unsigned int pads = (requiredWidth >= width) ? 0 : (width - requiredWidth);

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

  if (isSigned) {
    write("-", 1); // write sign
  }

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
  SynchronizeExclusively();
  prepareForField();
}

FormatOutputStream::~FormatOutputStream() throw(IOException) {
  TRACE_MEMBER();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, bool value) {
  return stream << (value ? "true" : "false");
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
  case BIN:
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
  case OCT:
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
  case DEC:
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
  case HEX:
    {
      unsigned short int temp = (unsigned short int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f); // get bits of digit
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
  case BIN:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case OCT:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case DEC:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case HEX:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f); // get bits of digit
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
  case BIN:
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
  case OCT:
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
  case DEC:
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
  case HEX:
    {
      unsigned int temp = (unsigned int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f); // get bits of digit
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
  case BIN:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case OCT:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case DEC:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case HEX:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f); // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, long value) {
  char buffer[sizeof(long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case BIN:
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
  case OCT:
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
  case DEC:
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
  case HEX:
    {
      unsigned long temp = (unsigned long)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f); // get bits of digit
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

FormatOutputStream& operator<<(FormatOutputStream& stream, unsigned long value) {
  char buffer[sizeof(unsigned long) * 8];
  char* dest = &buffer[sizeof(buffer) - 1]; // point to least significant digit position

  switch (stream.getBase()) {
  case BIN:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case OCT:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case DEC:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case HEX:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f); // get bits of digit
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
  case BIN:
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
  case OCT:
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
  case DEC:
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
  case HEX:
    {
      unsigned long long int temp = (unsigned long long int)value; // no sign
      do {
        *dest = ASCIITraits::valueToDigit(temp & 0x0000000f); // get bits of digit
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
  case BIN:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000001); // get digit
        value >>= 1; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case OCT:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x00000007); // get digit
        value >>= 3; // bits per digit
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case DEC:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value % 10); // get digit
        value /= 10;
        --dest;
      } while(value > 0);
      ++dest; // go to first valid char in buffer
      break;
    }
  case HEX:
    {
      do {
        *dest = ASCIITraits::valueToDigit(value & 0x0000000f); // get bits of digit
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
  char* buffer = Thread::getLocalStorage()->getElements();
  sprintf(buffer, "%f", value);
  return stream << buffer;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, double value) {
  char* buffer = Thread::getLocalStorage()->getElements();
  sprintf(buffer, "%f", value);
  return stream << buffer;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, long double value) {
  char* buffer = Thread::getLocalStorage()->getElements();
  sprintf(buffer, "%Lf", value);
  return stream << buffer;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
