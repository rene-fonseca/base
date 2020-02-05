/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/data/CSVFormat.h>
#include <base/string/StringOutputStream.h>
#include <base/io/FileInputStream.h>
#include <base/io/EndOfFile.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class BufferedInputStream {
private:

  InputStream* is = nullptr;
  Allocator<uint8> buffer;
  const uint8* end = nullptr;
  bool eof = false;
public:

  const uint8* src = nullptr;
public:

  BufferedInputStream() noexcept
  {
  }

  BufferedInputStream(InputStream* _is) noexcept
    : is(_is)
  {
    buffer.setSize(16 * 1024);
  }

  /** Returns true if eof has been reached. */
  inline bool atEof() const noexcept
  {
    return eof;
  }

  /** Reads data to buffer. */
  void read()
  {
    unsigned int bytesRead = 0;
    if (!eof) {
      try {
        bytesRead = is->read(buffer.getElements(), buffer.getSize(), true);
      } catch (EndOfFile&) {
        eof = true;
      }
    }
    if (!eof) {
      src = buffer.getElements();
      end = src + bytesRead;
    } else {
      src = nullptr;
      end = nullptr;
    }
  }

  /** Returns true if buffer has available bytes. */
  inline bool hasBytes() const noexcept
  {
    return src != end;
  }

  /** Returns the available bytes. */
  inline MemorySpan getBytes() noexcept
  {
    return MemorySpan(src, end);
  }

  /** Skips the given number of bytes. */
  void skip(MemorySize size)
  {
    BASSERT((src + size) <= end);
    src += size;
  }
};

/** Line reader. */
class LineReader {
private:

  BufferedInputStream bis;
  char skipCode = 0;
public:

  // TAG: add support for encodings
  // TAG: repair UTF8 support

  LineReader() noexcept
  {
  }

  LineReader(InputStream* is) noexcept
    : bis(is)
  {
  }

  /** Returns true if eof has not been reached. */
  inline bool hasMore() const noexcept
  {
    return !bis.atEof() || bis.hasBytes();
  }

  /** Reads the next line. */
  String readLine();

  /** Reads all lines. */
  Array<String> readLines();
};

String LineReader::readLine()
{
  if (bis.atEof() && !bis.hasBytes()) {
    _throw EndOfFile("No more lines.");
  }
  String result;
  while (!bis.atEof() || bis.hasBytes()) {

    if (!bis.hasBytes()) {
      bis.read();
      if (!bis.hasBytes()) {
        _throw EndOfFile("No more lines.");
      }
    }

    const uint8* src = bis.getBytes().begin;
    const uint8* end = bis.getBytes().end;

    // handle partial eol
    if (skipCode) {
      if (*src == skipCode) {
        skipCode = 0;
        ++src;
      }
    }

    // look for eol
    const uint8* begin = src;
    while (src != end) {
      if ((*src == '\n') || (*src == '\r')) {
        const uint8 otherCode = (*src == '\n') ? '\r' : '\n';
        ++src;
        if (src != end) {
          if (*src == otherCode) {
            ++src;
          }
        } else {
          skipCode = otherCode;
        }

        bis.skip(src - begin);
        if (!result) {
          return String(reinterpret_cast<const char*>(begin), src - begin); // common case
        } else {
          result.append(MemorySpan(begin, src));
          return result;
        }
      }
      ++src;
    }

    result.append(MemorySpan(begin, src));
    bis.skip(src - begin);
  }
  return result;
}

Array<String> LineReader::readLines()
{
  Array<String> lines;
  while (!hasMore()) {
    lines.append(readLine());
  }
  return lines;
}

String CSVFormat::quote(const String& text)
{
  String result(text.getLength() + 2);
  result += '\"';
  for (auto ch : text) {
    if (ch == '"') {
      result += '\\';
    }
    result += ch;
  }
  result += '\"';
  return result;
}

CSVFormat::CSVFormat(ucs4 _separator) noexcept
  : separator(_separator)
{
}

String CSVFormat::join(const Array<String>& items)
{
  MemorySize capacity = 0;
  for (const auto& item : items) {
    capacity += item.getLength();
    capacity += 1 + 2;
  }
  String result(capacity);
  bool first = true;
  for (const auto& item : items) {
    if (!first) {
      result += separator;
    }
    first = false;
    // TAG: only quote if required
    result += quote(item);
  }
  return result;
}

String CSVFormat::join(const Array<AnyValue>& items)
{
  StringOutputStream stream;
  bool first = true;
  for (const auto& item : items) {
    // TAG: only quote if required
    if (!first) {
      stream << separator;
    }
    first = false;
    switch (item.getRepresentation()) {
    case AnyValue::CHARACTER:
    case AnyValue::WIDE_CHARACTER:
    case AnyValue::STRING:
    case AnyValue::WIDE_STRING:
      stream << quote(item.getString());
      break;
    default:
      stream << item.getString();
    }
  }
  return stream;
}

String CSVFormat::join(const std::initializer_list<String>& items)
{
  MemorySize capacity = 0;
  for (const auto& item : items) {
    capacity += item.getLength();
    capacity += 1 + 2;
  }
  String result(capacity);
  bool first = true;
  for (const auto& item : items) {
    // TAG: only quote if required
    if (!first) {
      result += separator;
    }
    first = false;
    result += quote(item);
  }
  return result;
}

Array<String> CSVFormat::parse(const String& line)
{
  Array<String> result;
  String field(1024);
  UTF8Iterator i = line.getUTF8BeginReadIterator();
  const UTF8Iterator end = line.getUTF8EndReadIterator();
  bool inQuote = false;
  while (i < end) {
    const ucs4 ch = *i++;

    if (!inQuote && (ch == ' ')) { // trim spaces
      continue;
    }
    
    if (ch == '\\') { // escape
      if (inQuote) {
        if (!(i < end)) {
          _throw InvalidFormat("Invalid CSV format.");
        }
        const ucs4 ch = *i++;
        if (ch == '"') {
          field.append(ch);
        } else {
          _throw InvalidFormat("Invalid CSV format.");
        }
      } else {
        field.append(ch);
      }
    } else if (ch == '"') {
      if (inQuote) {
        inQuote = false;

        // trim spaces
        while (i < end) {
          const ucs4 ch = *i;
          if (ch == ' ') {
            continue;
          }
          ++i;
        }
        if (!(i < end)) {
          result.append(field.copy());
          field.forceToLength(0);
          return result;
        }

        if (*i++ != separator) {
          _throw InvalidFormat("Expected separator.");
        }
        result.append(field.copy());
        field.forceToLength(0);
      } else {
        inQuote = true;
      }
    } else if (ch == separator) {
      result.append(field.copy());
      field.forceToLength(0);
    } else {
      field.append(ch);
    }
  }

    result.append(field.copy());
    field.forceToLength(0);

  return result;
}

Array<Array<String> > CSVFormat::load(InputStream* is)
{
  Array<Array<String> > result;
  LineReader reader(is);
  while (!reader.hasMore()) {
    if (String line = reader.readLine()) {
      result.append(parse(line));
    }
  }
  return result;
}

Array<Array<String> > CSVFormat::load(const String& path)
{
  FileInputStream fis(path);
  return load(&fis);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
