/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/io/FileDescriptorInputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FormatInputStream::FormatInputStream(InputStream& in)
  : FilterInputStream(in),
    buffer(WINDOW_SIZE),
    head(buffer.getBeginIterator()),
    tail(head) {
}

unsigned int FormatInputStream::available() const {
  return (unsigned int)(FilterInputStream::available() + (head - tail));
}

bool FormatInputStream::overwriteFromSource() {
  FilterInputStream::wait(); // blocking wait for more input (or eof)
  unsigned int available = FilterInputStream::available();
  if (available == 0) {
    return false;
  }
  unsigned int bytesToRead = minimum<unsigned int>(WINDOW_SIZE, available);
  head = buffer.getBeginIterator();
  tail = head;
  unsigned int bytesRead =
    FilterInputStream::read(head.getValue(), bytesToRead); // will not block
  BASSERT(bytesRead == bytesToRead);
  head += bytesRead;
  return true;
}

bool FormatInputStream::appendFromSource() {
  FilterInputStream::wait(); // blocking wait for more input (or eof)
  unsigned int available = FilterInputStream::available();
  if (available == 0) {
    return false;
  }
  unsigned int size = (unsigned int)
    minimum<MemorySize>(buffer.getEndReadIterator() - head, available);
  unsigned int bytesRead = FilterInputStream::read(head.getValue(), size); // will not block
  BASSERT(bytesRead == size);
  head += size;
  return true;
}

String FormatInputStream::getWord() {
  String result;
  while (true) { // skip prefix spaces
    for (; (tail < head) && (ASCIITraits::isSpace(*tail)); ++tail) {
    }
    if (tail < head) { // end of spaces
      break;
    }
    if (!overwriteFromSource()) {
      return result; // eof reached (empty word)
    }
  }
  
  while (true) {
    const ReadIterator beginning = tail; // beginning of substring
    for (; (tail < head) && (!ASCIITraits::isSpace(*tail)); ++tail) {
    }
    unsigned int length = static_cast<unsigned int>(tail - beginning);
    unsigned int offset = static_cast<unsigned int>(result.getLength());
    result.forceToLength(result.getLength() + length); // extend string
    copy<uint8>(
      Cast::pointer<uint8*>((result.getBeginIterator() + offset).getValue()),
      beginning.getValue(),
      length
    );
    
    if (tail < head) { // end of word
      break;
    }
    if (!overwriteFromSource()) {
      return result; // eof reached
    }
  }
  return result;
}

// TAG: need limit arg: unsigned int maximumLength
String FormatInputStream::getLine() {
  line.forceToLength(0);
  while (true) { // find new line (i.e. any of the substrings "\n", "\r", "\n\r", or "\r\n")
    const ReadIterator beginning = tail; // beginning of substring
    for (; tail < head; ++tail) {
      char ch = *tail;
      if (ch == '\n') {
        unsigned int length = static_cast<unsigned int>(tail - beginning);
        unsigned int offset = static_cast<unsigned int>(line.getLength());
        line.forceToLength(line.getLength() + length); // extend string
        copy<uint8>(
          Cast::pointer<uint8*>((line.getBeginIterator() + offset).getValue()),
          beginning.getValue(),
          length
        );
        ++tail; // skip '\n'
        if ((tail == head) && !overwriteFromSource()) {
          return line; // eof reached
        }
        if (*tail == '\r') {
          ++tail; // skip '\r'
        }
        return line;
      } else if (ch == '\r') {
        unsigned int length = static_cast<unsigned int>(tail - beginning);
        unsigned int offset = static_cast<unsigned int>(line.getLength());
        line.forceToLength(line.getLength() + length); // extend string
        copy<uint8>(
          Cast::pointer<uint8*>((line.getBeginIterator() + offset).getValue()),
          beginning.getValue(),
          length
        );
        ++tail; // skip '\r'
        if ((tail == head) && !overwriteFromSource()) {
          return line; // eof reached
        }
        if (*tail == '\n') {
          ++tail; // skip '\n'
        }
        return line;
      }
    }
    unsigned int length = static_cast<unsigned int>(tail - beginning);
    unsigned int offset = static_cast<unsigned int>(line.getLength());
    line.forceToLength(line.getLength() + length); // extend string
    copy<uint8>(
      Cast::pointer<uint8*>((line.getBeginIterator() + offset).getValue()),
      beginning.getValue(),
      length
    );
    if (!overwriteFromSource()) {
      return line; // eof reached
    }
  }
}

unsigned int FormatInputStream::read(uint8* buffer,
                                     unsigned int size,
                                     bool nonblocking)
{
  unsigned int bytesRead = 0; // number of bytes that have been copied into external buffer
  while (true) {
    // copy from internal to external buffer - no overlap
    unsigned int bytesToCopy = minimum<unsigned int>(size, static_cast<unsigned int>(head - tail));
    copy(buffer + bytesRead, tail.getValue(), bytesToCopy);
    tail += bytesToCopy;
    bytesRead += bytesToCopy;
    size -= bytesToCopy;
    if (size == 0) { // have we read all the bytes we were asked to read
      break;
    }
    if (!overwriteFromSource()) { // refill buffer
      break; // eof has been reached
    }
  }
  return bytesRead;
}

FormatInputStream::~FormatInputStream()
{
}



FormatInputStream& operator>>(
  FormatInputStream& stream, char& value) {
  value = stream.getCharacter();
  return stream;
}

FormatInputStream& operator>>(FormatInputStream& stream, String& value)
{
  value = stream.getLine();
  return stream;
}

FormatInputStream& operator>>(FormatInputStream& stream, unsigned int& value)
{
  value = 0;
  char ch = 0;
  do {
    ch = stream.getCharacter();
  } while (ASCIITraits::isSpace(ch));

  do {
    bassert(ASCIITraits::isDigit(ch), InvalidFormat("Not an unsigned int."));
    unsigned char temp = ASCIITraits::digitToValue(ch);
    if ((value < PrimitiveTraits<unsigned int>::MAXIMUM/10) ||
        ((value == PrimitiveTraits<unsigned int>::MAXIMUM/10) &&
         (temp <= PrimitiveTraits<unsigned int>::MAXIMUM%10))) {
      value = ch + 10 * value;
      try {
        ch = stream.getCharacter(); // TAG: must accept eof
      } catch (EndOfFile&) {
        break;
      }
      continue;
    }
  } while (!ASCIITraits::isSpace(ch));
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
