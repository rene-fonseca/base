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
#include <base/string/LineReader.h>
#include <base/io/FileInputStream.h>
#include <base/io/EndOfFile.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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

void CSVFormat::parse(const String& line, Array<String>& result)
{
  result.setSize(0);
  
  String field = String::makeCapacity(1024);
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
          return;
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

  return;
}

Array<Array<String> > CSVFormat::load(InputStream* is)
{
  Array<Array<String> > result;
  LineReader reader(is);
  while (!reader.hasMore()) {
    if (String line = reader.readLine()) {
      Array<String> fields;
      parse(line, fields);
      result.append(fields);
    }
  }
  return result;
}

Array<Array<String> > CSVFormat::load(const String& path)
{
  FileInputStream fis(path);
  return load(&fis);
}

void CSVFormat::load(InputStream* is, LineConsumer* consumer)
{
  Array<String> fields;
  fields.ensureCapacity(64);
  LineReader reader(is);
  while (!reader.hasMore()) {
    if (String line = reader.readLine()) { // TAG: add support for reading to line buffer
      parse(line, fields);
      (*consumer)(fields);
    }
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
