/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/io/FileDescriptorInputStream.h>
#include <base/Trace.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptorInputStream standardInputStream(FileDescriptor::getStandardInput());
FormatInputStream fin(standardInputStream);

FormatInputStream::FormatInputStream(InputStream& in) throw(BindException) :
  FilterInputStream(in),
  buffer(WINDOW_SIZE),
  head(buffer.getBeginIterator()),
  tail(head) {
}

bool FormatInputStream::overwriteFromSource() throw(IOException) {
  FilterInputStream::wait(); // blocking wait for more input
  unsigned int available = FilterInputStream::available();
  if (available == 0) { // TAG: what about eof
    return false;
  }
  unsigned int size = minimum<unsigned int>(WINDOW_SIZE, available);
  head = buffer.getBeginIterator();
  tail = head;
  FilterInputStream::read(head.getValue(), size); // will not block
  head += size;
  return true;
}

bool FormatInputStream::appendFromSource() throw(InvalidFormat, IOException) {
  FilterInputStream::wait(); // blocking wait for more input
  unsigned int available = FilterInputStream::available();
  if (available == 0) { // TAG: what about eof
    return false;
  }
  unsigned int size = minimum<unsigned int>(buffer.getEndReadIterator() - head, available);
  assert(size > 0, InvalidFormat("Window size exceeded"));
  FilterInputStream::read(head.getValue(), size); // will not block
  head += size;
  return true;
}

void FormatInputStream::appendToString(String& result, ReadIterator end) throw(InvalidFormat) {
  assert(result.getLength() + (end - tail) <= MAXIMUM_LINE_SIZE, InvalidFormat("Maximum length of line exceeded"));
  for (; tail < end; ++tail) { // TAG: need alternative
    result += *tail;
  }
}

String FormatInputStream::getWord() throw(IOException) {
  // TAG: slow implementation - better approach is iterators
  String result;
  char ch;
  do { // skip spaces
    if (tail == head) {
      assert(overwriteFromSource(), EndOfFile());
    }
    ch = *tail++;
  } while (String::Traits::isSpace(ch));
  while (!String::Traits::isSpace(ch)) {
    result += ch;
    if (tail == head) {
      if (overwriteFromSource()) {
        break;
      }
    }
    ch = *tail++;
  }
  return result;
}

String FormatInputStream::getLine() throw(IOException) {
  // TAG: better approach is iterators
  String result;
  while (true) { // find new line (i.e. any of the substrings "\n", "\r", "\n\r", or "\r\n")
    ReadIterator i = tail;
    for (; i < head; ++i) {
      char ch = *i;
      if (ch == '\n') {
        appendToString(result, i);
        ++tail;
        if ((tail == head) && !overwriteFromSource()) {
          return result; // eof reached
        }
        if (*tail == '\r') {
          ++tail;
        }
        return result;
      } else if (ch == '\r') {
        appendToString(result, i);
        ++tail;
        if ((tail == head) && !overwriteFromSource()) {
          return result; // eof reached
        }
        if (*tail == '\n') {
          ++tail;
        }
        return result;
      }
    }
    appendToString(result, i);
    if (!overwriteFromSource()) {
      return result; // eof reached
    }
  }
}

unsigned int FormatInputStream::read(char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  unsigned int bytesRead = 0; // number of bytes that have been copied into external buffer
  while (true) {
    // copy from internal to external buffer - no overlap
    unsigned int bytesToCopy = minimum<unsigned int>(size, head - tail);
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

FormatInputStream::~FormatInputStream() throw(IOException) {
  TRACE_MEMBER();
}



FormatInputStream& operator>>(FormatInputStream& stream, char& value) throw(IOException) {
  value = stream.getCharacter();
  return stream;
}

FormatInputStream& operator>>(FormatInputStream& stream, String& value) throw(IOException) {
  value = stream.getLine();
  return stream;
}

FormatInputStream& operator>>(FormatInputStream& stream, unsigned int& value) throw(InvalidFormat, IOException) {
  value = 0;
  char ch;
  do {
    ch = stream.getCharacter();
  } while (String::Traits::isSpace(ch));

  do {
    assert(ASCIITraits::isDigit(ch), InvalidFormat("Not an unsigned int"));
    unsigned char temp = ASCIITraits::digitToValue(ch);
    if ((value < UnsignedInt::MAXIMUM/10) || ((value == UnsignedInt::MAXIMUM/10) && (temp <= UnsignedInt::MAXIMUM%10))) {
      value = ch + 10 * value;
      try {
        ch = stream.getCharacter(); // TAG: must accept eof
      } catch (EndOfFile& e) {
        break;
      }
      continue;
    }
  } while (!String::Traits::isSpace(ch));
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
